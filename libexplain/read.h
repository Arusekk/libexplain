/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
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

#ifndef LIBEXPLAIN_READ_H
#define LIBEXPLAIN_READ_H

/**
  * @file
  * @brief explain read(2) errors
  */

#include <libexplain/warn_unused_result.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
  * The libexplain_read_or_die function is
  * used to call the read(2) system call.  On
  * failure an explanation will be printed to stderr,
  * obtained from libexplain_read(3), and
  * then the process terminates by calling exit(EXIT_FAILURE).
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * long result = libexplain_read_or_die(fildes, data, data_size);
  * @endcode
  *
  * @param fildes
  *     The fildes, exactly as to be passed to the read(2) system call.
  * @param data
  *     The data, exactly as to be passed to the read(2) system call.
  * @param data_size
  *     The data_size, exactly as to be passed to the read(2) system call.
  * @returns
  *     This function only returns on success.
  *     On failure, prints an explanation and exits,
  *     it does not return.
  */
long libexplain_read_or_die(int fildes, void *data, long data_size);

/**
  * The libexplain_read function is used to
  * obtain an explanation of an error returned by the
  * read(2) system call.
  * The least the message will contain is the value of
  * strerror(errno), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * The errno global variable will be used to obtain the
  * error value to be decoded.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * ssize_t result = read(fildes, data, data_size);
  * if (result < 0)
  * {
  *     fprintf(stderr, "%s\n", libexplain_read(fildes, data, data_size));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param fildes
  *     The original fildes, exactly as passed to the read(2) system call.
  * @param data
  *     The original data, exactly as passed to the read(2) system call.
  * @param data_size
  *     The original data_size, exactly as passed to the read(2) system call.
  * @returns
  *     The message explaining the error.  This
  *     message buffer is shared by all libexplain
  *     functions which do not supply a buffer in their
  *     argument list.  This will be overwritten by the
  *     next call to any libexplain function which shares
  *     this buffer, including other threads.
  * @note
  *     This function is <b>not</b> thread safe, because
  *     it shares a return buffer across all threads, and
  *     many other functions in this library.
  */
const char *libexplain_read(int fildes, const void *data, long data_size)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The libexplain_errno_read function is
  * used to obtain an explanation of an error returned by
  * the read(2) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * ssize_t result = read(fildes, data, data_size);
  * if (result < 0)
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_read(err, fildes, data, data_size));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param errnum
  *     The error value to be decoded, usually obtained
  *     from the errno global variable just before this
  *     function is called.  This is necessary if you need
  *     to call <b>any</b> code between the system call to
  *     be explained and this function, because many libc
  *     functions will alter the value of errno.
  * @param fildes
  *     The original fildes, exactly as passed to the read(2) system call.
  * @param data
  *     The original data, exactly as passed to the read(2) system call.
  * @param data_size
  *     The original data_size, exactly as passed to the read(2) system call.
  * @returns
  *     The message explaining the error.  This
  *     message buffer is shared by all libexplain
  *     functions which do not supply a buffer in their
  *     argument list.  This will be overwritten by the
  *     next call to any libexplain function which shares
  *     this buffer, including other threads.
  * @note
  *     This function is <b>not</b> thread safe, because
  *     it shares a return buffer across all threads, and
  *     many other functions in this library.
  */
const char *libexplain_errno_read(int errnum, int fildes, const void *data,
    long data_size)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The libexplain_message_read function is
  * used to obtain an explanation of an error returned by
  * the read(2) system call.
  * The least the message will contain is the value of
  * strerror(errno), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * The errno global variable will be used to obtain the
  * error value to be decoded.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * ssize_t result = read(fildes, data, data_size);
  * if (result < 0)
  * {
  *     char message[3000];
  *     libexplain_message_read(message, sizeof(message), fildes, data,
  *         data_size);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param message
  *     The location in which to store the returned
  *     message.  If a suitable message return buffer is
  *     supplied, this function is thread safe.
  * @param message_size
  *     The size in bytes of the location in which to
  *     store the returned message.
  * @param fildes
  *     The original fildes, exactly as passed to the read(2) system call.
  * @param data
  *     The original data, exactly as passed to the read(2) system call.
  * @param data_size
  *     The original data_size, exactly as passed to the read(2) system call.
  */
void libexplain_message_read(char *message, int message_size, int fildes,
    const void *data, long data_size);

/**
  * The libexplain_message_errno_read
  * function is used to obtain an explanation of an error
  * returned by the
  * read(2) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * ssize_t result = read(fildes, data, data_size);
  * if (result < 0)
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_read(message, sizeof(message), err, fildes,
  *         data, data_size);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param message
  *     The location in which to store the returned
  *     message.  If a suitable message return buffer is
  *     supplied, this function is thread safe.
  * @param message_size
  *     The size in bytes of the location in which to
  *     store the returned message.
  * @param errnum
  *     The error value to be decoded, usually obtained
  *     from the errno global variable just before this
  *     function is called.  This is necessary if you need
  *     to call <b>any</b> code between the system call to
  *     be explained and this function, because many libc
  *     functions will alter the value of errno.
  * @param fildes
  *     The original fildes, exactly as passed to the read(2) system call.
  * @param data
  *     The original data, exactly as passed to the read(2) system call.
  * @param data_size
  *     The original data_size, exactly as passed to the read(2) system call.
  */
void libexplain_message_errno_read(char *message, int message_size, int errnum,
    int fildes, const void *data, long data_size);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_READ_H */
