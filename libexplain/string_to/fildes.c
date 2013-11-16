/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2013 Peter Miller
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>

#include <libexplain/string_to_thing.h>
#include <libexplain/open.h>


int
explain_parse_fildes_or_die(const char *text)
{
    /* First try, it could be a number */
    char *ep;
    long result = strtol(text, &ep, 0);
    if (text != ep && !*ep)
        return result;

    /* Second try, it could be a specific stream */
    if (0 == strcmp(text, "stdin"))
        return fileno(stdin);
    if (0 == strcmp(text, "stdout"))
        return fileno(stdout);
    if (0 == strcmp(text, "stderr"))
        return fileno(stderr);

    /* Third try, it could be a pathname */
    return explain_open_or_die(text, O_RDONLY, 0);
}


/* vim: set ts=8 sw=4 et : */
