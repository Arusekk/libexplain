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

#include <libexplain/ac/ctype.h>
#include <libexplain/ac/errno.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>


static const libexplain_parse_bits_table_t ioc_dir_table[] =
{
    { "_IOC_NONE", _IOC_NONE },
    { "_IOC_WRITE", _IOC_WRITE },
    { "_IOC_READ", _IOC_READ },
};


static void
libexplain_buffer_ioc_dir(libexplain_string_buffer_t *sb, int ioc_dir)
{
    const libexplain_parse_bits_table_t *tp;

    tp =
        libexplain_parse_bits_find_by_value
        (
            ioc_dir,
            ioc_dir_table,
            SIZEOF(ioc_dir_table)
        );
    if (tp)
        libexplain_string_buffer_puts(sb, tp->name);
    else
        libexplain_string_buffer_printf(sb, "%d", ioc_dir);
}


static void
print_name(const libexplain_iocontrol_t *p, libexplain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)data;
    if (SIOCDEVPRIVATE <= request && request < SIOCDEVPRIVATE + 16)
    {
        libexplain_string_buffer_printf
        (
            sb,
            "SIOCDEVPRIVATE + %d",
            request - SIOCDEVPRIVATE
        );
    }
    else if (SIOCPROTOPRIVATE <= request && request < SIOCPROTOPRIVATE + 16)
    {
        libexplain_string_buffer_printf
        (
            sb,
            "SIOCPROTOPRIVATE + %d",
            request - SIOCPROTOPRIVATE
        );
    }
    else
    {
        int             ioc_dir;
        int             ioc_type;
        int             ioc_nr;
        int             ioc_size;

        ioc_dir = _IOC_DIR(request);
        ioc_type = _IOC_TYPE(request);
        ioc_nr = _IOC_NR(request);
        ioc_size = _IOC_SIZE(request);
        switch (ioc_dir)
        {
        case _IOC_NONE:
            if (ioc_size != 0)
                goto yuck;
            libexplain_string_buffer_puts(sb, "_IO(");
            break;

        case _IOC_READ:
            libexplain_string_buffer_puts(sb, "_IOR(");
            break;

        case _IOC_WRITE:
            libexplain_string_buffer_puts(sb, "_IOW(");
            break;

        case _IOC_READ | _IOC_WRITE:
            libexplain_string_buffer_puts(sb, "_IORW(");
            break;

        default:
            yuck:
            libexplain_string_buffer_puts(sb, "_IOC(");
            libexplain_buffer_ioc_dir(sb, ioc_dir);
            libexplain_string_buffer_puts(sb, ", ");
            break;
        }
        if (ioc_type >= 0 && ioc_type < 256 && isprint(ioc_type))
            libexplain_string_buffer_putc_quoted(sb, ioc_type);
        else
            libexplain_string_buffer_printf(sb, "%#x", ioc_type);
        libexplain_string_buffer_printf(sb, ", %#x, %#x)", ioc_nr, ioc_size);
    }
}


static void
print_explanation(const libexplain_iocontrol_t *p,
    libexplain_string_buffer_t *sb, int errnum, int fildes, int request,
    const void *data)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/ioctl.html
     */
    (void)p;
    (void)request;
    (void)data;
    switch (errnum)
    {
    case EBADF:
        libexplain_buffer_ebadf(sb, fildes, "fildes");
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "data");
        break;

    case EINVAL:
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This message is used to explain an EINVAL
             * error reorted by an ioctl(2) system call, when a more
             * specific explanation is not availble.
             */
            i18n("ioctl request or ioctl data is not valid")
        );
        break;

    case ENOTTY:
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This message is used to explain an ENOTTY
             * error reported by an ioctl(2) system call, when a more
             * specific explanation is not availble.
             */
            i18n("fildes is not associated with a character special device; "
            "or, fildes is not associated with an object to which request can "
            "be applied")
        );
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


const libexplain_iocontrol_t libexplain_iocontrol_generic =
{
    NULL, /* name */
    -1, /* value */
    NULL, /* disambiguate */
    print_name,
    NULL, /* print data */
    print_explanation
};
