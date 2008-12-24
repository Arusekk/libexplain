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

#ifndef LIBEXPLAIN_CONNECT_H
#define LIBEXPLAIN_CONNECT_H

/**
  * @file
  * @brief explain connect(2) errors
  */

#include <libexplain/warn_unused_result.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sockaddr; /* forward */

/**
  * The libexplain_connect_or_die function is
  * used to call the connect(2) system call.  On
  * failure an explanation will be printed to stderr,
  * obtained from libexplain_connect(3), and
  * then the process terminates by calling
  * exit(EXIT_FAILURE).
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * libexplain_connect_or_die(fildes, serv_addr, serv_addr_size);
  * @endcode
  *
  * @param fildes
  *     The fildes, exactly as to be passed to the connect(2) system call.
  * @param serv_addr
  *     The serv_addr, exactly as to be passed to the connect(2) system call.
  * @param serv_addr_size
  *     The serv_addr_size, exactly as to be passed to the connect(2)
  *     system call.
  * @returns
  *     This function only returns on success.
  *     On failure, prints an explanation and exits,
  *     it does not return.
  */
void libexplain_connect_or_die(int fildes, const struct sockaddr *serv_addr,
    int serv_addr_size);

/**
  * The libexplain_connect function is used to
  * obtain an explanation of an error returned by the
  * connect(2) system call.
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
  * if (connect(fildes, serv_addr, serv_addr_size) < 0)
  * {
  *     fprintf(stderr, "%s\n", libexplain_connect(fildes,
  *         serv_addr, serv_addr_size));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param fildes
  *     The original fildes, exactly as passed to the connect(2) system call.
  * @param serv_addr
  *     The original serv_addr, exactly as passed to the connect(2) system call.
  * @param serv_addr_size
  *     The original serv_addr_size, exactly as passed to the connect(2)
  *     system call.
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
const char *libexplain_connect(int fildes, const struct sockaddr *serv_addr,
    int serv_addr_size)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The libexplain_errno_connect function is
  * used to obtain an explanation of an error returned by
  * the connect(2) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * if (connect(fildes, serv_addr, serv_addr_size) < 0)
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_connect(err,
  *         fildes, serv_addr, serv_addr_size));
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
  *     The original fildes, exactly as passed to the connect(2) system call.
  * @param serv_addr
  *     The original serv_addr, exactly as passed to the connect(2) system call.
  * @param serv_addr_size
  *     The original serv_addr_size, exactly as passed to the connect(2)
  *     system call.
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
const char *libexplain_errno_connect(int errnum, int fildes,
    const struct sockaddr *serv_addr, int serv_addr_size)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The libexplain_message_connect function is
  * used to obtain an explanation of an error returned by
  * the connect(2) system call.
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
  * if (connect(fildes, serv_addr, serv_addr_size) < 0)
  * {
  *     char message[3000];
  *     libexplain_message_connect(message, sizeof(message),
  *         fildes, serv_addr, serv_addr_size);
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
  *     The original fildes, exactly as passed to the connect(2) system call.
  * @param serv_addr
  *     The original serv_addr, exactly as passed to the connect(2) system call.
  * @param serv_addr_size
  *     The original serv_addr_size, exactly as passed to the connect(2)
  *     system call.
  */
void libexplain_message_connect(char *message, int message_size, int fildes,
    const struct sockaddr *serv_addr, int serv_addr_size);

/**
  * The libexplain_message_errno_connect
  * function is used to obtain an explanation of an error
  * returned by the
  * connect(2) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * if (connect(fildes, serv_addr, serv_addr_size) < 0)
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_connect(message, sizeof(message), err,
  *         fildes, serv_addr, serv_addr_size);
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
  *     The original fildes, exactly as passed to the connect(2) system call.
  * @param serv_addr
  *     The original serv_addr, exactly as passed to the connect(2) system call.
  * @param serv_addr_size
  *     The original serv_addr_size, exactly as passed to the connect(2)
  *     system call.
  */
void libexplain_message_errno_connect(char *message, int message_size,
    int errnum, int fildes, const struct sockaddr *serv_addr,
    int serv_addr_size);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_CONNECT_H */
