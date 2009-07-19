/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/getppcwd.h>
#include <libexplain/lsof.h>


#ifdef PROC_FS_USEFUL

char *
explain_getppcwd(char *data, size_t data_size)
{
    int             n;
    char            path[100];

    snprintf(path, sizeof(path), "/proc/%d/cwd", getppid());
    n = readlink(path, data, data_size - 1);
    if (n <= 0)
        return 0;
    data[n] = '\0';
    return data;
}

#else

typedef struct adapter adapter;
struct adapter
{
    explain_lsof_t  inherited;
    char            *data;
    size_t          data_size;
    int             count;
};


static void
n_callback(explain_lsof_t *context, const char *name)
{
    if (context->fildes == LIBEXPLAIN_LSOF_FD_cwd)
    {
        adapter         *a;

        a = (adapter *)context;
        strendcpy(a->data, name, a->data + a->data_size);
        a->count++;
    }
}


char *
explain_getppcwd(char *data, size_t data_size)
{
    int             ppid;
    adapter         obj;
    char            options[20];

    ppid = getppid();
    if (ppid <= 0)
        return 0;
    snprintf(options, sizeof(options), "-p%d", ppid);
    obj.inherited.n_callback = n_callback;
    obj.data = data;
    obj.data_size = data_size;
    obj.count = 0;
    explain_lsof(options, &obj.inherited);

    if (obj.count == 0)
        return 0;
    return data;
}

#endif
