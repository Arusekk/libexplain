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

TEST_SUBJECT="ustat ENOSYS"
. test_prelude

cat > test.ok << 'fubar'
ustat(dev = MKDEV(18, 52), data = 0x00012345) failed, Function not
implemented (ENOSYS) because this system does not support the ustat system
call
fubar
test $? -eq 0 || no_result

cat > test.ok2 << 'fubar'
ustat(dev = makedev(18, 52), data = 0x00012345) failed, Function not
implemented (ENOSYS) because this system does not support the ustat system
call
fubar
test $? -eq 0 || no_result

explain -eENOSYS ustat 0x1234 0x12345 > test.out
test $? -eq 0 || fail

diff test.ok test.out > /dev/null 2> /dev/null || \
diff test.ok2 test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
