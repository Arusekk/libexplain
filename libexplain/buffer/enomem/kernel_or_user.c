/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2013 Peter Miller
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/gettext.h>


void
explain_buffer_enomem_kernel_or_user(explain_string_buffer_t *sb)
{
    explain_buffer_gettext
    (
        sb,
        /*
         * xgettext:  This message is used when explaining an ENOMEM
         * error, when it is not possible to distinguish whether it was
         * kernel memory of user space memory that was exhausted.
         */
        i18n("insufficient user-space or kernel memory was available")
    );
}


/* vim: set ts=8 sw=4 et : */
