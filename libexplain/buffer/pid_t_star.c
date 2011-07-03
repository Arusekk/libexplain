/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2011 Peter Miller
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/long.h>
#include <libexplain/buffer/pid_t_star.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/option.h>
#include <libexplain/is_efault.h>


#ifdef PROC_FS_USEFUL

static int
proc_exe(explain_string_buffer_t *sb, pid_t pid)
{
    int             n;
    const char      *slash;
    char            proc[100];
    char            exe[2000];

    /*
     * See if we can find the name of the executable of the give
     * process ID.  Don't worry about it if nothing comes of it.
     */
    snprintf(proc, sizeof(proc), "/proc/%ld/exe", (long)pid);
    n = readlink(proc, exe, sizeof(exe) - 1);
    if (n <= 0)
        return 0;
    exe[n] = '\0';
    slash = strrchr(exe, '/');
    if (slash)
        ++slash;
    else
        slash = exe;
    explain_string_buffer_putc(sb, ' ');
    explain_string_buffer_puts_quoted(sb, slash);
    return 1;
}


static void
proc_cmdline(explain_string_buffer_t *sb, pid_t pid)
{
    int             fd;
    int             n;
    char            *cp;
    char            proc[100];
    char            exe[2000];

    snprintf(proc, sizeof(proc), "/proc/%ld/cmdline", (long)pid);
    fd = open(proc, O_RDONLY);
    if (fd < 0)
        return;
    n = read(fd, exe, sizeof(exe) - 1);
    close(fd);
    if (n <= 0)
        return;
    exe[n] = '\0';

    /*
     * The data consists of each command line argument, and each is
     * terminated by a NUL character, not a space.  Since we only care
     * about the first one (argv[0], the command name) we just proceed
     * as if there was only the one C string present.
     */
    cp = strrchr(exe, '/');
    if (cp)
        ++cp;
    else
        cp = exe;
    explain_string_buffer_putc(sb, ' ');
    explain_string_buffer_puts_quoted(sb, cp);
}

#endif


void
explain_buffer_pid_t(explain_string_buffer_t *sb, pid_t pid)
{
    explain_buffer_long(sb, pid);

#ifdef PROC_FS_USEFUL
    if (explain_option_dialect_specific() && !proc_exe(sb, pid))
        proc_cmdline(sb, pid);
#endif
}


void
explain_buffer_pid_t_star(explain_string_buffer_t *sb, const pid_t *value)
{
    if (explain_is_efault_pointer(value, sizeof(*value)))
        explain_buffer_pointer(sb, value);
    else
    {
        explain_string_buffer_puts(sb, "{ ");
        explain_buffer_pid_t(sb, *value);
        explain_string_buffer_puts(sb, " }");
    }
}


void
explain_buffer_int_pid_star(explain_string_buffer_t *sb, const int *value)
{
    if (explain_is_efault_pointer(value, sizeof(*value)))
        explain_buffer_pointer(sb, value);
    else
    {
        explain_string_buffer_puts(sb, "{ ");
        explain_buffer_pid_t(sb, *value);
        explain_string_buffer_puts(sb, " }");
    }
}
