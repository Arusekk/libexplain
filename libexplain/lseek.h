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

#ifndef LIBEXPLAIN_LSEEK_H
#define LIBEXPLAIN_LSEEK_H

/**
  * @file
  * @brief explain lseek(2) errors
  */

#ifdef c_plus_plus
extern "C" {
#endif

/**
  * The libexplain_lseek function may be used to obtain a human readable
  * explanation of what went wrong in an lseek(2) system call.  The
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
  * if (lseek(fd, offset, whence) == (off_t)-1)
  * {
  *     fprintf(stderr, "%s\n", libexplain_lseek(fd, offset, whence);
  *     exit(1);
  * }
  * @endcode
  *
  * @param fildes
  *    The original fildes,
  *    exactly as passed to the lseek(2) system call.
  * @param offset
  *    The original offset,
  *    exactly as passed to the lseek(2) system call.
  * @param whence
  *    The original whence,
  *    exactly as passed to the lseek(2) system call.
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
const char *libexplain_lseek(int fildes, long long offset, int whence);

/**
  * The libexplain_errno_lseek function may be used to obtain a human
  * readable explanation of what went wrong in an lseek(2) system call.
  * The least the message will contain is the value of strerror(errnum),
  * but usually it will do much better, and indicate the underlying
  * cause in more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (lseek(fd, offset, whence) == (off_t)-1)
  * {
  *     int errnum = errno;
  *     fprintf(stderr, "%s\n", libexplain_errno_lseek(fd, eernum, offset,
  *         whence);
  *     exit(1);
  * }
  * @endcode
  *
  * @param errnum
  *    The error value to be decoded, usually obtained from the errno
  *    global variable just before this function is called.  This
  *    is necessary if you need to call <b>any</b> code between the
  *    system call to be explained and this function, because many libc
  *    functions will alter the value of errno.
  * @param fildes
  *    The original fildes,
  *    exactly as passed to the lseek(2) system call.
  * @param offset
  *    The original offset,
  *    exactly as passed to the lseek(2) system call.
  * @param whence
  *    The original whence,
  *    exactly as passed to the lseek(2) system call.
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
const char *libexplain_errno_lseek(int errnum, int fildes, long long offset,
    int whence);

/**
  * The libexplain_message_lseek function may be used to obtain
  * a human readable explanation of what went wrong in an lseek(2)
  * system call.  The least the message will contain is the value of
  * strerror(errno), but usually it will do much better, and indicate
  * the underlying cause in more detail.
  *
  * The errno global variable will be used to obtain the error value to
  * be decoded.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (lseek(fd, offset, whence) == (off_t)-1)
  * {
  *     char message[3000];
  *     libexplain_message_lseek(message, sizeof(message), fd, offset, whence);
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
  * @param fildes
  *    The original fildes,
  *    exactly as passed to the lseek(2) system call.
  * @param offset
  *    The original offset,
  *    exactly as passed to the lseek(2) system call.
  * @param whence
  *    The original whence,
  *    exactly as passed to the lseek(2) system call.
  */
void libexplain_message_lseek(char *message, int message_size, int fildes,
    long long offset, int whence);

/**
  * The libexplain_message_errno_lseek function may be used to obtain
  * a human readable explanation of what went wrong in an lseek(2)
  * system call.  The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better, and indicate
  * the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (lseek(fd, offset, whence) == (off_t)-1)
  * {
  *     char message[3000];
  *     int errnum = errno;
  *     libexplain_message_errno_lseek(message, sizeof(message), errnum, fd,
  *         offset, whence);
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
  *    The error value to be decoded, usually obtained from the errno
  *    global variable just before this function is called.  This
  *    is necessary if you need to call <b>any</b> code between the
  *    system call to be explained and this function, because many libc
  *    functions will alter the value of errno.
  * @param fildes
  *    The original fildes,
  *    exactly as passed to the lseek(2) system call.
  * @param offset
  *    The original offset,
  *    exactly as passed to the lseek(2) system call.
  * @param whence
  *    The original whence,
  *    exactly as passed to the lseek(2) system call.
  */
void libexplain_message_errno_lseek(char *message, int message_size,
    int errnum, int fildes, long long offset, int whence);

#ifdef c_plus_plus
}
#endif

#endif /* LIBEXPLAIN_LSEEK_H */
