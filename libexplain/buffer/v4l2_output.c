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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/linux/videodev2.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/int32_t.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/v4l2_output.h>
#include <libexplain/buffer/v4l2_output_capabilities.h>
#include <libexplain/buffer/v4l2_output_type.h>
#include <libexplain/buffer/v4l2_std_id.h>
#include <libexplain/is_efault.h>
#include <libexplain/sizeof.h>


#ifdef HAVE_LINUX_VIDEODEV2_H

void
explain_buffer_v4l2_output(explain_string_buffer_t *sb,
    const struct v4l2_output *data, int extra)
{
    if (explain_is_efault_pointer(data, sizeof(*data)))
    {
        explain_buffer_pointer(sb, data);
        return;
    }

    explain_string_buffer_puts(sb, "{ index = ");
    explain_buffer_uint32_t(sb, data->index);
    if (extra)
    {
        explain_string_buffer_puts(sb, ", name = ");
        explain_string_buffer_putsu_quoted_n(sb, data->name,
            sizeof(data->name));
        explain_string_buffer_puts(sb, ", type = ");
        explain_buffer_v4l2_output_type(sb, data->type);
        explain_string_buffer_puts(sb, ", audioset = ");
        explain_buffer_uint32_t(sb, data->audioset); /* FIXME: bitset */
        explain_string_buffer_puts(sb, ", modulator = ");
        explain_buffer_uint32_t(sb, data->modulator);
        explain_string_buffer_puts(sb, ", std = ");
        explain_buffer_v4l2_std_id(sb, data->std);
#ifdef V4L2_OUT_CAP_PRESETS
        if (data->capabilities)
        {
            explain_string_buffer_puts(sb, ", capabilities = ");
            explain_buffer_v4l2_output_capabilities(sb, data->capabilities);
        }
#endif
        if (!explain_uint32_array_all_zero(data->reserved,
            SIZEOF(data->reserved)))
        {
            explain_string_buffer_puts(sb, ", reserved = ");
            explain_buffer_uint32_array(sb, data->reserved,
                SIZEOF(data->reserved));
        }
    }
    explain_string_buffer_puts(sb, " }");
}


void
explain_buffer_v4l2_output_index(explain_string_buffer_t *sb, int value,
    int fildes)
{
    struct v4l2_output in;

    explain_buffer_uint32_t(sb, value);

    memset(&in, 0, sizeof(in));
    in.index = value;
    if (ioctl(fildes, VIDIOC_ENUMOUTPUT, &in) >= 0)
    {
        explain_string_buffer_putc(sb, ' ');
        explain_string_buffer_putsu_quoted_n(sb, in.name, sizeof(in.name));
    }
}


void
explain_buffer_v4l2_output_index_ptr(explain_string_buffer_t *sb,
    const int *data, int fildes)
{
    if (explain_is_efault_pointer(data, sizeof(*data)))
    {
        explain_buffer_pointer(sb, data);
        return;
    }

    explain_string_buffer_puts(sb, "{ ");
    explain_buffer_v4l2_output_index(sb, *data, fildes);
    explain_string_buffer_puts(sb, " }");
}


int
explain_v4l2_output_get_noutputs(int fildes)
{
    int lo = 0;
    int hi = 200;
    for (;;)
    {
        int mid = (lo + hi) / 2;
        struct v4l2_output qry;
        memset(&qry, 0, sizeof(qry));
        qry.index = mid;
        if (ioctl(fildes, VIDIOC_ENUMOUTPUT, &qry) >= 0)
        {
            if (hi <= 0 && lo <= 0)
                return -1;
            /* mid < noutputs */
            lo = mid + 1;
            if (lo > hi)
                return lo;
        }
        else if (errno != EINVAL)
        {
            return -1;
        }
        else
        {
            if (hi <= 0 && lo <= 0)
                return -1;
            /* mid >= noutputs */
            hi = mid - 1;
            if (lo >= hi)
                return lo;
        }
    }
}

#endif

/* vim: set ts=8 sw=4 et : */
