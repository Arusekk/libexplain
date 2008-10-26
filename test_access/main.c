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

#include <libexplain/access.h>
#include <libexplain/buffer/access_mode.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_access [ <option>... ] <pathname>\n");
    fprintf(stderr, "       test_access -V\n");
    exit(1);
}


int
main(int argc, char **argv)
{
    int             fd;
    const char      *pathname;
    int             mode;

    fd = -1;
    pathname = 0;
    mode = F_OK;
    for (;;)
    {
        int c = getopt(argc, argv, "m:V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'm':
            mode = libexplain_access_mode_parse(optarg);
            if (mode < 0)
            {
                fprintf
                (
                    stderr,
                    "-m \"%s\" does not look like an access mode\n",
                    optarg
                );
                exit(1);
            }
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

    if (access(pathname, mode) < 0)
    {
        libexplain_wrap_and_print(stderr, libexplain_access(pathname, mode));
        exit(1);
    }
    return 0;
}
