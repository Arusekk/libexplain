/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/etxtbsy.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/path_to_pid.h>


static void
report_error(libexplain_string_buffer_t *sb)
{
    libexplain_buffer_because(sb);
    libexplain_buffer_gettext
    (
        sb,
        i18n("write access was requested to an executable image that is "
        "currently being executed")
    );
}


void
libexplain_buffer_etxtbsy(libexplain_string_buffer_t *sb, const char *pathname)
{
    report_error(sb);
    libexplain_buffer_path_to_pid(sb, pathname);
}


void
libexplain_buffer_etxtbsy_fildes(libexplain_string_buffer_t *sb, int fildes)
{
    report_error(sb);
    libexplain_buffer_fildes_to_pid(sb, fildes);
}
