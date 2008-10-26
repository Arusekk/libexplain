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

#ifndef LIBEXPLAIN_CLOSE_H
#define LIBEXPLAIN_CLOSE_H

/**
  * @file
  * @brief explain close(2) errors
  */

#ifdef c_plus_plus
extern "C" {
#endif

/**
  * The libexplain_close function is used to obtain an explanation of an
  * error returned by the close(2) system call.  The least the message will
  * contain is the value of strerror(errno), but usually it will do much
  * better, and indicate the underlying cause in more detail.
  *
  * The errno global variable will be used to obtain the error value to
  * be decoded.
  *
  * This function is intended to be used in a fashion similar to the
  * following exmaple:
  * @code
  * if (close(fd) < 0)
  * {
  *     fprintf(stderr, "%s\n", libexplain_close(fd));
  *     exit(1);
  * }
  * @endcode
  *
  * @param fildes
  *    The original file descriptor, exactly as passed to the close(2)
  *    system call.
  * @returns
  *     The message explaining the error.  This message buffer is shared
  *     by all libexplain functions which do not supply a buffer in
  *     their argument list.  This will be overwritten by the next call
  *     to any libexplain function which shares this buffer, including
  *     other threads.
  * @note
  *     This function is <b>not</b> thread safe, because it shares a
  *     return buffer across all threads, and many other functions in
  *     this library.
  */
const char *libexplain_close(int fildes);

/**
  * The libexplain_errno_close function is used to obtain an explanation
  * of an error returned by the close(2) system call.  The least the
  * message will contain is the value of strerror(errnum), but usually
  * it will do much better, and indicate the underlying cause in more
  * detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following exmaple:
  * @code
  * if (close(fd) < 0)
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_errno_close(err, fd));
  *     exit(1);
  * }
  * @endcode
  *
  * @param errnum
  *     The error value to be decoded, usually obtain from the errno
  *     global variable just before this function is called.  This
  *     is necessary if you need to call <b>any</b> code between the
  *     system call to be explained and this function, because many libc
  *     functions will alter the value of errno.
  * @param fildes
  *    The original file descriptor, exactly as passed to the close(2)
  *    system call.
  * @returns
  *     The message explaining the error.  This message buffer is shared
  *     by all libexplain functions which do not supply a buffer in
  *     their argument list.  This will be overwritten by the next call
  *     to any libexplain function which shares this buffer, including
  *     other threads.
  * @note
  *     This function is <b>not</b> thread safe, because it shares a
  *     return buffer across all threads, and many other functions in
  *     this library.
  */
const char *libexplain_errno_close(int errnum, int fildes);

/**
  * The libexplain_message_close function is used to obtain an
  * explanation of an error returned by the close(2) system call.  The
  * least the message will contain is the value of strerror(errno), but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * The errno global variable will be used to obtain the error value to
  * be decoded.
  *
  * This function is intended to be used in a fashion similar to the
  * following exmaple:
  * @code
  * if (close(fd) < 0)
  * {
  *     char message[3000];
  *     libexplain_message_errno_close(message, sizeof(message), fd);
  *     fprintf(stderr, "%s\n", message);
  *     exit(1);
  * }
  * @endcode
  *
  * @param message
  *     The location in which to store the returned message.  Because
  *     a message return buffer has been supplied, this function is
  *     thread safe.
  * @param message_size
  *     The size in bytes of the location in which to store the returned
  *     message.
  * @param fildes
  *    The original file descriptor, exactly as passed to the close(2)
  *    system call.
  */
void libexplain_message_close(char *message, int message_size, int fildes);

/**
  * The libexplain_message_errno_close function is used to obtain an
  * explanation of an error returned by the close(2) system call.  The
  * least the message will contain is the value of strerror(errnum), but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following exmaple:
  * @code
  * if (close(fd) < 0)
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_close(message, sizeof(message), err, fd);
  *     fprintf(stderr, "%s\n", message);
  *     exit(1);
  * }
  * @endcode
  *
  * @param message
  *     The location in which to store the returned message.  Because
  *     a message return buffer has been supplied, this function is
  *     thread safe.
  * @param message_size
  *     The size in bytes of the location in which to store the returned
  *     message.
  * @param errnum
  *     The error value to be decoded, usually obtain from the errno
  *     global variable just before this function is called.  This
  *     is necessary if you need to call <b>any</b> code between the
  *     system call to be explained and this function, because many libc
  *     functions will alter the value of errno.
  * @param fildes
  *    The original file descriptor, exactly as passed to the close(2)
  *    system call.
  */
void libexplain_message_errno_close(char *message, int message_size,
    int errnum, int fildes);

#ifdef c_plus_plus
}
#endif

#endif /* LIBEXPLAIN_CLOSE_H */
