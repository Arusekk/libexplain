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
#include <libexplain/ac/netdb.h>

#include <libexplain/buffer/addrinfo.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/getaddrinfo.h>
#include <libexplain/buffer/pathname.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>
#include <libexplain/path_is_efault.h>


static void
libexplain_buffer_errcode_getaddrinfo_system_call(
    libexplain_string_buffer_t *sb, int errcode, const char *node,
    const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
    (void)errcode;
    libexplain_string_buffer_puts(sb, "getaddrinfo(node = ");
    libexplain_buffer_pathname(sb, node);
    libexplain_string_buffer_puts(sb, ", service = ");
    libexplain_buffer_pathname(sb, service);
    libexplain_string_buffer_puts(sb, ", hints = ");
    libexplain_buffer_addrinfo(sb, hints);
    libexplain_string_buffer_puts(sb, ", res = ");
    libexplain_buffer_pointer(sb, res);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errcode_getaddrinfo_explanation(
    libexplain_string_buffer_t *sb, int errcode, const char *node,
    const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
    if (errcode > 0)
    {
        switch (errcode)
        {
        case EFAULT:
            if (node && libexplain_path_is_efault(node))
            {
                libexplain_buffer_efault(sb, "node");
                break;
            }
            if (service && libexplain_path_is_efault(service))
            {
                libexplain_buffer_efault(sb, "service");
                break;
            }
            if (hints && libexplain_pointer_is_efault(hints, sizeof(*hints)))
            {
                libexplain_buffer_efault(sb, "hints");
                break;
            }
            if (res && libexplain_pointer_is_efault(res, sizeof(*res)))
            {
                libexplain_buffer_efault(sb, "res");
                break;
            }
            break;

        case ENOMEM:
            libexplain_buffer_enomem_kernel(sb);
            break;

        default:
            break;
        }
        return;
    }

    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/getaddrinfo.html
     */
    switch (errcode)
    {
    case EAI_ADDRFAMILY:
        libexplain_string_buffer_puts
        (
            sb,
            "the specified network host does not have any network "
            "addresses in the requested address family"
        );

    case EAI_AGAIN:
        libexplain_string_buffer_puts
        (
            sb,
            "the name server returned a temporary failure indication, "
            "future attempts may succeed"
        );
        break;

    case EAI_BADFLAGS:
        libexplain_string_buffer_puts
        (
            sb,
            "the ai->ai_flags parameter had an invalid value"
        );
        break;

    case EAI_FAIL:
        libexplain_string_buffer_puts
        (
            sb,
            "the name server returned a permanent failure indication "
            "when attempting to resolve the name"
        );
        break;

    case EAI_FAMILY:
        libexplain_string_buffer_puts
        (
            sb,
            "the requested address family is not supported"
        );
        break;

    case EAI_NODATA:
        libexplain_string_buffer_puts
        (
            sb,
            "the specified network host exists, but does not have any "
            "network addresses defined"
        );
        break;

    case EAI_NONAME:
        if (!node && !service)
        {
            libexplain_string_buffer_puts
            (
                sb,
                "both node and service are NULL, you have to specify at "
                "least one of them"
            );
            break;
        }
        if (hints && (hints->ai_flags & AI_NUMERICSERV) && service)
        {
            libexplain_string_buffer_puts
            (
                sb,
                "service was not a numeric port-number string"
            );
            break;
        }
        if (!service)
        {
            libexplain_string_buffer_puts(sb, "the node is not known");
            break;
        }
        if (!node)
        {
            libexplain_string_buffer_puts(sb, "the service is not known");
            break;
        }
        libexplain_string_buffer_puts
        (
            sb,
            "the node or service is not known"
        );
        break;

    case EAI_SERVICE:
        libexplain_string_buffer_puts
        (
            sb,
            "the requested service is not available for the requested "
            "socket type, it may be available through another socket "
            "type"
        );
        break;

    case EAI_SOCKTYPE:
        libexplain_string_buffer_puts
        (
            sb,
            "the requested socket type is not supported"
        );
        break;

    case EAI_SYSTEM:
        libexplain_string_buffer_puts
        (
            sb,
            "a system error occurred, the error code can be found in errno"
        );
        break;

    case EAI_MEMORY:
        libexplain_buffer_enomem_user(sb);
        break;

    case EAI_OVERFLOW:
        libexplain_string_buffer_puts
        (
            sb,
            "an argument buffer overflowed"
        );
        break;

    default:
        break;
    }
}


void
libexplain_buffer_errcode_getaddrinfo(libexplain_string_buffer_t *sb,
    int errcode, const char *node, const char *service,
    const struct addrinfo *hints, struct addrinfo **res)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errcode);
    libexplain_buffer_errcode_getaddrinfo_system_call
    (
        &exp.system_call_sb,
        errcode,
        node,
        service,
        hints,
        res
    );
    libexplain_buffer_errcode_getaddrinfo_explanation
    (
        &exp.explanation_sb,
        errcode,
        node,
        service,
        hints,
        res
    );
    libexplain_explanation_assemble_gai(&exp, sb);
}
