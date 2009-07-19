/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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

#include <libexplain/string_to_thing.h>


void
explain_string_to_timeval(const char *text, struct timeval *result)
{
    double          seconds;

    seconds = explain_string_to_double(text);
    result->tv_sec = seconds;
    result->tv_usec = (seconds - result->tv_sec) * 1e6 + 0.5;
    if (result->tv_usec < 0)
    {
        result->tv_sec--;
        result->tv_usec += 1000000;
    }
}
