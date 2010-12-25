/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010 Peter Miller
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
#include <libexplain/accept4.h>
#include <libexplain/access.h>
#include <libexplain/acct.h>
#include <libexplain/adjtime.h>
#include <libexplain/adjtimex.h>
#include <libexplain/bind.h>
#include <libexplain/calloc.h>
#include <libexplain/chdir.h>
#include <libexplain/chmod.h>
#include <libexplain/chown.h>
#include <libexplain/chroot.h>
#include <libexplain/close.h>
#include <libexplain/closedir.h>
#include <libexplain/connect.h>
#include <libexplain/creat.h>
#include <libexplain/dirfd.h>
#include <libexplain/dup.h>
#include <libexplain/dup2.h>
#include <libexplain/eventfd.h>
#include <libexplain/execlp.h>
#include <libexplain/execve.h>
#include <libexplain/execvp.h>
#include <libexplain/exit.h>
#include <libexplain/fchdir.h>
#include <libexplain/fchmod.h>
#include <libexplain/fchown.h>
#include <libexplain/fclose.h>
#include <libexplain/fcntl.h>
#include <libexplain/fdopen.h>
#include <libexplain/fdopendir.h>
#include <libexplain/feof.h>
#include <libexplain/ferror.h>
#include <libexplain/fflush.h>
#include <libexplain/fgetc.h>
#include <libexplain/fgetpos.h>
#include <libexplain/fgets.h>
#include <libexplain/fileno.h>
#include <libexplain/flock.h>
#include <libexplain/fopen.h>
#include <libexplain/fork.h>
#include <libexplain/fpathconf.h>
#include <libexplain/fprintf.h>
#include <libexplain/fpurge.h>
#include <libexplain/fputc.h>
#include <libexplain/fputs.h>
#include <libexplain/fread.h>
#include <libexplain/freopen.h>
#include <libexplain/fseek.h>
#include <libexplain/fsetpos.h>
#include <libexplain/fstat.h>
#include <libexplain/fstatfs.h>
#include <libexplain/fstatvfs.h>
#include <libexplain/fsync.h>
#include <libexplain/ftell.h>
#include <libexplain/ftime.h>
#include <libexplain/ftruncate.h>
#include <libexplain/futimes.h>
#include <libexplain/fwrite.h>
#include <libexplain/getaddrinfo.h>
#include <libexplain/getc.h>
#include <libexplain/getchar.h>
#include <libexplain/getcwd.h>
#include <libexplain/getdomainname.h>
#include <libexplain/getgroups.h>
#include <libexplain/gethostname.h>
#include <libexplain/getpeername.h>
#include <libexplain/getrlimit.h>
#include <libexplain/getsockname.h>
#include <libexplain/getsockopt.h>
#include <libexplain/gettimeofday.h>
#include <libexplain/getw.h>
#include <libexplain/ioctl.h>
#include <libexplain/kill.h>
#include <libexplain/lchown.h>
#include <libexplain/link.h>
#include <libexplain/listen.h>
#include <libexplain/lseek.h>
#include <libexplain/lstat.h>
#include <libexplain/malloc.h>
#include <libexplain/mkdir.h>
#include <libexplain/mkdtemp.h>
#include <libexplain/mknod.h>
#include <libexplain/mkostemp.h>
#include <libexplain/mkstemp.h>
#include <libexplain/mktemp.h>
#include <libexplain/mmap.h>
#include <libexplain/munmap.h>
#include <libexplain/nice.h>
#include <libexplain/open.h>
#include <libexplain/opendir.h>
#include <libexplain/output.h>
#include <libexplain/pathconf.h>
#include <libexplain/pclose.h>
#include <libexplain/pipe.h>
#include <libexplain/poll.h>
#include <libexplain/popen.h>
#include <libexplain/pread.h>
#include <libexplain/printf.h>
#include <libexplain/program_name.h>
#include <libexplain/ptrace.h>
#include <libexplain/public_config.h>
#include <libexplain/putc.h>
#include <libexplain/putchar.h>
#include <libexplain/putenv.h>
#include <libexplain/puts.h>
#include <libexplain/putw.h>
#include <libexplain/pwrite.h>
#include <libexplain/raise.h>
#include <libexplain/read.h>
#include <libexplain/readdir.h>
#include <libexplain/readlink.h>
#include <libexplain/readv.h>
#include <libexplain/realloc.h>
#include <libexplain/remove.h>
#include <libexplain/rename.h>
#include <libexplain/rmdir.h>
#include <libexplain/select.h>
#include <libexplain/setbuf.h>
#include <libexplain/setbuffer.h>
#include <libexplain/setdomainname.h>
#include <libexplain/setenv.h>
#include <libexplain/setgroups.h>
#include <libexplain/sethostname.h>
#include <libexplain/setlinebuf.h>
#include <libexplain/setsockopt.h>
#include <libexplain/setvbuf.h>
#include <libexplain/signalfd.h>
#include <libexplain/snprintf.h>
#include <libexplain/socket.h>
#include <libexplain/socketpair.h>
#include <libexplain/sprintf.h>
#include <libexplain/stat.h>
#include <libexplain/statfs.h>
#include <libexplain/statvfs.h>
#include <libexplain/stime.h>
#include <libexplain/strdup.h>
#include <libexplain/strndup.h>
#include <libexplain/strtod.h>
#include <libexplain/strtof.h>
#include <libexplain/strtol.h>
#include <libexplain/strtold.h>
#include <libexplain/strtoll.h>
#include <libexplain/strtoul.h>
#include <libexplain/strtoull.h>
#include <libexplain/symlink.h>
#include <libexplain/system.h>
#include <libexplain/tcdrain.h>
#include <libexplain/tcflow.h>
#include <libexplain/tcflush.h>
#include <libexplain/tcgetattr.h>
#include <libexplain/tcsendbreak.h>
#include <libexplain/tcsetattr.h>
#include <libexplain/telldir.h>
#include <libexplain/tempnam.h>
#include <libexplain/time.h>
#include <libexplain/timerfd_create.h>
#include <libexplain/tmpfile.h>
#include <libexplain/tmpnam.h>
#include <libexplain/truncate.h>
#include <libexplain/ungetc.h>
#include <libexplain/unlink.h>
#include <libexplain/unsetenv.h>
#include <libexplain/ustat.h>
#include <libexplain/utime.h>
#include <libexplain/utimes.h>
#include <libexplain/vfork.h>
#include <libexplain/vfprintf.h>
#include <libexplain/vprintf.h>
#include <libexplain/vsnprintf.h>
#include <libexplain/vsprintf.h>
#include <libexplain/wait.h>
#include <libexplain/wait3.h>
#include <libexplain/wait4.h>
#include <libexplain/waitpid.h>
#include <libexplain/write.h>
#include <libexplain/writev.h>

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
  *     fprintf(stderr, "%s\n", explain_open(path, flags, mode));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  *
  * If, for example, you were to try to open <tt>no-such-dir/some-file</tt>,
  * the above code would print the following error message:
  * @code
  * open(path = "no-such-dir/some-file", flags = O_RDONLY)
  * failed, No such file or directory (2, ENOENT) because there is no
  * "no-such-dir" directory in the current directory
  * @endcode
  *
  * The above code comes pre-packaged in the form of the
  * #explain_open_or_die function.  Using this helper function, the code
  * fragment above may be written as:
  * @code
  * fd = explain_open_or_die(path, flags, mode);
  * @endcode
  *
  * Helper functions are available for all supported system calls.
  * They come in two varieties:
  * <tt>explain_</tt><i>name</i><tt>_or_die</tt> and
  * <tt>explain_</tt><i>name</i><tt>_on_error</tt>.
  * The latter prints the error, but still returns to the caller
  * with the original error code.
  *
  * The library also provides thread safe function variations.
  *
  * @ref lfs "Large File Support"
  *
  * <a href="http://libexplain.sourceforge.net/" >LibExplain Home Page</a>
  */

#endif /* LIBEXPLAIN_LIBEXPLAIN_H */
