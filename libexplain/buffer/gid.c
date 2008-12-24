/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#include <libexplain/ac/grp.h>
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/gid.h>


void
libexplain_buffer_gid(libexplain_string_buffer_t *sb, int gid)
{
    libexplain_string_buffer_printf(sb, "%d", gid);
    if (gid >= 0)
    {
        struct group    *gr;

        gr = getgrgid(gid);
        if (gr)
        {
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_string_buffer_puts_quoted(sb, gr->gr_name);
        }
    }
}


void
libexplain_buffer_gid_supplementary(libexplain_string_buffer_t *sb)
{
    gid_t           groups[NGROUPS_MAX];
    int             n;
    int             j;

    n = getgroups(NGROUPS_MAX, groups);
    for (j = 0; j < n; ++j)
    {
        if (j)
            libexplain_string_buffer_putc(sb, ',');
        libexplain_string_buffer_putc(sb, ' ');
        libexplain_buffer_gid(sb, groups[j]);
    }
}
