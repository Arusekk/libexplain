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

#ifndef LIBEXPLAIN_BUFFER_ERRNO_PATH_RESOLUTION_H
#define LIBEXPLAIN_BUFFER_ERRNO_PATH_RESOLUTION_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_path_resolution function may be used to
  * check a path for problems.
  *
  * @param sb
  *    The string buffer to write the error to, once it is found
  * @param errnum
  *    The error number expected.
  * @param pathname
  *    The path being checked.
  * @param flags
  *    Flags controlling the final component (O_CREAT, O_EXCL,
  *    O_NOFOLLOW, O_DIRECTORY) styled after open(2) deliberately.
  * @param caption
  *    The name of the argument being checks in the functions argument
  *    of the system call being deciphered.
  * @returns
  *    0 on success, meaning it found an error and wrote it to sb;
  *    -1 on failure, meaning it didn't find an error or it didn't find
  *    the expected error.
  */
int libexplain_buffer_errno_path_resolution(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, int flags, const char *caption);

#endif /* LIBEXPLAIN_BUFFER_ERRNO_PATH_RESOLUTION_H */
