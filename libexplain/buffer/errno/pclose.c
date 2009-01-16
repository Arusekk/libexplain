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

#include <libexplain/ac/stdio.h>

#include <libexplain/buffer/errno/pclose.h>
#include <libexplain/buffer/errno/waitpid.h>
#include <libexplain/buffer/stream.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_pclose_system_call(libexplain_string_buffer_t *sb,
    int errnum, FILE *fp)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "pclose(fp = ");
    libexplain_buffer_stream(sb, fp);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_pclose_explanation(libexplain_string_buffer_t *sb,
    int errnum, FILE *fp)
{
    /* punt everything to waitpid */
    (void)fp;
    libexplain_buffer_errno_waitpid_explanation(sb, errnum, 0, 0, 0);
}


void
libexplain_buffer_errno_pclose(libexplain_string_buffer_t *sb, int errnum,
    FILE *fp)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_pclose_system_call
    (
        &exp.system_call_sb,
        errnum,
        fp
    );
    libexplain_buffer_errno_pclose_explanation
    (
        &exp.explanation_sb,
        errnum,
        fp
    );
    libexplain_explanation_assemble(&exp, sb);
}

/* vim:ts=8:sw=4:et */
