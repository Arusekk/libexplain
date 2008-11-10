dnl!
dnl! libexplain - Explain errno values returned by libc functions
dnl! Copyright (C) 2008 Peter Miller
dnl! Written by Peter Miller <millerp@canb.auug.org.au>
dnl!
dnl! This program is free software; you can redistribute it and/or
dnl! modify it under the terms of the GNU General Public License as
dnl! published by the Free Software Foundation; either version 3 of the
dnl! License, or (at your option) any later version.
dnl!
dnl! This program is distributed in the hope that it will be useful,
dnl! but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl! General Public License for more details.
dnl!
dnl! You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
dnl!
AC_INIT(install-sh)
AC_CONFIG_HEADER(libexplain/config.h)
AC_PROG_CC
AC_PROG_CPP
AC_CANONICAL_HOST
AC_GNU_SOURCE
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_YACC
AC_MINIX
AC_ISC_POSIX
AC_OBJEXT
AC_EXEEXT

dnl
dnl Large file support.
dnl
AC_FUNC_FSEEKO
AC_SYS_LARGEFILE

dnl
dnl we need to check for libiconv before we check for libintl
dnl
AC_CHECK_LIB(iconv, iconv, ,
    unset ac_cv_func_iconv)

dnl
dnl Solaris needs to use the GNU Gettext to have any chance of
dnl having working error message translation.  So we look for -lintl
dnl unconditionally.
dnl
AC_CHECK_LIB(intl, gettext, ,
  [
    unset ac_cv_func_gettext
    AC_CHECK_LIB(gettext, gettext, ,
      unset ac_cv_func_gettext)
  ]
)

AC_CHECK_PROGS(LIBTOOL, libtool)

if test -z "$LIBTOOL"
then
  AC_MSG_RESULT([
        You must have GNU Libtool installed to build libexplain.
        Homepage: http://www.gnu.org/software/libtool/])
  OK=no
  if apt-get --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo apt-get install libtool
    ])
    OK=yes
  fi
  if yum --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo yum install libtool
    ])
    OK=yes
  fi
  if test "$OK" != "yes"; then
    AC_MSG_RESULT([
        If you are using a package based install, you will need the
        libtool package.
    ])
  fi
  exit 1
fi

AC_CHECK_PROGS(LSOF, lsof)

if test -z "$LSOF"
then
  AC_MSG_RESULT([
        You must have lsof(1) installed to use libexplain.
        Homepage: http://people.freebsd.org/~abe/])
  OK=no
  if apt-get --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo apt-get install lsof
    ])
    OK=yes
  fi
  if yum --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo yum install lsof
    ])
    OK=yes
  fi
  if test "$OK" != "yes"; then
    AC_MSG_RESULT([
        If you are using a package based install, you will need the
        lsof package.
    ])
  fi
  exit 1
fi

if test -L /proc/self/fd/0 2> /dev/null
then
    AC_DEFINE(PROC_FS_USEFUL, 1,
    [Define this symbol if your system has symbolic links containing the
     absolute path of open files in /proc/self/fd/nnn])
fi

AC_CHECK_PROGS(MSGFMT, gmsgfmt msgfmt)
AC_CHECK_PROGS(MSGCAT, gmsgcat msgcat)
AC_CHECK_PROGS(GROFF, groff roff)
AC_CHECK_PROGS(SOELIM, gsoelim soelim)
AC_CHECK_PROGS(REFER, refer grefer)

dnl
dnl We need some way to determine the value of the default MANPATH
dnl setting, so that we can append to it.  There are (at least) two
dnl incompatible man implementations out there.  The one used by Debian
dnl Linux has a "manpath -q" command, the one used by RedHat Linux has
dnl a "man -w" command.  If neither works, assume a constant string.
dnl
AC_CHECK_PROGS(MANPATH_PROG, manpath)

if test -n "$MANPATH_PROG"
then
    dnl
    dnl Sometimes manpath prints irritating warnings,
    dnl but not all versions have -q to silence them.
    dnl
    MANPATH_PROG="$MANPATH_PROG 2>/dev/null"
else
    dnl
    dnl Some versions of man have a -w option, but it's short for --whatis
    dnl and so they print nothing on stdout, and an error message about
    dnl the missing argument on stderr.  Some versions of man -w do the
    dnl same thing as manpath.
    dnl
    temp_str=`man -w 2>/dev/null`
    if test -n "$temp_str"
    then
        MANPATH_PROG="man -w 2>/dev/null"
    else
        MANPATH_PROG="echo /usr/man:/usr/share/man"
    fi
fi

AC_CHECK_PROGS(LOCALE_PROG, locale, , [${PATH}:/usr/sbin:/sbin])
if test -n "$LOCALE_PROG"
then
    AC_DEFINE(HAVE_LOCALE_PROG, 1,
    [Define this symbol if your system has the locale(1) program.])
fi

AC_CHECK_PROGS(LOCALE_GEN_PROG, locale-gen, , [${PATH}:/usr/sbin:/sbin])
if test -n "$LOCALE_GEN_PROG"
then
    AC_DEFINE(HAVE_LOCALE_GEN_PROG, 1,
    [Define this symbol if your system has the locale-gen(8) program.])
fi

dnl
dnl     Test if groff takes -ms or -mgs for the macro package.
dnl
AC_MSG_CHECKING([for $GROFF -ms macros])
if test `echo ' ' | groff -mgs 2> /dev/null | wc -l` -gt 0
then
    GROFF_MS_MACROS=gs
else
    GROFF_MS_MACROS=s
fi
AC_SUBST(GROFF_MS_MACROS)
AC_MSG_RESULT([-m$GROFF_MS_MACROS])

dnl
dnl     Test if groff takes -mm or -mgm for the macro package.
dnl
AC_MSG_CHECKING([for $GROFF -mm macros])
if test `echo ' ' | groff -mgm 2> /dev/null | wc -l` -gt 0
then
    GROFF_MM_MACROS=gm
else
    GROFF_MM_MACROS=m
fi
AC_SUBST(GROFF_MM_MACROS)
AC_MSG_RESULT([-m$GROFF_MM_MACROS])

AC_CHECK_HEADERS(        \
    fcntl.h              \
    libintl.h            \
    limits.h             \
    mntent.h             \
    pwd.h                \
    stddef.h             \
    stdlib.h             \
    string.h             \
    sys/capability.h     \
    sys/file.h           \
    sys/mount.h          \
    sys/param.h          \
    sys/time.h           \
    sys/types.h          \
    sys/stat.h           \
    sys/vfs.h            \
    termios.h            \
    time.h               \
    unistd.h             \
    )

if test "$HAVE_MNTENT_H" != "1"
then
    _PATH_MNTTAB=/dev/null
    test -f /etc/fstab && _PATH_MNTTAB=/etc/fstab
    test -f /etc/vfstab && _PATH_MNTTAB=/etc/vfstab # Solaris
    AC_DEFINE_UNQUOTED(_PATH_MNTTAB, ["${_PATH_MNTTAB}"],
    [File listing canonical interesting mount points.])

    _PATH_MOUNTED=${_PATH_MNTTAB}
    # OpenBSD doesn't have /etc/mtab
    test -f /etc/mtab && _PATH_MOUNTED=/etc/mtab
    AC_DEFINE_UNQUOTED(_PATH_MOUNTED, ["${_PATH_MOUNTED}"],
    [File listing currently active mount points.])
fi

AC_SEARCH_LIBS(cap_get_proc, cap)

AC_HEADER_DIRENT
AC_TYPE_SIGNAL
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_C_CONST
AC_SYS_LONG_FILE_NAMES
AC_CHECK_FUNCS(  \
    cap_get_proc \
    gettext      \
    setlocale    \
    remove       \
    snprintf     \
    strcasecmp   \
    strendcpy    \
    strerror     \
    strncasecmp  \
    strsignal    \
    vsnprintf    \
    )

dnl
dnl     Test to see where TIOCGWINSZ is defined.
dnl
AC_MSG_CHECKING([for TIOCGWINSZ])
AC_TRY_RUN([
#include <sys/ioctl.h>
int main(argc, argv)int argc; char **argv;{
#ifdef TIOCGWINSZ
struct winsize ws; ws.ws_col = 0; exit(ws.ws_col);
#else
exit(1);
#endif
}]
, AC_DEFINE(HAVE_winsize_SYS_IOCTL_H,1,
[Define this symbol if your system as <sys/ioctl.h> AND it
defines TIOCGWINSZ and struct winsize.])
AC_MSG_RESULT([sys/ioctl.h]),
AC_MSG_RESULT([not found]),
AC_MSG_RESULT(cross))

dnl the strsignal function is usually defined in <string.h>
dnl but is sometimes defined in <unistd.h>
AC_CHECK_DECLS([strsignal],,, [
#ifndef __USE_GNU
#define __USE_GNU
#endif
#if HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
])
AC_CHECK_DECLS([strverscmp])

dnl
dnl   Test to find a Bourne shell which understands functions
dnl
AC_MSG_CHECKING([for a Bourne shell which understands functions])
if test "z$SH" = "z"; then
    if test -f /bin/sh5; then
        SH=/bin/sh5
    else
        SH=/bin/sh
    fi
fi
AC_SUBST(SH)
AC_DEFINE_UNQUOTED(CONF_SHELL, ["$SH"],
[Set this to be the absolute path of a Bourne shell
which understands functions.])
AC_MSG_RESULT($SH)

dnl
dnl Evaluate some of the variables, to remove ${prefix} references.
dnl This way, they can be used in C programs and Roff input.
dnl Make sure that libexplain is mentioned in the libdir and datadir paths;
dnl add it if it is not already there.
dnl
test "x$prefix" = xNONE && prefix="${ac_default_prefix-/usr/local}"
test "x$exec_prefix" = xNONE && exec_prefix="$prefix"
eval "bindir=$bindir"
eval "datarootdir=$datarootdir"
eval "includedir=$includedir"
eval "datadir=$datadir"
eval "libdir=$libdir"
eval "mandir=$mandir"
eval "sharedstatedir=$sharedstatedir"
case "$datadir" in
*/libexplain/* | */libexplain )
        ;;
*)
        datadir="${datadir}/libexplain"
        ;;
esac
case "$datarootdir" in
*/libexplain/* | */libexplain )
        ;;
*)
        datarootdir="${datarootdir}/libexplain"
        ;;
esac
case "$sharedstatedir" in
*/libexplain/* | */libexplain )
        ;;
*)
        sharedstatedir="${sharedstatedir}/libexplain"
        ;;
esac
eval "sysconfdir=$sysconfdir"

dnl support for NLSDIR option
AC_ARG_WITH(nlsdir,
        [  --with-nlsdir=PATH   specify where the locale stuff should go ])

if test "x$NLSDIR" = "x"; then
        if test "x$with_nlsdir" != "x"; then
                NLSDIR=$with_nlsdir
        else
                NLSDIR=${libdir}
        fi
fi

AC_SUBST(NLSDIR)

dnl
dnl     If the gettext function exists, assume the msgfmt program does
dnl     also, and arrange for the Makefile to install the .mo files
dnl     from the .po files.
dnl
po_files=no
if test "$ac_cv_func_gettext" = "yes"
then
  if test -n "$MSGFMT"
  then
    po_files=yes
  fi
fi
if test "$po_files" = "no"
then
  AC_MSG_RESULT([
        Libexplain will emit much more useful error messages if
        you install GNU Gettext and then re-run this
        ./configure script.
        Remember to use \`\`rm config.cache'' before you do.
        Homepage: http://www.gnu.org/software/gettext/])
  OK=no
  if apt-get --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo apt-get install gettext
    ])
    OK=yes
  fi
  if yum --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo yum install gettext
    ])
    OK=yes
  fi
  if test "$OK" != "yes"; then
    AC_MSG_RESULT([
        If you are using a package based install, you will need the
        gettext package.
    ])
  fi
fi
AC_SUBST(po_files)

dnl
dnl     If the soelim program exists, and understands the -I option,
dnl     arrange for the Makefile to install the .ps files from the
dnl     documentation source files.
dnl
if test -n "$SOELIM"
then
  if $SOELIM -I. /dev/null > /dev/null 2>&1
  then
    : nothing
  else
    AC_MSG_RESULT([
        The $SOELIM program does not understand the -I option.
        GNU Groff 1.15 or later works correctly.
        See the BUILDING file for details.])
    GROFF=
  fi
else
  GROFF=
fi
if test -n "$REFER"
then
    : nothing
else
    # It turns out that somehow Debian systems get a half-assed groff
    # install from build-essential.  Looking for "refer" is a better
    # test of the completeness of the groff install than either
    # "groff" or "soelim" on their own, plus it's needed for the build.
    GROFF=
fi
if test -n "$GROFF"
then
  HAVE_GROFF=yes
else
  HAVE_GROFF=no
  AC_MSG_RESULT([
        The libexplain documentation set and manual pages will
        be formatted and installed if you first install
        GNU Groff 1.15 or later and then re-run this ./configure script.
        Remember to use \`\`rm config.cache'' before you do.
        Homepage: http://www.gnu.org/software/groff/])
  OK=no
  if apt-get --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo apt-get install groff
    ])
    OK=yes
  fi
  if yum --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo yum install groff
    ])
    OK=yes
  fi
  if test "$OK" != "yes"; then
    AC_MSG_RESULT([
        If you are using a package based install, you will need the
        groff package.
    ])
  fi
fi
AC_SUBST(HAVE_GROFF)

# program prefix is the bit to add to the start of the name
if test ${PROGRAM_PREFIX-NONE} != NONE -a ${program_prefix-NONE} != NONE; then
PROGRAM_PREFIX=${program_prefix}
fi
AC_SUBST(PROGRAM_PREFIX)

# program suffix is the bit to add to the end of the name (before .exe)
if test ${PROGRAM_SUFFIX-NONE} != NONE -a ${program_suffix-NONE} != NONE; then
PROGRAM_SUFFIX=${program_suffix}
fi
AC_SUBST(PROGRAM_SUFFIX)

AH_TOP(
[/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEXPLAIN_CONFIG_H
#define LIBEXPLAIN_CONFIG_H
])

AH_BOTTOM([
/*
 * There is more to do, but we need to insulate it from config.status,
 * because it screws up the #undef lines.  They are all implications of
 * the above information, so there is no need for you to edit the file,
 * if you are configuring libexplain manually.
 */
#include <libexplain/config.messy.h>

#endif /* LIBEXPLAIN_CONFIG_H */])

AC_OUTPUT(Makefile)
