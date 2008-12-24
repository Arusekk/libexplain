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

TEST_SUBJECT="execve EACCES"
. test_prelude

cat > test.ok << 'fubar'
execve(pathname = "oops", argv = ["oops"], envp = [/* vars */]) failed,
Permission denied (EACCES) because the process does not have execute
permission to the "oops" regular file in the current directory
fubar
test $? -eq 0 || no_result

date > oops

test_execve oops > test.out 2>&1
if test $? -ne 1
then
    echo expected to fail
    cat test.out
    fail
fi

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
