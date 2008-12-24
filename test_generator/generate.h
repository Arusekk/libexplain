/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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

#ifndef TEST_GENERATOR_GENERATE_H
#define TEST_GENERATOR_GENERATE_H

#include <test_generator/node.h>

/**
  * The generate function is used to generate the content, give the
  * function declaration.
  *
  * @param arg
  *    blah blah
  * @returns
  *    blah blah
  */
void generate(node_t *declspec, node_t *decl);

#endif /* TEST_GENERATOR_GENERATE_H */
