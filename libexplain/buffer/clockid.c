/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/sys/timerfd.h>

#include <libexplain/buffer/clockid.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>


#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0
#endif
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME (CLOCK_MONOTONIC+1)
#endif


static const explain_parse_bits_table_t table[] =
{
    { "CLOCK_MONOTONIC", CLOCK_MONOTONIC },
    { "CLOCK_REALTIME", CLOCK_REALTIME },
};


void
explain_buffer_clockid(explain_string_buffer_t *sb, int value)
{
    explain_parse_bits_print_single(sb, value, table, SIZEOF(table));
}


int
explain_parse_clockid_or_die(const char *text, const char *caption)
{
    return explain_parse_bits_or_die(text, table, SIZEOF(table), caption);
}