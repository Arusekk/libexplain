/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/ac/sys/ioctl.h>
#include <libexplain/ac/termios.h>

#include <libexplain/buffer/termios.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>


static const libexplain_parse_bits_table_t c_iflag_table[] =
{
    { "IGNBRK", IGNBRK },
    { "BRKINT", BRKINT },
    { "IGNPAR", IGNPAR },
    { "PARMRK", PARMRK },
    { "INPCK", INPCK },
    { "ISTRIP", ISTRIP },
    { "INLCR", INLCR },
    { "IGNCR", IGNCR },
    { "ICRNL", ICRNL },
    { "IUCLC", IUCLC },
    { "IXON", IXON },
    { "IXANY", IXANY },
    { "IXOFF", IXOFF },
    { "IMAXBEL", IMAXBEL },
    { "IUTF8", IUTF8 },
};

#define simple(name)                                       \
    do {                                                   \
        if (flag & name)                                   \
        {                                                  \
            if (!first)                                    \
                libexplain_string_buffer_puts(sb, " | ");  \
            libexplain_string_buffer_puts(sb, #name);      \
            flag &= ~name;                                \
            first = 0;                                     \
        }                                                  \
    } while (0)

#define simple_case(name)                                  \
    do {                                                   \
        if (!first)                                        \
            libexplain_string_buffer_puts(sb, " | ");      \
        libexplain_string_buffer_puts(sb, #name);          \
        first = 0;                                         \
    } while (0)


static void
libexplain_buffer_termios_oflag(libexplain_string_buffer_t *sb, int flag)
{
    int             first;

    first = 1;
    simple(OPOST);
    simple(OLCUC);
    simple(ONLCR);
    simple(OCRNL);
    simple(ONOCR);
    simple(ONLRET);
    simple(OFILL);
    simple(OFDEL);
#ifdef NLDLY
    simple(NL1);
#endif

#ifdef CRDLY
    switch (flag & CRDLY)
    {
    case CR1: simple_case(CR1); break;
    case CR2: simple_case(CR2); break;
    case CR3: simple_case(CR3); break;
    default: break;
    }
    flag &= ~CRDLY;
#endif

#ifdef TABDLY
    switch (flag & TABDLY)
    {
    case TAB1: simple_case(TAB1); break;
    case TAB2: simple_case(TAB2); break;
    case TAB3: simple_case(TAB3); break;
    default: break;
    }
    flag &= ~TABDLY;
#endif

#ifdef BS1
    simple(BS1);
#endif
#ifdef FF1
    simple(FF1);
#endif
#ifdef VT1
    simple(VT1);
#endif

    if (flag)
    {
        if (!first)
            libexplain_string_buffer_puts(sb, " | ");
        libexplain_string_buffer_printf(sb, "%#x", flag);
    }
};


static void
libexplain_buffer_termios_cflag(libexplain_string_buffer_t *sb, int flag)
{
    int             first;

    first = 1;
    switch (flag & CBAUD)
    {
    /* case B0: simple_case(B0); break; */
    case B50: simple_case(B50); break;
    case B75: simple_case(B75); break;
    case B110: simple_case(B110); break;
    case B134: simple_case(B134); break;
    case B150: simple_case(B150); break;
    case B200: simple_case(B200); break;
    case B300: simple_case(B300); break;
    case B600: simple_case(B600); break;
    case B1200: simple_case(B1200); break;
    case B1800: simple_case(B1800); break;
    case B2400: simple_case(B2400); break;
    case B4800: simple_case(B4800); break;
    case B9600: simple_case(B9600); break;
    case B19200: simple_case(B19200); break;
    case B38400: simple_case(B38400); break;
    case B57600: simple_case(B57600); break;
    case B115200: simple_case(B115200); break;
    case B230400: simple_case(B230400); break;
    case B460800: simple_case(B460800); break;
    case B500000: simple_case(B500000); break;
    case B576000: simple_case(B576000); break;
    case B921600: simple_case(B921600); break;
    case B1000000: simple_case(B1000000); break;
    case B1152000: simple_case(B1152000); break;
    case B1500000: simple_case(B1500000); break;
    case B2000000: simple_case(B2000000); break;
    case B2500000: simple_case(B2500000); break;
    case B3000000: simple_case(B3000000); break;
    case B3500000: simple_case(B3500000); break;
    case B4000000: simple_case(B4000000); break;
    default: break;
    }
    flag &= ~CBAUD;

    switch (flag & CSIZE)
    {
    case CS5: simple_case(CS5); break;
    case CS6: simple_case(CS6); break;
    case CS7: simple_case(CS7); break;
    case CS8: simple_case(CS8); break;
    default: break;
    }
    flag &= ~CSIZE;

    simple(CSTOPB);
    simple(CREAD);
    simple(PARENB);
    simple(PARODD);
    simple(HUPCL);
    simple(CLOCAL);

#ifdef CMSPAR
    simple(CMSPAR);
#endif
#ifdef CRTSCTS
    simple(CRTSCTS);
#endif

    if (flag)
    {
        if (!first)
            libexplain_string_buffer_puts(sb, " | ");
        libexplain_string_buffer_printf(sb, "%#x", flag);
    }
};


static const libexplain_parse_bits_table_t c_lflag_table[] =
{
    { "ISIG", ISIG },
    { "ICANON", ICANON },
    { "XCASE", XCASE },
    { "ECHO", ECHO },
    { "ECHOE", ECHOE },
    { "ECHOK", ECHOK },
    { "ECHONL", ECHONL },
    { "NOFLSH", NOFLSH },
    { "TOSTOP", TOSTOP },
    { "ECHOCTL", ECHOCTL },
    { "ECHOPRT", ECHOPRT },
    { "ECHOKE", ECHOKE },
    { "FLUSHO", FLUSHO },
    { "PENDIN", PENDIN },
    { "IEXTEN", IEXTEN },
};


static const libexplain_parse_bits_table_t cc_table[] =
{
    { "VINTR", VINTR },
    { "VQUIT", VQUIT },
    { "VERASE", VERASE },
    { "VKILL", VKILL },
    { "VEOF", VEOF },
    { "VTIME", VTIME },
    { "VMIN", VMIN },
    { "VSWTC", VSWTC },
    { "VSTART", VSTART },
    { "VSTOP", VSTOP },
    { "VSUSP", VSUSP },
    { "VEOL", VEOL },
    { "VREPRINT", VREPRINT },
    { "VDISCARD", VDISCARD },
    { "VWERASE", VWERASE },
    { "VLNEXT", VLNEXT },
    { "VEOL2", VEOL2 },
};


static const libexplain_parse_bits_table_t line_discipline_table[] =
{
    { "N_TTY", N_TTY },
    { "N_SLIP", N_SLIP },
    { "N_MOUSE", N_MOUSE },
    { "N_PPP", N_PPP },
    { "N_STRIP", N_STRIP },
    { "N_AX25", N_AX25 },
    { "N_X25", N_X25 },
    { "N_6PACK", N_6PACK },
    { "N_MASC", N_MASC },
    { "N_R3964", N_R3964 },
    { "N_PROFIBUS_FDL", N_PROFIBUS_FDL },
    { "N_IRDA", N_IRDA },
    { "N_SMSBLOCK", N_SMSBLOCK },
    { "N_HDLC", N_HDLC },
    { "N_SYNC_PPP", N_SYNC_PPP },
    { "N_HCI", N_HCI },
};


void
libexplain_buffer_termios(libexplain_string_buffer_t *sb,
    const struct termios *value)
{
    int             j;

    libexplain_string_buffer_puts(sb, "{ c_iflag = ");
    libexplain_parse_bits_print
    (
        sb,
        value->c_iflag,
        c_iflag_table,
        SIZEOF(c_iflag_table)
    );
    libexplain_string_buffer_puts(sb, ", c_oflag = ");
    libexplain_buffer_termios_oflag(sb, value->c_oflag);
    libexplain_string_buffer_puts(sb, ", c_cflag = ");
    libexplain_buffer_termios_cflag(sb, value->c_oflag);
    libexplain_string_buffer_puts(sb, ", c_lflag = ");
    libexplain_parse_bits_print
    (
        sb,
        value->c_lflag,
        c_lflag_table,
        SIZEOF(c_lflag_table)
    );
    libexplain_string_buffer_puts(sb, ", c_line = ");
    libexplain_parse_bits_print
    (
        sb,
        value->c_lflag,
        line_discipline_table,
        SIZEOF(line_discipline_table)
    );
    libexplain_string_buffer_puts(sb, ", c_cc[] = {");
    for (j = 0; j < NCCS; ++j)
    {
        const libexplain_parse_bits_table_t *tp;

        if (j)
            libexplain_string_buffer_putc(sb, ',');
        libexplain_string_buffer_putc(sb, ' ');
        tp = libexplain_parse_bits_find_by_value(j, cc_table, SIZEOF(cc_table));
        if (tp)
            libexplain_string_buffer_printf(sb, "%s = ", tp->name);
        libexplain_string_buffer_putc_quoted
        (
            sb,
            (unsigned char)value->c_cc[j]
        );
    }
    libexplain_string_buffer_puts(sb, " } }");
}


void
libexplain_buffer_termio(libexplain_string_buffer_t *sb,
    const struct termio *value)
{
    int             j;

    libexplain_string_buffer_puts(sb, "{ c_iflag = ");
    libexplain_parse_bits_print
    (
        sb,
        value->c_iflag,
        c_iflag_table,
        SIZEOF(c_iflag_table)
    );
    libexplain_string_buffer_puts(sb, ", c_oflag = ");
    libexplain_buffer_termios_oflag(sb, value->c_oflag);
    libexplain_string_buffer_puts(sb, ", c_cflag = ");
    libexplain_buffer_termios_cflag(sb, value->c_oflag);
    libexplain_string_buffer_puts(sb, ", c_lflag = ");
    libexplain_parse_bits_print
    (
        sb,
        value->c_lflag,
        c_lflag_table,
        SIZEOF(c_lflag_table)
    );
    libexplain_string_buffer_puts(sb, ", c_line = ");
    libexplain_parse_bits_print
    (
        sb,
        value->c_lflag,
        line_discipline_table,
        SIZEOF(line_discipline_table)
    );
    libexplain_string_buffer_puts(sb, ", c_cc[] = {");
    for (j = 0; j < NCC; ++j)
    {
        const libexplain_parse_bits_table_t *tp;

        if (j)
            libexplain_string_buffer_putc(sb, ',');
        libexplain_string_buffer_putc(sb, ' ');
        tp = libexplain_parse_bits_find_by_value(j, cc_table, SIZEOF(cc_table));
        if (tp)
            libexplain_string_buffer_printf(sb, "%s = ", tp->name);
        libexplain_string_buffer_putc_quoted
        (
            sb,
            (unsigned char)value->c_cc[j]
        );
    }
    libexplain_string_buffer_puts(sb, " } }");
}
