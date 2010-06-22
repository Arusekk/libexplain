/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2010 Peter Miller
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

#include <libexplain/ac/ctype.h>
#include <libexplain/ac/mntent.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/gcc_attributes.h>


#if !defined(HAVE_SETMNTENT) && \
    !defined(HAVE_GETMNTENT) && \
    !defined(HAVE_ENDMNTENT)

typedef struct bogus_t bogus_t;
struct bogus_t
{
    FILE            *fp;
    char            line[1000];
    struct          mntent data;
};


LINKAGE_HIDDEN
FILE *
setmntent(const char *filename, const char *mode)
{
    bogus_t         *bp;

    bp = malloc(sizeof(bogus_t));
    if (!bp)
        return 0;
    bp->fp = fopen(filename, mode);
    return (FILE *)bp;
}


LINKAGE_HIDDEN
struct mntent *
getmntent(FILE *p)
{
    bogus_t         *bp;

    if (!p)
        return 0;
    bp = (bogus_t *)p;
    if (!bp->fp)
        return 0;

    for (;;)
    {
        char            *lp;
        int             ac;
        char            *av[6];

        if (!fgets(bp->line, sizeof(bp->line), bp->fp))
            return 0;
        lp = bp->line;
        ac = 0;
        while (ac < 6)
        {
            if (!*lp)
                break;
            if (isspace((unsigned char)*lp))
            {
                ++lp;
                continue;
            }
            if (*lp == '#')
            {
                while (*lp)
                    ++lp;
                break;
            }
            av[ac++] = lp;
            while (*lp && !isspace((unsigned char)*lp))
                ++lp;
            if (!*lp)
                break;
            *lp++ = '\0';
        }
        if (ac < 2)
            continue;
        while (ac < 6)
            av[ac++] = lp;
        bp->data.mnt_fsname = av[0];
#ifdef __solaris__
        bp->data.mnt_dir = av[2];
        bp->data.mnt_type = av[3];
        bp->data.mnt_opts = av[4];
        bp->data.mnt_freq = 0;
        bp->data.mnt_passno = 0;
#else
        bp->data.mnt_dir = av[1];
        bp->data.mnt_type = av[2];
        bp->data.mnt_opts = av[3];
        bp->data.mnt_freq = atoi(av[4]);
        bp->data.mnt_passno = atoi(av[5]);
#endif
        return &bp->data;
    }
}


LINKAGE_HIDDEN
int
endmntent(FILE *p)
{
    if (p)
    {
        bogus_t         *bp;

        bp = (bogus_t *)p;
        if (bp->fp)
        {
            fclose(bp->fp);
            bp->fp = 0;
        }
        free(bp);
    }
    return 0;
}

#endif /* HAVE_MNTENT_H */

#ifndef HAVE_HASMNTOPT

#include <libexplain/ac/string.h>

LINKAGE_HIDDEN
char *
hasmntopt(const struct mntent *mnt, const char *opt)
{
    const char      *cp;
    size_t          opt_len;

    opt_len = strlen(opt);
    cp = mnt->mnt_opts;
    for (;;)
    {
        unsigned char   c;
        const char      *start;
        const char      *end;
        size_t          len;

        c = *cp;
        if (!cp)
            return 0;
        if (isspace(c) || c == ',')
        {
            ++cp;
            continue;
        }
        start = cp;
        for (;;)
        {
            ++cp;
            c = *cp;
            if (!c || isspace(c) || c == ',')
                break;
        }
        end = cp;
        len = end - start;
        if
        (
            opt_len < len
        &&
            start[opt_len] == '='
        &&
            0 == memcmp(opt, start, opt_len)
        )
            return (char *)start;
        if (opt_len == len && 0 == memcmp(opt, start, opt_len))
            return (char *)start;
    }
}

#endif
