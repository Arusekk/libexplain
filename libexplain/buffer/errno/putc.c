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

#include <libexplain/buffer/char.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/putc.h>
#include <libexplain/buffer/errno/write.h>
#include <libexplain/buffer/stream.h>
#include <libexplain/explanation.h>
#include <libexplain/stream_to_fildes.h>


static void
libexplain_buffer_errno_putc_system_call(libexplain_string_buffer_t *sb,
    int errnum, int c, FILE *fp)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "putc(c = ");
    libexplain_buffer_char(sb, c);
    libexplain_string_buffer_puts(sb, ", fp = ");
    libexplain_buffer_stream(sb, fp);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_putc_explanation(libexplain_string_buffer_t *sb,
    int errnum, int c, FILE *fp)
{
    int             fildes;

    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/putc.html
     */
    (void)c;
    fildes = libexplain_stream_to_fildes(fp);
    libexplain_buffer_errno_write_explanation(sb, errnum, fildes, NULL, 0);
}


void
libexplain_buffer_errno_putc(libexplain_string_buffer_t *sb, int errnum, int c,
    FILE *fp)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_putc_system_call
    (
        &exp.system_call_sb,
        errnum,
        c,
        fp
    );
    libexplain_buffer_errno_putc_explanation
    (
        &exp.explanation_sb,
        errnum,
        c,
        fp
    );
    libexplain_explanation_assemble(&exp, sb);
}
