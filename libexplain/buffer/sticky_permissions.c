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

#include <libexplain/ac/assert.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/sticky_permissions.h>
#include <libexplain/buffer/uid.h>
#include <libexplain/dirname.h>
#include <libexplain/have_permission.h>


int
libexplain_buffer_sticky_permissions(libexplain_string_buffer_t *sb,
    const char *pathname, const char *caption)
{
    uid_t           uid;
    struct stat     pathname_st;
    struct stat     dir_st;
    char            dir[PATH_MAX + 1];

    assert(0 == strcmp(caption, "pathname") || 0 == strcmp(caption, "oldpath")
        || 0 == strcmp(caption, "newpath"));

    if (lstat(pathname, &pathname_st) < 0)
        return -1;
    libexplain_dirname(dir, pathname, sizeof(dir));
    if (lstat(dir, &dir_st) < 0)
        return -1;

    uid = geteuid();
    if ((dir_st.st_mode & S_ISVTX) == 0)
    {
        /*
         * No sticky bit set, therefore only need write permissions on
         * the parent directory.
         */
        if (!libexplain_have_write_permission(&dir_st))
        {
            char            filename[PATH_MAX + 1];

            libexplain_string_buffer_puts
            (
                sb,
                "the process does not have write permission to the"
            );
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_string_buffer_puts(sb, caption);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_string_buffer_puts_quoted(sb, dir);
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_buffer_gettext(sb, i18n("directory"));

            libexplain_basename(filename, pathname, sizeof(filename));
            libexplain_string_buffer_puts
            (
                sb,
                ", this is needed to remove the "
            );
            libexplain_string_buffer_puts_quoted(sb, filename);
            libexplain_string_buffer_puts(sb, " directory entry");
            return 0;
        }
        return -1;
    }
    if (uid == pathname_st.st_uid || uid == dir_st.st_uid)
        return -1;
    libexplain_string_buffer_puts(sb, "the directory containing ");
    libexplain_string_buffer_puts(sb, caption);
    libexplain_string_buffer_puts
    (
        sb,
        " has the sticky bit (S_ISVTX) set and the process's effective "
        "user ID ("
    );
    libexplain_buffer_uid(sb, uid);
    libexplain_string_buffer_puts
    (
        sb,
        ") is neither the user ID of the file to be deleted ("
    );
    libexplain_buffer_uid(sb, pathname_st.st_uid);
    libexplain_string_buffer_puts
    (
        sb,
        ") nor that of the directory containing it ("
    );
    libexplain_buffer_uid(sb, dir_st.st_uid);
    libexplain_string_buffer_puts
    (
        sb,
        "), and the process is not privileged"
#ifdef __linux__
        " (does not have the CAP_FOWNER capability)"
#endif
    );
    return 0;
}
