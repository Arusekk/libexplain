/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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
#include <libexplain/malloc.h>
#include <libexplain/sizeof.h>
#include <libexplain/strdup.h>
#include <libexplain/system.h>

#include <codegen/aegis.h>
#include <codegen/boolean.h>
#include <codegen/generate.h>
#include <codegen/header.h>
#include <codegen/string_buffer.h>
#include <codegen/wrapper.h>


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


static const char *synopsis;


static void
try_to_get_synopsis(const char *function_name, int section)
{
    const char      *cat;
    FILE            *fp;
    int             state;
    char            filename[80];
    char            command[200];
    char            text[300];

    snprintf
    (
        text,
        sizeof(text),
        "Execute \\f[I]%s\\fP(%d)",
        function_name,
        section
    );

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
            snprintf(text, sizeof(text), "%s", p);
            break;
        }
        state = 0;
    }
    pclose(fp);

    synopsis = explain_strdup_or_die(text);
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


static node_t *
look_for_error_cases(const char *function_name, int section)
{
    const char      *cat;
    FILE            *ifp;
    char            filename[80];
    char            command[200];
    node_t          *result;

    result = node_new("errors");
    node_push_back(result, node_new("upper-case"));
    node_push_back(result, node_new("lower-case"));
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
        return result;
    for (;;)
    {
        char            *cp;
        char            *ep;
        const explain_errno_info_t *eip;
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
        eip = explain_errno_info_by_name(cp);
        if (!eip)
            continue;

        node_push_back(result->child[0], node_new_literal(cp));
        downcase_insitu(cp);
        node_push_back(result->child[1], node_new_literal(cp));
    }
    pclose(ifp);

    return result;
}


static void
generate_lgpl_header(FILE *fp)
{
    lgpl_header(fp, " * ");
}


static void
generate_gpl_header(FILE *fp)
{
    gpl_header(fp, " * ");
}


static void
groff_license_header(FILE *fp)
{
    gpl_header(fp, ".\\\" ");
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
    if (0 == strcmp(name, "sockfd"))
        name_should_be_different(name, "fildes");
    if (0 == strcmp(name, "buf"))
        name_should_be_different(name, "data");
    if (0 == strcmp(name, "bufsiz"))
        name_should_be_different(name, "data_size");
    if (0 == strcmp(name, "stream"))
        name_should_be_different(name, "fp");
    if (0 == strcmp(name, "dirp"))
        name_should_be_different(name, "dir");
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
    wrapper(fp, "  * ",
        "The errno global variable will be used to obtain the error "
        "value to be decoded.");
}


static void
groff_errno(FILE *fp)
{
    fprintf(fp, ".PP\n");
    wrapper(fp, "",
        "The \\f[I]errno\\fP global variable will be used to obtain "
        "the error value to be decoded.");
}


static void
blurb_args(FILE *fp, node_t *call_args, const char *function_name, int section)
{
    size_t          j;
    string_buffer_t sb;

    string_buffer_constructor(&sb);
    for (j = 0; j < call_args->nchild; j += 2)
    {
        const char *pname = call_args->child[j]->literal;
        fprintf(fp, "  * @param %s\n", pname);
        string_buffer_rewind(&sb);
        string_buffer_puts(&sb, "The original ");
        string_buffer_puts(&sb, pname);
        string_buffer_puts(&sb, ", exactly as passed to the ");
        string_buffer_printf(&sb, "<i>%s</i>(%d)", function_name, section);
        string_buffer_puts(&sb, " system call.");
        wrapper(fp, "  *     ", string_buffer_get(&sb));
    }
    string_buffer_destructor(&sb);
}


static void
groff_args(FILE *fp, node_t *call_args, const char *function_name, int section)
{
    size_t          j;
    string_buffer_t sb;

    string_buffer_constructor(&sb);
    for (j = 0; j < call_args->nchild; j += 2)
    {
        const char *pname = call_args->child[j]->literal;
        fprintf(fp, ".TP 8n\n");
        fprintf(fp, "\\f[I]%s\\fP\n", pname);
        string_buffer_rewind(&sb);
        string_buffer_puts(&sb, "The original ");
        string_buffer_puts(&sb, pname);
        string_buffer_puts(&sb, ", exactly as passed to the ");
        string_buffer_printf(&sb, "\\f[I]%s\\fP(%d)", function_name, section);
        string_buffer_puts(&sb, " system call.");
        wrapper(fp, "", string_buffer_get(&sb));
    }
    string_buffer_destructor(&sb);
}


static void
blurb_returns(FILE *fp)
{
    fprintf(fp, "  * @returns\n");
    wrapper(fp, "  *     ",
        "The message explaining the error.  This message buffer is "
        "shared by all libexplain functions which do not supply a "
        "buffer in their argument list.  This will be overwritten by "
        "the next call to any libexplain function which shares this "
        "buffer, including other threads.");
    fprintf(fp, "  * @note\n");
    wrapper(fp, "  *     ",
        "This function is <b>not</b> thread safe, because it shares a "
        "return buffer across all threads, and many other functions in "
        "this library.");
}


static void
groff_returns(FILE *fp)
{
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "Returns:\n");
    wrapper(fp, "",
        "The message explaining the error.  This message buffer is "
        "shared by all libexplain functions which do not supply a "
        "buffer in their argument list.  This will be overwritten by "
        "the next call to any libexplain function which shares this "
        "buffer, including other threads.");
    fprintf(fp, ".PP\n");
    wrapper(fp, "",
        "\\f[B]Note:\\fP This function is \\f[B]not\\fP thread safe, "
        "because it shares a return buffer across all threads, and many "
        "other functions in this library.");
}


static void
blurb_errnum(FILE *fp)
{
    fprintf(fp, "  * @param errnum\n");
    wrapper(fp, "  *     ",
        "The error value to be decoded, usually obtained from the errno "
        "global variable just before this function is called.  This "
        "is necessary if you need to call <b>any</b> code between the "
        "system call to be explained and this function, because many "
        "libc functions will alter the value of errno.");
}


static void
groff_errnum(FILE *fp)
{
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]errnum\\fP\n");
    wrapper(fp, "",
        "The error value to be decoded, usually obtained from the "
        "\\f[I]errno\\fP global variable just before this function is "
        "called.  This is necessary if you need to call \\f[B]any\\fP "
        "code between the system call to be explained and this "
        "function, because many libc functions will alter the value of "
        "\\f[I]errno\\fP.");
}


static void
blurb_message(FILE *fp)
{
    fprintf(fp, "  * @param message\n");
    wrapper(fp, "  *     ",
        "The location in which to store the returned message.  If a "
        "suitable message return buffer is supplied, this function is "
        "thread safe.");
    fprintf(fp, "  * @param message_size\n");
    wrapper(fp, "  *     ",
        "The size in bytes of the location in which to store the "
        "returned message.");
}


static void
groff_message(FILE *fp)
{
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]message\\fP\n");
    wrapper(fp, "",
        "The location in which to store the returned message.  If a "
        "suitable message return buffer is supplied, this function is "
        "thread safe.");
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]message_size\\fP\n");
    wrapper(fp, "",
        "The size in bytes of the location in which to store the "
        "returned message.");
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
vim_line(FILE *fp, const char *before, const char *after)
{
    fprintf(fp, "%s vim: set ts=8 sw=4 et", before);
    if (after && *after)
        fprintf(fp, " %s", after);
    fprintf(fp, "\n");
}


static const char **slist;
static size_t slist_length;
static size_t slist_maximum;


void
generate_specific(const char *filename)
{
    if (slist_length >= slist_maximum)
    {
        size_t          new_maximum;
        const char      **new_slist;

        new_maximum = slist_maximum * 2 + 16;
        new_slist =
            (const char **)
            explain_malloc_or_die(new_maximum * sizeof(const char *));
        if (slist)
        {
            size_t          j;

            for (j = 0; j < slist_length; ++j)
                new_slist[j] = slist[j];
            free(slist);
        }
        slist = new_slist;
        slist_maximum = new_maximum;
    }
    slist[slist_length++] = filename;
}


static int
selected(const char *filename)
{
    size_t          j;

    if (!slist_length)
        return 1;
    for (j = 0; j < slist_length; ++j)
    {
        if (0 == strcmp(filename, slist[j]))
            return 1;
    }
    return 0;
}


static const char *function_name;
static int      section;
static node_t   *args;
static node_t   *call_args;
static node_t   *result_decl;
static int      ret_int;
static int      ret_ptr;
static int      use_return_value;
static int      or_die_warn_unused;
static int      reset_errno;


static void
libexplain_fubar_h_code_begin(FILE *fp, string_buffer_t *sb)
{
    if (reset_errno)
        fprintf(fp, "  * errno = 0;\n");
    string_buffer_rewind(sb);
    if (ret_int && !use_return_value)
    {
        string_buffer_puts(sb, "if (");
        string_buffer_puts(sb, function_name);
        string_buffer_putc(sb, '(');
        node_print_sb(call_args, sb, node_print_style_normal);
        string_buffer_puts(sb, ") < 0");
        if (reset_errno)
            string_buffer_puts(sb, " && errno != 0");
        string_buffer_putc(sb, ')');
    }
    else
    {
        node_print_sb(result_decl, sb, node_print_style_normal);
        string_buffer_puts(sb, " = ");
        string_buffer_puts(sb, function_name);
        string_buffer_putc(sb, '(');
        node_print_sb(call_args, sb, node_print_style_normal);
        string_buffer_puts(sb, ");");
        wrapper_hang(fp, "  * ", string_buffer_get(sb));
        string_buffer_rewind(sb);

        string_buffer_puts(sb, "if (");
        if (ret_ptr)
            string_buffer_puts(sb, "!result");
        else
            string_buffer_puts(sb, "result < 0");
        if (reset_errno)
            string_buffer_puts(sb, " && errno != 0");
        string_buffer_putc(sb, ')');
    }
    wrapper_hang(fp, "  * ", string_buffer_get(sb));
}


static void
print_forward_struct(node_t *np, FILE *fp)
{
    size_t          j;

    if (node_is_a_literal(np))
        return;
    if (node_is(np, "struct_or_union_specifier"))
    {
        node_print(np, fp);
        fprintf(fp, "; /* forward */\n");
        return;
    }
    for (j = 0; j < np->nchild; ++j)
    {
        print_forward_struct(np->child[j], fp);
    }
}


static void
libexplain_fubar_h(node_t *declaration)
{
    FILE            *fp;
    size_t           j;
    string_buffer_t sb;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "libexplain/%s.h", function_name);
    if (!selected(filename))
        return;
    string_buffer_constructor(&sb);
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    upcase_insitu(filename);
    fprintf(fp, "#ifndef %s\n", filename);
    fprintf(fp, "#define %s\n", filename);

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    fprintf(fp, "  * @file\n");
    fprintf(fp, "  * @brief explain %s(%d) errors\n", function_name, section);
    fprintf(fp, "  *\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "These functions may be used to obtain "
        "explanations for errors returned by the ");
    string_buffer_printf(&sb, "<i>%s</i>(%d)", function_name, section);
    string_buffer_puts(&sb, " system call.");
    wrapper(fp, "  * ", string_buffer_get(&sb));
    fprintf(fp, "  */\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/warn_unused_result.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#ifdef __cplusplus\n");
    fprintf(fp, "extern \"C\" {\n");
    fprintf(fp, "#endif\n");

    /*
     * We need forward declarations of all the structs that will appear
     * in the function prototypes.
     */
    print_forward_struct(args, fp);

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_or_die function is used to call the ");
    string_buffer_printf(&sb, "<i>%s</i>(%d)", function_name, section);
    string_buffer_puts(&sb,
        " system call.  On failure an explanation will be printed to stderr, "
        "obtained from the <i>#explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "</i>(3) function, and then the process terminates "
        "by calling <tt>exit(EXIT_FAILURE)</tt>.");
    wrapper(fp, "  * ", string_buffer_get(&sb));
    fprintf(fp, "  *\n");
    for (j = 0; j < call_args->nchild; j += 2)
    {
        const char *pname = call_args->child[j]->literal;
        fprintf(fp, "  * @param %s\n", pname);
        string_buffer_rewind(&sb);
        string_buffer_puts(&sb, "The ");
        string_buffer_puts(&sb, pname);
        string_buffer_puts(&sb, ", exactly as to be passed to the ");
        string_buffer_printf(&sb, "<i>%s</i>(%d)", function_name, section);
        string_buffer_puts(&sb, " system call.");
        wrapper(fp, "  *     ", string_buffer_get(&sb));
    }
    fprintf(fp, "  * @returns\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "This function only returns on success, see ");
    string_buffer_printf(&sb, "<i>%s</i>(%d)", function_name, section);
    string_buffer_puts(&sb, " for more information.  On failure, prints an "
        "explanation and exits, it does not return.");
    wrapper(fp, "  *     ", string_buffer_get(&sb));
    fprintf(fp, "  *\n");
    fprintf(fp, "  * @par Example:\n");
    wrapper(fp, "  * ",
        "This function is intended to be used in a fashion similar to the "
        "following example:");
    fprintf(fp, "  * @code\n");
    string_buffer_rewind(&sb);
    if (use_return_value)
    {
        node_print_sb(result_decl, &sb, node_print_style_normal);
        string_buffer_puts(&sb, " = ");
    }
    string_buffer_printf(&sb, "explain_%s_or_die(", function_name);
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "  * ", string_buffer_get(&sb));
    fprintf(fp, "  * @endcode\n");
    fprintf(fp, "  */\n");
    string_buffer_rewind(&sb);
    {
        node_t          *np;

        np = node_synth_or_die(declaration, use_return_value);
        node_print_sb(np, &sb, node_print_style_normal);
        node_free(np);
    }
    if (!use_return_value || !or_die_warn_unused)
        string_buffer_putc(&sb, ';');
    wrapper_hang(fp, "", string_buffer_get(&sb));
    if (use_return_value && or_die_warn_unused)
        fprintf(fp, "%80s\n", "LIBEXPLAIN_WARN_UNUSED_RESULT;");

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_on_error function is used to call the ");
    string_buffer_printf(&sb, "<i>%s</i>(%d)", function_name, section);
    string_buffer_puts(&sb, " system call.  On failure an explanation will be "
        "printed to stderr, obtained from the <i>#explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "</i>(3) function.\n");
    wrapper(fp, "  * ", string_buffer_get(&sb));
    fprintf(fp, "  *\n");
    for (j = 0; j < call_args->nchild; j += 2)
    {
        const char *pname = call_args->child[j]->literal;
        fprintf(fp, "  * @param %s\n", pname);
        string_buffer_rewind(&sb);
        string_buffer_puts(&sb, "The ");
        string_buffer_puts(&sb, pname);
        string_buffer_puts(&sb, ", exactly as to be passed to the ");
        string_buffer_printf(&sb, "<i>%s</i>(%d)", function_name, section);
        string_buffer_puts(&sb, " system call.\n");
        wrapper(fp, "  *     ", string_buffer_get(&sb));
    }
    fprintf(fp, "  * @returns\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The value returned by the wrapped ");
    string_buffer_printf(&sb, "<i>%s</i>(%d)", function_name, section);
    string_buffer_puts(&sb, " system call.");
    wrapper(fp, "  *     ", string_buffer_get(&sb));
    fprintf(fp, "  *\n");
    fprintf(fp, "  * @par Example:\n");
    wrapper(fp, "  * ",
        "This function is intended to be used in a fashion similar to the "
        "following example:");
    fprintf(fp, "  * @code\n");
    if (reset_errno)
        fprintf(fp, "  * errno = 0;\n");
    string_buffer_rewind(&sb);
    if (ret_int && !use_return_value)
    {
        string_buffer_puts(&sb, "if (explain_");
        string_buffer_puts(&sb, function_name);
        string_buffer_puts(&sb, "_on_error(");
        node_print_sb(call_args, &sb, node_print_style_normal);
        string_buffer_puts(&sb, ") < 0");
        if (reset_errno)
            string_buffer_puts(&sb, " && errno != 0");
        string_buffer_putc(&sb, ')');
    }
    else
    {
        node_print_sb(result_decl, &sb, node_print_style_normal);
        string_buffer_puts(&sb, " = explain_");
        string_buffer_puts(&sb, function_name);
        string_buffer_puts(&sb, "_on_error(");
        node_print_sb(call_args, &sb, node_print_style_normal);
        string_buffer_puts(&sb, ");");
        wrapper_hang(fp, "  * ", string_buffer_get(&sb));
        string_buffer_rewind(&sb);

        if (ret_ptr)
            string_buffer_puts(&sb, "if (!result");
        else
            string_buffer_puts(&sb, "if (result < 0");
        if (reset_errno)
            string_buffer_puts(&sb, " && errno != 0");
        string_buffer_putc(&sb, ')');
    }
    wrapper_hang(fp, "  * ", string_buffer_get(&sb));
    fprintf(fp, "  * {\n");
    fprintf(fp, "  *     ...cope with error\n");
    fprintf(fp, "  *     ...no need to print error message\n");
    fprintf(fp, "  * }\n");
    fprintf(fp, "  * @endcode\n");
    fprintf(fp, "  */\n");
    string_buffer_rewind(&sb);
    {
        node_t *np = node_synth_on_error(declaration);
        node_print_sb(np, &sb, node_print_style_normal);
        node_free(np);
    }
    if (!or_die_warn_unused)
        string_buffer_putc(&sb, ';');
    wrapper_hang(fp, "", string_buffer_get(&sb));
    if (or_die_warn_unused)
        fprintf(fp, "%80s\n", "LIBEXPLAIN_WARN_UNUSED_RESULT;");

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, " function is used to obtain an explanation of an "
        "error returned by the ");
    string_buffer_printf(&sb, "<i>%s</i>(%d)", function_name, section);
    string_buffer_puts(&sb,
        " system call.  The least the message will contain is the value "
        "of <tt>strerror(errno)</tt>, but usually it will do much "
        "better, and indicate the underlying cause in more detail.");
    wrapper(fp, "  * ", string_buffer_get(&sb));
    blurb_errno(fp);
    fprintf(fp, "  *\n");
    blurb_args(fp, call_args, function_name, section);
    blurb_returns(fp);
    fprintf(fp, "  *\n");
    fprintf(fp, "  * @par Example:\n");
    wrapper(fp, "  * ",
        "This function is intended to be used in a fashion similar to "
        "the following example:");
    fprintf(fp, "  * @code\n");
    libexplain_fubar_h_code_begin(fp, &sb);
    fprintf(fp, "  * {\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "fprintf(stderr, \"%s\\n\", explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_putc(&sb, '(');
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, "));");
    wrapper_hang(fp, "  *     ", string_buffer_get(&sb));
    fprintf(fp, "  *     exit(EXIT_FAILURE);\n");
    fprintf(fp, "  * }\n");
    fprintf(fp, "  * @endcode\n");
    fprintf(fp, "  * @par\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The above code example is available pre-packaged "
        "as the #explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_or_die function.");
    wrapper(fp, "  * ", string_buffer_get(&sb));
    fprintf(fp, "  */\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "const char *explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_putc(&sb, '(');
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_putc(&sb, ')');
    wrapper_hang(fp, "", string_buffer_get(&sb));
    fprintf(fp, "%80s\n", "LIBEXPLAIN_WARN_UNUSED_RESULT;");

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The explain_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, " function is used to obtain an explanation of an "
        "error returned by the ");
    string_buffer_printf(&sb, "<i>%s</i>(%d)", function_name, section);
    string_buffer_puts(&sb,
        " system call.  The least the message will contain is the value "
        "of <tt>strerror(errnum)</tt>, but usually it will do much "
        "better, and indicate the underlying cause in more detail.");
    wrapper(fp, "  * ", string_buffer_get(&sb));
    fprintf(fp, "  *\n");
    blurb_errnum(fp);
    blurb_args(fp, call_args, function_name, section);
    blurb_returns(fp);
    fprintf(fp, "  *\n");
    fprintf(fp, "  * @par Example:\n");
    wrapper(fp, "  * ",
        "This function is intended to be used in a fashion similar to "
        "the following example:");
    fprintf(fp, "  * @code\n");
    libexplain_fubar_h_code_begin(fp, &sb);
    fprintf(fp, "  * {\n");
    fprintf(fp, "  *     int err = errno;\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "fprintf(stderr, \"%s\\n\", explain_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(err, ");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, "));");
    wrapper_hang(fp, "  *     ", string_buffer_get(&sb));
    fprintf(fp, "  *     exit(EXIT_FAILURE);\n");
    fprintf(fp, "  * }\n");
    fprintf(fp, "  * @endcode\n");
    fprintf(fp, "  * @par\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The above code example is available pre-packaged "
        "as the #explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_or_die function.");
    wrapper(fp, "  * ", string_buffer_get(&sb));
    fprintf(fp, "  */\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "const char *explain_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(int errnum, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_putc(&sb, ')');
    wrapper_hang(fp, "", string_buffer_get(&sb));
    fprintf(fp, "%80s\n", "LIBEXPLAIN_WARN_UNUSED_RESULT;");

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The explain_message_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, " function is used to obtain an explanation of an "
        "error returned by the ");
    string_buffer_printf(&sb, "<i>%s</i>(%d)", function_name, section);
    string_buffer_puts(&sb,
        " system call.  The least the message will contain is the value "
        "of <tt>strerror(errnum)</tt>, but usually it will do much "
        "better, and indicate the underlying cause in more detail.");
    wrapper(fp, "  * ", string_buffer_get(&sb));
    blurb_errno(fp);
    fprintf(fp, "  *\n");
    blurb_message(fp);
    blurb_args(fp, call_args, function_name, section);
    fprintf(fp, "  *\n");
    fprintf(fp, "  * @par Example:\n");
    wrapper(fp, "  * ",
        "This function is intended to be used in a fashion similar to the "
        "following example:");
    fprintf(fp, "  * @code\n");
    libexplain_fubar_h_code_begin(fp, &sb);
    fprintf(fp, "  * {\n");
    fprintf(fp, "  *     char message[3000];\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_message_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(message, sizeof(message), ");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "  *     ", string_buffer_get(&sb));
    fprintf(fp, "  *     fprintf(stderr, \"%%s\\n\", message);\n");
    fprintf(fp, "  *     exit(EXIT_FAILURE);\n");
    fprintf(fp, "  * }\n");
    fprintf(fp, "  * @endcode\n");
    fprintf(fp, "  * @par\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The above code example is available pre-packaged "
        "as the #explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_or_die function.");
    wrapper(fp, "  * ", string_buffer_get(&sb));
    fprintf(fp, "  */\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "void explain_message_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(char *message, int message_size, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "", string_buffer_get(&sb));

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The explain_message_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, " function is used to obtain an explanation of an "
        "error returned by the ");
    string_buffer_printf(&sb, "<i>%s</i>(%d)", function_name, section);
    string_buffer_puts(&sb,
        " system call.  The least the message will contain is the value "
        "of <tt>strerror(errnum)</tt>, but usually it will do much "
        "better, and indicate the underlying cause in more detail.");
    wrapper(fp, "  * ", string_buffer_get(&sb));
    fprintf(fp, "  *\n");
    blurb_message(fp);
    blurb_errnum(fp);
    blurb_args(fp, call_args, function_name, section);
    fprintf(fp, "  *\n");
    fprintf(fp, "  * @par Example:\n");
    wrapper(fp, "  * ",
        "This function is intended to be used in a fashion similar to the "
        "following example:");
    fprintf(fp, "  * @code\n");
    libexplain_fubar_h_code_begin(fp, &sb);
    fprintf(fp, "  * {\n");
    fprintf(fp, "  *     int err = errno;\n");
    fprintf(fp, "  *     char message[3000];\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_message_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(message, sizeof(message), err, ");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "  *     ", string_buffer_get(&sb));
    fprintf(fp, "  *     fprintf(stderr, \"%%s\\n\", message);\n");
    fprintf(fp, "  *     exit(EXIT_FAILURE);\n");
    fprintf(fp, "  * }\n");
    fprintf(fp, "  * @endcode\n");
    fprintf(fp, "  * @par\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The above code example is available pre-packaged "
        "as the #explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_or_die function.");
    wrapper(fp, "  * ", string_buffer_get(&sb));
    fprintf(fp, "  */\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "void explain_message_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(char *message, int message_size, int errnum, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "", string_buffer_get(&sb));

    fprintf(fp, "\n");
    fprintf(fp, "#ifdef __cplusplus\n");
    fprintf(fp, "}\n");
    fprintf(fp, "#endif\n");

    vim_line(fp, "\n/*", "*/");
    fprintf(fp, "#endif /* %s */\n", filename);
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static void
man_man3_explain_fubar_3_code_begin(FILE *fp, string_buffer_t *sb)
{
    if (reset_errno)
        fprintf(fp, "errno = 0;\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ad l\n");
    string_buffer_rewind(sb);
    if (ret_int && !use_return_value)
    {
        string_buffer_puts(sb, "if (");
        string_buffer_puts(sb, function_name);
        string_buffer_putc(sb, '(');
        node_print_sb(call_args, sb, node_print_style_normal);
        string_buffer_puts(sb, ") < 0");
        if (reset_errno)
            string_buffer_puts(sb, " && errno != 0");
        string_buffer_putc(sb, ')');
    }
    else
    {
        node_print_sb(result_decl, sb, node_print_style_normal);
        string_buffer_puts(sb, " = ");
        string_buffer_puts(sb, function_name);
        string_buffer_putc(sb, '(');
        node_print_sb(call_args, sb, node_print_style_normal);
        string_buffer_puts(sb, ");");
        wrapper(fp, "", string_buffer_get(sb));
        fprintf(fp, ".br\n");
        string_buffer_rewind(sb);

        string_buffer_puts(sb, "if (");
        if (ret_ptr)
            string_buffer_puts(sb, "!result");
        else
            string_buffer_puts(sb, "result < 0");
        if (reset_errno)
            string_buffer_puts(sb, " && errno != 0");
        string_buffer_putc(sb, ')');
    }
    wrapper(fp, "", string_buffer_get(sb));
    fprintf(fp, ".ad b\n");
    fprintf(fp, ".nf\n");
}


static void
man_man3_explain_fubar_3(void)
{
    FILE            *fp;
    string_buffer_t sb;
    char            filename[1000];

    string_buffer_constructor(&sb);
    snprintf(filename, sizeof(filename), "man/man3/explain_%s.3",
        function_name);
    if (!selected(filename))
        return;
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
    groff_license_header(fp);
    fprintf(fp, ".ds n) explain_%s\n", function_name);
    fprintf(fp, ".TH explain_%s 3\n", function_name);
    fprintf(fp, ".SH NAME\n");
    fprintf(fp, "explain_%s \\- explain %s(%d) errors\n", function_name,
        function_name, section);
    fprintf(fp, ".XX \"explain_%s(3)\" \"explain %s(%d) errors\"\n",
        function_name, function_name, section);
    fprintf(fp, ".SH SYNOPSIS\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, ".sp 0.3\n");
    fprintf(fp, ".ad l\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "const char *explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_putc(&sb, '(');
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".br\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "const char *explain_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(int errnum, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_printf(&sb, ");");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".br\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "void explain_message_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(char *message, int message_size, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".br\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "void explain_message_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(char *message, int message_size, int errnum, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".ad b\n");
    fprintf(fp, ".SH DESCRIPTION\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "These functions may be used to obtain "
        "explanations for errors returned by the ");
    string_buffer_printf(&sb, "\\f[I]%s\\fP(%d)", function_name, section);
    string_buffer_puts(&sb, " system call.");
    wrapper(fp, "", string_buffer_get(&sb));

    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SS explain_%s\n", function_name);
    fprintf(fp, ".ad l\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "const char *explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_putc(&sb, '(');
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".ad b\n");
    fprintf(fp, ".PP\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The \\f[B]explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "\\fP function is\n");
    string_buffer_puts(&sb, "used to obtain an explanation of an error "
        "returned by the ");
    string_buffer_printf(&sb, "\\f[I]%s\\fP(%d)", function_name, section);
    string_buffer_puts(&sb, " system call.  The least the message will "
        "contain is the value of \\f[CW]strerror(errno)\\fP, but usually it "
        "will do much better, and indicate the underlying cause in more "
        "detail.");
    wrapper(fp, "", string_buffer_get(&sb));
    groff_errno(fp);
    groff_args(fp, call_args, function_name, section);
    groff_returns(fp);
    fprintf(fp, ".PP\n");
    wrapper(fp, "", "\\f[B]Example:\\fP This function is intended to be used "
        "in a fashion similar to the following example:");
    fprintf(fp, ".RS\n");
    fprintf(fp, ".ft CW\n");
    man_man3_explain_fubar_3_code_begin(fp, &sb);
    fprintf(fp, "{\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ad l\n");
    fprintf(fp, ".in +4n\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "fprintf(stderr, \"%s\\en\", explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_putc(&sb, '(');
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, "));");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".in -4n\n");
    fprintf(fp, ".nf\n");
    fprintf(fp, "    exit(EXIT_FAILURE);\n");
    fprintf(fp, "}\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ft R\n");
    fprintf(fp, ".ad b\n");
    fprintf(fp, ".RE\n");
    fprintf(fp, ".PP\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The above code example is available pre-packaged "
        "as the \\f[I]explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_or_die\\fP(3) function.");
    wrapper(fp, "", string_buffer_get(&sb));

    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SS explain_errno_%s\n", function_name);
    fprintf(fp, ".ad l\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "const char *explain_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(int errnum, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".ad b\n");
    fprintf(fp, ".PP\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The \\f[B]explain_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "\\fP function is used to obtain an explanation of "
        "an error returned by the ");
    string_buffer_printf(&sb, "\\f[I]%s\\fP(%d)", function_name, section);
    string_buffer_puts(&sb, " system call.  The least the message will contain "
        "is the value of \\f[CW]strerror(errno)\\fP, but usually it will do "
        "much better, and indicate the underlying cause in more detail.");
    wrapper(fp, "", string_buffer_get(&sb));
    groff_errnum(fp);
    groff_args(fp, call_args, function_name, section);
    groff_returns(fp);
    fprintf(fp, ".PP\n");
    wrapper(fp, "", "\\f[B]Example:\\fP This function is intended to be used "
        "in a fashion similar to the following example:");
    fprintf(fp, ".RS\n");
    fprintf(fp, ".ft CW\n");
    man_man3_explain_fubar_3_code_begin(fp, &sb);
    fprintf(fp, "{\n");
    fprintf(fp, "    int err = errno;\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ad l\n");
    fprintf(fp, ".in +4n\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "fprintf(stderr, \"%s\\en\", explain_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(err, ");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, "));");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".in -4n\n");
    fprintf(fp, ".nf\n");
    fprintf(fp, "    exit(EXIT_FAILURE);\n");
    fprintf(fp, "}\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ft R\n");
    fprintf(fp, ".ad b\n");
    fprintf(fp, ".RE\n");
    fprintf(fp, ".PP\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The above code example is available pre-packaged "
        "as the \\f[I]explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_or_die\\fP(3) function.");
    wrapper(fp, "", string_buffer_get(&sb));

    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SS explain_message_%s\n", function_name);
    fprintf(fp, ".ad l\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "void explain_message_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(char *message, int message_size, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".ad b\n");
    fprintf(fp, ".PP\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The \\f[B]explain_message_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "\\fP function is\n");
    string_buffer_puts(&sb, "used to obtain an explanation of an error "
        "returned by the ");
    string_buffer_printf(&sb, "\\f[I]%s\\fP(%d)", function_name, section);
    string_buffer_puts(&sb,
        " system call.  The least the message will contain is the value "
        "of \\f[CW]strerror(errno)\\fP, but usually it will do much "
        "better, and indicate the underlying cause in more detail.");
    wrapper(fp, "", string_buffer_get(&sb));
    groff_errno(fp);
    groff_message(fp);
    groff_args(fp, call_args, function_name, section);
    fprintf(fp, ".PP\n");
    wrapper(fp, "", "\\f[B]Example:\\fP This function is intended to be used "
        "in a fashion similar to the following example:");
    fprintf(fp, ".RS\n");
    fprintf(fp, ".ft CW\n");
    man_man3_explain_fubar_3_code_begin(fp, &sb);
    fprintf(fp, "{\n");
    fprintf(fp, "    char message[3000];\n");
    fprintf(fp, ".in +4n\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ad l\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_message_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(message, sizeof(message), ");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".nf\n");
    fprintf(fp, ".in -4n\n");
    fprintf(fp, "    fprintf(stderr, \"%%s\\en\", message);\n");
    fprintf(fp, "    exit(EXIT_FAILURE);\n");
    fprintf(fp, "}\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ft R\n");
    fprintf(fp, ".ad b\n");
    fprintf(fp, ".RE\n");
    fprintf(fp, ".PP\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The above code example is available pre-packaged "
        "as the \\f[I]explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_or_die\\fP(3) function.");
    wrapper(fp, "", string_buffer_get(&sb));

    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SS explain_message_errno_%s\n", function_name);
    fprintf(fp, ".ad l\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "void explain_message_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(char *message, int message_size, int errnum, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");\n");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".ad b\n");
    fprintf(fp, ".PP\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The \\f[B]explain_message_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "\\fP function is used to obtain an explanation of "
        "an error returned by the ");
    string_buffer_printf(&sb, "\\f[I]%s\\fP(%d)", function_name, section);
    string_buffer_puts(&sb,
        " system call.  The least the message will contain is the value "
        "of \\f[CW]strerror(errno)\\fP, but usually it will do much "
        "better, and indicate the underlying cause in more detail.");
    wrapper(fp, "", string_buffer_get(&sb));
    groff_message(fp);
    groff_errnum(fp);
    groff_args(fp, call_args, function_name, section);
    fprintf(fp, ".PP\n");
    wrapper(fp, "", "\\f[B]Example:\\fP This function is intended to be used "
        "in a fashion similar to the following example:");
    fprintf(fp, ".RS\n");
    fprintf(fp, ".ft CW\n");
    man_man3_explain_fubar_3_code_begin(fp, &sb);
    fprintf(fp, "{\n");
    fprintf(fp, "    int err = errno;\n");
    fprintf(fp, "    char message[3000];\n");
    fprintf(fp, ".in +4n\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ad l\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_message_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(message, sizeof(message), err, ");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");\n");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".nf\n");
    fprintf(fp, ".in -4n\n");
    fprintf(fp, "    fprintf(stderr, \"%%s\\en\", message);\n");
    fprintf(fp, "    exit(EXIT_FAILURE);\n");
    fprintf(fp, "}\n");
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ft R\n");
    fprintf(fp, ".ad b\n");
    fprintf(fp, ".RE\n");
    fprintf(fp, ".PP\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The above code example is available pre-packaged "
        "as the \\f[I]explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_or_die\\fP(3) function.");
    wrapper(fp, "", string_buffer_get(&sb));

    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SH SEE ALSO\n");
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]%s\\fP(%d)\n", function_name, section);
    fprintf(fp, "%s\n", synopsis);
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]explain_%s_or_die\\fP(3)\n", function_name);
    fprintf(fp, "%s and report errors\n", synopsis);
    groff_footer(fp);
    vim_line(fp, ".\\\"", 0);
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static void
man_man3_explain_fubar_or_die_3(node_t *declaration)
{
    size_t          j;
    FILE            *fp;
    string_buffer_t sb;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "man/man3/explain_%s_or_die.3",
        function_name);
    if (!selected(filename))
        return;
    string_buffer_constructor(&sb);
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
    groff_license_header(fp);
    fprintf(fp, ".ds n) explain_%s_or_die\n", function_name);
    fprintf(fp, ".TH explain_%s_or_die 3\n", function_name);
    fprintf(fp, ".SH NAME\n");
    fprintf(fp, "explain_%s_or_die \\- ", function_name);
    fprintf(fp, "%s and report errors\n", synopsis);
    fprintf(fp, ".XX \"explain_%s_or_die(3)\" \\\n", function_name);
    fprintf(fp, "    \"%s and report errors\"\n", synopsis);
    fprintf(fp, ".SH SYNOPSIS\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, ".sp 0.3\n");
    string_buffer_rewind(&sb);
    {
        node_t *np = node_synth_or_die(declaration, use_return_value);
        node_print_sb(np, &sb, node_print_style_normal);
        string_buffer_putc(&sb, ';');
        node_free(np);
    }
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".br\n");
    string_buffer_rewind(&sb);
    {
        node_t *np = node_synth_on_error(declaration);
        node_print_sb(np, &sb, node_print_style_normal);
        string_buffer_putc(&sb, ';');
        node_free(np);
    }
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".SH DESCRIPTION\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The \\f[B]explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_or_die\\fP function is used to call the ");
    string_buffer_printf(&sb, "\\f[I]%s\\fP(%d)", function_name, section);
    string_buffer_puts(&sb,
        " system call.  On failure an explanation will be printed to "
        "\\f[I]stderr\\fP, obtained from the \\f[I]explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb,
        "\\fP(3) function, and then the process terminates by calling "
        "\\f[CW]exit(EXIT_FAILURE)\\fP.");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".PP\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The \\f[B]explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_on_error\\fP function is used to call the ");
    string_buffer_printf(&sb, "\\f[I]%s\\fP(%d)", function_name, section);
    string_buffer_puts(&sb,
        " system call.  On failure an explanation will be printed to "
        "\\f[I]stderr\\fP, obtained from the \\f[I]explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb,
        "\\fP(3) function, but still returns to the caller.");
    wrapper(fp, "", string_buffer_get(&sb));

    for (j = 0; j < call_args->nchild; j += 2)
    {
        const char *pname = call_args->child[j]->literal;
        fprintf(fp, ".TP 8n\n");
        fprintf(fp, "\\f[I]%s\\fP\n", pname);
        string_buffer_rewind(&sb);
        string_buffer_puts(&sb, "The ");
        string_buffer_puts(&sb, pname);
        string_buffer_puts(&sb, ", exactly as to be passed to the ");
        string_buffer_printf(&sb, "\\f[I]%s\\fP(%d)", function_name, section);
        string_buffer_puts(&sb, " system call.");
        wrapper(fp, "", string_buffer_get(&sb));
    }
    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SH RETURN VALUE\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The \\f[B]explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_or_die\\fP function only returns on success, ");
    string_buffer_printf(&sb, "see \\f[I]%s\\fP(%d)", function_name, section);
    string_buffer_puts(&sb, " for more information.  On failure, prints an "
        "explanation and exits, it does not return.");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".PP\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The \\f[B]explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_on_error\\fP function always returns the "
        "value return by the wrapped ");
    string_buffer_printf(&sb, "\\f[I]%s\\fP(%d)", function_name, section);
    string_buffer_puts(&sb, " system call.");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SH EXAMPLE\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The \\f[B]explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_or_die\\fP function is intended to be used in a "
        "fashion similar to the following example:");
    wrapper(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".RS\n");
    fprintf(fp, ".ft CW\n");
    fprintf(fp, ".nf\n");
    string_buffer_rewind(&sb);
    if (use_return_value)
    {
        node_t *np = node_synth_result_variable(declaration);
        node_print_sb(np, &sb, node_print_style_normal);
        node_free(np);
        string_buffer_puts(&sb, " = ");
    }
    string_buffer_puts(&sb, "explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_or_die(");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "", string_buffer_get(&sb));
    fprintf(fp, ".fi\n");
    fprintf(fp, ".ft R\n");
    fprintf(fp, ".RE\n");
    fprintf(fp, ".\\\" ----------------------------------------------------\n");
    fprintf(fp, ".SH SEE ALSO\n");
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]%s\\fP(%d)\n", function_name, section);
    fprintf(fp, "%s\n", synopsis);
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]explain_%s\\fP(3)\n", function_name);
    fprintf(fp, "explain \\f[I]%s\\fP(%d) errors\n", function_name, section);
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]exit\\fP(2)\n");
    fprintf(fp, "terminate the calling process\n");
    groff_footer(fp);
    vim_line(fp, ".\\\"", 0);
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static void
libexplain_fubar_or_die_c(node_t *declaration)
{
    FILE            *fp;
    string_buffer_t sb;
    node_t          *np;
    char            filename[1000];

    ret_ptr = function_returns_pointer(declaration);
    snprintf(filename, sizeof(filename), "libexplain/%s_or_die.c",
        function_name);
    if (!selected(filename))
        return;
    string_buffer_constructor(&sb);
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/ac/stdlib.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "\n");
    fprintf(fp, "\n");

    string_buffer_rewind(&sb);
    np = node_synth_or_die(declaration, use_return_value);
    node_print_sb(np, &sb, node_print_style_function);
    node_free(np);
    wrapper_hang(fp, "", string_buffer_get(&sb));
    fprintf(fp, "{\n");
    if (ret_int && !use_return_value)
    {
        if (reset_errno)
            fprintf(fp, "    errno = 0;\n");
        string_buffer_rewind(&sb);
        string_buffer_puts(&sb, "if (");
        if (ret_ptr)
            string_buffer_putc(&sb, '!');
        string_buffer_puts(&sb, "explain_");
        string_buffer_puts(&sb, function_name);
        string_buffer_puts(&sb, "_on_error(");
        node_print_sb(call_args, &sb, node_print_style_normal);
        string_buffer_putc(&sb, ')');
        if (!ret_ptr)
            string_buffer_puts(&sb, " < 0");
        if (reset_errno)
            string_buffer_puts(&sb, " && errno != 0");
        string_buffer_putc(&sb, ')');
        wrapper_hang(fp, "    ", string_buffer_get(&sb));
    }
    else
    {
        string_buffer_rewind(&sb);
        {
            node_t *result = node_synth_result_variable(declaration);
            node_print_sb(result, &sb, node_print_style_column16);
            node_free(result);
            string_buffer_putc(&sb, ';');
        }
        wrapper_hang(fp, "    ", string_buffer_get(&sb));
        fprintf(fp, "\n");
        if (reset_errno)
            fprintf(fp, "    errno = 0;\n");

        string_buffer_rewind(&sb);
        string_buffer_puts(&sb, "result = explain_");
        string_buffer_puts(&sb, function_name);
        string_buffer_puts(&sb, "_on_error(");
        node_print_sb(call_args, &sb, node_print_style_normal);
        string_buffer_puts(&sb, ");");
        wrapper_hang(fp, "    ", string_buffer_get(&sb));

        fprintf(fp, "    if (");
        if (ret_ptr)
            fprintf(fp, "!result");
        else
            fprintf(fp, "result < 0");
        if (reset_errno)
            string_buffer_puts(&sb, " && errno != 0");
        fprintf(fp, ")\n");
    }
    fprintf(fp, "    {\n");
    fprintf(fp, "        exit(EXIT_FAILURE);\n");
    fprintf(fp, "    }\n");
    if (use_return_value)
        fprintf(fp, "    return result;\n");
    fprintf(fp, "}\n");
    vim_line(fp, "\n\n/*", "*/");
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static void
libexplain_fubar_on_error_c(node_t *declaration)
{
    FILE            *fp;
    string_buffer_t sb;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "libexplain/%s_on_error.c",
        function_name);
    if (!selected(filename))
        return;
    string_buffer_constructor(&sb);
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/ac/stdio.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "#include <libexplain/option.h>\n");
    fprintf(fp, "#include <libexplain/wrap_and_print.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    string_buffer_rewind(&sb);
    {
        node_t *np = node_synth_on_error(declaration);
        node_print_sb(np, &sb, node_print_style_function);
        node_free(np);
    }
    wrapper_hang(fp, "", string_buffer_get(&sb));
    fprintf(fp, "{\n");
    string_buffer_rewind(&sb);
    {
        node_t *np = node_synth_result_variable(declaration);
        node_print_sb(np, &sb, node_print_style_column16);
        node_free(np);
        string_buffer_putc(&sb, ';');
    }
    wrapper_hang(fp, "    ", string_buffer_get(&sb));
    fprintf(fp, "\n");
    if (reset_errno)
        fprintf(stderr, "    errno = 0;\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "result = ");
    string_buffer_puts(&sb, function_name);
    string_buffer_putc(&sb, '(');
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "    ", string_buffer_get(&sb));
    fprintf(fp, "    if (");
    if (ret_ptr)
        fprintf(fp, "!result");
    else
        fprintf(fp, "result < 0");
    if (reset_errno)
        fprintf(fp, " && errno != 0");
    fprintf(fp, ")\n");
    fprintf(fp, "    {\n");
    fprintf(fp, "        explain_program_name_assemble_internal(1);\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_wrap_and_print(stderr, explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_putc(&sb, '(');
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, "));");
    wrapper_hang(fp, "        ", string_buffer_get(&sb));
    fprintf(fp, "    }\n");
    fprintf(fp, "    return result;\n");
    fprintf(fp, "}\n");
    vim_line(fp, "\n\n/*", "*/");
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static void
libexplain_fubar_c(void)
{
    FILE            *fp;
    string_buffer_t sb;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "libexplain/%s.c", function_name);
    if (!selected(filename))
        return;
    string_buffer_constructor(&sb);
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/ac/errno.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "const char *\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_");
    string_buffer_puts(&sb, function_name);
    string_buffer_putc(&sb, '(');
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_putc(&sb, ')');
    wrapper_hang(fp, "", string_buffer_get(&sb));
    fprintf(fp, "{\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "return explain_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(errno, ");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "    ", string_buffer_get(&sb));
    fprintf(fp, "}\n");
    vim_line(fp, "\n\n/*", "*/");
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static void
libexplain_errno_fubar_c(void)
{
    FILE            *fp;
    string_buffer_t sb;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "libexplain/errno/%s.c",
        function_name);
    if (!selected(filename))
        return;
    string_buffer_constructor(&sb);
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/common_message_buffer.h>\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "const char *\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(int errnum, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_putc(&sb, ')');
    wrapper_hang(fp, "", string_buffer_get(&sb));
    fprintf(fp, "{\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_message_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(explain_common_message_buffer, "
        "explain_common_message_buffer_size, errnum, ");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "    ", string_buffer_get(&sb));
    fprintf(fp, "    return explain_common_message_buffer;\n");
    fprintf(fp, "}\n");
    vim_line(fp, "\n\n/*", "*/");
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static void
libexplain_message_fubar_c(void)
{
    FILE            *fp;
    string_buffer_t sb;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "libexplain/message/%s.c",
        function_name);
    if (!selected(filename))
        return;
    string_buffer_constructor(&sb);
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/ac/errno.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "void\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_message_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(char *message, int message_size, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_putc(&sb, ')');
    wrapper_hang(fp, "", string_buffer_get(&sb));
    fprintf(fp, "{\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_message_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(message, message_size, errno, ");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "    ", string_buffer_get(&sb));
    fprintf(fp, "}\n");
    vim_line(fp, "\n\n/*", "*/");
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static void
libexplain_message_errno_fubar_c(void)
{
    FILE            *fp;
    string_buffer_t sb;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "libexplain/message/errno/%s.c",
        function_name);
    if (!selected(filename))
        return;
    string_buffer_constructor(&sb);
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/buffer/errno/%s.h>\n", function_name);
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "void\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_message_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(char *message, int message_size, int errnum, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_putc(&sb, ')');
    wrapper_hang(fp, "", string_buffer_get(&sb));
    fprintf(fp, "{\n");
    fprintf(fp, "    explain_string_buffer_t sb;\n");
    fprintf(fp, "\n");
    fprintf(fp, "    explain_string_buffer_init(&sb, message, "
        "message_size);\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_buffer_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(&sb, errnum, ");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "    ", string_buffer_get(&sb));
    fprintf(fp, "}\n");
    vim_line(fp, "\n\n/*", "*/");
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static void
libexplain_buffer_errno_fubar_h(void)
{
    FILE            *fp;
    string_buffer_t sb;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "libexplain/buffer/errno/%s.h",
        function_name);
    if (!selected(filename))
        return;
    string_buffer_constructor(&sb);
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    upcase_insitu(filename);
    fprintf(fp, "\n");
    fprintf(fp, "#ifndef %s\n", filename);
    fprintf(fp, "#define %s\n", filename);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/string_buffer.h>\n");

    /*
     * We need forward declarations of all the structs that will appear
     * in the function prototypes.
     */
    print_forward_struct(args, fp);

    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The explain_buffer_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, " function is used to obtain an explanation of an "
        "error returned by the ");
    string_buffer_printf(&sb, "<i>%s</i>(%d)", function_name, section);
    string_buffer_puts(&sb,
        " system call.  The least the message will contain is the value "
        "of <tt>strerror(errnum)</tt>, but usually it will do much "
        "better, and indicate the underlying cause in more detail.");
    wrapper(fp, "  * ", string_buffer_get(&sb));
    fprintf(fp, "  *\n");
    fprintf(fp, "  * @param sb\n");
    wrapper(fp, "  *     ", "The string buffer to print the message into.  "
        "If a suitable buffer is specified, this function is thread safe.");
    blurb_errnum(fp);
    blurb_args(fp, call_args, function_name, section);
    fprintf(fp, "  */\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "void explain_buffer_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(explain_string_buffer_t *sb, int errnum, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "", string_buffer_get(&sb));

    vim_line(fp, "\n/*", "*/");
    fprintf(fp, "#endif /* %s */\n", filename);
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static int
name_is_fildes(const char *name)
{
    size_t          len;

    if (0 == strcmp(name, "fildes"))
        return 1;
    len = strlen(name);
    return (len >= 2 && 0 == strcmp(name + len - 2, "fd"));
}


static int
name_is_stream(const char *name)
{
    return (0 == strcmp(name, "fp") || 0 == strcmp(name, "stream"));
}


static void
libexplain_buffer_errno_fubar_c(void)
{
    FILE            *fp;
    size_t          j;
    const char      *opengroup_url;
    string_buffer_t sb;
    char            filename[1000];
    node_t          *error_cases;

    error_cases = look_for_error_cases(function_name, section);

    snprintf(filename, sizeof(filename), "libexplain/buffer/errno/%s.c",
        function_name);
    if (!selected(filename))
        return;
    string_buffer_constructor(&sb);
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
    generate_lgpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/ac/errno.h>\n");
    fprintf(fp, "\n");
    for (j = 0; j < error_cases->child[0]->nchild; ++j)
    {
        const char *lc = error_cases->child[1]->child[j]->literal;
        fprintf(fp, "#include <libexplain/buffer/%s.h>\n", lc);
    }
    fprintf(fp, "#include <libexplain/buffer/errno/generic.h>\n");
    fprintf(fp, "#include <libexplain/buffer/errno/%s.h>\n", function_name);
    fprintf(fp, "#include <libexplain/explanation.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "static void\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_buffer_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_system_call(explain_string_buffer_t *sb, "
        "int errnum, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_putc(&sb, ')');
    wrapper_hang(fp, "", string_buffer_get(&sb));
    fprintf(fp, "{\n");
    fprintf(fp, "    (void)errnum;\n");
    for (j = 0; j < call_args->nchild; j += 2)
    {
        node_t          *arg;
        const char      *arg_name;
        char            fmt[40];

        arg = args->child[0]->child[j];
        arg_name = call_args->child[j]->literal;
        fprintf(fp, "    explain_string_buffer_puts(sb, \"");
        if (!j)
            fprintf(fp, "%s(", function_name);
        else
            fprintf(fp, ", ");
        fprintf(fp, "%s = \");\n", arg_name);
        if (name_is_fildes(arg_name))
        {
            fprintf(fp, "    explain_buffer_fildes(sb, %s);\n", arg_name);
        }
        else if (name_is_stream(arg_name))
        {
            fprintf(fp, "    explain_buffer_stream(sb, %s);\n", arg_name);
        }
        else if (node_parameter_is_formatable(arg, fmt, sizeof(fmt)))
        {
            if (fmt[0] == '%')
            {
                fprintf
                (
                    fp,
                    "    explain_string_buffer_printf(sb, \"%s\", %s);\n",
                    fmt,
                    arg_name
                );
            }
            else
            {
                fprintf(fp, "    %s(sb, %s);\n", fmt, arg_name);
            }
        }
        else
        {
            fprintf
            (
                fp,
                "    explain_string_buffer_printf(sb, \"%%ld\", (long)%s);\n",
                arg_name
            );
        }
    }
    fprintf(fp, "    explain_string_buffer_putc(sb, ')');\n");
    fprintf(fp, "}\n");

    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "static void\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_buffer_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_explanation(explain_string_buffer_t *sb, "
        "int errnum, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_putc(&sb, ')');
    wrapper_hang(fp, "", string_buffer_get(&sb));
    fprintf(fp, "{\n");
    fprintf(fp, "    /*\n");
    opengroup_url = "http://www.opengroup.org/onlinepubs/009695399";
    fprintf(fp, "     * %s/functions/%s.html\n", opengroup_url, function_name);
    fprintf(fp, "     */\n");
    fprintf(fp, "    switch (errnum)\n");
    fprintf(fp, "    {\n");

    for (j = 0; j < error_cases->child[0]->nchild; ++j)
    {
        const char *uc = error_cases->child[0]->child[j]->literal;
        const char *lc = error_cases->child[1]->child[j]->literal;

        fprintf(fp, "    case %s:\n", uc);
        fprintf(fp, "        explain_buffer_%s(sb", lc);
        if (0 == strcmp(lc, "ebadf"))
            fprintf(fp, ", fildes, \"fildes\"");
        fprintf(fp, ");\n");
        fprintf(fp, "        break;\n");
        fprintf(fp, "\n");
    }

    fprintf(fp, "    default:\n");
    fprintf(fp, "        explain_buffer_errno_generic(sb, errnum);\n");
    fprintf(fp, "        break;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "}\n");

    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "void\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_buffer_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(explain_string_buffer_t *sb, int errnum, ");
    node_print_sb(args, &sb, node_print_style_normal);
    string_buffer_putc(&sb, ')');
    wrapper_hang(fp, "", string_buffer_get(&sb));
    fprintf(fp, "{\n");
    fprintf(fp, "    explain_explanation_t exp;\n");
    fprintf(fp, "\n");
    fprintf(fp, "    explain_explanation_init(&exp, errnum);\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_buffer_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_system_call(&exp.system_call_sb, errnum, ");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "    ", string_buffer_get(&sb));
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_buffer_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "_explanation(&exp.explanation_sb, errnum, ");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, ");");
    wrapper_hang(fp, "    ", string_buffer_get(&sb));
    fprintf(fp, "    explain_explanation_assemble(&exp, sb);\n");
    fprintf(fp, "}\n");
    vim_line(fp, "\n\n/*", "*/");
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static void
test_fubar_main_c(void)
{
    FILE            *fp;
    size_t          j;
    string_buffer_t sb;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "test_%s/main.c", function_name);
    if (!selected(filename))
        return;
    string_buffer_constructor(&sb);
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
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
        string_buffer_rewind(&sb);
        node_print_sb(args->child[0]->child[j], &sb, node_print_style_column16);
        string_buffer_putc(&sb, ';');
        wrapper_hang(fp, "    ", string_buffer_get(&sb));
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
    fprintf(fp, "            explain_version_print();\n");
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
        node_t *param_decl = args->child[0]->child[j];
        const char *param_name = call_args->child[j]->literal;
        char argv[30];
        if (j == 0)
            snprintf(argv, sizeof(argv), "argv[optind]");
        else
            snprintf(argv, sizeof(argv), "argv[optind + %d]", (int)(j / 2));
        fprintf(fp, "    %s = ", param_name);
        if (node_parameter_is_const_char_star(param_decl))
        {
            fprintf(fp, "%s;\n", argv);
        }
        else
        {
            char fmt[30];
            if (node_parameter_from_string(param_decl, fmt, sizeof(fmt)))
                fprintf(fp, "%s(%s);\n", fmt, argv);
            else
                fprintf(fp, "explain_strtol_or_die(%s, 0, 0);\n", argv);
        }
    }
    fprintf(fp, "\n");
    fprintf(fp, "    explain_%s_or_die(", function_name);
    node_print(call_args, fp);
    fprintf(fp, ");\n");
    fprintf(fp, "    return EXIT_SUCCESS;\n");
    fprintf(fp, "}\n");
    vim_line(fp, "\n\n/*", "*/");
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static void
explain_syscall_fubar_c(void)
{
    FILE            *fp;
    size_t          j;
    string_buffer_t sb;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "explain/syscall/%s.c", function_name);
    if (!selected(filename))
        return;
    string_buffer_constructor(&sb);
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
    generate_gpl_header(fp);
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/ac/stdio.h>\n");
    fprintf(fp, "#include <libexplain/ac/stdlib.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    fprintf(fp, "#include <libexplain/wrap_and_print.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "#include <explain/syscall/%s.h>\n", function_name);
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "void\n");
    fprintf(fp, "explain_syscall_%s(int errnum, int argc, char **argv)\n",
        function_name);
    fprintf(fp, "{\n");
    for (j = 0; j < args->child[0]->nchild; j += 2)
    {
        string_buffer_rewind(&sb);
        node_print_sb(args->child[0]->child[j], &sb, node_print_style_column16);
        string_buffer_putc(&sb, ';');
        wrapper_hang(fp, "    ", string_buffer_get(&sb));
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
        char argv[30];
        node_t *param_decl = args->child[0]->child[j];
        const char *param_name = call_args->child[j]->literal;
        snprintf(argv, sizeof(argv), "argv[%d]", (int)(j / 2));
        fprintf(fp, "    %s = ", param_name);
        if (node_parameter_is_const_char_star(param_decl))
        {
            fprintf(fp, "%s;\n", argv);
        }
        else
        {
            char fmt[30];
            if (node_parameter_from_string(param_decl, fmt, sizeof(fmt)))
                fprintf(fp, "%s(%s);\n", fmt, argv);
            else
                fprintf(fp, "explain_strtol_or_die(%s, 0, 0);\n", argv);
        }
    }
    fprintf(fp, "\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "explain_wrap_and_print(stdout, explain_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, "(errnum, ");
    node_print_sb(call_args, &sb, node_print_style_normal);
    string_buffer_puts(&sb, "));\n");
    wrapper_hang(fp, "    ", string_buffer_get(&sb));
    fprintf(fp, "}\n");
    vim_line(fp, "\n\n/*", "*/");
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static void
explain_syscall_fubar_h(void)
{
    FILE            *fp;
    string_buffer_t sb;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "explain/syscall/%s.h", function_name);
    if (!selected(filename))
        return;
    string_buffer_constructor(&sb);
    aegis_new_file(filename);
    fp = explain_fopen_or_die(filename, "w");
    generate_gpl_header(fp);
    upcase_insitu(filename);
    fprintf(fp, "\n");
    fprintf(fp, "#ifndef %s\n", filename);
    fprintf(fp, "#define %s\n", filename);
    fprintf(fp, "\n");
    fprintf(fp, "/**\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "The explain_syscall_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, " function is used to interpret a ");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, " \"call\" from the command line, and then run it "
        "through the explain_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, " function for an explanation.");
    wrapper(fp, "  * ", string_buffer_get(&sb));
    fprintf(fp, "  *\n");
    fprintf(fp, "  * @note\n");
    string_buffer_rewind(&sb);
    string_buffer_puts(&sb, "Because the event being explain happened in "
        "another process, the results are not as good as if explain_errno_");
    string_buffer_puts(&sb, function_name);
    string_buffer_puts(&sb, " had been called from within the process that "
        "saw the error.");
    wrapper(fp, "  *     ", string_buffer_get(&sb));
    fprintf(fp, "  *\n");
    fprintf(fp, "  * @param errnum\n");
    fprintf(fp, "  *     The number of the error to be explained.\n");
    fprintf(fp, "  * @param argc\n");
    wrapper(fp, "  *     ",
        "the number of command line arguments, corresponding 1:1 to the call "
        "arguments.");
    fprintf(fp, "  * @param argv\n");
    wrapper(fp, "  *     ",
        "the actual command line arguments, corresponding 1:1 to the call "
        "arguments.");
    fprintf(fp, "  */\n");
    fprintf(fp, "void explain_syscall_%s(int errnum, int argc, char **argv);\n",
        function_name);
    vim_line(fp, "\n/*", "*/");
    fprintf(fp, "#endif /* %s */\n", filename);
    explain_fclose_or_die(fp);
    string_buffer_destructor(&sb);
}


static void
explain_syscall_c(void)
{
    FILE            *fp;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "explain/syscall.c");
    if (!selected(filename))
        return;
    aegis_copy_file(filename);
    fp = explain_fopen_or_die(filename, "a");
    fprintf(fp, "#include <explain/syscall/%s.h>\n", function_name);
    fprintf(fp, "    { \"%s\", ", function_name);
    fprintf(fp, "explain_syscall_%s },\n", function_name);
    explain_fclose_or_die(fp);
}


static void
man_man3_explain_3(void)
{
    FILE            *fp;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "man/man3/explain.3");
    if (!selected(filename))
        return;
    aegis_copy_file(filename);
    fp = explain_fopen_or_die(filename, "a");
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]explain_%s\\fP(3)\n", function_name);
    fprintf(fp, "Explain \\f[I]%s\\fP(%d) errors\n", function_name, section);
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "\\f[I]explain_%s_or_die\\fP(3)\n", function_name);
    fprintf(fp, "%s and report errors\n", synopsis);
    explain_fclose_or_die(fp);
}


static void
libexplain_libexplain_h(void)
{
    FILE            *fp;
    char            filename[100];

    snprintf(filename, sizeof(filename), "libexplain/libexplain.h");
    if (!selected(filename))
        return;
    aegis_copy_file(filename);
    fp = explain_fopen_or_die(filename, "a");
    fprintf(fp, "#include <libexplain/%s.h>\n", function_name);
    explain_fclose_or_die(fp);
}


static void
man_man1_explain_1(void)
{
    FILE            *fp;
    size_t          j;
    char            filename[1000];

    snprintf(filename, sizeof(filename), "man/man1/explain.1");
    if (!selected(filename))
        return;
    aegis_copy_file(filename);
    fp = explain_fopen_or_die(filename, "a");
    fprintf(fp, ".TP 8n\n");
    fprintf(fp, "%s", function_name);
    for (j = 0; j < call_args->nchild; j += 2)
    {
        fprintf(fp, " \\f[I]%s\\fP", call_args->child[j]->literal);
    }
    fprintf(fp, "\n");
    fprintf(fp, "The \\f[I]%s\\fP(%d) system call.\n", function_name, section);
    explain_fclose_or_die(fp);
}


void
generate(node_t *declaration, catalogue_t *cat)
{
    node_t          *declspec;
    node_t          *decl;

    assert(node_is(declaration, "declaration"));
    assert(node_is(declaration->child[1]->child[0], "init_declarator"));
    assert(declaration->child[1]->child[0]->nchild == 1);
    assert(node_is(declaration->child[1]->child[0]->child[0], "declarator"));
    assert(declaration->child[1]->child[0]->child[0]->nchild == 1
        || declaration->child[1]->child[0]->child[0]->nchild == 2);

    declspec = declaration->child[0];
    assert(node_is(declspec, "declaration_specifiers"));
    decl = declaration->child[1]->child[0]->child[0];
    assert(node_is(decl, "declarator"));

    function_name = find_function_name(declaration);
    section = try_to_guess_section(function_name);

    synopsis = catalogue_get(cat, "Synopsis");
    if (!synopsis)
    {
        try_to_get_synopsis(function_name, section);
        catalogue_set(cat, "Synopsis", synopsis);
    }

    args = find_argument_list(decl);

    call_args = synth_call_args(args);
    result_decl = node_synth_result_variable(declaration);
    ret_ptr = function_returns_pointer(declaration);
    ret_int = function_returns_int(declaration);

    {
        const char *uret = catalogue_get(cat, "Use-Return-Value");
        if (uret)
        {
            use_return_value = string_to_boolean(uret);
        }
        else
        {
            use_return_value = ret_ptr || !ret_int;
            catalogue_set
            (
                cat,
                "Use-Return-Value",
                boolean_to_string(use_return_value)
            );
        }
    }

    or_die_warn_unused =
        catalogue_get_bool(cat, "Or-Die-Warn-Unused", use_return_value);
    reset_errno = catalogue_get_bool(cat, "Reset-Errno", 0);

    /* create... */
    explain_syscall_fubar_c();
    explain_syscall_fubar_h();
    libexplain_buffer_errno_fubar_c();
    libexplain_buffer_errno_fubar_h();
    libexplain_errno_fubar_c();
    libexplain_fubar_c();
    libexplain_fubar_h(declaration);
    libexplain_fubar_on_error_c(declaration);
    libexplain_fubar_or_die_c(declaration);
    libexplain_message_errno_fubar_c();
    libexplain_message_fubar_c();
    man_man3_explain_fubar_3();
    man_man3_explain_fubar_or_die_3(declaration);
    test_fubar_main_c();

    /* modify... */
    explain_syscall_c();
    libexplain_libexplain_h();
    man_man1_explain_1();
    man_man3_explain_3();
}
