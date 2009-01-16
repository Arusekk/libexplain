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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/mman.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/path_is_efault.h>


int
libexplain_path_is_efault(const char *path)
{
    struct stat     st;

    if (!path)
        return 1;
    return (lstat(path, &st) < 0 && errno == EFAULT);
}


int
libexplain_pointer_is_efault(const void *data, size_t data_size)
{
#ifdef HAVE_MINCORE
    static ssize_t  page_size;
    size_t          lo_pages;
    size_t          hi_pages;
    void            *lo;
    void            *hi;
    size_t          bsize;
    size_t          vec_bytes;
    unsigned char   dummy[256];

    if (!page_size)
    {
#ifdef HAVE_GETPAGESIZE
        page_size = getpagesize();
#else
        page_size = sysconf(_SC_PAGESIZE);
#endif
    }
    if (page_size <= 0)
    {
        plan_b:
        return libexplain_path_is_efault((const char *)data);
    }
    lo_pages = (size_t)data / page_size;
    hi_pages = ((size_t)data + data_size + page_size - 1) / page_size;
    lo = (void *)(lo_pages * page_size);
    hi = (void *)(hi_pages * page_size);
    bsize = (char *)hi - (char *)lo;
    vec_bytes = (hi_pages - lo_pages + 7) / 8;
    if (vec_bytes > sizeof(dummy))
    {
        unsigned char   *vec;

        /*
         * We can't hang on to the array and avoid the malloc next time,
         * because that wouldn't be thread safe.
         */
        vec = malloc(vec_bytes);
        if (!vec)
            goto plan_b;
        if (mincore(lo, bsize, vec) < 0)
        {
            free(vec);
            if (errno == ENOMEM)
                return 1;
            goto plan_b;
        }
        free(vec);
        return 0;
    }

    if (mincore(lo, bsize, dummy) < 0)
    {
        if (errno == ENOMEM)
            return 1;
        goto plan_b;
    }
    return 0;

#else
    return libexplain_path_is_efault((const char *)data);
#endif
}
