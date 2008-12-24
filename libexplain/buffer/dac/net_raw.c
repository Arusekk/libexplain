/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/buffer/dac.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/capability.h>
#include <libexplain/option.h>


void
libexplain_buffer_dac_net_raw(libexplain_string_buffer_t *sb)
{
    if (libexplain_capability_net_raw())
        return;
    libexplain_string_buffer_puts(sb, ", ");
    libexplain_buffer_and_the_process_is_not_privileged(sb);
#ifdef HAVE_SYS_CAPABILITY_H
    if (libexplain_option_dialect_specific())
    {
        libexplain_buffer_does_not_have_capability(sb, "DAC_NET_RAW");
    }
#endif
}
