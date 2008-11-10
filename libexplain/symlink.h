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

#ifndef LIBEXPLAIN_SYMLINK_H
#define LIBEXPLAIN_SYMLINK_H

/**
  * @file
  * @brief explain symlink(2) errors
  */

#ifdef c_plus_plus
extern "C" {
#endif

/**
  * The libexplain_symlink_or_die function is used to call the
  * symlink(2) system call.  On failure an explanation will be printed
  * to stderr, obtained from libexplain_symlink(3), and then the process
  * terminates by calling exit(1).
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * libexplain_symlink_or_die(oldpath, newpath);
  * @endcode
  *
  * @param oldpath
  *     The oldpath, exactly as to be passed to the symlink(2) system call.
  * @param newpath
  *     The newpath, exactly as to be passed to the symlink(2) system call.
  * @returns
  *     This function only returns on success.
  *     On failure, prints an explanation and exits,
  *     it does not return.
  */
void libexplain_symlink_or_die(const char *oldpath, const char *newpath);

/**
  * The libexplain_symlink function is used to obtain an explanation
  * of an error returned by the symlink(2) system call.  The least the
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
  * if (symlink(oldpath, rewpath) < 0)
  * {
  *     fprintf(stderr, "%s\n", libexplain_symlink(oldpath, newpath));
  *     exit(1);
  * }
  * @endcode
  *
  * @param oldpath
  *     The original oldpath, exactly as passed to the symlink(2) system call.
  * @param newpath
  *     The original newpath, exactly as passed to the symlink(2) system call.
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
const char *libexplain_symlink(const char *oldpath, const char *newpath);

/**
  * The libexplain_message_symlink function is used to obtain an
  * explanation of an error returned by the symlink(2) system call.  The
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
  * if (symlink(oldpath, newpath) < 0)
  * {
  *     char message[3000];
  *     libexplain_message_symlink(message, sizeof(message), oldpath,
  *         newpath);
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
  * @param oldpath
  *     The original oldpath, exactly as passed to the symlink(2) system call.
  * @param newpath
  *     The original newpath, exactly as passed to the symlink(2) system call.
  */
void libexplain_message_symlink(char *message, int message_size,
    const char *oldpath, const char *newpath);

/**
  * The libexplain_errno_symlink function is used to obtain an
  * explanation of an error returned by the symlink(2) system call.  The
  * least the message will contain is the value of strerror(errnum), but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (symlink(oldpath, newpath) < 0)
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_errno_symlink(err, oldpath,
  *         newpath));
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
  * @param oldpath
  *     The original oldpath, exactly as passed to the symlink(2) system call.
  * @param newpath
  *     The original newpath, exactly as passed to the symlink(2) system call.
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
const char *libexplain_errno_symlink(int errnum, const char *oldpath,
    const char *newpath);

/**
  * The libexplain_message_errno_symlink function is used to obtain an
  * explanation of an error returned by the symlink(2) system call.  The
  * least the message will contain is the value of strerror(errnum), but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (symlink(oldpath, newpath) < 0)
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_symlink(message, sizeof(message), err,
  *         oldpath, newpath);
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
  * @param oldpath
  *     The original oldpath, exactly as passed to the symlink(2) system call.
  * @param newpath
  *     The original newpath, exactly as passed to the symlink(2) system call.
  */
void libexplain_message_errno_symlink(char *message, int message_size,
    int errnum, const char *oldpath, const char *newpath);

#ifdef c_plus_plus
}
#endif

#endif /* LIBEXPLAIN_SYMLINK_H */
