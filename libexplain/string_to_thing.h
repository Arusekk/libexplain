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

#ifndef LIBEXPLAIN_STRING_TO_THING_H
#define LIBEXPLAIN_STRING_TO_THING_H

#include <libexplain/ac/stddef.h>
#include <libexplain/ac/sys/socket.h>
#include <libexplain/ac/unistd.h>

/**
  * The explain_string_to_thing function may be used to convert text to
  * a size_t value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a size_t value.  On error, prints an error message
  *    and does not return.
  */
size_t explain_string_to_size_t(const char *text);

/**
  * The explain_string_to_thing function may be used to convert text to
  * a ssize_t value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a ssize_t value.  On error, prints an error message
  *    and does not return.
  */
ssize_t explain_string_to_ssize_t(const char *text);

/**
  * The explain_string_to_thing function may be used to convert text to
  * a ptrdiff_t value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a ptrdiff_t value.  On error, prints an error message
  *    and does not return.
  */
ptrdiff_t explain_string_to_ptrdiff_t(const char *text);

/**
  * The explain_string_to_thing function may be used to convert text to
  * a off_t value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a off_t value.  On error, prints an error message
  *    and does not return.
  */
off_t explain_string_to_off_t(const char *text);

/**
  * The explain_string_to_thing function may be used to convert text to
  * a pointer value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a pointer value.  On error, prints an error message
  *    and does not return.
  */
void *explain_string_to_pointer(const char *text);

/**
  * The explain_string_to_long function may be used to convert text to
  * a long value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a pointer value.  On error, prints an error message
  *    and does not return.
  */
long explain_string_to_long(const char *text);

/**
  * The explain_string_to_ulong function may be used to convert text to
  * an unsigned long value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a pointer value.  On error, prints an error message
  *    and does not return.
  */
unsigned long explain_string_to_ulong(const char *text);

/**
  * The explain_string_to_longlong function may be used to convert text
  * to a long long value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a pointer value.  On error, prints an error message
  *    and does not return.
  */
long long explain_string_to_longlong(const char *text);

/**
  * The explain_string_to_ulonglong function may be used to convert text
  * to an unsigned long long value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a pointer value.  On error, prints an error message
  *    and does not return.
  */
unsigned long long explain_string_to_ulonglong(const char *text);

/**
  * The explain_string_to_socklen_t function may be used to convert text
  * to a socklen_t value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a pointer value.  On error, prints an error message
  *    and does not return.
  */
socklen_t explain_string_to_socklen_t(const char *text);

/**
  * The explain_string_to_int function may be used to convert text
  * to a int value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a pointer value.  On error, prints an error message
  *    and does not return.
  */
int explain_string_to_int(const char *text);

/**
  * The explain_string_to_uint function may be used to convert text to
  * an unsigned int value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a pointer value.  On error, prints an error message
  *    and does not return.
  */
unsigned explain_string_to_uint(const char *text);

/**
  * The explain_string_to_double function may be used to convert text to
  * a double-precision floating-point value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a floating-point value.  On error, prints an error
  *    message and does not return.
  */
double explain_string_to_double(const char *text);

/**
  * The explain_string_to_float function may be used to convert text to
  * a single-precision floating-point value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a floating-point value.  On error, prints an error
  *    message and does not return.
  */
float explain_string_to_float(const char *text);

/**
  * The explain_string_to_long_double function may be used to convert
  * text to a triple-precision floating-point value.
  *
  * @param text
  *    The text to be converted.
  * @returns
  *    On success, a floating-point value.  On error, prints an error
  *    message and does not return.
  */
long double explain_string_to_long_double(const char *text);

struct timeval; /* forward */

void explain_string_to_timeval(const char *text, struct timeval *result);

#endif /* LIBEXPLAIN_STRING_TO_THING_H */
