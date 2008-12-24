/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/stream.h>
#include <libexplain/buffer/stream_to_pathname.h>


void
libexplain_buffer_stream(libexplain_string_buffer_t *sb, FILE *fp)
{
    if (fp == stdin)
        libexplain_string_buffer_puts(sb, "stdin");
    else if (fp == stdout)
        libexplain_string_buffer_puts(sb, "stdout");
    else if (fp == stderr)
        libexplain_string_buffer_puts(sb, "stderr");
    else
        libexplain_buffer_pointer(sb, fp);
    libexplain_buffer_stream_to_pathname(sb, fp);
}
