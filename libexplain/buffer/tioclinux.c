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

#include <libexplain/ac/linux/tiocl.h>

#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/tioclinux.h>
#include <libexplain/parse_bits.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/sizeof.h>


#ifdef HAVE_LINUX_TIOCL_H

static void
explain_buffer_tiocl(explain_string_buffer_t *sb, int value)
{
    static const explain_parse_bits_table_t table[] =
    {
        { "TIOCL_SETSEL", TIOCL_SETSEL },
        { "TIOCL_PASTESEL", TIOCL_PASTESEL },
        { "TIOCL_UNBLANKSCREEN", TIOCL_UNBLANKSCREEN },
        { "TIOCL_SELLOADLUT", TIOCL_SELLOADLUT },
        { "TIOCL_GETSHIFTSTATE", TIOCL_GETSHIFTSTATE },
        { "TIOCL_GETMOUSEREPORTING", TIOCL_GETMOUSEREPORTING },
        { "TIOCL_SETVESABLANK", TIOCL_SETVESABLANK },
        { "TIOCL_SETKMSGREDIRECT", TIOCL_SETKMSGREDIRECT },
        { "TIOCL_GETFGCONSOLE", TIOCL_GETFGCONSOLE },
        { "TIOCL_SCROLLCONSOLE", TIOCL_SCROLLCONSOLE },
        { "TIOCL_BLANKSCREEN", TIOCL_BLANKSCREEN },
        { "TIOCL_BLANKEDSCREEN", TIOCL_BLANKEDSCREEN },
#ifdef TIOCL_GETKMSGREDIRECT
        { "TIOCL_GETKMSGREDIRECT", TIOCL_GETKMSGREDIRECT },
#endif
    };

    explain_parse_bits_print_single(sb, value, table, SIZEOF(table));
}


static void
explain_buffer_tiocl_setsel(explain_string_buffer_t *sb, int value)
{
    static const explain_parse_bits_table_t table[] =
    {
        { "TIOCL_SELCHAR", TIOCL_SELCHAR },
        { "TIOCL_SELWORD", TIOCL_SELWORD },
        { "TIOCL_SELLINE", TIOCL_SELLINE },
        { "TIOCL_SELPOINTER", TIOCL_SELPOINTER },
        { "TIOCL_SELCLEAR", TIOCL_SELCLEAR },
        { "TIOCL_SELMOUSEREPORT", TIOCL_SELMOUSEREPORT },
        { "TIOCL_SELBUTTONMASK", TIOCL_SELBUTTONMASK },
    };

    explain_parse_bits_print_single(sb, value, table, SIZEOF(table));
}


void
explain_buffer_tioclinux(explain_string_buffer_t *sb, const void *data)
{
    /* See console_ioctl(4) for more information. */
    if (!data)
    {
        print_pointer:
        explain_buffer_pointer(sb, data);
    }
    else
    {
        const unsigned char *cp;

        cp = data;
        explain_string_buffer_puts(sb, "{ ");
        switch (*cp)
        {
        case TIOCL_SETSEL:
            {
                typedef struct foo_t foo_t;
                struct foo_t
                {
                    char            subcode;
                    struct tiocl_selection sel;
                };
                const foo_t     *foo;

                foo = data;
                if (explain_pointer_is_efault(foo, sizeof(*foo)))
                    goto print_pointer;
                explain_buffer_tiocl(sb, cp[0]);
                explain_string_buffer_puts(sb, ", { ");
                explain_string_buffer_printf(sb, "xs = %u, ", foo->sel.xs);
                explain_string_buffer_printf(sb, "ys = %u, ", foo->sel.ys);
                explain_string_buffer_printf(sb, "xe = %u, ", foo->sel.xe);
                explain_string_buffer_printf(sb, "ye = %u, ", foo->sel.ye);
                explain_string_buffer_puts(sb, "sel_mode = ");
                explain_buffer_tiocl_setsel(sb, foo->sel.sel_mode);
                explain_string_buffer_puts(sb, " }");
            }
            break;

        case TIOCL_SETVESABLANK:
            if (explain_pointer_is_efault(data, 2))
                goto print_pointer;
            explain_buffer_tiocl(sb, cp[0]);
            explain_string_buffer_printf(sb, ", %d", cp[1]);
            break;

        default:
            if (explain_pointer_is_efault(data, 1))
                goto print_pointer;
            explain_buffer_tiocl(sb, *cp);
            break;
        }
        explain_string_buffer_puts(sb, " }");
    }
}

#else

void
explain_buffer_tioclinux(explain_string_buffer_t *sb, const void *data)
{
    explain_buffer_pointer(sb, data);
}

#endif
