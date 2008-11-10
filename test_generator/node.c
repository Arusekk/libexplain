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

#include <libexplain/ac/assert.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>

#include <test_generator/node.h>


static void *
malloc_or_die(size_t n)
{
    void            *p;

    if (n == 0)
        n = 1;
    p = malloc(n);
    if (!p)
    {
        fprintf(stderr, "out of memory, aborting\n");
        exit(1);
    }
    return p;
}


static char *
strdup_or_die(const char *old)
{
    size_t len = strlen(old) + 1;
    char *new = malloc_or_die(len);
    memcpy(new, old, len);
    return new;
}


node_t *
node_new(const char *name)
{
    node_t          *np;

    np = malloc_or_die(sizeof(node_t));
    np->name = strdup_or_die(name);
    np->child = 0;
    np->nchild = 0;
    np->nchild_max = 0;
    np->literal = 0;
    return np;
}


node_t *
node_new_literal(const char *text)
{
    node_t          *np;

    np = malloc_or_die(sizeof(node_t));
    np->name = strdup_or_die("LITERAL");
    np->child = 0;
    np->nchild = 0;
    np->nchild_max = 0;
    np->literal = strdup_or_die(text);
    return np;
}


void
node_push_back(node_t *parent, node_t *child)
{
    assert(0 != strcmp(parent->name, "LITERAL"));
    if (parent->nchild >= parent->nchild_max)
    {
        size_t          new_nchild_max;
        size_t          nbytes;
        node_t          **new_child;
        size_t          j;

        new_nchild_max = parent->nchild_max * 2 + 4;
        nbytes = sizeof(parent->child[0]) * new_nchild_max;
        new_child = malloc_or_die(nbytes);
        for (j = 0; j < parent->nchild; ++j)
            new_child[j] = parent->child[j];
        if (parent->child)
            free(parent->child);
        parent->child = new_child;
        parent->nchild_max = new_nchild_max;
    }
    parent->child[parent->nchild++] = child;
}


void
node_print(const node_t *np, FILE *fp)
{
    if (0 == strcmp(np->name, "LITERAL"))
    {
        assert(!np->nchild);
        fputs(np->literal, fp);
    }
    else
    {
        size_t          j;

        assert(!np->literal);
        for (j = 0; j < np->nchild; ++j)
        {
            if (j)
                putc(' ', fp);
            node_print(np->child[j], fp);
        }
    }
}


void
node_print_lisp(const node_t *np, FILE *fp)
{
    if (0 == strcmp(np->name, "LITERAL"))
    {
        assert(!np->nchild);
        fputc('"', fp);
        fputs(np->literal, fp);
        fputc('"', fp);
    }
    else
    {
        size_t          j;

        assert(!np->literal);
        putc('(', fp);
        fputs(np->name, fp);
        for (j = 0; j < np->nchild; ++j)
        {
            putc(' ', fp);
            node_print_lisp(np->child[j], fp);
        }
        putc(')', fp);
    }
}


int
node_is(const node_t *np, const char *name)
{
    assert(name);
    return (0 == strcmp(np->name, name));
}


int
node_is_literal(const node_t *np, const char *text)
{
    if (!node_is(np, "LITERAL"))
        return 0;
    assert(np->literal);
    assert(text);
    return (0 == strcmp(text, np->literal));
}
