/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_STRSIGNAL_H
#define LIBEXPLAIN_BUFFER_STRSIGNAL_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_strsignal function may be used to decode a
  * signal number an pront information about it into the given string
  * buffer.
  *
  * @param sb
  *    The string buffer to print into
  * @param signum
  *    The number of the signal to be decoded
  */
void libexplain_buffer_strsignal(libexplain_string_buffer_t *sb, int signum);

/**
  * The libexplain_signal_parse function may be used to parse text and
  * extract the signal number.
  *
  * @param text
  *     The text to be parsed.
  * @param caption
  *     addition text to add to start of error message
  * @returns
  *     the signal number
  */
int libexplain_signal_parse_or_die(const char *text, const char *caption);

#endif /* LIBEXPLAIN_BUFFER_STRSIGNAL_H */
