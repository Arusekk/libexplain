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

#include <libexplain/buffer/mtconfiginfo.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/path_is_efault.h>


void
libexplain_buffer_mtconfiginfo(libexplain_string_buffer_t *sb,
    const struct mtconfiginfo *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
        libexplain_buffer_pointer(sb, data);
    else
    {
        libexplain_string_buffer_printf(sb, "{ mt_type = %ld, ", data->mt_type);
#ifdef __linux__
        libexplain_string_buffer_printf(sb, "ifc_type = %ld, ", data->ifc_type);
        libexplain_string_buffer_printf(sb, "irqnr = %u, ", data->irqnr);
        libexplain_string_buffer_printf(sb, "dmanr = %u, ", data->dmanr);
        libexplain_string_buffer_printf(sb, "port = %u, ", data->port);
        libexplain_string_buffer_printf(sb, "debug = %#lx, ", data->debug);
        libexplain_string_buffer_printf
        (
            sb,
            "have_dens = %d, ",
            data->have_dens
        );
        libexplain_string_buffer_printf(sb, "have_bsf = %d, ", data->have_bsf);
        libexplain_string_buffer_printf(sb, "have_fsr = %d, ", data->have_fsr);
        libexplain_string_buffer_printf(sb, "have_bsr = %d, ", data->have_bsr);
        libexplain_string_buffer_printf(sb, "have_eod = %d, ", data->have_eod);
        libexplain_string_buffer_printf
        (
            sb,
            "have_seek = %d, ",
            data->have_seek
        );
        libexplain_string_buffer_printf
        (
            sb,
            "have_tell = %d, ",
            data->have_tell
        );
        libexplain_string_buffer_printf
        (
            sb,
            "have_ras1 = %d, ",
            data->have_ras1
        );
        libexplain_string_buffer_printf
        (
            sb,
            "have_ras2 = %d, ",
            data->have_ras2
        );
        libexplain_string_buffer_printf
        (
            sb,
            "have_ras3 = %d, ",
            data->have_ras3
        );
        libexplain_string_buffer_printf
        (
            sb,
            "have_qfa = %d, ",
            data->have_qfa
        );
#endif
        libexplain_string_buffer_puts(sb, " }");
    }
}
