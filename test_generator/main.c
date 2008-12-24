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

#include <libexplain/ac/assert.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/version_print.h>

#include <test_generator/generate.h>
#include <test_generator/gram.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_generator <declaration>\n");
    fprintf(stderr, "       test_generator -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char *decl;
    node_t *np;
    int lisp;

    lisp = 0;
    for (;;)
    {
        int c = getopt(argc, argv, "lV");
        if (c < 0)
            break;
        switch (c)
        {
        case 'l':
            lisp = 1;
            break;

        case 'V':
            libexplain_version_print();
            return 0;

        default:
            usage();
            /* NOTREACHED */
        }
    }
    if (optind + 1 != argc)
        usage();
    decl = argv[optind];
    np = grammar(decl);
    if (lisp)
        node_print_lisp(np, stdout);
    else
        node_print(np, stdout);
    putchar('\n');

    /*
     * Make there is only one name declaed.
     */
    assert(node_is(np, "declaration"));
    assert(np->nchild >= 1);
    if (np->nchild != 3)
    {
        fprintf(stderr, "no names declared\n");
        exit(EXIT_FAILURE);
    }
    assert(node_is(np->child[0], "declaration_specifiers"));
    assert(node_is(np->child[1], "init_declarator_list"));
    if (np->child[1]->nchild != 1)
    {
        fprintf(stderr, "too many names declared\n");
        exit(EXIT_FAILURE);
    }
    assert(node_is(np->child[1]->child[0], "init_declarator"));
    assert(np->child[1]->child[0]->nchild == 1);
    assert(node_is(np->child[1]->child[0]->child[0], "declarator"));
    assert(np->child[1]->child[0]->child[0]->nchild == 1
        || np->child[1]->child[0]->child[0]->nchild == 2);

    generate(np->child[0], np->child[1]->child[0]->child[0]);

    return 0;
}
