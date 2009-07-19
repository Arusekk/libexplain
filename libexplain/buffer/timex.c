/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/ac/sys/timex.h>

#include <libexplain/buffer/timex.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>


#ifdef HAVE_SYS_TIMEX_H

static void
explain_buffer_timex_mode(explain_string_buffer_t *sb, int mode)
{
    static const struct explain_parse_bits_table_t table[] =
    {
        { "ADJ_OFFSET", ADJ_OFFSET },
        { "ADJ_FREQUENCY", ADJ_FREQUENCY },
        { "ADJ_MAXERROR", ADJ_MAXERROR },
        { "ADJ_ESTERROR", ADJ_ESTERROR },
        { "ADJ_STATUS", ADJ_STATUS },
        { "ADJ_TIMECONST", ADJ_TIMECONST },
        { "ADJ_TICK", ADJ_TICK },
    };

    if (mode == ADJ_OFFSET_SINGLESHOT)
        explain_string_buffer_puts(sb, "ADJ_OFFSET_SINGLESHOT");
#ifdef ADJ_OFFSET_SS_READ
    else if (mode == ADJ_OFFSET_SS_READ)
        explain_string_buffer_puts(sb, "ADJ_OFFSET_SS_READ");
#endif
    else
        explain_parse_bits_print(sb, mode, table, SIZEOF(table));
}


static void
explain_buffer_timex_status(explain_string_buffer_t *sb, int status)
{
    static const struct explain_parse_bits_table_t table[] =
    {
        { "STA_PLL", STA_PLL },
        { "STA_PPSFREQ", STA_PPSFREQ },
        { "STA_PPSTIME", STA_PPSTIME },
        { "STA_FLL", STA_FLL },
        { "STA_INS", STA_INS },
        { "STA_DEL", STA_DEL },
        { "STA_UNSYNC", STA_UNSYNC },
        { "STA_FREQHOLD", STA_FREQHOLD },
        { "STA_PPSSIGNAL", STA_PPSSIGNAL },
        { "STA_PPSJITTER", STA_PPSJITTER },
        { "STA_PPSWANDER", STA_PPSWANDER },
        { "STA_PPSERROR", STA_PPSERROR },
        { "STA_CLOCKERR", STA_CLOCKERR },
    };

    explain_parse_bits_print(sb, status, table, SIZEOF(table));
}

#endif


void
explain_buffer_timex(explain_string_buffer_t *sb,
    const struct timex *data)
{
#ifdef HAVE_SYS_TIMEX_H
    if (explain_pointer_is_efault(data, sizeof(*data)))
    {
        explain_buffer_pointer(sb, data);
        return;
    }

    explain_string_buffer_puts(sb, "{ modes = ");
    explain_buffer_timex_mode(sb, data->modes);
    if (data->modes == ADJ_OFFSET_SINGLESHOT)
    {
        explain_string_buffer_printf(sb, ", offset = %ld", data->offset);
    }
#ifdef ADJ_OFFSET_SS_READ
    else if (data->modes == ADJ_OFFSET_SS_READ)
    {
        /* do nothing more */
    }
#endif
    else
    {
        if (data->modes & ADJ_OFFSET)
            explain_string_buffer_printf(sb, ", offset = %ld", data->offset);
        if (data->modes & ADJ_FREQUENCY)
            explain_string_buffer_printf(sb, ", freq = %ld", data->freq);
        if (data->modes & ADJ_MAXERROR)
        {
            explain_string_buffer_printf
            (
                sb,
                ", maxerror = %ld",
                data->maxerror
            );
        }
        if (data->modes & ADJ_ESTERROR)
        {
            explain_string_buffer_printf
            (
                sb,
                ", esterror = %ld",
                data->esterror
            );
        }
        if (data->modes & ADJ_STATUS)
        {
            explain_string_buffer_puts(sb, ", status = ");
            explain_buffer_timex_status(sb, data->status);
        }
        if (data->modes & ADJ_TIMECONST)
        {
            explain_string_buffer_printf
            (
                sb,
                ", constant = %ld",
                data->constant
            );
        }
        if (data->modes & ADJ_TICK)
            explain_string_buffer_printf(sb, ", tick = %ld", data->tick);
    }
    explain_string_buffer_puts(sb, " }");
#else
    explain_buffer_pointer(sb, data);
#endif
}
