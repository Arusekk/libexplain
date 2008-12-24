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

#ifndef LIBEXPLAIN_BUFFER_EIO_H
#define LIBEXPLAIN_BUFFER_EIO_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_eio function may be used to
  * print a description of an EIO error.
  *
  * @param sb
  *    The buffer to print the explanation on
  */
void libexplain_buffer_eio(libexplain_string_buffer_t *sb);

/**
  * The libexplain_buffer_eio_fildes function may be used to
  * print a description of an EIO error.
  *
  * @param sb
  *    The buffer to print the explanation on
  * @param fildes
  *    the file descriptor with the problem.
  */
void libexplain_buffer_eio_fildes(libexplain_string_buffer_t *sb, int fildes);

/**
  * The libexplain_buffer_eio_path function may be used to
  * print a description of an EIO error.
  *
  * @param sb
  *    The buffer to print the explanation on
  * @param path
  *    the file with the problem.
  */
void libexplain_buffer_eio_path(libexplain_string_buffer_t *sb,
    const char *path);

/**
  * The libexplain_buffer_eio_path_dirname function may be used to
  * print a description of an EIO error.
  *
  * @param sb
  *    The buffer to print the explanation on
  * @param path
  *    the file in the directory with the problem.
  */
void libexplain_buffer_eio_path_dirname(libexplain_string_buffer_t *sb,
    const char *path);

#endif /* LIBEXPLAIN_BUFFER_EIO_H */
