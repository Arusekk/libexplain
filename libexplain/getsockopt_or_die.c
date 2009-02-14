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

#include <libexplain/getsockopt.h>
#include <libexplain/option.h>
#include <libexplain/wrap_and_print.h>


void
libexplain_getsockopt_or_die(int fildes, int level, int name, void *data,
    socklen_t *data_size)
{
    if (getsockopt(fildes, level, name, data, data_size) < 0)
    {
        libexplain_program_name_assemble_internal(1);
        libexplain_wrap_and_print
        (
            stderr,
            libexplain_getsockopt(fildes, level, name, data, data_size)
        );
        exit(EXIT_FAILURE);
    }
}

/* vim:ts=8:sw=4:et */
