/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#include <libexplain/buffer/socket_domain.h>
#include <libexplain/buffer/socket_protocol.h>
#include <libexplain/buffer/socket_type.h>
#include <libexplain/socket.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_socket <domain> <type> <protocol>\n");
    fprintf(stderr, "       test_socket -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             domain;
    int             type;
    int             protocol;

    for (;;)
    {
        int c = getopt(argc, argv, "V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'V':
            libexplain_version_print();
            return EXIT_SUCCESS;

        default:
            usage();
        }
    }
    if (optind + 3 != argc)
        usage();

    domain = libexplain_parse_socket_domain(argv[optind]);
    if (domain < 0)
    {
        fprintf
        (
            stderr,
            "option \"%s\" does not look like a socket domain\n",
            argv[optind]
        );
        exit(EXIT_FAILURE);
    }

    type = libexplain_parse_socket_type(argv[optind + 1]);
    if (type < 0)
    {
        fprintf
        (
            stderr,
            "option \"%s\" does not look like a socket type\n",
            argv[optind + 1]
        );
        exit(EXIT_FAILURE);
    }

    protocol = libexplain_parse_socket_protocol(argv[optind + 2]);
    if (protocol < 0)
    {
        fprintf
        (
            stderr,
            "option \"%s\" does not look like a socket protocol\n",
            argv[optind + 2]
        );
        exit(EXIT_FAILURE);
    }

    libexplain_socket_or_die(domain, type, protocol);
    return EXIT_SUCCESS;
}
