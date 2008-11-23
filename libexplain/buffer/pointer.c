/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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


void
libexplain_buffer_pointer(libexplain_string_buffer_t *sb, const void *ptr)
{
    if (ptr == NULL)
    {
        libexplain_string_buffer_puts(sb, "NULL");
    }
    else
    {
        /*
         * Some old systems don't have %p, and some new systems format
         * %p differently than glibc.  If this is an issue, we will
         * look at some ./configure magic to cope.
         */
        libexplain_string_buffer_printf(sb, "0x%08lX", (unsigned long)ptr);
    }
}
