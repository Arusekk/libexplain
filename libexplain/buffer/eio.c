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
#include <libexplain/ac/dirent.h>
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <sys/mtio.h>
#ifdef __linux__
#include <linux/hdreg.h>
#endif
#include <libexplain/ac/termios.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/dirname.h>


static void
possibly_as_a_result_of_a_preceeding(libexplain_string_buffer_t *sb, int fildes)
{
    int             flags;

    flags = fcntl(fildes, F_GETFL);
    if (flags < 0)
        flags = O_RDWR;
    libexplain_string_buffer_puts
    (
        sb,
        ", possibly as a result of a preceeding "
    );
    switch (flags & O_ACCMODE)
    {
    case O_RDONLY:
        libexplain_string_buffer_puts(sb, "read");
        break;

    case O_WRONLY:
        libexplain_string_buffer_puts(sb, "write");
        break;

    default:
        libexplain_string_buffer_puts(sb, "read or write");
        break;
    }
}


void
libexplain_buffer_eio(libexplain_string_buffer_t *sb)
{
    libexplain_buffer_because(sb);
    libexplain_string_buffer_puts
    (
        sb,
        "a low-level I/O error occurred, probably in hardware"
    );
    possibly_as_a_result_of_a_preceeding(sb, -1);
}


static int
dev_stat_recursive(libexplain_string_buffer_t *path, dev_t dev, struct stat *st,
    libexplain_string_buffer_t *dev_buf)
{
    DIR             *dp;
    int             pos;
    int             result;

    result = -1;
    dp = opendir(path->message);
    if (!dp)
        return -1;
    pos = path->position;
    for (;;)
    {
        struct dirent   *dep;
        struct stat     st2;

        dep = readdir(dp);
        if (!dep)
            break;
        if (0 == strcmp(dep->d_name, "."))
            continue;
        if (0 == strcmp(dep->d_name, ".."))
            continue;
        path->position = pos;
        path->message[pos] = '\0';
        libexplain_string_buffer_path_join(path, dep->d_name);
        if (lstat(path->message, &st2) >= 0)
        {
            switch (st2.st_mode & S_IFMT)
            {
            case S_IFDIR:
                if (dev_stat_recursive(path, dev, st, dev_buf) >= 0)
                    result = 0;
                break;

            case S_IFBLK:
            case S_IFCHR:
                if (dev == st2.st_rdev)
                {
                    if
                    (
                        !dev_buf->position
                    ||
                        path->position < dev_buf->position
                    )
                    {
                        dev_buf->position = 0;
                        *st = st2;
                        libexplain_string_buffer_puts(dev_buf, path->message);
                    }
                    result = 0;
                }
                break;

            default:
                /* ignore everything else */
                break;
            }
        }
    }
    closedir(dp);
    return result;
}


static int
dev_stat(dev_t dev, struct stat *st, libexplain_string_buffer_t *dev_buf)
{
    libexplain_string_buffer_t sb;
    char            path[PATH_MAX + 1];

    libexplain_string_buffer_init(&sb, path, sizeof(path));
    libexplain_string_buffer_puts(&sb, "/dev");
    return dev_stat_recursive(&sb, dev, st, dev_buf);
}


static void
libexplain_buffer_eio_stat(libexplain_string_buffer_t *sb, int fildes,
    struct stat *st)
{
    char            dev_path[150];
    libexplain_string_buffer_t dev_buf;

    libexplain_string_buffer_init(&dev_buf, dev_path, sizeof(dev_path));
    assert(dev_path[0] == '\0');
    switch (st->st_mode & S_IFMT)
    {
    case S_IFDIR:
    case S_IFREG:
        if (dev_stat(st->st_dev, st, &dev_buf) < 0)
        {
            libexplain_buffer_eio(sb);
            return;
        }
        break;

    default:
        dev_stat(st->st_rdev, st, &dev_buf);
        /* no problem if it failed, use the st we were given */
        break;
    }

    libexplain_buffer_because(sb);
    if (S_ISBLK(st->st_mode))
    {
#if 0
        /*
         * See if it is a disk drive, or something that looks like a
         * disk drive.
         */
        {
            struct hd_geometry x;

            if (ioctl(fildes, HDIO_GETGEO, &x) >= 0)
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "a low-level I/O error occurred in the "
                );
                if (dev_buf.position)
                if (dev_path[0])
                {
                    libexplain_string_buffer_puts_quoted(sb, dev_path);
                    libexplain_string_buffer_putc(sb, ' ');
                }
                libexplain_string_buffer_puts(sb, "disk");
                possibly_as_a_result_of_a_preceeding(sb, fildes);
                return;
            }
        }
#endif

        libexplain_string_buffer_puts
        (
            sb,
            "a low-level I/O error occurred in the "
        );
        if (dev_path[0])
        {
            libexplain_string_buffer_puts_quoted(sb, dev_path);
            libexplain_string_buffer_putc(sb, ' ');
        }
        libexplain_buffer_file_type(sb, st->st_mode);
        possibly_as_a_result_of_a_preceeding(sb, fildes);
        return;
    }
    if (S_ISCHR(st->st_mode))
    {
        /* see if it is a tape */
        {
            struct mtget m;
            if (ioctl(fildes, MTIOCGET, &m) >= 0)
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "a low-level I/O error occurred in the "
                );
                if (dev_path[0])
                {
                    libexplain_string_buffer_puts_quoted(sb, dev_path);
                    libexplain_string_buffer_putc(sb, ' ');
                }
                libexplain_string_buffer_puts
                (
                    sb,
                    "tape device"
                );
                possibly_as_a_result_of_a_preceeding(sb, fildes);
                return;
            }
        }

        /* see if it is a serial line */
        if (isatty(fildes))
        {
            libexplain_string_buffer_puts
            (
                sb,
                "a low-level I/O error occurred in the "
            );
            if (dev_path[0])
            {
                libexplain_string_buffer_puts_quoted(sb, dev_path);
                libexplain_string_buffer_putc(sb, ' ');
            }
            libexplain_string_buffer_puts
            (
                sb,
                "serial device"
            );
            possibly_as_a_result_of_a_preceeding(sb, fildes);
            return;
        }

        libexplain_string_buffer_puts
        (
            sb,
            "a low-level I/O error occurred in the "
        );
        if (dev_path[0])
        {
            libexplain_string_buffer_puts_quoted(sb, dev_path);
            libexplain_string_buffer_putc(sb, ' ');
        }
        libexplain_buffer_file_type(sb, st->st_mode);
        possibly_as_a_result_of_a_preceeding(sb, fildes);
        return;
    }

    libexplain_string_buffer_puts
    (
        sb,
        "a low-level I/O error occurred, probably in hardware"
    );
    possibly_as_a_result_of_a_preceeding(sb, fildes);
}


void
libexplain_buffer_eio_fildes(libexplain_string_buffer_t *sb, int fildes)
{
    struct stat     st;

    if (fstat(fildes, &st) < 0)
    {
        libexplain_buffer_eio(sb);
        return;
    }
    return libexplain_buffer_eio_stat(sb, fildes, &st);
}


void
libexplain_buffer_eio_path(libexplain_string_buffer_t *sb, const char *path)
{
    struct stat     st;

    if (stat(path, &st) < 0 && lstat(path, &st) < 0)
    {
        libexplain_buffer_eio(sb);
        return;
    }
    return libexplain_buffer_eio_stat(sb, -1, &st);
}


void
libexplain_buffer_eio_path_dirname(libexplain_string_buffer_t *sb,
    const char *path)
{
    char            path_dir[PATH_MAX + 1];

    libexplain_dirname(path_dir, path, sizeof(path_dir));
    libexplain_buffer_eio_path(sb, path_dir);
}
