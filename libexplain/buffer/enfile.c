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

#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/enfile.h>
#include <libexplain/string_buffer.h>

#ifdef __linux__
#include <sys/syscall.h>
#include <linux/sysctl.h>
#endif


static long
get_maxfile(void)
{
#ifdef __linux__
    struct __sysctl_args args;
    long maxfile;
    size_t maxfile_size = sizeof(maxfile);
    int name[] = { CTL_FS, FS_MAXFILE };

    /*
     * The Linux sysctl(2) man page has this to say:
     *
     *     Glibc does not provide a wrapper for this system call; call it
     *     using syscall(2).
     *
     *     Or rather... don't call it: use of this system call has long
     *     been discouraged, and it is so unloved that it is likely
     *     to disappear in a future kernel version.  Remove it from your
     *     programs now; use the /proc/sys interface instead.
     *
     *     The object names vary between kernel versions, making this system
     *     call worthless for applications.
     *
     * Catch 22: you have to open a file to discover the limit of
     * open files AFTER you have hit the limit of open files.  Sigh.
     */
    memset(&args, 0, sizeof(struct __sysctl_args));
    args.name = name;
    args.nlen = sizeof(name)/sizeof(name[0]);
    args.oldval = &maxfile;
    args.oldlenp = &maxfile_size;

    if (syscall(SYS__sysctl, &args) >= 0)
        return maxfile;
#endif
    return -1;
}

void
libexplain_buffer_enfile(libexplain_string_buffer_t *sb)
{
    long            maxfile;

    libexplain_buffer_because(sb);

    /*
     * FIXME: say the limit -- except that there doesn't seem to be
     * a sysconf() name for this.  The _SC_OPEN_MAX name is for the
     * EMFILE error, which is different.
     */
    libexplain_string_buffer_puts
    (
        sb,
        "the system limit on the total number of open files "
        "has been reached"
    );

    /*
     * Some systems provide a way to discover the actual limit.
     */
    maxfile = get_maxfile();
    if (maxfile > 0)
        libexplain_string_buffer_printf(sb, " (%ld)", maxfile);
}
