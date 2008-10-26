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

#include <libexplain/ac/dirent.h>
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/string.h>

#include <sys/mtio.h>
#ifdef __linux__
#include <linux/hdreg.h>
#endif
#include <termios.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/eio.h>
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
dev_stat_recursive(libexplain_string_buffer_t *path, dev_t dev, struct stat *st)
{
    DIR             *dp;
    int             pos;

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
                if (dev_stat_recursive(path, dev, st) >= 0)
                {
                    closedir(dp);
                    return 0;
                }
                break;

            case S_IFBLK:
            case S_IFCHR:
                if (dev == st2.st_rdev)
                {
                    *st = st2;
                    closedir(dp);
                    return 0;
                }
                break;

            default:
                /* ignore everything else */
                break;
            }
        }
    }
    closedir(dp);
    return -1;
}


static int
dev_stat(dev_t dev, struct stat *st)
{
    libexplain_string_buffer_t sb;
    char            path[PATH_MAX + 1];

    libexplain_string_buffer_init(&sb, path, sizeof(path));
    libexplain_string_buffer_puts(&sb, "/dev");
    return dev_stat_recursive(&sb, dev, st);
}


static void
libexplain_buffer_eio_stat(libexplain_string_buffer_t *sb, int fildes,
    struct stat *st)
{
    if (S_ISREG(st->st_mode))
    {
        if (dev_stat(st->st_dev, st) < 0)
        {
            libexplain_buffer_eio(sb);
            return;
        }
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
                    "a low-level I/O error occurred in the disk device"
                );
                possibly_as_a_result_of_a_preceeding(sb, fildes);
                return;
            }
        }
#endif

        libexplain_string_buffer_puts
        (
            sb,
            "a low-level I/O error occurred in the block special device"
        );
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
                    "a low-level I/O error occurred in the tape "
                    "device"
                );
                possibly_as_a_result_of_a_preceeding(sb, fildes);
                return;
            }
        }

        /* see if it is a serial line */
        {
            struct termios t;
            if (ioctl(fildes, TCGETS, &t) >= 0)
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "a low-level I/O error occurred in the serial "
                    "device"
                );
                possibly_as_a_result_of_a_preceeding(sb, fildes);
                return;
            }
        }

        libexplain_string_buffer_puts
        (
            sb,
            "a low-level I/O error occurred in the character special device"
        );
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

    if (lstat(path, &st) < 0)
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
