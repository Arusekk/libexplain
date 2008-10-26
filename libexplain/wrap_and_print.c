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

#include <libexplain/ac/ctype.h>

#include <libexplain/wrap_and_print.h>
#include <libexplain/string_buffer.h>

#define LINE_LENGTH 75


void
libexplain_wrap_and_print(FILE *fp, const char *text)
{
    const char      *cp;
    char            line_string[LINE_LENGTH + 1];
    libexplain_string_buffer_t line_buf;
    char            word_string[LINE_LENGTH + 1];
    libexplain_string_buffer_t word_buf;

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
            if (word_buf.position >= LINE_LENGTH)
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
        else if (line_buf.position + 1 + word_buf.position <= LINE_LENGTH)
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
    }
}
