/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011, 2013 Peter Miller
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

#include <libexplain/ac/linux/videodev2.h>

#include <libexplain/buffer/int32_t.h>
#include <libexplain/buffer/kernel_version.h>
#include <libexplain/buffer/v4l2_capability.h>
#include <libexplain/buffer/v4l2_capabilities.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/is_efault.h>
#include <libexplain/sizeof.h>


#ifdef HAVE_LINUX_VIDEODEV2_H

void
explain_buffer_v4l2_capability(explain_string_buffer_t *sb,
    const struct v4l2_capability *data)
{
    if (explain_is_efault_pointer(data, sizeof(*data)))
    {
        explain_buffer_pointer(sb, data);
        return;
    }

    explain_string_buffer_puts(sb, "{ driver = ");
    explain_string_buffer_putsu_quoted_n(sb, data->driver,
        sizeof(data->driver));
    explain_string_buffer_puts(sb, ", card = ");
    explain_string_buffer_putsu_quoted_n(sb, data->card, sizeof(data->card));
    explain_string_buffer_puts(sb, ", bus_info = ");
    explain_string_buffer_putsu_quoted_n(sb, data->bus_info,
        sizeof(data->bus_info));
    explain_string_buffer_puts(sb, ", version = ");
    explain_buffer_kernel_version(sb, data->version);
    explain_string_buffer_puts(sb, ", capabilities = ");
    explain_buffer_v4l2_capabilities(sb, data->capabilities);
    if (!explain_uint32_array_all_zero(data->reserved, SIZEOF(data->reserved)))
    {
        explain_string_buffer_puts(sb, ", reserved = ");
        explain_buffer_uint32_array(sb, data->reserved, SIZEOF(data->reserved));
    }
    explain_string_buffer_puts(sb, " }");
}

#endif


/* vim: set ts=8 sw=4 et : */
