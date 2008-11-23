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

#ifndef LIBEXPLAIN_REMOVE_H
#define LIBEXPLAIN_REMOVE_H

/**
  * @file
  * @brief explain remove(2) errors
  */

#ifdef __cplusplus
extern "C" {
#endif

/**
  * The libexplain_remove_or_die function is
  * used to call the remove(2) system call.  On
  * failure an explanation will be printed to stderr,
  * obtained from libexplain_remove(3), and
  * then the process terminates by calling exit(EXIT_FAILURE).
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * libexplain_remove_or_die(pathname);
  * @endcode
  *
  * @param pathname
  *     The pathname, exactly as to be passed to the remove(2) system call.
  * @returns
  *     This function only returns on success.
  *     On failure, prints an explanation and exits.
  */
void libexplain_remove_or_die(const char *pathname);

/**
  * The libexplain_remove function may be used
  * to describe errors returned by the remove()
  * system call.  The least the message will contain is
  * the value of strerror(errno), but usually it will do
  * much better, and indicate the underlying cause in more
  * detail.
  *
  * The errno global variable will be used to obtain the
  * error value to be decoded.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * if (remove(pathname) < 0)
  * {
  *     fprintf(stderr, "%s\n", libexplain_remove(pathname));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param pathname
  *     The original pathname, exactly as passed to the remove(2) system call.
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
const char *libexplain_remove(const char *pathname);

/**
  * The libexplain_errno_remove function may be
  * used to describe errors returned by the
  * remove() system call.  The least the
  * message will contain is the value of strerror(errnum),
  * but usually it will do much better, and indicate the
  * underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * if (remove(pathname) < 0)
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_remove(err, pathname));
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
  * @param pathname
  *     The original pathname, exactly as passed to the remove(2) system call.
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
const char *libexplain_errno_remove(int errnum, const char *pathname);

/**
  * The libexplain_message_remove function may
  * be used to describe errors returned by the
  * remove() system call.  The least the
  * message will contain is the value of strerror(errno),
  * but usually it will do much better, and indicate the
  * underlying cause in more detail.
  *
  * The errno global variable will be used to obtain the
  * error value to be decoded.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * if (remove(pathname) < 0)
  * {
  *     char message[3000];
  *     libexplain_message_remove(message, sizeof(message), pathname);
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
  * @param pathname
  *     The original pathname, exactly as passed to the remove(2) system call.
  */
void libexplain_message_remove(char *message, int message_size,
    const char *pathname);

/**
  * The libexplain_message_errno_remove function may be used to describe
  * errors returned by the remove() system call.  The least the message
  * will contain is the value of strerror(errnum), but usually it will
  * do much better, and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * if (remove(pathname) < 0)
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_remove(message, sizeof(message), err,
  *         pathname);
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
  * @param pathname
  *     The original pathname, exactly as passed to the remove(2) system call.
  */
void libexplain_message_errno_remove(char *message, int message_size,
    int errnum, const char *pathname);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_REMOVE_H */
