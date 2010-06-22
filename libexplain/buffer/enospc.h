/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEXPLAIN_BUFFER_ENOSPC_H
#define LIBEXPLAIN_BUFFER_ENOSPC_H

#include <libexplain/string_buffer.h>

/**
  * The explain_buffer_enospc function may be used to print an
  * explanation of an ENOSPC error.
  *
  * @param sb
  *     The string buffer to print into.
  * @param pathname
  *     The path of the file that could not be created.
  * @param pathname_caption
  *     The name of the argument containing the file that could not be created.
  */
void explain_buffer_enospc(explain_string_buffer_t *sb, const char *pathname,
    const char *pathname_caption);

/**
  * The explain_buffer_enospc_fildes function may be used to print an
  * explanation of an ENOSPC error.
  *
  * @param sb
  *     The string buffer to print into.
  * @param fildes
  *     The file descriptor with the problem.
  * @param fildes_caption
  *     The name of the syscall argument containing the file descriptor
  *     with the problem.
  */
void explain_buffer_enospc_fildes(explain_string_buffer_t *sb, int fildes,
    const char *fildes_caption);

#endif /* LIBEXPLAIN_BUFFER_ENOSPC_H */
