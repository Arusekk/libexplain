/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/ac/assert.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/lsof.h>
#include <libexplain/program_name.h>


static char progname[NAME_MAX + 1];


static void
explain_program_name_set_real(const char *name)
{
    const char      *cp;

    if (!name)
        name = "";
    progname[0] = '\0';
    if (!name)
        return;

    cp = name;
    for (;;)
    {
        char            *pnp;

        if (*cp == '/')
        {
            /*
             * Weird but true: some Unix implementations allow trailing
             * slahses on command names.  Ugh.  Not POSIX conforming, either.
             */
            ++cp;
            continue;
        }
        if (*cp == '\0')
            break;

        /*
         * GNU Libtool makes intermediate binaries with "lt-" prefixes,
         * ignore them when we see them.
         */
        if (0 == memcmp(cp, "lt-", 3))
            cp += 3;

        pnp = progname;
        for (;;)
        {
            if (pnp < progname + sizeof(progname) - 1)
                *pnp++ = *cp;
            ++cp;
            if (*cp == '\0' || *cp == '/')
                break;
        }
        *pnp = '\0';
    }
}


#ifndef PROC_FS_USEFUL

static void
n_callback(explain_lsof_t *context, const char *name)
{
    if (context->fildes == LIBEXPLAIN_LSOF_FD_txt)
    {
        explain_program_name_set_real(name);
    }
}

#endif


const char *
explain_program_name_get(void)
{
    if (progname[0])
        return progname;

#ifdef PROC_FS_USEFUL
    {
        int             n;
        char            buf[PATH_MAX + 1];

        n = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
        if (n > 0)
        {
            buf[n] = 0;
            explain_program_name_set_real(buf);
        }
    }
#else
    {
        explain_lsof_t obj;
        char            options[40];

        obj.n_callback = n_callback;
        snprintf(options, sizeof(options), "-p %d", getpid());
        explain_lsof(options, &obj);
    }
#endif
    if (progname[0])
        return progname;

    /*
     * bash(1) sets the "_" environment variable,
     * use that if available.
     */
    explain_program_name_set_real(getenv("_"));
    if (progname[0])
        return progname;

    return "";
}


void
explain_program_name_set(const char *name)
{
    explain_program_name_set_real(name);
    if (!progname[0])
        explain_program_name_assemble(0);
}
