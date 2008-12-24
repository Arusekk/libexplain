/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
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
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/get_current_directory.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/errno/fstat.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/is_same_inode.h>


/**
  * The recursive_pwd function is used to recursively walk up the
  * ".." chain until "/" is reached, and then glue on the discovered
  * directory entry names as it unwinds.
  *
  * @param sb
  *     The string buffer to print error messages into.
  * @param dot
  *     The string buffer to build the ".." chain into.
  * @param dot_st
  *     The file status information of the drectory entry to search ".." for.
  * @param result
  *     The constructed pathname as parent names are discovered.
  * @returns
  *     int; 0 if a path was found, -1 if no peth could be found AND an
  *     error message has already been printed.
  */
static int
recursive_pwd(libexplain_string_buffer_t *sb, libexplain_string_buffer_t *dot,
    const struct stat *dot_st, libexplain_string_buffer_t *result)
{
    DIR             *dp;
    struct stat     dotdot_st;

    libexplain_string_buffer_puts(dot, "/..");
    dp = opendir(dot->message);
    if (!dp)
    {
        int             errnum;
        libexplain_final_t final_component;
        int             ok;

        errnum = errno;
        libexplain_final_init(&final_component);
        final_component.want_to_read = 1;
        final_component.must_be_a_st_mode = 1;
        final_component.st_mode = S_IFDIR;
        ok =
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                dot->message,
                "pathname",
                &final_component
            );
        return (ok ? 0 : -1);
    }
    if (fstat(dirfd(dp), &dotdot_st) < 0)
    {
        libexplain_buffer_errno_fstat_explanation
        (
            sb,
            errno,
            dirfd(dp),
            &dotdot_st
        );
        return -1;
    }
    if (libexplain_is_same_inode(dot_st, &dotdot_st))
    {
        libexplain_string_buffer_putc(result, '/');
        return 0;
    }
    for (;;)
    {
        size_t          old_pos;
        struct dirent   *dep;
        struct stat     dirent_st;

        dep = readdir(dp);
        if (!dep)
            break;
        if (0 == strcmp(dep->d_name, "."))
            continue;
        if (0 == strcmp(dep->d_name, ".."))
            continue;
        old_pos = dot->position;
        libexplain_string_buffer_path_join(dot, dep->d_name);
        if (lstat(dot->message, &dirent_st) < 0)
        {
            int             errnum;
            libexplain_final_t final_component;
            int             ok;

            errnum = errno;
            libexplain_final_init(&final_component);
            ok =
                libexplain_buffer_errno_path_resolution
                (
                    sb,
                    errnum,
                    dot->message,
                    "pathname",
                    &final_component
                );
            return (ok ? 0 : -1);
        }
        libexplain_string_buffer_truncate(dot, old_pos);
        if (libexplain_is_same_inode(dot_st, &dirent_st))
        {
            char            name[NAME_MAX + 1];

            strendcpy(name, dep->d_name, name + sizeof(name));
            closedir(dp);
            /*
             * Now we recurse up the directory tree until we find the
             * root.  This is mostly tail recursion, but factoring that
             * out makes the code ugly, let's hope the compiler notices
             * and does something clever.
             */
            if (recursive_pwd(sb, dot, &dotdot_st, result) < 0)
            {
                return -1;
            }
            libexplain_string_buffer_path_join(result, name);
            return 0;
        }
    }
    closedir(dp);

    /*
     * No such directory.
     */
    {
        libexplain_string_buffer_t msg1_sb;
        libexplain_string_buffer_t msg2_sb;
        char            msg1[PATH_MAX + 1];
        char            msg2[PATH_MAX + 5];

        libexplain_string_buffer_init(&msg1_sb, msg1, sizeof(msg1));
        libexplain_string_buffer_puts(&msg1_sb, dot->message);
        libexplain_string_buffer_puts(&msg1_sb, "/..");

        libexplain_string_buffer_init(&msg2_sb, msg2, sizeof(msg2));
        libexplain_string_buffer_puts_quoted(&msg2_sb, msg1);

        libexplain_string_buffer_init(&msg1_sb, msg1, sizeof(msg1));
        libexplain_string_buffer_puts_quoted(&msg1_sb, dot->message);

        libexplain_string_buffer_printf_gettext
        (
            sb,
            /*
             * xgettext: This message is used when getcwd() is trying
             * to reconstruct the problem, and discovers that the
             * backwards ".." chain is broken.
             */
            i18n("there is no directory entry in %s that has the same "
                 "inode number as %s; this means that the directory has "
                 "been unlinked"),
            msg2,
            msg1
        );
        if (0 == strcmp(result->message, "/") && dot_st->st_ino != 2)
        {
            libexplain_string_buffer_puts(sb, ", ");
            libexplain_buffer_gettext
            (
                sb,
                /*
                 * xgettext: This message is used when getcwd() is
                 * trying to reconstruct the problem, and discovers that
                 * the process is probably running inside a chroot jail,
                 * and that the current directory is actually ouside
                 * that chroot jail.
                 */
                i18n("or is outside your chroot jail")
            );
        }
    }
    return -1;
}


int
libexplain_buffer_get_current_directory(libexplain_string_buffer_t *sb,
    char *data, size_t data_size)
{
    assert(data_size > PATH_MAX);

    /*
     * The first thing to try is the PWD environment variable, which is
     * maintained by many of the shell interpreters.
     */
    {
        char            *pwd;

        pwd = getenv("PWD");
        if (pwd && pwd[0] == '/')
        {
            struct stat dot_st;
            struct stat pwd_st;

            if
            (
                lstat(".", &dot_st) >= 0
            &&
                lstat(pwd, &pwd_st) >= 0
            &&
                libexplain_is_same_inode(&dot_st, &pwd_st)
            )
            {
                strendcpy(data, pwd, data + data_size);
                return 0;
            }
        }
    }

#ifdef PROC_FS_USEFUL
    /*
     * The next thing to try is the /proc file system.
     */
    {
        ssize_t         n;

        n = readlink("/proc/self/pwd", data, data_size - 1);
        if (n > 0)
        {
            struct stat     dot_st;
            struct stat     pwd_st;

            data[n] = '\0';
            if
            (
                lstat(".", &dot_st) >= 0
            &&
                lstat(data, &pwd_st) >= 0
            &&
                libexplain_is_same_inode(&dot_st, &pwd_st)
            )
            {
                return 0;
            }
        }
    }
#endif

    /*
     * If all else fails, do it the slow way.
     */
    {
        libexplain_string_buffer_t dot_sb;
        libexplain_string_buffer_t result_sb;
        struct stat     dot_st;
        char dot[PATH_MAX * 2 + 1];

        if (lstat(".", &dot_st) < 0)
        {
            int             errnum;
            libexplain_final_t final_component;
            int             ok;

            errnum = errno;
            libexplain_final_init(&final_component);
            final_component.must_be_a_st_mode = 1;
            final_component.st_mode = S_IFDIR;
            ok =
                libexplain_buffer_errno_path_resolution
                (
                    sb,
                    errnum,
                    ".",
                    "pathname",
                    &final_component
                );
            return (ok ? 0 : -1);
        }
        libexplain_string_buffer_init(&dot_sb, dot, sizeof(dot));
        libexplain_string_buffer_putc(&dot_sb, '.');
        libexplain_string_buffer_init(&result_sb, data, data_size);
        return recursive_pwd(sb, &dot_sb, &dot_st, &result_sb);
    }
}
