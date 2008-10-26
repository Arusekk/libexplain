/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/fopen.h>
#include <libexplain/buffer/errno/open.h>
#include <libexplain/buffer/strerror.h>
#include <libexplain/buffer/success.h>
#include <libexplain/string_buffer.h>


void
libexplain_buffer_errno_fopen(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, const char *flags_string)
{
    int             flags_mode_part;
    int             flags_flags_part;
    int             flags_string_valid;
    int             flags;
    int             permission_mode;
    int             rwa_seen;
    const char      *cp;
    unsigned char   c;
    char            yuck_msg[50];
    libexplain_string_buffer_t yuck_buf;

    /*
     * Note: EFAULT has to be the pathname, because if flags was broken,
     * it would have raised a SEGFAULT signal from user space.
     */

    libexplain_string_buffer_printf(sb, "fopen(pathname = ");
    if (errnum == EFAULT)
        libexplain_string_buffer_printf(sb, "%p", pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_puts(sb, ", flags = ");
    libexplain_string_buffer_puts_quoted(sb, flags_string);
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_string_buffer_puts(sb, " failed, ");
    libexplain_buffer_strerror(sb, errnum);

    /*
     * Parse the flags string.
     *
     * (It turns out glibc is more generous than this, when it comes to
     * validity, but we only complain for EINVAL.  Different systems
     * will see validity differently.)
     */
    rwa_seen = 0;
    flags_mode_part = O_RDONLY;
    flags_flags_part = 0;
    flags_string_valid = 1;
    cp = flags_string;
    c = *cp++;
    switch (c)
    {
    case 'b':
        flags_flags_part |= O_BINARY;
        c = *cp++;
        break;

    case 't':
        flags_flags_part |= O_TEXT;
        c = *cp++;
        break;

    default:
        break;
    }
    switch (c)
    {
    case 'r':
        flags_mode_part = O_RDONLY;
        rwa_seen = 1;
        break;

    case 'w':
        flags_mode_part = O_WRONLY;
        flags_flags_part |= O_CREAT | O_TRUNC;
        rwa_seen = 1;
        break;

    case 'a':
        flags_mode_part = O_WRONLY;
        flags_flags_part |= O_CREAT | O_APPEND;
        rwa_seen = 1;
        break;

    case '\0':
        --cp;
        /* fall through... */

    default:
        flags_string_valid = 0;
        break;
    }
    libexplain_string_buffer_init(&yuck_buf, yuck_msg, sizeof(yuck_msg));
    for (;;)
    {
        c = *cp++;
        switch (c)
        {
        case '+':
            flags_mode_part = O_RDWR;
            continue;

        case 'b':
            flags_flags_part |= O_BINARY;
            continue;

        case 'c':
            /* no cancel */
            continue;

        case 'e':
            flags_flags_part |= O_CLOEXEC;
            continue;

        case 'm':
            /* mmap */
            continue;

        case 't':
            flags_flags_part |= O_TEXT;
            continue;

        case 'x':
            flags_flags_part |= O_EXCL;
            continue;

        default:
            flags_string_valid = 0;
            libexplain_string_buffer_putc(&yuck_buf, c);
            continue;

        case '\0':
            --cp;
            break;
        }
        break;
    }
    flags = flags_mode_part | flags_flags_part;
    permission_mode = 0666;

    switch (errnum)
    {
    case EINVAL:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the flags argument is not valid"
        );
        if (!rwa_seen)
        {
            libexplain_string_buffer_puts
            (
                sb,
                ", you must specify 'r', 'w' or 'a' at the start of the string"
            );
        }
        if (yuck_buf.position)
        {
            libexplain_string_buffer_printf
            (
                sb,
                ", flag character%s ",
                (yuck_buf.position == 1 ? "" : "s")
            );
            libexplain_string_buffer_puts_quoted(sb, yuck_msg);
            libexplain_string_buffer_puts(sb, " unknown");
        }
        return;

    case ENOMEM:
        {
            int             fd;

            libexplain_string_buffer_printf(sb, "fopen(pathname = ");
            libexplain_string_buffer_puts_quoted(sb, pathname);
            libexplain_string_buffer_puts(sb, ", flags = ");
            libexplain_string_buffer_puts_quoted(sb, flags_string);
            libexplain_string_buffer_puts(sb, ") failed, ");
            libexplain_buffer_strerror(sb, errnum);

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
                    return;
                }
                libexplain_buffer_enomem_kernel_or_user(sb);
                return;
            }
            close(fd);
            libexplain_buffer_enomem_user(sb);
        }
        return;

    default:
        /*
         * Punt everything else to open()
         */
        libexplain_buffer_errno_open_because
        (
            sb,
            errnum,
            pathname,
            flags,
            permission_mode
        );
        break;
    }
}
