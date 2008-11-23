/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <getopt.h>

#include <libexplain/errno_info.h>
#include <libexplain/freopen.h>
#include <libexplain/fstrcmp.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>

#include <explain/access.h>
#include <explain/chdir.h>
#include <explain/chmod.h>
#include <explain/chown.h>
#include <explain/close.h>
#include <explain/closedir.h>
#include <explain/creat.h>
#include <explain/dup.h>
#include <explain/execve.h>
#include <explain/fchdir.h>
#include <explain/fchmod.h>
#include <explain/fcntl.h>
#include <explain/ferror.h>
#include <explain/fgetc.h>
#include <explain/fgets.h>
#include <explain/fopen.h>
#include <explain/fork.h>
#include <explain/fread.h>
#include <explain/fstat.h>
#include <explain/ftruncate.h>
#include <explain/fwrite.h>
#include <explain/getc.h>
#include <explain/getchar.h>
#include <explain/gettimeofday.h>
#include <explain/lchown.h>
#include <explain/link.h>
#include <explain/lseek.h>
#include <explain/lstat.h>
#include <explain/mkdir.h>
#include <explain/opendir.h>
#include <explain/open.h>
#include <explain/read.h>
#include <explain/readdir.h>
#include <explain/readlink.h>
#include <explain/remove.h>
#include <explain/rename.h>
#include <explain/rmdir.h>
#include <explain/socket.h>
#include <explain/stat.h>
#include <explain/strerror.h>
#include <explain/symlink.h>
#include <explain/system.h>
#include <explain/truncate.h>
#include <explain/unlink.h>
#include <explain/utime.h>
#include <explain/wait.h>
#include <explain/wait3.h>
#include <explain/wait4.h>
#include <explain/waitpid.h>
#include <explain/write.h>


typedef void (*func_t)(int err, int argc, char **argv);


typedef struct table_t table_t;
struct table_t
{
    const char      *name;
    func_t          func;
};


static const table_t table[] =
{
    /* ----------  A  ------------------------------------------------------- */
    /* FIXME: add support for accept */
    { "access", explain_access },
    /* FIXME: add support for acct */
    /* FIXME: add support for add_key */
    /* FIXME: add support for adjtimex */
    /* FIXME: add support for afs_syscall */
    /* FIXME: add support for alarm */
    /* FIXME: add support for alloc_hugepages */
    /* ----------  B  ------------------------------------------------------- */
    /* FIXME: add support for bdflush */
    /* FIXME: add support for bind */
    /* FIXME: add support for break */
    /* FIXME: add support for brk */
    /* ----------  C  ------------------------------------------------------- */
    /* FIXME: add support for cacheflush */
    /* FIXME: add support for capget */
    /* FIXME: add support for capset */
    { "chdir", explain_chdir },
    { "chmod", explain_chmod },
    { "chown", explain_chown },
    /* FIXME: add support for chroot */
    /* FIXME: add support for clock_getres */
    /* FIXME: add support for clock_gettime */
    /* FIXME: add support for clock_nanosleep */
    /* FIXME: add support for clock_settime */
    /* FIXME: add support for clone */
    { "close", explain_close },
    { "closedir", explain_closedir },
    /* FIXME: add support for connect */
    { "creat", explain_creat },
    /* FIXME: add support for create_module */
    /* ----------  D  ------------------------------------------------------- */
    /* FIXME: add support for delete_module */
    /* FIXME: add support for dirfd */
    { "dup", explain_dup },
    /* FIXME: add support for dup2 */
    /* ----------  E  ------------------------------------------------------- */
    /* FIXME: add support for epoll_create */
    /* FIXME: add support for epoll_ctl */
    /* FIXME: add support for epoll_pwait */
    /* FIXME: add support for epoll_wait */
    /* FIXME: add support for eventfd */
    { "execve", explain_execve },
    /* ----------  F  ------------------------------------------------------- */
    /* FIXME: add support for faccess */
    /* FIXME: add support for fadvise64 */
    /* FIXME: add support for fallocate */
    { "fchdir", explain_fchdir },
    { "fchmod", explain_fchmod },
    /* FIXME: add support for fchown */
    { "fcntl", explain_fcntl },
    /* FIXME: add support for fdatasync */
    /* FIXME: add support for fdopen */
    /* FIXME: add support for fdopendir */
    { "ferror", explain_ferror },
    /* FIXME: add support for fflush */
    { "fgetc", explain_fgetc },
    /* FIXME: add support for fgetpos */
    { "fgets", explain_fgets },
    /* FIXME: add support for fgetxattr */
    /* FIXME: add support for fileno */
    /* FIXME: add support for flistxattr */
    /* FIXME: add support for flock */
    { "fopen", explain_fopen },
    { "fork", explain_fork },
    /* FIXME: add support for fprintf */
    /* FIXME: add support for fpurge */
    /* FIXME: add support for fputc */
    /* FIXME: add support for fputs */
    { "fread", explain_fread },
    /* FIXME: add support for free_hugepages */
    /* FIXME: add support for fremovexattr */
    /* FIXME: add support for fscanf */
    /* FIXME: add support for fseek */
    /* FIXME: add support for fsetpos */
    /* FIXME: add support for fsetxattr */
    { "fstat", explain_fstat },
    /* FIXME: add support for fstatfs */
    /* FIXME: add support for fsync */
    /* FIXME: add support for ftell */
    /* FIXME: add support for ftime */
    { "ftruncate", explain_ftruncate },
    /* FIXME: add support for futex */
    /* FIXME: add support for futimes */
    { "fwrite", explain_fwrite },
    /* ----------  G  ------------------------------------------------------- */
    { "getc", explain_getc },
    { "getchar", explain_getchar },
    /* FIXME: add support for getcpu */
    /* FIXME: add support for getcwd */
    /* FIXME: add support for getdents */
    /* FIXME: add support for getegid */
    /* FIXME: add support for geteuid */
    /* FIXME: add support for getgid */
    /* FIXME: add support for getgroups */
    /* FIXME: add support for getitimer */
    /* FIXME: add support for get_kernel_syms */
    /* FIXME: add support for get_mempolicy */
    /* FIXME: add support for getpagesize */
    /* FIXME: add support for getpeername */
    /* FIXME: add support for getpgid */
    /* FIXME: add support for getpgrp */
    /* FIXME: add support for getpid */
    /* FIXME: add support for getpmsg */
    /* FIXME: add support for getppid */
    /* FIXME: add support for getpriority */
    /* FIXME: add support for getresgid */
    /* FIXME: add support for getresuid */
    /* FIXME: add support for getrlimit */
    /* FIXME: add support for get_robust_list */
    /* FIXME: add support for getrusage */
    /* FIXME: add support for gets */
    /* FIXME: add support for getsid */
    /* FIXME: add support for getsockname */
    /* FIXME: add support for getsockopt */
    /* FIXME: add support for get_thread_area */
    /* FIXME: add support for gettid */
    { "gettimeofday", explain_gettimeofday },
    /* FIXME: add support for getuid */
    /* FIXME: add support for getw */
    /* FIXME: add support for getxattr */
    /* FIXME: add support for gtty */
    /* ----------  H  ------------------------------------------------------- */
    /* ----------  I  ------------------------------------------------------- */
    /* FIXME: add support for idle */
    /* FIXME: add support for init_module */
    /* FIXME: add support for inotify_add_watch */
    /* FIXME: add support for inotify_init */
    /* FIXME: add support for inotify_rm_watch */
    /* FIXME: add support for io_cancel */
    /* FIXME: add support for ioctl */
    /* FIXME: add support for io_destroy */
    /* FIXME: add support for io_getevents */
    /* FIXME: add support for ioperm */
    /* FIXME: add support for iopl */
    /* FIXME: add support for ioprio_get */
    /* FIXME: add support for ioprio_set */
    /* FIXME: add support for io_setup */
    /* FIXME: add support for io_submit */
    /* FIXME: add support for ipc */
    /* ----------  J  ------------------------------------------------------- */
    /* ----------  K  ------------------------------------------------------- */
    /* FIXME: add support for kexec_load */
    /* FIXME: add support for keyctl */
    /* FIXME: add support for kill */
    /* ----------  L  ------------------------------------------------------- */
    { "lchown", explain_lchown },
    /* FIXME: add support for lgetxattr */
    { "link", explain_link },
    /* FIXME: add support for listen */
    /* FIXME: add support for listxattr */
    /* FIXME: add support for llistxattr */
    /* FIXME: add support for lock */
    /* FIXME: add support for lookup_dcookie */
    /* FIXME: add support for lremovexattr */
    { "lseek", explain_lseek },
    /* FIXME: add support for lsetxattr */
    { "lstat", explain_lstat },
    /* ----------  M  ------------------------------------------------------- */
    /* FIXME: add support for madvise */
    /* FIXME: add support for madvise1 */
    /* FIXME: add support for mbind */
    /* FIXME: add support for migrate_pages */
    /* FIXME: add support for mincore */
    { "mkdir", explain_mkdir },
    /* FIXME: add support for mknod */
    /* FIXME: add support for mktemp */
    /* FIXME: add support for mlock */
    /* FIXME: add support for mlockall */
    /* FIXME: add support for mmap */
    /* FIXME: add support for mmap2 */
    /* FIXME: add support for modify_ldt */
    /* FIXME: add support for mount */
    /* FIXME: add support for move_pages */
    /* FIXME: add support for mprotect */
    /* FIXME: add support for mpx */
    /* FIXME: add support for mq_getsetattr */
    /* FIXME: add support for mq_notify */
    /* FIXME: add support for mq_open */
    /* FIXME: add support for mq_timedreceive */
    /* FIXME: add support for mq_timedsend */
    /* FIXME: add support for mq_unlink */
    /* FIXME: add support for mremap */
    /* FIXME: add support for msgctl */
    /* FIXME: add support for msgget */
    /* FIXME: add support for msgrcv */
    /* FIXME: add support for msgsnd */
    /* FIXME: add support for msync */
    /* FIXME: add support for munlock */
    /* FIXME: add support for munlockall */
    /* FIXME: add support for munmap */
    /* ----------  N  ------------------------------------------------------- */
    /* FIXME: add support for nanosleep */
    /* FIXME: add support for nfsservctl */
    /* FIXME: add support for nice */
    /* ----------  O  ------------------------------------------------------- */
    { "open", explain_open },
    { "opendir", explain_opendir },
    /* ----------  P  ------------------------------------------------------- */
    /* FIXME: add support for pause */
    /* FIXME: add support for pciconfig_iobase */
    /* FIXME: add support for pciconfig_read */
    /* FIXME: add support for pciconfig_write */
    /* FIXME: add support for perror */
    /* FIXME: add support for personality */
    /* FIXME: add support for phys */
    /* FIXME: add support for pipe */
    /* FIXME: add support for pivot_root */
    /* FIXME: add support for poll */
    /* FIXME: add support for ppoll */
    /* FIXME: add support for prctl */
    /* FIXME: add support for pread */
    /* FIXME: add support for printf */
    /* FIXME: add support for prof */
    /* FIXME: add support for profil */
    /* FIXME: add support for pselect6 */
    /* FIXME: add support for ptrace */
    /* FIXME: add support for putc */
    /* FIXME: add support for putchar */
    /* FIXME: add support for putpmsg */
    /* FIXME: add support for puts */
    /* FIXME: add support for putw */
    /* FIXME: add support for pwrite */
    /* ----------  Q  ------------------------------------------------------- */
    /* FIXME: add support for query_module */
    /* FIXME: add support for quotactl */
    /* ----------  R  ------------------------------------------------------- */
    { "read", explain_read },
    /* FIXME: add support for read */
    /* FIXME: add support for readahead */
    { "readdir", explain_readdir },
    { "readlink", explain_readlink },
    /* FIXME: add support for readv */
    /* FIXME: add support for reboot */
    /* FIXME: add support for recv */
    /* FIXME: add support for recvfrom */
    /* FIXME: add support for recvmsg */
    /* FIXME: add support for remap_file_pages */
    { "remove", explain_remove },
    /* FIXME: add support for removexattr */
    { "rename", explain_rename },
    /* FIXME: add support for request_key */
    /* FIXME: add support for restart_syscall */
    /* FIXME: add support for rewind */
    /* FIXME: add support for rewinddir */
    { "rmdir", explain_rmdir },
    /* FIXME: add support for rt_sigaction */
    /* FIXME: add support for rt_sigpending */
    /* FIXME: add support for rt_sigprocmask */
    /* FIXME: add support for rt_sigqueueinfo */
    /* FIXME: add support for rt_sigreturn */
    /* FIXME: add support for rt_sigsuspend */
    /* FIXME: add support for rt_sigtimedwait */
    /* ----------  S  ------------------------------------------------------- */
    /* FIXME: add support for scandir */
    /* FIXME: add support for scanf */
    /* FIXME: add support for sched_getaffinity */
    /* FIXME: add support for sched_getparam */
    /* FIXME: add support for sched_get_priority_max */
    /* FIXME: add support for sched_get_priority_min */
    /* FIXME: add support for sched_getscheduler */
    /* FIXME: add support for sched_rr_get_interval */
    /* FIXME: add support for sched_setaffinity */
    /* FIXME: add support for sched_setparam */
    /* FIXME: add support for sched_setscheduler */
    /* FIXME: add support for sched_yield */
    /* FIXME: add support for security */
    /* FIXME: add support for seekdir */
    /* FIXME: add support for select */
    /* FIXME: add support for semctl */
    /* FIXME: add support for semget */
    /* FIXME: add support for semop */
    /* FIXME: add support for semtimedop */
    /* FIXME: add support for send */
    /* FIXME: add support for sendfile */
    /* FIXME: add support for sendmsg */
    /* FIXME: add support for sendto */
    /* FIXME: add support for setbuf */
    /* FIXME: add support for setbuffer */
    /* FIXME: add support for setdomainname */
    /* FIXME: add support for setfsgid */
    /* FIXME: add support for setfsuid */
    /* FIXME: add support for setgid */
    /* FIXME: add support for setgroups */
    /* FIXME: add support for sethostname */
    /* FIXME: add support for setitimer */
    /* FIXME: add support for setlinebuf */
    /* FIXME: add support for set_mempolicy */
    /* FIXME: add support for setpgid */
    /* FIXME: add support for setpriority */
    /* FIXME: add support for setregid */
    /* FIXME: add support for setresgid */
    /* FIXME: add support for setresuid */
    /* FIXME: add support for setreuid */
    /* FIXME: add support for setrlimit */
    /* FIXME: add support for set_robust_list */
    /* FIXME: add support for setsid */
    /* FIXME: add support for setsockopt */
    /* FIXME: add support for set_thread_area */
    /* FIXME: add support for set_tid_address */
    /* FIXME: add support for settimeofday */
    /* FIXME: add support for setuid */
    /* FIXME: add support for setup */
    /* FIXME: add support for setvbuf */
    /* FIXME: add support for setxattr */
    /* FIXME: add support for set_zone_reclaim */
    /* FIXME: add support for sgetmask */
    /* FIXME: add support for shmat */
    /* FIXME: add support for shmctl */
    /* FIXME: add support for shmdt */
    /* FIXME: add support for shmget */
    /* FIXME: add support for shutdown */
    /* FIXME: add support for sigaction */
    /* FIXME: add support for sigaltstack */
    /* FIXME: add support for signal */
    /* FIXME: add support for signalfd */
    /* FIXME: add support for sigpending */
    /* FIXME: add support for sigprocmask */
    /* FIXME: add support for sigreturn */
    /* FIXME: add support for sigsuspend */
    { "socket", explain_socket },
    /* FIXME: add support for socketcall */
    /* FIXME: add support for socketpair */
    /* FIXME: add support for splice */
    /* FIXME: add support for sprintf */
    /* FIXME: add support for spu_create */
    /* FIXME: add support for spu_run */
    /* FIXME: add support for sscanf */
    /* FIXME: add support for ssetmask */
    { "stat", explain_stat },
    /* FIXME: add support for statfs */
    /* FIXME: add support for stime */
    { "strerror", explain_strerror },
    /* FIXME: add support for strerror */
    /* FIXME: add support for stty */
    /* FIXME: add support for swapoff */
    /* FIXME: add support for swapon */
    { "symlink", explain_symlink },
    /* FIXME: add support for sync */
    /* FIXME: add support for sync_file_range */
    /* FIXME: add support for sys_errlist */
    /* FIXME: add support for sysfs */
    /* FIXME: add support for sysinfo */
    /* FIXME: add support for syslog */
    { "system", explain_system },
    /* ----------  T  ------------------------------------------------------- */
    /* FIXME: add support for tee */
    /* FIXME: add support for telldir */
    /* FIXME: add support for tempnam */
    /* FIXME: add support for tgkill */
    /* FIXME: add support for time */
    /* FIXME: add support for timer_create */
    /* FIXME: add support for timer_delete */
    /* FIXME: add support for timer_getoverrun */
    /* FIXME: add support for timer_gettime */
    /* FIXME: add support for timer_settime */
    /* FIXME: add support for times */
    /* FIXME: add support for tkill */
    /* FIXME: add support for tmpfile */
    /* FIXME: add support for tmpnam */
    { "truncate", explain_truncate },
    /* FIXME: add support for tuxcall */
    /* ----------  U  ------------------------------------------------------- */
    /* FIXME: add support for ugetrlimit */
    /* FIXME: add support for ulimit */
    /* FIXME: add support for umask */
    /* FIXME: add support for umount */
    /* FIXME: add support for uname */
    /* FIXME: add support for ungetc */
    { "unlink", explain_unlink },
    /* FIXME: add support for unshare */
    /* FIXME: add support for uselib */
    /* FIXME: add support for ustat */
    { "utime", explain_utime },
    /* FIXME: add support for utimens */
    /* FIXME: add support for utimes */
    /* ----------  V  ------------------------------------------------------- */
    /* FIXME: add support for vfork */
    /* FIXME: add support for vfprintf */
    /* FIXME: add support for vfscanf */
    /* FIXME: add support for vhangup */
    /* FIXME: add support for vmsplice */
    /* FIXME: add support for vprintf */
    /* FIXME: add support for vscanf */
    /* FIXME: add support for vserver */
    /* FIXME: add support for vsprintf */
    /* FIXME: add support for vsscanf */
    /* ----------  W  ------------------------------------------------------- */
    { "wait", explain_wait },
    { "wait3", explain_wait3 },
    { "wait4", explain_wait4 },
    /* FIXME: add support for wait4 */
    /* FIXME: add support for waitid */
    { "waitpid", explain_waitpid },
    { "write", explain_write },
    /* FIXME: add support for writev */
    /* ----------  X  ------------------------------------------------------- */
    /* ----------  Y  ------------------------------------------------------- */
    /* ----------  Z  ------------------------------------------------------- */
};


static func_t
find_function(const char *name)
{
    const table_t   *tp;
    const table_t   *best_tp;
    double          best_weight;

    for (tp = table; tp < ENDOF(table); ++tp)
    {
        if (0 == strcasecmp(name, tp->name))
            return tp->func;
    }

    best_tp = 0;
    best_weight = 0.6;
    for (tp = table; tp < ENDOF(table); ++tp)
    {
        double          weight;

        weight = libexplain_fstrcmp(name, tp->name);
        if (best_weight < weight)
        {
            best_tp = tp;
            best_weight = weight;
        }
    }
    if (best_tp)
    {
        fprintf
        (
            stderr,
            "function \"%s\" unknown, closest is the \"%s\" funtion\n",
            name,
            best_tp->name
        );
        return best_tp->func;
    }
    else
        fprintf(stderr, "function \"%s\" unknown\n", name);
    exit(EXIT_FAILURE);
    return 0;
}


static void
usage(void)
{
    fprintf(stderr, "Usage: explain -e <errno> <function> [ <args> ... ]\n");
    fprintf(stderr, "       explain -m <message> <function> [ <args> ... ]\n");
    fprintf(stderr, "       explain -V\n");
    exit(EXIT_FAILURE);
}


static const struct option options[] =
{
    { "errno", 1, 0, 'e' },
    { "message", 1, 0, 'm' },
    { "version", 0, 0, 'V' },
    { 0, 0, 0, 0 }
};


static int exit_status;


static int
figure_out_error(const char *text)
{
    long            n;
    char            *ep;
    const libexplain_errno_info_t *eip;
    libexplain_string_buffer_t sb;
    char            message[200];

    libexplain_string_buffer_init(&sb, message, sizeof(message));

    /* is it a number? */
    n = strtol(text, &ep, 0);
    if (ep != text && !*ep)
        return n;

    /* is it a symbol? */
    eip = libexplain_errno_info_by_name(text);
    if (eip)
        return eip->error_number;

    /* is it an exact text? */
    eip = libexplain_errno_info_by_text(text);
    if (eip)
        return eip->error_number;

    /* is it a fuzzy symbol? */
    eip = libexplain_errno_info_by_name_fuzzy(text);
    if (eip)
    {
        libexplain_string_buffer_puts(&sb, "the error ");
        libexplain_string_buffer_puts_quoted(&sb, text);
        libexplain_string_buffer_puts
        (
            &sb,
            " doesn't match any known symbol, guessing you meant "
        );
        libexplain_string_buffer_puts_quoted(&sb, eip->name);
        libexplain_string_buffer_puts(&sb, " instead");
        libexplain_wrap_and_print(stderr, message);
        exit_status = EXIT_FAILURE;
        return eip->error_number;
    }

    /* is it a fuzzy text? */
    eip = libexplain_errno_info_by_text_fuzzy(text);
    if (eip)
    {
        libexplain_string_buffer_puts(&sb, "the error ");
        libexplain_string_buffer_puts_quoted(&sb, text);
        libexplain_string_buffer_puts
        (
            &sb,
            " doesn't match any known symbol, guessing you meant "
        );
        libexplain_string_buffer_puts_quoted(&sb, strerror(eip->error_number));
        libexplain_string_buffer_puts(&sb, " instead");
        libexplain_wrap_and_print(stderr, message);
        exit_status = EXIT_FAILURE;
        return eip->error_number;
    }

    libexplain_string_buffer_puts(&sb, "the error ");
    libexplain_string_buffer_puts_quoted(&sb, text);
    libexplain_string_buffer_puts
    (
        &sb,
        " doesn't look like a number, or an error symbol, or a "
        "strerror() string, aborting"
    );
    libexplain_wrap_and_print(stderr, message);
    exit(EXIT_FAILURE);
    /* NOTREACHED */
    return 0;
}


int
main(int argc, char **argv)
{
    func_t          func;
    int             err;

    exit_status = EXIT_SUCCESS;
    err = -1;
    for (;;)
    {
        int c = getopt_long(argc, argv, "e:o:V", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'e':
            err = figure_out_error(optarg);
            break;

        case 'o':
            libexplain_freopen_or_die(optarg, "w", stdout);
            break;

        case 'V':
            libexplain_version_print();
            return 0;

        default:
            usage();
            /* NOTREACHED */
        }
    }
    if (err < 0)
    {
        fprintf(stderr, "please specify an error number (-e)\n");
        exit(EXIT_FAILURE);
    }
    if (optind >= argc)
        usage();
    func = find_function(argv[optind]);
    ++optind;
    func(err, argc - optind, argv + optind);
    return exit_status;
}
