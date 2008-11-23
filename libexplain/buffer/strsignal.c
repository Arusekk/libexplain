/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#include <libexplain/ac/signal.h>
#include <libexplain/ac/string.h>

#include <libexplain/buffer/strsignal.h>
#include <libexplain/option.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>


static const libexplain_parse_bits_table_t table[] =
{
#ifdef SIGABRT
    { "SIGABRT", SIGABRT },
#endif
#ifdef SIGALRM
    { "SIGALRM", SIGALRM },
#endif
#ifdef SIGBUS
    { "SIGBUS", SIGBUS },
#endif
#ifdef SIGCHLD
    { "SIGCHLD", SIGCHLD },
#endif
#ifdef SIGCLD
    { "SIGCLD", SIGCLD },
#endif
#ifdef SIGCONT
    { "SIGCONT", SIGCONT },
#endif
#ifdef SIGEMT
    { "SIGEMT", SIGEMT },
#endif
#ifdef SIGFPE
    { "SIGFPE", SIGFPE },
#endif
#ifdef SIGHUP
    { "SIGHUP", SIGHUP },
#endif
#ifdef SIGILL
    { "SIGILL", SIGILL },
#endif
#ifdef SIGINFO
    { "SIGINFO", SIGINFO },
#endif
#ifdef SIGINT
    { "SIGINT", SIGINT },
#endif
#ifdef SIGIO
    { "SIGIO", SIGIO },
#endif
#ifdef SIGIOT
    { "SIGIOT", SIGIOT },
#endif
#ifdef SIGKILL
    { "SIGKILL", SIGKILL },
#endif
#ifdef SIGLOST
    { "SIGLOST", SIGLOST },
#endif
#ifdef SIGPIPE
    { "SIGPIPE", SIGPIPE },
#endif
#ifdef SIGPOLL
    { "SIGPOLL", SIGPOLL },
#endif
#ifdef SIGPROF
    { "SIGPROF", SIGPROF },
#endif
#ifdef SIGPWR
    { "SIGPWR", SIGPWR },
#endif
#ifdef SIGQUIT
    { "SIGQUIT", SIGQUIT },
#endif
#ifdef SIGSEGV
    { "SIGSEGV", SIGSEGV },
#endif
#ifdef SIGSTKFLT
    { "SIGSTKFLT", SIGSTKFLT },
#endif
#ifdef SIGSTOP
    { "SIGSTOP", SIGSTOP },
#endif
#ifdef SIGSYS
    { "SIGSYS", SIGSYS },
#endif
#ifdef SIGTERM
    { "SIGTERM", SIGTERM },
#endif
#ifdef SIGTRAP
    { "SIGTRAP", SIGTRAP },
#endif
#ifdef SIGTSTP
    { "SIGTSTP", SIGTSTP },
#endif
#ifdef SIGTTIN
    { "SIGTTIN", SIGTTIN },
#endif
#ifdef SIGTTOU
    { "SIGTTOU", SIGTTOU },
#endif
#ifdef SIGUNUSED
    { "SIGUNUSED", SIGUNUSED },
#endif
#ifdef SIGURG
    { "SIGURG", SIGURG },
#endif
#ifdef SIGUSR1
    { "SIGUSR1", SIGUSR1 },
#endif
#ifdef SIGUSR2
    { "SIGUSR2", SIGUSR2 },
#endif
#ifdef SIGVTALRM
    { "SIGVTALRM", SIGVTALRM },
#endif
#ifdef SIGWINCH
    { "SIGWINCH", SIGWINCH },
#endif
#ifdef SIGXCPU
    { "SIGXCPU", SIGXCPU },
#endif
#ifdef SIGXFSZ
    { "SIGXFSZ", SIGXFSZ },
#endif

#ifdef __SIGRTMIN
    { "SIGRTMIN", __SIGRTMIN },
#endif
#ifdef __SIGRTMAX
    { "SIGRTMAX", __SIGRTMAX },
#endif
};


int
libexplain_signal_parse(const char *text)
{
    return libexplain_parse_bits(text, table, SIZEOF(table));
}



void
libexplain_buffer_strsignal(libexplain_string_buffer_t *sb, int signum)
{
    libexplain_string_buffer_puts(sb, strsignal(signum));
    if (libexplain_option_dialect_specific())
    {
        const libexplain_parse_bits_table_t *tp;

        libexplain_string_buffer_printf(sb, " (%d", signum);
        tp = libexplain_parse_bits_find_by_value(signum, table, SIZEOF(table));
        if (tp)
        {
            libexplain_string_buffer_puts(sb, ", ");
            libexplain_string_buffer_puts(sb, tp->name);
        }
#ifdef SIGRTMIN
        else if (SIGRTMIN <= signum && signum <= SIGRTMAX)
        {
            libexplain_string_buffer_printf
            (
                sb,
                ", SIGRTMIN + %d",
                signum - SIGRTMIN
            );
        }
#endif
        libexplain_string_buffer_putc(sb, ')');
    }
}
