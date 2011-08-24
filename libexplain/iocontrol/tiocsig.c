/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/stdint.h>
#include <libexplain/ac/sys/ioctl.h>
#include <libexplain/ac/termios.h>

#include <libexplain/buffer/is_the_null_pointer.h>
#include <libexplain/buffer/signal.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/tiocsig.h>

#ifdef TIOCSIG


static void
print_data(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    explain_buffer_signal(sb, (intptr_t)data);
}


/*
 * FIXME: explain that EINVAL or ENOTTY could be because tty is not a
 * master pty.  How do we tell if a pty *is* a master pty?
 */

const explain_iocontrol_t explain_iocontrol_tiocsig =
{
    "TIOCSIG", /* name */
    TIOCSIG, /* value */
    0, /* disambiguate */
    0, /* print_name */
    print_data,
    0, /* print_explanation */
    0, /* print_data_returned */
    NOT_A_POINTER, /* data_size */
    "intptr_t", /* data_type */
    IOCONTROL_FLAG_NON_META, /* flags */
    __FILE__,
    __LINE__,
};

#else /* ndef TIOCSIG */

const explain_iocontrol_t explain_iocontrol_tiocsig =
{
    0, /* name */
    0, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
    0, /* print_data_returned */
    0, /* data_size */
    0, /* data_type */
    0, /* flags */
    __FILE__,
    __LINE__,
};

#endif /* TIOCSIG */

/* vim: set ts=8 sw=4 et : */
