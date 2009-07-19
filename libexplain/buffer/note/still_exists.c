/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/note/still_exists.h>


void
explain_buffer_note_still_exists(explain_string_buffer_t *sb,
    const char *caption)
{
    explain_string_buffer_puts(sb, "; ");
    explain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used to supplement a
         * rename(2) error explanation, and oldpath still
         * exists.
         *
         * %1$s => the name of the relevant system call argument
         */
        i18n("note that %s still exists"),
        caption
    );
}


void
explain_buffer_note_if_still_exists(explain_string_buffer_t *sb,
    const char *pathname, const char *caption)
{
    struct stat     st;

    if (lstat(pathname, &st) == 0)
        explain_buffer_note_still_exists(sb, caption);
}


void
explain_buffer_note_exists(explain_string_buffer_t *sb,
    const char *caption)
{
    explain_string_buffer_puts(sb, "; ");
    explain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used to supplement a
         * rename(2) error explanation, and oldpath still
         * exists.
         *
         * %1$s => the name of the relevant system call argument
         */
        i18n("note that %s exists"),
        caption
    );
}


void
explain_buffer_note_if_exists(explain_string_buffer_t *sb,
    const char *pathname, const char *caption)
{
    struct stat     st;

    if (lstat(pathname, &st) == 0)
        explain_buffer_note_exists(sb, caption);
}
