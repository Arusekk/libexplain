/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011, 2013 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdlib.h>


float
explain_ac_strtof(const char *nptr, char **endptr)
{
#ifdef HAVE_STRTOF
    return strtof(nptr, endptr);
#else
    return strtod(nptr, endptr);
#endif
}


long double
explain_ac_strtold(const char *nptr, char **endptr)
{
#ifdef HAVE_STRTOLD
    return strtold(nptr, endptr);
#else
    return strtod(nptr, endptr);
#endif
}


/* vim: set ts=8 sw=4 et : */
