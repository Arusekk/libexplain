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

#ifndef LIBEXPLAIN_FCLOSE_H
#define LIBEXPLAIN_FCLOSE_H

#include <libexplain/warn_unused_result.h>
#include <libexplain/large_file_support.h>

/**
  * @file
  * @brief explain fclose(3) errors
  */

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
  * The libexplain_fclose_or_die function is used to fflush(3) and
  * fclose(3) the given stream.  If there is an error, it will be
  * reported using libexplain_fclose, and then terminates by calling
  * exit(EXIT_FAILURE).
  *
  * @code
  * libexplain_fclose_or_die(fp);
  * @endcode
  *
  * @param fp
  *     The fp, exactly as to be passed to the fclose(3) system call.
  * @returns
  *     Only returns on success.  Reports error and process exits on failure.
  */
void libexplain_fclose_or_die(FILE *fp);

/**
  * The libexplain_fclose function is used to obtain an
  * explanation of an error returned by the fclose(3) function.  The
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
  * if (fclose(fp))
  * {
  *     fprintf(stderr, "%s\n", libexplain_fclose(fp));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param fp
  *     The original fp, exactly as passed to the fclose(3) system call.
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
  * @note
  *     This function may be of little diagnostic value, because libc
  *     may have destroyed any useful context, leaving nothing for
  *     libexplain to work with (this is true of glibc in particular).
  *     For files that are open for writing, you will obtain more useful
  *     information by first calling fflush(3), as in the
  *     following example
  * @code
  * if (fflush(fp))
  * {
  *     fprintf(stderr, "%s\n", libexplain_fflush(fp));
  *     exit(EXIT_FAILURE);
  * }
  * if (fclose(fp))
  * {
  *     fprintf(stderr, "%s\n", libexplain_fclose(fp));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  * Alternatively, the libexplain_fclose_or_die function can do all that
  * for you.
  */
const char *libexplain_fclose(FILE *fp)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The libexplain_errno_fclose function is used to obtain an explanation
  * of an error returned by the fclose(3) function.  The least the message
  * will contain is the value of strerror(errnum), but usually it will do
  * much better, and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (fclose(fp))
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_errno_fclose(err, fp));
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
  * @param fp
  *     The original fp, exactly as passed to the fclose(3) system call.
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
  * @note
  *     This function may be of little diagnostic value, because libc
  *     may have destroyed any useful context, leaving nothing for
  *     libexplain to work with (this is true of glibc in particular).
  *     For files that are open for writing, you will obtain more useful
  *     information by first calling fflush(3), as in the
  *     following example
  * @code
  * if (fflush(fp))
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_errno_fflush(err, fp));
  *     exit(EXIT_FAILURE);
  * }
  * if (fclose(fp))
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_errno_fclose(err, fp));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  */
const char *libexplain_errno_fclose(int errnum, FILE *fp)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The libexplain_message_fclose function is used to obtain an
  * explanation of an error returned by the fclose(3) function.  The
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
  * if (fclose(fp))
  * {
  *     char message[3000];
  *     libexplain_message_fclose(message, sizeof(message), fp);
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
  * @param fp
  *     The original fp, exactly as passed to the fclose(3) system call.
  * @note
  *     This function may be of little diagnostic value, because libc
  *     may have destroyed any useful context, leaving nothing for
  *     libexplain to work with (this is true of glibc in particular).
  *     For files that are open for writing, you will obtain more useful
  *     information by first calling fflush(3), as in the
  *     following example
  * @code
  * if (fflush(fp))
  * {
  *     char message[3000];
  *     libexplain_message_fflush(message, sizeof(message), fp);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
  * }
  * if (fclose(fp))
  * {
  *     char message[3000];
  *     libexplain_message_fclose(message, sizeof(message), fp);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  */
void libexplain_message_fclose(char *message, int message_size, FILE *fp);

/**
  * The libexplain_message_errno_fclose function is used to obtain an
  * explanation of an error returned by the fclose(3) function.  The
  * least the message will contain is the value of strerror(errnum), but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following exameple:
  * @code
  * if (fclose(fp))
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_fclose(message, sizeof(message), err, fp);
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
  * @param fp
  *     The original fp, exactly as passed to the fclose(3) system call.
  * @note
  *     This function may be of little diagnostic value, because libc
  *     may have destroyed any useful context, leaving nothing for
  *     libexplain to work with (this is true of glibc in particular).
  *     For files that are open for writing, you will obtain more useful
  *     information by first calling fflush(3), as in the
  *     following example
  * @code
  * if (fflush(fp))
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_fflush(message, sizeof(message), err, fp);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
  * }
  * if (fclose(fp))
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_fclose(message, sizeof(message), err, fp);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  */
void libexplain_message_errno_fclose(char *message, int message_size,
    int errnum, FILE *fp);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_FCLOSE_H */
