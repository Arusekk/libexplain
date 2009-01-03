/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enfile.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/execve.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/path_to_pid.h>
#include <libexplain/buffer/pathname.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>
#include <libexplain/option.h>
#include <libexplain/path_is_efault.h>


static int
count(char *const *p)
{
    int             result;

    result = 0;
    while (*p)
    {
        ++result;
        ++p;
    }
    return result;
}


static void
libexplain_buffer_errno_execve_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, char *const *argv, char *const *envp)
{
    libexplain_string_buffer_puts(sb, "execve(pathname = ");
    libexplain_buffer_pathname(sb, pathname);
    if (errnum == EFAULT)
    {
        libexplain_string_buffer_puts(sb, ", argv = ");
        libexplain_buffer_pointer(sb, argv);
        libexplain_string_buffer_puts(sb, ", envp = ");
        libexplain_buffer_pointer(sb, envp);
    }
    else
    {
        int             n;
        int             argsize;

        /*
         * produce output similar to strace
         */
        libexplain_string_buffer_puts(sb, ", argv = [");
        argsize = 0;
        for (n = 0; ; ++n)
        {
            const char      *s;

            s = argv[n];
            if (!s)
                break;
            argsize += strlen(s);
            if (n)
            {
                libexplain_string_buffer_puts(sb, ", ");
                if (argsize >= 1000)
                {
                    libexplain_string_buffer_printf
                    (
                        sb,
                        /* FIXME: i18n */
                        "... plus another %d command line arguments",
                        count(argv) - n
                    );
                    break;
                }
            }
            libexplain_string_buffer_puts_quoted(sb, s);
        }
        libexplain_string_buffer_puts(sb, "], envp = [/*");
        if (libexplain_option_dialect_specific())
            libexplain_string_buffer_printf(sb, " %d", count(envp));
        libexplain_string_buffer_puts(sb, " vars */]");
    }
    libexplain_string_buffer_putc(sb, ')');
}


static int
wonky_pointer(libexplain_string_buffer_t *sb, char *const *array,
    const char *array_caption)
{
    int             n;

    if (libexplain_pointer_is_efault(array))
    {
        libexplain_buffer_efault(sb, array_caption);
        return 0;
    }
    for (n = 0; array[n]; ++n)
    {
        if (libexplain_path_is_efault(array[n]))
        {
            char            temp[20];

            snprintf(temp, sizeof(temp), "%s[%d]", array_caption, n);
            libexplain_buffer_efault(sb, temp);
            return 0;
        }
    }
    return -1;
}


/**
  * The libexplain_buffer_file1 function is used to run the file(1)
  * command, if available, and insert its output into the given buffer.
  *
  * @param sb
  *     The string buffer to print into.
  * @param pathname
  *     The pathname of interest.
  * @returns
  *     int; 0 on success (file type info printed),
  *     or -1 on failure (nothing printed).
  */
static void
libexplain_buffer_file1(libexplain_string_buffer_t *sb, const char *pathname)
{
    FILE            *fp;
    char            buffer[PATH_MAX + 20];

    if (!libexplain_option_dialect_specific())
        return;

    /* FIXME: need shell quoting */
    snprintf(buffer, sizeof(buffer), "file -b %s 2> /dev/null", pathname);

    fp = popen(buffer, "r");
    if (fp)
    {
        char            *bp;

        bp = buffer;
        for (;;)
        {
            int c = getc(fp);
            if (c == EOF || c == '\n')
                break;
            *bp++ = c;
            if (bp >= buffer + sizeof(buffer) - 1)
                break;
        }
        pclose(fp);
        *bp = '\0';
        if (buffer[0])
        {
            libexplain_string_buffer_puts(sb, " (");
            libexplain_string_buffer_puts(sb, buffer);
            libexplain_string_buffer_putc(sb, ')');
        }
    }
}


void
libexplain_buffer_errno_execve_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, char *const *argv, char *const *envp)
{
    libexplain_final_t final_component;

    libexplain_final_init(&final_component);
    final_component.want_to_execute = 1;

    switch (errnum)
    {
    case E2BIG:
        libexplain_string_buffer_puts
        (
            sb,
            /* FIXME: i18n */
            "the total number of bytes in the argument list (argv) plus "
            "the environment (envp) is too large"
        );
        if (libexplain_option_dialect_specific())
        {
            long            total;
            long            arg_max;

            /*
             * Count the total size.
             */
            total = 0;
            if (argv)
            {
                int             argc;

                for (argc = 0; ; ++argc)
                {
                    const char      *p;

                    p = argv[argc];
                    if (!p)
                        break;
                    total += strlen(p) + 1;
                }
                total += (argc + 1) * sizeof(argv[0]);
            }
            if (envp)
            {
                int             envc;

                for (envc = 0; ; ++envc)
                {
                    const char      *p;

                    p = envp[envc];
                    if (!p)
                        break;
                    total += strlen(p) + 1;
                }
                total += (envc + 1) * sizeof(envp[0]);
            }

            /*
             * Get the system limit.
             */
            arg_max = sysconf(_SC_ARG_MAX);
#ifdef ARG_MAX
            if (arg_max <= 0)
                arg_max = ARG_MAX;
#endif

            /*
             * Print the dialect-specific part of the explanation.
             */
            libexplain_string_buffer_printf(sb, " (%ld", total);
            if (arg_max > 0)
                libexplain_string_buffer_printf(sb, " > %ld", arg_max);
            libexplain_string_buffer_putc(sb, ')');
        }
        break;

    case EACCES:
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                "pathname",
                &final_component
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                /* FIXME: i18n */
                "search permission is denied on a component of the path prefix "
                "of pathname or the name of a script interpreter; or, the "
                "file or a script interpreter is not a regular file; or, "
                "execute permission is denied for the file or a script or ELF "
                "interpreter; or, the file system is mounted noexec"
            );
        }
        break;

    case EFAULT:
        /*
         * FIXME: Some historical systems return [EFAULT] rather than [ENOEXEC]
         * when the new process image file is corrupted.  They are
         * non-conforming.  Only add code to handle that case if anyone
         * ever gripes.
         */
        if (libexplain_path_is_efault(pathname))
        {
            libexplain_buffer_efault(sb, "pathname");
        }
        else if
        (
            wonky_pointer(sb, argv, "argv")
        &&
            wonky_pointer(sb, envp, "envp")
        )
        {
            libexplain_buffer_efault(sb, "pathname or argv or envp");
        }
        break;

    case EINTR:
        libexplain_buffer_eintr(sb, "exec");
        break;

    case EINVAL:
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                "pathname",
                &final_component
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                /* FIXME: i18n */
                "an ELF executable had more than one PT_INTERP segment "
                "(tried to name more than one interpreter); or, the new "
                "process image file has the appropriate permission and has "
                "a recognized executable binary format, but the system does "
                "not support execution of a file with this format"
            );
            libexplain_buffer_file1(sb, pathname);
        }
        break;

    case EIO:
        libexplain_buffer_eio_path(sb, pathname);
        break;

    case EISDIR:
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                "pathname",
                &final_component
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                /*
                 * xgettext:  This message is used when explaining an EISDIR
                 * error from an execve system call, in the case where an ELF
                 * interpreter was a directory.
                 */
                i18n("an ELF interpreter was a directory")
            );
        }
        break;

#ifdef ELIBBAD
    case ELIBBAD:
        libexplain_string_buffer_puts
        (
            sb,
            /* FIXME: i18n */
            "an ELF interpreter was not in a recognized format"
        );
        break;
#endif

    case ELOOP:
        libexplain_buffer_eloop(sb, pathname, "pathname", &final_component);
        break;

    case EMFILE:
        libexplain_buffer_emfile(sb);
        break;

    case ENAMETOOLONG:
        libexplain_buffer_enametoolong
        (
            sb,
            pathname,
            "pathname",
            &final_component
        );
        break;

    case ENFILE:
        libexplain_buffer_enfile(sb);
        break;

    case ENOENT:
        libexplain_buffer_enoent(sb, pathname, "pathname", &final_component);
        break;

    case ENOEXEC:
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                "pathname",
                &final_component
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                /* FIXME: i18n */
                "an executable is not in a recognized format, is for the wrong "
                "architecture, or has some other format error that means it "
                "cannot be executed"
            );
            libexplain_buffer_file1(sb, pathname);
        }
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    case ENOTDIR:
        libexplain_buffer_enotdir(sb, pathname, "pathname", &final_component);
        break;

    case EPERM:
        /* FIXME: say which one */
        libexplain_string_buffer_puts
        (
            sb,
            /* FIXME: i18n */
            "the file system is mounted nosuid; or, the pocess is being "
            "traced; or, the user is not the superuser, and the file "
            "has the set-user-ID or set-group-ID bit set"
        );
        break;

    case ETXTBSY:
        libexplain_string_buffer_puts
        (
            sb,
            /* FIXME: i18n */
            "pathname is open for writing by one or more processes"
        );
        libexplain_buffer_path_to_pid(sb, pathname);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_execve(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, char *const *argv, char *const *envp)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_execve_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        argv,
        envp
    );
    libexplain_buffer_errno_execve_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname,
        argv,
        envp
    );
    libexplain_explanation_assemble(&exp, sb);
}
