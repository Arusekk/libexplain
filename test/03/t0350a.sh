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

TEST_SUBJECT="ioctl ENOTTY"
. test_prelude

cat > test.ok << 'fubar'
ioctl(fildes = 42, request = TCGETS, data = 0x00012345) failed,
Inappropriate ioctl for device (ENOTTY) because fildes is not associated
with a character special device; or, fildes is not associated with an
object to which request can be applied
fubar
test $? -eq 0 || no_result

explain -eENOTTY ioctl 42 TCGETS 0x12345 > test.out
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
