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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/string.h>

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/getcwd.h>
#include <libexplain/buffer/get_current_directory.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>
#include <libexplain/option.h>


static void
libexplain_buffer_errno_getcwd_system_call(libexplain_string_buffer_t *sb,
    int errnum, char *data, size_t data_size)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "getcwd(data = ");
    libexplain_buffer_pointer(sb, data);
    libexplain_string_buffer_printf(sb, ", data_size = %ld)", (long)data_size);
}


static void
libexplain_buffer_errno_getcwd_explanation(libexplain_string_buffer_t *sb,
    int errnum, char *data, size_t data_size)
{
    char            pathname[PATH_MAX * 2 + 1];

    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/getcwd.html
     */
    switch (errnum)
    {
    case EINVAL:
        libexplain_string_buffer_puts
        (
            sb,
            "the data_size argument is zero and data is not the NULL pointer"
        );
        break;

    case ERANGE:
        libexplain_string_buffer_puts
        (
            sb,
            "the data_size argument is less than the length of the "
            "working directory name, you need to allocate a bigger "
            "array and try again"
        );
        if (data && data_size && libexplain_option_dialect_specific())
        {
            libexplain_string_buffer_t nowhere;

            libexplain_string_buffer_init(&nowhere, 0, 0);
            if
            (
                !libexplain_buffer_get_current_directory
                (
                    &nowhere,
                    pathname,
                    sizeof(pathname)
                )
            )
            {
                libexplain_string_buffer_printf
                (
                    sb,
                    " (%ld < %ld)",
                    (long)data_size,
                    (long)(strlen(pathname) + 1)
                );
            }
        }
        break;

    case EACCES:
        if
        (
            !libexplain_buffer_get_current_directory
            (
                sb,
                pathname,
                sizeof(pathname)
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "read or search permission was denied for a component "
                "of the pathname"
            );
        }
        break;

    case ENOMEM:
        libexplain_buffer_enomem_user(sb);
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "data");
        break;

    case ENOENT:
        if
        (
            !libexplain_buffer_get_current_directory
            (
                sb,
                pathname,
                sizeof(pathname)
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "the current working directory has been unlinked"
            );
        }
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_getcwd(libexplain_string_buffer_t *sb, int errnum,
    char *data, size_t data_size)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_getcwd_system_call
    (
        &exp.system_call_sb,
        errnum,
        data,
        data_size
    );
    libexplain_buffer_errno_getcwd_explanation
    (
        &exp.explanation_sb,
        errnum,
        data,
        data_size
    );
    libexplain_explanation_assemble(&exp, sb);
}
