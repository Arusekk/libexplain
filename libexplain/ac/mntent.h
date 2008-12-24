/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#ifndef LIBEXPLAIN_AC_MNTENT_H
#define LIBEXPLAIN_AC_MNTENT_H

#include <libexplain/ac/stdio.h>

#if HAVE_MNTENT_H

#include <mntent.h>

#ifndef MOUNTED
# ifdef _PATH_MOUNTED
#  define MOUNTED _PATH_MOUNTED
# else
#  define MOUNTED "/etc/mtab"
# endif
#endif

#else /* !HAVE_MNTENT_H */

#ifndef MOUNTED
# ifdef _PATH_MOUNTED
#  define MOUNTED _PATH_MOUNTED
# else
#  define MOUNTED "/etc/mtab"
# endif
#endif

struct mntent
{
    char            *mnt_fsname;
    char            *mnt_dir;
    char            *mnt_type;
    char            *mnt_opts;
    int             mnt_freq;
    int             mnt_passno;
};


FILE *setmntent(const char *, const char *);
struct mntent *getmntent(FILE *);
int endmntent(FILE *);

#endif /* !HAVE_MNTENT_H */

#endif /* LIBEXPLAIN_AC_MNTENT_H */
