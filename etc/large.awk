#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2009-2011 Peter Miller
# Written by Peter Miller <pmiller@opensource.org.au>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>.
#
#
#     This script is used to reproduce a subset of the config.h file,
#     the parts that apply to large files, using a LIBEXPLAIN name
#     prefix, to avoid collisions with the user's program.
BEGIN {
    print "#ifndef LIBEXPLAIN_PUBLIC_CONFIG_H"
    print "#define LIBEXPLAIN_PUBLIC_CONFIG_H"
}
/_FILE_OFFSET_BITS/ {
    print ""
    line = $0

    # with GNU Awk we can say
    #   sub(/_FILE_OFFSET_BITS/, "LIBEXPLAIN_FILE_OFFSET_BITS", line)
    # but we have to do it the slow way
    n = index(line, "_FILE_OFFSET_BITS");
    if (n > 0)
    {
        line = substr(line, 1, n - 1) "LIBEXPLAIN" substr(line, n, 999)
    }

    print prev;
    print line;
    prev = "";
    next
}
/_LARGEFILE_SOURCE/ {
    print ""
    line = $0

    # with GNU Awk we can say
    #   sub(/_LARGEFILE_SOURCE/, "LIBEXPLAIN_LARGEFILE_SOURCE", line)
    # but we have to do it the slow way
    n = index(line, "_LARGEFILE_SOURCE");
    if (n > 0)
    {
        line = substr(line, 1, n - 1) "LIBEXPLAIN" substr(line, n, 999)
    }

    print prev;
    print line;
    prev = "";
    next
}
/_LARGE_FILES/ {
    print ""
    line = $0

    # with GNU Awk we can say
    #   sub(/_LARGE_FILES/, "LIBEXPLAIN_LARGE_FILES", line)
    # but we have to do it the slow way
    n = index(line, "_LARGE_FILES");
    if (n > 0)
    {
        line = substr(line, 1, n - 1) "LIBEXPLAIN" substr(line, n, 999)
    }

    print prev;
    print line;
    prev = "";
    next
}
/HAVE_SIGSET_T/{
    print ""
    line = $0

    # with GNU Awk we can say
    #   sub(/HAVE_SIGSET_T/, "LIBEXPLAIN_SIGSET_T", line)
    # but we have to do it the slow way
    n = index(line, "HAVE_SIGSET_T");
    if (n > 0)
    {
        line = substr(line, 1, n - 1) "LIBEXPLAIN" substr(line, n + 4, 999)
    }

    print prev;
    print line;
    prev = "";
    next
    print
    line = $0
}
/HAVE_USTAT_H/{
    print ""
    line = $0

    # with GNU Awk we can say
    #   sub(/HAVE_USTAT_H/, "LIBEXPLAIN_HAVE_USTAT_H", line)
    # but we have to do it the slow way
    n = index(line, "HAVE_USTAT_H");
    if (n > 0)
    {
        line = substr(line, 1, n - 1) "LIBEXPLAIN_" substr(line, n, 999)
    }
    print prev;
    print line;
    prev = "";
    next
    print
    line = $0
}
/STATFS_NARGS/{
    print ""
    line = $0

    # with GNU Awk we can say
    #   sub(/STATFS_NARGS/, "LIBEXPLAIN_STATFS_NARGS", line)
    # but we have to do it the slow way
    n = index(line, "STATFS_NARGS");
    if (n > 0)
    {
        line = substr(line, 1, n - 1) "LIBEXPLAIN_" substr(line, n, 999)
    }
    print prev;
    print line;
    prev = "";
    next
    print
    line = $0
}
{ prev = $0; }
END {
    print ""
    print "#endif /* LIBEXPLAIN_PUBLIC_CONFIG_H */"
}
