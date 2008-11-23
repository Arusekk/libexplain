/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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

#ifndef LIBEXPLAIN_FCNTL_H
#define LIBEXPLAIN_FCNTL_H

/**
  * @file
  * @brief explain fcntl(2) errors
  */

#ifdef __cplusplus
extern "C" {
#endif

/**
  * The libexplain_fcntl_or_die function is
  * used to call the fcntl(2) system call.  On
  * failure an explanation will be printed to stderr,
  * obtained from libexplain_fcntl(3), and
  * then the process terminates by calling exit(EXIT_FAILURE).
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * int result = libexplain_fcntl_or_die(fildes, command, arg);
  * @endcode
  *
  * @param fildes
  *     The fildes, exactly as to be passed to the fcntl(2) system call.
  * @param command
  *     The command, exactly as to be passed to the fcntl(2) system call.
  * @param arg
  *     The arg, exactly as to be passed to the fcntl(2) system call.
  * @returns
  *     This function only returns on success, and it returns whatever was
  *     returned by the fcntl(2) call; depending on the command, this
  *     may have no use.  On failure, prints an explanation and exits.
  */
int libexplain_fcntl_or_die(int fildes, int command, long arg);

/**
  * The libexplain_fcntl function is used to obtain an explanation of an
  * error returned by the fcntl(2) system call.  The least the message
  * will contain is the value of strerror(errno), but usually it will do
  * much better, and indicate the underlying cause in more detail.
  *
  * The errno global variable will be used to obtain the error value to
  * be decoded.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * int ok = fcntl(fd, command, arg);
  * if (ok < 0)
  * {
  *     fprintf(stderr, "%s\n", libexplain_fcntl(fd, command, arg));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param fildes
  *     The original fildes, exactly as passed to the fcntl(2) system call.
  * @param command
  *     The original command, exactly as passed to the fcntl(2) system call.
  * @param arg
  *     The original arg, exactly as passed to the fcntl(2) system call
  *     (or zero if the original call didn't need a mode argument, or
  *     cast to long if it was a pointer).
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
const char *libexplain_fcntl(int fildes, int command, long arg);

/**
  * The libexplain_errno_fcntl function is used to obtain an explanation
  * of an error returned by the fcntl(2) system call.  The least the
  * message will contain is the value of strerror(errnum), but usually
  * it will do much better, and indicate the underlying cause in more
  * detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * int ok = fcntl(fd, command, arg);
  * if (ok < 0)
  * {
  *     int err = errno;
  *     fprintf
  *     (
  *         stderr,
  *         "%s\n",
  *         libexplain_errno_fcntl(errnum, fd, command, arg)
  *     );
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
  * @param fildes
  *     The original fildes, exactly as passed to the fcntl(2) system call.
  * @param command
  *     The original command, exactly as passed to the fcntl(2) system call.
  * @param arg
  *     The original arg, exactly as passed to the fcntl(2) system call
  *     (or zero if the original call didn't need a mode argument, or
  *     cast to long if it was a pointer).
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
const char *libexplain_errno_fcntl(int errnum, int fildes, int command,
    long arg);

/**
  * The libexplain_message_fcntl function is used to obtain an
  * explanation of an error returned by the fcntl(2) system call.  The
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
  * int ok = fcntl(fd, cmd, arg);
  * if (ok < 0)
  * {
  *     char message[3000];
  *     libexplain_message_fcntl(message, sizeof(message), fd, cmd, arg);
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
  * @param fildes
  *     The original fildes, exactly as passed to the fcntl(2) system call.
  * @param command
  *     The original command, exactly as passed to the fcntl(2) system call.
  * @param arg
  *     The original arg, exactly as passed to the fcntl(2) system call
  *     (or zero if the original call didn't need a mode argument, or
  *     cast to long if it was a pointer).
  */
void libexplain_message_fcntl(char *message, int message_size, int fildes,
    int command, long arg);

/**
  * The libexplain_message_errno_fcntl function is used to obtain an
  * explanation of an error returned by the fcntl(2) system call.  The
  * least the message will contain is the value of strerror(errnum), but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * int ok = fcntl(fd, command, arg);
  * if (ok < 0)
  * {
  *     int errnum = errno;
  *     char message[3000];
  *     libexplain_message_fcntl(message, sizeof(message), errnum, fd,
  *         command, arg);
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
  * @param fildes
  *     The original fildes, exactly as passed to the fcntl(2) system call.
  * @param command
  *     The original command, exactly as passed to the fcntl(2) system call.
  * @param arg
  *     The original arg, exactly as passed to the fcntl(2) system call
  *     (or zero if the original call didn't need a mode argument, or
  *     cast to long if it was a pointer).
  */
void libexplain_message_errno_fcntl(char *message, int message_size,
    int errnum, int fildes, int command, long arg);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_FCNTL_H */
