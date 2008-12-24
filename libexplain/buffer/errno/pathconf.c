/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>

#include <libexplain/buffer/eacces.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/errno/pathconf.h>
#include <libexplain/buffer/pathconf_name.h>
#include <libexplain/buffer/pathname.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_pathconf_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, int name)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "pathconf(pathname = ");
    libexplain_buffer_pathname(sb, pathname);
    libexplain_string_buffer_puts(sb, ", name = ");
    libexplain_buffer_pathconf_name(sb, name);
    libexplain_string_buffer_putc(sb, ')');
}


void
libexplain_buffer_pathconf_einval(libexplain_string_buffer_t *sb,
    const char *arg1_caption, int name, const char *name_caption)
{
    if (!libexplain_valid_pathconf_name(name))
    {
        libexplain_string_buffer_printf_gettext
        (
            sb,
            /*
             * xgettext: This message is used to explain an EINVAL error
             * reported by the pathconf system call.
             *
             * %1$s => the name of the offending system call argument.
             */
            i18n("%s does not refer to a known file configuration value"),
            name_caption
        );
    }
    else
    {
        libexplain_string_buffer_printf_gettext
        (
            sb,
            /*
             * xgettext: This message is used to explain an EINVAL error
             * reported by the pathconf system call.
             *
             * %1$s => the name of the system call argument containing
             *         the 'name' selector, e.g. _PC_NAME_MAX
             * %2$s => the name of the first argument, "pathname" or "fildes"
             */
            i18n("the implementation does not support an association of "
                "%s with %s"),
            name_caption,
            arg1_caption
        );
    }
}


static void
libexplain_buffer_errno_pathconf_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, int name)
{
    libexplain_final_t final_component;

    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/pathconf.html
     */
    (void)name;
    libexplain_final_init(&final_component);
    switch (errnum)
    {
    case ELOOP:
        libexplain_buffer_eloop(sb, pathname, "pathname", &final_component);
        break;

    case EACCES:
        libexplain_buffer_eacces(sb, pathname, "pathname", &final_component);
        break;

    case EINVAL:
        libexplain_buffer_pathconf_einval(sb, "pathname", name, "name");
        break;

    case ENAMETOOLONG:
        libexplain_buffer_enametoolong
        (
            sb,
            pathname,
            "pathname",
            &final_component
        );
        break;

    case ENOENT:
        libexplain_buffer_enoent(sb, pathname, "pathname", &final_component);
        break;

    case ENOTDIR:
        libexplain_buffer_enotdir(sb, pathname, "pathname", &final_component);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_pathconf(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, int name)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_pathconf_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        name
    );
    libexplain_buffer_errno_pathconf_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname,
        name
    );
    libexplain_explanation_assemble(&exp, sb);
}
