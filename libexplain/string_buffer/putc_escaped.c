/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/ctype.h>

#include <libexplain/string_buffer.h>


void
libexplain_string_buffer_putc_escaped(libexplain_string_buffer_t *sb, int c,
    int delimiter)
{
    if (c == delimiter || c == '\\')
    {
        libexplain_string_buffer_putc(sb, '\\');
        libexplain_string_buffer_putc(sb, c);
        return;
    }
    switch (c)
    {
    case '\a':
        libexplain_string_buffer_puts(sb, "\\a");
        break;

    case '\b':
        libexplain_string_buffer_puts(sb, "\\b");
        break;

    case '\f':
        libexplain_string_buffer_puts(sb, "\\f");
        break;

    case '\n':
        libexplain_string_buffer_puts(sb, "\\n");
        break;

    case '\r':
        libexplain_string_buffer_puts(sb, "\\r");
        break;

    case '\t':
        libexplain_string_buffer_puts(sb, "\\t");
        break;

    case '\v':
        libexplain_string_buffer_puts(sb, "\\v");
        break;

    case ' ':
    case '!':
    case '"':
    case '#':
    case '$':
    case '%':
    case '&':
    case '\'':
    case '(':
    case ')':
    case '*':
    case '+':
    case ',':
    case '-':
    case '.':
    case '/':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case ':':
    case ';':
    case '<':
    case '=':
    case '>':
    case '?':
    case '@':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '[':
    case '\\':
    case ']':
    case '^':
    case '_':
    case '`':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case '{':
    case '|':
    case '}':
    case '~':
        printable:
        libexplain_string_buffer_putc(sb, c);
        break;

    default:
        /* this test will be locale specific */
        if (isprint((unsigned char)c))
            goto printable;
        libexplain_string_buffer_printf(sb, "\\%03o", (unsigned char)c);
        break;
    }
}
