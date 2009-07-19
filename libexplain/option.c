/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/ctype.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/fstrcmp.h>
#include <libexplain/option.h>
#include <libexplain/program_name.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>
#include <libexplain/wrap_and_print.h>


typedef enum option_level_t option_level_t;
enum option_level_t
{
    option_level_default,
    option_level_something_or_die,
    option_level_environment_variable,
    option_level_client,
};

typedef char option_value_t;

typedef struct option_t option_t;
struct option_t
{
    option_level_t level;
    option_value_t value;
};

typedef char value_t;

static int initialised;
static option_t debug = { option_level_default, 0 };
static option_t numeric_errno = { option_level_default, 1 };
static option_t dialect_specific = { option_level_default, 1 };
static option_t assemble_program_name = { option_level_default, 0 };

typedef struct table_t table_t;
struct table_t
{
    const char      *name;
    option_t        *location;
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
process(char *name, option_level_t level)
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
            if (level >= tp->location->level)
            {
                tp->location->level = level;
                tp->location->value = value;
            }
            return;
        }
    }
    if (debug.value)
    {
        const table_t   *best_tp;
        double          best_weight;
        explain_string_buffer_t buf;
        char            message[200];

        best_tp = 0;
        best_weight = 0.6;
        for (tp = table; tp < ENDOF(table); ++tp)
        {
            double          weight;

            weight = explain_fstrcmp(tp->name, name);
            if (best_weight < weight)
            {
                best_tp = tp;
                best_weight = weight;
            }
        }

        explain_string_buffer_init(&buf, message, sizeof(message));
        explain_string_buffer_puts(&buf, "libexplain: Warning: option ");
        explain_string_buffer_puts_quoted(&buf, name);
        explain_string_buffer_puts(&buf, " unknown");
        if (best_tp)
        {
            if (level >= best_tp->location->level)
            {
                best_tp->location->level = level;
                best_tp->location->value = value;
            }

            explain_string_buffer_puts(&buf, ", assuming you meant ");
            explain_string_buffer_puts_quoted(&buf, best_tp->name);
            explain_string_buffer_puts(&buf, " instead");
        }
        explain_wrap_and_print(stderr, message);
    }
}


static void
initialise(void)
{
    const char      *cp;
    int             err;

    err = errno;
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

        process(name, option_level_environment_variable);
    }
    errno = err;
}


int
explain_option_debug(void)
{
    if (!initialised)
        initialise();
    return debug.value;
}


int
explain_option_numeric_errno(void)
{
    if (!initialised)
        initialise();
    return numeric_errno.value;
}


int
explain_option_dialect_specific(void)
{
    if (!initialised)
        initialise();
    return dialect_specific.value;
}


int
explain_option_assemble_program_name(void)
{
    if (!initialised)
        initialise();
    return assemble_program_name.value;
}


void
explain_program_name_assemble(int yesno)
{
    /*
     * This is the public interface, it has highest
     * precedence.  For the internal interface, see the
     * explain_program_name_assemble_internal function.
     */
    if (!initialised)
        initialise();
    if (assemble_program_name.level <= option_level_client)
    {
        assemble_program_name.level = option_level_client;
        assemble_program_name.value = !!yesno;
    }
}


void
explain_program_name_assemble_internal(int yesno)
{
    /*
     * This is the private interface.
     */
    if (!initialised)
        initialise();
    if (assemble_program_name.level <= option_level_something_or_die)
    {
        assemble_program_name.level = option_level_something_or_die;
        assemble_program_name.value = !!yesno;
    }
}
