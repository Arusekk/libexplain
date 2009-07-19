/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/version_print.h>

#include <codegen/catalogue.h>
#include <codegen/generate.h>
#include <codegen/gram.h>
#include <codegen/string_buffer.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: codegen <declaration>\n");
    fprintf(stderr, "       codegen -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *decl;
    node_t          *np;
    int             lisp;
    catalogue_t     *cp;
    int             save_at_end;

    lisp = 0;
    for (;;)
    {
        int c = getopt(argc, argv, "g:lV");
        if (c < 0)
            break;
        switch (c)
        {
        case 'g':
            generate_specific(optarg);
            break;

        case 'l':
            lisp = 1;
            break;

        case 'V':
            explain_version_print();
            return 0;

        default:
            usage();
            /* NOTREACHED */
        }
    }
    if (optind + 1 != argc)
        usage();
    decl = argv[optind];
    save_at_end = !!strchr(decl, '(');
    if (!save_at_end)
    {
        char            catpath[300];

        snprintf(catpath, sizeof(catpath), "catalogue/%s", decl);
        /* using prototype from message catalogue */
        cp = catalogue_open(catpath);
        decl = catalogue_get(cp, "Prototype");
        if (!decl)
        {
            fprintf(stderr, "catalogue has no Prototype\n");
            exit(1);
        }
        np = grammar(decl);
        if (lisp)
        {
            node_print_lisp(np, stdout);
            return 0;
        }
    }
    else
    {
        char            catpath[300];

        np = grammar(decl);
        if (lisp)
        {
            node_print_lisp(np, stdout);
            return 0;
        }
        node_print(np, stdout);
        putchar('\n');

        /*
         * Make there is only one name declared.
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

        snprintf
        (
            catpath,
            sizeof(catpath),
            "catalogue/%s",
            find_function_name(np)
        );
        cp = catalogue_creat(catpath);
        catalogue_set(cp, "Prototype", decl);
        catalogue_save(cp);

        /* don't actually generate anything */
        generate_specific("garbage");
    }

    generate(np, cp);
    if (save_at_end)
        catalogue_save(cp);
    return 0;
}