/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/buffer/ssize_t.h>


void
explain_buffer_ssize_t(explain_string_buffer_t *sb, ssize_t value)
{
#ifdef HAVE_Z_MODIFIER
    explain_string_buffer_printf("%zd", value);
#else
#if SIZEOF_SIZE_T > SIZEOF_LONG
        explain_string_buffer_printf(sb, "%lld", (long long)value);
#else
        explain_string_buffer_printf(sb, "%ld", (long)value);
#endif
#endif
}
