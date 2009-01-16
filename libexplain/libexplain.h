/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#ifndef LIBEXPLAIN_LIBEXPLAIN_H
#define LIBEXPLAIN_LIBEXPLAIN_H

/**
  * @file
  * @brief This is the main user include file.
  *
  * The simplest way for users to access the functions in the library is
  * to use a single include line, like this:
  * @code
  * #include <libexplain/libexplain.h>
  * @endcode
  */

#include <libexplain/accept.h>
#include <libexplain/access.h>
#include <libexplain/bind.h>
#include <libexplain/chdir.h>
#include <libexplain/chmod.h>
#include <libexplain/chown.h>
#include <libexplain/close.h>
#include <libexplain/closedir.h>
#include <libexplain/connect.h>
#include <libexplain/creat.h>
#include <libexplain/dup.h>
#include <libexplain/dup2.h>
#include <libexplain/execve.h>
#include <libexplain/execvp.h>
#include <libexplain/fchdir.h>
#include <libexplain/fchmod.h>
#include <libexplain/fchown.h>
#include <libexplain/fclose.h>
#include <libexplain/fcntl.h>
#include <libexplain/fdopen.h>
#include <libexplain/ferror.h>
#include <libexplain/fflush.h>
#include <libexplain/fgetc.h>
#include <libexplain/fgets.h>
#include <libexplain/fopen.h>
#include <libexplain/fork.h>
#include <libexplain/fpathconf.h>
#include <libexplain/fputc.h>
#include <libexplain/fread.h>
#include <libexplain/freopen.h>
#include <libexplain/fstat.h>
#include <libexplain/ftruncate.h>
#include <libexplain/futimes.h>
#include <libexplain/fwrite.h>
#include <libexplain/getaddrinfo.h>
#include <libexplain/getc.h>
#include <libexplain/getchar.h>
#include <libexplain/getcwd.h>
#include <libexplain/getrlimit.h>
#include <libexplain/gettimeofday.h>
#include <libexplain/ioctl.h>
#include <libexplain/lchown.h>
#include <libexplain/link.h>
#include <libexplain/listen.h>
#include <libexplain/lseek.h>
#include <libexplain/lstat.h>
#include <libexplain/malloc.h>
#include <libexplain/mkdir.h>
#include <libexplain/open.h>
#include <libexplain/opendir.h>
#include <libexplain/pathconf.h>
#include <libexplain/pclose.h>
#include <libexplain/pipe.h>
#include <libexplain/popen.h>
#include <libexplain/program_name.h>
#include <libexplain/putc.h>
#include <libexplain/putchar.h>
#include <libexplain/read.h>
#include <libexplain/readdir.h>
#include <libexplain/readlink.h>
#include <libexplain/realloc.h>
#include <libexplain/remove.h>
#include <libexplain/rename.h>
#include <libexplain/rmdir.h>
#include <libexplain/select.h>
#include <libexplain/socket.h>
#include <libexplain/stat.h>
#include <libexplain/symlink.h>
#include <libexplain/system.h>
#include <libexplain/truncate.h>
#include <libexplain/unlink.h>
#include <libexplain/utime.h>
#include <libexplain/wait.h>
#include <libexplain/wait3.h>
#include <libexplain/wait4.h>
#include <libexplain/waitpid.h>
#include <libexplain/write.h>

/**
  * @mainpage
  *
  * The libexplain library provides a set of functions that may be used
  * to explain Unix and Linux system call errors.  This will make your
  * application's error messages much more understandable to your users.
  *
  * The library is not quite a drop-in replacement for strerror, but it
  * comes close.  Each system call has a dedicated libexplain function,
  * for example
  * @code
  * fd = open(path, flags, mode);
  * if (fd < 0)
  * {
  *     fprintf(stderr, "%s\n", libexplain_open(path, flags, mode));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * If, for example, you were to try to open <tt>no-such-dir/some-file</tt>,
  * the above code would print the following error message:
  * @code
  * popen(command = "no-such-dir/some-file", flags = O_RDONLY)
  * failed, No such file or directory (2, ENOENT) because there is no
  * "no-such-dir" directory in the current directory
  * @endcode
  *
  * The above code comes pre-packaged in the form of the
  * #libexplain_open_or_die function.  This is available for all
  * supported system calls.
  *
  * The library also provides thread safe function variations.
  *
  * \ref lfs "Large File Support"
  */

#endif /* LIBEXPLAIN_LIBEXPLAIN_H */
