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

TEST_SUBJECT="rename EACCES"
. test_prelude

cat > test.ok << 'fubar'
rename(oldpath = "a/foo", newpath = "b/bar") failed, Permission denied (13,
EACCES) because write permission is denied for the "b" directory; note that
oldpath still exists
fubar
test $? -eq 0 || no_result

mkdir -p a/foo b
test $? -eq 0 || no_result

chmod a-w b
test $? -eq 0 || no_result

test_rename a/foo b/bar > test.out 2>&1
if test $? -ne 1
then
    echo "expected to fail"
    test -f test.out && cat test.out
    chmod -R u+w .
    fail
fi

chmod -R u+w .

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
