#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2009, 2010 Peter Miller
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

TEST_SUBJECT="puts EIO"
. test_prelude

cat > test.ok << 'fubar'
puts(s = "hello") failed, Input/output error (EIO) because a low-level I/O
error occurred, probably in hardware, possibly as a result of a preceeding
read(2) or write(2) system call
fubar
test $? -eq 0 || no_result

# The "<&-" means "close stdin"
#
# We need to close stding because otherwise we get false negatives
# depending on how make or aegis (etc) choose how to execute the test.
explain -eEIO puts hello <&- > test.out 2>&1
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