/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010 Peter Miller
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

#include <libexplain/ac/errno.h>

#include <libexplain/access.h>
#include <libexplain/buffer/errno/access.h>
#include <libexplain/common_message_buffer.h>
#include <libexplain/string_buffer.h>


const char *
explain_access(const char *pathname, int mode)
{
    return explain_errno_access(errno, pathname, mode);
}


const char *
explain_errno_access(int errnum, const char *pathname, int mode)
{
    explain_message_errno_access
    (
        explain_common_message_buffer,
        explain_common_message_buffer_size,
        errnum,
        pathname,
        mode
    );
    return explain_common_message_buffer;
}


void
explain_message_access(char *message, int message_size, const char *pathname,
    int mode)
{
    explain_message_errno_access
    (
        message,
        message_size,
        errno,
        pathname,
        mode
    );
}


void
explain_message_errno_access(char *message, int message_size, int errnum,
    const char *pathname, int mode)
{
    explain_string_buffer_t sb;

    explain_string_buffer_init(&sb, message, message_size);
    explain_buffer_errno_access(&sb, errnum, pathname, mode);
}
