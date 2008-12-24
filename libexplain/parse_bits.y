/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

%{

#include <libexplain/ac/stdarg.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>

#include <libexplain/gcc_attributes.h>
#include <libexplain/parse_bits.h>
#include <libexplain/string_buffer.h>
#include <libexplain/wrap_and_print.h>

%}

%token BITWISE_AND
%token BITWISE_OR
%token JUNK
%token LP
%token MINUS
%token NUMBER
%token PLUS
%token RP
%token TILDE

%union
{
    int lv_number;
}

%type <lv_number> NUMBER
%type <lv_number> expression

%left BITWISE_OR
%left BITWISE_AND
%left PLUS MINUS
%right UNARY TILDE

%{

static const libexplain_parse_bits_table_t *lex_table;
static size_t   lex_table_size;
static const char *lex_cp;
static int      result;
static char     error_message[1000];
static libexplain_string_buffer_t error_buffer;
static int      error_count;
extern YYSTYPE yylval;
extern int yyparse(void);


static void yyerror(const char *fmt, ...) LIBEXPLAIN_FORMAT_PRINTF(1, 2);

static void
yyerror(const char *fmt, ...)
{
    va_list         ap;

    if (error_buffer.position > 0)
        libexplain_string_buffer_puts(&error_buffer, "; ");
    va_start(ap, fmt);
    libexplain_string_buffer_vprintf(&error_buffer, fmt, ap);
    va_end(ap);
    if
    (
        error_buffer.position > 0
    &&
        error_buffer.message[error_buffer.position - 1] == '\n'
    )
        error_buffer.position--;
    ++error_count;
}


static int
yylex(void)
{
    for (;;)
    {
        unsigned char c = *lex_cp++;
        switch (c)
        {
        case '\0':
            --lex_cp;
            return 0;

        case '|':
            return BITWISE_OR;

        case '&':
            return BITWISE_AND;

        case '+':
            return PLUS;

        case '-':
            return MINUS;

        case '~':
            return TILDE;

        case '(':
            return LP;

        case ')':
            return RP;

        case '_':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
            {
                char            name[200];
                libexplain_string_buffer_t buf;
                const libexplain_parse_bits_table_t *tp;

                libexplain_string_buffer_init(&buf, name, sizeof(name));
                for (;;)
                {
                    libexplain_string_buffer_putc(&buf, c);
                    c = *lex_cp;
                    switch (c)
                    {
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                    case '_':
                    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
                    case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
                    case 's': case 't': case 'u': case 'v': case 'w': case 'x':
                    case 'y': case 'z':
                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                    case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
                    case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
                    case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
                    case 'Y': case 'Z':
                        ++lex_cp;
                        continue;

                    default:
                        break;
                    }
                    break;
                }
                tp =
                    libexplain_parse_bits_find_by_name
                    (
                        name,
                        lex_table,
                        lex_table_size
                    );
                if (tp)
                {
                    yylval.lv_number = tp->value;
                    return NUMBER;
                }
                tp =
                    libexplain_parse_bits_find_by_name_fuzzy
                    (
                        name,
                        lex_table,
                        lex_table_size
                    );
                if (tp)
                {
                    yyerror
                    (
                        "name \"%s\" unknown, did you mean \"%s\" instead?",
                        name,
                        tp->name
                    );
                    yylval.lv_number = tp->value;
                    return NUMBER;
                }
                yyerror("name \"%s\" unknown", name);
            }
            return 0;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            {
                libexplain_string_buffer_t buf;
                char            *ep;
                char            number[200];

                libexplain_string_buffer_init(&buf, number, sizeof(number));
                for (;;)
                {
                    libexplain_string_buffer_putc(&buf, c);
                    c = *lex_cp;
                    switch (c)
                    {
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    case 'x':
                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                    case 'X':
                        ++lex_cp;
                        continue;

                    default:
                        break;
                    }
                    break;
                }
                ep = 0;
                yylval.lv_number = strtol(number, &ep, 0);
                if (ep == number || *ep)
                    yyerror("number \"%s\" malformed", number);
            }
            return NUMBER;

        default:
            return JUNK;
        }
    }
}


int
libexplain_parse_bits(const char *text,
    const libexplain_parse_bits_table_t *table, size_t table_size,
    int *result_p)
{
    libexplain_string_buffer_init
    (
        &error_buffer,
        error_message,
        sizeof(error_message)
    );
    error_count = 0;

    lex_cp = text;
    lex_table = table;
    lex_table_size = table_size;
    result = -1;
    yyparse();
    if (error_count > 0)
        return -1;
    return *result_p = result;
    return 0;
}


const char *
libexplain_parse_bits_get_error(void)
{
    return error_message;
}

%}

%%

result
    : expression
        { result = $1; }
    ;

expression
    : NUMBER
        { $$ = $1; }
    | LP expression RP
        { $$ = $2; }
    | TILDE expression
        { $$ = ~$2; }
    | PLUS expression
        %prec UNARY
        { $$ = $2; }
    | MINUS expression
        %prec UNARY
        { $$ = -$2; }
    | expression PLUS expression
        { $$ = $1 + $3; }
    | expression MINUS expression
        { $$ = $1 - $3; }
    | expression BITWISE_AND expression
        { $$ = $1 & $3; }
    | expression BITWISE_OR expression
        { $$ = $1 | $3; }
    ;
