/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/fpathconf.h>
#include <libexplain/buffer/errno/pathconf.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/pathconf_name.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_fpathconf_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes, int name)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "fpathconf(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_puts(sb, ", name = ");
    libexplain_buffer_pathconf_name(sb, name);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_fpathconf_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes, int name)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/fpathconf.html
     */
    switch (errnum)
    {
    case EBADF:
        libexplain_buffer_ebadf(sb, fildes, "fildes");
        break;

    case EINVAL:
        libexplain_buffer_pathconf_einval(sb, "fildes", name, "name");
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_fpathconf(libexplain_string_buffer_t *sb, int errnum,
    int fildes, int name)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_fpathconf_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes,
        name
    );
    libexplain_buffer_errno_fpathconf_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes,
        name
    );
    libexplain_explanation_assemble(&exp, sb);
}
