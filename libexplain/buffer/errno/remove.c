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
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/errno/remove.h>
#include <libexplain/buffer/errno/rmdir.h>
#include <libexplain/buffer/errno/unlink.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/success.h>


static int
is_dir(const char *pathname)
{
    struct stat     st;

    return (stat(pathname, &st) >= 0 && S_ISDIR(st.st_mode));
}


void
libexplain_buffer_errno_remove(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname)
{
    libexplain_string_buffer_puts(sb, "remove(pathname = ");
    if (errnum == EFAULT)
        libexplain_string_buffer_printf(sb, "%p", pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_buffer_failed(sb, errnum);

    if (errno != EFAULT && is_dir(pathname))
        libexplain_buffer_errno_rmdir_because(sb, errnum, pathname);
    else
        libexplain_buffer_errno_unlink_because(sb, errnum, pathname);
}
