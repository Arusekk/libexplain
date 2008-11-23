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

#ifndef LIBEXPLAIN_GETCHAR_H
#define LIBEXPLAIN_GETCHAR_H

#include <stdio.h>

/**
  * @file
  * @brief explain getchar(3) errors
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
 * Private function, provided for the exclusive use of the
 * libexplain_getchar_or_die inline function.  Clients of libexplain must
 * not use it, because it's existence and signature is subject to change
 * without notice.  Think of it as a C++ private method.
 */
void libexplain_getchar_or_die_failed(void);
#endif

/**
  * The libexplain_getchar_or_die function is used to call the
  * getchar(3) system call.  On failure an explanation will be printed
  * to stderr, obtained from libexplain_getchar(3), and then the process
  * terminates by calling exit(EXIT_FAILURE).
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * int c = libexplain_getchar_or_die();
  * @endcode
  *
  * @returns
  *     This function only returns on success, next character or EOF at
  *     end-of-file.  On failure, prints an explanation and exits, it
  *     does not return.
  */
#if __GNUC__ >= 3
static __inline__
#endif
int libexplain_getchar_or_die(void)
#if __GNUC__ >= 3
__attribute__((always_inline))
#endif
;

#if __GNUC__ >= 3

static __inline__ int
libexplain_getchar_or_die(void)
{
    /*
     * By using inline, the users dosn't have to pay a one-function-
     * call-per-character penalty for using libexplain, because getc is
     * usually a macro or an inline that only calls underflow when the
     * buffer is exhausted.
     */
    int c = getchar();
    if (c == EOF && ferror(stdin))
        libexplain_getchar_or_die_failed();
    return c;
}

#endif

/**
  * The libexplain_getchar function is used to
  * obtain an explanation of an error returned by the
  * getchar(3) system call.
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
  * int c = getchar();
  * if (c == EOF && ferror(stdin))
  * {
  *     fprintf(stderr, "%s\n", libexplain_getchar());
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
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
const char *libexplain_getchar(void);

/**
  * The libexplain_errno_getchar function is
  * used to obtain an explanation of an error returned by
  * the getchar(3) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * int c = getchar();
  * if (c == EOF && ferror(stdin))
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", libexplain_getchar(err, ));
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
const char *libexplain_errno_getchar(int errnum);

/**
  * The libexplain_message_getchar function is
  * used to obtain an explanation of an error returned by
  * the getchar(3) system call.
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
  * int c = getchar();
  * if (c == EOF && ferror(stdin))
  * {
  *     char message[3000];
  *     libexplain_message_getchar(message, sizeof(message), );
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
  */
void libexplain_message_getchar(char *message, int message_size);

/**
  * The libexplain_message_errno_getchar
  * function is used to obtain an explanation of an error
  * returned by the
  * getchar(3) system call.
  * The least the message will contain is the value of
  * strerror(errnum), but usually it will do much better,
  * and indicate the underlying cause in more detail.
  *
  * This function is intended to be used in a fashion
  * similar to the following example:
  * @code
  * int c = getchar();
  * if (c == EOF && ferror(stdin))
  * {
  *     int err = errno;
  *     char message[3000];
  *     libexplain_message_errno_getchar(message, sizeof(message), err, );
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
  */
void libexplain_message_errno_getchar(char *message, int message_size,
    int errnum);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_GETCHAR_H */
