/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/linux/if_eql.h>
#include <libexplain/ac/net/if.h>

#include <libexplain/buffer/ifreq/slave_config.h>
#include <libexplain/buffer/long.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/path_is_efault.h>


#if defined(EQL_GETSLAVECFG) || defined(EQL_SETSLAVECFG)

void
explain_buffer_ifreq_slave_config(explain_string_buffer_t *sb,
    const struct ifreq *data, int extended_form)
{
    const struct slave_config *s;

    if (explain_pointer_is_efault(data, sizeof(*data)))
    {
        explain_buffer_pointer(sb, data);
        return;
    }

    explain_string_buffer_puts(sb, "{ ifr_data = ");
    s = (const struct slave_config *)data->ifr_data;
    if (!extended_form || explain_pointer_is_efault(s, sizeof(*s)))
    {
        explain_buffer_pointer(sb, s);
    }
    else
    {
        explain_string_buffer_puts(sb, "{ slave_name = ");
        explain_string_buffer_puts_quoted_n(sb, s->slave_name,
            sizeof(s->slave_name));
        if (s->priority)
        {
            explain_string_buffer_puts(sb, ", priority = ");
            explain_buffer_long(sb, s->priority);
        }
        explain_string_buffer_puts(sb, " }");
    }
    explain_string_buffer_puts(sb, " }");
}

#else

void
explain_buffer_ifreq_slave_config(explain_string_buffer_t *sb,
    const struct ifreq *data, int extended_form)
{
    (void)extended_form;
    explain_buffer_pointer(sb, data);
}

#endif
