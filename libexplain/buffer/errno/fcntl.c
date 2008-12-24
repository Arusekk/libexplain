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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/check_fildes_range.h>
#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/errno/fcntl.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/flock.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/strsignal.h>
#include <libexplain/explanation.h>
#include <libexplain/fcntl.h>
#include <libexplain/open_flags.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>


static const libexplain_parse_bits_table_t table[] =
{
    { "F_DUPFD",         F_DUPFD,         },
#ifdef F_DUPFD_CLOEXEC
    { "F_DUPFD_CLOEXEC", F_DUPFD_CLOEXEC, },
#endif
    { "F_GETFD",         F_GETFD,         },
    { "F_SETFD",         F_SETFD,         },
    { "F_GETFL",         F_GETFL,         },
    { "F_SETFL",         F_SETFL,         },
    { "F_GETLK",         F_GETLK,         },
    { "F_SETLK",         F_SETLK,         },
    { "F_SETLKW",        F_SETLKW,        },
    { "F_SETOWN",        F_SETOWN,        },
    { "F_GETOWN",        F_GETOWN,        },
#ifdef F_SETSIG
    { "F_SETSIG",        F_SETSIG,        },
#endif
#ifdef F_GETSIG
    { "F_GETSIG",        F_GETSIG,        },
#endif
#ifdef F_INPROGRESS
    { "F_INPROGRESS",    F_INPROGRESS,    },
#endif
#ifdef F_GETLEASE
    { "F_GETLEASE",      F_GETLEASE,      },
#endif
#ifdef F_SETLEASE
    { "F_SETLEASE",      F_SETLEASE,      },
#endif
#ifdef F_GETLK64
#if F_GETLK64 != F_GETLK
    { "F_GETLK64",       F_GETLK64,       },
    { "F_SETLK64",       F_SETLK64,       },
    { "F_SETLKW64",      F_SETLKW64,      },
#endif
#endif
#ifdef F_NOTIFY
    { "F_NOTIFY",        F_NOTIFY,        },
#endif
};


int
libexplain_fcntl_command_parse_or_die(const char *text, const char *caption)
{
    return libexplain_parse_bits_or_die(text, table, sizeof(table), caption);
}


static void
libexplain_buffer_errno_fcntl_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes, int command, long arg)
{
    const libexplain_parse_bits_table_t *tp;

    libexplain_string_buffer_printf(sb, "fcntl(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_puts(sb, ", command = ");
    tp = libexplain_parse_bits_find_by_value(command, table, SIZEOF(table));
    if (tp)
        libexplain_string_buffer_puts(sb, tp->name);
    else
        libexplain_string_buffer_printf(sb, "%d", command);
    switch (command)
    {
    default:
        break;

    case F_DUPFD:
#ifdef F_DUPFD_CLOEXEC
    case F_DUPFD_CLOEXEC:
#endif
    case F_SETFD:
    case F_SETOWN:
        libexplain_string_buffer_printf(sb, ", arg = %ld", arg);
        break;

#ifdef F_SETSIG
    case F_SETSIG:
        libexplain_string_buffer_puts(sb, ", arg = ");
        libexplain_buffer_strsignal(sb, arg);
        break;
#endif

#ifdef F_SETLEASE
    case F_SETLEASE:
        /* FIXME: decode lease flag */
        libexplain_string_buffer_printf(sb, ", arg = %ld", arg);
        break;
#endif

#ifdef F_NOTIFY
    case F_NOTIFY:
        /* FIXME: decode notify bits */
        libexplain_string_buffer_printf(sb, ", arg = %ld", arg);
        break;
#endif

    case F_SETFL:
        libexplain_string_buffer_puts(sb, ", arg = ");
        libexplain_buffer_open_flags(sb, arg);
        break;

    case F_GETLK:
    case F_SETLK:
    case F_SETLKW:
        {
            const struct flock *p;

            p = (const struct flock *)arg;
            libexplain_string_buffer_puts(sb, ", arg = ");
            if (errnum == EFAULT)
                libexplain_buffer_pointer(sb, p);
            else
                libexplain_buffer_flock(sb, p);
        }
        break;

#ifdef F_GETLK64
#if F_GETLK64 != F_GETLK
    case F_GETLK64:
    case F_SETLK64:
    case F_SETLKW64:
        {
            const struct flock64 *p;

            p = (const struct flock64 *)arg;
            libexplain_string_buffer_puts(sb, ", arg = ");
            if (errnum == EFAULT)
                libexplain_buffer_pointer(sb, p);
            else
                libexplain_buffer_flock64(sb, p);
        }
        break;
#endif
#endif
    }
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_fcntl_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes, int command, long arg)
{
    switch (errnum)
    {
    case EACCES:
        /* FIXME: what other processes? */
        libexplain_string_buffer_puts
        (
            sb,
            "the operation is prohibited by locks held by other processes"
        );
        break;

    case EAGAIN:
        /* FIXME: what other processes? */
        libexplain_string_buffer_puts
        (
            sb,
            "the operation is prohibited by locks held by other "
            "processes; or, the operation is prohibited because the "
            "file has been memory-mapped by another process"
        );
        break;

    case EBADF:
        if (fcntl(fildes, F_GETFL) < 0)
        {
            libexplain_buffer_ebadf(sb, fildes, "fildes");
            break;
        }
        switch (command)
        {
        case F_SETLK:
        case F_SETLKW:
            libexplain_string_buffer_puts
            (
                sb,
                "the file descriptor open flags"
            );
            {
                /*
                 *  Insert the actual open mode here, if we can get it.
                 */
                int n = fcntl(fildes, F_GETFL);
                if (n >= 0)
                {
                    libexplain_string_buffer_puts(sb, " (");
                    libexplain_buffer_open_flags(sb, n);
                    libexplain_string_buffer_putc(sb, ')');
                }
            }
            libexplain_string_buffer_puts
            (
                sb,
                " do not match the type of lock requested"
            );
            break;

        default:
            break;
        }
        break;

    case EDEADLK:
        /* FIXME: which other process? */
        libexplain_string_buffer_puts
        (
            sb,
            "it was detected that the specified F_SETLKW "
            "command would cause a deadlock"
        );
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "arg");
        break;

    case EINTR:
        switch (command)
        {
        default:
            libexplain_buffer_eintr(sb, "command");
            break;

        case F_SETLK:
        case F_GETLK:
            libexplain_string_buffer_puts
            (
                sb,
                "the command was interrupted by a signal "
                "before the lock was checked or acquired; most likely "
                "when locking a remote file (e.g. locking over NFS), "
                "but can sometimes happen locally"
            );
            break;
        }
        break;

    case EINVAL:
        switch (command)
        {
        case F_DUPFD:
#ifdef F_DUPFD_CLOEXEC
        case F_DUPFD_CLOEXEC:
#endif
            libexplain_buffer_check_fildes_range(sb, arg, "arg");
            break;

#ifdef F_SETSIG
        case F_SETSIG:
            libexplain_string_buffer_puts
            (
                sb,
                "the arg is not an allowable signal number"
            );
            break;
#endif

        default:
            break;
        }
        break;

    case EMFILE:
        switch (command)
        {
        case F_DUPFD:
#ifdef F_DUPFD_CLOEXEC
        case F_DUPFD_CLOEXEC:
#endif
            libexplain_buffer_emfile(sb);
            break;

        default:
            break;
        }
        break;

    case ENOLCK:
        libexplain_string_buffer_puts
        (
            sb,
            "too many segment locks are open, or the lock table is full, "
            "or a remote locking protocol failed (e.g. locking over "
            "NFS)"
        );
        break;

    case EPERM:
        libexplain_string_buffer_puts
        (
            sb,
            "it was attempted to clear the O_APPEND flag on a file "
            "that has the append-only attribute set"
        );
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_fcntl(libexplain_string_buffer_t *sb, int errnum,
    int fildes, int command, long arg)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_fcntl_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes,
        command,
        arg
    );
    libexplain_buffer_errno_fcntl_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes,
        command,
        arg
    );
    libexplain_explanation_assemble(&exp, sb);
}
