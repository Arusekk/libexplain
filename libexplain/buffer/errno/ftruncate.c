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
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/efbig.h>
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/ftruncate.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/etxtbsy.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/fildes_not_open_for_writing.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/explanation.h>
#include <libexplain/open_flags.h>


static void
libexplain_buffer_errno_ftruncate_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes, long long length)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "ftruncate(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_printf(sb, ", length = %lld)", length);
}


static void
libexplain_buffer_errno_ftruncate_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes, long long length)
{
    switch (errnum)
    {
    case EACCES:
        libexplain_string_buffer_puts
        (
            sb,
            "the process does not have write permission to the file"
        );
        break;

    case EBADF:
        if (libexplain_buffer_fildes_not_open_for_writing(sb, fildes, "fildes"))
            libexplain_buffer_ebadf(sb, fildes, "fildes");
        break;

    case EFBIG:
        libexplain_buffer_efbig_fildes(sb, fildes, length, "length");
        break;

    case EINTR:
        libexplain_buffer_eintr(sb, "ftruncate");
        break;

    case EINVAL:
        if (length < 0)
        {
            libexplain_string_buffer_puts(sb, "'length' is negative");
        }
        else
        {
            long            file_size_bits;

#ifdef _PC_FILESIZEBITS
            /*
             * FIXME: also use getrlimit(RLIMIT_FSIZE)
             *
             * RLIMIT_FSIZE
             *     The maximum size of files that the process may
             *     create.  Attempts to extend a file beyond this limit
             *     result in delivery of a SIGXFSZ signal.  By default,
             *     this signal terminates a process, but a process can
             *     catch this signal instead, in which case the relevant
             *     system call (e.g., write(2), truncate(2)) fails with
             *     the error EFBIG.
             */
            file_size_bits = fpathconf(fildes, _PC_FILESIZEBITS);
            if
            (
                file_size_bits > 0
            &&
                (size_t)file_size_bits < 8 * sizeof(long long)
            &&
                length > (1LL << file_size_bits)
            )
            {
                libexplain_string_buffer_printf
                (
                    sb,
                    "'length' is larger than the maximum file size (2 ** %ld)",
                    file_size_bits
                );
            }
            else
#endif
            {
                int             flags;

                flags = fcntl(fildes, F_GETFL);
                if (flags >= 0 && (flags & O_ACCMODE) == O_RDONLY)
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "the file is not open for writing ("
                    );
                    libexplain_buffer_open_flags(sb, flags);
                    libexplain_string_buffer_putc(sb, ')');
                }
                else
                {
                    struct stat     st;

                    if (fstat(fildes, &st) >= 0 && !S_ISREG(st.st_mode))
                    {
                        libexplain_string_buffer_puts
                        (
                            sb,
                            "fildes refers to a "
                        );
                        libexplain_buffer_file_type(sb, st.st_mode);
                        libexplain_string_buffer_puts
                        (
                            sb,
                            ", it is only possible to truncate a "
                        );
                        libexplain_buffer_file_type(sb, S_IFREG);
                    }
                    else
                    {
                        libexplain_string_buffer_puts
                        (
                            sb,
                            "the file is not open for writing; or, the file is "
                            "not a "
                        );
                        libexplain_buffer_file_type(sb, S_IFREG);
                    }
                }
            }
        }
        break;

    case EIO:
        libexplain_buffer_eio_fildes(sb, fildes);
        break;

    case EISDIR:
        libexplain_string_buffer_puts
        (
            sb,
            "fildes refers to a "
        );
        libexplain_buffer_file_type(sb, S_IFDIR);
        libexplain_string_buffer_puts
        (
            sb,
            ", it is only possible to truncate a "
        );
        libexplain_buffer_file_type(sb, S_IFREG);
        break;

    case EPERM:
        libexplain_string_buffer_puts
        (
            sb,
            "the underlying file system does not support extending a "
            "file beyond its current size"
        );
        libexplain_buffer_mount_point_fd(sb, fildes);
        break;

    case EROFS:
        libexplain_buffer_erofs_fildes(sb, fildes, "fildes");
        break;

    case ETXTBSY:
        libexplain_buffer_etxtbsy_fildes(sb, fildes);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_ftruncate(libexplain_string_buffer_t *sb, int errnum,
    int fildes, long long length)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_ftruncate_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes,
        length
    );
    libexplain_buffer_errno_ftruncate_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes,
        length
    );
    libexplain_explanation_assemble(&exp, sb);
}
