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
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
BEGIN {
    printf("<html>\n");
    printf("<body>\n");
    printf("<h1> Fix Me, Please! </h1>\n");
    printf("The following notes were automatically extracted from\n");
    printf("the source files, at places where the code is marked\n");
    printf("&ldquo;<tt>FIXME:</tt>&rdquo;.\n");
    printf("<table>\n");
    printf("<tr><th>Filename</th><th>Line</th><th>Comment</th></tr>\n");
}

/FIXME/ {
    text = $0
    sub(/ *\*\/.*/, "", text)
    sub(/.*FIXME: */, "", text)

    fn = FILENAME
    sub(/^bl\//, "", fn);
    sub(/^blbl\//, "", fn);
    sub(/^blblbl\//, "", fn);
    sub(/^blblblbl\//, "", fn);

    if (fn != "web-src/fixme.awk" && fn != "web-src/index.html")
    {
        printf("<tr><td valign=\"top\">%s</td>\n", fn);
        printf("    <td valign=\"top\" align=\"right\">%d</td>\n", FNR);
        printf("    <td valign=\"top\"><i>%s</i></td></tr>\n", text);
        printf("    </tr>\n");
    }
}
END {
    printf("</table>\n");
    printf("</body>\n");
    printf("</html>\n");
}
