/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/assert.h>
#include <libexplain/ac/ctype.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/unistd.h>

#ifdef HAVE_winsize_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#include <libexplain/wrap_and_print.h>
#include <libexplain/string_buffer.h>

#define DEFAULT_LINE_WIDTH 75

#define MAX_LINE_LENGTH (PATH_MAX + 10)


void
libexplain_wrap_and_print_width(FILE *fp, const char *text, int width)
{
    const char      *cp;
    char            line_string[MAX_LINE_LENGTH + 1];
    libexplain_string_buffer_t line_buf;
    char            word_string[MAX_LINE_LENGTH + 1];
    libexplain_string_buffer_t word_buf;

    assert(width > 0);
    if (width <= 0)
        width = DEFAULT_LINE_WIDTH ;
    if (width > MAX_LINE_LENGTH)
        width = MAX_LINE_LENGTH;
    assert(sizeof(word_string) <= sizeof(line_string));
    cp = text;
    libexplain_string_buffer_init(&line_buf, line_string, sizeof(line_string));
    libexplain_string_buffer_init(&word_buf, word_string, sizeof(word_string));
    for (;;)
    {
        unsigned char c = *cp++;
        if (c == '\0')
        {
            if (line_buf.position)
            {
                fwrite(line_string, line_buf.position, 1, fp);
                putc('\n', fp);
            }
            return;
        }

        if (isspace(c))
            continue;

        /*
         * Grab the next word.
         */
        word_buf.position = 0;
        for (;;)
        {
            libexplain_string_buffer_putc(&word_buf, c);
            if (libexplain_string_buffer_full(&word_buf))
                break;
            c = *cp;
            if (c == '\0')
                break;
            if (isspace(c))
                break;
            ++cp;
        }

        if (line_buf.position == 0)
        {
            /* do nothing */
        }
        else if (line_buf.position + 1 + word_buf.position <= (size_t)width)
        {
            libexplain_string_buffer_putc(&line_buf, ' ');
        }
        else
        {
            fwrite(line_string, line_buf.position, 1, fp);
            putc('\n', fp);
            line_buf.position = 0;
        }
        libexplain_string_buffer_puts(&line_buf, word_string);
        /*
         * Note: it is possible for a line to be longer than (width)
         * when it contains a single word that is itself longer than
         * (width).
         */
    }
}


void
libexplain_wrap_and_print(FILE *fp, const char *text)
{
    int             width;
    int             fildes;

    width = DEFAULT_LINE_WIDTH;

    /*
     * If output is going to a terminal, use the terminal's with when
     * formatting error messages.
     */
    fildes = fileno(fp);
    if (isatty(fildes))
    {
        const char      *cp;

        cp = getenv("COLS");
        if (cp && *cp)
        {
            char            *ep;

            width = strtol(cp, &ep, 0);
            if (ep == cp || *ep)
                width = 0;
        }
#ifdef TIOCGWINSZ
        if (width <= 0 )
        {
            struct winsize  ws;

            if (ioctl(fildes, TIOCGWINSZ, &ws) >= 0)
                width = ws.ws_col;
        }
#endif
        if (width <= 0)
            width = DEFAULT_LINE_WIDTH;
    }

    /*
     * Print the text using the window width.
     */
    libexplain_wrap_and_print_width(fp, text, width);
}
