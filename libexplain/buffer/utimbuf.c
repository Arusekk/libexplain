/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/utime.h>

#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/utimbuf.h>
#include <libexplain/path_is_efault.h>


void
explain_buffer_utimbuf(explain_string_buffer_t *sb,
    const struct utimbuf *buf)
{
    if (explain_pointer_is_efault(buf, sizeof(*buf)))
    {
        explain_buffer_pointer(sb, buf);
        return;
    }
    explain_string_buffer_printf(sb, "{ actime = %ld", (long)buf->actime);
    explain_string_buffer_printf(sb, ", modtime = %ld", (long)buf->modtime);
    explain_string_buffer_puts(sb, " }");
}
