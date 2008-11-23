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

#ifndef LIBEXPLAIN_GETC_H
#define LIBEXPLAIN_GETC_H

#include <stdio.h>

/**
  * @file
  * @brief explain getc(3) errors
  */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * FIXME: need a better inline-availability test,
 * but the defines in libexplain/config.h are not suitable for inclusion
 * in the client API because they have no LIBEXPLAIN_ namespace prefix.
 */
#if __GNUC__ >= 3
/**
  * Private function, provided for the explusive use of the
  * libexplain_getc_or_die inline function.  Clients of libexplain must
  * not use it, because it's exisnece and signature is subject to change
  * without notice.  Think of it as a C++ private method.
  */
void libexplain_getc_or_die_failed(FILE *fp);
#endif

/**
  * The libexplain_getc_or_die function is
  * used to call the getc(3) system call.  On
  * failure an explanation will be printed to stderr,
  * obtained from libexplain_getc(3), and
  * then the process terminates by calling exit(EXIT_FAILURE).
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * int c = libexplain_getc_or_die(fp);
  * @endcode
  *
  * @param fp
  *     The fp, exactly as to be passed to the getc(3) system call.
  * @returns
  *     This function only returns on success, returning the next input
  *     character, or EOF at end of input.  On failure, prints an
  *     explanation and exits, it does not return.
  */
#if __GNUC__ >= 3
static __inline__
#endif
int libexplain_getc_or_die(FILE *fp)
#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
;

#if __GNUC__ >= 3

static __inline__ int
libexplain_getc_or_die(FILE *fp)
{
    /*
     * By using inline, the users dosn't have to pay a one-function-
     * call-per-character penalty for using libexplain, because getc is
     * usually a macro or an inline that only calls underflow when the
     * buffer is exhausted.
     */
    int c = getc(fp);
    if (c == EOF && ferror(fp))
        libexplain_getc_or_die_failed(fp);
    return c;
}

#endif

/**
  * The libexplain_getc function is used to
  * obtain an explanation of an error returned by the
  * getc(3) system call.
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
  * int c = getc(fp);
  * if (c == EOF && ferror(fp))
  * {
  *     fprintf(stderr, "%s\n", libexplain_getc(fp));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * @param fp
  *     The original fp, exactly as passed to the getc(3) system call.
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
const char *libexplain_getc(FILE *fp);

/**
  * The libexplain_errno_getc function is
  * used to obtain an explanation of an error returned by
  * the getc(3) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * int c = getc(fp);
  * if (c == EOF && ferror(fp))
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_getc(err, fp));
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
  * @param fp
  *     The original fp, exactly as passed to the getc(3) system call.
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
const char *libexplain_errno_getc(int errnum, FILE * fp);

/**
  * The libexplain_message_getc function is
  * used to obtain an explanation of an error returned by
  * the getc(3) system call.
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
  * int c = getc(fp);
  * if (c == EOF && ferror(fp))
  * {
  *     char message[3000];
  *     libexplain_message_getc(message, sizeof(message), fp);
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
  * @param fp
  *     The original fp, exactly as passed to the getc(3) system call.
  */
void libexplain_message_getc(char *message, int message_size, FILE * fp);

/**
  * The libexplain_message_errno_getc
  * function is used to obtain an explanation of an error
  * returned by the
  * getc(3) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * int c = getc(fp);
  * if (c == EOF && ferror(fp))
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_getc(message, sizeof(message), err, fp);
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
  * @param fp
  *     The original fp, exactly as passed to the getc(3) system call.
  */
void libexplain_message_errno_getc(char *message, int message_size, int errnum,
    FILE * fp);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_GETC_H */
