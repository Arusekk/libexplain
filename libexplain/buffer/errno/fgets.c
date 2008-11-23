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

#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/fgets.h>
#include <libexplain/buffer/errno/read.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/stream_to_pathname.h>
#include <libexplain/explanation.h>
#include <libexplain/stream_to_fildes.h>


static void
libexplain_buffer_errno_fgets_system_call(libexplain_string_buffer_t *sb,
    int errnum, char *data, int data_size, FILE *fp)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "fgets(data = ");
    libexplain_buffer_pointer(sb, data);
    libexplain_string_buffer_printf(sb, ", data_size = %d", data_size);
    libexplain_string_buffer_puts(sb, ", fp = ");
    libexplain_buffer_pointer(sb, fp);
    libexplain_buffer_stream_to_pathname(sb, fp);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_fgets_explanation(libexplain_string_buffer_t *sb,
    int errnum, char *data, int data_size, FILE *fp)
{
    if (!fp)
    {
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This message is used when a file stream pointer
             * is invalid because it is the NULL pointer.
             */
            i18n("fp is the NULL pointer")
        );
        return;
    }
    libexplain_buffer_errno_read_explanation
    (
        sb,
        errnum,
        libexplain_stream_to_fildes(fp),
        data,
        data_size
    );
}


void
libexplain_buffer_errno_fgets(libexplain_string_buffer_t *sb, int errnum,
    char *data, int data_size, FILE *fp)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_fgets_system_call
    (
        &exp.system_call_sb,
        errnum,
        data,
        data_size,
        fp
    );
    libexplain_buffer_errno_fgets_explanation
    (
        &exp.explanation_sb,
        errnum,
        data,
        data_size,
        fp
    );
    libexplain_explanation_assemble(&exp, sb);
}
