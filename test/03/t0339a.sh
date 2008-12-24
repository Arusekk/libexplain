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

TEST_SUBJECT="fchown EINVAL"
. test_prelude

cat > test.ok << 'fubar'
fchown(fildes = 42, owner = 0 "root", group = 0 "root") failed, Invalid
argument (EINVAL) because the owner UID or group GID is not a value
supported by the implementation; or, the fildes argument refers to a pipe
or socket and the implementation disallows execution of fchown() on a pipe
fubar
test $? -eq 0 || no_result

explain -e EINVAL fchown 42 0 0 > test.out
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
