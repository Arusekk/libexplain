#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008 Peter Miller
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

TEST_SUBJECT="chmod EPERM"
. test_prelude

cat > test.in << 'fubar'
Hello, World!
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
chmod(pathname = "foobar", mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
failed, Operation not permitted (EPERM) because the process does not have
inode modification permission, and the process is not privileged
fubar
test $? -eq 0 || no_result

touch foobar
test $? -eq 0 || no_result

explain chmod foobar -e EPERM -o test.out
test $? -eq 0 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
