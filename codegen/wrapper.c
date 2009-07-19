/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/ac/ctype.h>
#include <libexplain/ac/string.h>

#include <codegen/string_buffer.h>
#include <codegen/wrapper.h>

#define DEFAULT_LINE_WIDTH 75


void
wrapper(FILE *fp, const char *prefix, const char *text)
{
    size_t          prefix_len;
    size_t          width;
    const char      *cp;
    static string_buffer_t line_buf;
    static string_buffer_t word_buf;

    if (!prefix)
        prefix = "";
    prefix_len = strlen(prefix);
    width = DEFAULT_LINE_WIDTH;
    if (prefix_len < DEFAULT_LINE_WIDTH)
        width = DEFAULT_LINE_WIDTH - prefix_len;
    string_buffer_rewind(&line_buf);
    cp = text;
    for (;;)
    {
        unsigned char c = *cp++;
        if (c == '\0')
        {
            if (line_buf.data_length)
            {
                fputs(prefix, fp);
                string_buffer_fwrite(&line_buf, fp);
                putc('\n', fp);
            }
            return;
        }

        if (isspace(c))
            continue;

        /*
         * Grab the next word.
         */
        string_buffer_rewind(&word_buf);
        for (;;)
        {
            string_buffer_putc(&word_buf, c);
            c = *cp;
            if (c == '\0')
                break;
            if (isspace(c))
                break;
            ++cp;
        }

        if (line_buf.data_length == 0)
        {
            /* do nothing */
        }
        else if (line_buf.data_length + 1 + word_buf.data_length <= width)
        {
            string_buffer_putc(&line_buf, ' ');
        }
        else
        {
            fputs(prefix, fp);
            string_buffer_fwrite(&line_buf, fp);
            putc('\n', fp);
            string_buffer_rewind(&line_buf);
        }
        string_buffer_puts(&line_buf, string_buffer_get(&word_buf));
    }
}


void
wrapper_hang(FILE *fp, const char *prefix, const char *text)
{
    size_t          prefix_len;
    size_t          width;
    const char      *cp;
    static string_buffer_t line_buf;
    static string_buffer_t word_buf;
    int             extra_indent;

    if (!prefix)
        prefix = "";
    prefix_len = strlen(prefix);
    width = 80;
    if (prefix_len < DEFAULT_LINE_WIDTH)
        width -= prefix_len;
    string_buffer_rewind(&line_buf);
    cp = text;
    extra_indent = 0;
    for (;;)
    {
        unsigned char c = *cp++;
        if (c == '\0')
        {
            if (line_buf.data_length)
            {
                fputs(prefix, fp);
                if (extra_indent)
                    fputs("    ", fp);
                string_buffer_fwrite(&line_buf, fp);
                putc('\n', fp);
            }
            return;
        }

        if (c == '\t')
        {
            for (;;)
            {
                string_buffer_putc(&line_buf, ' ');
                if (line_buf.data_length >= 16)
                    break;
            }
            continue;
        }
        if (c == '\n')
        {
            if (line_buf.data_length)
            {
                fputs(prefix, fp);
                if (extra_indent)
                    fputs("    ", fp);
                string_buffer_fwrite(&line_buf, fp);
                putc('\n', fp);
            }
            string_buffer_rewind(&line_buf);
            extra_indent = 0;
            continue;
        }

        if (isspace(c))
            continue;

        /*
         * Grab the next word.
         */
        string_buffer_rewind(&word_buf);
        for (;;)
        {
            string_buffer_putc(&word_buf, c);
            c = *cp;
            if (c == '\0')
                break;
            if (isspace(c))
                break;
            ++cp;
        }

        if (line_buf.data_length == 0)
        {
            /* do nothing */
        }
        else if (line_buf.data[line_buf.data_length - 1] == ' ')
        {
            /* do nothing */
        }
        else if (line_buf.data_length + 1 + word_buf.data_length <= width)
        {
            string_buffer_putc(&line_buf, ' ');
        }
        else
        {
            fputs(prefix, fp);
            if (extra_indent)
                fputs("    ", fp);
            string_buffer_fwrite(&line_buf, fp);
            putc('\n', fp);
            string_buffer_rewind(&line_buf);
            if (!extra_indent)
            {
                extra_indent = 1;
                width -= 4;
            }
        }
        string_buffer_puts(&line_buf, string_buffer_get(&word_buf));
    }
}
