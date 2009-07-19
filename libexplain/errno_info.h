/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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

#ifndef LIBEXPLAIN_ERRNO_INFO_H
#define LIBEXPLAIN_ERRNO_INFO_H

/**
  * The explain_errno_info_t type describes an errno value, and its
  * (macro) name.
  *
  * The table does NOT contain the strerror() translation bause, (a)
  * that would be redundant, and (b) strerror generously translate
  * according to locale, and that is something we don't want to
  * duplicate, either.
  */
typedef struct explain_errno_info_t explain_errno_info_t;
struct explain_errno_info_t
{
    int error_number;
    const char *name;
};

/**
  * The explain_errno_info_by_number function may be used to locate
  * errno information by errno number.
  *
  * @param errnum
  *     The error number
  * @returns
  *     pointer to info on success, or NULL on failure
  */
const explain_errno_info_t *explain_errno_info_by_number(int errnum);

/**
  * The explain_errno_info_by_name function may be used to locate
  * errno information by errno name.
  *
  * @param name
  *     The error name ("ENOENT", etc)
  * @returns
  *     pointer to info on success, or NULL on failure
  */
const explain_errno_info_t *explain_errno_info_by_name(const char *name);

/**
  * The explain_errno_info_by_name_fuzzy function may be used to
  * locate errno information by errno name, using fuzzy matching.  (For
  * best results, try the exact match first.)
  *
  * @param name
  *     The error name ("ENOENT", etc)
  * @returns
  *     pointer to info on success, or NULL on failure
  */
const explain_errno_info_t *explain_errno_info_by_name_fuzzy(
    const char *name);

/**
  * The explain_errno_info_by_text function may be used to locate
  * errno information by errno text.
  *
  * @param text
  *     The error name ("No such file or directory", etc)
  * @returns
  *     pointer to info on success, or NULL on failure
  */
const explain_errno_info_t *explain_errno_info_by_text(const char *text);

/**
  * The explain_errno_info_by_text_fuzzy function may be used to
  * locate errno information by errno text, using fuzzy matching.  (For
  * best results, try the exact match first.)
  *
  * @param text
  *     The error text ("No such file or directory", etc)
  * @returns
  *     pointer to info on success, or NULL on failure
  */
const explain_errno_info_t *explain_errno_info_by_text_fuzzy(
    const char *text);

#endif /* LIBEXPLAIN_ERRNO_INFO_H */
