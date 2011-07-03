/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011 Peter Miller
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
#include <libexplain/buffer/v4l2_bt_timings.h>
#include <libexplain/buffer/v4l2_dv_timings.h>
#include <libexplain/buffer/v4l2_dv_timings_type.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/is_efault.h>
#include <libexplain/sizeof.h>


#ifdef HAVE_LINUX_VIDEODEV2_H
#if defined(VIDIOC_G_DV_TIMINGS) || defined(VIDIOC_S_DV_TIMINGS)

void
explain_buffer_v4l2_dv_timings(explain_string_buffer_t *sb,
    const struct v4l2_dv_timings *data)
{
    if (explain_is_efault_pointer(data, sizeof(*data)))
    {
        explain_buffer_pointer(sb, data);
        return;
    }

    explain_string_buffer_puts(sb, "{ type = ");
    explain_buffer_v4l2_dv_timings_type(sb, data->type);
    switch (data->type)
    {
#ifdef V4L2_DV_BT_656_1120
    case V4L2_DV_BT_656_1120:
        explain_string_buffer_puts(sb, ", bt = ");
        explain_buffer_v4l2_bt_timings(sb, &data->bt);
        break;
#endif

    default:
        if (!explain_uint32_array_all_zero(data->reserved,
            SIZEOF(data->reserved)))
        {
            explain_string_buffer_puts(sb, ", reserved = ");
            explain_buffer_uint32_array(sb, data->reserved,
                SIZEOF(data->reserved));
        }
        break;
    }
    explain_string_buffer_puts(sb, " }");
}

#endif
#endif

/* vim: set ts=8 sw=4 et : */
