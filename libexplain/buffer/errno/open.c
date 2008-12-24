/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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
#include <libexplain/ac/errno.h>
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/dac.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/enfile.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/open.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/etxtbsy.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/path_to_pid.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/pretty_size.h>
#include <libexplain/buffer/uid.h>
#include <libexplain/capability.h>
#include <libexplain/explanation.h>
#include <libexplain/open_flags.h>
#include <libexplain/option.h>
#include <libexplain/permission_mode.h>
#include <libexplain/string_buffer.h>


static void
libexplain_buffer_errno_open_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, int flags, int mode)
{
    libexplain_string_buffer_printf(sb, "open(pathname = ");
    if (errnum == EFAULT)
        libexplain_buffer_pointer(sb, pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_puts(sb, ", flags = ");
    libexplain_buffer_open_flags(sb, flags);
    if (flags & O_CREAT)
    {
        libexplain_string_buffer_puts(sb, ", mode = ");
        libexplain_buffer_permission_mode(sb, mode);
    }
    libexplain_string_buffer_putc(sb, ')');
}


static void
you_can_not_open_a_socket(libexplain_string_buffer_t *sb)
{
    libexplain_buffer_gettext
    (
        sb,
        /*
         * xgettext:  This message is used when open(2) received an
         * ENODEV error, and the pathname it attempted to open was a
         * socket (first character "s" is ls(1) long output).  They
         * probably meant to use a named pipe (first character "p" in
         * ls(1) long outout).
         */
        i18n("you cannot use open(2) to open socket files, you must use "
        "connect(2) instead; a named pipe may be what was intended")
    );
}


static void
no_corresponding_device(libexplain_string_buffer_t *sb, const struct stat *st)
{
    char            ftype[100];
    char            numbers[40];
    libexplain_string_buffer_t ftype_sb;
    libexplain_string_buffer_t numbers_sb;

    libexplain_string_buffer_init(&ftype_sb, ftype, sizeof(ftype));
    libexplain_buffer_file_type(&ftype_sb, st->st_mode);

    libexplain_string_buffer_init(&numbers_sb, numbers, sizeof(numbers));
    libexplain_string_buffer_printf
    (
        &numbers_sb,
        "(%d, %d)",
        major(st->st_dev),
        minor(st->st_dev)
    );
    libexplain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used to explain
         * an ENODEV error reported by an open(2)
         * system call, and the device does not
         * actually exist.
         *
         * %1$s => the file type of the special file,
         *         already translated.
         * %2$s => the major and minor device numbers
         *
         * Example: "pathname refers to a block special file (42, 13) and no
         *           corresponding device exists"
         */
        i18n("pathname refers to a %s %s and no corresponding device exists"),
        ftype,
        numbers
    );
}


void
libexplain_buffer_errno_open_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, int flags, int mode)
{
    libexplain_final_t final_component;

    (void)mode;
    libexplain_final_init(&final_component);
    switch (flags & O_ACCMODE)
    {
    case O_RDONLY:
        final_component.want_to_read = 1;
        break;

    case O_RDWR:
        final_component.want_to_read = 1;
        final_component.want_to_write = 1;
        break;

    case O_WRONLY:
        final_component.want_to_write = 1;
        break;

    default:
        assert(!"unknown open access mode");
        break;
    }
    if (flags & O_CREAT)
    {
        final_component.want_to_create = 1;
        final_component.must_exist = 0;
        if (flags & O_EXCL)
            final_component.must_not_exist = 1;
    }
    if (flags & O_DIRECTORY)
    {
        final_component.must_be_a_st_mode = 1;
        final_component.st_mode = S_IFDIR;
    }
    if (flags & O_NOFOLLOW)
        final_component.follow_symlink = 0;

    switch (errnum)
    {
    case EACCES:
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                "pathname",
                &final_component
            )
        )
        {
            libexplain_buffer_gettext
            (
                sb,
                /*
                 * xgettext:  This message is used when explaining an
                 * EACCES error returned by an open(2) system call.
                 * Usually path_resolution(7) will have a better
                 * explanation, this explanation is only used when a
                 * more specific explanation is not available.
                 */
                i18n("the requested access to the file is not allowed, or "
                "search permission is denied for one of the directories "
                "in the path prefix of pathname, or the file did not "
                "exist yet and write access to the parent directory is "
                "not allowed")
            );
        }
        break;

    case EINVAL:
        libexplain_buffer_einval_bits(sb, "flags");
        break;

    case EEXIST:
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                "pathname",
                &final_component
            )
        )
        {
            libexplain_buffer_gettext
            (
                sb,
                /*
                 * xgettext:  This message is used when explaining an
                 * EEXIST error returned by an open(2) system call.
                 * Usually path_resolution(7) will have a better
                 * explanation, this explanation is only used when a
                 * more specific explanation is not available.
                 */
                i18n("the file already exists when it should not (flags "
                "O_CREAT and O_EXCL were used)")
            );
        }
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "pathname");
        break;

    case EFBIG:
    case EOVERFLOW:
        if (!(flags & O_LARGEFILE))
        {
            struct stat     st;

            if (stat(pathname, &st) >= 0 && S_ISREG(st.st_mode))
            {
                char            siz[20];
                libexplain_string_buffer_t siz_sb;

                libexplain_string_buffer_init(&siz_sb, siz, sizeof(siz));
                {
                    libexplain_string_buffer_putc(sb, '(');
                    libexplain_buffer_pretty_size(sb, st.st_size);
                    libexplain_string_buffer_putc(sb, ')');
                }
                libexplain_string_buffer_printf_gettext
                (
                    sb,
                    /*
                     * xgettext:  This message is used to explain an EFBIG
                     * or EOVERFLOW error reported by and open(2) system
                     * call.  The file is, in fact, too large to be opened
                     * without the O_LARGEFILE flag.
                     *
                     * %1$s => The size of the file, in parentheses
                     */
                    i18n("pathname referes to a regular file that is too large "
                        "to be opened %s, the O_LARGEFILE flag is necessary"),
                    siz
                );
            }
        }
        break;

    case EISDIR:
        if ((flags & O_ACCMODE) != O_RDONLY)
        {
            struct stat     st;

            if (stat(pathname, &st) >= 0 && S_ISDIR(st.st_mode))
            {
                libexplain_buffer_gettext
                (
                    sb,
                    /*
                     * xgettext: This message is used to explain an EISDIR
                     * reported by an open(2) system call.  You may not open
                     * a directory for writing.
                     */
                    i18n("pathname refers to a directory and the access "
                    "requested involved writing")
                );
            }
        }
        break;

    case ELOOP:
    case EMLINK: /* BSD */
        if (flags & O_NOFOLLOW)
        {
            struct stat     st;

            if (lstat(pathname, &st) >= 0 && S_ISLNK(st.st_mode))
            {
                libexplain_buffer_gettext
                (
                    sb,
                    /*
                     * xgettext: This message is used to explain an
                     * ELOOP or EMLINK error reported by an open(2)
                     * system call, in the case where the O_NOFOLLOW
                     * flags was specified but the final path component
                     * was a symbolic link.
                     */
                    i18n("O_NOFOLLOW was specified but pathname refers to a "
                    "symbolic link")
                );
                /* FIXME: mention this may indicate some kind of attack? */
                break;
            }
        }
        libexplain_buffer_eloop(sb, pathname, "pathname", &final_component);
        break;

    case EMFILE:
        libexplain_buffer_emfile(sb);
        break;

    case ENAMETOOLONG:
        libexplain_buffer_enametoolong
        (
            sb,
            pathname,
            "pathname",
            &final_component
        );
        break;

    case ENFILE:
        libexplain_buffer_enfile(sb);
        break;

    case ENOENT:
        libexplain_buffer_enoent(sb, pathname, "pathname", &final_component);
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    case ENOSPC:
        {
            char            mntpt[100];
            libexplain_string_buffer_t mntpt_sb;

            libexplain_string_buffer_init(&mntpt_sb, mntpt, sizeof(mntpt));
            libexplain_buffer_mount_point_dirname(&mntpt_sb, pathname);
            libexplain_string_buffer_printf_gettext
            (
                sb,
                /*
                 * xgettext: This message is used to provide an
                 * explanation for and ENOSPC error returned by an
                 * open(2) system call, in the case where there is no
                 * more room for a new file.
                 *
                 * %1$s => The name of the problematic system call argument
                 * %2$s => The file system mount point and usage,
                 *         in parentheses
                 */
                i18n("the file system containing %s %s has no "
                    "room for the new file"),
                "pathname",
                mntpt
            );
            /* FIXME: has no blocks left or has no inodes left? */
        }
        break;

    case ENOTDIR:
        libexplain_buffer_enotdir(sb, pathname, "pathname", &final_component);
        break;

    case ENXIO:
        {
            struct stat     st;

            if (stat(pathname, &st) < 0)
            {
                enxio_generic:
                libexplain_buffer_gettext
                (
                    sb,
                    /*
                     * xgettext:  This message is used to explain an
                     * ENXIO error returned by an open(2) system call.
                     * This is the generic explanation, used when no
                     * more specific cause can be determined.
                     */
                    i18n("O_NONBLOCK | O_WRONLY is set, the named file "
                    "is a FIFO and no process has the file open for "
                    "reading; or, the file is a device special file and "
                    "no corresponding device exists")
                );
                break;
            }
            switch (st.st_mode & S_IFMT)
            {
            case S_IFIFO:
                libexplain_buffer_gettext
                (
                    sb,
                    /*
                     * xgettext: This message is used to explain an
                     * ENXIO error returned by an open(2) system call,
                     * in the case where a named pipe has no readers,
                     * and a non-blocking writer tried to open it.
                     */
                    i18n("O_NONBLOCK | O_WRONLY is set, and the named file "
                    "is a FIFO, and no process has the file open for "
                    "reading")
                );
                /* FIXME: what happens if you open a named pipe O_RDWR? */
                break;

            case S_IFCHR:
            case S_IFBLK:
                no_corresponding_device(sb, &st);
                break;

            case S_IFSOCK:
                you_can_not_open_a_socket(sb);
                break;

            default:
                goto enxio_generic;
            }
        }
        break;

    case EPERM:
        if (flags & O_NOATIME)
        {
            struct stat     st;
            libexplain_string_buffer_t puid_sb;
            libexplain_string_buffer_t ftype_sb;
            libexplain_string_buffer_t fuid_sb;
            char puid[100];
            char ftype[100];
            char fuid[100];

            libexplain_string_buffer_init(&puid_sb, puid, sizeof(puid));
            libexplain_buffer_uid(&puid_sb, geteuid());
            libexplain_string_buffer_init(&ftype_sb, ftype, sizeof(ftype));
            libexplain_string_buffer_init(&fuid_sb, fuid, sizeof(fuid));
            if (stat(pathname, &st) >= 0)
            {
                libexplain_buffer_file_type(&ftype_sb, st.st_mode);
                libexplain_buffer_uid(&fuid_sb, st.st_uid);
            }
            else
                libexplain_buffer_file_type(&ftype_sb, S_IFREG);
            libexplain_string_buffer_printf_gettext
            (
                sb,
                /*
                 * xgettext: This message is used when an EPERM erro is
                 * returned by an open(2) system call, and the O_NOATIME
                 * open flag was specific, but the process lacked the
                 * permissions required.
                 *
                 * %1$s => the number and name of the process effective UID,
                 *         already quoted if needed
                 * %2$s => the file type of the file in question,
                 *         almost always "regular file" (already translated)
                 * %3$s => the number and name of the file owner UID,
                 *         already quoted if needed
                 */
                i18n("the O_NOATIME flags was specified, but the process "
                "effective UID %s does not match the %s owner UID %s"),
                puid,
                ftype,
                fuid
            );

            /*
             * also explain the necessary priviledge
             */
            libexplain_buffer_dac_fowner(sb);
        }
        break;

    case EROFS:
        libexplain_buffer_erofs(sb, pathname, "pathname");
        break;

    case ETXTBSY:
        libexplain_buffer_etxtbsy(sb, pathname);
        break;

    case EWOULDBLOCK:
        if (flags & O_NONBLOCK)
        {
            libexplain_buffer_gettext
            (
                sb,
                /*
                 * xgettext: This message is used to explain an
                 * EWOULDBLOCK error returned by an open(2) system call,
                 * when the use of thr O_NONBLOCK flags would otherwise
                 * cause the open(2) system call to block.
                 */
                i18n("the O_NONBLOCK flag was specified, and an "
                "incompatible lease was held on the file")
            );

            /*
             * Look for other processes with this file open,
             * and list their PIDs.
             */
            libexplain_buffer_path_to_pid(sb, pathname);
        }
        break;

    case ENODEV:
        {
            struct stat     st;

            if (stat(pathname, &st) >= 0)
            {
                switch (st.st_mode & S_IFMT)
                {
                case S_IFSOCK:
                    you_can_not_open_a_socket(sb);
                    break;

                case S_IFBLK:
                case S_IFCHR:
                    no_corresponding_device(sb, &st);
#ifdef __linux__
                    if (libexplain_option_dialect_specific())
                    {
                        libexplain_string_buffer_puts(sb, "; ");
                        libexplain_buffer_gettext
                        (
                            sb,
                            /*
                             * xgettext: This message is used to explain an
                             * ENODEV error reported by an open(2) system
                             * call, which shoudl actually have been a ENXIO
                             * error instead.  They are easy to confuse,
                             * they have exactly the same English text
                             * returned from strerror(3).
                             */
                            i18n("this is a Linux kernel bug, in this "
                            "situation POSIX says ENXIO should have been "
                            "returned")
                        );
                    }
#endif
                    break;

                default:
                    break;
                }
            }
        }
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
    if ((flags & O_EXCL) && !(flags & O_CREAT))
    {
        libexplain_string_buffer_puts(sb, "; ");
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This message is used to supplement an explanation for
             * an error reported by open(2) system call, and the caller used a
             * flags combination with explicitly undefined behavior.
             */
            i18n("note that the behavior of O_EXCL is undefined if "
            "O_CREAT is not specified")
        );
    }
}


void
libexplain_buffer_errno_open(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, int flags, int mode)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_open_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        flags,
        mode
    );
    libexplain_buffer_errno_open_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname,
        flags,
        mode
    );
    libexplain_explanation_assemble(&exp, sb);
}
