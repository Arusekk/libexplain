/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2012 Peter Miller
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

#include <libexplain/setregid.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_setregid <rgid> <egid>\n");
    fprintf(stderr, "       test_setregid -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    gid_t           rgid;
    gid_t           egid;

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

    rgid = -1;
    egid = -1;
    switch (argc - optind)
    {
    case 2:
        egid = explain_string_to_gid_t(argv[optind + 1]);
    case 1:
        rgid = explain_string_to_gid_t(argv[optind]);
    case 0:
        break;

    default:
        usage();
    }

    explain_setregid_or_die(rgid, egid);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
