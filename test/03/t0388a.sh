#!/bin/sh
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

TEST_SUBJECT="sethostname EINVAL"
. test_prelude

cat > test.ok << 'fubar'
gethostname(name = "hello", name_size = 5) failed, Invalid argument
(EINVAL) because (this error is unknown for this system call, you could
improve libexplain by contributing code to explain this error)
fubar
test $? -eq 0 || no_result

# This is tricky.  We want to exersize the code path, but we don't want
# system specific HOST_NAME_MAX in the output.  These argument values
# couln't possibly result in this error in real life, but it makes it
# possible to test that code path.
explain -eEINVAL sethostname "hello" > test.out 2>&1
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