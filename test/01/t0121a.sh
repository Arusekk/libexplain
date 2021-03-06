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

TEST_SUBJECT="unlink ELOOP"
. test_prelude

cat > test.ok << 'fubar'
unlink(pathname = "a/b/foobar") failed, Too many levels of symbolic links
(ELOOP) because too many symbolic links were encountered in pathname
fubar
test $? -eq 0 || no_result

mkdir a
test $? -eq 0 || no_result

n=1
while test $n -lt 256
do
    nn=`expr $n + 1`
    ln -s $nn a/$n || no_result
    n=$nn
done

ln -s 1 a/b
test $? -eq 0 || no_result

test_unlink a/b/foobar > test.out 2>&1
if test $? -ne 1
then
    echo expected to fail
    test -f test.out && cat test.out
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
