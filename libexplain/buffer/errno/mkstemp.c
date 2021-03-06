/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/eexist.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/errno/open.h>
#include <libexplain/buffer/errno/mkstemp.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/pathname.h>
#include <libexplain/explanation.h>


static void
explain_buffer_errno_mkstemp_system_call(explain_string_buffer_t *sb, int
    errnum, const char *pathname)
{
    (void)errnum;
    /*
     * the argument must be called "pathname" so that the explanations
     * for open() make sense.
     */
    explain_string_buffer_puts(sb, "mkstemp(pathname = ");
    explain_buffer_pathname(sb, pathname);
    explain_string_buffer_putc(sb, ')');
}


void
explain_buffer_errno_mkstemp_explanation(explain_string_buffer_t *sb,
    int errnum, const char *syscall_name, const char *pathname)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/mkstemp.html
     */
    switch (errnum)
    {
    case EFAULT:
        explain_buffer_efault(sb, "pathname");
        break;

    case EINVAL:
        explain_buffer_einval_mkstemp(sb, pathname, "pathname");
        break;

    case EEXIST:
        explain_buffer_eexist_tempname_dirname(sb, pathname);
        break;

    default:
        explain_buffer_errno_open_explanation
        (
            sb,
            errnum,
            syscall_name,
            pathname,
            O_RDWR | O_CREAT | O_EXCL,
            S_IRUSR | S_IWUSR
        );
        break;
    }
}


void
explain_buffer_errno_mkstemp(explain_string_buffer_t *sb, int errnum,
    const char *pathname)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_mkstemp_system_call(&exp.system_call_sb, errnum,
        pathname);
    explain_buffer_errno_mkstemp_explanation(&exp.explanation_sb, errnum,
        "mkstemp", pathname);
    explain_explanation_assemble(&exp, sb);
}


/* vim: set ts=8 sw=4 et : */
