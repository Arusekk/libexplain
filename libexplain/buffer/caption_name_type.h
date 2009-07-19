/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_CAPTION_NAME_TYPE_H
#define LIBEXPLAIN_BUFFER_CAPTION_NAME_TYPE_H

#include <libexplain/string_buffer.h>

/**
  * The explain_buffer_caption_name_type function may be used to
  *
  * @param sb
  *    The string buffer to print into.
  * @param caption
  *    The caption "pathname", "oldpath", "newpath", etc, or NULL
  * @param name
  *    The name of the file or directory, or NULL
  * @param st_mode
  *    The file type (S_IFREG, etc)
  */
void explain_buffer_caption_name_type(explain_string_buffer_t *sb,
    const char *caption, const char *name, int st_mode);

#endif /* LIBEXPLAIN_BUFFER_CAPTION_NAME_TYPE_H */
