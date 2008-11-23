/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#include <libexplain/ac/string.h>

#include <libexplain/buffer/strerror.h>
#include <libexplain/explanation.h>
#include <libexplain/gettext.h>


void
libexplain_explanation_init(libexplain_explanation_t *exp, int errnum)
{
    libexplain_string_buffer_init
    (
        &exp->system_call_sb,
        exp->system_call,
        sizeof(exp->system_call)
    );
    exp->errnum = errnum;
    libexplain_string_buffer_init
    (
        &exp->explanation_sb,
        exp->explanation,
        sizeof(exp->explanation)
    );
}


void
libexplain_explanation_assemble(libexplain_explanation_t *exp,
    libexplain_string_buffer_t *result)
{
    long            fmt_len;
    long            prob_len;
    long            exp_len;
    int             err_len;
    const char      *fmt;
    char            errstr[100];
    libexplain_string_buffer_t errstr_sb;

    if (exp->errnum == 0)
    {
        libexplain_string_buffer_printf_gettext
        (
            result,
            /*
             * xgettext: this message is issued when a system call
             * succeeds, when there was, in fact, no error.
             */
            i18n("%s: success"),
            exp->system_call
        );
        return;
    }

    libexplain_string_buffer_init(&errstr_sb, errstr, sizeof(errstr));
    libexplain_buffer_strerror(&errstr_sb, exp->errnum);

    /*
     * If there is no extended explanation available,
     * use a shortened form of the message.
     */
    prob_len = exp->system_call_sb.position;
    err_len = errstr_sb.position;
    if (exp->explanation_sb.position == 0)
    {
        use_short_form:
        fmt = libexplain_gettext("%s failed, %s");
        fmt_len = strlen(fmt);
        if (prob_len + err_len + fmt_len > (long)result->maximum)
        {
            long new_len = (long)result->maximum - (fmt_len + err_len);
            libexplain_string_buffer_truncate(&exp->explanation_sb, new_len);
        }

        libexplain_string_buffer_printf_gettext
        (
            result,
            /*
             * xgettext: this message is printed when there is no
             * extended explanation available.  In english, the stuff
             * to the left of "because" is a statement of the problem,
             * including function name and function argument names and
             * values.
             *
             * Usually a longer message, including a prose explanation, is
             * used.  This shorter message is used when there is no extended
             * explanation, or when the user-supplied message buffer is too
             * small.
             *
             * Depending on the grammar of the language being translated
             * to, you may need to rearrange these two pieces using
             * positional arguments.
             *
             * %1$s => the system call and its arguments
             *         e.g. "open(pathname = "foo/bar", flags = O_RDONLY)"
             * %2$s => the strerror text, plus the name and number of
             *         the errno.h constant
             *         e.g. "No such file or directory (2, ENOENT)"
             *
             * For example:
             *
             * msgid "%s failed, %s"
             * msgstr "%2$s was returned by %1$s"
             */
            i18n("%s failed, %s"),
            exp->system_call,
            errstr
        );
        return;
    }

    fmt = libexplain_gettext("%s failed, %s because %s");
    fmt_len = strlen(fmt);
    exp_len = exp->explanation_sb.position;
    if (prob_len + err_len + fmt_len + exp_len > (long)result->maximum)
    {
        long new_exp_len =
            (long)result->maximum - (prob_len + fmt_len + err_len);
        if (new_exp_len <= 0)
        {
            goto use_short_form;
        }
        libexplain_string_buffer_truncate(&exp->explanation_sb, new_exp_len);
    }

    libexplain_string_buffer_printf_gettext
    (
        result,
        /*
         * xgettext: This message is used to join the problem to the
         * explanation.  In english, the stuff to the left of "because"
         * is a statement of the problem, including function name and
         * function argument names and values; and the stuff to the
         * right of "because" is the explanation text.
         *
         * Depending on the grammar of the language being translated to,
         * you may need to rearrange these two pieces using positional
         * arguments.
         *     %1$s => the system call and its arguments
         *             e.g. "open(pathname = 'foo/bar', flags = O_RDONLY)"
         *     %2$s => the strerror text, plus the name and number of
         *             the errno.h constant
         *             e.g. "No such file or directory (2, ENOENT)"
         *     %3$s => the explanation text
         *             e.g. "there is no 'bar' file in the pathname
         *                  'foo'; directory"
         *
         * For example:
         *
         * msgid "%s failed, %s because %s"
         * msgstr "%3$s caused %2$s to be returned by %1$s"
         *
         * msgid "%s failed, %s because %s"
         * msgstr "a %2$s error, due to %3$s, was reported by %1$s"
         *
         * This has a follow-on effect for how the explanations themselves
         * are translated, to ensure that sensable sentences result.  In
         * particular, the explanation portion should only ever be one
         * sentence, so that a clause (e.g. above) may be appended.
         */
        i18n("%s failed, %s because %s"),
        exp->system_call,
        errstr,
        exp->explanation
    );

    /*
     * NB: in a perfect world, we would used the above "fmt" variable,
     * thus avoiding the second call to gettext.  But this means that
     * gcc can't check the type of the arguments, and issues a warning;
     * the author's default build environment is -Werror which then
     * bombs out.  On balance, I like the warnings (they are usually
     * right) more than this annoys me.  The redundant call to gettext
     * is to very expensive, as the results are cached.
     */
}
