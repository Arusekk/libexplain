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

#include <libexplain/close.h>
#include <libexplain/open.h>
#include <libexplain/open_flags.h>
#include <libexplain/permission_mode.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_open [ <option>... ] <path>\n");
    fprintf(stderr, "       test_open -V\n");
    exit(1);
}


int
main(int argc, char **argv)
{
    int             fd;
    const char      *path;
    int             flags;
    int             mode;

    fd = -1;
    path = 0;
    flags = O_RDONLY;
    mode = 0666;
    for (;;)
    {
        int c = getopt(argc, argv, "f:m:V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'f':
            flags = libexplain_open_flags_parse(optarg);
            if (flags < 0)
            {
                fprintf
                (
                    stderr,
                    "-f \"%s\" does not look like open flags\n",
                    optarg
                );
                exit(1);
            }
            break;

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
    path = argv[optind];

    fd = open(path, flags, mode);
    if (fd < 0)
    {
        libexplain_wrap_and_print(stderr, libexplain_open(path, flags, mode));
        exit(1);
    }
    if (close(fd))
    {
        libexplain_wrap_and_print(stderr, libexplain_close(fd));
        exit(1);
    }
    return 0;
}
