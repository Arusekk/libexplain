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
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/errno/lseek.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/lseek_whence.h>
#include <libexplain/buffer/success.h>


void
libexplain_buffer_errno_lseek(libexplain_string_buffer_t *sb, int errnum,
    int fildes, off_t offset, int whence)
{
    libexplain_string_buffer_printf(sb, "lseek(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_printf(sb, ", offset = %lld", (long long)offset);
    libexplain_string_buffer_puts(sb, ", whence = ");
    libexplain_buffer_lseek_whence(sb, whence);
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_buffer_failed(sb, errnum);

    switch (errnum)
    {
    case EBADF:
        libexplain_buffer_ebadf(sb, "fildes");
        break;

    case EINVAL:
        {
            long long       current_position;
            long long       destination;

            current_position = lseek(fildes, 0, SEEK_CUR);
            switch (whence)
            {
            default:
                libexplain_buffer_because(sb);
                libexplain_string_buffer_puts
                (
                    sb,
                    "'whence' is not one of SEEK_SET, SEEK_CUR, SEEK_END"
                );
                break;

            case SEEK_SET:
                current_position = 0;
                destination = offset;
                break;

            case SEEK_CUR:
                destination = current_position + offset;
                break;

            case SEEK_END:
                destination = lseek(fildes, 0, SEEK_END);
                if (destination < 0)
                {
                    current_position = -1;
                    destination = 0;
                }
                else
                {
                    current_position = 0;
                    destination += offset;
                }
                break;
            }

            if (current_position != (off_t)-1)
            {
                if (destination < 0)
                {
                    libexplain_buffer_because(sb);
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "the resulting file offset would be "
                        "negative"
                    );
                }
                else
                {
                    libexplain_buffer_because(sb);
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "the resulting offset would be beyond "
                        "the end of a seekable device"
                    );
                }
                libexplain_string_buffer_printf
                (
                    sb,
                    " (%lld)",
                    (long long)destination
                );
                break;
            }

            libexplain_buffer_because(sb);
            libexplain_string_buffer_puts
            (
                sb,
                "the resulting file offset would be negative, "
                "or beyond the end of a seekable device"
            );
        }
        break;

    case EOVERFLOW:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the resulting file offset cannot be represented in an off_t"
        );
        break;

    case ESPIPE:
        {
            struct stat     st;

            if (fstat(fildes, &st) == 0)
            {
                libexplain_buffer_because(sb);
                libexplain_string_buffer_puts
                (
                    sb,
                    "the file descriptor is associated with a "
                );
                libexplain_buffer_file_type(sb, st.st_mode);
            }
            else
            {
                libexplain_buffer_because(sb);
                libexplain_string_buffer_puts
                (
                    sb,
                    "the file descriptor is associated with a "
                    "pipe, socket, or FIFO"
                );
            }
        }
        break;

    case ENOSYS:
#if defined(EOPNOTSUPP) && EOPNOTSUPP != ENOSYS
    case EOPNOTSUPP:
#endif
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the device is incapable of seeking"
        );
        break;

    default:
        /* no explanation for other errno values */
        break;
    }
}
