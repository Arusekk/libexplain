/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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

#ifndef LIBEXPLAIN_PARSE_BITS_H
#define LIBEXPLAIN_PARSE_BITS_H

#include <libexplain/ac/stddef.h>

typedef struct libexplain_parse_bits_table_t libexplain_parse_bits_table_t;
struct libexplain_parse_bits_table_t
{
    const char      *name;
    int             value;
};

/**
  * The libexplain_parse_bits function may be used to parse an input
  * string against a table of bitfields.  There may be symbols (from the
  * bits table) or numeric conatsnts (using C notation) and there may be
  * plus (+) or bit-wise-or (|) operators.
  *
  * @param text
  *    The text to be parsed.
  * @param table
  *    The table of symbols for the parser.
  * @param table_size
  *    The lentgh of the table of symbols.
  * @returns
  *    the union of the given bits, or -1 on error
  * @note
  *    this function is <b>not</b> thread safe
  */
int libexplain_parse_bits(const char *text,
    const libexplain_parse_bits_table_t *table, size_t table_size);

/**
  * The libexplain_parse_bits_get_error function may be used to obtain
  * the error message emitted by the parser by the previous call to
  * #libexplain_parse_bits.
  */
const char *libexplain_parse_bits_get_error(void);

#endif /* LIBEXPLAIN_PARSE_BITS_H */
