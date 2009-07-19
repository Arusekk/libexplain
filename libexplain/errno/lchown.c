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

#include <libexplain/common_message_buffer.h>
#include <libexplain/lchown.h>


const char *
explain_errno_lchown(int errnum, const char *pathname, int owner, int group)
{
    explain_message_errno_lchown
    (
        explain_common_message_buffer,
        explain_common_message_buffer_size,
        errnum,
        pathname,
        owner,
        group
    );
    return explain_common_message_buffer;
}