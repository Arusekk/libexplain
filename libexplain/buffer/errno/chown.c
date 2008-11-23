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
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/chown.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/gid.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/uid.h>
#include <libexplain/capability.h>
#include <libexplain/explanation.h>
#include <libexplain/group_in_groups.h>
#include <libexplain/option.h>


static void
libexplain_buffer_errno_chown_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, int owner, int group)
{
    libexplain_string_buffer_puts(sb, "chown(pathname = ");
    if (errnum == EFAULT)
        libexplain_buffer_pointer(sb, pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_puts(sb, ", owner = ");
    libexplain_buffer_uid(sb, owner);
    libexplain_string_buffer_puts(sb, ", group = ");
    libexplain_buffer_gid(sb, group);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_chown_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, int owner, int group)
{
    libexplain_final_t final_component;

    libexplain_final_init(&final_component);
    final_component.want_to_modify_inode = 1;

    libexplain_buffer_errno_chown_explanation_fc
    (
        sb,
        errnum,
        pathname,
        owner,
        group,
        &final_component
    );
}


void
libexplain_buffer_errno_chown_explanation_fc(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, int owner, int group,
    libexplain_final_t *final_component)
{
    final_component->want_to_modify_inode = 1;

    switch (errnum)
    {
    case EACCES:
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                "pathname",
                final_component
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "search permission is denied on a component of pathname"
            );
        }
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "pathname");
        break;

    case ELOOP:
        libexplain_buffer_eloop(sb, pathname, "pathname", final_component);
        break;

    case ENAMETOOLONG:
        libexplain_buffer_enametoolong
        (
            sb,
            pathname,
            "pathname",
            final_component
        );
        break;

    case ENOENT:
        libexplain_buffer_enoent(sb, pathname, "pathname", final_component);
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    case ENOTDIR:
        libexplain_buffer_enotdir(sb, pathname, "pathname", final_component);
        break;

    case EPERM:
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                "pathname",
                final_component
            )
        )
        {
            /*
             * we get to here if the process DOES have inode modify permission,
             * or permissions coult not be determined
             */
            const char      *which;
            struct stat     st;

            if (stat(pathname, &st) >= 0)
            {
                uid_t           uid;

                uid = geteuid();
                if (owner >= 0 && (uid_t)owner != st.st_uid)
                {
                    long            chown_restricted;

                    chown_restricted = pathconf(pathname, _PC_CHOWN_RESTRICTED);
                    if (chown_restricted >= 0)
                    {
                        int             may_change_file_owner;

                        may_change_file_owner =
                            (
                                (!chown_restricted && uid == st.st_uid)
                            ||
                                libexplain_capability_chown()
                            );
                        if (!may_change_file_owner)
                        {
                            libexplain_string_buffer_puts
                            (
                                sb,
                                "the process does not have the privileges "
                                "needed to change the owner of pathname"
                            );
                            if (chown_restricted)
                            {
                                if (!libexplain_capability_chown())
                                {
                                    libexplain_string_buffer_puts
                                    (
                                        sb,
                                        " (effective uid is "
                                    );
                                    libexplain_buffer_uid(sb, uid);
                                    libexplain_string_buffer_puts
                                    (
                                        sb,
                                        " but needs to be "
                                    );
                                    libexplain_buffer_uid(sb, 0);
                                    libexplain_string_buffer_putc(sb, ')');
                                }
                            }
                            else
                            {
                                if
                                (
                                    uid != st.st_uid
                                &&
                                    !libexplain_capability_chown()
                                )
                                {
                                    libexplain_string_buffer_puts
                                    (
                                        sb,
                                        " (effective uid is "
                                    );
                                    libexplain_buffer_uid(sb, uid);
                                    libexplain_string_buffer_puts
                                    (
                                        sb,
                                        " but needs to be "
                                    );
                                    libexplain_buffer_uid(sb, st.st_uid);
#ifdef HAVE_SYS_CAPABILITY_H
                                    if (libexplain_option_dialect_specific())
                                    {
                                        libexplain_string_buffer_puts
                                        (
                                            sb,
                                            ", or have the CAP_CHOWN capability"
                                        );
                                    }
                                    else
#endif
                                    {
                                        libexplain_string_buffer_puts
                                        (
                                            sb,
                                            " or "
                                        );
                                        libexplain_buffer_uid(sb, 0);
                                    }
                                    libexplain_string_buffer_putc(sb, ')');
                                }
                            }
                            break;
                        }
                    }
                }

                if (group >= 0 && (gid_t)group != st.st_gid)
                {
                    int             may_change_file_group;

                    /*
                     * The owner of a file may change the group of the file
                     * to any group of which that owner is a member.  A
                     * privileged process (root, or CAP_CHOWN) may change
                     * the group arbitrarily.
                     */
                    may_change_file_group =
                        (
                            (
                                uid == st.st_uid
                            &&
                                libexplain_group_in_groups(group)
                            )
                        ||
                            libexplain_capability_chown()
                        );
                    if (!may_change_file_group)
                    {
                        libexplain_string_buffer_puts
                        (
                            sb,
                            "the process does not have the privileges "
                            "needed to change the owner of pathname"
                        );

                        if (uid != st.st_uid)
                        {
                            libexplain_string_buffer_puts
                            (
                                sb,
                                " (effective uid is "
                            );
                            libexplain_buffer_uid(sb, uid);
                            libexplain_string_buffer_puts
                            (
                                sb,
                                " but needs to be "
                            );
                            libexplain_buffer_uid(sb, st.st_uid);
                            libexplain_string_buffer_putc(sb, ')');
                        }
                        if (!libexplain_group_in_groups(group))
                        {
                            libexplain_string_buffer_puts(sb, " (group ");
                            libexplain_buffer_uid(sb, group);
                            libexplain_string_buffer_puts
                            (
                                sb,
                                " is not the effective gid "
                            );
                            libexplain_buffer_uid(sb, getegid());
                            libexplain_string_buffer_puts
                            (
                                sb,
                                " and not in the supplementary gid list "
                            );
                            libexplain_buffer_gid_supplementary(sb);
                            libexplain_string_buffer_putc(sb, ')');
                        }
#ifdef HAVE_SYS_CAPABILITY_H
                        if (libexplain_option_dialect_specific())
                        {
                            libexplain_string_buffer_puts
                            (
                                sb,
                                " (does not have the CAP_CHOWN capability)"
                            );
                        }
#endif
                        break;
                    }
                }
            }

            which =
                (
                    owner >= 0
                ?
                    (group >= 0 ?  "owner and group" : "owner")
                :
                    (group >= 0 ?  "group" : "owner or group")
                );
            libexplain_string_buffer_puts
            (
                sb,
                "the process did not have the required permissions to change "
            );
            libexplain_string_buffer_puts(sb, which);
        }
        break;

    case EROFS:
        libexplain_buffer_erofs(sb, pathname, "pathname");
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_chown(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, int owner, int group)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_chown_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        owner,
        group
    );
    libexplain_buffer_errno_chown_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname,
        owner,
        group
    );
    libexplain_explanation_assemble(&exp, sb);
}
