/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/stdint.h>
#include <libexplain/ac/signal.h>
#include <libexplain/ac/sys/ptrace.h>
#include <libexplain/ac/sys/user.h>

#include <libexplain/buffer/ebusy.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/eperm.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/ptrace.h>
#include <libexplain/buffer/esrch.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/long.h>
#include <libexplain/buffer/pid_t_star.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/ptrace_options.h>
#include <libexplain/buffer/ptrace_request.h>
#include <libexplain/buffer/signal.h>
#include <libexplain/explanation.h>
#include <libexplain/path_is_efault.h>


static void
explain_buffer_errno_ptrace_system_call(explain_string_buffer_t *sb, int errnum,
    int request, pid_t pid, void *addr, void *data)
{
    (void)errnum;
    explain_string_buffer_puts(sb, "ptrace(request = ");
    explain_buffer_ptrace_request(sb, request);
    explain_string_buffer_puts(sb, ", pid = ");
    explain_buffer_pid_t(sb, pid);
    explain_string_buffer_puts(sb, ", addr = ");
    explain_buffer_pointer(sb, addr);
    explain_string_buffer_puts(sb, ", data = ");
    switch (request)
    {
    case PTRACE_CONT:
    case PTRACE_SYSCALL:
    case PTRACE_SINGLESTEP:
#ifdef PT_SYSEMU
    case PTRACE_SYSEMU:
#endif
#ifdef PT_SYSEMU_SINGLESTEP
    case PTRACE_SYSEMU_SINGLESTEP:
#endif
    case PTRACE_DETACH:
        explain_buffer_signal(sb, (intptr_t)data);
        break;

    case PTRACE_SETOPTIONS:
#ifdef PT_OLDSETOPTIONS
    case PTRACE_OLDSETOPTIONS:
#endif
        explain_buffer_ptrace_options(sb, (intptr_t)data);
        break;

    case PTRACE_POKETEXT:
    case PTRACE_POKEDATA:
    case PTRACE_POKEUSER:
        explain_buffer_long(sb, (intptr_t)data);
        break;

    default:
        explain_buffer_pointer(sb, data);
        break;
    }
    explain_string_buffer_putc(sb, ')');
}


#include <libexplain/ac/stdio.h> /* HACK */

static int
request_is_known(int request)
{
    switch (request)
    {
#ifdef PT_TRACE_ME
    case PTRACE_TRACEME:
#endif
#ifdef PT_READ_I
    case PTRACE_PEEKTEXT:
#endif
#ifdef PT_READ_D
    case PTRACE_PEEKDATA:
#endif
#ifdef PT_READ_U
    case PTRACE_PEEKUSER:
#endif
#ifdef PT_WRITE_I
    case PTRACE_POKETEXT:
#endif
#ifdef PT_WRITE_D
    case PTRACE_POKEDATA:
#endif
#ifdef PT_WRITE_U
    case PTRACE_POKEUSER:
#endif
#ifdef PT_CONTINUE
    case PTRACE_CONT:
#endif
#ifdef PT_KILL
    case PTRACE_KILL:
#endif
#ifdef PT_SINGLESTEP
    case PTRACE_SINGLESTEP:
#endif
#ifdef PT_GETREGS
    case PTRACE_GETREGS:
#endif
#ifdef PT_SETREGS
    case PTRACE_SETREGS:
#endif
#ifdef PT_GETFPREGS
    case PTRACE_GETFPREGS:
#endif
#ifdef PT_SETFPREGS
    case PTRACE_SETFPREGS:
#endif
#ifdef PT_ATTACH
    case PTRACE_ATTACH:
#endif
#ifdef PT_DETACH
    case PTRACE_DETACH:
#endif
#ifdef PT_GETFPXREGS
    case PTRACE_GETFPXREGS:
#endif
#ifdef PT_SETFPXREGS
    case PTRACE_SETFPXREGS:
#endif
#ifdef PT_SYSCALL
    case PTRACE_SYSCALL:
#endif
#ifdef PT_SETOPTIONS
    case PTRACE_SETOPTIONS:
#endif
#ifdef PT_OLDSETOPTIONS
    case PTRACE_OLDSETOPTIONS:
#endif
#ifdef PT_GETEVENTMSG
    case PTRACE_GETEVENTMSG:
#endif
#ifdef PT_GETSIGINFO
    case PTRACE_GETSIGINFO:
#endif
#ifdef PT_SETSIGINFO
    case PTRACE_SETSIGINFO:
#endif
#ifdef PT_SYSEMU
    case PTRACE_SYSEMU:
#endif
#ifdef PT_SYSEMU_SINGLESTEP
    case PTRACE_SYSEMU_SINGLESTEP:
#endif
        return 1;

    default:
        break;
    }
    return 0;
}


static int
calculate_addr_size(int request)
{
    switch (request)
    {
#ifdef PT_READ_U
    case PTRACE_PEEKUSER:
#endif
#ifdef PT_WRITE_I
    case PTRACE_POKETEXT:
#endif
#ifdef PT_WRITE_D
    case PTRACE_POKEDATA:
#endif
#ifdef PT_WRITE_U
    case PTRACE_POKEUSER:
#endif
        return sizeof(int);

    default:
        break;
    }
    return 0;
}


static int
calculate_data_size(int request)
{
    switch (request)
    {
#ifdef PT_GETREGS
    case PTRACE_GETREGS:
        return sizeof(struct user_regs_struct);
#endif
#ifdef PT_SETREGS
    case PTRACE_SETREGS:
        return sizeof(struct user_regs_struct);
#endif
#ifdef PT_GETFPREGS
    case PTRACE_GETFPREGS:
        return sizeof(struct user_fpregs_struct);
#endif
#ifdef PT_SETFPREGS
    case PTRACE_SETFPREGS:
        return sizeof(struct user_fpregs_struct);
#endif

#if defined(linux) && (__WORDSIZE < 64)
#ifdef PT_GETFPXREGS
    case PTRACE_GETFPXREGS:
        return sizeof(struct user_fpxregs_struct);
#endif
#ifdef PT_SETFPXREGS
    case PTRACE_SETFPXREGS:
        return sizeof(struct user_fpxregs_struct);
#endif
#endif

#ifdef PT_GETEVENTMSG
    case PTRACE_GETEVENTMSG:
        return sizeof(unsigned long);
#endif
#ifdef PT_GETSIGINFO
    case PTRACE_GETSIGINFO:
        return sizeof(siginfo_t);
#endif
#ifdef PT_SETSIGINFO
    case PTRACE_SETSIGINFO:
        return sizeof(siginfo_t);
#endif

    default:
        break;
    }
    return 0;
}


void
explain_buffer_errno_ptrace_explanation(explain_string_buffer_t *sb, int errnum,
    const char *syscall_name, int request, pid_t pid, void *addr, void *data)
{
    size_t addr_size = calculate_addr_size(request);
    size_t data_size = calculate_data_size(request);

    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/ptrace.html
     */
    switch (errnum)
    {
    case EBUSY:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This error message is issued to explain an EBUSY error
             * reported by the ptrace(2) system call, in the case where there
             * was an error with allocating or freeing a debug register.
             */
            i18n("there was an error with allocating or freeing a debug "
                "register")
        );
        break;

    case EFAULT:
        if (addr_size && explain_pointer_is_efault(addr, addr_size))
        {
            explain_buffer_efault(sb, "addr");
            break;
        }
        if (data_size && explain_pointer_is_efault(data, data_size))
        {
            explain_buffer_efault(sb, "data");
            break;
        }
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This error message is issued to explain an EFAULT error
             * reported by the ptrace(2) system call, in the cases where there
             * was an attempt to read from or write to an invalid area in the
             * parent's or child's memory, probably because the area wasn't
             * mapped or accessible.
             */
            i18n("there was an attempt to read from or write to an invalid "
                "area in the parent's or child's memory, probably because the "
                "area wasn't mapped or accessible")
        );
        /*
         * Unfortunately, under Linux, different variations of this
         * fault will return EIO or EFAULT more or less arbitrarily.
         */
        break;

    case EINVAL:
        if (!request_is_known(request))
        {
            explain_buffer_einval_vague(sb, "request");
            return;
        }
        if
        (
            request == PTRACE_SETOPTIONS
#ifdef PT_OLDSETOPTIONS
        ||
            request == PTRACE_OLDSETOPTIONS
#endif
        )
        {
            explain_buffer_gettext
            (
                sb,
                /*
                 * xgettext: This error message is issued to explain an EINVAL
                 * error reported by the ptrace(2) system call, in the case
                 * where an attempt was made to set an invalid option.
                 */
                i18n("an attempt was made to set an invalid option")
            );
            break;
        }
        goto generic;

    case EIO:
        if (!request_is_known(request))
        {
            explain_buffer_einval_vague(sb, "request");
            return;
        }
        if (addr_size && explain_pointer_is_efault(addr, addr_size))
        {
            explain_buffer_efault(sb, "addr");
            break;
        }
        if (data_size && explain_pointer_is_efault(data, data_size))
        {
            explain_buffer_efault(sb, "data");
            break;
        }
        switch (request)
        {
        case PTRACE_CONT:
        case PTRACE_SYSCALL:
        case PTRACE_SINGLESTEP:
#ifdef PT_SYSEMU
        case PTRACE_SYSEMU:
#endif
#ifdef PT_SYSEMU_SINGLESTEP
        case PTRACE_SYSEMU_SINGLESTEP:
#endif
        case PTRACE_DETACH:
            explain_buffer_gettext
            (
                sb,
                /*
                 * xgettext: This error message is issued to explain an EIO
                 * error reported by the ptrace(2) system call, in the case
                 * where an invalid signal was specified during a restart
                 * request.
                 */
                i18n("an invalid signal was specified during a restart request")
            );
            return;

        default:
            break;
        }
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This error message is issued to explain an EIO error
             * reported by the ptrace(2) system call, in the case where there
             * was a word-alignment violation.
             */
            i18n("there was a word-alignment violation")
        );
        break;

    case EPERM:
        if (pid <= 0 || kill(pid, 0) < 0)
        {
            explain_buffer_eperm_kill(sb);
            break;
        }

        /*
         * Unprivileged processes cannot trace processes that are
         * running set-user-ID or set-group-ID programs.
         *
         * The process may already be being traced.
         * FIXME: can prctl tell us this?
         *
         * The process may be init(8), i.e. pid == 1
         */
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This error message is issued to explain an EPERM error
             * reported by the ptrace(2) system call, in the case where the
             * specified process cannot be traced.
             */
            i18n("the specified process cannot be traced")
        );
        break;

    case ESRCH:
        if (pid <= 0 || kill(pid, 0) < 0)
        {
            explain_buffer_eperm_kill(sb);
            break;
        }

        /* FIXME: figure out which case applies */
        /* FIXME: i18n */
        explain_string_buffer_puts
        (
            sb,
            "the specified process is not currently being traced by the "
            "caller, or is not stopped (for requests that require that)"
        );
        break;

    default:
        generic:
        explain_buffer_errno_generic(sb, errnum, syscall_name);
        break;
    }
}


void
explain_buffer_errno_ptrace(explain_string_buffer_t *sb, int errnum, int
    request, pid_t pid, void *addr, void *data)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_ptrace_system_call(&exp.system_call_sb, errnum,
        request, pid, addr, data);
    explain_buffer_errno_ptrace_explanation(&exp.explanation_sb, errnum,
        "ptrace", request, pid, addr, data);
    explain_explanation_assemble(&exp, sb);
}


/* vim: set ts=8 sw=4 et */
