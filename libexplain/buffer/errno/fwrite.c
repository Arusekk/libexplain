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

#include <libexplain/buffer/errno/fwrite.h>
#include <libexplain/buffer/errno/write.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/stream.h>
#include <libexplain/explanation.h>
#include <libexplain/stream_to_fildes.h>


static void
explain_buffer_errno_fwrite_system_call(explain_string_buffer_t *sb,
    int errnum, const void *ptr, size_t size, size_t nmemb, FILE *fp)
{
    (void)errnum;
    explain_string_buffer_puts(sb, "fwrite(ptr = ");
    explain_buffer_pointer(sb, ptr);
    explain_string_buffer_printf(sb, ", size = %ld", (long)size);
    explain_string_buffer_printf(sb, ", nmemb = %ld", (long)nmemb);
    explain_string_buffer_puts(sb, ", fp = ");
    explain_buffer_stream(sb, fp);
    explain_string_buffer_putc(sb, ')');
}


static void
explain_buffer_errno_fwrite_explanation(explain_string_buffer_t *sb,
    int errnum, const void *ptr, size_t size, size_t nmemb, FILE *fp)
{
    int             fildes;

    fildes = explain_stream_to_fildes(fp);
    explain_buffer_errno_write_explanation
    (
        sb,
        errnum,
        "fwrite",
        fildes,
        ptr,
        size * nmemb
    );
}


void
explain_buffer_errno_fwrite(explain_string_buffer_t *sb, int errnum,
    const void *ptr, size_t size, size_t nmemb, FILE *fp)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_fwrite_system_call
    (
        &exp.system_call_sb,
        errnum,
        ptr,
        size,
        nmemb,
        fp
    );
    explain_buffer_errno_fwrite_explanation
    (
        &exp.explanation_sb,
        errnum,
        ptr,
        size,
        nmemb,
        fp
    );
    explain_explanation_assemble(&exp, sb);
}
