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

#ifndef LIBEXPLAIN_FREAD_H
#define LIBEXPLAIN_FREAD_H

#include <stdio.h>

/**
  * @file
  * @brief explain fread(3) errors
  */

#ifdef __cplusplus
extern "C" {
#endif

/**
  * The libexplain_fread_or_die function is
  * used to call the fread(3) system call.  On
  * failure an explanation will be printed to stderr,
  * obtained from libexplain_fread(3), and
  * then the process terminates by calling exit(EXIT_FAILURE).
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * long how_many = libexplain_fread_or_die(ptr, size, nmemb, fp);
  * @endcode
  *
  * @param ptr
  *     The ptr, exactly as to be passed to the fread(3) system call.
  * @param size
  *     The size, exactly as to be passed to the fread(3) system call.
  * @param nmemb
  *     The nmemb, exactly as to be passed to the fread(3) system call.
  * @param fp
  *     The fp, exactly as to be passed to the fread(3) system call.
  * @returns
  *     This function only returns on success, the number read or 0 on
  *     end-of-file.  On failure, prints an explanation and exits, it
  *     does not return.
  */
long libexplain_fread_or_die(void *ptr, long size, long nmemb, FILE *fp);

/**
  * The libexplain_fread function is used to
  * obtain an explanation of an error returned by the
  * fread(3) system call.
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
  * long how_many = fread(ptr, size, nmemb, fp);
  * if (how_many == 0 && ferror(fp))
  * {
  *     fprintf(stderr, "%s\n", libexplain_fread(ptr, size, nmemb, fp));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param ptr
  *     The original ptr, exactly as passed to the fread(3) system call.
  * @param size
  *     The original size, exactly as passed to the fread(3) system call.
  * @param nmemb
  *     The original nmemb, exactly as passed to the fread(3) system call.
  * @param fp
  *     The original fp, exactly as passed to the fread(3) system call.
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
const char *libexplain_fread(void *ptr, long size, long nmemb, FILE *fp);

/**
  * The libexplain_errno_fread function is
  * used to obtain an explanation of an error returned by
  * the fread(3) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * long how_many = fread(ptr, size, nmemb, fp);
  * if (how_many == 0 && ferror(fp))
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_fread(err, ptr, size, nmemb, fp));
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
  * @param ptr
  *     The original ptr, exactly as passed to the fread(3) system call.
  * @param size
  *     The original size, exactly as passed to the fread(3) system call.
  * @param nmemb
  *     The original nmemb, exactly as passed to the fread(3) system call.
  * @param fp
  *     The original fp, exactly as passed to the fread(3) system call.
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
const char *libexplain_errno_fread(int errnum, void *ptr, long size,
    long nmemb, FILE *fp);

/**
  * The libexplain_message_fread function is
  * used to obtain an explanation of an error returned by
  * the fread(3) system call.
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
  * long how_many = fread(ptr, size, nmemb, fp);
  * if (how_many == 0 && ferror(fp))
  * {
  *     char message[3000];
  *     libexplain_message_fread(message, sizeof(message),
  *         ptr, size, nmemb, fp);
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
  * @param ptr
  *     The original ptr, exactly as passed to the fread(3) system call.
  * @param size
  *     The original size, exactly as passed to the fread(3) system call.
  * @param nmemb
  *     The original nmemb, exactly as passed to the fread(3) system call.
  * @param fp
  *     The original fp, exactly as passed to the fread(3) system call.
  */
void libexplain_message_fread(char *message, int message_size, void *ptr,
    long size, long nmemb, FILE *fp);

/**
  * The libexplain_message_errno_fread
  * function is used to obtain an explanation of an error
  * returned by the
  * fread(3) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * long how_many = fread(ptr, size, nmemb, fp);
  * if (how_many == 0 && ferror(fp))
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_fread(message, sizeof(message), err,
  *         ptr, size, nmemb, fp);
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
  * @param ptr
  *     The original ptr, exactly as passed to the fread(3) system call.
  * @param size
  *     The original size, exactly as passed to the fread(3) system call.
  * @param nmemb
  *     The original nmemb, exactly as passed to the fread(3) system call.
  * @param fp
  *     The original fp, exactly as passed to the fread(3) system call.
  */
void libexplain_message_errno_fread(char *message, int message_size, int errnum,
    void *ptr, long size, long nmemb, FILE *fp);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_FREAD_H */
