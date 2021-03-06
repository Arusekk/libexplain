/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011, 2013 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/linux/videodev2.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/enotsup.h>
#include <libexplain/buffer/is_the_null_pointer.h>
#include <libexplain/buffer/v4l2_std_id.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/vidioc_s_std.h>
#include <libexplain/is_efault.h>

#ifdef VIDIOC_S_STD


static void
print_data(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    explain_buffer_v4l2_std_id_ptr(sb, data);
}


static void
print_explanation(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    switch (errnum)
    {
    case EINVAL:
        if (!data)
        {
            explain_buffer_is_the_null_pointer(sb, "data");
            return;
        }

        {
            const v4l2_std_id *arg;
            int             j;

            arg = data;
            if (explain_is_efault_pointer(arg, sizeof(*arg)))
                goto generic;

            /*
             * Enumerate all the supported standards, to determine whether or
             * not the given standard is amongst those supported.
             */
            for (j = 0; ; ++j)
            {
                struct v4l2_standard std;

                memset(&std, 0, sizeof(std));
                std.index = j;
                if (ioctl(fildes, VIDIOC_ENUMSTD, &std) < 0)
                {
                    if (j == 0)
                        explain_buffer_einval_no_vid_std(sb);
                    else
                        explain_buffer_enotsup_device(sb, "*data");
                    return;
                }
                if (*arg == std.id)
                    break;
            }
        }
        /*
         * To get to here, the output video standard IS supported,
         * so we have no idea why there was a problem.
         *
         * Fall through...
         */

    default:
        generic:
        explain_iocontrol_generic_print_explanation
        (
            p,
            sb,
            errnum,
            fildes,
            request,
            data
        );
        break;
    }
}


const explain_iocontrol_t explain_iocontrol_vidioc_s_std =
{
    "VIDIOC_S_STD", /* name */
    VIDIOC_S_STD, /* value */
    0, /* disambiguate */
    0, /* print_name */
    print_data,
    print_explanation,
    0, /* print_data_returned */
    sizeof(v4l2_std_id), /* data_size */
    "v4l2_std_id *", /* data_type */
    0, /* flags */
    __FILE__,
    __LINE__,
};

#else /* ndef VIDIOC_S_STD */

const explain_iocontrol_t explain_iocontrol_vidioc_s_std =
{
    0, /* name */
    0, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
    0, /* print_data_returned */
    0, /* data_size */
    0, /* data_type */
    0, /* flags */
    __FILE__,
    __LINE__,
};

#endif /* VIDIOC_S_STD */

/* vim: set ts=8 sw=4 et : */
