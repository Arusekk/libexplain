/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/fopen.h>
#include <libexplain/buffer/errno/open.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>
#include <libexplain/string_buffer.h>
#include <libexplain/string_flags.h>


static void
libexplain_buffer_errno_fopen_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, const char *flags_string)
{
    /*
     * Note: EFAULT has to be the pathname, because if flags was broken,
     * it would have raised a SEGFAULT signal from user space.
     */

    libexplain_string_buffer_printf(sb, "fopen(pathname = ");
    if (errnum == EFAULT)
        libexplain_buffer_pointer(sb, pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_puts(sb, ", flags = ");
    libexplain_string_buffer_puts_quoted(sb, flags_string);
    libexplain_string_buffer_putc(sb, ')');
}


void
libexplain_buffer_errno_fopen_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, const char *flags)
{
    libexplain_string_flags_t sf;
    int             permission_mode;

    libexplain_string_flags_init(&sf, flags);
    permission_mode = 0666;
    switch (errnum)
    {
    case EINVAL:
        libexplain_string_flags_einval(&sf, sb, "flags");
        break;

    case ENOMEM:
        {
            int             fd;

            /*
             * Try to figure out if it was a kernel ENOMEM or a user-space
             * (sbrk) ENOMEM.  This is doomed to be inaccurate.
             */
            errno = 0;
            fd = open(pathname, O_RDONLY);
            if (fd < 0)
            {
                if (errno == ENOMEM)
                {
                    libexplain_buffer_enomem_kernel(sb);
                    break;
                }
                libexplain_buffer_enomem_kernel_or_user(sb);
                break;
            }
            close(fd);
            libexplain_buffer_enomem_user(sb);
        }
        break;

    default:
        /*
         * Punt everything else to open()
         */
        libexplain_buffer_errno_open_explanation
        (
            sb,
            errnum,
            pathname,
            sf.flags,
            permission_mode
        );
        break;
    }
}


void
libexplain_buffer_errno_fopen(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, const char *flags_string)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_fopen_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        flags_string
    );
    libexplain_buffer_errno_fopen_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname,
        flags_string
    );
    libexplain_explanation_assemble(&exp, sb);
}


void
libexplain_string_flags_einval(const libexplain_string_flags_t *sf,
    libexplain_string_buffer_t *sb, const char *caption)
{
    size_t          n;

    libexplain_string_buffer_printf
    (
        sb,
        "the %s argument is not valid",
        caption
    );
    if (!sf->rwa_seen)
    {
        libexplain_string_buffer_puts
        (
            sb,
            ", you must specify 'r', 'w' or 'a' at the start of the string"
        );
    }
    n = strlen(sf->invalid);
    if (n > 0)
    {
        libexplain_string_buffer_printf
        (
            sb,
            ", flag character%s ",
            (n == 1 ? "" : "s")
        );
        libexplain_string_buffer_puts_quoted(sb, sf->invalid);
        libexplain_string_buffer_puts(sb, " unknown");
    }
}
