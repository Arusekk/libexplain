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

#ifndef LIBEXPLAIN_WAIT3_H
#define LIBEXPLAIN_WAIT3_H

/**
  * @file
  * @brief explain wait3(2) errors
  */

#ifdef __cplusplus
extern "C" {
#endif

struct rusage; /* forward */

/**
  * The libexplain_wait3_or_die function is
  * used to call the wait3(2) system call.  On
  * failure an explanation will be printed to stderr,
  * obtained from libexplain_wait3(3), and
  * then the process terminates by calling
  * exit(EXIT_FAILURE).
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * int pid = libexplain_wait3_or_die(status, options, rusage);
  * @endcode
  *
  * @param status
  *     The status, exactly as to be passed to the wait3(2) system call.
  * @param options
  *     The options, exactly as to be passed to the wait3(2) system call.
  * @param rusage
  *     The rusage, exactly as to be passed to the wait3(2) system call.
  * @returns
  *     This function only returns on success, see wait(3) for more information.
  *     On failure, prints an explanation and exits,
  *     it does not return.
  */
int libexplain_wait3_or_die(int *status, int options, struct rusage *rusage);

/**
  * The libexplain_wait3 function is used to
  * obtain an explanation of an error returned by the
  * wait3(2) system call.
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
  * int pid = wait3(status, options, rusage);
  * if (pid < 0)
  * {
  *     fprintf(stderr, "%s\n", libexplain_wait3(status, options, rusage));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param status
  *     The original status, exactly as passed to the wait3(2) system call.
  * @param options
  *     The original options, exactly as passed to the wait3(2) system call.
  * @param rusage
  *     The original rusage, exactly as passed to the wait3(2) system call.
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
const char *libexplain_wait3(int *status, int options, struct rusage *rusage);

/**
  * The libexplain_errno_wait3 function is
  * used to obtain an explanation of an error returned by
  * the wait3(2) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * int pid = wait3(status, options, rusage);
  * if (pid < 0)
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_wait3(err, status, options, rusage));
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
  * @param status
  *     The original status, exactly as passed to the wait3(2) system call.
  * @param options
  *     The original options, exactly as passed to the wait3(2) system call.
  * @param rusage
  *     The original rusage, exactly as passed to the wait3(2) system call.
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
const char *libexplain_errno_wait3(int errnum, int *status, int options,
    struct rusage *rusage);

/**
  * The libexplain_message_wait3 function is
  * used to obtain an explanation of an error returned by
  * the wait3(2) system call.
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
  * int pid = wait3(status, options, rusage);
  * if (pid < 0)
  * {
  *     char message[3000];
  *     libexplain_message_wait3(message, sizeof(message),
  *         status, options, rusage);
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
  * @param status
  *     The original status, exactly as passed to the wait3(2) system call.
  * @param options
  *     The original options, exactly as passed to the wait3(2) system call.
  * @param rusage
  *     The original rusage, exactly as passed to the wait3(2) system call.
  */
void libexplain_message_wait3(char *message, int message_size, int *status,
    int options, struct rusage *rusage);

/**
  * The libexplain_message_errno_wait3
  * function is used to obtain an explanation of an error
  * returned by the
  * wait3(2) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * int pid = wait3(status, options, rusage);
  * if (pid < 0)
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_wait3(message, sizeof(message), err,
  *         status, options, rusage);
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
  * @param status
  *     The original status, exactly as passed to the wait3(2) system call.
  * @param options
  *     The original options, exactly as passed to the wait3(2) system call.
  * @param rusage
  *     The original rusage, exactly as passed to the wait3(2) system call.
  */
void libexplain_message_errno_wait3(char *message, int message_size, int errnum,
    int *status, int options, struct rusage *rusage);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_WAIT3_H */
