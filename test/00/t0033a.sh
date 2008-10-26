#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008 Peter Miller
# Written by Peter Miller <millerp@canb.auug.org.au>
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

TEST_SUBJECT="write vs ENOSPC"
. test_prelude

fmt > test.ok << 'fubar'
write(fildes = 1, data = 0x123, data_size = 1110) failed, No space
left on device (28, ENOSPC) because the file system containing the file
("/example", 99% full) has no room for the data
fubar
test $? -eq 0 || no_result

explain write 1 0x123 0x456 -e ENOSPC -o test.out.narrow
test $? -eq 0 || fail

fmt -w300 test.out.narrow > test.out.wide
test $? -eq 0 || no_result

sed -e 's|= 1 \/\* ".*" \*\/,|= 1,|' \
    -e 's|(".*", .*% full)|("/example", 99% full)|' \
        test.out.wide > test.out.cooked
test $? -eq 0 || no_result

fmt test.out.cooked > test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
