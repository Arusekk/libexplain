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

#ifndef LIBEXPLAIN_BUFFER_TCFLUSH_SELECTOR_H
#define LIBEXPLAIN_BUFFER_TCFLUSH_SELECTOR_H

#include <libexplain/string_buffer.h>

/**
  * The explain_buffer_tcflush_selector function may be used
  * to print a representation of a tcflush selector value.
  *
  * @param sb
  *     The string buffer to print into.
  * @param data
  *     The tcflush selector value to be printed.
  */
void explain_buffer_tcflush_selector(explain_string_buffer_t *sb, int data);

/**
  * the explain_parse_tcflush_selector_or_die function is used to parse
  * a text representation into a tcflush selector value.
  *
  * @param text
  *     The text to be parsed.
  * @param caption
  *     Additional information for error messages.
  * @return
  *     the int selector value on success; on failure it does not return
  *     but instead prints an error message and exits.
  */
int explain_parse_tcflush_selector_or_die(const char *text,
    const char *caption);

#endif /* LIBEXPLAIN_BUFFER_TCFLUSH_SELECTOR_H */
