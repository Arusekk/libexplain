/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011 Peter Miller
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
#include <libexplain/buffer/v4l2_control.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/vidioc_s_ctrl.h>
#include <libexplain/is_efault.h>

#ifdef VIDIOC_S_CTRL


static void
print_data(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)request;
    explain_buffer_v4l2_control(sb, data, 1, fildes);
}


static void
print_explanation(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    switch (errnum)
    {
    case EIO:
        explain_string_buffer_puts
        (
            sb,
            /* FIXME: i18n */
            "writing a device register failed"
        );
        return;

    case EINVAL:
        {
            const struct v4l2_control *arg;
            struct v4l2_queryctrl ctrl;

            arg = data;
            if (explain_is_efault_pointer(arg, sizeof(*arg)))
                goto generic;

            /*
             * Check the the control id.
             */
            memset(&ctrl, 0, sizeof(ctrl));
            ctrl.id = arg->id;
            if (ioctl(fildes, VIDIOC_QUERYCTRL, &ctrl) < 0)
            {
                if (errno != EINVAL)
                    goto generic;
                explain_buffer_einval_vague(sb, "data->id");
                return;
            }

            /*
             * Check the control value.
             */
            if (arg->value < ctrl.minimum || arg->value > ctrl.maximum)
            {
                explain_buffer_einval_out_of_range
                (
                    sb,
                    "data->value",
                    ctrl.minimum,
                    ctrl.maximum
                );
                return;
            }
            if (ctrl.step >= 2 && (arg->value - ctrl.minimum) % ctrl.step != 0)
            {
                explain_buffer_einval_multiple(sb, "data->value", ctrl.step);
                return;
            }
        }

        /* no idea */
        goto generic;

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


static void
print_data_returned(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)request;
    explain_buffer_v4l2_control(sb, data, 1, fildes);
}


const explain_iocontrol_t explain_iocontrol_vidioc_s_ctrl =
{
    "VIDIOC_S_CTRL", /* name */
    VIDIOC_S_CTRL, /* value */
    0, /* disambiguate */
    0, /* print_name */
    print_data,
    print_explanation,
    print_data_returned,
    sizeof(struct v4l2_control), /* data_size */
    "struct v4l2_control *", /* data_type */
    __FILE__,
    __LINE__,
};

#else /* ndef VIDIOC_S_CTRL */

const explain_iocontrol_t explain_iocontrol_vidioc_s_ctrl =
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
    __FILE__,
    __LINE__,
};

#endif /* VIDIOC_S_CTRL */

/* vim: set ts=8 sw=4 et : */
