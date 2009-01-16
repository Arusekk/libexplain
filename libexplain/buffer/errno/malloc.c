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

#include <libexplain/ac/errno.h>

#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/malloc.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_malloc_system_call(libexplain_string_buffer_t *sb,
    int errnum, size_t size)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "malloc(size = %ld)", (long)size);
}


static void
libexplain_buffer_errno_malloc_explanation(libexplain_string_buffer_t *sb,
    int errnum, size_t size)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/malloc.html
     */
    (void)size;
    switch (errnum)
    {
    case ENOMEM:
        libexplain_buffer_enomem_user(sb);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_malloc(libexplain_string_buffer_t *sb, int errnum,
    size_t size)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_malloc_system_call
    (
        &exp.system_call_sb,
        errnum,
        size
    );
    libexplain_buffer_errno_malloc_explanation
    (
        &exp.explanation_sb,
        errnum,
        size
    );
    libexplain_explanation_assemble(&exp, sb);
}

/* vim:ts=8:sw=4:et */
