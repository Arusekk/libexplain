/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller,,,
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>

#include <libexplain/tcdrain.h>


void
explain_message_tcdrain(char *message, int message_size, int fildes)
{
    explain_message_errno_tcdrain(message, message_size, errno, fildes);
}


/* vim: set ts=8 sw=4 et */
