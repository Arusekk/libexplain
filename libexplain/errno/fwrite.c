/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#include <libexplain/common_message_buffer.h>
#include <libexplain/fwrite.h>


const char *
libexplain_errno_fwrite(int errnum, const void *ptr, size_t size, size_t nmemb,
    FILE *fp)
{
    libexplain_message_errno_fwrite
    (
        libexplain_common_message_buffer,
        libexplain_common_message_buffer_size,
        errnum,
        ptr,
        size,
        nmemb,
        fp
    );
    return libexplain_common_message_buffer;
}
