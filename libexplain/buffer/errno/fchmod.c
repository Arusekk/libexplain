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
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/fchmod.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/success.h>
#include <libexplain/buffer/uid.h>
#include <libexplain/capability.h>
#include <libexplain/option.h>
#include <libexplain/permission_mode.h>


void
libexplain_buffer_errno_fchmod(libexplain_string_buffer_t *sb, int errnum,
    int fildes, int mode)
{
    libexplain_string_buffer_printf(sb, "fchmod(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_puts(sb, ", mode = ");
    libexplain_buffer_permission_mode(sb, mode);
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_buffer_failed(sb, errnum);

    switch (errnum)
    {
    case EBADF:
        libexplain_buffer_ebadf(sb, "fildes");
        break;

    case EIO:
        libexplain_buffer_eio_fildes(sb, fildes);
        break;

    case EPERM:
        {
            libexplain_buffer_because(sb);
            libexplain_string_buffer_puts(sb, "the effective UID");
            if (libexplain_option_dialect_specific())
            {
                libexplain_string_buffer_puts(sb, " (");
                libexplain_buffer_uid(sb, geteuid());
                libexplain_string_buffer_putc(sb, ')');
            }
            libexplain_string_buffer_puts
            (
                sb,
                " does not match the owner of the file"
            );
            if (libexplain_option_dialect_specific())
            {
                struct stat     st;

                if (fstat(fildes, &st) >= 0)
                {
                    libexplain_string_buffer_puts(sb, " (");
                    libexplain_buffer_uid(sb, st.st_uid);
                    libexplain_string_buffer_putc(sb, ')');
                }
            }
            libexplain_string_buffer_puts
            (
                sb,
                ", and the process is not privileged"
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
        libexplain_buffer_erofs_fildes(sb, fildes, "fildes");
        break;

    default:
        /* no explanation for other errors */
        break;
    }
}
