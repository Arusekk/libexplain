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

#include <libexplain/ac/sys/mtio.h>

#include <libexplain/buffer/mtget.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/path_is_efault.h>


void
libexplain_buffer_mtget(libexplain_string_buffer_t *sb,
    const struct mtget *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
        libexplain_buffer_pointer(sb, data);
    else
    {
        libexplain_string_buffer_printf(sb, "{ mt_type = %ld, ", data->mt_type);
        libexplain_string_buffer_printf(sb, "mt_resid = %ld, ", data->mt_resid);
        libexplain_string_buffer_printf(sb, "mt_dsreg = %ld, ", data->mt_dsreg);
        libexplain_string_buffer_printf(sb, "mt_gstat = %ld, ", data->mt_gstat);
        libexplain_string_buffer_printf(sb, "mt_erreg = %ld, ", data->mt_erreg);
        libexplain_string_buffer_printf
        (
            sb,
            "mt_fileno = %ld, ",
            (long)data->mt_fileno
        );
        libexplain_string_buffer_printf
        (
            sb,
            "mt_blkno = %ld }",
            (long)data->mt_blkno
        );
    }
}
