/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#ifndef LIBEXPLAIN_BUFFER_ERRNO_FFLUSH_H
#define LIBEXPLAIN_BUFFER_ERRNO_FFLUSH_H

#include <libexplain/ac/stdio.h>

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_errno_fflush function may be used to obtain
  * a human readable explanation of what went wrong in a fflush(3)
  * system call.  The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better, and indicate
  * the underlying cause in more detail.
  *
  * @param sb
  *    The string buffer into which the message is to be written.
  * @param errnum
  *    The error value to be decoded, usually obtain from the errno
  *    global variable just before this function is called.  This
  *    is necessary if you need to call <b>any</b> code between the
  *    system call to be explained and this function, because many libc
  *    functions will alter the value of errno.
  * @param fp
  *    The original fp, exactly as passed to the fflush(3) system call.
  * @note
  *    Given a suitably thread safe buffer, this function is thread safe.
  */
void libexplain_buffer_errno_fflush(libexplain_string_buffer_t *sb, int errnum,
    FILE *fp);

/**
  * The libexplain_buffer_errno_fflush_because is used by the
  * libexplain_buffer_errno_fflush function (and others) to decode the
  * errno value.
  *
  * @param sb
  *    The string buffer into which the message is to be written.
  * @param errnum
  *    The error value to be decoded.
  * @param fp
  *    The original fp, exactly as passed to the fflush(3) system call.
  */
void libexplain_buffer_errno_fflush_because(libexplain_string_buffer_t *sb,
    int errnum, FILE *fp);

#endif /* LIBEXPLAIN_BUFFER_ERRNO_FFLUSH_H */
