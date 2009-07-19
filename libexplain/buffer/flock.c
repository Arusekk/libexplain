/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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

#include <libexplain/ac/fcntl.h>

#include <libexplain/buffer/flock.h>
#include <libexplain/buffer/lseek_whence.h>
#include <libexplain/string_buffer.h>


void
explain_buffer_flock(explain_string_buffer_t *sb, const struct flock *flp)
{
    explain_string_buffer_puts(sb, "{ l_type = ");
    switch (flp->l_type)
    {
    case F_RDLCK:
        explain_string_buffer_puts(sb, "F_RDLCK");
        break;

    case F_WRLCK:
        explain_string_buffer_puts(sb, "F_WRLCK");
        break;

    case F_UNLCK:
        explain_string_buffer_puts(sb, "F_UNLCK");
        break;

    default:
        explain_string_buffer_printf(sb, "%d", flp->l_type);
        break;
    }
    explain_string_buffer_puts(sb, "; l_whence = ");
    explain_buffer_lseek_whence(sb, flp->l_whence);
    explain_string_buffer_printf
    (
        sb,
        "; l_start = %lld; ",
        (long long)flp->l_start
    );
    explain_string_buffer_printf
    (
        sb,
        "l_len = %lld; ",
        (long long)flp->l_len
    );
    explain_string_buffer_printf(sb, "l_pid = %d }", (int)flp->l_pid);
}


#ifdef F_GETLK64
#if F_GETLK64 != F_GETLK

void
explain_buffer_flock64(explain_string_buffer_t *sb,
    const struct flock64 *flp)
{
    explain_string_buffer_puts(sb, "{ l_type = ");
    switch (flp->l_type)
    {
    case F_RDLCK:
        explain_string_buffer_puts(sb, "F_RDLCK");
        break;

    case F_WRLCK:
        explain_string_buffer_puts(sb, "F_WRLCK");
        break;

    case F_UNLCK:
        explain_string_buffer_puts(sb, "F_UNLCK");
        break;

    default:
        explain_string_buffer_printf(sb, "%d", flp->l_type);
        break;
    }
    explain_string_buffer_puts(sb, "; l_whence = ");
    explain_buffer_lseek_whence(sb, flp->l_whence);
    explain_string_buffer_printf
    (
        sb,
        "; l_start = %lld; ",
        (long long)flp->l_start
    );
    explain_string_buffer_printf
    (
        sb,
        "l_len = %lld; ",
        (long long)flp->l_len
    );
    explain_string_buffer_printf(sb, "l_pid = %d }", (int)flp->l_pid);
}

#endif
#endif
