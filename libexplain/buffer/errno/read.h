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

#ifndef LIBEXPLAIN_BUFFER_ERRNO_READ_H
#define LIBEXPLAIN_BUFFER_ERRNO_READ_H

#include <libexplain/string_buffer.h>

#ifdef __cplusplus
extern "C" {
#endif

struct libexplain_string_buffer_t; /* forward */

/**
  * The libexplain_buffer_errno_read function may be used to obtain
  * a human readable explanation of what went wrong in a read(2)
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
  * @param fildes
  *    The file descriptor to be read from,
  *    exactly as passed to the read(2) system call.
  * @param data
  *    The address of the base address in memory to write the data
  *    (the original read call modified the data, this function will not),
  *    exactly as passed to the read(2) system call.
  * @param data_size
  *    The maximum number of bytes of data to be read,
  *    exactly as passed to the read(2) system call.
  * @note
  *    Given a suitably thread safe buffer, this function is thread safe.
  */
void libexplain_buffer_errno_read(libexplain_string_buffer_t *sb,
    int errnum, int fildes, const void *data, size_t data_size);

/**
  * The libexplain_buffer_errno_read_explanation function is called by the
  * libexplain_buffer_errno_read function (and others) to print the
  * explanation, the part after "because..."
  *
  * @param sb
  *    The string buffer into which the message is to be written.
  * @param errnum
  *    The error value to be decoded, usually obtain from the errno
  *    global variable just before this function is called.  This
  *    is necessary if you need to call <b>any</b> code between the
  *    system call to be explained and this function, because many libc
  *    functions will alter the value of errno.
  * @param fildes
  *    The file descriptor to be read from,
  *    exactly as passed to the read(2) system call.
  * @param data
  *    The address of the base address in memory to write the data
  *    (the original read call modified the data, this function will not),
  *    exactly as passed to the read(2) system call.
  * @param data_size
  *    The maximum number of bytes of data to be read,
  *    exactly as passed to the read(2) system call.
  * @note
  *    Given a suitably thread safe buffer, this function is thread safe.
  */
void libexplain_buffer_errno_read_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes, const void *data, size_t data_size);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_BUFFER_ERRNO_READ_H */
