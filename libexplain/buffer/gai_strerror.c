/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/netdb.h>

#include <libexplain/buffer/gai_strerror.h>
#include <libexplain/buffer/strerror.h>
#include <libexplain/option.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>


static const libexplain_parse_bits_table_t table[] =
{
    { "EAI_BADFLAGS", EAI_BADFLAGS },
    { "EAI_NONAME", EAI_NONAME },
    { "EAI_AGAIN", EAI_AGAIN },
    { "EAI_FAIL", EAI_FAIL },
    { "EAI_FAMILY", EAI_FAMILY },
    { "EAI_SOCKTYPE", EAI_SOCKTYPE },
    { "EAI_SERVICE", EAI_SERVICE },
    { "EAI_MEMORY", EAI_MEMORY },
    { "EAI_SYSTEM", EAI_SYSTEM },
    { "EAI_OVERFLOW", EAI_OVERFLOW },
    { "EAI_NODATA", EAI_NODATA },
    { "EAI_ADDRFAMILY", EAI_ADDRFAMILY },
    { "EAI_INPROGRESS", EAI_INPROGRESS },
    { "EAI_CANCELED", EAI_CANCELED },
    { "EAI_NOTCANCELED", EAI_NOTCANCELED },
    { "EAI_ALLDONE", EAI_ALLDONE },
    { "EAI_INTR", EAI_INTR },
    { "EAI_IDN_ENCODE", EAI_IDN_ENCODE },
};


void
libexplain_buffer_gai_strerror(libexplain_string_buffer_t *sb, int errnum)
{
    const libexplain_parse_bits_table_t *tp;
    int             first;

    if (errnum > 0)
    {
        libexplain_buffer_strerror(sb, errnum);
        return;
    }

    libexplain_string_buffer_puts(sb, gai_strerror(errnum));

    first = 1;
    if (libexplain_option_numeric_errno())
    {
        libexplain_string_buffer_printf(sb, " (%d", errnum);
        first = 0;
    }
    tp = libexplain_parse_bits_find_by_value(errnum, table, SIZEOF(table));
    if (tp)
    {
        if (first)
            libexplain_string_buffer_puts(sb, " (");
        else
            libexplain_string_buffer_puts(sb, ", ");
        libexplain_string_buffer_puts(sb, tp->name);
        first = 0;
    }
    if (!first)
        libexplain_string_buffer_putc(sb, ')');
}
