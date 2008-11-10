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
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/success.h>


void
libexplain_buffer_errno_fwrite(libexplain_string_buffer_t *sb, int errnum,
    const void *ptr, size_t size, size_t nmemb, FILE *fp)
{
    int             fildes;

    libexplain_string_buffer_puts(sb, "fwrite(ptr = ");
    libexplain_string_buffer_printf(sb, "%p", ptr);
    libexplain_string_buffer_puts(sb, ", size = ");
    libexplain_string_buffer_printf(sb, "%ld", (long)size);
    libexplain_string_buffer_puts(sb, ", nmemb = ");
    libexplain_string_buffer_printf(sb, "%ld", (long)nmemb);
    libexplain_string_buffer_puts(sb, ", fp = ");
    libexplain_string_buffer_printf(sb, "%p", fp);
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_buffer_failed(sb, errnum);

    fildes = -1;
    if (fp)
        fildes = fileno(fp);

    libexplain_buffer_errno_write_because
    (
        sb,
        errnum,
        fildes,
        ptr,
        size * nmemb
    );
}
