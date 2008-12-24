/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEXPLAIN_BUFFER_ERRNO_FREOPEN_H
#define LIBEXPLAIN_BUFFER_ERRNO_FREOPEN_H

#include <libexplain/ac/stdio.h>

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_errno_freopen function is used to obtain an
  * explanation of an error returned by the freopen(3) system call.  The
  * least the message will contain is the value of strerror(errnum), but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * @param sb
  *     The string buffer in which the message is being constructed.
  *     If a safe buffer is specified, this function is thread safe.
  * @param errnum
  *     The error value to be decoded, usually obtain from the errno
  *     global variable just before this function is called.  This
  *     is necessary if you need to call <b>any</b> code between the
  *     system call to be explained and this function, because many libc
  *     functions will alter the value of errno.
  * @param path
  *     The original path, exactly has passed to the freopen(3) system call.
  * @param flags
  *     The original flags, exactly has passed to the freopen(3) system call.
  * @param fp
  *     The original mode, exactly has passed to the freopen(3) system call.
  */
void libexplain_buffer_errno_freopen(struct libexplain_string_buffer_t *sb,
    int errnum, const char *path, const char *flags, FILE *fp);

#endif /* LIBEXPLAIN_BUFFER_ERRNO_FREOPEN_H */
