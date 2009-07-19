/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
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
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/gethostname.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/software_error.h>
#include <libexplain/explanation.h>


static void
explain_buffer_errno_gethostname_system_call(explain_string_buffer_t *sb,
    int errnum, char *data, size_t data_size)
{
    (void)errnum;
    explain_string_buffer_puts(sb, "gethostname(data = ");
    explain_buffer_pointer(sb, data);
    explain_string_buffer_printf(sb, ", data_size = %zd)", data_size);
}


static size_t
get_actual_hostname_size(void)
{
    /*
     * SUSv2 guarantees that "Host names are limited to 255
     * bytes".  POSIX.1-2001 guarantees that "Host names (not
     * including the terminating null byte) are limited to
     * HOST_NAME_MAX bytes". On Linux, HOST_NAME_MAX is defined
     * with the value 64, which has been the limit since Linux
     * 1.0 (earlier kernels imposed a limit of 8 bytes).
     */
#if HOST_NAME_MAX > 255
    char name[HOST_NAME_MAX + 2];
#else
    char name[257];
#endif
    if (gethostname(name, sizeof(name) - 1) == 0)
    {
        /* paranoia */
        name[sizeof(name) - 1] = '\0';

        return strlen(name);
    }

    /*
     * no idea what the length is
     */
    return 0;
}


static void
enametoolong(explain_string_buffer_t *sb, size_t actual_size,
    const char *caption)
{
    explain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used when explaining an EINVAL or
         * ENAMETOOLONG error returned by the gethostname system call,
         * in the case where the supplied data buffer is smaller than
         * the actual host name.
         *
         * %1$s => the name of the offending system call argument
         * %2$d => the minimum size (in bytes) needed to hold the actual
         *         host name
         */
        i18n("the %s argument was incorrectly specified, the actual host name "
            "requires at least %d bytes, or preferably use the HOST_NAME_MAX "
            "macro"),
        caption,
        (int)(actual_size + 1)
    );
}


static void
explain_buffer_errno_gethostname_explanation(explain_string_buffer_t *sb,
    int errnum, char *data, size_t data_size)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/gethostname.html
     */
    (void)data;
    switch (errnum)
    {
    case EFAULT:
        explain_buffer_efault(sb, "data");
        break;

    case EINVAL:
        {
            size_t          actual_size;

            if ((int)data_size <= 0)
            {
                explain_buffer_einval_too_small(sb, "data_size", data_size);
                break;
            }
            actual_size = get_actual_hostname_size();
            if (actual_size > 0 && data_size < actual_size + 1)
                enametoolong(sb, actual_size, "data_size");
            else
                explain_buffer_einval_vague(sb, "data_size");
            explain_buffer_software_error(sb);
        }
        break;

    case ENAMETOOLONG:
        /* data_size is smaller than the actual size.  */
        {
            size_t actual_size = get_actual_hostname_size();
            if (actual_size > 0 && data_size < actual_size + 1)
                enametoolong(sb, actual_size, "data_size");
            else
                explain_buffer_einval_too_small(sb, "data_size", data_size);
            explain_buffer_software_error(sb);
        }
        break;

    default:
        explain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
explain_buffer_errno_gethostname(explain_string_buffer_t *sb, int errnum,
    char *data, size_t data_size)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_gethostname_system_call
    (
        &exp.system_call_sb,
        errnum,
        data,
        data_size
    );
    explain_buffer_errno_gethostname_explanation
    (
        &exp.explanation_sb,
        errnum,
        data,
        data_size
    );
    explain_explanation_assemble(&exp, sb);
}

/* vim: set ts=8 sw=4 et */
