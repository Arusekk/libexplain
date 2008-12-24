/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#include <libexplain/ac/sys/socket.h>
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/wrong_file_type.h>


void
libexplain_buffer_wrong_file_type_st(libexplain_string_buffer_t *sb,
    const struct stat *st, const char *caption, int required_file_type)
{
    if (st)
    {
        libexplain_string_buffer_t wrong_sb;
        libexplain_string_buffer_t right_sb;
        char wrong[100];
        char right[100];

        libexplain_string_buffer_init(&wrong_sb, wrong, sizeof(wrong));
        libexplain_buffer_file_type(&wrong_sb, st->st_mode);
        libexplain_string_buffer_init(&right_sb, right, sizeof(right));
        libexplain_buffer_file_type(&right_sb, required_file_type);

        libexplain_string_buffer_printf_gettext
        (
            sb,
            /*
             * xgettext: This message is used when a file descriptor is
             * passed to a system call, and it has the wrong file type.
             *
             * %1$s => The name of the offending system call argument
             * %2$s => the actual (wrong) file type, already translated.
             * %3$s => the required file type, already translated.
             */
            i18n("%s refers to a %s, not a %s"),
            caption,
            wrong,
            right
        );
    }
    else
    {
        libexplain_string_buffer_t right_sb;
        char right[100];

        libexplain_string_buffer_init(&right_sb, right, sizeof(right));
        libexplain_buffer_file_type(&right_sb, required_file_type);

        libexplain_string_buffer_printf_gettext
        (
            sb,
            /*
             * xgettext: This message is used when a file descriptor is
             * passed to a system call, and it has the wrong file type,
             * but the actual file type is unavailable.
             *
             * %1$s => The name of the offending system call argument
             * %2$s => the required file type, already translated.
             */
            i18n("%s does not refer to a %s"),
            caption,
            right
        );
    }
}


void
libexplain_buffer_wrong_file_type(libexplain_string_buffer_t *sb, int fildes,
    const char *caption, int want_file_type)
{
    struct stat     st;

    if (fstat(fildes, &st) >= 0)
        libexplain_buffer_wrong_file_type_st(sb, &st, caption, want_file_type);
    else
        libexplain_buffer_wrong_file_type_st(sb, 0, caption, want_file_type);
}
