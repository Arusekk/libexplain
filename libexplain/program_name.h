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

#ifndef LIBEXPLAIN_PROGRAM_NAME_H
#define LIBEXPLAIN_PROGRAM_NAME_H

/**
  * @file
  * @brief explain program_name(2) errors
  */

#ifdef __cplusplus
extern "C" {
#endif

/**
  * The libexplain_program_name_get function may be used to obtain the
  * command name of the process.  Depending on how capable /proc is
  * on your system, or, failing that, how capanle lsof(1) is on your
  * system, this may or may not produce a sensable result.  It works
  * well on Linux.
  *
  * @returns
  *    pointer to string containing the command name (no slashes) of the
  *    calling process.
  */
const char *libexplain_program_name_get(void);

/**
  * The libexplain_program_name_set function may be used to set
  * the libexplain libraries' idea of the command name of the
  * calling process, setting the string to be returned by the
  * libexplain_program_name_get function.  This overrides the automatic
  * behaviour, which can be quite desirable in commands that can be
  * invoked with more than one name, e.g. if they are a hard link
  * synoym.
  *
  * This also sets the option to include the program name in all of the
  * error messages issued by the libexplain_*_or_die functions.
  *
  * @param name
  *     The name of the calling process.
  */
void libexplain_program_name_set(const char *name);

/**
  * The libexplain_program_name_assemble function is used to control
  * whether or not the name of the calling process is to be included in
  * error messages issued by the libexplain_*_or_die functions.  If not
  * expolicitly set, is controlled by the LIBEXPLAIN_OPTIONS environemnt
  * variable, or defaults to false if not set there either.
  *
  * @param yesno
  *     non-zero (true) to have program name included,
  *     zero (false) to have program name excluded.
  */
void libexplain_program_name_assemble(int yesno);

#ifdef __cplusplus
}
#endif

#endif /* LIBEXPLAIN_PROGRAM_NAME_H */
