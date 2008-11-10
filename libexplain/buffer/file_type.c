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

#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/gettext.h>


void
libexplain_buffer_file_type(libexplain_string_buffer_t *sb, int mode)
{
    mode &= S_IFMT;
    switch (mode)
    {
    case S_IFSOCK:
        libexplain_buffer_gettext(sb, i18n("socket"));
        break;

    case S_IFLNK:
        libexplain_buffer_gettext(sb, i18n("symbolic link"));
        break;

    case S_IFREG:
        libexplain_buffer_gettext(sb, i18n("regular file"));
        break;

    case S_IFBLK:
        libexplain_buffer_gettext(sb, i18n("block special device"));
        break;

    case S_IFDIR:
        libexplain_buffer_gettext(sb, i18n("directory"));
        break;

    case S_IFCHR:
        libexplain_buffer_gettext(sb, i18n("character special device"));
        break;

    case S_IFIFO:
        libexplain_buffer_gettext(sb, i18n("named pipe"));
        break;

#ifdef S_IFMPC
    case S_IFMPC:
        libexplain_buffer_gettext
        (
            sb,
            i18n("multiplexed character special device")
        );
        break;
#endif

#ifdef S_IFNAM
    case S_IFNAM:
        libexplain_buffer_gettext(sb, i18n("named special file"));
        /*
         * with two subtypes, distinguished by st_rdev values 1, 2
         * 0001   S_INSEM    s    000001   XENIX semaphore subtype of IFNAM
         * 0002   S_INSHD    m    000002   XENIX shared data subtype of IFNAM
         */
        break;
#endif

#ifdef S_IFMPB
    case S_IFMPB:
        libexplain_buffer_gettext(sb, i18n("multiplexed block special device"));
        break;
#endif

#ifdef S_IFCMP
    case S_IFCMP:
        libexplain_buffer_gettext(sb, i18n("VxFS compressed file"));
        break;
#endif

#ifdef S_IFNWK
    case S_IFNWK:
        libexplain_buffer_gettext(sb, i18n("network special file"));
        break;
#endif

#ifdef S_IFDOOR
    case S_IFDOOR:
        libexplain_buffer_gettext(sb, i18n("Solaris door"));
        break;
#endif

#ifdef S_IFWHT
    case S_IFWHT:
        libexplain_buffer_gettext(sb, i18n("BSD whiteout"));
        break;
#endif

    default:
        libexplain_buffer_gettext(sb, i18n("unknown file type"));
        libexplain_string_buffer_printf(sb, " (%#o)", mode);
        break;
    }
}
