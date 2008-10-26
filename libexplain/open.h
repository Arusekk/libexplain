/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEXPLAIN_OPEN_H
#define LIBEXPLAIN_OPEN_H

/**
  * @file
  * @brief explain open(2) errors
  */

#ifdef c_plus_plus
extern "C" {
#endif

/**
  * The libexplain_open_or_die function is used to open a file using
  * open(3).  If an error occurs, this will be reported to stderr, and
  * then the function will call exit(1) to terminate the process.  This
  * presents a quick and simple way to have good error reporting from
  * command line programs.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * int fd = libexplain_open_or_die(pathname, flags, mode);
  * @endcode
  *
  * @param pathname
  *     The pathname to open, exactly as will be passed to the open(2)
  *     system call.
  * @param flags
  *     The open flags, exactly as will be passed to the open(3) system call.
  * @param mode
  *     The new file modes, exactly as will be passed to the open(3)
  *     system call.
  * @returns
  *     The open file descriptor.
  *     Does not return on failure.
  * @note
  *     If the open fails, the libexplain_open(3) function will be called,
  *     and the process will exit via a exit(1) call.
  */
int libexplain_open_or_die(const char *pathname, int flags, int mode);

/**
  * The libexplain_open function is used to obtain an
  * explanation of an error returned by the open(2) system call.  The
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
  * int fd = open(pathname, flags, mode);
  * if (fd < 0)
  * {
  *     fprintf(stderr, "%s\n", libexplain_open(pathname, flags, mode));
  *     exit(1);
  * }
  * @endcode
  *
  * @param pathname
  *     The original pathname, exactly as passed to the open(2) system call.
  * @param flags
  *     The original flags, exactly as passed to the open(2) system call.
  * @param mode
  *     The original mode, exactly as passed to the open(2) system call (or
  *     zero if the original call didn't need a mode argument).
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
const char *libexplain_open(const char *pathname, int flags, int mode);

/**
  * The libexplain_message_open function is used to obtain an
  * explanation of an error returned by the open(2) system call.  The
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
  * int fd = open(pathname, flags, mode);
  * if (fd < 0)
  * {
  *     char message[3000];
  *     libexplain_message_open(message, sizeof(message), pathname, flags,
  *         mode);
  *     fprintf(stderr, "%s\n", message);
  *     exit(1);
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
  *     The original pathname, exactly as passed to the open(2) system call.
  * @param flags
  *     The original flags, exactly as passed to the open(2) system call.
  * @param mode
  *     The original mode, exactly as passed to the open(2) system call (or
  *     zero if the original call didn't need a mode argument).
  */
void libexplain_message_open(char *message, int message_size,
    const char *pathname, int flags, int mode);

/**
  * The libexplain_errno_open function is used to obtain an explanation
  * of an error returned by the open(2) system call.  The least the message
  * will contain is the value of strerror(errnum), but usually it will do
  * much better, and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * int fd = open(pathname, flags, mode);
  * if (fd < 0)
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_errno_open(err, pathname,
  *         flags, mode));
  *     exit(1);
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
  *     The original pathname, exactly as passed to the open(2) system call.
  * @param flags
  *     The original flags, exactly as passed to the open(2) system call.
  * @param mode
  *     The original mode, exactly as passed to the open(2) system call (or
  *     zero if the original call didn't need a mode argument).
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
const char *libexplain_errno_open(int errnum, const char *pathname, int flags,
    int mode);

/**
  * The libexplain_message_errno_open function is used to obtain an
  * explanation of an error returned by the open(2) system call.  The
  * least the message will contain is the value of strerror(errnum), but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following exameple:
  * @code
  * int fd = open(pathname, flags, mode);
  * if (fd < 0)
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_open(message, sizeof(message), err, pathname,
  *         flags, mode);
  *     fprintf(stderr, "%s\n", message);
  *     exit(1);
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
  *     The original pathname, exactly as passed to the open(2) system call.
  * @param flags
  *     The original flags, exactly as passed to the open(2) system call.
  * @param mode
  *     The original mode, exactly as passed to the open(2) system call (or
  *     zero if the original call didn't need a mode argument).
  */
void libexplain_message_errno_open(char *message, int message_size,
    int errnum, const char *pathname, int flags, int mode);

#ifdef c_plus_plus
}
#endif

#endif /* LIBEXPLAIN_OPEN_H */
