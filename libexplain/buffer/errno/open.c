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
#include <libexplain/ac/errno.h>
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/enfile.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/open.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/etxtbsy.h>
#include <libexplain/buffer/file_type.h>
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
        final_component.must_be_a_directory = 1;
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
            libexplain_string_buffer_puts
            (
                sb,
                "the requested access to the file is not allowed, or "
                "search permission is denied for one of the directories "
                "in the path prefix of pathname, or the file did not "
                "exist yet and write access to the parent directory is "
                "not allowed"
            );
        }
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
            libexplain_string_buffer_puts
            (
                sb,
                "the file already exists when it should not (flags "
                "O_CREAT and O_EXCL were used)"
            );
        }
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "pathname");
        break;

    case EFBIG:
    case EOVERFLOW:
        {
            struct stat     st;

            libexplain_string_buffer_puts
            (
                sb,
                "it is a regular file which is too large to be opened"
            );
            if (stat(pathname, &st) == 0 && S_ISREG(st.st_mode))
            {
                libexplain_string_buffer_puts(sb, " (");
                libexplain_buffer_pretty_size(sb, st.st_size);
                libexplain_string_buffer_putc(sb, ')');
            }
            libexplain_string_buffer_puts
            (
                sb,
                ", the O_LARGEFILE flag is necessary"
            );
        }
        break;

    case EISDIR:
        libexplain_string_buffer_puts
        (
            sb,
            "pathname refers to a directory and the access requested "
            "involved writing (that is, O_WRONLY or O_RDWR is set)"
        );
        break;

    case ELOOP:
    case EMLINK: /* BSD */
        if (flags & O_NOFOLLOW)
        {
            struct stat st;
            if (lstat(pathname, &st) == 0 && S_ISLNK(st.st_mode))
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "O_NOFOLLOW was specified but pathname was a "
                    "symbolic link"
                );
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

    case ENODEV:
        libexplain_string_buffer_puts
        (
            sb,
            "pathname refers to a device special file and no "
            "corresponding device exists.  (This is a Linux kernel bug; "
            "in this situation ENXIO must be returned.)"
        );
        break;

    case ENOENT:
        libexplain_buffer_enoent(sb, pathname, "pathname", &final_component);
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    case ENOSPC:
        libexplain_string_buffer_puts
        (
            sb,
            "the file system containing the pathname"
        );
        libexplain_buffer_mount_point_dirname(sb, pathname);
        libexplain_string_buffer_puts
        (
            sb,
            " has no room for the new file"
            /* has no blocks left or has no indoes left */
        );
        break;

    case ENOTDIR:
        libexplain_buffer_enotdir(sb, pathname, "pathname", &final_component);
        break;

    case ENXIO:
        {
            struct stat st;
            if (stat(pathname, &st) != 0)
            {
                enxio_generic:
                libexplain_string_buffer_puts
                (
                    sb,
                    "O_NONBLOCK | O_WRONLY is set, the named file is a "
                    "FIFO and no process has the file open for reading; "
                    "or, the file is a device special file and no "
                    "corresponding device exists"
                );
                break;
            }
            switch (st.st_mode & S_IFMT)
            {
            case S_IFIFO:
                libexplain_string_buffer_puts
                (
                    sb,
                    "the named file is a FIFO and no process has the "
                    "file open for reading"
                );
                break;

            case S_IFCHR:
            case S_IFBLK:
                libexplain_string_buffer_printf
                (
                    sb,
                    "the file is a device special file (%d, %d) and no "
                        "corresponding hardware device exists",
                    major(st.st_rdev),
                    minor(st.st_rdev)
                );
                break;

            default:
                goto enxio_generic;
            }
        }
        break;

    case EPERM:
        {
            struct stat     st;

            /*
             * The O_NOATIME flag needs privileges.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "the effective user ID of the caller ("
            );
            libexplain_buffer_uid(sb, geteuid());
            libexplain_string_buffer_puts
            (
                sb,
                ") did not match the owner of the file"
            );
            if (stat(pathname, &st) >= 0)
            {
                libexplain_string_buffer_puts(sb, " (");
                libexplain_buffer_uid(sb, st.st_uid);
                libexplain_string_buffer_putc(sb, ')');
            }
            libexplain_buffer_uid(sb, st.st_uid);
            libexplain_string_buffer_puts
            (
                sb,
                ", and the caller was not privileged"
            );
#ifdef HAVE_SYS_CAPABILITY_H
            if (libexplain_option_dialect_specific())
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    " (does not have the CAP_FOWNER capability)"
                );
            }
#endif
        }
        break;

    case EROFS:
        libexplain_buffer_erofs(sb, pathname, "pathname");
        break;

    case ETXTBSY:
        libexplain_buffer_etxtbsy(sb, pathname);
        break;

    case EWOULDBLOCK:
        libexplain_string_buffer_puts
        (
            sb,
            "the O_NONBLOCK flag was specified, and an incompatible "
            "lease was held on the file"
        );
        libexplain_buffer_path_to_pid(sb, pathname);
        break;

    default:
        /* no additional info for other errno values */
        break;
    }
    if ((flags & O_EXCL) && !(flags & O_CREAT))
    {
        libexplain_string_buffer_puts
        (
            sb,
            "; note that the behavior of O_EXCL is undefined if O_CREAT "
            "is not specified"
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
