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

#ifndef LIBEXPLAIN_IOCONTROL_H
#define LIBEXPLAIN_IOCONTROL_H

struct explain_string_buffer_t; /* forward */

/**
  * The explain_iocontrol_t type is used to represent an ioctl(2) call,
  * providing name and number.
  *
  * @note
  *     This information is not kept in a single table for all values,
  *     like every other set of constants, because (a) some values
  *     are ambiguous, and (b) the includes files have bugs making it
  *     impossible to include all of them in the same combilation unit.
  */
typedef struct explain_iocontrol_t explain_iocontrol_t;
struct explain_iocontrol_t
{
    /**
      * The name of the ioctl(2) request.
      */
    const char *name;

    /**
      * The numeric value of the ioctl(2) request.
      */
    int number;

    /**
      * The disambiguate method is used to ambiguous ioctl(2) request
      * values, to attempt to discern which was intended from
      * information about the state of the file descriptor, and the
      * request itself.  This is not always possible, given that the
      * code is not psychic.
      *
      * If the NULL pointer value is used, the value is assumbed to be
      * non-ambiguous.  This is almost always the case.
      *
      * Returns 0 on success (i.e. a match), -1 on failure (i.e. not a match).
      */
    int (*disambiguate)(int fildes, int request, const void *data);

    /**
      * The print_name method is used to print the name of a request
      * argument passed to an ioctl(2) system call.
      *
      * @param p
      *     Pointer to the explain_iocontrol_t instance,
      *     rather like "this" in C++ code.
      * @param sb
      *     The string buffer to print into.
      * @param errnum
      *     The error number that caused all this, obtained from the
      *     global errno variable, possibly indirectly.
      * @param fildes
      *     The original fildes, exactly as passed to the ioctl(2) system call.
      * @param request
      *     The original request, exactly as passed to the ioctl(2) system call.
      * @param data
      *     The original data, exactly as passed to the ioctl(2) system call.
      */
    void (*print_name)(const explain_iocontrol_t *p,
        struct explain_string_buffer_t *sb, int errnum, int fildes,
        int request, const void *data);

    /**
      * The print_data method is used to print a representation of the data
      * argument passed to an ioctl(2) system call.
      *
      * @param p
      *     Pointer to the explain_iocontrol_t instance,
      *     rather like "this" in C++ code.
      * @param sb
      *     The string buffer to print into.
      * @param errnum
      *     The error number that caused all this, obtained from the
      *     global errno variable, possibly indirectly.
      * @param fildes
      *     The original fildes, exactly as passed to the ioctl(2) system call.
      * @param request
      *     The original request, exactly as passed to the ioctl(2) system call.
      * @param data
      *     The original data, exactly as passed to the ioctl(2) system call.
      */
    void (*print_data)(const explain_iocontrol_t *p,
        struct explain_string_buffer_t *sb, int errnum, int fildes,
        int request, const void *data);

    /**
      * The print_explanation method is used to print an explanation for
      * an error reported by an ioctl(2) system call.
      *
      * @param p
      *     Pointer to the explain_iocontrol_t instance,
      *     rather like "this" in C++ code.
      * @param sb
      *     The string buffer to print into.
      * @param errnum
      *     The error number that caused all this, obtained from the
      *     global errno variable, possibly indirectly.
      * @param fildes
      *     The original fildes, exactly as passed to the ioctl(2) system call.
      * @param request
      *     The original request, exactly as passed to the ioctl(2) system call.
      * @param data
      *     The original data, exactly as passed to the ioctl(2) system call.
      */
    void (*print_explanation)(const explain_iocontrol_t *p,
        struct explain_string_buffer_t *sb, int errnum, int fildes,
        int request, const void *data);
};

/**
  * The explain_iocontrol_find_by_number function may be used to
  * locate an ioctl by number.  A few ioctl(2) calls are ambiguous, so
  * the more information you can give the better.
  *
  * @param fildes
  *     The file descriptor the ioctl(2) call is made against
  * @param request
  *     The request passed to the ioctl(2) system call.
  * @param data
  *     The data passed to the ioctl(2) system call.
  * @returns
  *     a pointer to an iocontrol object, that may be used to describe
  *     the call the ioctl(2) system call.
  */
const explain_iocontrol_t *explain_iocontrol_find_by_number(int fildes,
    int request, const void *data);

/**
  * The explain_iocontrol_print_name function is used to print the
  * name of a request argument passed to an ioctl(2) system call.
  *
  * @param p
  *     Pointer to the explain_iocontrol_t instance,
  *     rather like "this" in C++ code.
  * @param sb
  *     The string buffer to print into.
  * @param errnum
  *     The error number that caused all this, obtained from the
  *     global errno variable, possibly indirectly.
  * @param fildes
  *     The original fildes, exactly as passed to the ioctl(2) system call.
  * @param request
  *     The original request, exactly as passed to the ioctl(2) system call.
  * @param data
  *     The original data, exactly as passed to the ioctl(2) system call.
  */
void explain_iocontrol_print_name(const explain_iocontrol_t *p,
    struct explain_string_buffer_t *sb, int errnum, int fildes,
    int request, const void *data);

/**
  * The explain_iocontrol_print_data function is used to print a
  * representation of the data argument passed to an ioctl(2) system
  * call.
  *
  * @param p
  *     Pointer to the explain_iocontrol_t instance,
  *     rather like "this" in C++ code.
  * @param sb
  *     The string buffer to print into.
  * @param errnum
  *     The error number that caused all this, obtained from the
  *     global errno variable, possibly indirectly.
  * @param fildes
  *     The original fildes, exactly as passed to the ioctl(2) system call.
  * @param request
  *     The original request, exactly as passed to the ioctl(2) system call.
  * @param data
  *     The original data, exactly as passed to the ioctl(2) system call.
  */
void explain_iocontrol_print_data(const explain_iocontrol_t *p,
    struct explain_string_buffer_t *sb, int errnum, int fildes,
    int request, const void *data);

/**
  * The explain_iocontrol_print_explanation function is used to print
  * an explanation for an error reported by an ioctl(2) system call.
  *
  * @param p
  *     Pointer to the explain_iocontrol_t instance,
  *     rather like "this" in C++ code.
  * @param sb
  *     The string buffer to print into.
  * @param errnum
  *     The error number that caused all this, obtained from the
  *     global errno variable, possibly indirectly.
  * @param fildes
  *     The original fildes, exactly as passed to the ioctl(2) system call.
  * @param request
  *     The original request, exactly as passed to the ioctl(2) system call.
  * @param data
  *     The original data, exactly as passed to the ioctl(2) system call.
  */
void explain_iocontrol_print_explanation(const explain_iocontrol_t *p,
    struct explain_string_buffer_t *sb, int errnum, int fildes,
    int request, const void *data);

/**
  * The explain_parse_ioctl_request_or_die function is used to parse
  * a text string to produce an ioctl(2) request value.
  *
  * @param text
  *     The text string to be parsed.
  * @returns
  *     On success, the ioctl(2) request value; on failure, this
  *     function does not return, but instead prints an error message
  *     and terminates via exit(2).
  */
int explain_parse_ioctl_request_or_die(const char *text);

#endif /* LIBEXPLAIN_IOCONTROL_H */
