/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/fgets.h>
#include <libexplain/buffer/errno/read.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/is_the_null_pointer.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/stream.h>
#include <libexplain/explanation.h>
#include <libexplain/stream_to_fildes.h>


static void
explain_buffer_errno_fgets_system_call(explain_string_buffer_t *sb,
    int errnum, char *data, int data_size, FILE *fp)
{
    (void)errnum;
    explain_string_buffer_puts(sb, "fgets(data = ");
    explain_buffer_pointer(sb, data);
    explain_string_buffer_printf(sb, ", data_size = %d", data_size);
    explain_string_buffer_puts(sb, ", fp = ");
    explain_buffer_stream(sb, fp);
    explain_string_buffer_putc(sb, ')');
}


static void
explain_buffer_errno_fgets_explanation(explain_string_buffer_t *sb,
    int errnum, char *data, int data_size, FILE *fp)
{
    if (!fp)
    {
        explain_buffer_is_the_null_pointer(sb, "fp");
        return;
    }
    explain_buffer_errno_read_explanation
    (
        sb,
        errnum,
        explain_stream_to_fildes(fp),
        data,
        data_size
    );
}


void
explain_buffer_errno_fgets(explain_string_buffer_t *sb, int errnum,
    char *data, int data_size, FILE *fp)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_fgets_system_call
    (
        &exp.system_call_sb,
        errnum,
        data,
        data_size,
        fp
    );
    explain_buffer_errno_fgets_explanation
    (
        &exp.explanation_sb,
        errnum,
        data,
        data_size,
        fp
    );
    explain_explanation_assemble(&exp, sb);
}
