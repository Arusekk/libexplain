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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/socket.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/sockaddr.h>
#include <libexplain/lsof.h>
#include <libexplain/string_buffer.h>


#ifndef PROC_FS_USEFUL


typedef struct adapter adapter;
struct adapter
{
    explain_lsof_t inherited;
    explain_string_buffer_t *sb;
};


static void
n_callback(explain_lsof_t *context, const char *name)
{
    adapter         *a;

    a = (adapter *)context;
    explain_string_buffer_putc(a->sb, ' ');
    explain_string_buffer_puts_quoted(a->sb, name);
}

#endif


static void
explain_buffer_fildes_to_path(explain_string_buffer_t *sb, int fildes)
{
#ifdef PROC_FS_USEFUL
    int             n;
    char            procpath[100];
    char            symlink_data[PATH_MAX + 1];

    snprintf(procpath, sizeof(procpath), "/proc/self/fd/%d", fildes);
    n = readlink(procpath, symlink_data, sizeof(symlink_data) - 1);
    if (n > 0)
    {
        symlink_data[n] = 0;
        explain_string_buffer_putc(sb, ' ');
        explain_string_buffer_puts_quoted(sb, symlink_data);
    }
#else
    adapter         obj;
    char            options[40];

    obj.inherited.n_callback = n_callback;
    obj.sb = sb;
    snprintf(options, sizeof(options), "-p %d -d %d", getpid(), fildes);
    explain_lsof(options, &obj.inherited);
#endif
}


static int
explain_buffer_fildes_to_sockaddr(explain_string_buffer_t *sb, int fildes)
{
    struct sockaddr_storage sa;
    socklen_t       sas;

    /*
     * Print the address of the local end of the connection.
     */
    sas = sizeof(sa);
    if (getsockname(fildes, (struct sockaddr *)&sa, &sas) < 0)
        return -1;
    explain_string_buffer_putc(sb, ' ');
    explain_buffer_sockaddr(sb, (struct sockaddr *)&sa, sas);

    /*
     * If available, also
     * print the address of the remote end of the connection.
     */
    sas = sizeof(sa);
    if (getpeername(fildes, (struct sockaddr *)&sa, &sas) < 0)
        return 0;
    explain_string_buffer_puts(sb, " => ");
    explain_buffer_sockaddr(sb, (struct sockaddr *)&sa, sas);
    return 0;
}


void
explain_buffer_fildes_to_pathname(explain_string_buffer_t *sb, int fildes)
{
    struct stat st;

    if (fstat(fildes, &st) < 0)
        return;
    switch (st.st_mode & S_IFMT)
    {
    case S_IFSOCK:
        if (explain_buffer_fildes_to_sockaddr(sb, fildes))
            goto oops;
        break;

    default:
        oops:
        explain_buffer_fildes_to_path(sb, fildes);
        break;
    }
}
