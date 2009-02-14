/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
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
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/execve.h>
#include <libexplain/option.h>
#include <libexplain/wrap_and_print.h>


void
libexplain_execve_or_die(const char *pathname, char *const *argv,
    char *const *envp)
{
    /*
     * The casts are because different systems declare execve
     * differently, but all will accept const-less arguments.
     */
    execve(pathname, argv, envp);

    /*
     * If it returned, it failed.
     * There is no need to rest the result.
     */
    libexplain_program_name_assemble_internal(1);
    libexplain_wrap_and_print(stderr, libexplain_execve(pathname, argv, envp));
    exit(EXIT_FAILURE);
}
