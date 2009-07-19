/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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
#include <libexplain/ac/string.h>
#include <getopt.h>

#include <libexplain/errno_info.h>
#include <libexplain/freopen.h>
#include <libexplain/string_buffer.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: explain -e <errno> <function> [ <args> ... ]\n");
    fprintf(stderr, "       explain -m <message> <function> [ <args> ... ]\n");
    fprintf(stderr, "       explain -V\n");
    exit(EXIT_FAILURE);
}


static const struct option options[] =
{
    { "errno", 1, 0, 'e' },
    { "message", 1, 0, 'm' },
    { "version", 0, 0, 'V' },
    { 0, 0, 0, 0 }
};


static int exit_status;


static int
figure_out_error(const char *text)
{
    long            n;
    char            *ep;
    const explain_errno_info_t *eip;
    explain_string_buffer_t sb;
    char            message[200];

    explain_string_buffer_init(&sb, message, sizeof(message));

    /* is it a number? */
    n = strtol(text, &ep, 0);
    if (ep != text && !*ep)
        return n;

    /* is it a symbol? */
    eip = explain_errno_info_by_name(text);
    if (eip)
        return eip->error_number;

    /* is it an exact text? */
    eip = explain_errno_info_by_text(text);
    if (eip)
        return eip->error_number;

    /* is it a fuzzy symbol? */
    eip = explain_errno_info_by_name_fuzzy(text);
    if (eip)
    {
        explain_string_buffer_puts(&sb, "the error ");
        explain_string_buffer_puts_quoted(&sb, text);
        explain_string_buffer_puts
        (
            &sb,
            " doesn't match any known symbol, guessing you meant "
        );
        explain_string_buffer_puts_quoted(&sb, eip->name);
        explain_string_buffer_puts(&sb, " instead");
        explain_wrap_and_print(stderr, message);
        exit_status = EXIT_FAILURE;
        return eip->error_number;
    }

    /* is it a fuzzy text? */
    eip = explain_errno_info_by_text_fuzzy(text);
    if (eip)
    {
        explain_string_buffer_puts(&sb, "the error ");
        explain_string_buffer_puts_quoted(&sb, text);
        explain_string_buffer_puts
        (
            &sb,
            " doesn't match any known symbol, guessing you meant "
        );
        explain_string_buffer_puts_quoted(&sb, strerror(eip->error_number));
        explain_string_buffer_puts(&sb, " instead");
        explain_wrap_and_print(stderr, message);
        exit_status = EXIT_FAILURE;
        return eip->error_number;
    }

    explain_string_buffer_puts(&sb, "the error ");
    explain_string_buffer_puts_quoted(&sb, text);
    explain_string_buffer_puts
    (
        &sb,
        " doesn't look like a number, or an error symbol, or a "
        "strerror() string, aborting"
    );
    explain_wrap_and_print(stderr, message);
    exit(EXIT_FAILURE);
    /* NOTREACHED */
    return 0;
}


int
main(int argc, char **argv)
{
    func_t          func;
    int             err;

    exit_status = EXIT_SUCCESS;
    err = -1;
    for (;;)
    {
        int c = getopt_long(argc, argv, "e:o:V", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'e':
            err = figure_out_error(optarg);
            break;

        case 'o':
            explain_freopen_or_die(optarg, "w", stdout);
            break;

        case 'V':
            explain_version_print();
            return 0;

        default:
            usage();
            /* NOTREACHED */
        }
    }
    if (err < 0)
    {
        fprintf(stderr, "please specify an error number (-e)\n");
        exit(EXIT_FAILURE);
    }
    if (optind >= argc)
        usage();
    func = find_function(argv[optind]);
    ++optind;
    func(err, argc - optind, argv + optind);
    return exit_status;
}