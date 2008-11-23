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

#ifndef LIBEXPLAIN_WAITPID_H
#define LIBEXPLAIN_WAITPID_H

/**
  * @file
  * @brief explain waitpid(2) errors
  */

#ifdef __cplusplus
extern "C" {
#endif

/**
  * The libexplain_waitpid_or_die function is
  * used to call the waitpid(2) system call.  On
  * failure an explanation will be printed to stderr,
  * obtained from libexplain_waitpid(3), and
  * then the process terminates by calling
  * exit(EXIT_FAILURE).
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * int pif = libexplain_waitpid_or_die(pid, status, options);
  * @endcode
  *
  * @param pid
  *     The pid, exactly as to be passed to the waitpid(2) system call.
  * @param status
  *     The status, exactly as to be passed to the waitpid(2) system call.
  * @param options
  *     The options, exactly as to be passed to the waitpid(2) system call.
  * @returns
  *     This function only returns on success, see waitpid(2) for more
  *     information.  On failure, prints an explanation and exits, it
  *     does not return.
  */
int libexplain_waitpid_or_die(int pid, int *status, int options);

/**
  * The libexplain_waitpid function is used to
  * obtain an explanation of an error returned by the
  * waitpid(2) system call.
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
  * if (waitpid(pid, status, options) < 0)
  * {
  *     fprintf(stderr, "%s\n", libexplain_waitpid(pid, status, options));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param pid
  *     The original pid, exactly as passed to the waitpid(2) system call.
  * @param status
  *     The original status, exactly as passed to the waitpid(2) system call.
  * @param options
  *     The original options, exactly as passed to the waitpid(2) system call.
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
const char *libexplain_waitpid(int pid, int *status, int options);

/**
  * The libexplain_errno_waitpid function is
  * used to obtain an explanation of an error returned by
  * the waitpid(2) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * if (waitpid(pid, status, options) < 0)
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_waitpid(err, pid, status, options));
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
  * @param pid
  *     The original pid, exactly as passed to the waitpid(2) system call.
  * @param status
  *     The original status, exactly as passed to the waitpid(2) system call.
  * @param options
  *     The original options, exactly as passed to the waitpid(2) system call.
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
const char *libexplain_errno_waitpid(int errnum, int pid, int *status,
    int options);

/**
  * The libexplain_message_waitpid function is
  * used to obtain an explanation of an error returned by
  * the waitpid(2) system call.
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
  * if (waitpid(pid, status, options) < 0)
  * {
  *     char message[3000];
  *     libexplain_message_waitpid(message, sizeof(message),
  *         pid, status, options);
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
  * @param pid
  *     The original pid, exactly as passed to the waitpid(2) system call.
  * @param status
  *     The original status, exactly as passed to the waitpid(2) system call.
  * @param options
  *     The original options, exactly as passed to the waitpid(2) system call.
  */
void libexplain_message_waitpid(char *message, int message_size, int pid,
    int *status, int options);

/**
  * The libexplain_message_errno_waitpid
  * function is used to obtain an explanation of an error
  * returned by the
  * waitpid(2) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * if (waitpid(pid, status, options) < 0)
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_waitpid(message, sizeof(message), err,
  *         pid, status, options);
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
  * @param pid
  *     The original pid, exactly as passed to the waitpid(2) system call.
  * @param status
  *     The original status, exactly as passed to the waitpid(2) system call.
  * @param options
  *     The original options, exactly as passed to the waitpid(2) system call.
  */
void libexplain_message_errno_waitpid(char *message, int message_size,
    int errnum, int pid, int *status, int options);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_WAITPID_H */
