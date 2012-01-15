#
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#
#    W   W    A    RRRR   N   N   III  N   N  III  N   N   GGG
#    W   W   A A   R   R  NN  N    I   NN  N   I   NN  N  G   G
#    W   W  A   A  RRRR   N N N    I   N N N   I   N N N  G
#    W W W  AAAAA  R R    N  NN    I   N  NN   I   N  NN  G  GG
#    W W W  A   A  R  R   N   N    I   N   N   I   N   N  G   G
#     W W   A   A  R   R  N   N   III  N   N  III  N   N   GGG
#
# Warning: DO NOT send patches which fix this file. IT IS NOT the original
# source file. This file is GENERATED from the Aegis repository file manifest.
# If you find a bug in this file, it could well be an Aegis bug.
#
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#
Summary: library of syscall-specific strerror replacements
Name: libexplain
Version: 0.50.D001
Release: 1
License: LGPL, GPL
Group: Development/Tools
Source: http://libexplain.sourceforge.net/%{name}-%{version}.tar.gz
URL: http://libexplain.sourceforge.net/
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires: bison
BuildRequires: diffutils
BuildRequires: glibc-devel
BuildRequires: groff
BuildRequires: libcap-devel
BuildRequires: libtool
BuildRequires: lsof
BuildRequires: sharutils

%description
This package provides a library which may be used to explain Unix and Linux system call errors.  The library is not quite a drop-in replacement for strerror, but it comes close, with each system call having a dedicated libexplain function.

%package bin
Summary: libexplain programs

%description bin
This package provides a command to explain Unix and Linux system call errors, after the fact.

%package libs
Summary: libexplain libraries
Group: Development/Tools

%description libs
This package provides a library which may be used to explain Unix and Linux system call errors.  The library is not quite a drop-in replacement for strerror, but it comes close, with each system call having a dedicated libexplain function.

%package devel
Summary: libexplain development files
Group: Development/Tools
Requires: srecord-libs%{?_isa} = %{?epoch:%{epoch}:}%{version}-%{release}

%description devel
This package contains static libraries and header files for compiling applications that use libexplain to explain Unix and Linux system call errors.

%post
/sbin/ldconfig

%postun
/sbin/ldconfig


%prep
%setup -q


%build
%configure
make


%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install
rm -f $RPM_BUILD_ROOT/usr/lib*/*.la


%check
make sure


%clean
rm -rf $RPM_BUILD_ROOT


%files bin
%defattr (-,root,root,-)
%doc LICENSE BUILDING README
%{_bindir}/explain
%{_mandir}/man1/explain.1.gz
%{_mandir}/man1/explain_lca2010.1.gz
%{_mandir}/man1/explain_license.1.gz


%files libs
%defattr (-,root,root,-)
%{_libdir}/libexplain.so*


%files devel
%defattr (-,root,root,-)
%{_includedir}/libexplain
%{_libdir}/libexplain.a
%{_libdir}/pkgconfig/libexplain.pc
%{_mandir}/man3/explain.3.gz
%{_mandir}/man3/explain_accept.3.gz
%{_mandir}/man3/explain_accept4.3.gz
%{_mandir}/man3/explain_accept4_or_die.3.gz
%{_mandir}/man3/explain_accept_or_die.3.gz
%{_mandir}/man3/explain_access.3.gz
%{_mandir}/man3/explain_access_or_die.3.gz
%{_mandir}/man3/explain_acct.3.gz
%{_mandir}/man3/explain_acct_or_die.3.gz
%{_mandir}/man3/explain_adjtime.3.gz
%{_mandir}/man3/explain_adjtime_or_die.3.gz
%{_mandir}/man3/explain_adjtimex.3.gz
%{_mandir}/man3/explain_adjtimex_or_die.3.gz
%{_mandir}/man3/explain_bind.3.gz
%{_mandir}/man3/explain_bind_or_die.3.gz
%{_mandir}/man3/explain_calloc.3.gz
%{_mandir}/man3/explain_calloc_or_die.3.gz
%{_mandir}/man3/explain_chdir.3.gz
%{_mandir}/man3/explain_chdir_or_die.3.gz
%{_mandir}/man3/explain_chmod.3.gz
%{_mandir}/man3/explain_chmod_or_die.3.gz
%{_mandir}/man3/explain_chown.3.gz
%{_mandir}/man3/explain_chown_or_die.3.gz
%{_mandir}/man3/explain_chroot.3.gz
%{_mandir}/man3/explain_chroot_or_die.3.gz
%{_mandir}/man3/explain_close.3.gz
%{_mandir}/man3/explain_close_or_die.3.gz
%{_mandir}/man3/explain_closedir.3.gz
%{_mandir}/man3/explain_closedir_or_die.3.gz
%{_mandir}/man3/explain_connect.3.gz
%{_mandir}/man3/explain_connect_or_die.3.gz
%{_mandir}/man3/explain_creat.3.gz
%{_mandir}/man3/explain_creat_or_die.3.gz
%{_mandir}/man3/explain_dirfd.3.gz
%{_mandir}/man3/explain_dirfd_or_die.3.gz
%{_mandir}/man3/explain_dup.3.gz
%{_mandir}/man3/explain_dup2.3.gz
%{_mandir}/man3/explain_dup2_or_die.3.gz
%{_mandir}/man3/explain_dup_or_die.3.gz
%{_mandir}/man3/explain_eventfd.3.gz
%{_mandir}/man3/explain_eventfd_or_die.3.gz
%{_mandir}/man3/explain_execlp.3.gz
%{_mandir}/man3/explain_execlp_or_die.3.gz
%{_mandir}/man3/explain_execve.3.gz
%{_mandir}/man3/explain_execve_or_die.3.gz
%{_mandir}/man3/explain_execvp.3.gz
%{_mandir}/man3/explain_execvp_or_die.3.gz
%{_mandir}/man3/explain_exit.3.gz
%{_mandir}/man3/explain_fchdir.3.gz
%{_mandir}/man3/explain_fchdir_or_die.3.gz
%{_mandir}/man3/explain_fchmod.3.gz
%{_mandir}/man3/explain_fchmod_or_die.3.gz
%{_mandir}/man3/explain_fchown.3.gz
%{_mandir}/man3/explain_fchown_or_die.3.gz
%{_mandir}/man3/explain_fclose.3.gz
%{_mandir}/man3/explain_fclose_or_die.3.gz
%{_mandir}/man3/explain_fcntl.3.gz
%{_mandir}/man3/explain_fcntl_or_die.3.gz
%{_mandir}/man3/explain_fdopen.3.gz
%{_mandir}/man3/explain_fdopen_or_die.3.gz
%{_mandir}/man3/explain_fdopendir.3.gz
%{_mandir}/man3/explain_fdopendir_or_die.3.gz
%{_mandir}/man3/explain_feof.3.gz
%{_mandir}/man3/explain_feof_or_die.3.gz
%{_mandir}/man3/explain_ferror.3.gz
%{_mandir}/man3/explain_ferror_or_die.3.gz
%{_mandir}/man3/explain_fflush.3.gz
%{_mandir}/man3/explain_fflush_or_die.3.gz
%{_mandir}/man3/explain_fgetc.3.gz
%{_mandir}/man3/explain_fgetc_or_die.3.gz
%{_mandir}/man3/explain_fgetpos.3.gz
%{_mandir}/man3/explain_fgetpos_or_die.3.gz
%{_mandir}/man3/explain_fgets.3.gz
%{_mandir}/man3/explain_fgets_or_die.3.gz
%{_mandir}/man3/explain_fileno.3.gz
%{_mandir}/man3/explain_fileno_or_die.3.gz
%{_mandir}/man3/explain_flock.3.gz
%{_mandir}/man3/explain_flock_or_die.3.gz
%{_mandir}/man3/explain_fopen.3.gz
%{_mandir}/man3/explain_fopen_or_die.3.gz
%{_mandir}/man3/explain_fork.3.gz
%{_mandir}/man3/explain_fork_or_die.3.gz
%{_mandir}/man3/explain_fpathconf.3.gz
%{_mandir}/man3/explain_fpathconf_or_die.3.gz
%{_mandir}/man3/explain_fprintf.3.gz
%{_mandir}/man3/explain_fprintf_or_die.3.gz
%{_mandir}/man3/explain_fpurge.3.gz
%{_mandir}/man3/explain_fpurge_or_die.3.gz
%{_mandir}/man3/explain_fputc.3.gz
%{_mandir}/man3/explain_fputc_or_die.3.gz
%{_mandir}/man3/explain_fputs.3.gz
%{_mandir}/man3/explain_fputs_or_die.3.gz
%{_mandir}/man3/explain_fread.3.gz
%{_mandir}/man3/explain_fread_or_die.3.gz
%{_mandir}/man3/explain_freopen.3.gz
%{_mandir}/man3/explain_freopen_or_die.3.gz
%{_mandir}/man3/explain_fseek.3.gz
%{_mandir}/man3/explain_fseek_or_die.3.gz
%{_mandir}/man3/explain_fsetpos.3.gz
%{_mandir}/man3/explain_fsetpos_or_die.3.gz
%{_mandir}/man3/explain_fstat.3.gz
%{_mandir}/man3/explain_fstat_or_die.3.gz
%{_mandir}/man3/explain_fstatfs.3.gz
%{_mandir}/man3/explain_fstatfs_or_die.3.gz
%{_mandir}/man3/explain_fstatvfs.3.gz
%{_mandir}/man3/explain_fstatvfs_or_die.3.gz
%{_mandir}/man3/explain_fsync.3.gz
%{_mandir}/man3/explain_fsync_or_die.3.gz
%{_mandir}/man3/explain_ftell.3.gz
%{_mandir}/man3/explain_ftell_or_die.3.gz
%{_mandir}/man3/explain_ftime.3.gz
%{_mandir}/man3/explain_ftime_or_die.3.gz
%{_mandir}/man3/explain_ftruncate.3.gz
%{_mandir}/man3/explain_ftruncate_or_die.3.gz
%{_mandir}/man3/explain_futimes.3.gz
%{_mandir}/man3/explain_futimes_or_die.3.gz
%{_mandir}/man3/explain_fwrite.3.gz
%{_mandir}/man3/explain_fwrite_or_die.3.gz
%{_mandir}/man3/explain_getaddrinfo.3.gz
%{_mandir}/man3/explain_getaddrinfo_or_die.3.gz
%{_mandir}/man3/explain_getc.3.gz
%{_mandir}/man3/explain_getc_or_die.3.gz
%{_mandir}/man3/explain_getchar.3.gz
%{_mandir}/man3/explain_getchar_or_die.3.gz
%{_mandir}/man3/explain_getcwd.3.gz
%{_mandir}/man3/explain_getcwd_or_die.3.gz
%{_mandir}/man3/explain_getdomainname.3.gz
%{_mandir}/man3/explain_getdomainname_or_die.3.gz
%{_mandir}/man3/explain_getgroups.3.gz
%{_mandir}/man3/explain_getgroups_or_die.3.gz
%{_mandir}/man3/explain_gethostname.3.gz
%{_mandir}/man3/explain_gethostname_or_die.3.gz
%{_mandir}/man3/explain_getpeername.3.gz
%{_mandir}/man3/explain_getpeername_or_die.3.gz
%{_mandir}/man3/explain_getpgid.3.gz
%{_mandir}/man3/explain_getpgid_or_die.3.gz
%{_mandir}/man3/explain_getpgrp.3.gz
%{_mandir}/man3/explain_getpgrp_or_die.3.gz
%{_mandir}/man3/explain_getrlimit.3.gz
%{_mandir}/man3/explain_getrlimit_or_die.3.gz
%{_mandir}/man3/explain_getsockname.3.gz
%{_mandir}/man3/explain_getsockname_or_die.3.gz
%{_mandir}/man3/explain_getsockopt.3.gz
%{_mandir}/man3/explain_getsockopt_or_die.3.gz
%{_mandir}/man3/explain_gettimeofday.3.gz
%{_mandir}/man3/explain_gettimeofday_or_die.3.gz
%{_mandir}/man3/explain_getw.3.gz
%{_mandir}/man3/explain_getw_or_die.3.gz
%{_mandir}/man3/explain_ioctl.3.gz
%{_mandir}/man3/explain_ioctl_or_die.3.gz
%{_mandir}/man3/explain_kill.3.gz
%{_mandir}/man3/explain_kill_or_die.3.gz
%{_mandir}/man3/explain_lchown.3.gz
%{_mandir}/man3/explain_lchown_or_die.3.gz
%{_mandir}/man3/explain_license.3.gz
%{_mandir}/man3/explain_link.3.gz
%{_mandir}/man3/explain_link_or_die.3.gz
%{_mandir}/man3/explain_listen.3.gz
%{_mandir}/man3/explain_listen_or_die.3.gz
%{_mandir}/man3/explain_lseek.3.gz
%{_mandir}/man3/explain_lseek_or_die.3.gz
%{_mandir}/man3/explain_lstat.3.gz
%{_mandir}/man3/explain_lstat_or_die.3.gz
%{_mandir}/man3/explain_malloc.3.gz
%{_mandir}/man3/explain_malloc_or_die.3.gz
%{_mandir}/man3/explain_mkdir.3.gz
%{_mandir}/man3/explain_mkdir_or_die.3.gz
%{_mandir}/man3/explain_mkdtemp.3.gz
%{_mandir}/man3/explain_mkdtemp_or_die.3.gz
%{_mandir}/man3/explain_mknod.3.gz
%{_mandir}/man3/explain_mknod_or_die.3.gz
%{_mandir}/man3/explain_mkostemp.3.gz
%{_mandir}/man3/explain_mkostemp_or_die.3.gz
%{_mandir}/man3/explain_mkstemp.3.gz
%{_mandir}/man3/explain_mkstemp_or_die.3.gz
%{_mandir}/man3/explain_mktemp.3.gz
%{_mandir}/man3/explain_mktemp_or_die.3.gz
%{_mandir}/man3/explain_mmap.3.gz
%{_mandir}/man3/explain_mmap_or_die.3.gz
%{_mandir}/man3/explain_munmap.3.gz
%{_mandir}/man3/explain_munmap_or_die.3.gz
%{_mandir}/man3/explain_nice.3.gz
%{_mandir}/man3/explain_nice_or_die.3.gz
%{_mandir}/man3/explain_open.3.gz
%{_mandir}/man3/explain_open_or_die.3.gz
%{_mandir}/man3/explain_opendir.3.gz
%{_mandir}/man3/explain_opendir_or_die.3.gz
%{_mandir}/man3/explain_output.3.gz
%{_mandir}/man3/explain_pathconf.3.gz
%{_mandir}/man3/explain_pathconf_or_die.3.gz
%{_mandir}/man3/explain_pclose.3.gz
%{_mandir}/man3/explain_pclose_or_die.3.gz
%{_mandir}/man3/explain_pipe.3.gz
%{_mandir}/man3/explain_pipe_or_die.3.gz
%{_mandir}/man3/explain_poll.3.gz
%{_mandir}/man3/explain_poll_or_die.3.gz
%{_mandir}/man3/explain_popen.3.gz
%{_mandir}/man3/explain_popen_or_die.3.gz
%{_mandir}/man3/explain_pread.3.gz
%{_mandir}/man3/explain_pread_or_die.3.gz
%{_mandir}/man3/explain_printf.3.gz
%{_mandir}/man3/explain_printf_or_die.3.gz
%{_mandir}/man3/explain_program_name.3.gz
%{_mandir}/man3/explain_ptrace.3.gz
%{_mandir}/man3/explain_ptrace_or_die.3.gz
%{_mandir}/man3/explain_putc.3.gz
%{_mandir}/man3/explain_putc_or_die.3.gz
%{_mandir}/man3/explain_putchar.3.gz
%{_mandir}/man3/explain_putchar_or_die.3.gz
%{_mandir}/man3/explain_putenv.3.gz
%{_mandir}/man3/explain_putenv_or_die.3.gz
%{_mandir}/man3/explain_puts.3.gz
%{_mandir}/man3/explain_puts_or_die.3.gz
%{_mandir}/man3/explain_putw.3.gz
%{_mandir}/man3/explain_putw_or_die.3.gz
%{_mandir}/man3/explain_pwrite.3.gz
%{_mandir}/man3/explain_pwrite_or_die.3.gz
%{_mandir}/man3/explain_raise.3.gz
%{_mandir}/man3/explain_raise_or_die.3.gz
%{_mandir}/man3/explain_read.3.gz
%{_mandir}/man3/explain_read_or_die.3.gz
%{_mandir}/man3/explain_readdir.3.gz
%{_mandir}/man3/explain_readdir_or_die.3.gz
%{_mandir}/man3/explain_readlink.3.gz
%{_mandir}/man3/explain_readlink_or_die.3.gz
%{_mandir}/man3/explain_readv.3.gz
%{_mandir}/man3/explain_readv_or_die.3.gz
%{_mandir}/man3/explain_realloc.3.gz
%{_mandir}/man3/explain_realloc_or_die.3.gz
%{_mandir}/man3/explain_realpath.3.gz
%{_mandir}/man3/explain_realpath_or_die.3.gz
%{_mandir}/man3/explain_remove.3.gz
%{_mandir}/man3/explain_remove_or_die.3.gz
%{_mandir}/man3/explain_rename.3.gz
%{_mandir}/man3/explain_rename_or_die.3.gz
%{_mandir}/man3/explain_rmdir.3.gz
%{_mandir}/man3/explain_rmdir_or_die.3.gz
%{_mandir}/man3/explain_select.3.gz
%{_mandir}/man3/explain_select_or_die.3.gz
%{_mandir}/man3/explain_setbuf.3.gz
%{_mandir}/man3/explain_setbuf_or_die.3.gz
%{_mandir}/man3/explain_setbuffer.3.gz
%{_mandir}/man3/explain_setbuffer_or_die.3.gz
%{_mandir}/man3/explain_setdomainname.3.gz
%{_mandir}/man3/explain_setdomainname_or_die.3.gz
%{_mandir}/man3/explain_setenv.3.gz
%{_mandir}/man3/explain_setenv_or_die.3.gz
%{_mandir}/man3/explain_setgroups.3.gz
%{_mandir}/man3/explain_setgroups_or_die.3.gz
%{_mandir}/man3/explain_sethostname.3.gz
%{_mandir}/man3/explain_sethostname_or_die.3.gz
%{_mandir}/man3/explain_setlinebuf.3.gz
%{_mandir}/man3/explain_setlinebuf_or_die.3.gz
%{_mandir}/man3/explain_setpgid.3.gz
%{_mandir}/man3/explain_setpgid_or_die.3.gz
%{_mandir}/man3/explain_setpgrp.3.gz
%{_mandir}/man3/explain_setpgrp_or_die.3.gz
%{_mandir}/man3/explain_setsid.3.gz
%{_mandir}/man3/explain_setsid_or_die.3.gz
%{_mandir}/man3/explain_setsockopt.3.gz
%{_mandir}/man3/explain_setsockopt_or_die.3.gz
%{_mandir}/man3/explain_setvbuf.3.gz
%{_mandir}/man3/explain_setvbuf_or_die.3.gz
%{_mandir}/man3/explain_shmat.3.gz
%{_mandir}/man3/explain_shmat_or_die.3.gz
%{_mandir}/man3/explain_shmctl.3.gz
%{_mandir}/man3/explain_shmctl_or_die.3.gz
%{_mandir}/man3/explain_signalfd.3.gz
%{_mandir}/man3/explain_signalfd_or_die.3.gz
%{_mandir}/man3/explain_snprintf.3.gz
%{_mandir}/man3/explain_snprintf_or_die.3.gz
%{_mandir}/man3/explain_socket.3.gz
%{_mandir}/man3/explain_socket_or_die.3.gz
%{_mandir}/man3/explain_socketpair.3.gz
%{_mandir}/man3/explain_socketpair_or_die.3.gz
%{_mandir}/man3/explain_sprintf.3.gz
%{_mandir}/man3/explain_sprintf_or_die.3.gz
%{_mandir}/man3/explain_stat.3.gz
%{_mandir}/man3/explain_stat_or_die.3.gz
%{_mandir}/man3/explain_statfs.3.gz
%{_mandir}/man3/explain_statfs_or_die.3.gz
%{_mandir}/man3/explain_statvfs.3.gz
%{_mandir}/man3/explain_statvfs_or_die.3.gz
%{_mandir}/man3/explain_stime.3.gz
%{_mandir}/man3/explain_stime_or_die.3.gz
%{_mandir}/man3/explain_strdup.3.gz
%{_mandir}/man3/explain_strdup_or_die.3.gz
%{_mandir}/man3/explain_strndup.3.gz
%{_mandir}/man3/explain_strndup_or_die.3.gz
%{_mandir}/man3/explain_strtod.3.gz
%{_mandir}/man3/explain_strtod_or_die.3.gz
%{_mandir}/man3/explain_strtof.3.gz
%{_mandir}/man3/explain_strtof_or_die.3.gz
%{_mandir}/man3/explain_strtol.3.gz
%{_mandir}/man3/explain_strtol_or_die.3.gz
%{_mandir}/man3/explain_strtold.3.gz
%{_mandir}/man3/explain_strtold_or_die.3.gz
%{_mandir}/man3/explain_strtoll.3.gz
%{_mandir}/man3/explain_strtoll_or_die.3.gz
%{_mandir}/man3/explain_strtoul.3.gz
%{_mandir}/man3/explain_strtoul_or_die.3.gz
%{_mandir}/man3/explain_strtoull.3.gz
%{_mandir}/man3/explain_strtoull_or_die.3.gz
%{_mandir}/man3/explain_symlink.3.gz
%{_mandir}/man3/explain_symlink_or_die.3.gz
%{_mandir}/man3/explain_system.3.gz
%{_mandir}/man3/explain_system_or_die.3.gz
%{_mandir}/man3/explain_tcdrain.3.gz
%{_mandir}/man3/explain_tcdrain_or_die.3.gz
%{_mandir}/man3/explain_tcflow.3.gz
%{_mandir}/man3/explain_tcflow_or_die.3.gz
%{_mandir}/man3/explain_tcflush.3.gz
%{_mandir}/man3/explain_tcflush_or_die.3.gz
%{_mandir}/man3/explain_tcgetattr.3.gz
%{_mandir}/man3/explain_tcgetattr_or_die.3.gz
%{_mandir}/man3/explain_tcsendbreak.3.gz
%{_mandir}/man3/explain_tcsendbreak_or_die.3.gz
%{_mandir}/man3/explain_tcsetattr.3.gz
%{_mandir}/man3/explain_tcsetattr_or_die.3.gz
%{_mandir}/man3/explain_telldir.3.gz
%{_mandir}/man3/explain_telldir_or_die.3.gz
%{_mandir}/man3/explain_tempnam.3.gz
%{_mandir}/man3/explain_tempnam_or_die.3.gz
%{_mandir}/man3/explain_time.3.gz
%{_mandir}/man3/explain_time_or_die.3.gz
%{_mandir}/man3/explain_timerfd_create.3.gz
%{_mandir}/man3/explain_timerfd_create_or_die.3.gz
%{_mandir}/man3/explain_tmpfile.3.gz
%{_mandir}/man3/explain_tmpfile_or_die.3.gz
%{_mandir}/man3/explain_tmpnam.3.gz
%{_mandir}/man3/explain_tmpnam_or_die.3.gz
%{_mandir}/man3/explain_truncate.3.gz
%{_mandir}/man3/explain_truncate_or_die.3.gz
%{_mandir}/man3/explain_ungetc.3.gz
%{_mandir}/man3/explain_ungetc_or_die.3.gz
%{_mandir}/man3/explain_unlink.3.gz
%{_mandir}/man3/explain_unlink_or_die.3.gz
%{_mandir}/man3/explain_unsetenv.3.gz
%{_mandir}/man3/explain_unsetenv_or_die.3.gz
%{_mandir}/man3/explain_ustat.3.gz
%{_mandir}/man3/explain_ustat_or_die.3.gz
%{_mandir}/man3/explain_utime.3.gz
%{_mandir}/man3/explain_utime_or_die.3.gz
%{_mandir}/man3/explain_utimes.3.gz
%{_mandir}/man3/explain_utimes_or_die.3.gz
%{_mandir}/man3/explain_vfork.3.gz
%{_mandir}/man3/explain_vfork_or_die.3.gz
%{_mandir}/man3/explain_vfprintf.3.gz
%{_mandir}/man3/explain_vfprintf_or_die.3.gz
%{_mandir}/man3/explain_vprintf.3.gz
%{_mandir}/man3/explain_vprintf_or_die.3.gz
%{_mandir}/man3/explain_vsnprintf.3.gz
%{_mandir}/man3/explain_vsnprintf_or_die.3.gz
%{_mandir}/man3/explain_vsprintf.3.gz
%{_mandir}/man3/explain_vsprintf_or_die.3.gz
%{_mandir}/man3/explain_wait.3.gz
%{_mandir}/man3/explain_wait3.3.gz
%{_mandir}/man3/explain_wait3_or_die.3.gz
%{_mandir}/man3/explain_wait4.3.gz
%{_mandir}/man3/explain_wait4_or_die.3.gz
%{_mandir}/man3/explain_wait_or_die.3.gz
%{_mandir}/man3/explain_waitpid.3.gz
%{_mandir}/man3/explain_waitpid_or_die.3.gz
%{_mandir}/man3/explain_write.3.gz
%{_mandir}/man3/explain_write_or_die.3.gz
%{_mandir}/man3/explain_writev.3.gz
%{_mandir}/man3/explain_writev_or_die.3.gz
