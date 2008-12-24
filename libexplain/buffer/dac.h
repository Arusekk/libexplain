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

#ifndef LIBEXPLAIN_BUFFER_DAC_H
#define LIBEXPLAIN_BUFFER_DAC_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_dac_fowner function may be used to
  * explain the absence of the DAC_FOWNER capability.
  *
  * @param sb
  *    The string buffer to print into.
  */
void libexplain_buffer_dac_fowner(libexplain_string_buffer_t *sb);

/**
  * The libexplain_buffer_dac_read_search function may be used to
  * explain the absence of the DAC_READ_SEARCH capability.
  *
  * @param sb
  *    The string buffer to print into.
  */
void libexplain_buffer_dac_read_search(libexplain_string_buffer_t *sb);

/**
  * The libexplain_buffer_dac_override function may be used to
  * explain the absence of the DAC_OVERRIDE capability.
  *
  * @param sb
  *    The string buffer to print into.
  */
void libexplain_buffer_dac_override(libexplain_string_buffer_t *sb);

/**
  * The libexplain_buffer_dac_net_raw function may be used to
  * explain the absence of the DAC_NET_RAW capability.
  *
  * @param sb
  *    The string buffer to print into.
  */
void libexplain_buffer_dac_net_raw(libexplain_string_buffer_t *sb);

/**
  * The libexplain_buffer_dac_net_bind_service function may be used to
  * explain the absence of the DAC_NET_BIND_SERVICE capability.
  *
  * @param sb
  *    The string buffer to print into.
  */
void libexplain_buffer_dac_net_bind_service(libexplain_string_buffer_t *sb);

/**
  * The libexplain_buffer_dac_chown function may be used to explain the
  * absence of the DAC_CHOWN capability.
  *
  * @param sb
  *    The string buffer to print into.
  */
void libexplain_buffer_dac_chown(libexplain_string_buffer_t *sb);

/**
  * The libexplain_buffer_and_the_process_is_not_privileged function is
  * called by the above functions to insert the text "and the process is
  * not privileged" or its translation.  This isolates the string to a
  * single function.
  *
  * @param sb
  *    The string buffer to print into.
  */
void libexplain_buffer_and_the_process_is_not_privileged(
    libexplain_string_buffer_t *sb);

/**
  * The libexplain_buffer_does_not_have_capability function is called
  * by the above functions to insert text naming the absent specific
  * capability.
  *
  * @param sb
  *    The string buffer to print into.
  * @param cap_name
  *     The name of the required capability.
  */
void libexplain_buffer_does_not_have_capability(libexplain_string_buffer_t *sb,
    const char *cap_name);

#endif /* LIBEXPLAIN_BUFFER_DAC_H */
