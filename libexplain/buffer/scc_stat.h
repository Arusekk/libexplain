/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010, 2011 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_SCC_STAT_H
#define LIBEXPLAIN_BUFFER_SCC_STAT_H

#include <libexplain/string_buffer.h>

struct scc_stat; /* forward */

/**
  * The explain_buffer_scc_stat function may be used
  * to print a representation of a scc_stat structure.
  *
  * @param sb
  *     The string buffer to print into.
  * @param data
  *     The scc_stat structure to be printed.
  */
void explain_buffer_scc_stat(explain_string_buffer_t *sb,
    const struct scc_stat *data);

#endif /* LIBEXPLAIN_BUFFER_SCC_STAT_H */