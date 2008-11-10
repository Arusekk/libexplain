/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#ifndef TEST_GENERATOR_NODE_H
#define TEST_GENERATOR_NODE_H

#include <libexplain/ac/stddef.h>
#include <libexplain/ac/stdio.h>

typedef struct node_t node_t;
struct node_t
{
    const char      *name;
    node_t          **child;
    size_t          nchild;
    size_t          nchild_max;
    const char      *literal;
};

node_t *node_new(const char *name);
node_t *node_new_literal(const char *text);
void node_push_back(node_t *parent, node_t *child);
void node_print(const node_t *np, FILE *fp);
void node_print_lisp(const node_t *np, FILE *fp);

int node_is(const node_t *np, const char *name);
int node_is_literal(const node_t *np, const char *text);

#endif /* TEST_GENERATOR_NODE_H */
