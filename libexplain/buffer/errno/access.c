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
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/access_mode.h>
#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/access.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/etxtbsy.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/path_to_pid.h>
#include <libexplain/buffer/success.h>
#include <libexplain/dirname.h>
#include <libexplain/have_permission.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/string_buffer.h>


static int
is_dir(const char *pathname)
{
    struct stat     st;

    return (stat(pathname, &st) && S_ISDIR(st.st_mode));
}


void
libexplain_buffer_errno_access(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, int mode)
{
    int             bad_pathname;
    libexplain_final_t final_component;

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
    libexplain_buffer_failed(sb, errnum);

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

    /*
     * Translate the mode into final component flags.
     */
    libexplain_final_init(&final_component);
    if (mode & R_OK)
        final_component.want_to_read = 1;
    if (mode & W_OK)
        final_component.want_to_write = 1;
    if (mode & X_OK)
    {
        if (!bad_pathname && is_dir(pathname))
            final_component.want_to_search = 1;
        else
            final_component.want_to_execute = 1;
    }

    switch (errnum)
    {
    case EACCES:
        libexplain_buffer_because(sb);
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
                "the requested access to pathname would be denied"
            );
        }

        /*
         * If they asked for more than one permission, explain that they
         * must all succeed.  (Obviously, to get this error, at least
         * one failed.)
         */
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
        break;

    case ELOOP:
    case EMLINK: /* BSD */
        libexplain_buffer_eloop(sb, pathname, "pathname", &final_component);
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

    case ENOENT:
        libexplain_buffer_enoent(sb, pathname, "pathname", &final_component);
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
                "pathname",
                &final_component
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
        libexplain_buffer_erofs(sb, pathname, "pathname");
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
        libexplain_buffer_etxtbsy(sb, pathname);
        break;

    default:
        /* no explanation for any other error */
        break;
    }
}
