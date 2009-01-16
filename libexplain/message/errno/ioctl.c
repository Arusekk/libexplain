/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/buffer/errno/ioctl.h>
#include <libexplain/ioctl.h>


void
libexplain_message_errno_ioctl(char *message, int message_size, int errnum,
    int fildes, int request, void *data)
{
    libexplain_string_buffer_t sb;

    libexplain_string_buffer_init(&sb, message, message_size);
    libexplain_buffer_errno_ioctl(&sb, errnum, fildes, request, data);
}

/* vim:ts=8:sw=4:et */
