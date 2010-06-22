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

#include <libexplain/buffer/cdrom_blk.h>
#include <libexplain/buffer/int.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/path_is_efault.h>


#ifdef HAVE_LINUX_CDROM_BLK_H

void
explain_buffer_cdrom_blk(explain_string_buffer_t *sb,
    const struct cdrom_blk *data)
{
    if (explain_pointer_is_efault(data, sizeof(*data)))
    {
        explain_buffer_pointer(sb, data);
        return;
    }

    explain_string_buffer_puts(sb, "{ from = ");
    explain_buffer_long(sb, data->from);
    explain_string_buffer_puts(sb, ", len = ");
    explain_buffer_long(sb, data->len);
    explain_string_buffer_puts(sb, " }");
}

#else

void
explain_buffer_cdrom_blk(explain_string_buffer_t *sb,
    const struct cdrom_blk *data)
{
    explain_buffer_pointer(sb, data);
}

#endif
