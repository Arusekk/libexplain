/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEXPLAIN_IOCONTROL_SIOCETHTOOL_H
#define LIBEXPLAIN_IOCONTROL_SIOCETHTOOL_H

#include <libexplain/iocontrol.h>

/**
  * The libexplain_iocontrol_siocethtool global variable is
  * used to store information about the SIOCETHTOOL
  * I/O control.
  *
  * @note
  *     This information is not kept in a single table for all values,
  *     like every other set of constants, because (a) some values
  *     are ambiguous, and (b) the includes files have bugs making it
  *     impossible to include all of them in the same combilation unit.
  */
extern const libexplain_iocontrol_t libexplain_iocontrol_siocethtool;

#endif /* LIBEXPLAIN_IOCONTROL_SIOCETHTOOL_H */
