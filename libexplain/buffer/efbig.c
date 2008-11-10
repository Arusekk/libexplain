/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#include <libexplain/ac/limits.h>
#include <libexplain/ac/sys/resource.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efbig.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/pretty_size.h>
#include <libexplain/option.h>


static unsigned long long
get_max_file_size(int file_size_bits)
{
    int             long_long_bits;
    struct rlimit   szlim;
    unsigned long long max_file_size;

    long_long_bits = sizeof(unsigned long long) * CHAR_BIT;
    if (file_size_bits > long_long_bits || file_size_bits <= 0)
        file_size_bits = long_long_bits;
    if (file_size_bits >= long_long_bits)
        max_file_size = ~(unsigned long long)0;
    else
        max_file_size = (unsigned long long)1 << file_size_bits;
    if (getrlimit(RLIMIT_FSIZE, &szlim) < 0 || szlim.rlim_cur == RLIM_INFINITY)
        return max_file_size;
    if (szlim.rlim_cur < max_file_size)
        max_file_size = szlim.rlim_cur;
    return max_file_size;
}


unsigned long long
libexplain_get_max_file_size_by_pathname(const char *pathname)
{
    return get_max_file_size(pathconf(pathname, _PC_FILESIZEBITS));
}


void
libexplain_buffer_efbig(libexplain_string_buffer_t *sb, const char *pathname,
    unsigned long long length, const char *length_caption)
{
    libexplain_buffer_because(sb);
    libexplain_string_buffer_puts(sb, length_caption);
    libexplain_string_buffer_putc(sb, ' ');
    libexplain_buffer_gettext(sb, i18n("is larger than the maximum file size"));

    if (libexplain_option_dialect_specific())
    {
        libexplain_string_buffer_puts(sb, " (");
        libexplain_buffer_pretty_size(sb, length);
        libexplain_string_buffer_puts(sb, " > ");
        libexplain_buffer_pretty_size
        (
            sb,
            libexplain_get_max_file_size_by_pathname(pathname)
        );
        libexplain_string_buffer_putc(sb, ')');
    }
}


unsigned long long
libexplain_get_max_file_size_by_fildes(int fildes)
{
    return get_max_file_size(fpathconf(fildes, _PC_FILESIZEBITS));
}


void
libexplain_buffer_efbig_fildes(libexplain_string_buffer_t *sb, int fildes,
    unsigned long long length, const char *length_caption)
{
    libexplain_buffer_because(sb);
    libexplain_string_buffer_puts(sb, length_caption);
    libexplain_string_buffer_putc(sb, ' ');
    libexplain_buffer_gettext(sb, i18n("is larger than the maximum file size"));

    if (libexplain_option_dialect_specific())
    {
        libexplain_string_buffer_puts(sb, " (");
        libexplain_buffer_pretty_size(sb, length);
        libexplain_string_buffer_puts(sb, " > ");
        libexplain_buffer_pretty_size
        (
            sb,
            libexplain_get_max_file_size_by_fildes(fildes)
        );
        libexplain_string_buffer_putc(sb, ')');
    }
}
