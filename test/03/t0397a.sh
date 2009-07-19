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

TEST_SUBJECT="strtol EINVAL"
. test_prelude

cat > test.ok << 'fubar'
strtol(nptr = "1000", endptr = 0xNNNNNNNN, base = 65) failed, Invalid
argument (EINVAL) because the base argument was incorrectly specified
fubar
test $? -eq 0 || no_result

test_strtol 1000 65 > test.out.2 2>&1
test $? -eq 1 || fail

sed 's|endptr = 0x........|endptr = 0xNNNNNNNN|' test.out.2 > test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

cat > test.ok << 'fubar'
strtol(nptr = "yuck", endptr = 0xNNNNNNNN, base = 0) failed, Invalid
argument (EINVAL) because the nptr argument does not appear to be a number
fubar
test $? -eq 0 || no_result

test_strtol yuck > test.out.2 2>&1
test $? -eq 1 || fail

sed 's|endptr = 0x........|endptr = 0xNNNNNNNN|' test.out.2 > test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

cat > test.ok << 'fubar'
strtol(nptr = "8000000000", endptr = 0xNNNNNNNN, base = 0) failed,
Numerical result out of range (ERANGE) because the resulting value would
have been too large to store
fubar
test $? -eq 0 || no_result

test_strtol 8000000000 > test.out.2 2>&1
test $? -eq 1 || fail

sed 's|endptr = 0x........|endptr = 0xNNNNNNNN|' test.out.2 > test.out
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
