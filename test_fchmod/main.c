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

#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/fchmod.h>
#include <libexplain/open.h>
#include <libexplain/permission_mode.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_fchmod [ <option>... ] <pathname>\n");
    fprintf(stderr, "       test_fchmod -V\n");
    exit(1);
}


int
main(int argc, char **argv)
{
    const char      *pathname;
    int             fildes;
    int             mode;

    pathname = 0;
    fildes = -1;
    mode = 0644;
    for (;;)
    {
        int c = getopt(argc, argv, "m:V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'm':
            mode = libexplain_permission_mode_parse(optarg);
            if (mode < 0)
            {
                fprintf
                (
                    stderr,
                    "-m \"%s\" does not look like a permission mode\n",
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

    fildes = libexplain_open_or_die(pathname, O_RDWR, 0);
    libexplain_fchmod_or_die(fildes, mode);
    return 0;
}
