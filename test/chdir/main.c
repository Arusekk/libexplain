/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010, 2012 Peter Miller
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

#include <libexplain/chdir.h>
#include <libexplain/output.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_chdir [ <option>... ] <path>\n");
    fprintf(stderr, "       test_chdir -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *path;

    path = 0;
    for (;;)
    {
        int c = getopt(argc, argv, "sV");
        if (c == EOF)
            break;
        switch (c)
        {
        case 's':
            explain_output_register
            (
                explain_output_tee_new
                (
                    explain_output_stderr_new(),
                    explain_output_syslog_new()
                )
            );
            break;

        case 'V':
            explain_version_print();
            return 0;

        default:
            usage();
        }
    }
    if (optind + 1 != argc)
        usage();
    path = argv[optind];

    explain_chdir_or_die(path);
    return 0;
}


/* vim: set ts=8 sw=4 et : */
