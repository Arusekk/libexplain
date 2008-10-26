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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/fopen.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_open [ <option>... ] <pathname>\n");
    fprintf(stderr, "       test_open -V\n");
    exit(1);
}


int
main(int argc, char **argv)
{
    FILE            *fp;
    const char      *pathname;
    const char      *flags;

    flags = "r";
    for (;;)
    {
        int c = getopt(argc, argv, "f:V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'f':
            flags = optarg;
            break;

        case 'V':
            libexplain_version_print();
            return 0;

        default:
            usage();
        }
    }
    if (optind + 1 != argc)
        usage();
    pathname = argv[optind];

    fp = libexplain_fopen_or_die(pathname, flags);
    if (fflush(fp))
    {
        libexplain_wrap_and_print(stderr, libexplain_fflush(fp));
        exit(1);
    }
    if (fclose(fp))
    {
        libexplain_wrap_and_print(stderr, libexplain_fclose(fp));
        exit(1);
    }
    return 0;
}
