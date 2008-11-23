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

#ifndef LIBEXPLAIN_ACCESS_H
#define LIBEXPLAIN_ACCESS_H

/**
  * @file
  * @brief explain access(2) errors
  */

#ifdef __cplusplus
extern "C" {
#endif

/**
  * The libexplain_access_or_die function is used to call the access(2)
  * system call and check the result.  On failure it prints an
  * explanation for the error, obtained from libexplain_access(3), and
  * then terminates by calling exit(EXIT_FAILURE).
  *
  * @code
  * libexplain_access_or_die(pathname, mode);
  * @endcode
  *
  * @param pathname
  *     The pathname, exactly as to be passed to the access(2) system call.
  * @param mode
  *     The mode, exactly as to be passed to the access(2) system call.
  * @returns
  *     Only ever return on success.
  *     On failure process will exit.
  */
void libexplain_access_or_die(const char *pathname, int mode);

/**
  * The libexplain_access function is used to obtain an explanation
  * of an error returned by the access(2) system call.  The least the
  * message will contain is the value of strerror(errno), but usually
  * it will do much better, and indicate the underlying cause in more
  * detail.
  *
  * The errno global variable will be used to obtain the error value to
  * be decoded.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * int fd = access(pathname, mode);
  * if (fd < 0)
  * {
  *     fprintf(stderr, "%s\n", libexplain_access(pathname, mode));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param pathname
  *     The original pathname, exactly as passed to the access(2) system call.
  * @param mode
  *     The original mode, exactly as passed to the access(2) system call.
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
const char *libexplain_access(const char *pathname, int mode);

/**
  * The libexplain_message_access function is used to obtain an
  * explanation of an error returned by the access(2) system call.  The
  * least the message will contain is the value of strerror(errno), but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * The errno global variable will be used to obtain the error value to
  * be decoded.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * int fd = access(pathname, mode);
  * if (fd < 0)
  * {
  *     char message[3000];
  *     libexplain_message_access(message, sizeof(message), pathname,
  *         mode);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
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
  * @param pathname
  *     The original pathname, exactly as passed to the access(2) system call.
  * @param mode
  *     The original mode, exactly as passed to the access(2) system call.
  */
void libexplain_message_access(char *message, int message_size,
    const char *pathname, int mode);

/**
  * The libexplain_errno_access function is used to obtain an explanation
  * of an error returned by the access(2) system call.  The least the message
  * will contain is the value of strerror(errnum), but usually it will do
  * much better, and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * int fd = access(pathname, mode);
  * if (fd < 0)
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_errno_access(err, pathname,
  *         mode));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param errnum
  *     The error value to be decoded, usually obtained from the errno
  *     global variable just before this function is called.  This
  *     is necessary if you need to call <b>any</b> code between the
  *     system call to be explained and this function, because many libc
  *     functions will alter the value of errno.
  * @param pathname
  *     The original pathname, exactly as passed to the access(2) system call.
  * @param mode
  *     The original mode, exactly as passed to the access(2) system call.
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
const char *libexplain_errno_access(int errnum, const char *pathname, int mode);

/**
  * The libexplain_message_errno_access function is used to obtain an
  * explanation of an error returned by the access(2) system call.  The
  * least the message will contain is the value of strerror(errnum), but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following exameple:
  * @code
  * int fd = access(pathname, mode);
  * if (fd < 0)
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_access(message, sizeof(message), err,
  *         pathname, mode);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
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
  *     The error value to be decoded, usually obtained from the errno
  *     global variable just before this function is called.  This
  *     is necessary if you need to call <b>any</b> code between the
  *     system call to be explained and this function, because many libc
  *     functions will alter the value of errno.
  * @param pathname
  *     The original pathname, exactly as passed to the access(2) system call.
  * @param mode
  *     The original mode, exactly as passed to the access(2) system call.
  */
void libexplain_message_errno_access(char *message, int message_size,
    int errnum, const char *pathname, int mode);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_ACCESS_H */
