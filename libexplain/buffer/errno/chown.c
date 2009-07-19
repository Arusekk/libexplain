/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/buffer/dac.h>
#include <libexplain/buffer/does_not_have_inode_modify_permission.h>
#include <libexplain/buffer/eacces.h>
#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/eperm.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/chown.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/gid.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/uid.h>
#include <libexplain/capability.h>
#include <libexplain/explanation.h>
#include <libexplain/group_in_groups.h>
#include <libexplain/option.h>


static void
explain_buffer_errno_chown_system_call(explain_string_buffer_t *sb,
    int errnum, const char *pathname, int owner, int group)
{
    explain_string_buffer_puts(sb, "chown(pathname = ");
    if (errnum == EFAULT)
        explain_buffer_pointer(sb, pathname);
    else
        explain_string_buffer_puts_quoted(sb, pathname);
    explain_string_buffer_puts(sb, ", owner = ");
    explain_buffer_uid(sb, owner);
    explain_string_buffer_puts(sb, ", group = ");
    explain_buffer_gid(sb, group);
    explain_string_buffer_putc(sb, ')');
}


static int
explain_buffer_eperm_chown_st(explain_string_buffer_t *sb,
    const char *pathname, const struct stat *st, int chown_restricted,
    int owner, int group, const explain_final_t *final_component)
{
    uid_t           uid;

    uid = final_component->id.uid;
    if (owner >= 0 && (uid_t)owner != st->st_uid)
    {
        int             may_change_file_owner;

        may_change_file_owner =
            (
                (!chown_restricted && uid == st->st_uid)
            ||
                explain_capability_chown()
            );
        if (!may_change_file_owner)
        {
            if (chown_restricted)
            {
                explain_buffer_gettext
                (
                    sb,
                    /*
                     * xgettext: This message is used to explain an
                     * EPERM error reported by a chown (or similar)
                     * system call, in the case where chown is
                     * restricted, i.e. when it is not sufficent to be
                     * the owner of the file to change its ownership.
                     */
                    i18n("chown is restricted")
                );
                explain_buffer_dac_chown(sb);
            }
            else
            {
                if (!pathname)
                {
                   explain_buffer_does_not_have_inode_modify_permission_fd_st
                    (
                        sb,
                        st,
                        "fildes",
                        &final_component->id
                    );
                }
                else
                {
                    explain_buffer_does_not_have_inode_modify_permission1
                    (
                        sb,
                        pathname,
                        st,
                        "pathname",
                        &final_component->id
                    );
                }
            }
            return 0;
        }
    }

    if (group >= 0 && (gid_t)group != st->st_gid)
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
                    uid == st->st_uid
                &&
                    explain_group_in_groups
                    (
                        group,
                        &final_component->id
                    )
                )
            ||
                explain_capability_chown()
            );
        if (!may_change_file_group)
        {
            explain_string_buffer_printf
            (
                sb,
                /* FIXME: i18n */
                "the process does not have the privileges "
                "needed to change the group GID of %s",
                (pathname ? "pathname" : "fildes")
            );

            if (uid != st->st_uid)
            {
                explain_string_buffer_puts
                (
                    sb,
                    " (effective uid is "
                );
                explain_buffer_uid(sb, uid);
                explain_string_buffer_puts
                (
                    sb,
                    " but needs to be "
                );
                explain_buffer_uid(sb, st->st_uid);
                explain_string_buffer_putc(sb, ')');
            }
            if
            (
                !explain_group_in_groups
                (
                    group,
                    &final_component->id
                )
            )
            {
                explain_string_buffer_puts(sb, " (group ");
                explain_buffer_uid(sb, group);
                explain_string_buffer_puts
                (
                    sb,
                    " is not the effective gid "
                );
                explain_buffer_uid(sb, getegid());
                explain_string_buffer_puts
                (
                    sb,
                    " and not in the supplementary gid list "
                );
                explain_buffer_gid_supplementary(sb);
                explain_string_buffer_putc(sb, ')');
            }
            explain_buffer_dac_chown(sb);
            return 0;
        }
    }

    /*
     * No error, yay.  Except, we were looking for an error, but none
     * was found, so we report failure.
     */
    return -1;
}


static void
explain_buffer_eperm_chown_vague(explain_string_buffer_t *sb, int owner,
    int group)
{
    if (owner >= 0)
    {
        if (group >= 0)
        {
            explain_buffer_gettext
            (
                sb,
                /*
                 * xgettext: The message is used when explaining an EPERM
                 * error reported by the chown(2) system call, in the case
                 * where no more specific explanation is available,
                 * but the call attempted to change both the UID and the GID.
                 */
                i18n("the process did not have the required permissions "
                "to change the owner UID and group GID")
            );
        }
        else
        {
            explain_buffer_gettext
            (
                sb,
                /*
                 * xgettext: The message is used when explaining an EPERM
                 * error reported by the chown(2) system call, in the case
                 * where no more specific explanation is available,
                 * but the call attempted to change only the UID.
                 */
                i18n("the process did not have the required permissions "
                "to change the owner UID")
            );
        }
    }
    else
    {
        if (group >= 0)
        {
            explain_buffer_eperm_setgid(sb);
        }
        else
        {
            explain_buffer_gettext
            (
                sb,
                /*
                 * xgettext: The message is used when explaining an EPERM
                 * error reported by the chown(2) system call, in the case
                 * where no more specific explanation is available.
                 */
                i18n("the process did not have the required permissions "
                "to change the owner UID or group GID")
            );
        }
    }
    explain_buffer_dac_chown(sb);
}


static void
explain_buffer_eperm_chown(explain_string_buffer_t *sb,
    const char *pathname, int owner, int group,
    explain_final_t *final_component)
{
    int             chown_restricted;

    /*
     * We have to ask whether or not the chown(2) system call
     * is restricted or not.  If the system has never heard of
     * _PC_CHOWN_RESTRICTED, we will assume that is is.
     *
     *  1 restricted
     *  0 not restricted
     * -1 assume restricted
     *
     *  If chown_restricted, it means only root can chown the file;
     *  if chown not restricted, it means owner can chown the file.
     */
    chown_restricted = !!pathconf(pathname, _PC_CHOWN_RESTRICTED);
    if (owner >= 0 && chown_restricted && group < 0)
        final_component->want_to_modify_inode = 0;

    /*
     * First we check to see if there is a problem with the pathname itself.
     */
    if
    (
        explain_buffer_errno_path_resolution
        (
            sb,
            EPERM,
            pathname,
            "pathname",
            final_component
        )
    >=
        0
    )
        return;

    /*
     * we get to here if the process DOES have inode modify permission,
     * or permissions could not be determined
     */
    {
        struct stat     st;

        if (stat(pathname, &st) >= 0)
        {
            if
            (
                explain_buffer_eperm_chown_st
                (
                    sb,
                    pathname,
                    &st,
                    chown_restricted,
                    owner,
                    group,
                    final_component
                )
            >=
                0
            )
                return;
        }
    }

    explain_buffer_eperm_chown_vague(sb, owner, group);
}


static void
explain_buffer_eperm_chown_fd(explain_string_buffer_t *sb, int fildes,
    int owner, int group)
{
    int             chown_restricted;
    explain_final_t final_component;

    /*
     * We have to ask whether or not the chown(2) system call
     * is restricted or not.  If the system has never heard of
     * _PC_CHOWN_RESTRICTED, we will assume that is is.
     *
     *  1 restricted
     *  0 not restricted
     * -1 assume restricted
     *
     *  If chown_restricted, it means only root can chown the file;
     *  if chown not restricted, it means owner can chown the file.
     */
    chown_restricted = !!fpathconf(fildes, _PC_CHOWN_RESTRICTED);
    explain_final_init(&final_component);
    if (owner >= 0 && chown_restricted && group < 0)
        final_component.want_to_modify_inode = 0;
    else
        final_component.want_to_modify_inode = 1;

    /*
     * we get to here if the process DOES have inode modify permission,
     * or permissions could not be determined
     */
    {
        struct stat     st;

        if (fstat(fildes, &st) >= 0)
        {
            if
            (
                explain_buffer_eperm_chown_st
                (
                    sb,
                    0,
                    &st,
                    chown_restricted,
                    owner,
                    group,
                    &final_component
                )
            >=
                0
            )
                return;
        }
    }
    explain_buffer_eperm_chown_vague(sb, owner, group);
}


static void
explain_buffer_errno_chown_explanation(explain_string_buffer_t *sb,
    int errnum, const char *pathname, int owner, int group)
{
    explain_final_t final_component;

    explain_final_init(&final_component);
    final_component.want_to_modify_inode = 1;

    explain_buffer_errno_chown_explanation_fc
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
explain_buffer_errno_chown_explanation_fc(explain_string_buffer_t *sb,
    int errnum, const char *pathname, int owner, int group,
    explain_final_t *final_component)
{
    final_component->want_to_modify_inode = 1;

    switch (errnum)
    {
    case EACCES:
        explain_buffer_eacces(sb, pathname, "pathname", final_component);
        break;

    case EFAULT:
        explain_buffer_efault(sb, "pathname");
        break;

    case EIO:
        explain_buffer_eio_path(sb, pathname);
        break;

    case EINVAL:
        explain_string_buffer_puts
        (
            sb,
            /* FIXME: i18n */
            "the owner UID or group GID is not a value supported by the "
            "implementation; or, the fildes argument refers to a pipe or "
            "socket and the implementation disallows execution of fchown() on "
            "a pipe"
        );
        break;

    case ELOOP:
        explain_buffer_eloop(sb, pathname, "pathname", final_component);
        break;

    case ENAMETOOLONG:
        explain_buffer_enametoolong
        (
            sb,
            pathname,
            "pathname",
            final_component
        );
        break;

    case ENOENT:
        explain_buffer_enoent(sb, pathname, "pathname", final_component);
        break;

    case ENOMEM:
        explain_buffer_enomem_kernel(sb);
        break;

    case ENOTDIR:
        explain_buffer_enotdir(sb, pathname, "pathname", final_component);
        break;

    case EPERM:
        explain_buffer_eperm_chown
        (
            sb,
            pathname,
            owner,
            group,
            final_component
        );
        break;

    case EROFS:
        explain_buffer_erofs(sb, pathname, "pathname");
        break;

    default:
        explain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
explain_buffer_errno_fchown_explanation(explain_string_buffer_t *sb,
    int errnum, int fildes, int owner, int group)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/fchown.html
     */
    switch (errnum)
    {
    case EBADF:
        explain_buffer_ebadf(sb, fildes, "fildes");
        break;

    case EIO:
        explain_buffer_eio_fildes(sb, fildes);
        break;

    case EINVAL:
        explain_string_buffer_puts
        (
            sb,
            /* FIXME: i18n */
            "the owner UID or group GID is not a value supported by the "
            "implementation; or, the fildes argument refers to a pipe or "
            "socket and the implementation disallows execution of fchown() on "
            "a pipe"
        );
        break;

    case ENOMEM:
        explain_buffer_enomem_kernel(sb);
        break;

    case EPERM:
        explain_buffer_eperm_chown_fd(sb, fildes, owner, group);
        break;

    case EROFS:
        explain_buffer_erofs_fildes(sb, fildes, "fildes");
        break;

    default:
        explain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
explain_buffer_errno_chown(explain_string_buffer_t *sb, int errnum,
    const char *pathname, int owner, int group)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_chown_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        owner,
        group
    );
    explain_buffer_errno_chown_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname,
        owner,
        group
    );
    explain_explanation_assemble(&exp, sb);
}
