/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/ac/linux/cdrom.h>

#include <libexplain/buffer/cdrom_addr.h>
#include <libexplain/buffer/cdrom_addr_format.h>
#include <libexplain/buffer/cdrom_read_audio.h>
#include <libexplain/buffer/int.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/parse_bits.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/sizeof.h>


#ifdef HAVE_LINUX_CDROM_H

void
explain_buffer_cdrom_read_audio(explain_string_buffer_t *sb,
    const struct cdrom_read_audio *data)
{
    if (explain_pointer_is_efault(data, sizeof(*data)))
    {
        explain_buffer_pointer(sb, data);
        return;
    }

    explain_string_buffer_puts(sb, "{ addr = ");
    explain_buffer_cdrom_addr(sb, &data->addr, data->addr_format);
    explain_string_buffer_puts(sb, ", addr_format = ");
    explain_buffer_cdrom_addr_format(sb, data->addr_format);
    explain_string_buffer_puts(sb, ", nframes = ");
    explain_buffer_int(sb, data->nframes);
    explain_string_buffer_puts(sb, ", buf = ");
    explain_buffer_pointer(sb, data->buf);
    explain_string_buffer_puts(sb, " }");
}

#else

void
explain_buffer_cdrom_read_audio(explain_string_buffer_t *sb,
    const struct cdrom_read_audio *data)
{
    explain_buffer_pointer(sb, data);
}

#endif
