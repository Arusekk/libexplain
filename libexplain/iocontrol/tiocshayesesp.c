/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/sys/ioctl.h>
#include <libexplain/ac/linux/hayesesp.h>

#include <libexplain/buffer/argument_is_invalid.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/tiocshayesesp.h>
#include <libexplain/path_is_efault.h>


#ifdef TIOCSHAYESESP

static void
libexplain_buffer_hayes_esp_config(libexplain_string_buffer_t *sb,
    const struct hayes_esp_config *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
        libexplain_buffer_pointer(sb, data);
    else
    {
        libexplain_string_buffer_printf
        (
            sb,
            "{ flow_on = %d, "
                "flow_off = %d, "
                "rx_trigger = %d, "
                "tx_trigger = %d, "
                "pio_threshold = %d, "
                "rx_timeout = %d, "
                "dma_channel = %d "
                "}",
            data->flow_on,
            data->flow_off,
            data->rx_trigger,
            data->tx_trigger,
            data->pio_threshold,
            data->rx_timeout,
            data->dma_channel
        );
    }
}


static void
print_data(const libexplain_iocontrol_t *p, libexplain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    libexplain_buffer_hayes_esp_config(sb, data);
}


static void
print_explanation(const libexplain_iocontrol_t *p,
    libexplain_string_buffer_t *sb, int errnum, int fildes, int request,
    const void *data)
{
    switch (errnum)
    {
    case EBUSY:
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This message is used to explain an EBUSY
             * error reported by an ioctl TIOCSHAYESESP system call.
             */
            i18n("the port is in use")
        );
        break;

    case EINVAL:
        {
            const struct hayes_esp_config *q;

            q = data;
            if (q->flow_on >= q->flow_off)
            {
                libexplain_buffer_argument_is_invalid
                (
                    sb,
                    "data->flow_on",
                    q->flow_on
                );
                break;
            }
            if (q->rx_trigger < 1 || q->rx_trigger > 1023)
            {
                libexplain_buffer_argument_is_invalid
                (
                    sb,
                    "data->rx_trigger",
                    q->rx_trigger
                );
                break;
            }
            if (q->tx_trigger > 1023)
            {
                libexplain_buffer_argument_is_invalid
                (
                    sb,
                    "data->tx_trigger",
                    q->tx_trigger
                );
                break;
            }
            if (q->flow_off < 1 || q->flow_off > 1023)
            {
                libexplain_buffer_argument_is_invalid
                (
                    sb,
                    "data->flow_off",
                    q->flow_off
                );
                break;
            }
            if (q->flow_on < 1 || q->flow_on > 1023)
            {
                libexplain_buffer_argument_is_invalid
                (
                    sb,
                    "data->flow_on",
                    q->flow_on
                );
                break;
            }
            if (q->pio_threshold < 1 || q->pio_threshold > 1024)
            {
                libexplain_buffer_argument_is_invalid
                (
                    sb,
                    "data->pio_threshold",
                    q->pio_threshold
                );
                break;
            }
        }
        break;

    default:
        libexplain_iocontrol_generic.print_explanation
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


const libexplain_iocontrol_t libexplain_iocontrol_tiocshayesesp =
{
    "TIOCSHAYESESP", /* name */
    TIOCSHAYESESP, /* value */
    0, /* disambiguate */
    0, /* print_name */
    print_data,
    print_explanation
};

#else

const libexplain_iocontrol_t libexplain_iocontrol_tiocshayesesp =
{
    0, /* name */
    -1, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
};

#endif
