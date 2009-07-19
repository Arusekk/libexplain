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

#ifndef LIBEXPLAIN_BUFFER_DEVICE_NAME_H
#define LIBEXPLAIN_BUFFER_DEVICE_NAME_H

#include <libexplain/ac/sys/stat.h>

#include <libexplain/string_buffer.h>

/**
  * The explain_buffer_device_name function may be used locate a
  * device's name an print it into the supplied buffer.
  *
  * @param sb
  *     The string buffer to print into.
  * @param dev
  *     The device to look for.
  * @param st
  *     The file info that provoked the request.
  * @returns
  *     0 on success, -1 on error
  */
int explain_buffer_device_name(explain_string_buffer_t *sb, dev_t dev,
    struct stat *st);

#endif /* LIBEXPLAIN_BUFFER_DEVICE_NAME_H */