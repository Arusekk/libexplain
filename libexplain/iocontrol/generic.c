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
#include <libexplain/buffer/enosys.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>


static const explain_parse_bits_table_t ioc_dir_table[] =
{
    { "_IOC_NONE", _IOC_NONE },
    { "_IOC_WRITE", _IOC_WRITE },
    { "_IOC_READ", _IOC_READ },
};


static void
explain_buffer_ioc_dir(explain_string_buffer_t *sb, int ioc_dir)
{
    const explain_parse_bits_table_t *tp;

    tp =
        explain_parse_bits_find_by_value
        (
            ioc_dir,
            ioc_dir_table,
            SIZEOF(ioc_dir_table)
        );
    if (tp)
        explain_string_buffer_puts(sb, tp->name);
    else
        explain_string_buffer_printf(sb, "%d", ioc_dir);
}


static void
print_name(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)data;
    if (SIOCDEVPRIVATE <= request && request < SIOCDEVPRIVATE + 16)
    {
        explain_string_buffer_printf
        (
            sb,
            "SIOCDEVPRIVATE + %d",
            request - SIOCDEVPRIVATE
        );
    }
    else if (SIOCPROTOPRIVATE <= request && request < SIOCPROTOPRIVATE + 16)
    {
        explain_string_buffer_printf
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
            explain_string_buffer_puts(sb, "_IO(");
            break;

        case _IOC_READ:
            explain_string_buffer_puts(sb, "_IOR(");
            break;

        case _IOC_WRITE:
            explain_string_buffer_puts(sb, "_IOW(");
            break;

        case _IOC_READ | _IOC_WRITE:
            explain_string_buffer_puts(sb, "_IORW(");
            break;

        default:
            yuck:
            explain_string_buffer_puts(sb, "_IOC(");
            explain_buffer_ioc_dir(sb, ioc_dir);
            explain_string_buffer_puts(sb, ", ");
            break;
        }
        if (ioc_type >= 0 && ioc_type < 256 && isprint(ioc_type))
            explain_string_buffer_putc_quoted(sb, ioc_type);
        else
            explain_string_buffer_printf(sb, "%#x", ioc_type);
        explain_string_buffer_printf(sb, ", %#x, %#x)", ioc_nr, ioc_size);
    }
}


static void
print_explanation(const explain_iocontrol_t *p,
    explain_string_buffer_t *sb, int errnum, int fildes, int request,
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
        explain_buffer_ebadf(sb, fildes, "fildes");
        break;

    case EFAULT:
        explain_buffer_efault(sb, "data");
        break;

    case EINVAL:
        explain_buffer_gettext
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
    case ENOSYS:
#if defined(EOPNOTSUPP) && EOPNOTSUPP != ENOSYS
    case EOPNOTSUPP:
#endif
        {
            char            syscall[30];
            explain_string_buffer_t syscall_buf;

            explain_string_buffer_init(&syscall_buf, syscall, sizeof(syscall));
            explain_string_buffer_puts(&syscall_buf, "ioctl ");
            if (p->name)
                explain_string_buffer_puts(&syscall_buf, p->name);
            else
                explain_string_buffer_printf(&syscall_buf, "0x%#X", request);
            explain_buffer_enosys_fildes(sb, fildes, "fildes", syscall);
        }
        break;

    default:
        explain_buffer_errno_generic(sb, errnum);
        break;
    }
}


const explain_iocontrol_t explain_iocontrol_generic =
{
    NULL, /* name */
    -1, /* value */
    NULL, /* disambiguate */
    print_name,
    NULL, /* print data */
    print_explanation
};
