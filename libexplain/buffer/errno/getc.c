/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
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

#include <libexplain/ac/errno.h>

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/errno/getc.h>
#include <libexplain/buffer/errno/read.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/is_the_null_pointer.h>
#include <libexplain/buffer/stream.h>
#include <libexplain/stream_to_fildes.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_getc_system_call(libexplain_string_buffer_t *sb,
    int errnum, FILE *fp)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "getc(fp = ");
    libexplain_buffer_stream(sb, fp);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_getc_explanation(libexplain_string_buffer_t *sb,
    int errnum, FILE *fp)
{
    int             fildes;

    if (!fp)
    {
        libexplain_buffer_is_the_null_pointer(sb, "fp");
        return;
    }

    fildes = libexplain_stream_to_fildes(fp);
    if (errnum == EBADF)
    {
        libexplain_buffer_ebadf(sb, fildes, "fp");
        return;
    }

    libexplain_buffer_errno_read_explanation(sb, errnum, fildes, NULL, 0);
}


void
libexplain_buffer_errno_getc(libexplain_string_buffer_t *sb, int errnum,
    FILE *fp)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_getc_system_call(&exp.system_call_sb, errnum, fp);
    libexplain_buffer_errno_getc_explanation(&exp.explanation_sb, errnum, fp);
    libexplain_explanation_assemble(&exp, sb);
}
