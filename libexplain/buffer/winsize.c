/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/ac/sys/ioctl.h>
#include <libexplain/ac/termios.h>

#include <libexplain/buffer/winsize.h>


void
libexplain_buffer_winsize(libexplain_string_buffer_t *sb,
    const struct winsize *value)
{
    libexplain_string_buffer_printf
    (
        sb,
        "{ ws_row = %d, ws_col = %d, ws_xpixel = %d, ws_ypixel = %d }",
        value->ws_row,
        value->ws_col,
        value->ws_xpixel,
        value->ws_ypixel
    );
}
