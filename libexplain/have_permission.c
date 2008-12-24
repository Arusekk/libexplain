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

#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/dac.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/gid.h>
#include <libexplain/buffer/uid.h>
#include <libexplain/capability.h>
#include <libexplain/gettext.h>
#include <libexplain/group_in_groups.h>
#include <libexplain/have_permission.h>
#include <libexplain/option.h>


static int
libexplain_have_permission(const struct stat *st,
    const libexplain_have_identity_t *hip, int wanted)
{
    /*
     * The first group of three is used when the effective user ID of
     * the process equals the owner ID of the file.
     */
    if (st->st_uid == (uid_t)hip->uid)
    {
        return (0 != (st->st_mode & wanted & S_IRWXU));
    }

    /*
     * The second group of three is used when the group ID of the file
     * either equals the effective group ID of the process, or
     * is one of the supplementary group IDs of the process (as
     * set by setgroups(2)).
     */
    if (libexplain_group_in_groups(st->st_gid, hip))
    {
        return (0 != (st->st_mode & wanted & S_IRWXG));
    }

    /*
     * When neither holds, the third group is used.
     */
    return (0 != (st->st_mode & wanted & S_IRWXO));
}


static void
libexplain_buffer_rwx(libexplain_string_buffer_t *sb, int mode_bits)
{
    libexplain_string_buffer_putc(sb, '"');
    libexplain_string_buffer_putc(sb, ((mode_bits & 0444) ? 'r' : '-'));
    libexplain_string_buffer_putc(sb, ((mode_bits & 0222) ? 'w' : '-'));
    libexplain_string_buffer_putc(sb, ((mode_bits & 0111) ? 'x' : '-'));
    libexplain_string_buffer_putc(sb, '"');
}


static void
owner_permission_mode_used(libexplain_string_buffer_t *sb,
    const struct stat *st, const libexplain_have_identity_t *hip)
{
    char            part1[40];
    libexplain_string_buffer_t part1_sb;
    char            part2[40];
    libexplain_string_buffer_t part2_sb;
    char            part3[8];
    libexplain_string_buffer_t part3_sb;
    char            filtyp[100];
    libexplain_string_buffer_t filtyp_sb;

    libexplain_string_buffer_init(&part1_sb, part1, sizeof(part1));
    libexplain_buffer_uid(&part1_sb, hip->uid);
    libexplain_string_buffer_init(&part2_sb, part2, sizeof(part2));
    libexplain_buffer_uid(&part2_sb, st->st_uid);
    libexplain_string_buffer_init(&part3_sb, part3, sizeof(part3));
    libexplain_buffer_rwx(&part3_sb, st->st_mode & S_IRWXU);
    libexplain_string_buffer_init(&filtyp_sb, filtyp, sizeof(filtyp));
    libexplain_buffer_file_type(&filtyp_sb, st->st_mode);

    libexplain_string_buffer_puts(sb, ", ");
    libexplain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used when explaining which
         * permission mode bits are used when determining file access
         * permissions.
         *
         * %1$s => the kind of UID, "real UID" or "effective UID",
         *         already translated.
         * %2$s => the UID of the process, number and name.
         * %3$s => the file type, e.g. "directory" or "regular file"
         * %4$s => the owner of the file, number and name.
         * %5$s => The mode bits like "rwx", including the quotes, in
         *         a form resembling the ls -l representation of mode
         *         bits.
         */
        i18n("the process %s %s matches the %s owner UID %s and the "
            "owner permission mode is %s"),
        libexplain_have_identity_kind_of_uid(hip),
        part1,
        filtyp,
        part2,
        part3
    );
}


static void
owner_permission_mode_ignored(libexplain_string_buffer_t *sb,
    const struct stat *st, const libexplain_have_identity_t *hip)
{
    char            part1[40];
    libexplain_string_buffer_t part1_sb;
    char            part2[40];
    libexplain_string_buffer_t part2_sb;
    char            part3[8];
    libexplain_string_buffer_t part3_sb;
    char            filtyp[100];
    libexplain_string_buffer_t filtyp_sb;

    libexplain_string_buffer_init(&part1_sb, part1, sizeof(part1));
    libexplain_buffer_uid(&part1_sb, hip->uid);
    libexplain_string_buffer_init(&part2_sb, part2, sizeof(part2));
    libexplain_buffer_uid(&part2_sb, st->st_uid);
    libexplain_string_buffer_init(&part3_sb, part3, sizeof(part3));
    libexplain_buffer_rwx(&part3_sb, st->st_mode & S_IRWXU);
    libexplain_string_buffer_init(&filtyp_sb, filtyp, sizeof(filtyp));
    libexplain_buffer_file_type(&filtyp_sb, st->st_mode);

    libexplain_string_buffer_puts(sb, ", ");
    libexplain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used when explaining which
         * permission mode bits are ignored when determining
         * file access permissions.
         *
         * %1$s => the kind of GID, "real GID" or "effective GID",
         *         already translated
         * %2$s => the GID of the process, number and name.
         * %3$s => the file type, e.g. "directory" or "regular file"
         * %4$s => the owner of the file, number and name.
         * %5$s => The mode bits like "rwx", including the quotes, in
         *         a form resembling the ls -l representation of mode
         *         bits.
         */
        i18n("the process %s %s does not match the %s owner "
            "%s so the owner permission mode %s is ignored"),
        libexplain_have_identity_kind_of_gid(hip),
        part1,
        filtyp,
        part2,
        part3
    );
}


static void
group_permission_mode_used(libexplain_string_buffer_t *sb,
    const struct stat *st, const libexplain_have_identity_t *hip)
{
    char            part1[40];
    libexplain_string_buffer_t part1_sb;
    char            part2[40];
    libexplain_string_buffer_t part2_sb;
    char            part3[8];
    libexplain_string_buffer_t part3_sb;
    char            filtyp[100];
    libexplain_string_buffer_t filtyp_sb;

    libexplain_string_buffer_init(&part1_sb, part1, sizeof(part1));
    libexplain_buffer_gid(&part1_sb, hip->gid);
    libexplain_string_buffer_init(&part2_sb, part2, sizeof(part2));
    libexplain_buffer_gid(&part2_sb, st->st_gid);
    libexplain_string_buffer_init(&part3_sb, part3, sizeof(part3));
    libexplain_buffer_rwx(&part3_sb, st->st_mode & S_IRWXG);
    libexplain_string_buffer_init(&filtyp_sb, filtyp, sizeof(filtyp));
    libexplain_buffer_file_type(&filtyp_sb, st->st_mode);

    libexplain_string_buffer_puts(sb, ", ");
    libexplain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used when explaining which
         * permission mode bits are used when determining file access
         * permissions.
         *
         * %1$s => the kind of GID, "real GID" or "effective GID",
         *         already translated
         * %2$s => the GID of the process, number and name.
         * %3$s => the file type, e.g. 'directory' or 'regular file'
         * %4$s => the group of the file, number and name.
         * %5$s => The mode bits like "rwx", including the quotes, in
         *         a form resembling the ls -l representation of mode
         *         bits.
         */
        i18n("the process %s %s matches the %s group GID %s and the "
            "group permission mode is %s"),
        libexplain_have_identity_kind_of_gid(hip),
        part1,
        filtyp,
        part2,
        part3
    );
}


static void
group_permission_mode_ignored(libexplain_string_buffer_t *sb,
    const struct stat *st, const libexplain_have_identity_t *hip)
{
    char            part3[10];
    libexplain_string_buffer_t part3_sb;

    (void)hip;
    libexplain_string_buffer_init(&part3_sb, part3, sizeof(part3));
    libexplain_buffer_rwx(&part3_sb, st->st_mode & S_IRWXG);
    libexplain_string_buffer_puts(sb, ", ");
    libexplain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used when explaining why some
         * permission mode bits are ignored.
         *
         * %1$s => text representation of the "rwx" bits, including the
         *         quotes.  The 3-character string will look like ls -l
         *         output.
         */
        i18n("the group permission mode %s is ignored"),
        part3
    );
}


static void
others_permission_mode_used(libexplain_string_buffer_t *sb,
    const struct stat *st, const libexplain_have_identity_t *hip)
{
    char            part3[10];
    libexplain_string_buffer_t part3_sb;

    (void)hip;
    libexplain_string_buffer_init(&part3_sb, part3, sizeof(part3));
    libexplain_buffer_rwx(&part3_sb, st->st_mode & S_IRWXO);
    libexplain_string_buffer_puts(sb, ", ");
    libexplain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used when explaining which
         * permission mode bits are used when determining file access
         * permsiions.
         *
         * %1$s => the "rwx" mode representation, including the quotes, in a
         *         form resembling the ls -l representation of mode bits.
         */
        i18n("the others permission mode is %s"),
        part3
    );
}


static void
others_permission_mode_ignored(libexplain_string_buffer_t *sb,
    const struct stat *st, const libexplain_have_identity_t *hip)
{
    char            mode_text[10];
    libexplain_string_buffer_t mode_text_sb;

    (void)hip;
    libexplain_string_buffer_init(&mode_text_sb, mode_text, sizeof(mode_text));
    libexplain_buffer_rwx(&mode_text_sb, st->st_mode & S_IRWXO);
    libexplain_string_buffer_puts(sb, ", ");
    libexplain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used when explaining why
         * the "other" permission mode bits are ignored.
         *
         * %1$s => the "rwx" bits, including the quotes, like the
         *         3-character string used in 'ls -l' output.
         */
        i18n("the others permission mode %s is ignored"),
        mode_text
    );
}


static int
libexplain_explain_permission(libexplain_string_buffer_t *sb,
    const struct stat *st, const libexplain_have_identity_t *hip, int wanted)
{
    if (!libexplain_option_dialect_specific())
    {
        /*
         * The test suite doesn't need to see the uid and name, or the
         * gid and name, because these give false negatives.
         */
        return -1;
    }

    /*
     * The first group of three (0700) is used when the effective user
     * ID of the process equals the owner ID of the file.
     */
    if (st->st_uid == (uid_t)hip->uid)
    {
        owner_permission_mode_used(sb, st, hip);

        if
        (
            (st->st_mode & wanted & S_IRWXG)
        &&
            libexplain_group_in_groups(st->st_gid, hip)
        )
        {
            group_permission_mode_ignored(sb, st, hip);
        }
        if (st->st_mode & wanted & S_IRWXO)
        {
            others_permission_mode_ignored(sb, st, hip);
        }
        return (0 != (st->st_mode & wanted & S_IRWXU));
    }

    /*
     * The second group of three (0070) is used when the group ID of the
     * file either equals the effective group ID of the process,
     * or is one of the supplementary group IDs of the process
     * (as set by setgroups(2)).
     */
    if (libexplain_group_in_groups(st->st_gid, hip))
    {
        if (st->st_mode & wanted & S_IRWXU)
        {
            owner_permission_mode_ignored(sb, st, hip);
        }

        group_permission_mode_used(sb, st, hip);

        if (st->st_mode & wanted & S_IRWXO)
        {
            others_permission_mode_ignored(sb, st, hip);
        }
        return (0 != (st->st_mode & wanted & S_IRWXG));
    }

    /*
     * When neither holds, the third group is used.
     */
    if (st->st_mode & wanted & S_IRWXU)
    {
        owner_permission_mode_ignored(sb, st, hip);
    }
    if (st->st_mode & wanted & S_IRWXG)
    {
        group_permission_mode_ignored(sb, st, hip);
    }
    others_permission_mode_used(sb, st, hip);
    return (0 != (st->st_mode & wanted & S_IRWXO));
}


int
libexplain_have_read_permission(const struct stat *st,
    const libexplain_have_identity_t *hip)
{
    if (libexplain_capability_dac_read_search())
        return 1;
    return libexplain_have_permission(st, hip, 0444);
}


int
libexplain_explain_read_permission(libexplain_string_buffer_t *sb,
    const struct stat *st, const libexplain_have_identity_t *hip)
{
    int             result;

    if (libexplain_capability_dac_read_search())
        return 1;
    result = libexplain_explain_permission(sb, st, hip, 0444);
    if (!result)
        libexplain_buffer_dac_read_search(sb);
    return result;
}


int
libexplain_have_write_permission(const struct stat *st,
    const libexplain_have_identity_t *hip)
{
    if (libexplain_capability_dac_override())
        return 1;
    return libexplain_have_permission(st, hip, 0222);
}


int
libexplain_explain_write_permission(libexplain_string_buffer_t *sb,
    const struct stat *st, const libexplain_have_identity_t *hip)
{
    int             result;

    if (libexplain_capability_dac_override())
        return 1;
    result = libexplain_explain_permission(sb, st, hip, 0222);
    if (!result)
        libexplain_buffer_dac_override(sb);
    return result;
}


int
libexplain_have_execute_permission(const struct stat *st,
    const libexplain_have_identity_t *hip)
{
    if (!S_ISREG(st->st_mode))
        return 0;
    if
    (
        libexplain_capability_dac_override()
#ifdef __linux__
    &&
        (st->st_mode & 0111)
#endif
    )
        return 1;
    return libexplain_have_permission(st, hip, 0111);
}


int
libexplain_explain_execute_permission(libexplain_string_buffer_t *sb,
    const struct stat *st, const libexplain_have_identity_t *hip)
{
    int             result;

    if (!S_ISREG(st->st_mode))
        return 0;
    if
    (
        libexplain_capability_dac_override()
#ifdef __linux__
    &&
        (st->st_mode & 0111)
#endif
    )
        return 1;
    result = libexplain_explain_permission(sb, st, hip, 0111);
    if (!result)
        libexplain_buffer_dac_override(sb);
    return result;
}


int
libexplain_have_search_permission(const struct stat *st,
    const libexplain_have_identity_t *hip)
{
    if (!S_ISDIR(st->st_mode))
        return 0;
    if (libexplain_capability_dac_read_search())
        return 1;
    return libexplain_have_permission(st, hip, 0111);
}


int
libexplain_explain_search_permission(libexplain_string_buffer_t *sb,
    const struct stat *st, const libexplain_have_identity_t *hip)
{
    int             result;

    if (!S_ISDIR(st->st_mode))
        return 0;
    if (libexplain_capability_dac_read_search())
        return 1;
    result = libexplain_explain_permission(sb, st, hip, 0111);
    if (!result)
        libexplain_buffer_dac_read_search(sb);
    return result;
}


int
libexplain_have_inode_permission(const struct stat *st,
    const libexplain_have_identity_t *hip)
{
    if (libexplain_capability_fowner())
        return 1;
    return ((uid_t)hip->uid == st->st_uid);
}


const char *
libexplain_have_identity_kind_of_uid(const libexplain_have_identity_t *hip)
{
    if ((uid_t)hip->uid != geteuid())
    {
        return
            libexplain_gettext
            (
                /*
                 * xgettext: This phrase is used to distinguish which of
                 * the process's UIDs are in use during the permissions
                 * check.  In this case, the real UID.
                 */
                i18n("real UID")
            );
    }
    else
    {
        return
            libexplain_gettext
            (
                /*
                 * xgettext: This phrase is used to distinguish which of
                 * the process's UIDs are in use during the permissions
                 * check.  In this case, the effective UID.
                 */
                i18n("effective UID")
            );
    }
}


const char *
libexplain_have_identity_kind_of_gid(const libexplain_have_identity_t *hip)
{
    if ((gid_t)hip->gid != getegid())
    {
        return
            libexplain_gettext
            (
                /*
                 * xgettext: This phrase is used to distinguish which of
                 * the process's GIDs are in use during the permissions
                 * check.  In this case, the real gid.
                 */
                i18n("real GID")
            );
    }
    else
    {
        return
            libexplain_gettext
            (
                /*
                 * xgettext: This phrase is used to distinguish which of
                 * the process's GIDs are in use during the permissions
                 * check.  In this case, the effective gid.
                 */
                i18n("effective GID")
            );
    }
}


void
libexplain_have_identity_init(libexplain_have_identity_t *hip)
{
    hip->uid = geteuid();
    hip->gid = getegid();
}
