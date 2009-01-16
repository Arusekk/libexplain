/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#ifndef LIBEXPLAIN_REALLOC_H
#define LIBEXPLAIN_REALLOC_H

/**
  * @file
  * @brief explain realloc(3) errors
  */

#include <libexplain/warn_unused_result.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/**
  * The libexplain_realloc_or_die function is
  * used to call the realloc(3) system call.  On
  * failure an explanation will be printed to stderr,
  * obtained from libexplain_realloc(3), and
  * then the process terminates by calling
  * exit(EXIT_FAILURE).
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * void *new_ptr = libexplain_realloc_or_die(ptr, size);
  * @endcode
  *
  * @param ptr
  *     The ptr, exactly as to be passed to the realloc(3) system call.
  * @param size
  *     The size, exactly as to be passed to the realloc(3) system call.
  * @returns
  *     This function only returns on success, see realloc(3) for more
  *     information.  On failure, prints an explanation and exits, it
  *     does not return.
  */
void *libexplain_realloc_or_die(void *ptr, size_t size);

/**
  * The libexplain_realloc function is used to
  * obtain an explanation of an error returned by the
  * realloc(3) system call.
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
  * void *new_ptr = realloc(ptr, size);
  * if (!new_ptr)
  * {
  *     fprintf(stderr, "%s\n", libexplain_realloc(ptr, size));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * The above code example is available pre-packaged as
  * the #libexplain_realloc_or_die function.
  *
  * @param ptr
  *     The original ptr, exactly as passed to the realloc(3) system call.
  * @param size
  *     The original size, exactly as passed to the realloc(3) system call.
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
const char *libexplain_realloc(void *ptr, size_t size)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The libexplain_errno_realloc function is
  * used to obtain an explanation of an error returned by
  * the realloc(3) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * void *new_ptr = realloc(ptr, size);
  * if (!new_ptr)
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_realloc(err, ptr, size));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * The above code example is available pre-packaged as
  * the #libexplain_realloc_or_die function.
  *
  * @param errnum
  *     The error value to be decoded, usually obtained
  *     from the errno global variable just before this
  *     function is called.  This is necessary if you need
  *     to call <b>any</b> code between the system call to
  *     be explained and this function, because many libc
  *     functions will alter the value of errno.
  * @param ptr
  *     The original ptr, exactly as passed to the realloc(3) system call.
  * @param size
  *     The original size, exactly as passed to the realloc(3) system call.
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
const char *libexplain_errno_realloc(int errnum, void *ptr, size_t size)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The libexplain_message_realloc function is
  * used to obtain an explanation of an error returned by
  * the realloc(3) system call.
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
  * void *new_ptr = realloc(ptr, size);
  * if (!new_ptr)
  * {
  *     char message[3000];
  *     libexplain_message_realloc(message, sizeof(message), ptr, size);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * The above code example is available pre-packaged as
  * the #libexplain_realloc_or_die function.
  *
  * @param message
  *     The location in which to store the returned
  *     message.  If a suitable message return buffer is
  *     supplied, this function is thread safe.
  * @param message_size
  *     The size in bytes of the location in which to
  *     store the returned message.
  * @param ptr
  *     The original ptr, exactly as passed to the realloc(3) system call.
  * @param size
  *     The original size, exactly as passed to the realloc(3) system call.
  */
void libexplain_message_realloc(char *message, int message_size, void *ptr,
    size_t size);

/**
  * The libexplain_message_errno_realloc
  * function is used to obtain an explanation of an error
  * returned by the
  * realloc(3) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * void *new_ptr = realloc(ptr, size);
  * if (!new_ptr)
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_realloc(message, sizeof(message),
  *         err, ptr, size);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * The above code example is available pre-packaged as
  * the #libexplain_realloc_or_die function.
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
  * @param ptr
  *     The original ptr, exactly as passed to the realloc(3) system call.
  * @param size
  *     The original size, exactly as passed to the realloc(3) system call.
  */
void libexplain_message_errno_realloc(char *message, int message_size,
    int errnum, void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

/* vim:ts=8:sw=4:et */
#endif /* LIBEXPLAIN_REALLOC_H */
