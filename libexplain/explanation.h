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

#ifndef LIBEXPLAIN_EXPLANATION_H
#define LIBEXPLAIN_EXPLANATION_H

#include <libexplain/ac/sys/param.h>
#include <libexplain/string_buffer.h>

typedef struct libexplain_explanation_t libexplain_explanation_t;
struct libexplain_explanation_t
{
    char system_call[PATH_MAX * 3 + 200];
    libexplain_string_buffer_t system_call_sb;
    int errnum;
    char explanation[PATH_MAX * 2 + 200];
    libexplain_string_buffer_t explanation_sb;
};

/**
  * The libexplain_explanation_init function i sused to initialize an
  * explanation struct for use.
  *
  * @param exp
  *     The explanation struct of interest
  * @param errnum
  *     The errno value provoking this message
  */
void libexplain_explanation_init(libexplain_explanation_t *exp, int errnum);

/**
  * The libexplain_explanation_assemble function may be used to
  * carefully glue the problem statement and the explanation together,
  * using internationalization, for which localizations may re-arrange
  * the order.
  *
  * @param exp
  *     The explanation of interest.  You are expected to have filled out
  *     the two string buffers already.
  * @param result
  *     where to print the resulting completed explanation.
  */
void libexplain_explanation_assemble(libexplain_explanation_t *exp,
    libexplain_string_buffer_t *result);

#endif /* LIBEXPLAIN_EXPLANATION_H */
