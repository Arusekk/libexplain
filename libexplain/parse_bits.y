/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/gcc_attributes.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>
#include <libexplain/wrap_and_print.h>

#define YYDEBUG 0

#if YYDEBUG
extern int yydebug;
#endif

%}

%token BITWISE_AND
%token BITWISE_OR
%token COMMA
%token JUNK
%token LP
%token MINUS
%token NUMBER
%token PLUS
%token RP
%token TILDE
%token FUNC_IOC
%token FUNC_IO
%token FUNC_IOR
%token FUNC_IOW
%token FUNC_IOWR

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

static const explain_parse_bits_table_t *lex_table;
static size_t   lex_table_size;
static const char *lex_cp;
static int      result;
static char     error_message[1000];
static explain_string_buffer_t error_buffer;
static int      error_count;
extern YYSTYPE yylval;
extern int yyparse(void);


static void yyerror(const char *fmt, ...) LIBEXPLAIN_FORMAT_PRINTF(1, 2);

static void
yyerror(const char *fmt, ...)
{
    va_list         ap;

    if (error_buffer.position > 0)
        explain_string_buffer_puts(&error_buffer, "; ");
    va_start(ap, fmt);
    explain_string_buffer_vprintf(&error_buffer, fmt, ap);
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


static explain_parse_bits_table_t constants[] =
{
    { "NULL", 0 },
    { "_IOC_NONE", _IOC_NONE },
    { "_IOC_READ", _IOC_READ },
    { "_IOC_WRITE", _IOC_WRITE },
    { "IOC_IN", IOC_IN },
    { "IOC_OUT", IOC_OUT },
    { "IOC_INOUT", IOC_INOUT },
    { "IOCSIZE_MASK", IOCSIZE_MASK },
    { "IOCSIZE_SHIFT", IOCSIZE_SHIFT },
};


static explain_parse_bits_table_t keywords[] =
{
    { "_IO", FUNC_IO },
    { "_IOC", FUNC_IOC },
    { "_IOR", FUNC_IOR },
    { "_IOWR", FUNC_IOWR },
    { "_IOW", FUNC_IOW },
};


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

        case ',':
            return COMMA;

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

        case '\'':
            c = *lex_cp;
            yylval.lv_number = 0;
            if (c == '\0' || c == '\'')
                return JUNK;
            yylval.lv_number = c;
            ++lex_cp;
            c = *lex_cp;
            if (c != '\'')
                return JUNK;
            /* FIXME: parse C escape sequences */
            ++lex_cp;
            return NUMBER;

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
                explain_string_buffer_t buf;
                const explain_parse_bits_table_t *tp;

                explain_string_buffer_init(&buf, name, sizeof(name));
                for (;;)
                {
                    explain_string_buffer_putc(&buf, c);
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
                    explain_parse_bits_find_by_name
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
                    explain_parse_bits_find_by_name
                    (
                        name,
                        constants,
                        SIZEOF(constants)
                    );
                if (tp)
                {
                    yylval.lv_number = tp->value;
                    return NUMBER;
                }
                tp =
                    explain_parse_bits_find_by_name
                    (
                        name,
                        keywords,
                        SIZEOF(keywords)
                    );
                if (tp)
                    return tp->value;
                tp =
                    explain_parse_bits_find_by_name_fuzzy
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
            return JUNK;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            {
                explain_string_buffer_t buf;
                char            *ep;
                char            number[200];

                explain_string_buffer_init(&buf, number, sizeof(number));
                for (;;)
                {
                    explain_string_buffer_putc(&buf, c);
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
explain_parse_bits(const char *text,
    const explain_parse_bits_table_t *table, size_t table_size,
    int *result_p)
{
#if YYDEBUG
    yydebug = 1;
#endif
    explain_string_buffer_init
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
    *result_p = result;
    return 0;
}


const char *
explain_parse_bits_get_error(void)
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
    | FUNC_IOC LP expression COMMA expression COMMA expression COMMA expression
            RP
        { $$ = _IOC($3, $5, $7, $9); }
    | FUNC_IO LP expression COMMA expression RP
        { $$ = _IO($3, $5); }
    | FUNC_IOR LP expression COMMA expression COMMA expression RP
        {
            /*
             * We don't use _IOR because this is a bit of a hack.  The
             * _IOR define uses sizeof() on its last argument, but we
             * want to specify the size explicitly.
             */
            $$ = _IOC(_IOC_READ, $3, $5, $7);
        }
    | FUNC_IOW LP expression COMMA expression COMMA expression RP
        {
            /*
             * We don't use _IOW because this is a bit of a hack.  The
             * _IOW define uses sizeof() on its last argument, but we
             * want to specify the size explicitly.
             */
            $$ = _IOC(_IOC_WRITE, $3, $5, $7);
        }
    | FUNC_IOWR LP expression COMMA expression COMMA expression RP
        {
            /*
             * We don't use _IORW because this is a bit of a hack.  The
             * _IORW define uses sizeof() on its last argument, but we
             * want to specify the size explicitly.
             */
            $$ = _IOC(_IOC_READ | _IOC_WRITE, $3, $5, $7);
        }
    ;