#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2009 Peter Miller
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
    sub(/_FILE_OFFSET_BITS/, "LIBEXPLAIN_FILE_OFFSET_BITS", line)
    print prev;
    print line;
    prev = "";
    next
}
/_LARGEFILE_SOURCE/ {
    print ""
    line = $0
    sub(/_LARGEFILE_SOURCE/, "LIBEXPLAIN_LARGEFILE_SOURCE", line)
    print prev;
    print line;
    prev = "";
    next
}
/_LARGE_FILES/ {
    print ""
    line = $0
    sub(/_LARGE_FILES/, "LIBEXPLAIN_LARGE_FILES", line)
    print prev;
    print line;
    prev = "";
    next
}
/HAVE_SIGSET_T/{
    print ""
    line = $0
    sub(/HAVE_SIGSET_T/, "LIBEXPLAIN_SIGSET_T", line)
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
