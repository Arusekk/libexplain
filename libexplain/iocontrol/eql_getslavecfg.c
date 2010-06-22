/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/sys/ioctl.h>
#include <libexplain/ac/errno.h>
#include <libexplain/ac/linux/if_eql.h>
#include <libexplain/ac/net/if.h>

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/ifreq/slave_config.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/eql_getslavecfg.h>
#include <libexplain/path_is_efault.h>

#ifdef EQL_GETSLAVECFG


static void
print_data(const explain_iocontrol_t *p, explain_string_buffer_t *sb, int
    errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    explain_buffer_ifreq_slave_config(sb, data, 0);
}


static void
print_explanation(const explain_iocontrol_t *p, explain_string_buffer_t *sb, int
    errnum, int fildes, int request, const void *data)
{
    /* see Linux kernel sources, drivers/net/eql.c */
    switch (errnum)
    {
    case EFAULT:
        {
            const struct ifreq *i;
            const struct slave_config *s;

            i = data;
            if (explain_pointer_is_efault(i, sizeof(*i)))
            {
                explain_buffer_efault(sb, "data");
                break;
            }
            s = (const struct slave_config *)i->ifr_data;
            if (explain_pointer_is_efault(s, sizeof(*s)))
            {
                explain_buffer_efault(sb, "data->ifr_data");
                break;
            }
        }
        goto generic;

    case ENODEV:
        /*
         * FIXME: named slave device does not exist
         */
        goto generic;

    case EINVAL:
        /*
         * FIXME: the named device may not be a slave, the named device
         * may not a an if_eql slave.
         */
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
    (void)fildes;
    (void)request;
    explain_buffer_ifreq_slave_config(sb, data, 1);
}


const explain_iocontrol_t explain_iocontrol_eql_getslavecfg =
{
    "EQL_GETSLAVECFG", /* name */
    EQL_GETSLAVECFG, /* value */
    explain_iocontrol_disambiguate_is_if_eql,
    0, /* print_name */
    print_data,
    print_explanation,
    print_data_returned,
    sizeof(struct ifreq), /* data_size */
    __FILE__,
    __LINE__,
};

#else /* ndef EQL_GETSLAVECFG */

const explain_iocontrol_t explain_iocontrol_eql_getslavecfg =
{
    0, /* name */
    0, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
    0, /* print_data_returned */
    0, /* data_size */
    __FILE__,
    __LINE__,
};

#endif /* EQL_GETSLAVECFG */