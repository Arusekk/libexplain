/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010, 2011, 2013 Peter Miller
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
#include <libexplain/ac/net/if_ppp.h>
#include <libexplain/ac/net/ppp-comp.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/enobufs.h>
#include <libexplain/buffer/ppp_option_data.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/pppiocscompress.h>
#include <libexplain/is_efault.h>

#ifdef PPPIOCSCOMPRESS


static void
print_data(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    explain_buffer_ppp_option_data(sb, data);
}


static void
print_explanation(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    const struct ppp_option_data *dp;

    /* Linux: drivers/net/ppp_generic.c */
    dp = data;
    switch (errnum)
    {
    case EFAULT:
        if (explain_is_efault_pointer(dp, sizeof(*dp)))
            explain_buffer_efault(sb, "data");
        else
            explain_buffer_efault(sb, "data->ptr");
        break;

    case EINVAL:
        if (dp->length > CCP_MAX_OPTION_LENGTH)
        {
            explain_buffer_einval_too_large(sb, "data->length");
            break;
        }
        if (dp->ptr[1] < 2)
        {
            explain_buffer_einval_too_small(sb, "data->ptr[1]", dp->ptr[1]);
            break;
        }
        if (dp->ptr[1] > dp->length)
        {
            explain_buffer_einval_too_large(sb, "data->ptr[2]");
            break;
        }
        explain_buffer_einval_vague(sb, "data");
        break;

    case ENOBUFS:
        explain_buffer_enobufs(sb);
        break;

    default:
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


const explain_iocontrol_t explain_iocontrol_pppiocscompress =
{
    "PPPIOCSCOMPRESS", /* name */
    PPPIOCSCOMPRESS, /* value */
    0, /* disambiguate */
    0, /* print_name */
    print_data,
    print_explanation,
    0, /* print_data_returned */
    sizeof(struct ppp_option_data), /* data_size */
    "struct ppp_option_data *", /* data_type */
    0, /* flags */
    __FILE__,
    __LINE__,
};

#else /* ndef PPPIOCSCOMPRESS */

const explain_iocontrol_t explain_iocontrol_pppiocscompress =
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

#endif /* PPPIOCSCOMPRESS */


/* vim: set ts=8 sw=4 et : */
