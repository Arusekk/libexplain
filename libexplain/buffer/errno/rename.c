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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/emlink.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/errno/rename.h>
#include <libexplain/buffer/exdev.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/path_to_pid.h>
#include <libexplain/buffer/sticky_permissions.h>
#include <libexplain/buffer/strerror.h>
#include <libexplain/buffer/success.h>
#include <libexplain/count_directory_entries.h>
#include <libexplain/dirname.h>
#include <libexplain/have_permission.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/string_buffer.h>


void
libexplain_buffer_errno_rename(libexplain_string_buffer_t *sb, int errnum,
    const char *oldpath, const char *newpath)
{
    int             oldpath_bad;
    int             newpath_bad;

    oldpath_bad = errnum == EFAULT && libexplain_path_is_efault(oldpath);
    newpath_bad = errnum == EFAULT && libexplain_path_is_efault(newpath);
    libexplain_string_buffer_printf(sb, "rename(oldpath = ");
    if (oldpath_bad)
        libexplain_string_buffer_printf(sb, "%p", oldpath);
    else
        libexplain_string_buffer_puts_quoted(sb, oldpath);
    libexplain_string_buffer_puts(sb, ", newpath = ");
    if (newpath_bad)
        libexplain_string_buffer_printf(sb, "%p", newpath);
    else
        libexplain_string_buffer_puts_quoted(sb, newpath);
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_string_buffer_puts(sb, " failed, ");
    libexplain_buffer_strerror(sb, errnum);

    switch (errnum)
    {
    case EACCES:
        libexplain_buffer_because(sb);

        /*
         * Check specific requirements for renaming directories.
         */
        {
            struct stat old_st;

            if
            (
                lstat(oldpath, &old_st) == 0
            &&
                S_ISDIR(old_st.st_mode)
            &&
                !libexplain_have_write_permission(&old_st)
            )
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "oldpath is a directory and does not allow write "
                    "permission (needed to update the .. entry)"
                );
                break;
            }
        }

        /*
         * Check that we have write permission in the directory above oldpath.
         */
        {
            char old_dotdot[PATH_MAX + 1];
            struct stat old_dotdot_st;

            libexplain_dirname(old_dotdot, oldpath, sizeof(old_dotdot));
            if
            (
                lstat(old_dotdot, &old_dotdot_st) == 0
            &&
                !libexplain_have_write_permission(&old_dotdot_st)
            )
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "write permission is denied for the "
                );
                libexplain_string_buffer_puts_quoted(sb, old_dotdot);
                libexplain_string_buffer_puts(sb, " directory");
                break;
            }
        }

        /*
         * Check that we have write permission in the directory above newpath.
         */
        {
            char new_dotdot[PATH_MAX + 1];
            struct stat new_dotdot_st;

            libexplain_dirname(new_dotdot, newpath, sizeof(new_dotdot));
            if
            (
                lstat(new_dotdot, &new_dotdot_st) == 0
            &&
                !libexplain_have_write_permission(&new_dotdot_st)
            )
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "write permission is denied for the "
                );
                libexplain_string_buffer_puts_quoted(sb, new_dotdot);
                libexplain_string_buffer_puts(sb, " directory");
                break;
            }
        }

        /*
         * Check the paths themselves
         */
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                oldpath,
                O_RDONLY,
                "oldpath"
            )
        &&
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                newpath,
                O_RDWR | O_CREAT,
                "newpath"
            )
        )
        {
            /*
             * Unable to find anything specific, give the generic
             * explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "write permission is denied for the directory "
                "containing oldpath or newpath; or, search "
                "permission is denied for one of the directories in "
                "the path prefix of oldpath or newpath"
            );
        }
        break;

    case EBUSY:
        libexplain_buffer_because(sb);
        if (libexplain_buffer_path_users(sb, oldpath, "oldpath") > 0)
            break;
        if (libexplain_buffer_path_users(sb, newpath, "newpath") > 0)
            break;
        libexplain_string_buffer_puts
        (
            sb,
            "oldpath or newpath is a directory that is in use "
            "by some process (perhaps as current working directory, "
            "or as root directory, or it was open for reading) or is "
            "in use by the system (for example as mount point)"
#if  0
            ", and "
            "the system considers this an error; note that there is "
            "no requirement to return EBUSY in such cases - there is "
            "nothing wrong with doing the rename anyway - but it is "
            "allowed to return EBUSY if the system cannot otherwise "
            "handle such situations"
#endif
        );
        break;

    case EFAULT:
        if (oldpath_bad)
            libexplain_buffer_efault(sb, "oldpath");
        else if (newpath_bad)
            libexplain_buffer_efault(sb, "newpath");
        else
            libexplain_buffer_efault(sb, "oldpath or newpath");
        break;

    case EINVAL:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the new pathname contained a path prefix of the "
            "old; or, more generally, an attempt was made to make a "
            "directory a subdirectory of itself"
        );
        break;

    case EISDIR:
        {
            struct stat     st;

            libexplain_buffer_because(sb);
            libexplain_string_buffer_puts
            (
                sb,
                "newpath is an existing directory, but oldpath is "
            );
            if (lstat(oldpath, &st) == 0)
            {
                libexplain_string_buffer_puts(sb, "a ");
                libexplain_buffer_file_type(sb, st.st_mode);
            }
            else
            {
                libexplain_string_buffer_puts(sb, "not a directory");
            }
        }
        break;

    case ELOOP:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                oldpath,
                O_RDONLY,
                "oldpath"
            )
        &&
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                newpath,
                O_RDWR + O_CREAT,
                "newpath"
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                " too many symbolic links were encountered in resolving "
                "oldpath or newpath"
            );
        }
        break;

    case EMLINK:
        libexplain_buffer_emlink(sb, oldpath, newpath);
        break;

    case ENAMETOOLONG:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                oldpath,
                O_RDONLY,
                "oldpath"
            )
        &&
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                newpath,
                O_RDWR | O_CREAT,
                "newpath"
            )
        )
        {
            /*
             * Unable to find the problem, issue the generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "oldpath or newpath, or a path component of oldpath "
                "or newpath, was too long"
            );
        }
        break;

    case ENOENT:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                oldpath,
                O_RDONLY,
                "oldpath"
            )
        &&
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                newpath,
                O_RDWR | O_CREAT,
                "newpath"
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "a directory component in oldpath or newpath does "
                "not exist or is a dangling symbolic link"
            );
        }
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    case ENOSPC:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the file system containing the file"
        );
        libexplain_buffer_mount_point(sb, oldpath);
        libexplain_string_buffer_puts
        (
            sb,
            " has no room for the new directory entry"
        );
        break;

    case ENOTDIR:
        libexplain_buffer_because(sb);
        {
            struct stat     oldpath_st;

            if (stat(oldpath, &oldpath_st) >= 0 && S_ISDIR(oldpath_st.st_mode))
            {
                struct stat     newpath_st;

                if
                (
                    stat(newpath, &newpath_st) >= 0
                &&
                    !S_ISDIR(newpath_st.st_mode)
                )
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "oldpath is a directory, but "
                    );
                    libexplain_buffer_file_type(sb, newpath_st.st_mode);
                    libexplain_string_buffer_puts
                    (
                        sb,
                        " newpath is not a directory"
                    );
                    break;
                }
            }
        }

        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                oldpath,
                O_RDONLY,
                "oldpath"
            )
        &&
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                newpath,
                O_RDONLY | O_CREAT,
                "newpath"
            )
        )
        {
            /*
             * Unable to pin point a specific cause, so go with the
             * generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "a component used as a directory in oldpath or newpath "
                "is not, in fact, a directory; or, oldpath is a "
                "directory, and newpath exists but is not a directory"
            );
        }
        break;

    case ENOTEMPTY:
    case EEXIST:
        {
            int             count;

            libexplain_buffer_because(sb);
            libexplain_string_buffer_puts
            (
                sb,
                "newpath is not an empty directory; that is, it "
                "contains entries other than \".\" and \"..\""
            );

            count = libexplain_count_directory_entries(newpath);
            if (count > 0)
                libexplain_string_buffer_printf(sb, " (%d)", count);
        }
        break;

    case EPERM:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_sticky_permissions(sb, oldpath, "oldpath")
        &&
            libexplain_buffer_sticky_permissions(sb, newpath, "newpath")
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "the directory containing oldpath has the sticky bit "
                "(S_ISVTX) set and the process's effective user ID is "
                "neither the user ID of the file to be deleted nor that "
                "of the directory containing it, and the process is "
                "not privileged"
#ifdef __linux__
                " (does not have the CAP_FOWNER capability)"
#endif
                "; or newpath is an existing file and the directory "
                "containing it has the sticky bit set and the "
                "process's effective user ID is neither the user ID "
                "of the file to be replaced nor that of the directory "
                "containing it, and the process is not privileged"
#ifdef __linux__
                " (does not have the CAP_FOWNER capability)"
#endif
                "; or the file system containing pathname does not "
                "support renaming of the type requested"
            );
        }
        break;

    case EROFS:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the file is on a read-only file system"
        );
        if (libexplain_buffer_mount_point(sb, newpath) < 0)
            libexplain_buffer_mount_point_dirname(sb, newpath);
        break;

    case EXDEV:
        libexplain_buffer_exdev(sb, oldpath, newpath, "rename");
        break;

    default:
        /* no explanation for other errno values */
        break;
    }

    /*
     * Let the user know where things stand after the failure.
     */
    {
        struct stat st;
        if (lstat(oldpath, &st) == 0)
        {
            libexplain_string_buffer_puts
            (
                sb,
                "; note that oldpath still exists"
            );
        }
        if (lstat(newpath, &st) == 0)
        {
            libexplain_string_buffer_puts
            (
                sb,
                "; note that newpath exists"
            );
        }
    }
}
