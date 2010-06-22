/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/string_to_thing.h>
#include <libexplain/strtoul.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_strtoul <nptr> <endptr> <base>\n");
    fprintf(stderr, "       test_strtoul -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *nptr;
    char            *end;
    int             base;
    unsigned long   result;

    for (;;)
    {
        int c = getopt(argc, argv, "V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'V':
            explain_version_print();
            return EXIT_SUCCESS;

        default:
            usage();
        }
    }
    nptr = 0;
    base = 0;
    switch (argc - optind)
    {
    case 2:
        base = explain_string_to_int(argv[optind + 1]);
        /* fall through... */

    case 1:
        nptr = argv[optind];
        break;

    default:
        usage();
    }
    end = 0;
    result = explain_strtoul_or_die(nptr, &end, base);
    printf("%lu\n", result);
    if (*end)
        printf("more text (\"%s\") follows number\n", end);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et */
