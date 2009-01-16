/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#ifndef LIBEXPLAIN_IOCTL_H
#define LIBEXPLAIN_IOCTL_H

/**
  * @file
  * @brief explain ioctl(2) errors
  */

#include <libexplain/warn_unused_result.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
  * The libexplain_ioctl_or_die function is
  * used to call the ioctl(2) system call.  On
  * failure an explanation will be printed to stderr,
  * obtained from libexplain_ioctl(3), and
  * then the process terminates by calling
  * exit(EXIT_FAILURE).
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * libexplain_ioctl_or_die(fildes, request, data);
  * @endcode
  *
  * @param fildes
  *     The fildes, exactly as to be passed to the ioctl(2) system call.
  * @param request
  *     The request, exactly as to be passed to the ioctl(2) system call.
  * @param data
  *     The data, exactly as to be passed to the ioctl(2) system call.
  * @returns
  *     This function only returns on success, see ioctl(2) for more
  *     information.  On failure, prints an explanation and exit()s, it does
  *     not return.
  */
int libexplain_ioctl_or_die(int fildes, int request, void *data);

/**
  * The libexplain_ioctl function is used to
  * obtain an explanation of an error returned by the
  * ioctl(2) system call.
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
  * if (ioctl(fildes, request, data) < 0)
  * {
  *     fprintf(stderr, "%s\n", libexplain_ioctl(fildes, request, data));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * The above code example is available pre-packaged as
  * the #libexplain_ioctl_or_die function.
  *
  * @param fildes
  *     The original fildes, exactly as passed to the ioctl(2) system call.
  * @param request
  *     The original request, exactly as passed to the ioctl(2) system call.
  * @param data
  *     The original data, exactly as passed to the ioctl(2) system call.
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
const char *libexplain_ioctl(int fildes, int request, void *data)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The libexplain_errno_ioctl function is
  * used to obtain an explanation of an error returned by
  * the ioctl(2) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * if (ioctl(fildes, request, data) < 0)
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_ioctl(err, fildes, request, data));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * The above code example is available pre-packaged as
  * the #libexplain_ioctl_or_die function.
  *
  * @param errnum
  *     The error value to be decoded, usually obtained
  *     from the errno global variable just before this
  *     function is called.  This is necessary if you need
  *     to call <b>any</b> code between the system call to
  *     be explained and this function, because many libc
  *     functions will alter the value of errno.
  * @param fildes
  *     The original fildes, exactly as passed to the ioctl(2) system call.
  * @param request
  *     The original request, exactly as passed to the ioctl(2) system call.
  * @param data
  *     The original data, exactly as passed to the ioctl(2) system call.
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
const char *libexplain_errno_ioctl(int errnum, int fildes, int request,
    void *data)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The libexplain_message_ioctl function is
  * used to obtain an explanation of an error returned by
  * the ioctl(2) system call.
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
  * if (ioctl(fildes, request, data) < 0)
  * {
  *     char message[3000];
  *     libexplain_message_ioctl(message, sizeof(message),
  *         fildes, request, data);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * The above code example is available pre-packaged as
  * the #libexplain_ioctl_or_die function.
  *
  * @param message
  *     The location in which to store the returned
  *     message.  If a suitable message return buffer is
  *     supplied, this function is thread safe.
  * @param message_size
  *     The size in bytes of the location in which to
  *     store the returned message.
  * @param fildes
  *     The original fildes, exactly as passed to the ioctl(2) system call.
  * @param request
  *     The original request, exactly as passed to the ioctl(2) system call.
  * @param data
  *     The original data, exactly as passed to the ioctl(2) system call.
  */
void libexplain_message_ioctl(char *message, int message_size, int fildes,
    int request, void *data);

/**
  * The libexplain_message_errno_ioctl
  * function is used to obtain an explanation of an error
  * returned by the
  * ioctl(2) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * if (ioctl(fildes, request, data) < 0)
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_ioctl(message, sizeof(message),
  *         err, fildes, request, data);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * The above code example is available pre-packaged as
  * the #libexplain_ioctl_or_die function.
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
  *     The original fildes, exactly as passed to the ioctl(2) system call.
  * @param request
  *     The original request, exactly as passed to the ioctl(2) system call.
  * @param data
  *     The original data, exactly as passed to the ioctl(2) system call.
  */
void libexplain_message_errno_ioctl(char *message, int message_size, int errnum,
    int fildes, int request, void *data);

#ifdef __cplusplus
}
#endif

/* vim:ts=8:sw=4:et */
#endif /* LIBEXPLAIN_IOCTL_H */
