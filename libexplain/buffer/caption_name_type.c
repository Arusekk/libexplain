/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/caption_name_type.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/gettext.h>


void
explain_buffer_caption_name_type(explain_string_buffer_t *sb,
    const char *caption, const char *name, int st_mode)
{
    if (name && S_ISDIR(st_mode) && 0 == strcmp(name, "."))
    {
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext: the name of the current directory, rather than
             * "." that not all users understand.
             */
            i18n("current directory")
        );
        return;
    }

    /* the rule is: [caption] [name] type */
    if (caption)
    {
        explain_string_buffer_puts(sb, caption);
        explain_string_buffer_putc(sb, ' ');
    }
    if (name)
    {
        explain_string_buffer_puts_quoted(sb, name);
        explain_string_buffer_putc(sb, ' ');
    }
    if (st_mode < 0)
        explain_string_buffer_puts(sb, "file");
    else
        explain_buffer_file_type(sb, st_mode);
}
