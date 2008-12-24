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

#include <libexplain/ac/ctype.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/fstrcmp.h>
#include <libexplain/option.h>
#include <libexplain/program_name.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>
#include <libexplain/wrap_and_print.h>


typedef char value_t;

static int initialised;
static value_t debug;
static value_t numeric_errno = 1;
static value_t dialect_specific = 1;
static value_t assemble_program_name = 1;

typedef struct table_t table_t;
struct table_t
{
    const char      *name;
    value_t         *location;
};

static const table_t table[] =
{
    { "assemble-program-name", &assemble_program_name },
    { "debug", &debug },
    { "dialect-specific", &dialect_specific },
    { "numeric-errno", &numeric_errno },
    { "program-name", &assemble_program_name },
};


static void
process(char *name)
{
    const table_t   *tp;
    value_t         value;
    char            *eq;

    if (!*name)
        return;
    value = 1;
    if (name[0] == 'n' && name[1] == 'o' && name[2] == '-')
    {
        value = 0;
        name += 3;
    }
    eq = strchr(name, '=');
    if (eq)
    {
        value = atoi(eq + 1);
        while (eq > name && isspace((unsigned char)eq[-1]))
            --eq;
        *eq = '\0';
    }
    for (tp = table; tp < ENDOF(table); ++tp)
    {
        if (0 == strcmp(tp->name, name))
        {
            *tp->location = value;
            return;
        }
    }
    if (debug)
    {
        const table_t   *best_tp;
        double          best_weight;
        libexplain_string_buffer_t buf;
        char            message[200];

        best_tp = 0;
        best_weight = 0.6;
        for (tp = table; tp < ENDOF(table); ++tp)
        {
            double          weight;

            weight = libexplain_fstrcmp(tp->name, name);
            if (best_weight < weight)
            {
                best_tp = tp;
                best_weight = weight;
            }
        }

        libexplain_string_buffer_init(&buf, message, sizeof(message));
        libexplain_string_buffer_puts(&buf, "libexplain: Warning: option ");
        libexplain_string_buffer_puts_quoted(&buf, name);
        libexplain_string_buffer_puts(&buf, " unknown");
        if (best_tp)
        {
            *best_tp->location = value;

            libexplain_string_buffer_puts(&buf, ", assuming you meant ");
            libexplain_string_buffer_puts_quoted(&buf, best_tp->name);
            libexplain_string_buffer_puts(&buf, " instead");
        }
        libexplain_wrap_and_print(stderr, message);
    }
}


static void
initialise(void)
{
    const char      *cp;

    initialised = 1;
    cp = getenv("LIBEXPLAIN_OPTIONS");
    if (!cp)
        cp = "";
    for (;;)
    {
        char            name[100];
        char            *np;
        unsigned char   c;

        c = *cp++;
        if (!c)
            break;
        if (isspace(c))
            continue;
        np = name;
        for (;;)
        {
            if (np < ENDOF(name) -1)
            {
                if (isupper(c))
                    c = tolower(c);
                *np++ = c;
            }
            c = *cp++;
            if (!c)
            {
                --cp;
                break;
            }
            if (c == ',')
                break;
        }
        while (np > name && isspace((unsigned char)np[-1]))
            --np;
        *np = '\0';

        process(name);
    }
}


int
libexplain_option_debug(void)
{
    if (!initialised)
        initialise();
    return debug;
}


int
libexplain_option_numeric_errno(void)
{
    if (!initialised)
        initialise();
    return numeric_errno;
}


int
libexplain_option_dialect_specific(void)
{
    if (!initialised)
        initialise();
    return dialect_specific;
}


int
libexplain_option_assemble_program_name(void)
{
    if (!initialised)
        initialise();
    return assemble_program_name;
}


void
libexplain_program_name_assemble(int yesno)
{
    if (!initialised)
        initialise();
    assemble_program_name = !!yesno;
}
