/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2010 Peter Miller
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

#include <libexplain/ac/sys/ptrace.h>

#include <libexplain/buffer/ptrace_request.h>
#include <libexplain/parse_bits.h>


static const explain_parse_bits_table_t table[] =
{
    { "PTRACE_TRACEME", PTRACE_TRACEME },
#ifdef PT_TRACEME
    { "PT_TRACEME", PT_TRACEME },
#endif

    { "PTRACE_PEEKTEXT", PTRACE_PEEKTEXT },
#ifdef PT_READ_I
    { "PT_READ_I", PT_READ_I },
#endif

    { "PTRACE_PEEKDATA", PTRACE_PEEKDATA },
#ifdef PT_READ_D
    { "PT_READ_D", PT_READ_D },
#endif

    { "PTRACE_PEEKUSER", PTRACE_PEEKUSER },
#ifdef PT_READ_U
    { "PT_READ_U", PT_READ_U },
#endif

    { "PTRACE_POKETEXT", PTRACE_POKETEXT },
#ifdef PT_WRITE_I
    { "PT_WRITE_I", PT_WRITE_I },
#endif

    { "PTRACE_POKEDATA", PTRACE_POKEDATA },
#ifdef PT_WRITE_D
    { "PT_WRITE_D", PT_WRITE_D },
#endif

    { "PTRACE_POKEUSER", PTRACE_POKEUSER },
#ifdef PT_WRITE_U
    { "PT_WRITE_U", PT_WRITE_U },
#endif

    { "PTRACE_CONT", PTRACE_CONT },
#ifdef PT_CONTINUE
    { "PT_CONTINUE", PT_CONTINUE },
#endif

    { "PTRACE_KILL", PTRACE_KILL },
#ifdef PT_KILL
    { "PT_KILL", PT_KILL },
#endif

    { "PTRACE_SINGLESTEP", PTRACE_SINGLESTEP },
#ifdef PT_STEP
    { "PT_STEP", PT_STEP },
#endif

    { "PTRACE_GETREGS", PTRACE_GETREGS },
#ifdef PT_GETREGS
    { "PT_GETREGS", PT_GETREGS },
#endif

    { "PTRACE_SETREGS", PTRACE_SETREGS },
#ifdef PT_SETREGS
    { "PT_SETREGS", PT_SETREGS },
#endif

    { "PTRACE_GETFPREGS", PTRACE_GETFPREGS },
#ifdef PT_GETFPREGS
    { "PT_GETFPREGS", PT_GETFPREGS },
#endif

    { "PTRACE_SETFPREGS", PTRACE_SETFPREGS },
#ifdef PT_SETFPREGS
    { "PT_SETFPREGS", PT_SETFPREGS },
#endif

    { "PTRACE_ATTACH", PTRACE_ATTACH },
#ifdef PT_ATTACH
    { "PT_ATTACH", PT_ATTACH },
#endif

    { "PTRACE_DETACH", PTRACE_DETACH },
#ifdef PT_DETACH
    { "PT_DETACH", PT_DETACH },
#endif

    { "PTRACE_GETFPXREGS", PTRACE_GETFPXREGS },
#ifdef PT_GETFPXREGS
    { "PT_GETFPXREGS", PT_GETFPXREGS },
#endif

    { "PTRACE_SETFPXREGS", PTRACE_SETFPXREGS },
#ifdef TP_SETFPXREGS
    { "TP_SETFPXREGS", TP_SETFPXREGS },
#endif

    { "PTRACE_SYSCALL", PTRACE_SYSCALL },
#ifdef PT_SYSCALL
    { "PT_SYSCALL", PT_SYSCALL },
#endif

    { "PTRACE_SETOPTIONS", PTRACE_SETOPTIONS },
#ifdef PT_SETOPTIONS
    { "PT_SETOPTIONS", PT_SETOPTIONS },
#endif

#ifdef PT_OLDSETOPTIONS
    { "PTRACE_OLDSETOPTIONS", PTRACE_OLDSETOPTIONS },
    { "PT_OLDSETOPTIONS", PT_OLDSETOPTIONS },
#endif

    { "PTRACE_GETEVENTMSG", PTRACE_GETEVENTMSG },
#ifdef PT_GETEVENTMSG
    { "PT_GETEVENTMSG", PT_GETEVENTMSG },
#endif

    { "PTRACE_GETSIGINFO", PTRACE_GETSIGINFO },
#ifdef PT_GETSIGINFO
    { "PT_GETSIGINFO", PT_GETSIGINFO },
#endif

    { "PTRACE_SETSIGINFO", PTRACE_SETSIGINFO },
#ifdef PT_SETSIGINFO
    { "PT_SETSIGINFO", PT_SETSIGINFO },
#endif

#ifdef PT_SYSEMU
    { "PTRACE_SYSEMU", PTRACE_SYSEMU },
    { "PT_SYSEMU", PT_SYSEMU },
#endif

#ifdef PT_SYSEMU_SINGLESTEP
    { "PTRACE_SYSEMU_SINGLESTEP", PTRACE_SYSEMU_SINGLESTEP },
    { "PT_SYSEMU_SINGLESTEP", PT_SYSEMU_SINGLESTEP },
#endif
};


void
explain_buffer_ptrace_request(explain_string_buffer_t *sb, int value)
{
    explain_parse_bits_print_single(sb, value, table, SIZEOF(table));
}


int
explain_parse_ptrace_request_or_die(const char *text, const char *caption)
{
    return explain_parse_bits_or_die(text, table, SIZEOF(table), caption);
}
