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

#ifndef LIBEXPLAIN_BUFFER_CDROM_VOLCTRL_H
#define LIBEXPLAIN_BUFFER_CDROM_VOLCTRL_H

#include <libexplain/string_buffer.h>

struct cdrom_volctrl; /* forward */

/**
  * The explain_buffer_cdrom_volctrl function may be used
  * to print a representation of a cdrom_volctrl structure.
  *
  * @param sb
  *     The string buffer to print into.
  * @param data
  *     The cdrom_volctrl structure to be printed.
  */
void explain_buffer_cdrom_volctrl(explain_string_buffer_t *sb,
    const struct cdrom_volctrl *data);

#endif /* LIBEXPLAIN_BUFFER_CDROM_VOLCTRL_H */