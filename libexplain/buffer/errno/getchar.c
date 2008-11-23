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

#include <libexplain/ac/stdio.h>

#include <libexplain/buffer/errno/getchar.h>
#include <libexplain/buffer/errno/read.h>
#include <libexplain/buffer/stream_to_pathname.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_getchar_system_call(libexplain_string_buffer_t *sb,
    int errnum)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "getchar(");
    libexplain_buffer_stream_to_pathname(sb, stdin);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_getchar_explanation(libexplain_string_buffer_t *sb,
    int errnum)
{
    int             fildes;

    fildes = fileno(stdin);
    libexplain_buffer_errno_read_explanation(sb, errnum, fildes, NULL, 0);
}


void
libexplain_buffer_errno_getchar(libexplain_string_buffer_t *sb, int errnum)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_getchar_system_call(&exp.system_call_sb, errnum);
    libexplain_buffer_errno_getchar_explanation(&exp.explanation_sb, errnum);
    libexplain_explanation_assemble(&exp, sb);
}
