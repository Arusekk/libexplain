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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/popen.h>
#include <libexplain/wrap_and_print.h>


FILE *
libexplain_popen_or_die(const char *command, const char *flags)
{
    FILE            *fp;

    fp = popen(command, flags);
    if (!fp)
    {
        libexplain_wrap_and_print(stderr, libexplain_popen(command, flags));
        exit(EXIT_FAILURE);
    }
    return fp;
}

/* vim:ts=8:sw=4:et */
