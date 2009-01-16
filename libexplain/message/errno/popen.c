/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
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

#include <libexplain/buffer/errno/popen.h>
#include <libexplain/popen.h>


void
libexplain_message_errno_popen(char *message, int message_size, int errnum,
    const char *command, const char *flags)
{
    libexplain_string_buffer_t sb;

    libexplain_string_buffer_init(&sb, message, message_size);
    libexplain_buffer_errno_popen(&sb, errnum, command, flags);
}

/* vim:ts=8:sw=4:et */
