/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2011-2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/shmat.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_shmat <shmid> <shmaddr> <shmflg>\n");
    fprintf(stderr, "       test_shmat -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             shmid;
    void            *shmaddr;
    int             shmflg;

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
    if (optind + 3 != argc)
        usage();
    shmid = explain_parse_int_or_die(argv[optind]);
    shmaddr = explain_parse_pointer_or_die(argv[optind + 1]);
    shmflg = explain_parse_int_or_die(argv[optind + 2]);

    if (explain_shmat_or_die(shmid, shmaddr, shmflg))
    {
        /* nothing */
    }
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
