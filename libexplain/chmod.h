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

#ifndef LIBEXPLAIN_CHMOD_H
#define LIBEXPLAIN_CHMOD_H

/**
  * @file
  * @brief explain chmod(2) errors
  */

#ifdef __cplusplus
extern "C" {
#endif

/**
  * The libexplain_chmod_or_die function is used to call the chmod(2)
  * system call.  On failure An explanation wiil be printed to stderr,
  * obtained from libexplain_chmod, and the the process terminates by
  * call ing exit(EXIT_FAILURE).
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * libexplain_chmod_or_die(pathname, mode);
  * @endcode
  *
  * @param pathname
  *     The pathname, exactly as to be passed to the chmod(2) system call.
  * @param mode
  *     The mode, exactly as to be passed to the chmod(2) system call.
  * @returns
  *     This function only returns on success.
  *     On failure, prints an explanation and exit(EXIT_FAILURE)s.
  */
void libexplain_chmod_or_die(const char *pathname, int mode);

/**
  * The libexplain_chmod function is used to obtain an explanation
  * of an error returned by the chmod(2) system call.  The least the
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
  * if (chmod(pathname, mode) < 0)
  * {
  *     fprintf(stderr, "%s\n", libexplain_chmod(pathname, mode));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param pathname
  *     The original pathname, exactly as passed to the chmod(2) system call.
  * @param mode
  *     The original mode, exactly as passed to the chmod(2) system call.
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
const char *libexplain_chmod(const char *pathname, int mode);

/**
  * The libexplain_errno_chmod function is used to obtain an
  * explanation of an error returned by the chmod(2) system call.  The
  * least the message will contain is the value of strerror(errnum), but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (chmod(pathname, mode) < 0)
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_errno_chmod(err, pathname,
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
  *     The original pathname, exactly as passed to the chmod(2) system call.
  * @param mode
  *     The original mode, exactly as passed to the chmod(2) system call.
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
const char *libexplain_errno_chmod(int errnum, const char *pathname, int mode);

/**
  * The libexplain_message_chmod function is used to obtain an
  * explanation of an error returned by the chmod(2) system call.  The
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
  * if (chmod(pathname, mode) < 0)
  * {
  *     char message[3000];
  *     libexplain_message_chmod(message, sizeof(message), pathname, mode);
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
  *     The original pathname, exactly as passed to the chmod(2) system call.
  * @param mode
  *     The original mode, exactly as passed to the chmod(2) system call.
  */
void libexplain_message_chmod(char *message, int message_size,
    const char *pathname, int mode);

/**
  * The libexplain_message_errno_chmod function is used to obtain an
  * explanation of an error returned by the chmod(2) system call.  The
  * least the message will contain is the value of strerror(errnum), but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (chmod(pathname, mode) < 0)
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_chmod(message, sizeof(message), err,
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
  *     The original pathname, exactly as passed to the chmod(2) system call.
  * @param mode
  *     The original mode, exactly as passed to the chmod(2) system call.
  */
void libexplain_message_errno_chmod(char *message, int message_size,
    int errnum, const char *pathname, int mode);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_CHMOD_H */
