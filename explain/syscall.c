/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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

#include <libexplain/sizeof.h>
#include <libexplain/fstrcmp.h>

#include <explain/syscall.h>
#include <explain/syscall/accept.h>
#include <explain/syscall/accept4.h>
#include <explain/syscall/access.h>
#include <explain/syscall/acct.h>
#include <explain/syscall/adjtime.h>
#include <explain/syscall/adjtimex.h>
#include <explain/syscall/bind.h>
#include <explain/syscall/chdir.h>
#include <explain/syscall/chmod.h>
#include <explain/syscall/chown.h>
#include <explain/syscall/chroot.h>
#include <explain/syscall/close.h>
#include <explain/syscall/closedir.h>
#include <explain/syscall/connect.h>
#include <explain/syscall/creat.h>
#include <explain/syscall/dirfd.h>
#include <explain/syscall/dup.h>
#include <explain/syscall/dup2.h>
#include <explain/syscall/eventfd.h>
#include <explain/syscall/execve.h>
#include <explain/syscall/execvp.h>
#include <explain/syscall/fchdir.h>
#include <explain/syscall/fchmod.h>
#include <explain/syscall/fchown.h>
#include <explain/syscall/fcntl.h>
#include <explain/syscall/fdopen.h>
#include <explain/syscall/ferror.h>
#include <explain/syscall/fflush.h>
#include <explain/syscall/fgetc.h>
#include <explain/syscall/fgets.h>
#include <explain/syscall/fileno.h>
#include <explain/syscall/flock.h>
#include <explain/syscall/fopen.h>
#include <explain/syscall/fork.h>
#include <explain/syscall/fpathconf.h>
#include <explain/syscall/fputc.h>
#include <explain/syscall/fread.h>
#include <explain/syscall/fstat.h>
#include <explain/syscall/fstatfs.h>
#include <explain/syscall/ftime.h>
#include <explain/syscall/ftruncate.h>
#include <explain/syscall/futimes.h>
#include <explain/syscall/fwrite.h>
#include <explain/syscall/getc.h>
#include <explain/syscall/getchar.h>
#include <explain/syscall/getcwd.h>
#include <explain/syscall/getgroups.h>
#include <explain/syscall/gethostname.h>
#include <explain/syscall/getpeername.h>
#include <explain/syscall/getrlimit.h>
#include <explain/syscall/getsockname.h>
#include <explain/syscall/getsockopt.h>
#include <explain/syscall/gettimeofday.h>
#include <explain/syscall/ioctl.h>
#include <explain/syscall/kill.h>
#include <explain/syscall/lchown.h>
#include <explain/syscall/link.h>
#include <explain/syscall/listen.h>
#include <explain/syscall/lseek.h>
#include <explain/syscall/lstat.h>
#include <explain/syscall/malloc.h>
#include <explain/syscall/mkdir.h>
#include <explain/syscall/nice.h>
#include <explain/syscall/opendir.h>
#include <explain/syscall/open.h>
#include <explain/syscall/pathconf.h>
#include <explain/syscall/pclose.h>
#include <explain/syscall/pipe.h>
#include <explain/syscall/popen.h>
#include <explain/syscall/pread.h>
#include <explain/syscall/putc.h>
#include <explain/syscall/putchar.h>
#include <explain/syscall/pwrite.h>
#include <explain/syscall/read.h>
#include <explain/syscall/readdir.h>
#include <explain/syscall/readlink.h>
#include <explain/syscall/realloc.h>
#include <explain/syscall/remove.h>
#include <explain/syscall/rename.h>
#include <explain/syscall/rmdir.h>
#include <explain/syscall/select.h>
#include <explain/syscall/setgroups.h>
#include <explain/syscall/sethostname.h>
#include <explain/syscall/setsockopt.h>
#include <explain/syscall/signalfd.h>
#include <explain/syscall/socket.h>
#include <explain/syscall/stat.h>
#include <explain/syscall/statfs.h>
#include <explain/syscall/strdup.h>
#include <explain/syscall/strerror.h>
#include <explain/syscall/strndup.h>
#include <explain/syscall/strtod.h>
#include <explain/syscall/strtof.h>
#include <explain/syscall/strtol.h>
#include <explain/syscall/strtold.h>
#include <explain/syscall/strtoll.h>
#include <explain/syscall/strtoul.h>
#include <explain/syscall/strtoull.h>
#include <explain/syscall/symlink.h>
#include <explain/syscall/system.h>
#include <explain/syscall/time.h>
#include <explain/syscall/timerfd_create.h>
#include <explain/syscall/truncate.h>
#include <explain/syscall/unlink.h>
#include <explain/syscall/utime.h>
#include <explain/syscall/wait.h>
#include <explain/syscall/wait3.h>
#include <explain/syscall/wait4.h>
#include <explain/syscall/waitpid.h>
#include <explain/syscall/write.h>


typedef struct table_t table_t;
struct table_t
{
    const char      *name;
    func_t          func;
};


static const table_t table[] =
{
    /* ----------  A  ------------------------------------------------------- */
    { "accept", explain_syscall_accept },
    { "accept4", explain_syscall_accept4 },
    { "access", explain_syscall_access },
    { "acct", explain_syscall_acct },
    { "adjtime", explain_syscall_adjtime },
    { "adjtimex", explain_syscall_adjtimex },
    /* FIXME: add support for afs_syscall */
    /* FIXME: add support for alloc_hugepages */
    /* ----------  B  ------------------------------------------------------- */
    /* FIXME: add support for bdflush */
    { "bind", explain_syscall_bind },
    /* FIXME: add support for break */
    /* FIXME: add support for brk */
    /* ----------  C  ------------------------------------------------------- */
    /* FIXME: add support for cacheflush */
    /* FIXME: add support for capget */
    /* FIXME: add support for capset */
    { "chdir", explain_syscall_chdir },
    { "chmod", explain_syscall_chmod },
    { "chown", explain_syscall_chown },
    { "chroot", explain_syscall_chroot },
    /* FIXME: add support for clock_getres */
    /* FIXME: add support for clock_gettime */
    /* FIXME: add support for clock_nanosleep */
    /* FIXME: add support for clock_settime */
    /* FIXME: add support for clone */
    { "close", explain_syscall_close },
    { "closedir", explain_syscall_closedir },
    { "connect", explain_syscall_connect },
    { "creat", explain_syscall_creat },
    /* FIXME: add support for create_module */
    /* ----------  D  ------------------------------------------------------- */
    /* FIXME: add support for delete_module */
    { "dirfd", explain_syscall_dirfd },
    { "dup", explain_syscall_dup },
    { "dup2", explain_syscall_dup2 },
    /* ----------  E  ------------------------------------------------------- */
    /* FIXME: add support for epoll_create */
    /* FIXME: add support for epoll_ctl */
    /* FIXME: add support for epoll_pwait */
    /* FIXME: add support for epoll_wait */
    { "eventfd", explain_syscall_eventfd },
    { "execve", explain_syscall_execve },
    { "execvp", explain_syscall_execvp },
    /* ----------  F  ------------------------------------------------------- */
    /* FIXME: add support for faccess */
    /* FIXME: add support for fadvise64 */
    /* FIXME: add support for fallocate */
    { "fchdir", explain_syscall_fchdir },
    { "fchmod", explain_syscall_fchmod },
    { "fchown", explain_syscall_fchown },
    { "fcntl", explain_syscall_fcntl },
    /* FIXME: add support for fdatasync */
    { "fdopen", explain_syscall_fdopen },
    /* FIXME: add support for fdopendir */
    { "ferror", explain_syscall_ferror },
    { "fflush", explain_syscall_fflush },
    { "fgetc", explain_syscall_fgetc },
    /* FIXME: add support for fgetpos */
    { "fgets", explain_syscall_fgets },
    /* FIXME: add support for fgetxattr */
    { "fileno", explain_syscall_fileno },
    /* FIXME: add support for flistxattr */
    { "flock", explain_syscall_flock },
    { "fopen", explain_syscall_fopen },
    { "fork", explain_syscall_fork },
    { "fpathconf", explain_syscall_fpathconf },
    /* FIXME: add support for fprintf */
    /* FIXME: add support for fpurge */
    { "fputc", explain_syscall_fputc },
    /* FIXME: add support for fputs */
    { "fread", explain_syscall_fread },
    /* FIXME: add support for free_hugepages */
    /* FIXME: add support for fremovexattr */
    /* FIXME: add support for fscanf */
    /* FIXME: add support for fseek */
    /* FIXME: add support for fsetpos */
    /* FIXME: add support for fsetxattr */
    { "fstat", explain_syscall_fstat },
    { "fstatfs", explain_syscall_fstatfs },
    /* FIXME: add support for fsync */
    /* FIXME: add support for ftell */
    { "ftime", explain_syscall_ftime },
    { "ftruncate", explain_syscall_ftruncate },
    /* FIXME: add support for futex */
    { "futimes", explain_syscall_futimes },
    { "fwrite", explain_syscall_fwrite },
    /* ----------  G  ------------------------------------------------------- */
    { "getc", explain_syscall_getc },
    { "getchar", explain_syscall_getchar },
    /* FIXME: add support for getcpu */
    { "getcwd", explain_syscall_getcwd },
    /* FIXME: add support for getdents */
    { "getgroups", explain_syscall_getgroups },
    { "gethostname", explain_syscall_gethostname },
    /* FIXME: add support for getitimer */
    /* FIXME: add support for get_kernel_syms */
    /* FIXME: add support for get_mempolicy */
    /* FIXME: add support for getpagesize */
    { "getpeername", explain_syscall_getpeername },
    /* FIXME: add support for getpgid */
    /* FIXME: add support for getpgrp */
    /* FIXME: add support for getpid */
    /* FIXME: add support for getpmsg */
    /* FIXME: add support for getppid */
    /* FIXME: add support for getpriority */
    /* FIXME: add support for getresgid */
    /* FIXME: add support for getresuid */
    { "getrlimit", explain_syscall_getrlimit },
    /* FIXME: add support for get_robust_list */
    /* FIXME: add support for getrusage */
    /* FIXME: add support for gets */
    /* FIXME: add support for getsid */
    { "getsockname", explain_syscall_getsockname },
    { "getsockopt", explain_syscall_getsockopt },
    /* FIXME: add support for get_thread_area */
    /* FIXME: add support for gettid */
    { "gettimeofday", explain_syscall_gettimeofday },
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
    { "ioctl", explain_syscall_ioctl },
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
    { "kill", explain_syscall_kill },
    /* ----------  L  ------------------------------------------------------- */
    { "lchown", explain_syscall_lchown },
    /* FIXME: add support for lgetxattr */
    { "link", explain_syscall_link },
    { "listen", explain_syscall_listen },
    /* FIXME: add support for listxattr */
    /* FIXME: add support for llistxattr */
    /* FIXME: add support for lock */
    /* FIXME: add support for lookup_dcookie */
    /* FIXME: add support for lremovexattr */
    { "lseek", explain_syscall_lseek },
    /* FIXME: add support for lsetxattr */
    { "lstat", explain_syscall_lstat },
    /* ----------  M  ------------------------------------------------------- */
    /* FIXME: add support for madvise */
    /* FIXME: add support for madvise1 */
    { "malloc", explain_syscall_malloc },
    /* FIXME: add support for mbind */
    /* FIXME: add support for migrate_pages */
    /* FIXME: add support for mincore */
    { "mkdir", explain_syscall_mkdir },
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
    { "nice", explain_syscall_nice },
    /* ----------  O  ------------------------------------------------------- */
    { "open", explain_syscall_open },
    { "opendir", explain_syscall_opendir },
    /* ----------  P  ------------------------------------------------------- */
    { "pathconf", explain_syscall_pathconf },
    /* FIXME: add support for pause */
    /* FIXME: add support for pciconfig_iobase */
    /* FIXME: add support for pciconfig_read */
    /* FIXME: add support for pciconfig_write */
    { "pclose", explain_syscall_pclose },
    /* FIXME: add support for perror */
    /* FIXME: add support for personality */
    /* FIXME: add support for phys */
    { "pipe", explain_syscall_pipe },
    /* FIXME: add support for pivot_root */
    /* FIXME: add support for poll */
    { "popen", explain_syscall_popen },
    /* FIXME: add support for ppoll */
    /* FIXME: add support for prctl */
    { "pread", explain_syscall_pread },
    /* FIXME: add support for printf */
    /* FIXME: add support for prof */
    /* FIXME: add support for profil */
    /* FIXME: add support for pselect6 */
    /* FIXME: add support for ptrace */
    { "putc", explain_syscall_putc },
    { "putchar", explain_syscall_putchar },
    /* FIXME: add support for putpmsg */
    /* FIXME: add support for puts */
    /* FIXME: add support for putw */
    { "pwrite", explain_syscall_pwrite },
    /* ----------  Q  ------------------------------------------------------- */
    /* FIXME: add support for query_module */
    /* FIXME: add support for quotactl */
    /* ----------  R  ------------------------------------------------------- */
    { "read", explain_syscall_read },
    /* FIXME: add support for readahead */
    { "readdir", explain_syscall_readdir },
    { "readlink", explain_syscall_readlink },
    /* FIXME: add support for readv */
    { "realloc", explain_syscall_realloc },
    /* FIXME: add support for reboot */
    /* FIXME: add support for recv */
    /* FIXME: add support for recvfrom */
    /* FIXME: add support for recvmsg */
    /* FIXME: add support for remap_file_pages */
    { "remove", explain_syscall_remove },
    /* FIXME: add support for removexattr */
    { "rename", explain_syscall_rename },
    /* FIXME: add support for request_key */
    /* FIXME: add support for restart_syscall */
    /* FIXME: add support for rewind */
    { "rmdir", explain_syscall_rmdir },
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
    { "select", explain_syscall_select },
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
    { "setgroups", explain_syscall_setgroups },
    { "sethostname", explain_syscall_sethostname },
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
    { "setsockopt", explain_syscall_setsockopt },
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
    { "signalfd", explain_syscall_signalfd },
    /* FIXME: add support for sigpending */
    /* FIXME: add support for sigprocmask */
    /* FIXME: add support for sigreturn */
    /* FIXME: add support for sigsuspend */
    { "socket", explain_syscall_socket },
    /* FIXME: add support for socketcall */
    /* FIXME: add support for socketpair */
    /* FIXME: add support for splice */
    /* FIXME: add support for sprintf */
    /* FIXME: add support for spu_create */
    /* FIXME: add support for spu_run */
    /* FIXME: add support for sscanf */
    /* FIXME: add support for ssetmask */
    { "stat", explain_syscall_stat },
    { "statfs", explain_syscall_statfs },
    /* FIXME: add support for stime */
    { "strdup", explain_syscall_strdup },
    { "strerror", explain_syscall_strerror },
    { "strndup", explain_syscall_strndup },
    { "strtod", explain_syscall_strtod },
    { "strtof", explain_syscall_strtof },
    { "strtol", explain_syscall_strtol },
    { "strtold", explain_syscall_strtold },
    { "strtoll", explain_syscall_strtoll },
    { "strtoul", explain_syscall_strtoul },
    { "strtoull", explain_syscall_strtoull },
    /* FIXME: add support for stty */
    /* FIXME: add support for swapoff */
    /* FIXME: add support for swapon */
    { "symlink", explain_syscall_symlink },
    /* FIXME: add support for sync */
    /* FIXME: add support for sync_file_range */
    /* FIXME: add support for sysfs */
    /* FIXME: add support for sysinfo */
    /* FIXME: add support for syslog */
    { "system", explain_syscall_system },
    /* ----------  T  ------------------------------------------------------- */
    /* FIXME: add support for tee */
    /* FIXME: add support for telldir */
    /* FIXME: add support for tempnam */
    /* FIXME: add support for tgkill */
    { "time", explain_syscall_time },
    /* FIXME: add support for timer_create */
    /* FIXME: add support for timer_delete */
    { "timerfd_create", explain_syscall_timerfd_create },
    /* FIXME: add support for timerfd_gettime */
    /* FIXME: add support for timerfd_settime */
    /* FIXME: add support for timer_getoverrun */
    /* FIXME: add support for timer_gettime */
    /* FIXME: add support for timer_settime */
    /* FIXME: add support for times */
    /* FIXME: add support for tkill */
    /* FIXME: add support for tmpfile */
    /* FIXME: add support for tmpnam */
    { "truncate", explain_syscall_truncate },
    /* FIXME: add support for tuxcall */
    /* ----------  U  ------------------------------------------------------- */
    /* FIXME: add support for ugetrlimit */
    /* FIXME: add support for ulimit */
    /* FIXME: add support for umount */
    /* FIXME: add support for uname */
    /* FIXME: add support for ungetc */
    { "unlink", explain_syscall_unlink },
    /* FIXME: add support for unshare */
    /* FIXME: add support for uselib */
    /* FIXME: add support for ustat */
    { "utime", explain_syscall_utime },
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
    { "wait", explain_syscall_wait },
    { "wait3", explain_syscall_wait3 },
    { "wait4", explain_syscall_wait4 },
    /* FIXME: add support for waitid */
    { "waitpid", explain_syscall_waitpid },
    { "write", explain_syscall_write },
    /* FIXME: add support for writev */
    /* ----------  X  ------------------------------------------------------- */
    /* ----------  Y  ------------------------------------------------------- */
    /* ----------  Z  ------------------------------------------------------- */
};


func_t
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

        weight = explain_fstrcmp(name, tp->name);
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
            "function \"%s\" unknown, did you mean the \"%s\" funtion?\n",
            name,
            best_tp->name
        );
        exit(EXIT_FAILURE);
        return best_tp->func;
    }
    else
        fprintf(stderr, "function \"%s\" unknown\n", name);
    exit(EXIT_FAILURE);
    return 0;
}