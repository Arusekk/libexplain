/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/access_mode.h>
#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/access.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/path_to_pid.h>
#include <libexplain/buffer/strerror.h>
#include <libexplain/buffer/success.h>
#include <libexplain/dirname.h>
#include <libexplain/have_permission.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/string_buffer.h>


void
libexplain_buffer_errno_access(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, int mode)
{
    int             bad_pathname;

    bad_pathname = errnum == EFAULT && libexplain_path_is_efault(pathname);
    libexplain_string_buffer_puts(sb, "access(pathname = ");
    if (bad_pathname)
        libexplain_string_buffer_printf(sb, "%p", pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_puts(sb, ", mode = ");
    libexplain_buffer_access_mode(sb, mode);
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_string_buffer_puts(sb, ": ");
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_string_buffer_puts(sb, " failed, ");
    libexplain_buffer_strerror(sb, errnum);

    /*
     * The Linux access(2) man page says
     *
     *     The check is done using the calling process's real UID and
     *     GID, rather than the effective IDs as is done when actually
     *     attempting an operation (e.g., open(2)) on the file.  This
     *     allows set-user-ID programs to easily determine the invoking
     *     user's authority.
     *
     * FIXME: should we parameterize the path_resolution function to
     * accomodate this test?  Especially when it is a bad idea:
     *
     *     Warning:  Using access() to check if a user is authorized
     *     to, for example, open a file before actually doing so using
     *     open(2) creates a security hole, because the user might
     *     exploit the short time interval between checking and opening
     *     the file to manipulate it.  For this reason, this use of this
     *     system call should be avoided.
     */
    if (getuid() != geteuid() || getgid() != getgid())
    {
        libexplain_string_buffer_puts
        (
            sb,
            "; warning: using access(2) to check if a user is "
            "authorized, for example to open a file before actually "
            "using open(2), creates a security hole, because the user "
            "might exploit the short time interval between checking "
            "and opening the file to manipulate it; for this reason, "
            "this use of access(2) should be avoided"
        );
        return;
    }

    switch (errnum)
    {
    case EACCES:
        {
            struct stat     st;
            char            dir[PATH_MAX + 1];

            libexplain_buffer_because(sb);
            libexplain_dirname(dir, pathname, sizeof(dir));
            if
            (
                libexplain_buffer_errno_path_resolution
                (
                    sb,
                    errnum,
                    dir,
                    O_RDONLY + O_DIRECTORY,
                    "pathname"
                )
            >=
                0
            )
                break;

            if (lstat(pathname, &st) >= 0)
            {
                int             available;
                int             first;

                available = 0;
                if (libexplain_have_read_permission(&st))
                    available |= R_OK;
                if (libexplain_have_write_permission(&st))
                    available |= W_OK;
                if (S_ISDIR(st.st_mode))
                {
                    if (libexplain_have_search_permission(&st))
                        available |= X_OK;
                }
                else
                {
                    if (libexplain_have_execute_permission(&st))
                        available |= X_OK;
                }
                first = 1;
                if ((mode & R_OK) && !(available & R_OK))
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "read access to pathname would be denied"
                    );
                    first = 0;
                }
                if ((mode & W_OK) && !(available & W_OK))
                {
                    if (!first)
                        libexplain_string_buffer_puts(sb, ", ");
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "write access to pathname would be denied"
                    );
                    first = 0;
                }
                if ((mode & X_OK) && !(available & X_OK))
                {
                    if (!first)
                        libexplain_string_buffer_puts(sb, ", ");
                    if (S_ISDIR(st.st_mode))
                    {
                        libexplain_string_buffer_puts
                        (
                            sb,
                            "search access to pathname would be denied"
                        );
                    }
                    else
                    {
                        libexplain_string_buffer_puts
                        (
                            sb,
                            "execute access to pathname would be denied"
                        );
                    }
                    first = 0;
                }
                if (mode != (mode & -mode))
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "; note that it is an error if any of the access "
                        "types in mode are denied, even if some of the other "
                        "access types in mode are permitted"
                    );
                }
            }
            else
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "the requested access to pathname would be denied"
                );
            }
        }
        break;

    case ELOOP:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                O_RDONLY,
                "pathname"
            )
        )
        {
            /*
             * Unable to find a specific cause,
             * emit the generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "too many symbolic links were encountered in resolving "
                "pathname"
            );
        }
        break;

    case ENAMETOOLONG:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                O_RDONLY,
                "pathname"
            )
        )
        {
            /*
             * Unable to find a specific cause,
             * emit the generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "pathname, or a component of pathname, is too long"
            );
        }
        break;

    case ENOENT:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                O_RDONLY,
                "pathname"
            )
        )
        {
            /*
             * Unable to find a specific cause,
             * emit the generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "a component of pathname does not exist or is a "
                "dangling symbolic link"
            );
        }
        break;

    case ENOTDIR:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                O_RDONLY,
                "pathname"
            )
        )
        {
            /*
             * Unable to find a specific cause,
             * emit the generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "a component used as a directory in pathname is not, in "
                "fact, a directory                                     "
            );
        }
        break;

    case EROFS:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "write access was requested for pathname that is on a "
            "read-only filesystem"
        );
        libexplain_buffer_mount_point(sb, pathname);
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "pathname");
        break;

    case EINVAL:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "mode was incorrectly specified"
        );
        break;

    case EIO:
        libexplain_buffer_eio_path(sb, pathname);
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    case ETXTBSY:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "write access was requested to an executable which is being "
            "executed"
        );
        libexplain_buffer_path_to_pid(sb, pathname);
        break;

    default:
        /* no explanation for any other error */
        break;
    }
}
