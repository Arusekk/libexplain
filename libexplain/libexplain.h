/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#include <libexplain/access.h>
#include <libexplain/chdir.h>
#include <libexplain/chmod.h>
#include <libexplain/chown.h>
#include <libexplain/close.h>
#include <libexplain/closedir.h>
#include <libexplain/dup.h>
#include <libexplain/execve.h>
#include <libexplain/fchdir.h>
#include <libexplain/fchmod.h>
#include <libexplain/fclose.h>
#include <libexplain/fcntl.h>
#include <libexplain/ferror.h>
#include <libexplain/fflush.h>
#include <libexplain/fgetc.h>
#include <libexplain/fgets.h>
#include <libexplain/fopen.h>
#include <libexplain/fork.h>
#include <libexplain/fread.h>
#include <libexplain/freopen.h>
#include <libexplain/fstat.h>
#include <libexplain/ftruncate.h>
#include <libexplain/fwrite.h>
#include <libexplain/getc.h>
#include <libexplain/getchar.h>
#include <libexplain/gettimeofday.h>
#include <libexplain/lchown.h>
#include <libexplain/link.h>
#include <libexplain/lseek.h>
#include <libexplain/lstat.h>
#include <libexplain/mkdir.h>
#include <libexplain/open.h>
#include <libexplain/opendir.h>
#include <libexplain/read.h>
#include <libexplain/readdir.h>
#include <libexplain/readlink.h>
#include <libexplain/remove.h>
#include <libexplain/rename.h>
#include <libexplain/rmdir.h>
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
  * open("no-such-dir/some-file", O_RDONLY) failed, No such file or directory
  * (2, ENOENT) because directory "no-such-dir" does not exist
  * @endcode
  *
  * The library also provides thread safe function variations.
  */

#endif /* LIBEXPLAIN_LIBEXPLAIN_H */
