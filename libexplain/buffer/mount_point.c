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

#include <libexplain/ac/mntent.h>
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/sys/vfs.h>

#include <libexplain/buffer/mount_point.h>
#include <libexplain/dirname.h>


int
explain_buffer_mount_point_stat(explain_string_buffer_t *sb,
    const struct stat *st1)
{
    FILE            *fp;

    fp = setmntent(MOUNTED, "r");
    if (!fp)
        return -1;
    for (;;)
    {
        struct mntent   *mnt;
        struct stat     st2;

        /* FIXME: use getmntent_r if available */
        mnt = getmntent(fp);
        if (!mnt)
            break;
        if (lstat(mnt->mnt_dir, &st2) == 0)
        {
            if (st1->st_dev == st2.st_dev)
            {
                struct statfs   f;

                /*
                 * Insert the name of the mount point.
                 */
                explain_string_buffer_puts(sb, " (");
                explain_string_buffer_puts_quoted(sb, mnt->mnt_dir);

                /*
                 * If possible, insert the percentage used of the file
                 * system, similar to how df(1) has a %used column.
                 */
                if (statfs(mnt->mnt_dir, &f) == 0)
                {
                    long            reserved;
                    long            blocks;
                    long            used;

                    reserved = f.f_bfree - f.f_bavail;
                    if (reserved < 0)
                        reserved = 0;
                    blocks = f.f_blocks - reserved;
                    used = blocks - f.f_bavail;
                    if (blocks > 0 && used >= 0 && used <= (long)f.f_blocks)
                    {
                        explain_string_buffer_printf
                        (
                            sb,
                            ", %d%% full",
                            (int)(0.5 + (100. * used / blocks))
                        );
                    }
                }
                explain_string_buffer_putc(sb, ')');

                endmntent(fp);
                return 0;
            }
        }
    }
    endmntent(fp);
    return -1;
}


int
explain_buffer_mount_point_fd(explain_string_buffer_t *sb, int fildes)
{
    struct stat     st;

    if (fstat(fildes, &st) < 0)
        return -1;
    return explain_buffer_mount_point_stat(sb, &st);
}


int
explain_buffer_mount_point(explain_string_buffer_t *sb, const char *path)
{
    struct stat     st;

    if (lstat(path, &st) < 0)
        return -1;
    return explain_buffer_mount_point_stat(sb, &st);
}


int
explain_buffer_mount_point_dirname(explain_string_buffer_t *sb,
    const char *path)
{
    char            dir[PATH_MAX + 1];

    explain_dirname(dir, path, sizeof(dir));
    return explain_buffer_mount_point(sb, dir);
}


static int
explain_mount_point_option(const char *pathname, const char *option)
{
    FILE            *fp;
    struct stat     st1;

    if (lstat(pathname, &st1) < 0)
        return 0;
    fp = setmntent(MOUNTED, "r");
    if (!fp)
        return 0;
    for (;;)
    {
        struct mntent   *mnt;
        struct stat     st2;

        /* FIXME: use getmntent_r if available */
        mnt = getmntent(fp);
        if (!mnt)
            break;
        if (lstat(mnt->mnt_dir, &st2) == 0)
        {
            if (st1.st_dev == st2.st_dev)
            {
                int             result;

                /*
                 * Found the mount point.
                 *
                 * (For linux this is "probably found the right mount
                 * point", because linux allows the same block special
                 * device to be mounted at more than one place, and they
                 * could have different options for different mount
                 * points.)
                 */
                result = !!hasmntopt(mnt, option);
                endmntent(fp);
                return result;
            }
        }
    }
    endmntent(fp);
    return 0;
}


int
explain_mount_point_noexec(const char *pathname)
{
    return explain_mount_point_option(pathname, "noexec");
}


int
explain_mount_point_nosuid(const char *pathname)
{
    return explain_mount_point_option(pathname, "nosuid");
}
