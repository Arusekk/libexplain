/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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
#include <libexplain/buffer/eacces.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/access.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/etxtbsy.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/path_to_pid.h>
#include <libexplain/buffer/pathname.h>
#include <libexplain/dirname.h>
#include <libexplain/explanation.h>
#include <libexplain/have_permission.h>
#include <libexplain/pathname_is_a_directory.h>


static void
libexplain_buffer_errno_access_call(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, int mode)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "access(pathname = ");
    libexplain_buffer_pathname(sb, pathname);
    libexplain_string_buffer_puts(sb, ", mode = ");
    libexplain_buffer_access_mode(sb, mode);
    libexplain_string_buffer_putc(sb, ')');
}


void
libexplain_buffer_errno_access_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, int mode)
{
    libexplain_final_t final_component;

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
        if (libexplain_pathname_is_a_directory(pathname))
            final_component.want_to_search = 1;
        else
            final_component.want_to_execute = 1;
    }

    /*
     * The Linux access(2) man page says
     *
     *     The check is done using the process's real UID and
     *     GID, rather than the effective IDs as is done when actually
     *     attempting an operation (e.g., open(2)) on the file.  This
     *     allows set-user-ID programs to easily determine the invoking
     *     user's authority.
     *
     * we will warn them about this later.
     */
    final_component.id.uid = getuid();
    final_component.id.gid = getgid();

    switch (errnum)
    {
    case EACCES:
        libexplain_buffer_eacces(sb, pathname, "pathname", &final_component);

        /*
         * If they asked for more than one permission, explain that they
         * must all succeed.  (Obviously, to get this error, at least
         * one failed.)
         */
        if (mode != (mode & -mode))
        {
            libexplain_string_buffer_puts(sb, "; ");
            libexplain_buffer_gettext
            (
                sb,
                /*
                 * xgettext: This message is used when supplementing an
                 * EACCES error returned by the access(2) system call,
                 * to remind users that it is an error if ANY of the
                 * access types in mode are denied, even if some of the
                 * other access types in mode would be permitted.
                 */
                i18n("note that it is an error if any of the access "
                "types in mode are denied, even if some of the other "
                "access types in mode would be permitted")
            );
        }

        if (getuid() != geteuid() || getgid() != getgid())
        {
            libexplain_string_buffer_puts(sb, "; ");
            libexplain_buffer_gettext
            (
                sb,
                /*
                 * xgettext: This message is used when supplementing
                 * and explanation for an EACCES error reported by
                 * an access(2) system call, in the case where the
                 * effective ID does not match the actual ID.
                 *
                 * This text taken from the Linux access(2) man page.
                 */
                i18n("warning: using access(2) to check if a user is "
                "authorized, for example to verify a file before actually "
                "using open(2), creates a security hole, because an attacker "
                "might exploit the short time interval between checking "
                "the file and opening the file to manipulate it; for this "
                "reason, this use of access(2) should be avoided")
            );

            /*
             * FIXME: should we mention setresuid as a better way?
             */
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
        libexplain_buffer_enotdir(sb, pathname, "pathname", &final_component);
        break;

    case EROFS:
        libexplain_buffer_erofs(sb, pathname, "pathname");
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "pathname");
        break;

    case EINVAL:
        libexplain_buffer_einval_bits(sb, "mode");
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
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_access(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, int mode)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_access_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        mode
    );
    libexplain_buffer_errno_access_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname,
        mode
    );
    libexplain_explanation_assemble(&exp, sb);
}
