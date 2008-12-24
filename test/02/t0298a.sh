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

TEST_SUBJECT="dup2 EBADF"
. test_prelude

cat > test.ok << 'fubar'
dup2(oldfd = 42, newfd = 99) failed, Bad file descriptor (EBADF) because
oldfd does not refer to an open file; this is more likely to be a software
error (a bug) than it is to be a user error
fubar
test $? -eq 0 || no_result

test_dup2 42 99 > test.out 2>&1
if test "$?" -ne 1
then
    echo expeted to fail
    cat test.out
    fail
fi

diff test.ok test.out
test $? -eq 0 || fail


fmt > test.ok << 'fubar'
dup2(oldfd = 1, newfd = 987654) failed, Bad file descriptor (EBADF)
because newfd is outside the allowed range for file descriptors
fubar
test $? -eq 0 || no_result

test_dup2 1 987654 > test.out4 2>&1
if test "$?" -ne 1
then
    echo expeted to fail
    cat test.out
    fail
fi

fmt -w700 test.out4 > test.out3
test $? -eq 0 || no_result

sed 's|\([0-9][0-9]*\) "[^"]*"|\1|g' test.out3 > test.out2
test $? -eq 0 || no_result

fmt test.out2 > test.out
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
