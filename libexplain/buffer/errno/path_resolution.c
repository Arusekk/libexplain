/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/assert.h>
#include <libexplain/ac/dirent.h>
#include <libexplain/ac/errno.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/uid.h>
#include <libexplain/capability.h>
#include <libexplain/fstrcmp.h>
#include <libexplain/have_permission.h>
#include <libexplain/option.h>
#include <libexplain/symloopmax.h>


static int
all_slash(const char *s)
{
    while (*s == '/')
        ++s;
    return !*s;
}


static void
look_for_similar(libexplain_string_buffer_t *sb, const char *lookup_directory,
    const char *component)
{
    DIR             *dp;
    char            best_name[NAME_MAX + 1];
    double          best_weight;

    dp = opendir(lookup_directory);
    if (!dp)
        return;

    best_name[0] = '\0';
    best_weight = 0.6;
    for (;;)
    {
        struct dirent   *dep;
        double          weight;

        dep = readdir(dp);
        if (!dep)
            break;
        if (0 == strcmp(dep->d_name, "."))
            continue;
        if (0 == strcmp(dep->d_name, ".."))
            continue;
        weight = libexplain_fstrcasecmp(component, dep->d_name);
        if (best_weight < weight)
        {
            best_weight = weight;
            strendcpy(best_name, dep->d_name, best_name + sizeof(best_name));
        }
    }
    closedir(dp);

    if (best_name[0] == '\0')
        return;

    libexplain_string_buffer_puts(sb, ", did you mean the ");
    /* the rule is: [caption] path file-type */
    libexplain_string_buffer_puts_quoted(sb, best_name);
    libexplain_string_buffer_putc(sb, ' ');

    {
        /* see if we can say what kind of file it is */
        struct stat st;
        char ipath[PATH_MAX + 1];
        char *ipath_end = ipath + sizeof(ipath);
        char *ip = ipath;
        ip = strendcpy(ip, lookup_directory, ipath_end);
        ip = strendcpy(ip, "/", ipath_end);
        ip = strendcpy(ip, best_name, ipath_end);
        if (lstat(ipath, &st) == 0)
            libexplain_buffer_file_type(sb, st.st_mode);
        else
            libexplain_string_buffer_puts(sb, "file");
    }

    libexplain_string_buffer_puts(sb, " instead?");
}


int
libexplain_buffer_errno_path_resolution(libexplain_string_buffer_t *sb,
    int expected_errno, const char *initial_pathname, const char *caption,
    const libexplain_final_t *final_component)
{
    char            pathname[PATH_MAX + 1];
    libexplain_string_buffer_t pathname_buf;
    int             number_of_symlinks_followed;
    const char      *pp;
    char            lookup_directory[PATH_MAX + 1];
    libexplain_string_buffer_t lookup_directory_buf;
    char            **symlinks_so_far;
    int             symloop_max;
    long            path_max;

    /*
     * It's easy to pass pathname twice, rather then pathname and
     * "pathname", so we do a little checking.  Add to this list of
     * something else turns up, but I don't expect any more.
     *
     * RESIST the temptation to add more, because it actually makes the
     * libexplain API inconsistent (at the expense of avoiding lame
     * POSIX argument name inconsitencies, hey).
     */
    assert(0 == strcmp(caption, "pathname") || 0 == strcmp(caption, "oldpath")
        || 0 == strcmp(caption, "newpath"));

    symlinks_so_far = 0;
    number_of_symlinks_followed = 0;

    /*
     * Empty pathname:
     * In the original Unix, the empty pathname referred to the current
     * directory.  Nowadays POSIX decrees that an empty pathname must
     * not be resolved successfully.  Linux returns ENOENT in this case.
     */
    if (!initial_pathname || !*initial_pathname)
    {
        libexplain_string_buffer_printf
        (
            sb,
            i18n("POSIX decrees that an empty %s must not be resolved "
                "successfully"),
            caption
        );
        return 0;
    }

    /*
     * Deal with some boundary conditions.
     * What we want is to always have at least one component.
     */
    if (!initial_pathname || !*initial_pathname)
        initial_pathname = ".";
    else if (all_slash(initial_pathname))
        initial_pathname = "/.";

    path_max = pathconf(initial_pathname, _PC_PATH_MAX);
    if (path_max <= 0)
        path_max = PATH_MAX;

    if (expected_errno == ENAMETOOLONG)
    {
        size_t          len;

        len = strlen(initial_pathname);
        if (len > (size_t)path_max)
        {
            libexplain_string_buffer_puts(sb, caption);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_buffer_gettext
            (
                sb,
                i18n("exceeds the system maximum path length")
            );
            libexplain_string_buffer_printf
            (
                sb,
                " (%ld > %ld)",
                (long)len,
                path_max
            );
            return 0;
        }
    }

    libexplain_string_buffer_init(&pathname_buf, pathname, sizeof(pathname));
    libexplain_string_buffer_puts(&pathname_buf, initial_pathname);

    /*
     * Trailing slashes
     * If a pathname ends in a '/', that forces resolution of the
     * preceding component as in Step 2: it has to exist and resolve
     * to a directory.  Otherwise a trailing '/' is ignored.  (Or,
     * equivalently, a pathname with a trailing '/' is equivalent to the
     * pathname obtained by appending '.' to it.)
     */
    if (pathname[pathname_buf.position - 1] == '/')
        libexplain_string_buffer_putc(&pathname_buf, '.');

    /*
     * Try to get the system's idea of the loop maximum.
     */
    symloop_max = libexplain_symloopmax();

    symlinks_so_far = malloc(2 * symloop_max * sizeof(char *));
    if
    (
        !symlinks_so_far
    &&
        (expected_errno == ELOOP || expected_errno == EMLINK)
    )
        return -1;

    pp = pathname;

    /*
     * Set the current lookup directory to the starting lookup directory.
     */
    libexplain_string_buffer_init
    (
        &lookup_directory_buf,
        lookup_directory,
        sizeof(lookup_directory)
    );
    libexplain_string_buffer_puts
    (
        &lookup_directory_buf,
        (*pp == '/' ? "/" : ".")
    );
    while (*pp == '/')
        ++pp;

    /*
     * Walk along the path
     */
    for (;;)
    {
        char            component[PATH_MAX + 1]; /* deliberately not NAME_MAX */
        libexplain_string_buffer_t component_buf;
        char            intermediate_path[PATH_MAX + 1];
        libexplain_string_buffer_t intermediate_path_buf;
        int             final;
        int             lookup_directory_writable;
        struct stat     lookup_directory_st;
        struct stat     intermediate_path_st;

        /*
         * Check that the lookup directory will play ball.
         */
        if (lstat(lookup_directory, &lookup_directory_st) != 0)
        {
            int lookup_directory_st_errnum = errno;
            if (lookup_directory_st_errnum == ENOENT)
            {
                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_puts_quoted(sb, lookup_directory);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, S_IFDIR);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_gettext(sb, i18n("does not exist"));
            }
            else
            {
                libexplain_string_buffer_puts(sb, "lstat(");
                libexplain_string_buffer_puts_quoted(sb, lookup_directory);
                libexplain_string_buffer_putc(sb, ')');
                libexplain_buffer_failed(sb, lookup_directory_st_errnum);
            }
            return_0:
            if (symlinks_so_far)
            {
                int             j;

                for (j = 0; j < number_of_symlinks_followed; ++j)
                    free(symlinks_so_far[j]);
                free(symlinks_so_far);
            }
            return 0;
        }

        /*
         * The lookup directory is never a symbolic link.
         * We take care to ensure it, see below.
         */
        assert(!S_ISLNK(lookup_directory_st.st_mode));

        if (!S_ISDIR(lookup_directory_st.st_mode))
        {
            /*
             * If the lookup_directory is found, but is not a directory,
             * an ENOTDIR error is returned ("Not a directory").
             */
            libexplain_string_buffer_puts(sb, "path ");
            /* the rule is: [caption] path file-type */
            libexplain_string_buffer_puts_quoted(sb, lookup_directory);
            libexplain_string_buffer_puts(sb, " is a ");
            libexplain_buffer_file_type(sb, lookup_directory_st.st_mode);
            libexplain_string_buffer_puts(sb, " and not a ");
            libexplain_buffer_file_type(sb, S_IFDIR);
            goto return_0;
        }

        /*
         * If the process does not have search permission on the current
         * lookup directory, an EACCES error is returned ("Permission
         * denied").
         */
        if (!libexplain_have_search_permission(&lookup_directory_st))
        {
            libexplain_buffer_gettext
            (
                sb,
                i18n("the process does not have search permission to the")
            );
            libexplain_string_buffer_putc(sb, ' ');
            /* the rule is: [caption] path file-type */
            libexplain_string_buffer_puts(sb, caption);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_string_buffer_puts_quoted(sb, lookup_directory);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_buffer_file_type(sb, S_IFDIR);

            /*
             * FIXME: may need to explain that the permissions are
             * exclusive, rather than inclusive as naive users expect.
             */
            goto return_0;
        }

        lookup_directory_writable =
            libexplain_have_write_permission(&lookup_directory_st);

        /*
         * Extract the next path component.
         * A path component is a substring delimited by '/' characters.
         */
        libexplain_string_buffer_init
        (
            &component_buf,
            component,
            sizeof(component)
        );
        for (;;)
        {
            unsigned char c = *pp++;
            switch (c)
            {
            default:
                libexplain_string_buffer_putc(&component_buf, c);
                continue;

            case '\0':
                --pp;
                break;

            case '/':
                break;
            }
            break;
        }
        while (*pp == '/')
            ++pp;
        final = !*pp;

        /*
         * Check the length of the component
         */
        if (expected_errno == ENAMETOOLONG)
        {
            long            no_trunc;
            int             silent_truncate;
            long            name_max;

            /*
             * Components can only be too long if the file system
             * they are on does not silently truncate over-long path
             * components.
             */
            no_trunc = pathconf(lookup_directory, _PC_NO_TRUNC);
            silent_truncate = (no_trunc == 0);

            name_max = pathconf(lookup_directory, _PC_NAME_MAX);
            if (name_max <= 0)
                name_max = NAME_MAX;
            if (!silent_truncate && component_buf.position > (size_t)name_max)
            {
                libexplain_string_buffer_puts(sb, caption);
                libexplain_string_buffer_puts(sb, " component ");
                libexplain_string_buffer_puts_quoted(sb, component);
                libexplain_string_buffer_puts
                (
                    sb,
                    " is longer than the system limit"
                );
                libexplain_string_buffer_printf
                (
                    sb,
                    " (%ld > %ld)",
                    (long)component_buf.position,
                    name_max
                );
                goto return_0;
            }
            if (component_buf.position > (size_t)name_max)
            {
                component_buf.position = name_max;
                component[name_max] = '\0';
            }
        }
        else
        {
            /*
             * Silently truncate over-long path components.
             */
            long name_max = pathconf(lookup_directory, _PC_NAME_MAX);
            if (name_max <= 0)
                name_max = NAME_MAX;
            if (component_buf.position > (size_t)name_max)
            {
                component_buf.position = name_max;
                component[name_max] = '\0';
            }
        }

        /*
         * Build the intermediate path by joining the lookup_directory
         * and the component.
         */
        libexplain_string_buffer_init
        (
            &intermediate_path_buf,
            intermediate_path,
            sizeof(intermediate_path)
        );
        libexplain_string_buffer_puts(&intermediate_path_buf, lookup_directory);
        libexplain_string_buffer_path_join(&intermediate_path_buf, component);

        if (lstat(intermediate_path, &intermediate_path_st) < 0)
        {
            int             intermediate_path_st_errnum;

            intermediate_path_st_errnum = errno;
            if (!final)
            {
                if (intermediate_path_st_errnum == ENOENT)
                {
                    libexplain_string_buffer_puts(sb, " there is no ");
                    /* the rule is: [caption] path file-type */
                    libexplain_string_buffer_puts_quoted(sb, component);
                    libexplain_string_buffer_putc(sb, ' ');
                    libexplain_buffer_file_type(sb, S_IFDIR);
                    libexplain_string_buffer_puts(sb, " in the ");
                    /* the rule is: [caption] path file-type */
                    libexplain_string_buffer_puts(sb, caption);
                    libexplain_string_buffer_putc(sb, ' ');
                    libexplain_string_buffer_puts_quoted(sb, lookup_directory);
                    libexplain_string_buffer_putc(sb, ' ');
                    libexplain_buffer_file_type(sb, S_IFDIR);

                    /*
                     * If it was a typo, see if we can find something similar.
                     */
                    look_for_similar(sb, lookup_directory, component);
                    goto return_0;
                }

                /*
                 * intermediate path of non-final component returned
                 * unexpected error to lstat (i.e. not ENOENT), bail out
                 */
                goto return_minus_1;
            }

            /*
             * At this point, we know it is a final component.
             */
            if (final_component->must_not_exist)
            {
                if (intermediate_path_st_errnum == ENOENT)
                {
                    if (!lookup_directory_writable)
                    {
                        /* EACCES */
                        libexplain_string_buffer_puts
                        (
                            sb,
                            "the process does not have write permission to the"
                        );
                        libexplain_string_buffer_putc(sb, ' ');
                        /* the rule is: [caption] path file-type */
                        libexplain_string_buffer_puts(sb, caption);
                        libexplain_string_buffer_putc(sb, ' ');
                        libexplain_string_buffer_puts_quoted
                        (
                            sb,
                            lookup_directory
                        );
                        libexplain_string_buffer_putc(sb, ' ');
                        libexplain_buffer_file_type(sb, S_IFDIR);

                        libexplain_string_buffer_puts
                        (
                            sb,
                            ", this is needed to create the "
                        );
                        /* the rule is: [caption] path file-type */
                        libexplain_string_buffer_puts_quoted(sb, component);
                        libexplain_string_buffer_puts(sb, " directory entry");

                        /*
                         * FIXME: may need to explain that the
                         * permissions are exclusive, rather than
                         * inclusive as naive users expect.
                         */
                        goto return_0;
                    }

                    /*
                     * yay,
                     * but we were looking for an error
                     * and didn't find it
                     */
                    goto return_minus_1;
                }

                /*
                 * The final component is not meant to exist, but the
                 * final component gave an unexpected error to lstat
                 * (i.e. not ENOENT), bail out
                 */
                goto return_minus_1;
            }
            if (final_component->must_exist)
            {
                if (intermediate_path_st_errnum != ENOENT)
                {
                    /*
                     * The final component is meant to exist, but the
                     * final component gave an unexpected error to
                     * lstat (i.e. not success and not ENOENT), bail out
                     */
                    goto return_minus_1;
                }

                /* ENOENT */
                libexplain_string_buffer_puts(sb, " there is no ");
                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_puts_quoted(sb, component);
                libexplain_string_buffer_putc(sb, ' ');
                if (final_component->must_be_a_directory)
                    libexplain_buffer_file_type(sb, S_IFDIR);
                else
                    libexplain_buffer_gettext(sb, i18n("file"));
                libexplain_string_buffer_puts(sb, " in the ");
                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_puts(sb, caption);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_string_buffer_puts_quoted(sb, lookup_directory);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, S_IFDIR);

                /*
                 * If it was a typo, see if we can find something similar.
                 */
                look_for_similar(sb, lookup_directory, component);

                goto return_0;
            }

            /*
             * Creating a new file requires write permission in the
             * containing directory.
             */
            if
            (
                (expected_errno == EACCES || expected_errno == EPERM)
            &&
                intermediate_path_st_errnum == ENOENT
            &&
                final_component->want_to_create
            &&
                !lookup_directory_writable
            )
            {
                /*
                 * No write permission to containing directory.
                 */
                libexplain_buffer_gettext
                (
                    sb,
                    i18n("the process does not have write permission to the")
                );
                libexplain_string_buffer_putc(sb, ' ');
                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_puts(sb, caption);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_string_buffer_puts_quoted(sb, lookup_directory);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, S_IFDIR);

                libexplain_string_buffer_puts
                (
                    sb,
                    ", this is needed to create the directory entry for the "
                );
                libexplain_string_buffer_puts_quoted(sb, component);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, final_component->st_mode);

                /*
                 * What if they meant to overwrite an existing file?
                 * That would not have needed write permission on the
                 * containing directory.
                 */
                look_for_similar(sb, lookup_directory, component);
                goto return_0;
            }

            /*
             * It's OK if the final component path doesn't exist,
             * but we were looking for an error and didn't find one.
             */
            goto return_minus_1;
        }

        /*
         * At this point, we know that the intermediate path exists.
         */
        if
        (
            S_ISLNK(intermediate_path_st.st_mode)
        &&
            (!final || final_component->follow_symlink)
        )
        {
            int             n;
            char            rlb[PATH_MAX + 1];

            /*
             * Have we read this symlink before?
             */
            if (symlinks_so_far)
            {
                int             j;

                for (j = 0; j < number_of_symlinks_followed; ++j)
                {
                    if (0 == strcmp(symlinks_so_far[j], intermediate_path))
                    {
                        libexplain_string_buffer_puts
                        (
                            sb,
                            "a symbolic link loop was encountered in "
                        );
                        libexplain_string_buffer_puts(sb, caption);
                        libexplain_string_buffer_puts(sb, ", starting at ");
                        libexplain_string_buffer_puts_quoted
                        (
                            sb,
                            intermediate_path
                        );
                        goto return_0;
                    }
                }
            }

            /*
             * Follow the symbolic link, that way we can give the actual
             * path in our error messages.
             */
            n = readlink(intermediate_path, rlb, sizeof(rlb) - 1);
            if (n >= 0)
            {
                char            new_pathname[PATH_MAX + 1];
                libexplain_string_buffer_t new_pathname_buf;

                if (symlinks_so_far)
                {
                    size_t          len;
                    char            *p;

                    len = strlen(intermediate_path) + 1;
                    p = malloc(len);
                    if (!p)
                    {
                        int             j;

                        for (j = 0; j < number_of_symlinks_followed; ++j)
                            free(symlinks_so_far[j]);
                        free(symlinks_so_far);
                        symlinks_so_far = 0;
                        if (expected_errno == ELOOP || expected_errno == EMLINK)
                            goto return_minus_1;
                    }
                    else
                    {
                        memcpy(p, intermediate_path, len);
                        symlinks_so_far[number_of_symlinks_followed] = p;
                    }
                }

                if (n == 0)
                {
                    rlb[0] = '.';
                    n = 1;
                }
                rlb[n] = '\0';

                libexplain_string_buffer_init
                (
                    &new_pathname_buf,
                    new_pathname,
                    sizeof(new_pathname)
                );
                libexplain_string_buffer_puts(&new_pathname_buf, rlb);
                libexplain_string_buffer_path_join(&new_pathname_buf, pp);

                libexplain_string_buffer_copy(&pathname_buf, &new_pathname_buf);

                /*
                 * Check for dangling symbolic links
                 */
                libexplain_string_buffer_init
                (
                    &intermediate_path_buf,
                    intermediate_path,
                    sizeof(intermediate_path)
                );
                if (rlb[0] == '/')
                    libexplain_string_buffer_puts(&intermediate_path_buf, rlb);
                else
                {
                    libexplain_string_buffer_puts
                    (
                        &intermediate_path_buf,
                        lookup_directory
                    );
                    libexplain_string_buffer_path_join
                    (
                        &intermediate_path_buf,
                        rlb
                    );
                }
                if
                (
                    lstat(intermediate_path, &intermediate_path_st) < 0
                &&
                    errno == ENOENT
                )
                {
                    /* the rule is: [caption] path file-type */
                    libexplain_string_buffer_puts_quoted(sb, component);
                    libexplain_string_buffer_putc(sb, ' ');
                    libexplain_buffer_gettext(sb, i18n("symbolic link"));
                    libexplain_string_buffer_puts(sb, " in ");
                    /* the rule is: [caption] path file-type */
                    libexplain_string_buffer_puts(sb, caption);
                    libexplain_string_buffer_putc(sb, ' ');
                    libexplain_string_buffer_puts_quoted(sb, lookup_directory);
                    libexplain_string_buffer_putc(sb, ' ');
                    libexplain_buffer_file_type(sb, S_IFDIR);
                    libexplain_string_buffer_puts(sb, " refers to ");
                    libexplain_string_buffer_puts_quoted(sb, rlb);
                    libexplain_string_buffer_puts(sb, " that does not exist");
                    goto return_0;
                }

                if (rlb[0] == '/')
                {
                    lookup_directory_buf.position = 1;
                    lookup_directory[0] = '/';
                }
                ++number_of_symlinks_followed;
                if (number_of_symlinks_followed >= symloop_max)
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "too many symbolic links "
                    );
                    if (libexplain_option_dialect_specific())
                    {
                        libexplain_string_buffer_printf
                        (
                            sb,
                            " (%d)",
                            number_of_symlinks_followed
                        );
                    }
                    libexplain_string_buffer_puts(sb, " were encountered in ");
                    libexplain_string_buffer_puts(sb, caption);
                    goto return_0;
                }
                pp = pathname;
                while (*pp == '/')
                    ++pp;
                continue;
            }
        }

        if (!final)
        {
            /*
             * check that intermediate_path is, in fact, a directory
             */
            if (!S_ISDIR(intermediate_path_st.st_mode))
            {
                /* ENOTDIR */
                libexplain_string_buffer_puts(sb, " the ");
                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_puts_quoted(sb, component);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, intermediate_path_st.st_mode);
                libexplain_string_buffer_puts(sb, " in the ");
                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_puts(sb, caption);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_string_buffer_puts_quoted(sb, lookup_directory);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, S_IFDIR);
                libexplain_string_buffer_puts
                (
                    sb,
                    " is being used as a directory when it is not"
                );
                return 0;
            }

            libexplain_string_buffer_copy
            (
                &lookup_directory_buf,
                &intermediate_path_buf
            );
            continue;
        }

        /*
         * At this point, we know that intermediate_path is the final
         * path, and we know it *does* exist.
         */
        if (final_component->must_not_exist)
        {
            libexplain_string_buffer_puts(sb, "in the ");
            /* the rule is: [caption] path file-type */
            libexplain_string_buffer_puts(sb, caption);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_string_buffer_puts_quoted(sb, lookup_directory);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_buffer_file_type(sb, S_IFDIR);
            libexplain_string_buffer_puts(sb, " there is a ");
            /* the rule is: [caption] path file-type */
            libexplain_string_buffer_puts_quoted(sb, component);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_buffer_file_type(sb, intermediate_path_st.st_mode);

            libexplain_string_buffer_puts(sb, ", but it should not exist yet");
            goto return_0;
        }

        if
        (
            final_component->must_be_a_directory
        &&
            !S_ISDIR(intermediate_path_st.st_mode)
        )
        {
            libexplain_string_buffer_puts(sb, "in the ");
            /* the rule is: [caption] path file-type */
            libexplain_string_buffer_puts(sb, caption);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_string_buffer_puts_quoted(sb, lookup_directory);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_buffer_file_type(sb, S_IFDIR);
            libexplain_string_buffer_puts(sb, " there is a ");
            /* the rule is: [caption] path file-type */
            libexplain_string_buffer_puts_quoted(sb, component);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_buffer_file_type(sb, intermediate_path_st.st_mode);

            libexplain_string_buffer_puts(sb, ", but it should be a ");
            libexplain_buffer_file_type(sb, S_IFDIR);
            goto return_0;
        }

        if
        (
            (expected_errno == EACCES || expected_errno == EPERM)
        &&
            final_component->want_to_modify_inode
        &&
            !libexplain_have_inode_permission(&intermediate_path_st)
        )
        {
            libexplain_buffer_gettext
            (
                sb,
                i18n("the process does not have inode modification "
                    "permission to the")
            );

            /* the rule is: [caption] path file-type */
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_string_buffer_puts_quoted(sb, component);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_buffer_file_type(sb, intermediate_path_st.st_mode);

            libexplain_string_buffer_puts(sb, " in the");

            /* the rule is: [caption] path file-type */
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_string_buffer_puts(sb, caption);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_string_buffer_puts_quoted(sb, lookup_directory);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_buffer_file_type(sb, S_IFDIR);
            goto return_0;
        }

        if
        (
            (expected_errno == EACCES || expected_errno == EPERM)
        &&
            final_component->want_to_unlink
        &&
            !lookup_directory_writable
        )
        {
            if ((lookup_directory_st.st_mode & S_ISVTX) == 0)
            {
                /*
                 * No sticky bit set, therefore only need write permissions on
                 * the lookup directory.
                 */
                libexplain_string_buffer_puts
                (
                    sb,
                    "the process does not have write permission to the"
                );
                libexplain_string_buffer_putc(sb, ' ');
                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_puts(sb, caption);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_string_buffer_puts_quoted(sb, lookup_directory);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, S_IFDIR);

                libexplain_string_buffer_puts
                (
                    sb,
                    ", this is needed to remove the directory entry for the "
                );
                libexplain_string_buffer_puts_quoted(sb, component);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type
                (
                    sb,
                    intermediate_path_st.st_mode
                );
                goto return_0;
            }
            else
            {
                int             uid;

                uid = geteuid();
                if
                (
                    uid != (int)intermediate_path_st.st_uid
                &&
                    uid != (int)lookup_directory_st.st_uid
                &&
                    !libexplain_capability_fowner()
                )
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "the process does not have write permission to the"
                    );
                    libexplain_string_buffer_putc(sb, ' ');
                    /* the rule is: [caption] path file-type */
                    libexplain_string_buffer_puts(sb, caption);
                    libexplain_string_buffer_putc(sb, ' ');
                    libexplain_string_buffer_puts_quoted(sb, lookup_directory);
                    libexplain_string_buffer_putc(sb, ' ');
                    libexplain_buffer_file_type(sb, S_IFDIR);

                    libexplain_string_buffer_puts
                    (
                        sb,
                        ", this is needed to remove the directory "
                        "entry for the"
                    );
                    /* the rule is: [caption] path file-type */
                    libexplain_string_buffer_puts_quoted(sb, component);
                    libexplain_string_buffer_putc(sb, ' ');
                    libexplain_buffer_file_type
                    (
                        sb,
                        intermediate_path_st.st_mode
                    );
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "; the directory has the sticky bit (S_ISVTX) "
                        "set and the process's effective user ID ("
                    );
                    libexplain_buffer_uid(sb, uid);
                    libexplain_string_buffer_puts
                    (
                        sb,
                        ") is neither the user ID of the "
                    );
                    /* the rule is: [caption] path file-type */
                    libexplain_string_buffer_puts_quoted(sb, component);
                    libexplain_string_buffer_putc(sb, ' ');
                    libexplain_buffer_file_type
                    (
                        sb,
                        intermediate_path_st.st_mode
                    );
                    libexplain_string_buffer_puts
                    (
                        sb,
                        " to be removed ("
                    );
                    libexplain_buffer_uid(sb, intermediate_path_st.st_uid);
                    libexplain_string_buffer_puts
                    (
                        sb,
                        ") nor that of the directory containing it ("
                    );
                    libexplain_buffer_uid(sb, lookup_directory_st.st_uid);
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "), and the process is not privileged"
                    );
#ifndef HAVE_SYS_CAPABILITY_H
                    if (libexplain_option_dialect_specific())
                    {
                        libexplain_string_buffer_puts
                        (
                            sb,
                            " (does not have the CAP_FOWNER capability)"
                        );
                    }
#endif
                    goto return_0;
                }
            }
        }

        if (expected_errno == EACCES)
        {
            if
            (
                final_component->want_to_read
            &&
                !libexplain_have_read_permission(&intermediate_path_st)
            )
            {
                libexplain_buffer_gettext
                (
                    sb,
                    i18n("the process does not have read permission to the")
                );

                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_string_buffer_puts_quoted(sb, component);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, intermediate_path_st.st_mode);

                libexplain_string_buffer_puts(sb, " in the");

                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_string_buffer_puts(sb, caption);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_string_buffer_puts_quoted(sb, lookup_directory);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, S_IFDIR);
                goto return_0;
            }

            if
            (
                final_component->want_to_write
            &&
                !libexplain_have_write_permission(&intermediate_path_st)
            )
            {
                libexplain_buffer_gettext
                (
                    sb,
                    i18n("the process does not have write permission to the")
                );

                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_string_buffer_puts_quoted(sb, component);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, intermediate_path_st.st_mode);

                libexplain_string_buffer_puts(sb, " in the");
                libexplain_string_buffer_putc(sb, ' ');

                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_puts(sb, caption);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_string_buffer_puts_quoted(sb, lookup_directory);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, S_IFDIR);
                goto return_0;
            }

            if
            (
                final_component->want_to_execute
            &&
                !libexplain_have_execute_permission(&intermediate_path_st)
            )
            {
                libexplain_buffer_gettext
                (
                    sb,
                    i18n("the process does not have execute permission to the")
                );

                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_string_buffer_puts_quoted(sb, component);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, intermediate_path_st.st_mode);

                libexplain_string_buffer_puts(sb, " in the");
                libexplain_string_buffer_putc(sb, ' ');

                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_puts(sb, caption);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_string_buffer_puts_quoted(sb, lookup_directory);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, S_IFDIR);
                goto return_0;
            }

            if
            (
                final_component->want_to_search
            &&
                !libexplain_have_search_permission(&intermediate_path_st)
            )
            {
                libexplain_buffer_gettext
                (
                    sb,
                    i18n("the process does not have search permission to the")
                );

                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_string_buffer_puts_quoted(sb, component);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, intermediate_path_st.st_mode);

                libexplain_string_buffer_puts(sb, " in the");
                libexplain_string_buffer_putc(sb, ' ');

                /* the rule is: [caption] path file-type */
                libexplain_string_buffer_puts(sb, caption);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_string_buffer_puts_quoted(sb, lookup_directory);
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, S_IFDIR);
                goto return_0;
            }
        }

        /*
         * No error, yay!  Except that we were looking for an error, and
         * did not find one.
         */
        return_minus_1:
        if (symlinks_so_far)
        {
            int             k;

            for (k = 0; k < number_of_symlinks_followed; ++k)
                free(symlinks_so_far[k]);
            free(symlinks_so_far);
        }
        return -1;
    }
}


void
libexplain_final_init(libexplain_final_t *p)
{
    p->want_to_read = 0;
    p->want_to_write = 0;
    p->want_to_search = 0;
    p->want_to_execute = 0;
    p->want_to_create = 0;
    p->want_to_modify_inode = 0;
    p->want_to_unlink = 0;
    p->must_exist = 1;
    p->must_not_exist = 0;
    p->must_be_a_directory = 0;
    p->follow_symlink = 1;
    p->st_mode = S_IFREG;
}
