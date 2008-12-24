/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/assert.h>
#include <libexplain/ac/ctype.h>
#include <libexplain/ac/pwd.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/time.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/errno_info.h>
#include <libexplain/fopen.h>
#include <libexplain/fclose.h>
#include <libexplain/sizeof.h>

#include <test_generator/generate.h>


static int
try_to_guess_section(const char *function_name)
{
    int             section;

    for (section = 2; section < 8; ++section)
    {
        struct stat     st;
        char            path[1000];

        snprintf
        (
            path,
            sizeof(path),
            "/usr/share/man/man%d/%s.%d.gz",
            section,
            function_name,
            section
        );
        if (lstat(path, &st) >= 0)
            return section;
    }
    return 2;
}


static char     synopsis[200];


static void
try_to_get_synopsis(const char *function_name, int section)
{
    const char      *cat;
    FILE            *fp;
    int             state;
    char            filename[80];
    char            command[200];

    snprintf(synopsis, sizeof(synopsis), "blah blah");

    cat = "cat";
    snprintf(filename, sizeof(filename), "/usr/share/man/man%d/%s.%d.gz",
        section, function_name, section);
    if (0 == access(filename, R_OK))
        cat = "gunzip";
    else
    {
        snprintf(filename, sizeof(filename), "/usr/share/man/man%d/%s.%d",
            section, function_name, section);
    }

    snprintf(command, sizeof(command), "%s < %s", cat, filename);
    fp = popen(command, "r");
    if (!fp)
        return;
    state = 0;
    for (;;)
    {
        char            line[1000];
        size_t          len;

        if (!fgets(line, sizeof(line), fp))
            break;
        len = strlen(line);
        while (len > 0 && isspace((unsigned char)line[len - 1]))
            line[--len] = '\0';
        if (0 == strcmp(".SH NAME", line))
        {
            state = 1;
            continue;
        }
        if (line[0] == '.')
        {
            state = 0;
            continue;
        }
        if (state == 1)
        {
            char            *p;

            p = strstr(line, "\\-");
            if (!p)
                break;
            p += 2;
            while (*p && isspace((unsigned char)*p))
                ++p;
            if (!*p)
                break;
            snprintf(synopsis, sizeof(synopsis), "%s", p);
            break;
        }
        state = 0;
    }
    pclose(fp);
}


static void
downcase_insitu(char *cp)
{
    for (;;)
    {
        unsigned char c = *cp;
        if (!c)
            break;
        if (isupper(c))
            *cp = tolower(c);
        ++cp;
    }
}


static void
look_for_error_cases(const char *function_name, int section, FILE *ofp)
{
    const char      *cat;
    FILE            *ifp;
    char            filename[80];
    char            command[200];

    snprintf(synopsis, sizeof(synopsis), "blah blah");

    cat = "cat";
    snprintf(filename, sizeof(filename), "/usr/share/man/man%d/%s.%d.gz",
        section, function_name, section);
    if (0 == access(filename, R_OK))
        cat = "gunzip";
    else
    {
        snprintf(filename, sizeof(filename), "/usr/share/man/man%d/%s.%d",
            section, function_name, section);
    }

    snprintf(command, sizeof(command), "%s < %s", cat, filename);
    ifp = popen(command, "r");
    if (!ifp)
        return;
    for (;;)
    {
        char            *cp;
        char            *ep;
        const libexplain_errno_info_t *eip;
        char            line[1000];

        if (!fgets(line, sizeof(line), ifp))
            break;
        if (line[0] != '.')
            continue;
        if (line[1] != 'B')
            continue;
        if (line[2] != ' ')
            continue;
        if (line[3] != 'E')
            continue;
        cp = line + 3;
        ep = cp + 1;
        while (*ep && !isspace((unsigned char)*ep))
            ++ep;
        *ep = 0;
        eip = libexplain_errno_info_by_name(cp);
        if (!eip)
            continue;

        fprintf(ofp, "    case %s:\n", cp);
        downcase_insitu(cp);
        fprintf(ofp, "        libexplain_buffer_%s(sb", cp);
        if (0 == strcmp(cp, "ebadf"))
            fprintf(ofp, ", fildes, \"fildes\"");
        fprintf(ofp, ");\n");
        fprintf(ofp, "        break;\n");
        fprintf(ofp, "\n");
    }
    pclose(ifp);
}


static void
generate_lgpl_header(FILE *fp)
{
    static char text1[] =
"/*\n"
" * libexplain - Explain errno values returned by libc functions\n"
        ;

    static char text2[] =
" *\n"
" * This program is free software; you can redistribute it and/or modify it\n"
" * under the terms of the GNU Lesser General Public License as published by\n"
" * the Free Software Foundation; either version 3 of the License, or (at\n"
" * your option) any later version.\n"
" *\n"
" * This program is distributed in the hope that it will be useful,\n"
" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
" * Lesser General Public License for more details.\n"
" *\n"
" * You should have received a copy of the GNU Lesser General Public License\n"
" * along with this program. If not, see <http://www.gnu.org/licenses/>.\n"
" */\n"
        ;

    time_t          now;
    struct tm       *tmp;
    struct passwd   *pw;

    fputs(text1, fp);

    time(&now);
    tmp = localtime(&now);
    pw = getpwuid(getuid());
    fprintf
    (
        fp,
        " * Copyright (C) %d %s\n",
        tmp->tm_year + 1900,
        pw ? pw->pw_gecos : "you"
    );

    fputs(text2, fp);
}


static void
generate_gpl_header(FILE *fp)
{
    static char text1[] =
"/*\n"
" * libexplain - Explain errno values returned by libc functions\n"
        ;

    static char text2[] =
" *\n"
" * This program is free software; you can redistribute it and/or modify it\n"
" * under the terms of the GNU General Public License as published by\n"
" * the Free Software Foundation; either version 3 of the License, or (at\n"
" * your option) any later version.\n"
" *\n"
" * This program is distributed in the hope that it will be useful,\n"
" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
" * General Public License for more details.\n"
" *\n"
" * You should have received a copy of the GNU General Public License\n"
" * along with this program. If not, see <http://www.gnu.org/licenses/>.\n"
" */\n"
        ;

    time_t          now;
    struct tm       *tmp;
    struct passwd   *pw;

    fputs(text1, fp);

    time(&now);
    tmp = localtime(&now);
    pw = getpwuid(getuid());
    fprintf
    (
        fp,
        " * Copyright (C) %d %s\n",
        tmp->tm_year + 1900,
        pw ? pw->pw_gecos : "you"
    );

    fputs(text2, fp);
}


static void
groff_license_header(FILE *fp)
{
    static char text1[] =
".\\\"\n"
".\\\" libexplain - Explain errno values returned by libc functions\n"
        ;

    static char text2[] =
".\\\"\n"
".\\\" This program is free software; you can redistribute it and/or modify\n"
".\\\" it under the terms of the GNU General Public License as published by\n"
".\\\" the Free Software Foundation; either version 3 of the License, or (at\n"
".\\\" your option) any later version.\n"
".\\\"\n"
".\\\" This program is distributed in the hope that it will be useful,\n"
".\\\" but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
".\\\" MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
".\\\" General Public License for more details.\n"
".\\\"\n"
".\\\" You should have received a copy of the GNU General Public License\n"
".\\\" along with this program. If not, see <http://www.gnu.org/licenses/>.\n"
".\\\"\n"
        ;

    time_t          now;
    struct tm       *tmp;
    struct passwd   *pw;

    fputs(text1, fp);

    time(&now);
    tmp = localtime(&now);
    pw = getpwuid(getuid());
    fprintf
    (
        fp,
        ".\\\" Copyright (C) %d %s\n",
        tmp->tm_year + 1900,
        pw ? pw->pw_gecos : "you"
    );

    fputs(text2, fp);
}


static const char *
find_function_name(node_t *np)
{
    assert(node_is(np, "declarator"));
    assert(np->nchild == 1 || np->nchild == 2);
    np = np->child[np->nchild - 1];
    assert(node_is(np, "direct_declarator"));

    if (np->nchild != 4 || !node_is_literal(np->child[1], "("))
    {
        not_a_func:
        fprintf(stderr, "not a function declaration\n");
        exit(EXIT_FAILURE);
    }
    assert(node_is_literal(np->child[3], ")"));

    np = np->child[0];
    assert(node_is(np, "direct_declarator"));
    if (np->nchild != 1 || !np->child[0]->literal)
        goto not_a_func;

    return np->child[0]->literal;
}


static node_t *
find_argument_list(node_t *np)
{
    /* call this *after* find_function_name */
    assert(node_is(np, "declarator"));
    assert(np->nchild == 1 || np->nchild == 2);
    np = np->child[np->nchild - 1];
    assert(node_is(np, "direct_declarator"));
    assert(np->nchild == 4);
    assert(node_is_literal(np->child[1], "("));
    assert(node_is_literal(np->child[3], ")"));
    return np->child[2];
}


static const char *extract_name_from_declarator(const node_t *np); /* forward */


static const char *
extract_name_from_direct_declarator(const node_t *np)
{
    for (;;)
    {
        assert(node_is(np, "direct_declarator"));
        if (np->nchild == 1)
        {
            assert(np->child[0]->literal);
            return np->child[0]->literal;
        }
        assert(np->nchild == 3);
        if (node_is_literal(np->child[0], "("))
        {
            return extract_name_from_declarator(np->child[1]);
        }
        np = np->child[0];
    }
}


static const char *
extract_name_from_declarator(const node_t *np)
{
    assert(node_is(np, "declarator"));
    assert(np->nchild == 1 || np->nchild == 2);
    return extract_name_from_direct_declarator(np->child[np->nchild - 1]);
}


static void
name_should_be_different(const char *name1, const char *name2)
{
    fprintf(stderr, "argument \"%s\" should be called \"%s\"\n", name1, name2);
    exit(EXIT_FAILURE);
}


static void
make_sure_name_is_acceptable(const char *name)
{
    if (0 == strcmp(name, "path"))
        name_should_be_different(name, "pathname");
    if (0 == strcmp(name, "fd"))
        name_should_be_different(name, "fildes");
    if (0 == strcmp(name, "buf"))
        name_should_be_different(name, "data");
    if (0 == strcmp(name, "bufsiz"))
        name_should_be_different(name, "data_size");
}


static void
synth_call_args2(const node_t *np, node_t *result)
{
    if
    (
        node_is(np, "parameter_type_list")
    ||
        node_is(np, "parameter_list")
    )
    {
        size_t          j;

        for (j = 0; j < np->nchild; ++j)
            synth_call_args2(np->child[j], result);
    }
    if (node_is(np, "parameter_declaration"))
    {
        if (np->nchild == 2)
        {
            const char      *name;

            assert(node_is(np->child[0], "declaration_specifiers"));
            assert(node_is(np->child[1], "declarator"));
            name = extract_name_from_declarator(np->child[1]);
            make_sure_name_is_acceptable(name);
            if (result->nchild)
                node_push_back(result, node_new_literal(","));
            node_push_back(result, node_new_literal(name));
        }
    }
}


static node_t *
synth_call_args(const node_t *np)
{
    node_t          *result;

    result = node_new("call_args");
    synth_call_args2(np, result);
    return result;
}


static void
upcase_insitu(char *s)
{
    for (;;)
    {
        unsigned char   c;

        c = *s;
        if (!c)
            return;
        if (islower(c))
            *s = toupper(c);
        else if (!isalnum(c))
            *s = '_';
        ++s;
    }
}


static void
blurb_errno(FILE *fp)
{
    fprintf(fp, "  *\n");
    fprintf(fp, "  * The errno global variable will be used to obtain the\n");
    fprintf(fp, "  * error value to be decoded.\n");
}


static void
groff_errno(FILE *fp)
{
    fprintf(fp, ".PP\n");
    fprintf(fp, "The \\f[I]errno\\fP global variable will be used to obtain\n");
    fprintf(fp, "the error value to be decoded.\n");
}


static void
blurb_args(FILE *fp, node_t *call_args, const char *function_name, int section)
{
    size_t          j;

    for (j = 0; j < call_args->nchild; j += 2)
    {
        fprintf(fp, "  * @param %s\n", call_args->child[j]->literal);
        fprintf(fp, "  *     The original %s, ", call_args->child[j]->literal);
        fprintf(fp, "exactly as passed to the %s(%d) ", function_name, section);
        fprintf(fp, "system call.\n");
    }
}


static void
groff_args(FILE *fp, node_t *call_args, const char *function_name, int section)
{
    size_t          j;

    for (j = 0; j < call_args->nchild; j += 2)
    {
        fprintf(fp, ".TP 8n\n");
        fprintf(fp, "\\f[I]%s\\fP\n", call_args->child[j]->literal);
        fprintf(fp, "The original %s, ", call_args->child[j]->literal);
        fprintf(fp, "exactly as passed to the ");
        fprintf(fp, "\\f[I]%s\\fP(%d) system call.\n", function_name, section);
    }
}


static void
blurb_returns(FILE *fp)
{
    fprintf(fp, "  * @returns\n");
    fprintf(fp, "  *     The message explaining the error.  This\n");
    fprintf(fp, "  *     message buffer is shared by all libexplain\n");
    fprintf(fp, "  *     functions which do not supply a buffer in their\n");
    fprintf(fp, "  *     argument list.  This will be overwritten by the\n");
    fprintf(fp, "  *     next call to any libexplain function which shares\n");
    fprintf(fp, "  *     this buffer, including other threads.\n");
    fprintf(fp, "  * @note\n");
    fprintf(fp, "  *     This function is <b>not</b> thread safe, because\n");
    fprintf(fp, "  *     it shares a return buffer across all threads, and\n");
    fprintf(fp, "  *     many other functions in this library.\n");
}


static void
groff_returns(FILE *fp)
{
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "Returns:\n");
    fprintf(fp, "The message explaining the error.  This message buffer is\n");
    fprintf(fp, "shared by all libexplain functions which do not supply a\n");
    fprintf(fp, "buffer in their argument list.  This will be overwritten\n");
    fprintf(fp, "by the next call to any libexplain function which shares\n");
    fprintf(fp, "this buffer, including other threads.\n");
    fprintf(fp, ".PP\n");
    fprintf(fp, "\\f[B]Note:\\fP\n");
    fprintf(fp, "This function is \\f[B]not\\fP thread safe, because it\n");
    fprintf(fp, "shares a return buffer across all threads, and many other\n");
    fprintf(fp, "functions in this library.\n");
}


static void
blurb_errnum(FILE *fp)
{
    fprintf(fp, "  * @param errnum\n");
    fprintf(fp, "  *     The error value to be decoded, usually obtained\n");
    fprintf(fp, "  *     from the errno global variable just before this\n");
    fprintf(fp, "  *     function is called.  This is necessary if you need\n");
    fprintf(fp, "  *     to call <b>any</b> code between the system call to\n");
    fprintf(fp, "  *     be explained and this function, because many libc\n");
    fprintf(fp, "  *     functions will alter the value of errno.\n");
}


static void
groff_errnum(FILE *fp)
{
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]errnum\\fP\n");
    fprintf(fp, "The error value to be decoded, usually obtained from\n");
    fprintf(fp, "the \\f[I]errno\\fP global variable just before this\n");
    fprintf(fp, "function is called.  This is necessary if you need to call\n");
    fprintf(fp, "\\f[B]any\\fP code between the system call to be explained\n");
    fprintf(fp, "and this function, because many libc functions will alter\n");
    fprintf(fp, "the value of \\f[I]errno\\fP.\n");
}


static void
blurb_message(FILE *fp)
{
    fprintf(fp, "  * @param message\n");
    fprintf(fp, "  *     The location in which to store the returned\n");
    fprintf(fp, "  *     message.  If a suitable message return buffer is\n");
    fprintf(fp, "  *     supplied, this function is thread safe.\n");
    fprintf(fp, "  * @param message_size\n");
    fprintf(fp, "  *     The size in bytes of the location in which to\n");
    fprintf(fp, "  *     store the returned message.\n");
}


static void
groff_message(FILE *fp)
{
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]message\\fP\n");
    fprintf(fp, "The location in which to store the returned message.\n");
    fprintf(fp, "If a suitable message return buffer is supplied, this\n");
    fprintf(fp, "function is thread safe.\n");
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]message_size\\fP\n");
    fprintf(fp, "The size in bytes of the location in which to\n");
    fprintf(fp, "store the returned message.\n");
}


static void
groff_footer(FILE *fp)
{
    time_t          now;
    struct tm       *tmp;
    struct passwd   *pw;

    time(&now);
    tmp = localtime(&now);
    pw = getpwuid(getuid());

    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SH COPYRIGHT\n");
    fprintf(fp, ".so etc/version.so\n");
    fprintf(fp, ".if n .ds C) (C)\n");
    fprintf(fp, ".if t .ds C) \\(co\n");
    fprintf(fp, "libexplain version \\*(v)\n");
    fprintf(fp, ".br\n");
    fprintf
    (
        fp,
        "Copyright \\*(C) %d %s\n",
        tmp->tm_year + 1900,
        pw ? pw->pw_gecos : "you"
    );
}


static void
new_file(const char *filename)
{
    char            command[1000];

    fprintf(stderr, "creating %s...\n", filename);
    snprintf
    (
        command,
        sizeof(command),
        "aegis --new-file '%s' > /dev/null 2>&1",
        filename
    );
    system(command);
}


static void
copy_file(const char *filename)
{
    char            command[1000];

    fprintf(stderr, "copying %s...\n", filename);
    snprintf
    (
        command,
        sizeof(command),
        "aegis --copy-file '%s' > /dev/null 2>&1",
        filename
    );
    system(command);
}


static void
vim_line(FILE *fp, const char *before, const char *after)
{
    fprintf(fp, "%s vim:ts=8:sw=4:et", before);
    if (after && *after)
        fprintf(fp, " %s", after);
    fprintf(fp, "\n");
}


void
generate(node_t *declspec, node_t *decl)
{
    const char      *function_name;
    node_t          *args;
    node_t          *call_args;
    FILE            *fp;
    size_t           j;
    int             section;
    char            filename[1000];
    const char      *opengroup_url;

    assert(node_is(declspec, "declaration_specifiers"));
    assert(node_is(decl, "declarator"));

    function_name = find_function_name(decl);
    section = try_to_guess_section(function_name);
    try_to_get_synopsis(function_name, section);
    args = find_argument_list(decl);
    call_args = synth_call_args(args);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "libexplain/%s.h", function_name);
    new_file(filename);
    fp = libexplain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    upcase_insitu(filename);
    fprintf(fp, "#ifndef %s\n", filename);
    fprintf(fp, "#define %s\n", filename);

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    fprintf(fp, "  * @file\n");
    fprintf(fp, "  * @brief explain %s(%d) errors\n", function_name, section);
    fprintf(fp, "  */\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/warn_unused_result.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#ifdef __cplusplus\n");
    fprintf(fp, "extern \"C\" {\n");
    fprintf(fp, "#endif\n");

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    fprintf(fp, "  * The libexplain_%s_or_die function is\n", function_name);
    fprintf(fp, "  * used to call the %s(%d) system call.  On\n", function_name,
        section);
    fprintf(fp, "  * failure an explanation will be printed to stderr,\n");
    fprintf(fp, "  * obtained from libexplain_%s(3), and\n", function_name);
    fprintf(fp, "  * then the process terminates by calling\n");
    fprintf(fp, "  * exit(EXIT_FAILURE).\n");
    fprintf(fp, "  *\n");
    fprintf(fp, "  * This function is intended to be used in a fashion\n");
    fprintf(fp, "  * similar to the following example:\n");
    fprintf(fp, "  * @code\n");
    fprintf(fp, "  * libexplain_%s_or_die(", function_name);
    node_print(call_args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, "  * @endcode\n");
    fprintf(fp, "  *\n");
    for (j = 0; j < call_args->nchild; j += 2)
    {
        fprintf(fp, "  * @param %s\n", call_args->child[j]->literal);
        fprintf(fp, "  *     The %s, ", call_args->child[j]->literal);
        fprintf(fp, "exactly as to be passed to the %s(%d) ", function_name,
            section);
        fprintf(fp, "system call.\n");
    }
    fprintf(fp, "  * @returns\n");
    fprintf(fp, "  *     This function only returns on success.\n");
    fprintf(fp, "  *     On failure, prints an explanation and exits,\n");
    fprintf(fp, "  *     it does not return.\n");
    fprintf(fp, "  */\n");
    fprintf(fp, "void libexplain_%s_or_die(", function_name);
    node_print(args, fp);
    fprintf(fp, ");\n");

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    fprintf(fp, "  * The libexplain_%s function is used to\n", function_name);
    fprintf(fp, "  * obtain an explanation of an error returned by the\n");
    fprintf(fp, "  * %s(%d) system call.\n", function_name, section);
    fprintf(fp, "  * The least the message will contain is the value of\n");
    fprintf(fp, "  * strerror(errno), but usually it will do much better,\n");
    fprintf(fp, "  * and indicate the underlying cause in more detail.\n");
    blurb_errno(fp);
    fprintf(fp, "  *\n");
    fprintf(fp, "  * This function is intended to be used in a fashion\n");
    fprintf(fp, "  * similar to the following example:\n");
    fprintf(fp, "  * @code\n");
    fprintf(fp, "  * if (%s(", function_name);
    node_print(call_args, fp);
    fprintf(fp, ") < 0)\n");
    fprintf(fp, "  * {\n");
    fprintf(fp, "  *     fprintf(stderr, \"%%s\\n\", ");
    fprintf(fp, "libexplain_%s(", function_name);
    node_print(call_args, fp);
    fprintf(fp, "));\n");
    fprintf(fp, "  *     exit(EXIT_FAILURE);\n");
    fprintf(fp, "  * }\n");
    fprintf(fp, "  * @endcode\n");
    fprintf(fp, "  *\n");
    fprintf(fp, "  * The above code example is available pre-packaged as\n");
    fprintf(fp, "  * the #libexplain_%s_or_die function.\n", function_name);
    fprintf(fp, "  *\n");
    blurb_args(fp, call_args, function_name, section);
    blurb_returns(fp);
    fprintf(fp, "  */\n");
    fprintf(fp, "const char *libexplain_%s(", function_name);
    node_print(args, fp);
    fprintf(fp, ")\n");
    fprintf(fp, "%80s\n", "LIBEXPLAIN_WARN_UNUSED_RESULT;");

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    fprintf(fp, "  * The libexplain_errno_%s function is\n", function_name);
    fprintf(fp, "  * used to obtain an explanation of an error returned by\n");
    fprintf(fp, "  * the %s(%d) system call.\n", function_name, section);
    fprintf(fp, "  * The least the message will contain is the value of\n");
    fprintf(fp, "  * strerror(errnum), but usually it will do much better,\n");
    fprintf(fp, "  * and indicate the underlying cause in more detail.\n");
    fprintf(fp, "  *\n");
    fprintf(fp, "  * This function is intended to be used in a fashion\n");
    fprintf(fp, "  * similar to the following example:\n");
    fprintf(fp, "  * @code\n");
    fprintf(fp, "  * if (%s(", function_name);
    node_print(call_args, fp);
    fprintf(fp, ") < 0)\n");
    fprintf(fp, "  * {\n");
    fprintf(fp, "  *     int err = errno;\n");
    fprintf(fp, "  *     fprintf(stderr, \"%%s\\n\", ");
    fprintf(fp, "libexplain_%s(err, ", function_name);
    node_print(call_args, fp);
    fprintf(fp, "));\n");
    fprintf(fp, "  *     exit(EXIT_FAILURE);\n");
    fprintf(fp, "  * }\n");
    fprintf(fp, "  * @endcode\n");
    fprintf(fp, "  *\n");
    fprintf(fp, "  * The above code example is available pre-packaged as\n");
    fprintf(fp, "  * the #libexplain_%s_or_die function.\n", function_name);
    fprintf(fp, "  *\n");
    blurb_errnum(fp);
    blurb_args(fp, call_args, function_name, section);
    blurb_returns(fp);
    fprintf(fp, "  */\n");
    fprintf(fp, "const char *libexplain_errno_%s(int errnum, ", function_name);
    node_print(args, fp);
    fprintf(fp, ")\n");
    fprintf(fp, "%80s\n", "LIBEXPLAIN_WARN_UNUSED_RESULT;");

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    fprintf(fp, "  * The libexplain_message_%s function is\n", function_name);
    fprintf(fp, "  * used to obtain an explanation of an error returned by\n");
    fprintf(fp, "  * the %s(%d) system call.\n", function_name, section);
    fprintf(fp, "  * The least the message will contain is the value of\n");
    fprintf(fp, "  * strerror(errno), but usually it will do much better,\n");
    fprintf(fp, "  * and indicate the underlying cause in more detail.\n");
    blurb_errno(fp);
    fprintf(fp, "  *\n");
    fprintf(fp, "  * This function is intended to be used in a fashion\n");
    fprintf(fp, "  * similar to the following example:\n");
    fprintf(fp, "  * @code\n");
    fprintf(fp, "  * if (%s(", function_name);
    node_print(call_args, fp);
    fprintf(fp, ") < 0)\n");
    fprintf(fp, "  * {\n");
    fprintf(fp, "  *     char message[3000];\n");
    fprintf(fp, "  *     libexplain_message_%s(", function_name);
    fprintf(fp, "message, sizeof(message), ");
    node_print(call_args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, "  *     fprintf(stderr, \"%%s\\n\", message);\n");
    fprintf(fp, "  *     exit(EXIT_FAILURE);\n");
    fprintf(fp, "  * }\n");
    fprintf(fp, "  * @endcode\n");
    fprintf(fp, "  *\n");
    fprintf(fp, "  * The above code example is available pre-packaged as\n");
    fprintf(fp, "  * the #libexplain_%s_or_die function.\n", function_name);
    fprintf(fp, "  *\n");
    blurb_message(fp);
    blurb_args(fp, call_args, function_name, section);
    fprintf(fp, "  */\n");
    fprintf(fp, "void libexplain_message_%s(", function_name);
    fprintf(fp, "char *message, int message_size, ");
    node_print(args, fp);
    fprintf(fp, ");\n");

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    fprintf(fp, "  * The libexplain_message_errno_%s\n", function_name);
    fprintf(fp, "  * function is used to obtain an explanation of an error\n");
    fprintf(fp, "  * returned by the\n");
    fprintf(fp, "  * %s(%d) system call.\n", function_name, section);
    fprintf(fp, "  * The least the message will contain is the value of\n");
    fprintf(fp, "  * strerror(errnum), but usually it will do much better,\n");
    fprintf(fp, "  * and indicate the underlying cause in more detail.\n");
    fprintf(fp, "  *\n");
    fprintf(fp, "  * This function is intended to be used in a fashion\n");
    fprintf(fp, "  * similar to the following example:\n");
    fprintf(fp, "  * @code\n");
    fprintf(fp, "  * if (%s(", function_name);
    node_print(call_args, fp);
    fprintf(fp, ") < 0)\n");
    fprintf(fp, "  * {\n");
    fprintf(fp, "  *     int err = errno;\n");
    fprintf(fp, "  *     char message[3000];\n");
    fprintf(fp, "  *     libexplain_message_errno_%s(", function_name);
    fprintf(fp, "message, sizeof(message), err, ");
    node_print(call_args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, "  *     fprintf(stderr, \"%%s\\n\", message);\n");
    fprintf(fp, "  *     exit(EXIT_FAILURE);\n");
    fprintf(fp, "  * }\n");
    fprintf(fp, "  * @endcode\n");
    fprintf(fp, "  *\n");
    fprintf(fp, "  * The above code example is available pre-packaged as\n");
    fprintf(fp, "  * the #libexplain_%s_or_die function.\n", function_name);
    fprintf(fp, "  *\n");
    blurb_message(fp);
    blurb_errnum(fp);
    blurb_args(fp, call_args, function_name, section);
    fprintf(fp, "  */\n");
    fprintf(fp, "void libexplain_message_errno_%s(", function_name);
    fprintf(fp, "char *message, int message_size, int errnum, ");
    node_print(args, fp);
    fprintf(fp, ");\n");

    fprintf(fp, "\n");
    fprintf(fp, "#ifdef __cplusplus\n");
    fprintf(fp, "}\n");
    fprintf(fp, "#endif\n");

    vim_line(fp, "\n/*", "*/");
    fprintf(fp, "#endif /* %s */\n", filename);
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "man/man3/libexplain_%s.3",
        function_name);
    new_file(filename);
    fp = libexplain_fopen_or_die(filename, "w");
    groff_license_header(fp);
    fprintf(fp, ".ds n) libexplain_%s\n", function_name);
    fprintf(fp, ".TH libexplain_%s 3\n", function_name);
    fprintf(fp, ".SH NAME\n");
    fprintf(fp, "libexplain_%s \\- explain %s(%d) errors\n", function_name,
        function_name, section);
    fprintf(fp, ".XX \"libexplain_%s(3)\" \"explain %s(%d) errors\"\n",
        function_name, function_name, section);
    fprintf(fp, ".SH SYNOPSIS\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, ".sp 0.3\n");
    fprintf(fp, "const char *libexplain_%s(", function_name);
    node_print(args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, ".br\n");
    fprintf(fp, "const char *libexplain_errno_%s(", function_name);
    fprintf(fp, "int errnum, ");
    node_print(args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, ".br\n");
    fprintf(fp, "void libexplain_message_%s(", function_name);
    fprintf(fp, "char *message, int message_size, ");
    node_print(args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, ".br\n");
    fprintf(fp, "void libexplain_message_errno_%s(", function_name);
    fprintf(fp, "char *message, int message_size, int errnum, ");
    node_print(args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, ".SH DESCRIPTION\n");
    fprintf(fp, "These functions may be used to obtain explanations for\n");
    fprintf(fp, "errors returned by the \\f[I]%s\\fP(%d) system call.\n",
        function_name, section);
    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SS libexplain_%s\n", function_name);
    fprintf(fp, "const char *libexplain_%s(", function_name);
    node_print(args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, ".PP\n");
    fprintf(fp, "The \\f[B]libexplain_%s\\fP function is\n", function_name);
    fprintf(fp, "used to obtain an explanation of an error returned by the ");
    fprintf(fp, "\\f[I]%s\\fP(%d)\n", function_name, section);
    fprintf(fp, "system call.  The least the message will contain is the\n");
    fprintf(fp, "value of \\f[CW]strerror(errno)\\fP, but usually it will\n");
    fprintf(fp, "do much better, and indicate the underlying cause in more\n");
    fprintf(fp, "detail.\n");
    groff_errno(fp);
    fprintf(fp, ".PP\n");
    fprintf(fp, "This function is intended to be used in a fashion\n");
    fprintf(fp, "similar to the following example:\n");
    fprintf(fp, ".RS\n");
    fprintf(fp, ".ft CW\n");
    fprintf(fp, ".nf\n");
    fprintf(fp, "if (%s(", function_name);
    node_print(call_args, fp);
    fprintf(fp, ") < 0)\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    fprintf(stderr, \"%%s\\en\", ");
    fprintf(fp, "libexplain_%s(", function_name);
    node_print(call_args, fp);
    fprintf(fp, "));\n");
    fprintf(fp, "    exit(EXIT_FAILURE);\n");
    fprintf(fp, "}\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ft R\n");
    fprintf(fp, ".RE\n");
    fprintf(fp, ".PP\n");
    fprintf(fp, "The above code example is available pre-packaged as the\n");
    fprintf(fp, "\\f[I]libexplain_%s_or_die\\fP(3) function.\n", function_name);
    groff_args(fp, call_args, function_name, section);
    groff_returns(fp);
    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SS libexplain_errno_%s\n", function_name);
    fprintf(fp, "const char *libexplain_errno_%s(int errnum, ", function_name);
    node_print(args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, ".PP\n");
    fprintf(fp, "The \\f[B]libexplain_errno_%s\\fP function\n", function_name);
    fprintf(fp, "is used to obtain an explanation of an error returned by\n");
    fprintf(fp, "the \\f[I]%s\\fP(%d) system call.\n", function_name, section);
    fprintf(fp, "The least the message will contain is the value of\n");
    fprintf(fp, "\\f[CW]strerror(errnum)\\fP, but usually it will do much\n");
    fprintf(fp, "better, and indicate the underlying cause in more detail.\n");
    fprintf(fp, ".PP\n");
    fprintf(fp, "This function is intended to be used in a fashion\n");
    fprintf(fp, "similar to the following example:\n");
    fprintf(fp, ".RS\n");
    fprintf(fp, ".ft CW\n");
    fprintf(fp, ".nf\n");
    fprintf(fp, "if (%s(", function_name);
    node_print(call_args, fp);
    fprintf(fp, ") < 0)\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    int err = errno;\n");
    fprintf(fp, "    fprintf(stderr, \"%%s\\en\", ");
    fprintf(fp, "libexplain_errno_%s(err, ", function_name);
    node_print(call_args, fp);
    fprintf(fp, "));\n");
    fprintf(fp, "    exit(EXIT_FAILURE);\n");
    fprintf(fp, "}\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ft R\n");
    fprintf(fp, ".RE\n");
    fprintf(fp, ".PP\n");
    fprintf(fp, "The above code example is available pre-packaged as the\n");
    fprintf(fp, "\\f[I]libexplain_%s_or_die\\fP(3) function.\n", function_name);
    groff_errnum(fp);
    groff_args(fp, call_args, function_name, section);
    groff_returns(fp);
    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SS libexplain_message_%s\n", function_name);
    fprintf(fp, "void libexplain_message_%s(", function_name);
    fprintf(fp, "char *message, int message_size, ");
    node_print(args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, ".PP\n");
    fprintf(fp, "The \\f[B]libexplain_message_%s\\fP\n", function_name);
    fprintf(fp, "function may be used to  obtain an explanation of an error\n");
    fprintf(fp, "returned by the\n");
    fprintf(fp, "\\f[I]%s\\fP(%d) system call.\n", function_name, section);
    fprintf(fp, "The least the message will contain is the value of\n");
    fprintf(fp, "\\f[CW]strerror(errno)\\fP, but usually it will do much\n");
    fprintf(fp, "better, and indicate the underlying cause in more detail.\n");
    groff_errno(fp);
    fprintf(fp, ".PP\n");
    fprintf(fp, "This function is intended to be used in a fashion\n");
    fprintf(fp, "similar to the following example:\n");
    fprintf(fp, ".RS\n");
    fprintf(fp, ".ft CW\n");
    fprintf(fp, ".nf\n");
    fprintf(fp, "if (%s(", function_name);
    node_print(call_args, fp);
    fprintf(fp, ") < 0)\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    char message[3000];\n");
    fprintf(fp, "    libexplain_message_%s(", function_name);
    fprintf(fp, "message, sizeof(message), ");
    node_print(call_args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, "    fprintf(stderr, \"%%s\\en\", message);\n");
    fprintf(fp, "    exit(EXIT_FAILURE);\n");
    fprintf(fp, "}\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ft R\n");
    fprintf(fp, ".RE\n");
    fprintf(fp, ".PP\n");
    fprintf(fp, "The above code example is available pre-packaged as the\n");
    fprintf(fp, "\\f[I]libexplain_%s_or_die\\fP(3) function.\n", function_name);
    groff_message(fp);
    groff_args(fp, call_args, function_name, section);
    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SS libexplain_message_errno_%s\n", function_name);
    fprintf(fp, "void libexplain_message_errno_%s(", function_name);
    fprintf(fp, "char *message, int message_size, int errnum, ");
    node_print(args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, ".PP\n");
    fprintf(fp, "The \\f[B]libexplain_message_errno_%s\\fP\n", function_name);
    fprintf(fp, "function may be used to obtain an explanation of an error\n");
    fprintf(fp, "returned by the\n");
    fprintf(fp, "\\f[I]%s\\fP(%d) system call.\n", function_name, section);
    fprintf(fp, "The least the message will contain is the value of\n");
    fprintf(fp, "\\f[CW]strerror(errnum)\\fP, but usually it will do much\n");
    fprintf(fp, "better, and indicate the underlying cause in more detail.\n");
    fprintf(fp, ".PP\n");
    fprintf(fp, "This function is intended to be used in a fashion\n");
    fprintf(fp, "similar to the following example:\n");
    fprintf(fp, ".RS\n");
    fprintf(fp, ".ft CW\n");
    fprintf(fp, ".nf\n");
    fprintf(fp, "if (%s(", function_name);
    node_print(call_args, fp);
    fprintf(fp, ") < 0)\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    int err = errno;\n");
    fprintf(fp, "    char message[3000];\n");
    fprintf(fp, "    libexplain_message_errno_%s(", function_name);
    fprintf(fp, "message, sizeof(message), err, ");
    node_print(call_args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, "    fprintf(stderr, \"%%s\\en\", message);\n");
    fprintf(fp, "    exit(EXIT_FAILURE);\n");
    fprintf(fp, "}\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ft R\n");
    fprintf(fp, ".RE\n");
    fprintf(fp, ".PP\n");
    fprintf(fp, "The above code example is available pre-packaged as the\n");
    fprintf(fp, "\\f[I]libexplain_%s_or_die\\fP(3) function.\n", function_name);
    groff_message(fp);
    groff_errnum(fp);
    groff_args(fp, call_args, function_name, section);
    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SH SEE ALSO\n");
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]%s\\fP(%d)\n", function_name, section);
    fprintf(fp, "%s\n", synopsis);
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]libexplain_%s_or_die\\fP(3)\n", function_name);
    fprintf(fp, "%s and report errors\n", synopsis);
    groff_footer(fp);
    vim_line(fp, ".\\\"", 0);
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "man/man3/libexplain_%s_or_die.3",
        function_name);
    new_file(filename);
    fp = libexplain_fopen_or_die(filename, "w");
    groff_license_header(fp);
    fprintf(fp, ".ds n) libexplain_%s_or_die\n", function_name);
    fprintf(fp, ".TH libexplain_%s_or_die 3\n", function_name);
    fprintf(fp, ".SH NAME\n");
    fprintf(fp, "libexplain_%s_or_die \\- ", function_name);
    fprintf(fp, "%s and report errors\n", synopsis);
    fprintf(fp, ".XX \"libexplain_%s_or_die(3)\" \\\n", function_name);
    fprintf(fp, "    \"%s and report errors\"\n", synopsis);
    fprintf(fp, ".SH SYNOPSIS\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, ".sp 0.3\n");
    fprintf(fp, "void libexplain_%s_or_die(", function_name);
    node_print(args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, ".SH DESCRIPTION\n");
    fprintf(fp, "The \\f[B]libexplain_%s_or_die\\fP function\n", function_name);
    fprintf(fp, "is used to call the \\f[I]%s\\fP(%d) system\n", function_name,
        section);
    fprintf(fp, "call.  On failure an explanation will be\n");
    fprintf(fp, "printed to \\f[I]stderr\\fP,\n");
    fprintf(fp, "obtained from \\f[I]libexplain_%s\\fP(3),\n", function_name);
    fprintf(fp, "and then the process terminates\n");
    fprintf(fp, "by calling \\f[CW]exit(EXIT_FAILURE)\\fP.\n");
    fprintf(fp, ".PP\n");
    fprintf(fp, "This function is intended to be used in a fashion\n");
    fprintf(fp, "similar to the following example:\n");
    fprintf(fp, ".RS\n");
    fprintf(fp, ".ft CW\n");
    fprintf(fp, ".nf\n");
    fprintf(fp, "libexplain_%s_or_die(", function_name);
    node_print(call_args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ft R\n");
    fprintf(fp, ".RE\n");
    for (j = 0; j < call_args->nchild; j += 2)
    {
        fprintf(fp, ".TP 8n\n");
        fprintf(fp, "\\f[I]%s\\fP\n", call_args->child[j]->literal);
        fprintf(fp, "The %s, exactly as ", call_args->child[j]->literal);
        fprintf(fp, "to be passed to the \\f[I]%s\\fP(%d) ", function_name,
            section);
        fprintf(fp, "system call.\n");
    }
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "Returns:\n");
    fprintf(fp, "This function only returns on success.\n");
    fprintf(fp, "On failure, prints an explanation and exits.\n");
    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SH SEE ALSO\n");
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]%s\\fP(%d)\n", function_name, section);
    fprintf(fp, "%s\n", synopsis);
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]libexplain_%s\\fP(3)\n", function_name);
    fprintf(fp, "explain \\f[I]%s\\fP(%d) errors\n", function_name, section);
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]exit\\fP(2)\n");
    fprintf(fp, "terminate the calling process\n");
    groff_footer(fp);
    vim_line(fp, ".\\\"", 0);
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "libexplain/%s_or_die.c",
        function_name);
    new_file(filename);
    fp = libexplain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/ac/stdio.h>\n");
    fprintf(fp, "#include <libexplain/ac/stdlib.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "#include <libexplain/wrap_and_print.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "void\n");
    fprintf(fp, "libexplain_%s_or_die(", function_name);
    node_print(args, fp);
    fprintf(fp, ")\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    if (%s(", function_name);
    node_print(call_args, fp);
    fprintf(fp, ") < 0)\n");
    fprintf(fp, "    {\n");
    fprintf(fp, "        libexplain_wrap_and_print(stderr, ");
    fprintf(fp, "libexplain_%s(", function_name);
    node_print(call_args, fp);
    fprintf(fp, "));\n");
    fprintf(fp, "        exit(EXIT_FAILURE);\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "}\n");
    vim_line(fp, "\n/*", "*/");
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "libexplain/%s.c", function_name);
    new_file(filename);
    fp = libexplain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/ac/errno.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "const char *\n");
    fprintf(fp, "libexplain_%s(", function_name);
    node_print(args, fp);
    fprintf(fp, ")\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    return libexplain_errno_%s(errno, ", function_name);
    node_print(call_args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, "}\n");
    vim_line(fp, "\n/*", "*/");
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "libexplain/errno/%s.c",
        function_name);
    new_file(filename);
    fp = libexplain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/common_message_buffer.h>\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "const char *\n");
    fprintf(fp, "libexplain_errno_%s(int errnum, ", function_name);
    node_print(args, fp);
    fprintf(fp, ")\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    libexplain_message_errno_%s\n", function_name);
    fprintf(fp, "    (\n");
    fprintf(fp, "        libexplain_common_message_buffer,\n");
    fprintf(fp, "        libexplain_common_message_buffer_size,\n");
    fprintf(fp, "        errnum");
    for (j = 0; j < call_args->nchild; j += 2)
        fprintf(fp, ",\n        %s", call_args->child[j]->literal);
    fprintf(fp, "\n");
    fprintf(fp, "    );\n");
    fprintf(fp, "    return libexplain_common_message_buffer;\n");
    fprintf(fp, "}\n");
    vim_line(fp, "\n/*", "*/");
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "libexplain/message/%s.c",
        function_name);
    new_file(filename);
    fp = libexplain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/ac/errno.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "void\n");
    fprintf(fp, "libexplain_message_%s(", function_name);
    fprintf(fp, "char *message, int message_size, ");
    node_print(args, fp);
    fprintf(fp, ")\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    libexplain_message_errno_%s\n", function_name);
    fprintf(fp, "    (\n");
    fprintf(fp, "        message,\n");
    fprintf(fp, "        message_size,\n");
    fprintf(fp, "        errno");
    for (j = 0; j < call_args->nchild; j += 2)
        fprintf(fp, ",\n        %s", call_args->child[j]->literal);
    fprintf(fp, "\n");
    fprintf(fp, "    );\n");
    fprintf(fp, "}\n");
    vim_line(fp, "\n/*", "*/");
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "libexplain/message/errno/%s.c",
        function_name);
    new_file(filename);
    fp = libexplain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/buffer/errno/%s.h>\n", function_name);
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "void\n");
    fprintf(fp, "libexplain_message_errno_%s(", function_name);
    fprintf(fp, "char *message, int message_size, int errnum, ");
    node_print(args, fp);
    fprintf(fp, ")\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    libexplain_string_buffer_t sb;\n");
    fprintf(fp, "\n");
    fprintf(fp, "    libexplain_string_buffer_init(&sb, message, ");
    fprintf(fp, "message_size);\n");
    fprintf(fp, "    libexplain_buffer_errno_%s(", function_name);
    fprintf(fp, "&sb, errnum, ");
    node_print(call_args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, "}\n");
    vim_line(fp, "\n/*", "*/");
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "libexplain/buffer/errno/%s.h",
        function_name);
    new_file(filename);
    fp = libexplain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    upcase_insitu(filename);
    fprintf(fp, "\n");
    fprintf(fp, "#ifndef %s\n", filename);
    fprintf(fp, "#define %s\n", filename);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/string_buffer.h>\n");

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    fprintf(fp, "  * The libexplain_buffer_errno_%s function\n", function_name);
    fprintf(fp, "  * is used to obtain an explanation of an error returned\n");
    fprintf(fp, "  * by the %s(%d) system call.\n", function_name, section);
    fprintf(fp, "  * The least the message will contain is the value of\n");
    fprintf(fp, "  * strerror(errnum), but usually it will do much better,\n");
    fprintf(fp, "  * and indicate the underlying cause in more detail.\n");
    fprintf(fp, "  *\n");
    fprintf(fp, "  * @param sb\n");
    fprintf(fp, "  *     The string buffer to print the message into.  If a\n");
    fprintf(fp, "  *     safe buffer is specified, this function is thread\n");
    fprintf(fp, "  *     safe.\n");
    blurb_errnum(fp);
    blurb_args(fp, call_args, function_name, section);
    fprintf(fp, "  */\n");
    fprintf(fp, "void libexplain_buffer_errno_%s(", function_name);
    fprintf(fp, "libexplain_string_buffer_t *sb, int errnum, ");
    node_print(args, fp);
    fprintf(fp, ");\n");

    vim_line(fp, "\n/*", "*/");
    fprintf(fp, "#endif /* %s */\n", filename);
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "libexplain/buffer/errno/%s.c",
        function_name);
    new_file(filename);
    fp = libexplain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/buffer/errno/generic.h>\n");
    fprintf(fp, "#include <libexplain/buffer/errno/%s.h>\n", function_name);
    fprintf(fp, "#include <libexplain/explanation.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "static void\n");
    fprintf(fp, "libexplain_buffer_errno_%s_system_call(", function_name);
    fprintf(fp, "libexplain_string_buffer_t *sb, int errnum, ");
    node_print(args, fp);
    fprintf(fp, ")\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    libexplain_string_buffer_puts(sb, \"%s(\");\n",
        function_name);
    for (j = 0; j < call_args->nchild; j += 2)
    {
        if (j)
            fprintf(fp, "    libexplain_string_buffer_puts(sb, \", \");\n");
        fprintf
        (
            fp,
            "    libexplain_string_buffer_printf(sb, \"%s = %%d\", %s);\n",
            call_args->child[j]->literal,
            call_args->child[j]->literal
        );
    }
    fprintf(fp, "    libexplain_string_buffer_putc(sb, ')');\n");
    fprintf(fp, "}\n");

    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "static void\n");
    fprintf(fp, "libexplain_buffer_errno_%s_explanation(", function_name);
    fprintf(fp, "libexplain_string_buffer_t *sb, int errnum, ");
    node_print(args, fp);
    fprintf(fp, ")\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    /*\n");
    opengroup_url = "http://www.opengroup.org/onlinepubs/009695399";
    fprintf(fp, "     * %s/functions/%s.html\n", opengroup_url, function_name);
    fprintf(fp, "     */\n");
    fprintf(fp, "    switch (errnum)\n");
    fprintf(fp, "    {\n");
    look_for_error_cases(function_name, section, fp);
    fprintf(fp, "    default:\n");
    fprintf(fp, "        libexplain_buffer_errno_generic(sb, errnum);\n");
    fprintf(fp, "        break;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "}\n");

    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "void\n");
    fprintf(fp, "libexplain_buffer_errno_%s(", function_name);
    fprintf(fp, "libexplain_string_buffer_t *sb, int errnum, ");
    node_print(args, fp);
    fprintf(fp, ")\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    libexplain_explanation_t exp;\n");
    fprintf(fp, "\n");
    fprintf(fp, "    libexplain_explanation_init(&exp, errnum);\n");
    fprintf(fp, "    libexplain_buffer_errno_%s_system_call\n", function_name);
    fprintf(fp, "    (\n");
    fprintf(fp, "        &exp.system_call_sb,\n");
    fprintf(fp, "        errnum");
    for (j = 0; j < call_args->nchild; j += 2)
        fprintf(fp, ",\n        %s", call_args->child[j]->literal);
    fprintf(fp, "\n");
    fprintf(fp, "    );\n");
    fprintf(fp, "    libexplain_buffer_errno_%s_explanation\n", function_name);
    fprintf(fp, "    (\n");
    fprintf(fp, "        &exp.explanation_sb,\n");
    fprintf(fp, "        errnum");
    for (j = 0; j < call_args->nchild; j += 2)
        fprintf(fp, ",\n        %s", call_args->child[j]->literal);
    fprintf(fp, "\n");
    fprintf(fp, "    );\n");
    fprintf(fp, "    libexplain_explanation_assemble(&exp, sb);\n");
    fprintf(fp, "}\n");
    vim_line(fp, "\n/*", "*/");
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "test_%s/main.c", function_name);
    new_file(filename);
    fp = libexplain_fopen_or_die(filename, "w");
    generate_gpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/ac/stdio.h>\n");
    fprintf(fp, "#include <libexplain/ac/stdlib.h>\n");
    fprintf(fp, "#include <libexplain/ac/unistd.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "#include <libexplain/version_print.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "static void\n");
    fprintf(fp, "usage(void)\n");
    fprintf(fp, "{\n");
    fprintf(fp, "    fprintf(stderr, \"Usage: test_%s", function_name);
    for (j = 0; j < call_args->nchild; j += 2)
    {
        fprintf(fp, " <%s>", call_args->child[j]->literal);
    }
    fprintf(fp, "\\n\");\n");
    fprintf
    (
        fp,
        "    fprintf(stderr, \"       test_%s -V\\n\");\n",
        function_name
    );
    fprintf(fp, "    exit(EXIT_FAILURE);\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "int\n");
    fprintf(fp, "main(int argc, char **argv)\n");
    fprintf(fp, "{\n");
    for (j = 0; j < call_args->nchild; j += 2)
    {
        fprintf(fp, "    const char      *%s;\n", call_args->child[j]->literal);
    }
    fprintf(fp, "\n");
    fprintf(fp, "    for (;;)\n");
    fprintf(fp, "    {\n");
    fprintf(fp, "        int c = getopt(argc, argv, \"V\");\n");
    fprintf(fp, "        if (c == EOF)\n");
    fprintf(fp, "            break;\n");
    fprintf(fp, "        switch (c)\n");
    fprintf(fp, "        {\n");
    fprintf(fp, "        case 'V':\n");
    fprintf(fp, "            libexplain_version_print();\n");
    fprintf(fp, "            return EXIT_SUCCESS;\n");
    fprintf(fp, "\n");
    fprintf(fp, "        default:\n");
    fprintf(fp, "            usage();\n");
    fprintf(fp, "        }\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    if (optind + %d != argc)\n", (int)(j / 2));
    fprintf(fp, "        usage();\n");
    for (j = 0; j < call_args->nchild; j += 2)
    {
        fprintf(fp, "    %s = argv[optind", call_args->child[j]->literal);
        if (j)
            fprintf(fp, " + %d", (int)(j / 2));
        fprintf(fp, "];\n");
    }
    fprintf(fp, "\n");
    fprintf(fp, "    libexplain_%s_or_die(", function_name);
    node_print(call_args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, "    return EXIT_SUCCESS;\n");
    fprintf(fp, "}\n");
    vim_line(fp, "\n/*", "*/");
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "explain/%s.c", function_name);
    new_file(filename);
    fp = libexplain_fopen_or_die(filename, "w");
    generate_gpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/ac/stdio.h>\n");
    fprintf(fp, "#include <libexplain/ac/stdlib.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "#include <libexplain/wrap_and_print.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <explain/%s.h>\n", function_name);
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "void\n");
    fprintf(fp, "explain_%s(int errnum, int argc, char **argv)\n",
        function_name);
    fprintf(fp, "{\n");
    for (j = 0; j < call_args->nchild; j += 2)
    {
        fprintf(fp, "    const char      *%s;\n", call_args->child[j]->literal);
    }
    fprintf(fp, "\n");
    fprintf(fp, "    if (argc != %d)\n", (int)((call_args->nchild + 1) / 2));
    fprintf(fp, "    {\n");
    fprintf
    (
        fp,
        "        fprintf(stderr, \"%s: requires %d argument%s, "
        "not %%d\\n\", argc);\n",
        function_name,
        (int)((call_args->nchild + 1) / 2),
        (call_args->nchild == 1 ? "" : "s")
    );
    fprintf(fp, "        exit(EXIT_FAILURE);\n");
    fprintf(fp, "    }\n");
    for (j = 0; j < call_args->nchild; j += 2)
    {
        fprintf(fp, "    %s = ", call_args->child[j]->literal);
        fprintf(fp, "argv[%d];\n", (int)(j / 2));
    }
    fprintf(fp, "\n");
    fprintf(fp, "    libexplain_wrap_and_print\n");
    fprintf(fp, "    (\n");
    fprintf(fp, "        stdout,\n");
    fprintf(fp, "        libexplain_errno_%s(errnum, ", function_name);
    node_print(call_args, fp);
    fprintf(fp, ")\n");
    fprintf(fp, "    );\n");
    fprintf(fp, "}\n");
    vim_line(fp, "\n/*", "*/");
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "explain/%s.h", function_name);
    new_file(filename);
    fp = libexplain_fopen_or_die(filename, "w");
    generate_gpl_header(fp);
    upcase_insitu(filename);
    fprintf(fp, "\n");
    fprintf(fp, "#ifndef %s\n", filename);
    fprintf(fp, "#define %s\n", filename);
    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    fprintf(fp, "  * The explain_%s function may be used to\n", function_name);
    fprintf(fp, "  * interpret a %s \"call\" from the\n", function_name);
    fprintf(fp, "  * command line, and then run it through\n");
    fprintf(fp, "  * libexplain_errno_%s for an explanation.\n", function_name);
    fprintf(fp, "  *\n");
    fprintf(fp, "  * @note\n");
    fprintf(fp, "  * Because the event being explain happened in another\n");
    fprintf(fp, "  * process, the results are not as good as if\n");
    fprintf(fp, "  * libexplain_errno_%s had been called\n", function_name);
    fprintf(fp, "  * from within the process that saw the error.\n");
    fprintf(fp, "  *\n");
    fprintf(fp, "  * @param errnum\n");
    fprintf(fp, "  *    The number of the error to be explained.\n");
    fprintf(fp, "  * @param argc\n");
    fprintf(fp, "  *    the number of command line arguments, corresponding\n");
    fprintf(fp, "  *    1:1 to the call arguments.\n");
    fprintf(fp, "  * @param argv\n");
    fprintf(fp, "  *    the actual command line arguments, corresponding\n");
    fprintf(fp, "  *    1:1 to the call arguments.\n");
    fprintf(fp, "  */\n");
    fprintf(fp, "void explain_%s(int errnum, int argc, char **argv);\n",
        function_name);
    vim_line(fp, "\n/*", "*/");
    fprintf(fp, "#endif /* %s */\n", filename);
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "explain/main.c");
    copy_file(filename);
    fp = libexplain_fopen_or_die(filename, "a");
    fprintf(fp, "\n");
    fprintf(fp, "#include <explain/%s.h>\n", function_name);
    fprintf(fp, "\n");
    fprintf(fp, "    { \"%s\", explain_%s },\n", function_name, function_name);
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "man/man3/libexplain.3");
    copy_file(filename);
    fp = libexplain_fopen_or_die(filename, "a");
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]libexplain_%s\\fP(3)\n", function_name);
    fprintf(fp, "Explain \\f[I]%s\\fP(%d) errors\n", function_name, section);
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]libexplain_%s_or_die\\fP(3)\n", function_name);
    fprintf(fp, "%s and report errors\n", synopsis);
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "libexplain/libexplain.h");
    copy_file(filename);
    fp = libexplain_fopen_or_die(filename, "a");
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    libexplain_fclose_or_die(fp);

    /* ********************************************************************** */

    snprintf(filename, sizeof(filename), "man/man1/explain.1");
    copy_file(filename);
    fp = libexplain_fopen_or_die(filename, "a");
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "%s", function_name);
    for (j = 0; j < call_args->nchild; j += 2)
    {
        fprintf(fp, " \\f[I]%s\\fP", call_args->child[j]->literal);
    }
    fprintf(fp, "\n");
    fprintf(fp, "The \\f[I]%s\\fP(%d) system call.\n", function_name, section);
    libexplain_fclose_or_die(fp);
}
