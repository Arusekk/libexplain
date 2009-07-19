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

#ifndef LIBEXPLAIN_OPTION_H
#define LIBEXPLAIN_OPTION_H

/**
  * The explain_option_numeric_errno function may be used to obtain
  * the LIBEXPLAIN_OPTIONS (no-)numeric-errno flag, used to control
  * whether or not the numeric value of errno is displayed.  This is
  * commonly turned off for the test suite, to cope with capricious
  * errno numbering on different UNIX dialects.
  *
  * @returns
  *    int; true (non-zero) if numeric errno values are to be displayed
  *    (this is the default), false (zero) if they are not to be
  *    dislayed.
  */
int explain_option_numeric_errno(void);

/**
  * The explain_option_dialect_specific function may be used to
  * obtain the LIBEXPLAIN_OPTIONS (no-)dialect-specific flag, used to
  * control whether or not informative text that is specific to a given
  * UNIX dialect is to be displayed.  This is commonly turned off for
  * the test suite.
  *
  * @returns
  *    int; true (non-zero) if additional dialect specific text is to be
  *    displayed (this is the default), false (zero) if it is not to be
  *    dislayed.
  */
int explain_option_dialect_specific(void);

/**
  * The explain_option_debug function may be used to obtain the
  * LIBEXPLAIN_OPTIONS (no-)debug flag, used to control whether or not
  * debug behaviour is enabled.  Defaults to off.
  *
  * @returns
  *    int; true (non-zero) if additional dialect specific text is to be
  *    displayed (this is the default), false (zero) if it is not to be
  *    dislayed.
  */
int explain_option_debug(void);

/**
  * The explain_option_assemble_program_name option is used to
  * determine whether or not the explain_assemble function should
  * include the program name in the assembled messages.
  *
  * @returns
  *     int; true (non-zero) is shall include program name, zero (false)
  *     if shall not include program name.
  */
int explain_option_assemble_program_name(void);

/**
  * The explain_program_name_assemble_internal function is used
  * to control whether or not the name of the calling process is to
  * be included in error messages issued by the explain_*_or_die
  * functions.  This will have a precedence below the LIBEXPLAIN_OPTIONS
  * environment variable, but higher than default.  It is intended for
  * use bu the explain_*_or_die functions.
  *
  * @param yesno
  *     non-zero (true) to have program name included,
  *     zero (false) to have program name excluded.
  */
void explain_program_name_assemble_internal(int yesno);

#endif /* LIBEXPLAIN_OPTION_H */