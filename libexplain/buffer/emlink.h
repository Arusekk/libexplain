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

#ifndef LIBEXPLAIN_BUFFER_EMLINK_H
#define LIBEXPLAIN_BUFFER_EMLINK_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_emlink function may be used to
  * print an explanation of an EMLINK error into the given buffer.
  *
  * @param sb
  *    The string buffer to print on.
  * @param oldpath
  *    The original oldpath, exactly as passed to system call.
  * @param newpath
  *    The original newpath, exactly as passed to system call.
  */
void libexplain_buffer_emlink(libexplain_string_buffer_t *sb,
    const char *oldpath, const char *newpath);

#endif /* LIBEXPLAIN_BUFFER_EMLINK_H */
