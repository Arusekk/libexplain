/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#ifndef LIBEXPLAIN_BUFFER_ERRNO_CHMOD_H
#define LIBEXPLAIN_BUFFER_ERRNO_CHMOD_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_errno_chmod function may be used to obtain a
  * detailed explanation of an error returned by a chmod(2) system call.
  *
  * @param sb
  *    The string buffer to print the explanation into.
  * @param errnum
  *    The returned error, usually obtained from the errno global variable.
  * @param pathname
  *    The original pathname, exactly as passed to the chmod(2) system call.
  * @param mode
  *    The original mode, acactly as passed to the chmod(2) system call.
  */
void libexplain_buffer_errno_chmod(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, int mode);

#endif /* LIBEXPLAIN_BUFFER_ERRNO_CHMOD_H */
