/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_EINVAL_H
#define LIBEXPLAIN_BUFFER_EINVAL_H

#include <libexplain/string_buffer.h>

/**
  * The explain_buffer_einval_bits function may be used to
  * explain a bit-field containing undefined bits.
  *
  * @param sb
  *    The string buffer to print into.
  * @param caption
  *    The name of the offending system call argument.
  */
void explain_buffer_einval_bits(explain_string_buffer_t *sb,
    const char *caption);

/**
  * The explain_buffer_einval_too_small function may be used to
  * explain that a system call argument is too small (out of range).
  *
  * @param sb
  *    The string buffer to print into.
  * @param caption
  *    The name of the offending system call argument.
  * @param data
  *    The value of the offending system call argument.
  */
void explain_buffer_einval_too_small(explain_string_buffer_t *sb,
    const char *caption, long data);

/**
  * The explain_buffer_einval_too_large function may be used to
  * explain that a system call argument is too large (out of range).
  *
  * @param sb
  *    The string buffer to print into.
  * @param caption
  *    The name of the offending system call argument.
  */
void explain_buffer_einval_too_large(explain_string_buffer_t *sb,
    const char *caption);

/**
  * The explain_buffer_einval_vague function may be used to explain
  * that a system call argument is invalid, but not why (use this one
  * sparingly).
  *
  * @param sb
  *    The string buffer to print into.
  * @param caption
  *    The name of the offending system call argument.
  */
void explain_buffer_einval_vague(explain_string_buffer_t *sb,
    const char *caption);

/**
  * The explain_buffer_einval_value function may be used to explain
  * that a system call argument is invalid, but not why.
  *
  * @param sb
  *    The string buffer to print into.
  * @param caption
  *    The name of the offending system call argument.
  * @param value
  *    The value of the offending system call argument.
  */
void explain_buffer_einval_value(explain_string_buffer_t *sb,
    const char *caption, long value);

/**
  * The explain_buffer_einval_not_a_number function may be used to
  * explain that a system call argument is invalid, when it is supposed
  * to be a string containing a number.
  *
  * @param sb
  *    The string buffer to print into.
  * @param caption
  *    The name of the offending system call argument.
  */
void explain_buffer_einval_not_a_number(explain_string_buffer_t *sb,
    const char *caption);

void explain_buffer_einval_not_listening(explain_string_buffer_t *sb);

/**
  * The explain_buffer_einval_out_of_range functions is used to print
  * an explanation for an EINVAL error, in the case where an argument's
  * value is outside the valid range.
  *
  * @param sb
  *     The buffer to print into.
  * @param caption
  *     The name of the offending system call argument.
  * @param lo
  *     The lower bound of the valid range.
  * @param hi
  *     The upper bound of the valid range.
  */
void explain_buffer_einval_out_of_range(explain_string_buffer_t *sb,
    const char *caption, long lo, long hi);

/**
  * The explain_buffer_einval_signalfd function is use to printan
  * explaiantion of an EINVAL error reported by the signalfd system
  * call reposrt an EINVAL error, in the case where the file descriptor
  * is actually open, but does not refer to a valid signalfd file
  * descriptor.
  *
  * @param sb
  *     The buffer to print into.
  * @param caption
  *     The name of the offending system call argument.
  */
void explain_buffer_einval_signalfd(explain_string_buffer_t *sb,
    const char *caption);

#endif /* LIBEXPLAIN_BUFFER_EINVAL_H */
