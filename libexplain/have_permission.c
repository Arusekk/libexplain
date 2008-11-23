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

#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/dac.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/gid.h>
#include <libexplain/buffer/uid.h>
#include <libexplain/capability.h>
#include <libexplain/group_in_groups.h>
#include <libexplain/have_permission.h>
#include <libexplain/option.h>


static int
libexplain_have_permission(const struct stat *st, int wanted)
{
    /*
     * The first group of three is used when the effective user ID of
     * the calling process equals the owner ID of the file.
     */
    if (st->st_uid == geteuid())
    {
        return (0 != (st->st_mode & wanted & S_IRWXU));
    }

    /*
     * The second group of three is used when the group ID of the file
     * either equals the effective group ID of the calling process, or
     * is one of the supplementary group IDs of the calling process (as
     * set by setgroups(2)).
     */
    if (libexplain_group_in_groups(st->st_gid))
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


static int
libexplain_explain_permission(libexplain_string_buffer_t *sb,
    const struct stat *st, int wanted)
{
    char            part1[40];
    libexplain_string_buffer_t part1_sb;
    char            part2[40];
    libexplain_string_buffer_t part2_sb;
    char            part3[8];
    libexplain_string_buffer_t part3_sb;
    char            filtyp[100];
    libexplain_string_buffer_t filtyp_sb;

    if (!libexplain_option_dialect_specific())
    {
        /*
         * The test suite doesn't need to see the uid and name, or the
         * gid and name, because these give false negatives.
         */
        return -1;
    }

    libexplain_string_buffer_init(&filtyp_sb, filtyp, sizeof(filtyp));
    libexplain_buffer_file_type(&filtyp_sb, st->st_mode);

    /*
     * The first group of three is used when the effective user ID of
     * the calling process equals the owner ID of the file.
     */
    if (st->st_uid == geteuid())
    {
        libexplain_string_buffer_init(&part1_sb, part1, sizeof(part1));
        libexplain_buffer_uid(&part1_sb, geteuid());
        libexplain_string_buffer_init(&part2_sb, part2, sizeof(part2));
        libexplain_buffer_uid(&part2_sb, st->st_uid);
        libexplain_string_buffer_init(&part3_sb, part3, sizeof(part3));
        libexplain_buffer_rwx(&part3_sb, st->st_mode & S_IRWXU);

        libexplain_string_buffer_puts(sb, ", ");
        libexplain_string_buffer_printf_gettext
        (
            sb,
            /*
             * xgettext: This message is used when explaining which
             * permission mode bits are used when determining file access
             * permissions.
             *
             * %1$s => the effective UID of the process, number and name.
             * %2$s => the file type, e.g. "directory" or "regular file"
             * %3$s => the owner of the file, number and name.
             * %4$s => The mode bits like "rwx", including the quotes, in
             *         a form resembling the ls -l representation of mode
             *         bits.
             */
            i18n("the process effective UID %s matches the %s owner %s and the "
                "owner permission mode is %s"),
            part1,
            filtyp,
            part2,
            part3
        );

        if
        (
            (st->st_mode & wanted & S_IRWXG)
        &&
            libexplain_group_in_groups(st->st_gid)
        )
        {
            libexplain_string_buffer_init(&part3_sb, part3, sizeof(part3));
            libexplain_buffer_rwx(&part3_sb, st->st_mode & S_IRWXG);
            libexplain_string_buffer_puts(sb, ", ");
            libexplain_string_buffer_printf_gettext
            (
                sb,
                /*
                 * xgettext: This message is used when explaining why
                 * some permission mode bits are ignored.  The %s string
                 * will be the "rwx" bits, including the quotes, as.
                 * The 3-character string will look like ls -l output.
                 */
                i18n("the group permission mode %s is ignored"),
                part3
            );
        }
        if (st->st_mode & wanted & S_IRWXO)
        {
            libexplain_string_buffer_init(&part3_sb, part3, sizeof(part3));
            libexplain_buffer_rwx(&part3_sb, st->st_mode & S_IRWXO);
            libexplain_string_buffer_puts(sb, ", ");
            libexplain_string_buffer_printf_gettext
            (
                sb,
                /*
                 * xgettext: This message is used when explaining why
                 * some permission mode bits are ignored.  The %s string
                 * will be the "rwx" bits, including the quotes, as.
                 * The 3-character string will look like ls -l output.
                 */
                i18n("the others permission mode %s is ignored"),
                part3
            );
        }
        return (0 != (st->st_mode & wanted & S_IRWXU));
    }

    /*
     * The second group of three is used when the group ID of the file
     * either equals the effective group ID of the calling process, or
     * is one of the supplementary group IDs of the calling process (as
     * set by setgroups(2)).
     */
    if (libexplain_group_in_groups(st->st_gid))
    {
        if (st->st_mode & wanted & S_IRWXU)
        {
            libexplain_string_buffer_init(&part1_sb, part1, sizeof(part1));
            libexplain_buffer_uid(&part1_sb, geteuid());
            libexplain_string_buffer_init(&part2_sb, part2, sizeof(part2));
            libexplain_buffer_uid(&part2_sb, st->st_uid);
            libexplain_string_buffer_init(&part3_sb, part3, sizeof(part3));
            libexplain_buffer_rwx(&part3_sb, st->st_mode & S_IRWXU);

            libexplain_string_buffer_puts(sb, ", ");
            libexplain_string_buffer_printf_gettext
            (
                sb,
                /*
                 * xgettext: This message is used when explaining which
                 * permission mode bits are ignored when determining
                 * file access permissions.
                 *
                 * %1$s => the effective UID of the process, number and name.
                 * %2$s => the file type, e.g. "firectory" or "regular file"
                 * %3$s => the owner of the file, number and name.
                 * %4$s => The mode bits like "rwx", including the quotes, in
                 *         a form resembling the ls -l representation of mode
                 *         bits.
                 */
                i18n("the process effective UID %s does not match the %s owner "
                    "%s so the owner permission mode %s is ignored"),
                part1,
                filtyp,
                part2,
                part3
            );
        }

        libexplain_string_buffer_init(&part1_sb, part1, sizeof(part1));
        libexplain_buffer_gid(&part1_sb, getegid());
        libexplain_string_buffer_init(&part2_sb, part2, sizeof(part2));
        libexplain_buffer_gid(&part2_sb, st->st_gid);
        libexplain_string_buffer_init(&part3_sb, part3, sizeof(part3));
        libexplain_buffer_rwx(&part3_sb, st->st_mode & S_IRWXG);

        libexplain_string_buffer_puts(sb, ", ");
        libexplain_string_buffer_printf_gettext
        (
            sb,
            /*
             * xgettext: This message is used when explaining which
             * permission mode bits are used when determining file access
             * permissions.
             *
             * %1$s => the effective GID of the process, number and name.
             * %2$s =? the file type, e.g. 'directory' or 'regular file'
             * %3$s => the group of the file, number and name.
             * %4$s => The mode bits like "rwx", including the quotes, in
             *         a form resembling the ls -l representation of mode
             *         bits.
             */
            i18n("the process effective GID %s matches the %s group %s and the "
                "group permission mode is %s"),
            part1,
            filtyp,
            part2,
            part3
        );

        if (st->st_mode & wanted & S_IRWXO)
        {
            libexplain_string_buffer_init(&part3_sb, part3, sizeof(part3));
            libexplain_buffer_rwx(&part3_sb, st->st_mode & S_IRWXO);
            libexplain_string_buffer_puts(sb, ", ");
            libexplain_string_buffer_printf_gettext
            (
                sb,
                i18n("the others permission mode %s is ignored"),
                part3
            );
        }
        return (0 != (st->st_mode & wanted & S_IRWXG));
    }

    /*
     * When neither holds, the third group is used.
     */
    if (st->st_mode & wanted & S_IRWXU)
    {
        libexplain_string_buffer_init(&part1_sb, part1, sizeof(part1));
        libexplain_buffer_uid(&part1_sb, geteuid());
        libexplain_string_buffer_init(&part2_sb, part2, sizeof(part2));
        libexplain_buffer_uid(&part2_sb, st->st_uid);
        libexplain_string_buffer_init(&part3_sb, part3, sizeof(part3));
        libexplain_buffer_rwx(&part3_sb, st->st_mode & S_IRWXU);

        libexplain_string_buffer_puts(sb, ", ");
        libexplain_string_buffer_printf_gettext
        (
            sb,
            i18n("the process effective UID %s does not match the %s owner %s "
                 "so the owner permission mode %s is ignored"),
            part1,
            filtyp,
            part2,
            part3
        );
    }
    if (st->st_mode & wanted & S_IRWXG)
    {
        libexplain_string_buffer_init(&part1_sb, part1, sizeof(part1));
        libexplain_buffer_gid(&part1_sb, getegid());
        libexplain_string_buffer_init(&part2_sb, part2, sizeof(part2));
        libexplain_buffer_gid(&part2_sb, st->st_gid);
        libexplain_string_buffer_init(&part3_sb, part3, sizeof(part3));
        libexplain_buffer_rwx(&part3_sb, st->st_mode & S_IRWXG);

        libexplain_string_buffer_puts(sb, ", ");
        libexplain_string_buffer_printf_gettext
        (
            sb,
            /*
             * xgettext: This message is used when explaining which
             * permission mode bits are ignored when determining file
             * access permissions.
             *
             * %1$s => the effective GID of the process, number and name.
             * %2$s => the file type, e.g. "directory" or "regular file"
             * %3$s => the group of the file, number and name.
             * %4$s => The mode bits like "rwx", including the quotes, in
             *         a form resembling the ls -l representation of mode
             *         bits.
             */
            i18n("the process effective GID %s does not match the %s group %s "
                 "so the group permission mode %s is ignored"),
            part1,
            filtyp,
            part2,
            part3
        );
    }
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
         * The %s string will contain "rwx", including the quotes, in a
         * form resembling the ls -l representation of mode bits.
         */
        i18n("the others permission mode is %s"),
        part3
    );
    return (0 != (st->st_mode & wanted & S_IRWXO));
}


int
libexplain_have_read_permission(const struct stat *st)
{
    if (libexplain_capability_dac_read_search())
        return 1;
    return libexplain_have_permission(st, 0444);
}


int
libexplain_explain_read_permission(libexplain_string_buffer_t *sb,
    const struct stat *st)
{
    int             result;

    if (libexplain_capability_dac_read_search())
        return 1;
    result = libexplain_explain_permission(sb, st, 0444);
    if (!result)
        libexplain_buffer_dac_read_search(sb);
    return result;
}


int
libexplain_have_write_permission(const struct stat *st)
{
    if (libexplain_capability_dac_override())
        return 1;
    return libexplain_have_permission(st, 0222);
}


int
libexplain_explain_write_permission(libexplain_string_buffer_t *sb,
    const struct stat *st)
{
    int             result;

    if (libexplain_capability_dac_override())
        return 1;
    result = libexplain_explain_permission(sb, st, 0222);
    if (!result)
        libexplain_buffer_dac_override(sb);
    return result;
}


int
libexplain_have_execute_permission(const struct stat *st)
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
    return libexplain_have_permission(st, 0111);
}


int
libexplain_explain_execute_permission(libexplain_string_buffer_t *sb,
    const struct stat *st)
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
    result = libexplain_explain_permission(sb, st, 0111);
    if (!result)
        libexplain_buffer_dac_override(sb);
    return result;
}


int
libexplain_have_search_permission(const struct stat *st)
{
    if (!S_ISDIR(st->st_mode))
        return 0;
    if (libexplain_capability_dac_read_search())
        return 1;
    return libexplain_have_permission(st, 0111);
}


int
libexplain_explain_search_permission(libexplain_string_buffer_t *sb,
    const struct stat *st)
{
    int             result;

    if (!S_ISDIR(st->st_mode))
        return 0;
    if (libexplain_capability_dac_read_search())
        return 1;
    result = libexplain_explain_permission(sb, st, 0111);
    if (!result)
        libexplain_buffer_dac_read_search(sb);
    return result;
}


int
libexplain_have_inode_permission(const struct stat *st)
{
    if (libexplain_capability_fowner())
        return 1;
    return (geteuid() == st->st_uid);
}
