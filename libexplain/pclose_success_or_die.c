/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/buffer/errno/pclose.h>
#include <libexplain/buffer/wait_status.h>
#include <libexplain/common_message_buffer.h>
#include <libexplain/pclose.h>
#include <libexplain/string_buffer.h>
#include <libexplain/wrap_and_print.h>


int
libexplain_pclose_success(FILE *fp)
{
    int             status;

    status = pclose(fp);
    if (status < 0)
    {
        libexplain_wrap_and_print(stderr, libexplain_pclose(fp));
    }
    else if (status != 0)
    {
        libexplain_string_buffer_t sb;

        libexplain_string_buffer_init
        (
            &sb,
            libexplain_common_message_buffer,
            libexplain_common_message_buffer_size
        );
        libexplain_buffer_errno_pclose(&sb, 0, 0);

        /* FIXME: i18n */
        libexplain_string_buffer_puts(&sb, ", but ");
        libexplain_buffer_wait_status(&sb, status);
        libexplain_wrap_and_print(stderr, libexplain_common_message_buffer);
    }
    return status;
}


void
libexplain_pclose_success_or_die(FILE *fp)
{
    if (libexplain_pclose_success(fp))
        exit(EXIT_FAILURE);
}
