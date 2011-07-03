/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2011 Peter Miller
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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>
#include <libexplain/ac/getopt.h>

#include <libexplain/iocontrol.h>
#include <libexplain/version_print.h>

#include <ioctl-scan/probe.h>
#include <ioctl-scan/scan.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: ioctl-scan --block <device>\n");
    fprintf(stderr, "       ioctl-scan -V\n");
    exit(EXIT_FAILURE);
}


static const struct option options[] =
{
    { "block", 1, 0, 'B' },
    { "dangerous-probe", 1, 0, 256 },
    { "version", 0, 0, 'V' },
    { 0, 0, 0, 0 }
};


int
main(int argc, char **argv)
{
    for (;;)
    {
        int c = getopt_long(argc, argv, "B:V", options, 0);
        if (c < 0)
            break;
        switch (c)
        {
        case 'B':
            scan_block_device(optarg);
            return 0;

        case 256:
            probe(optarg);
            return 0;

        case 'V':
            explain_version_print();
            return 0;

        default:
            usage();
            /* NOTREACHED */
        }
    }
    usage();
    return 0;
}
