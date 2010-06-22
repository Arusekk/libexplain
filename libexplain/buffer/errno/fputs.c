/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>

#include <libexplain/buffer/errno/fputs.h>
#include <libexplain/buffer/errno/write.h>
#include <libexplain/buffer/pathname.h>
#include <libexplain/buffer/stream.h>
#include <libexplain/explanation.h>
#include <libexplain/stream_to_fildes.h>


static void
explain_buffer_errno_fputs_system_call(explain_string_buffer_t *sb, int errnum,
    const char *s, FILE *fp)
{
    (void)errnum;
    explain_string_buffer_puts(sb, "fputs(s = ");
    explain_buffer_pathname(sb, s);
    explain_string_buffer_puts(sb, ", fp = ");
    explain_buffer_stream(sb, fp);
    explain_string_buffer_putc(sb, ')');
}


void
explain_buffer_errno_fputs_explanation(explain_string_buffer_t *sb, int errnum,
    const char *syscall_name, const char *s, FILE *fp)
{
    int             fildes;

    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/fputs.html
     */
    (void)s;
    fildes = explain_stream_to_fildes(fp);
    explain_buffer_errno_write_explanation
    (
        sb,
        errnum,
        syscall_name,
        fildes,
        NULL,
        0
    );
}


void
explain_buffer_errno_fputs(explain_string_buffer_t *sb, int errnum, const char
    *s, FILE *fp)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_fputs_system_call(&exp.system_call_sb, errnum, s, fp);
    explain_buffer_errno_fputs_explanation(&exp.explanation_sb, errnum, "fputs",
        s, fp);
    explain_explanation_assemble(&exp, sb);
}


/* vim: set ts=8 sw=4 et */