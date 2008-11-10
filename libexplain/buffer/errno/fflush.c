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
#include <libexplain/ac/signal.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/errno/fflush.h>
#include <libexplain/buffer/errno/write.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/success.h>
#include <libexplain/open_flags.h>


void
libexplain_buffer_errno_fflush(libexplain_string_buffer_t *sb, int errnum,
    FILE *fp)
{
    int             fildes;

    fildes = -1;
    if (fp)
        fildes = fileno(fp);

    libexplain_string_buffer_printf(sb, "fflush(fp = %p", fp);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_buffer_failed(sb, errnum);

    libexplain_buffer_errno_fflush_because(sb, errnum, fp);
}


void
libexplain_buffer_errno_fflush_because(libexplain_string_buffer_t *sb,
    int errnum, FILE *fp)
{
    int             fildes;

    if (!fp)
    {
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts(sb, "fp is the NULL pointer");
        return;
    }
    fildes = fileno(fp);

    libexplain_buffer_errno_write(sb, errnum, fildes, (void *)0, 0);
}
