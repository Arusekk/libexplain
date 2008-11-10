/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#ifndef LIBEXPLAIN_BUFFER_ERRNO_PATH_RESOLUTION_H
#define LIBEXPLAIN_BUFFER_ERRNO_PATH_RESOLUTION_H

#include <libexplain/string_buffer.h>

typedef struct libexplain_final_t libexplain_final_t;
struct libexplain_final_t
{
    unsigned        want_to_read    :1;
    unsigned        want_to_write   :1;
    unsigned        want_to_search  :1;
    unsigned        want_to_execute :1;
    unsigned        want_to_create  :1;
    unsigned        want_to_modify_inode :1;
    unsigned        want_to_unlink  :1;
    unsigned        must_exist      :1;
    unsigned        must_not_exist  :1;
    unsigned        must_be_a_directory :1;
    unsigned        follow_symlink  :1;

    /**
      * When want_to_create, this is an indication of what is about to
      * be created, so that it can be used to inform the error messages.
      */
    int             st_mode;

    /*
     * When you add to this struct, be sure to add to the
     * libexplain_final_init() code which initialises the fields to
     * default settings.
     */
};

/**
  * The libexplain_final_init function is used to initialise all of the
  * members of a libexplain_final_t struct to their defaults (usually 0).
  *
  * @param final_component
  *     Pointer to struct to be initialised.
  */
void libexplain_final_init(libexplain_final_t *final_component);

/**
  * The libexplain_buffer_errno_path_resolution function may be used to
  * check a path for problems.
  *
  * @param sb
  *    The string buffer to write the error to, once it is found
  * @param errnum
  *    The error number expected.
  * @param pathname
  *    The path being checked.
  * @param pathname_caption
  *    The name of the argument being checked in the function arguments
  *    of the system call being deciphered.
  * @param final_component
  *    Flags controlling the final component
  * @returns
  *    0 on success, meaning it found an error and wrote it to sb;
  *    -1 on failure, meaning it didn't find an error or it didn't find
  *    the expected error.
  */
int libexplain_buffer_errno_path_resolution(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, const char *pathname_caption,
    const libexplain_final_t *final_component);

#endif /* LIBEXPLAIN_BUFFER_ERRNO_PATH_RESOLUTION_H */
