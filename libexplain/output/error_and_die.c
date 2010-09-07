/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2010 Peter Miller
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

#include <libexplain/ac/limits.h> /* for PATH_MAX on Solaris */
#include <libexplain/ac/stdarg.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/sys/param.h> /* for PATH_MAX except Solaris */

#include <libexplain/option.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <libexplain/string_buffer.h>


void
explain_output_error_and_die(const char *format, ...)
{
    va_list         ap;
    explain_string_buffer_t sb;

    /*
     * Note: we can't use explain_common_message_buffer, just in case
     * one of the format argumnets *is* explain_common_message_buffer.
     * And for the same reason, we need to be about the same size.
     */
    char buf[PATH_MAX * 2 + 200];

    explain_string_buffer_init(&sb, buf, sizeof(buf));
    explain_program_name_assemble_internal(1);
    if (explain_option_assemble_program_name())
    {
        explain_string_buffer_puts(&sb, explain_program_name_get());
        explain_string_buffer_puts(&sb, ": ");
    }

    va_start(ap, format);
    explain_string_buffer_vprintf(&sb, format, ap);
    va_end(ap);

    explain_output_message(buf);
    explain_output_exit_failure();
}
