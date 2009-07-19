#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008, 2009 Peter Miller
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

TEST_SUBJECT="access EIO"
. test_prelude

cat > test.ok << 'fubar'
access(pathname = "foobar", mode = F_OK) failed, Input/output error
(EIO) because a low-level I/O error occurred in the block special device,
possibly as a result of a preceeding read(2) or write(2) system call
fubar
test $? -eq 0 || no_result

cat > test.ok2 << 'fubar'
access(pathname = "foobar", mode = F_OK) failed, Input/output error
(EIO) because a low-level I/O error occurred, probably in hardware,
possibly as a result of a preceeding read(2) or write(2) system call
fubar
test $? -eq 0 || no_result

touch foobar
test $? -eq 0 || no_result

explain access foobar -e EIO -o test.out4
test $? -eq 0 || fail

fmt -w700 test.out4 > test.out3
test $? -eq 0 || no_result

sed 's| "/dev/[^"]*" | |' test.out3 > test.out2
test $? -eq 0 || no_result

fmt test.out2 > test.out
test $? -eq 0 || no_result

diff test.ok test.out || diff test.ok2 test.out >/dev/null
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et