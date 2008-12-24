/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#ifndef LIBEXPLAIN_FREOPEN_H
#define LIBEXPLAIN_FREOPEN_H

#include <libexplain/warn_unused_result.h>
#include <libexplain/large_file_support.h>

#include <stdio.h>

/**
  * @file
  * @brief explain freopen(3) errors
  */

#ifdef __cplusplus
extern "C" {
#endif

/**
  * The libexplain_freopen_or_die function is used to reopen a file
  * via the freopen(3) system call.  On failure it will print an
  * explanation, obtained from the linexplain_freopen(3) function, on
  * the standard error stream and then exit.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * libexplain_freopen_or_die(pathname, flags, fp);
  * @endcode
  *
  * @param pathname
  *     The pathname, exactly as to be passed to the freopen(3) system call.
  * @param flags
  *     The flags, exactly as to be passed to the freopen(3) system call.
  * @param fp
  *     The fp, exactly as to be passed to the freopen(3) system call.
  * @returns
  *     Only ever return on success.  Never returns on failure.
  */
void libexplain_freopen_or_die(const char *pathname, const char *flags,
    FILE *fp);

/**
  * The libexplain_freopen function is used to obtain an explanation
  * of an error returned by the freopen(3) system call.  The least the
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
  * if (!freopen(pathname, flags, fp))
  * {
  *     fprintf(stderr, "%s\n", libexplain_freopen(pathname, flags, fp));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param pathname
  *     The original pathname, exactly as passed to the freopen(3) system call.
  * @param flags
  *     The original flags, exactly as passed to the freopen(3) system call.
  * @param fp
  *     The original fp, exactly as passed to the freopen(3) system call.
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
const char *libexplain_freopen(const char *pathname, const char *flags,
    FILE *fp)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The libexplain_errno_freopen function is used to obtain an explanation
  * of an error returned by the freopen(3) system call.  The least the message
  * will contain is the value of strerror(errnum), but usually it will do
  * much better, and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (freopen(pathname, flags, fp))
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_errno_freopen(err, pathname,
  *         flags, fp));
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
  *     The original pathname, exactly as passed to the freopen(3) system call.
  * @param flags
  *     The original flags, exactly as passed to the freopen(3) system call.
  * @param fp
  *     The original fp, exactly as passed to the freopen(3) system call.
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
const char *libexplain_errno_freopen(int errnum, const char *pathname,
    const char *flags, FILE *fp)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The libexplain_message_freopen function is used to obtain an
  * explanation of an error returned by the freopen(3) system call.  The
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
  * if (!freopen(pathname, flags, fp))
  * {
  *     char message[3000];
  *     libexplain_message_freopen(message, sizeof(message), pathname, flags,
  *         fp);
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
  *     The original pathname, exactly as passed to the freopen(3) system call.
  * @param flags
  *     The original flags, exactly as passed to the freopen(3) system call.
  * @param fp
  *     The original fp, exactly as passed to the freopen(3) system call.
  */
void libexplain_message_freopen(char *message, int message_size,
    const char *pathname, const char *flags, FILE *fp);

/**
  * The libexplain_message_errno_freopen function is used to obtain an
  * explanation of an error returned by the freopen(3) system call.  The
  * least the message will contain is the value of strerror(errnum), but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following exameple:
  * @code
  * if (!freopen(pathname, flags, fp))
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_freopen(message, sizeof(message), err,
  *         pathname, flags, fp);
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
  *     The original pathname, exactly as passed to the freopen(3) system call.
  * @param flags
  *     The original flags, exactly as passed to the freopen(3) system call.
  * @param fp
  *     The original fp, exactly as passed to the freopen(3) system call.
  */
void libexplain_message_errno_freopen(char *message, int message_size,
    int errnum, const char *pathname, const char *flags, FILE *fp);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_FREOPEN_H */
