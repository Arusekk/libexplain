/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/sys/ioctl.h>
#include <libexplain/ac/errno.h>
#include <libexplain/ac/linux/vt.h>

#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/eperm.h>
#include <libexplain/buffer/vt_consize.h>
#include <libexplain/capability.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/vt_resizex.h>
#include <libexplain/path_is_efault.h>

#ifdef VT_RESIZEX


static void
print_explanation(const explain_iocontrol_t *p, explain_string_buffer_t *sb, int
    errnum, int fildes, int request, const void *data)
{
    switch (errnum)
    {
    case EPERM:
        if (!explain_capability_sys_tty_config())
        {
            explain_buffer_eperm_sys_tty_config(sb, "ioctl VT_RESIZEX");
            break;
        }
        goto generic;

    case EINVAL:
        {
            const struct vt_consize *arg;

            arg = data;
            if (explain_pointer_is_efault(arg, sizeof(*arg)))
            {
                if (arg->v_clin > 32)
                {
                    explain_buffer_einval_too_large2(sb, "data.v_clin", 32);
                    break;
                }
                if
                (
                    arg->v_vcol
                &&
                    arg->v_ccol
                &&
                    arg->v_cols != arg->v_vcol / arg->v_ccol)
                {
                    explain_buffer_einval_vague(sb, "data.v_cols");
                    break;
                }
                if
                (
                    arg->v_clin
                &&
                    arg->v_rows
                &&
                    arg->v_rows != arg->v_vlin / arg->v_clin)
                {
                    explain_buffer_einval_vague(sb, "data.v_rows");
                    break;
                }
            }
        }
        goto generic;

    default:
        generic:
        explain_iocontrol_generic_print_explanation
        (
            p,
            sb,
            errnum,
            fildes,
            request,
            data
        );
        break;
    }
}


static void
print_data_returned(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    explain_buffer_vt_consize(sb, data);
}


const explain_iocontrol_t explain_iocontrol_vt_resizex =
{
    "VT_RESIZEX", /* name */
    VT_RESIZEX, /* value */
    0, /* disambiguate */
    0, /* print_name */
    explain_iocontrol_generic_print_data_pointer, /* print_data */
    print_explanation,
    print_data_returned,
    sizeof(struct vt_consize), /* data_size */
    __FILE__,
    __LINE__,
};

#else /* ndef VT_RESIZEX */

const explain_iocontrol_t explain_iocontrol_vt_resizex =
{
    0, /* name */
    0, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
    0, /* print_data_returned */
    0, /* data_size */
    __FILE__,
    __LINE__,
};

#endif /* VT_RESIZEX */
