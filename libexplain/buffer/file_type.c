/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/gettext.h>


void
explain_buffer_file_type(explain_string_buffer_t *sb, int mode)
{
    mode &= S_IFMT;
    switch (mode)
    {
#ifdef __linux__
    case 0:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2) for
             * more information) when that file is a Linux kernel special file.
             */
            i18n("kernel special file")
        );
        break;
#endif

    case S_IFSOCK:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is a network socket.
             */
            i18n("socket")
        );
        break;

    case S_IFLNK:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is a symbolic link.
             */
            i18n("symbolic link")
        );
        break;

    case S_IFREG:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is a regular file.
             */
            i18n("regular file")
        );
        break;

    case S_IFBLK:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is a block special
             * device.
             */
            i18n("block special device")
        );
        break;

    case S_IFDIR:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is a directory.
             */
            i18n("directory")
        );
        break;

    case S_IFCHR:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is a character
             * special device.
             */
            i18n("character special device")
        );
        break;

    case S_IFIFO:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is a fifo.
             */
            i18n("named pipe")
        );
        break;

#ifdef S_IFMPC
    case S_IFMPC:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is a multiplexed
             * character special device.
             * Not present on all POSIX implementations.
             */
            i18n("multiplexed character special device")
        );
        break;
#endif

#ifdef S_IFNAM
    case S_IFNAM:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is a named special
             * file.  Not present on all POSIX implementations.
             */
            i18n("named special file")
        );
        /*
         * with two subtypes, distinguished by st_rdev values 1, 2
         * 0001   S_INSEM    s    000001   XENIX semaphore subtype of IFNAM
         * 0002   S_INSHD    m    000002   XENIX shared data subtype of IFNAM
         */
        break;
#endif

#ifdef S_IFMPB
    case S_IFMPB:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see
             * stat(2) for more information) when that file is a
             * multiplexed block special device.  Not present on all
             * POSIX implementations.
             */
            i18n("multiplexed block special device")
        );
        break;
#endif

#ifdef S_IFCMP
    case S_IFCMP:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is a VxFS compressed
             * file.  Not present on all POSIX implementations.
             */
            i18n("VxFS compressed file")
        );
        break;
#endif

#ifdef S_IFNWK
    case S_IFNWK:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is a network special
             * file.  Not present on all POSIX implementations.
             */
            i18n("network special file")
        );
        break;
#endif

#ifdef S_IFDOOR
    case S_IFDOOR:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is a Solaris door.
             * Not present on all POSIX implementations.
             */
            i18n("Solaris door")
        );
        break;
#endif

#ifdef S_IFWHT
    case S_IFWHT:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is a BSD whiteout
             * file, used by the union file system.  Not present on all
             * POSIX implementations.
             */
            i18n("BSD whiteout")
        );
        break;
#endif

    default:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This string is the type of a file (see stat(2)
             * for more information) when that file is of an unknown
             * type, often the result of a bad inode block on a hard disk.
             */
            i18n("unknown file type")
        );
        explain_string_buffer_printf(sb, " (%#o)", mode);
        break;
    }
}