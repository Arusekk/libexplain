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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/string.h>

#include <libexplain/string_buffer.h>


void
libexplain_string_buffer_init(libexplain_string_buffer_t *sb, char *message,
    int message_size)
{
    if (!message || message_size <= 0)
    {
        static char dummy[1];
        message = dummy;
        message_size = 1;
    }

    sb->message = message;
    sb->position = 0;
    sb->maximum = message_size;
    sb->message[0] = '\0';
}


void
libexplain_string_buffer_putc(libexplain_string_buffer_t *sb, int c)
{
    char *end = sb->message + sb->maximum - 1;
    char *cp = sb->message + sb->position;
    if (cp < end)
    {
        cp[0] = c;
        cp[1] = '\0';
        sb->position++;
    }
}


void
libexplain_string_buffer_puts(libexplain_string_buffer_t *sb, const char *s)
{
    char            *end;
    char            *cp;

    if (!s)
        return;
    end = sb->message + sb->maximum;
    cp = sb->message + sb->position;
    cp = strendcpy(cp, s, end);
    sb->position = cp - sb->message;
}


void
libexplain_string_buffer_puts_quoted(libexplain_string_buffer_t *sb,
    const char *s)
{
    if (!s)
    {
        libexplain_string_buffer_puts(sb, "NULL");
        return;
    }
    libexplain_string_buffer_putc(sb, '"');
    for (;;)
    {
        unsigned char   c;

        c = *s++;
        switch (c)
        {
        case '\0':
            libexplain_string_buffer_putc(sb, '"');
            return;

        case '\\':
        case '"':
            libexplain_string_buffer_putc(sb, '\\');
            libexplain_string_buffer_putc(sb, c);
            break;

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

        default:
            libexplain_string_buffer_putc(sb, c);
            break;
        }
    }
}


void
libexplain_string_buffer_printf(libexplain_string_buffer_t *sb,
    const char *fmt, ...)
{
    va_list         ap;

    va_start(ap, fmt);
    libexplain_string_buffer_vprintf(sb, fmt, ap);
    va_end(ap);
}


void
libexplain_string_buffer_vprintf(libexplain_string_buffer_t *sb,
    const char *fmt, va_list ap)
{
    char *end = sb->message + sb->maximum;
    char *cp = sb->message + sb->position;
    vsnprintf(cp, end - cp, fmt, ap);
    sb->position += strlen(cp);
}


void
libexplain_string_buffer_copy(libexplain_string_buffer_t *dst,
    const libexplain_string_buffer_t *src)
{
    dst->position = 0;
    libexplain_string_buffer_puts(dst, src->message);
}


void
libexplain_string_buffer_path_join(libexplain_string_buffer_t *dst,
    const char *s)
{
    while (s[0] == '.' && s[1] == '/')
        s += 2;
    if (s[0] == '.' && s[1] == '\0')
        return;
    if (s[0] == '\0')
        return;
    if (dst->position == 1 && dst->message[0] == '.')
        dst->position = 0;
    else if (dst->position > 0 && dst->message[dst->position - 1] != '/')
        libexplain_string_buffer_putc(dst, '/');
    libexplain_string_buffer_puts(dst, s);
}


int
libexplain_string_buffer_full(const libexplain_string_buffer_t *sb)
{
    return (sb->position >= sb->maximum - 1);
}
