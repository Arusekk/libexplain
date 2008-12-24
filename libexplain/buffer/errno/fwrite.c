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

#include <libexplain/buffer/errno/fwrite.h>
#include <libexplain/buffer/errno/write.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/stream.h>
#include <libexplain/explanation.h>
#include <libexplain/stream_to_fildes.h>


static void
libexplain_buffer_errno_fwrite_system_call(libexplain_string_buffer_t *sb,
    int errnum, const void *ptr, size_t size, size_t nmemb, FILE *fp)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "fwrite(ptr = ");
    libexplain_buffer_pointer(sb, ptr);
    libexplain_string_buffer_printf(sb, ", size = %ld", (long)size);
    libexplain_string_buffer_printf(sb, ", nmemb = %ld", (long)nmemb);
    libexplain_string_buffer_puts(sb, ", fp = ");
    libexplain_buffer_stream(sb, fp);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_fwrite_explanation(libexplain_string_buffer_t *sb,
    int errnum, const void *ptr, size_t size, size_t nmemb, FILE *fp)
{
    int             fildes;

    fildes = libexplain_stream_to_fildes(fp);
    libexplain_buffer_errno_write_explanation
    (
        sb,
        errnum,
        fildes,
        ptr,
        size * nmemb
    );
}


void
libexplain_buffer_errno_fwrite(libexplain_string_buffer_t *sb, int errnum,
    const void *ptr, size_t size, size_t nmemb, FILE *fp)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_fwrite_system_call
    (
        &exp.system_call_sb,
        errnum,
        ptr,
        size,
        nmemb,
        fp
    );
    libexplain_buffer_errno_fwrite_explanation
    (
        &exp.explanation_sb,
        errnum,
        ptr,
        size,
        nmemb,
        fp
    );
    libexplain_explanation_assemble(&exp, sb);
}
