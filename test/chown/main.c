/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2012 Peter Miller
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

#include <libexplain/chown.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_chown <path> <owner> <group>\n");
    fprintf(stderr, "       test_chown -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *path;
    int             owner;
    int             group;

    for (;;)
    {
        int c = getopt(argc, argv, "V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'V':
            explain_version_print();
            return 0;

        default:
            usage();
        }
    }
    if (optind + 3 != argc)
        usage();
    path = argv[optind];
    owner = atoi(argv[optind + 1]);
    group = atoi(argv[optind + 2]);

    explain_chown_or_die(path, owner, group);
    return 0;
}


/* vim: set ts=8 sw=4 et : */
