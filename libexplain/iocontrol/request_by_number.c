/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/iocontrol.h>
#include <libexplain/iocontrol/fioasync.h>
#include <libexplain/iocontrol/fioclex.h>
#include <libexplain/iocontrol/fiogetown.h>
#include <libexplain/iocontrol/fionbio.h>
#include <libexplain/iocontrol/fionclex.h>
#include <libexplain/iocontrol/fionread.h>
#include <libexplain/iocontrol/fioqsize.h>
#include <libexplain/iocontrol/fiosetown.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/gio_cmap.h>
#include <libexplain/iocontrol/gio_font.h>
#include <libexplain/iocontrol/gio_fontx.h>
#include <libexplain/iocontrol/gio_scrnmap.h>
#include <libexplain/iocontrol/gio_unimap.h>
#include <libexplain/iocontrol/gio_uniscrnmap.h>
#include <libexplain/iocontrol/kdaddio.h>
#include <libexplain/iocontrol/kddelio.h>
#include <libexplain/iocontrol/kddisabio.h>
#include <libexplain/iocontrol/kdenabio.h>
#include <libexplain/iocontrol/kdfontop.h>
#include <libexplain/iocontrol/kdgetkeycode.h>
#include <libexplain/iocontrol/kdgetled.h>
#include <libexplain/iocontrol/kdgetmode.h>
#include <libexplain/iocontrol/kdgkbdiacr.h>
#include <libexplain/iocontrol/kdgkbdiacruc.h>
#include <libexplain/iocontrol/kdgkbent.h>
#include <libexplain/iocontrol/kdgkbled.h>
#include <libexplain/iocontrol/kdgkbmeta.h>
#include <libexplain/iocontrol/kdgkbmode.h>
#include <libexplain/iocontrol/kdgkbsent.h>
#include <libexplain/iocontrol/kdgkbtype.h>
#include <libexplain/iocontrol/kdkbdrep.h>
#include <libexplain/iocontrol/kdmapdisp.h>
#include <libexplain/iocontrol/kdmktone.h>
#include <libexplain/iocontrol/kdsetkeycode.h>
#include <libexplain/iocontrol/kdsetled.h>
#include <libexplain/iocontrol/kdsetmode.h>
#include <libexplain/iocontrol/kdsigaccept.h>
#include <libexplain/iocontrol/kdskbdiacr.h>
#include <libexplain/iocontrol/kdskbdiacruc.h>
#include <libexplain/iocontrol/kdskbent.h>
#include <libexplain/iocontrol/kdskbled.h>
#include <libexplain/iocontrol/kdskbmeta.h>
#include <libexplain/iocontrol/kdskbmode.h>
#include <libexplain/iocontrol/kdskbsent.h>
#include <libexplain/iocontrol/kdunmapdisp.h>
#include <libexplain/iocontrol/kiocsound.h>
#include <libexplain/iocontrol/mtiocget.h>
#include <libexplain/iocontrol/mtiocgetconfig.h>
#include <libexplain/iocontrol/mtiocpos.h>
#include <libexplain/iocontrol/mtiocsetconfig.h>
#include <libexplain/iocontrol/mtioctop.h>
#include <libexplain/iocontrol/pio_cmap.h>
#include <libexplain/iocontrol/pio_font.h>
#include <libexplain/iocontrol/pio_fontreset.h>
#include <libexplain/iocontrol/pio_fontx.h>
#include <libexplain/iocontrol/pio_scrnmap.h>
#include <libexplain/iocontrol/pio_unimap.h>
#include <libexplain/iocontrol/pio_unimapclr.h>
#include <libexplain/iocontrol/pio_uniscrnmap.h>
#include <libexplain/iocontrol/siocadddlci.h>
#include <libexplain/iocontrol/siocaddmulti.h>
#include <libexplain/iocontrol/siocaddrt.h>
#include <libexplain/iocontrol/siocatmark.h>
#include <libexplain/iocontrol/siocbondchangeactive.h>
#include <libexplain/iocontrol/siocbondenslave.h>
#include <libexplain/iocontrol/siocbondinfoquery.h>
#include <libexplain/iocontrol/siocbondrelease.h>
#include <libexplain/iocontrol/siocbondsethwaddr.h>
#include <libexplain/iocontrol/siocbondslaveinfoquery.h>
#include <libexplain/iocontrol/siocbraddbr.h>
#include <libexplain/iocontrol/siocbraddif.h>
#include <libexplain/iocontrol/siocbrdelbr.h>
#include <libexplain/iocontrol/siocbrdelif.h>
#include <libexplain/iocontrol/siocdarp.h>
#include <libexplain/iocontrol/siocdeldlci.h>
#include <libexplain/iocontrol/siocdelmulti.h>
#include <libexplain/iocontrol/siocdelrt.h>
#include <libexplain/iocontrol/siocdifaddr.h>
#include <libexplain/iocontrol/siocdrarp.h>
#include <libexplain/iocontrol/siocethtool.h>
#include <libexplain/iocontrol/siocgarp.h>
#include <libexplain/iocontrol/siocgifaddr.h>
#include <libexplain/iocontrol/siocgifbr.h>
#include <libexplain/iocontrol/siocgifbrdaddr.h>
#include <libexplain/iocontrol/siocgifconf.h>
#include <libexplain/iocontrol/siocgifcount.h>
#include <libexplain/iocontrol/siocgifdivert.h>
#include <libexplain/iocontrol/siocgifdstaddr.h>
#include <libexplain/iocontrol/siocgifencap.h>
#include <libexplain/iocontrol/siocgifflags.h>
#include <libexplain/iocontrol/siocgifhwaddr.h>
#include <libexplain/iocontrol/siocgifindex.h>
#include <libexplain/iocontrol/siocgifmap.h>
#include <libexplain/iocontrol/siocgifmem.h>
#include <libexplain/iocontrol/siocgifmetric.h>
#include <libexplain/iocontrol/siocgifmtu.h>
#include <libexplain/iocontrol/siocgifname.h>
#include <libexplain/iocontrol/siocgifnetmask.h>
#include <libexplain/iocontrol/siocgifpflags.h>
#include <libexplain/iocontrol/siocgifslave.h>
#include <libexplain/iocontrol/siocgiftxqlen.h>
#include <libexplain/iocontrol/siocgifvlan.h>
#include <libexplain/iocontrol/siocgmiiphy.h>
#include <libexplain/iocontrol/siocgmiireg.h>
#include <libexplain/iocontrol/siocgpgrp.h>
#include <libexplain/iocontrol/siocgrarp.h>
#include <libexplain/iocontrol/siocgstamp.h>
#include <libexplain/iocontrol/siocgstampns.h>
#include <libexplain/iocontrol/siocinq.h>
#include <libexplain/iocontrol/siocoutq.h>
#include <libexplain/iocontrol/siocrtmsg.h>
#include <libexplain/iocontrol/siocsarp.h>
#include <libexplain/iocontrol/siocsifaddr.h>
#include <libexplain/iocontrol/siocsifbr.h>
#include <libexplain/iocontrol/siocsifbrdaddr.h>
#include <libexplain/iocontrol/siocsifdivert.h>
#include <libexplain/iocontrol/siocsifdstaddr.h>
#include <libexplain/iocontrol/siocsifencap.h>
#include <libexplain/iocontrol/siocsifflags.h>
#include <libexplain/iocontrol/siocsifhwaddr.h>
#include <libexplain/iocontrol/siocsifhwbroadcast.h>
#include <libexplain/iocontrol/siocsiflink.h>
#include <libexplain/iocontrol/siocsifmap.h>
#include <libexplain/iocontrol/siocsifmem.h>
#include <libexplain/iocontrol/siocsifmetric.h>
#include <libexplain/iocontrol/siocsifmtu.h>
#include <libexplain/iocontrol/siocsifname.h>
#include <libexplain/iocontrol/siocsifnetmask.h>
#include <libexplain/iocontrol/siocsifpflags.h>
#include <libexplain/iocontrol/siocsifslave.h>
#include <libexplain/iocontrol/siocsiftxqlen.h>
#include <libexplain/iocontrol/siocsifvlan.h>
#include <libexplain/iocontrol/siocsmiireg.h>
#include <libexplain/iocontrol/siocspgrp.h>
#include <libexplain/iocontrol/siocsrarp.h>
#include <libexplain/iocontrol/siocwandev.h>
#include <libexplain/iocontrol/tcflsh.h>
#include <libexplain/iocontrol/tcgeta.h>
#include <libexplain/iocontrol/tcgets.h>
#include <libexplain/iocontrol/tcsbrk.h>
#include <libexplain/iocontrol/tcsbrkp.h>
#include <libexplain/iocontrol/tcseta.h>
#include <libexplain/iocontrol/tcsets.h>
#include <libexplain/iocontrol/tcxonc.h>
#include <libexplain/iocontrol/tioccbrk.h>
#include <libexplain/iocontrol/tioccons.h>
#include <libexplain/iocontrol/tiocexcl.h>
#include <libexplain/iocontrol/tiocgetd.h>
#include <libexplain/iocontrol/tiocgicount.h>
#include <libexplain/iocontrol/tiocglcktrmios.h>
#include <libexplain/iocontrol/tiocgpgrp.h>
#include <libexplain/iocontrol/tiocgptn.h>
#include <libexplain/iocontrol/tiocgserial.h>
#include <libexplain/iocontrol/tiocgsid.h>
#include <libexplain/iocontrol/tiocgsoftcar.h>
#include <libexplain/iocontrol/tiocgwinsz.h>
#include <libexplain/iocontrol/tiocinq.h>
#include <libexplain/iocontrol/tioclinux.h>
#include <libexplain/iocontrol/tiocmbic.h>
#include <libexplain/iocontrol/tiocmbis.h>
#include <libexplain/iocontrol/tiocmget.h>
#include <libexplain/iocontrol/tiocmiwait.h>
#include <libexplain/iocontrol/tiocmset.h>
#include <libexplain/iocontrol/tiocnotty.h>
#include <libexplain/iocontrol/tiocnxcl.h>
#include <libexplain/iocontrol/tiocoutq.h>
#include <libexplain/iocontrol/tiocsbrk.h>
#include <libexplain/iocontrol/tiocsctty.h>
#include <libexplain/iocontrol/tiocserconfig.h>
#include <libexplain/iocontrol/tiocsergetlsr.h>
#include <libexplain/iocontrol/tiocsergetmulti.h>
#include <libexplain/iocontrol/tiocsergstruct.h>
#include <libexplain/iocontrol/tiocsergwild.h>
#include <libexplain/iocontrol/tiocsersetmulti.h>
#include <libexplain/iocontrol/tiocserswild.h>
#include <libexplain/iocontrol/tiocsetd.h>
#include <libexplain/iocontrol/tiocshayesesp.h>
#include <libexplain/iocontrol/tiocslcktrmios.h>
#include <libexplain/iocontrol/tiocspgrp.h>
#include <libexplain/iocontrol/tiocsptlck.h>
#include <libexplain/iocontrol/tiocsserial.h>
#include <libexplain/iocontrol/tiocssoftcar.h>
#include <libexplain/iocontrol/tiocsti.h>
#include <libexplain/iocontrol/tiocswinsz.h>
#include <libexplain/iocontrol/tiocttygstruct.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>


/*
 * The ugly thing about ioctl(2) is that, in effect, each ioctl request is a
 * separate and unique system call.
 *
 * This information is not kept in a single table for all values, like every
 * other set of constants, because (a) some values are ambiguous, and (b) the
 * includes files have bugs making it impossible to include all of them in the
 * same combilation unit.
 *
 * By just storing pointers to our own data structure, there is no need to
 * include them all at once.
 *
 * Keep this array sorted alphabetically.
 */

static const explain_iocontrol_t *const table[] =
{
    &explain_iocontrol_fioasync,
    &explain_iocontrol_fioclex,
    &explain_iocontrol_fiogetown,
    &explain_iocontrol_fionbio,
    &explain_iocontrol_fionclex,
    &explain_iocontrol_fionread,
    &explain_iocontrol_fioqsize,
    &explain_iocontrol_fiosetown,
    &explain_iocontrol_gio_cmap,
    &explain_iocontrol_gio_font,
    &explain_iocontrol_gio_fontx,
    &explain_iocontrol_gio_scrnmap,
    &explain_iocontrol_gio_unimap,
    &explain_iocontrol_gio_uniscrnmap,
    &explain_iocontrol_kdaddio,
    &explain_iocontrol_kddelio,
    &explain_iocontrol_kddisabio,
    &explain_iocontrol_kdenabio,
    &explain_iocontrol_kdfontop,
    &explain_iocontrol_kdgetkeycode,
    &explain_iocontrol_kdgetled,
    &explain_iocontrol_kdgetmode,
    &explain_iocontrol_kdgkbdiacr,
    &explain_iocontrol_kdgkbdiacruc,
    &explain_iocontrol_kdgkbent,
    &explain_iocontrol_kdgkbled,
    &explain_iocontrol_kdgkbmeta,
    &explain_iocontrol_kdgkbmode,
    &explain_iocontrol_kdgkbsent,
    &explain_iocontrol_kdgkbtype,
    &explain_iocontrol_kdkbdrep,
    &explain_iocontrol_kdmapdisp,
    &explain_iocontrol_kdmktone,
    &explain_iocontrol_kdsetkeycode,
    &explain_iocontrol_kdsetled,
    &explain_iocontrol_kdsetmode,
    &explain_iocontrol_kdsigaccept,
    &explain_iocontrol_kdskbdiacr,
    &explain_iocontrol_kdskbdiacruc,
    &explain_iocontrol_kdskbent,
    &explain_iocontrol_kdskbled,
    &explain_iocontrol_kdskbmeta,
    &explain_iocontrol_kdskbmode,
    &explain_iocontrol_kdskbsent,
    &explain_iocontrol_kdunmapdisp,
    &explain_iocontrol_kiocsound,
    &explain_iocontrol_mtiocget,
    &explain_iocontrol_mtiocgetconfig,
    &explain_iocontrol_mtiocpos,
    &explain_iocontrol_mtiocsetconfig,
    &explain_iocontrol_mtioctop,
    &explain_iocontrol_pio_cmap,
    &explain_iocontrol_pio_font,
    &explain_iocontrol_pio_fontreset,
    &explain_iocontrol_pio_fontx,
    &explain_iocontrol_pio_scrnmap,
    &explain_iocontrol_pio_unimap,
    &explain_iocontrol_pio_unimapclr,
    &explain_iocontrol_pio_uniscrnmap,
    &explain_iocontrol_siocadddlci,
    &explain_iocontrol_siocaddmulti,
    &explain_iocontrol_siocaddrt,
    &explain_iocontrol_siocatmark,
    &explain_iocontrol_siocbondchangeactive,
    &explain_iocontrol_siocbondenslave,
    &explain_iocontrol_siocbondinfoquery,
    &explain_iocontrol_siocbondrelease,
    &explain_iocontrol_siocbondsethwaddr,
    &explain_iocontrol_siocbondslaveinfoquery,
    &explain_iocontrol_siocbraddbr,
    &explain_iocontrol_siocbraddif,
    &explain_iocontrol_siocbrdelbr,
    &explain_iocontrol_siocbrdelif,
    &explain_iocontrol_siocdarp,
    &explain_iocontrol_siocdeldlci,
    &explain_iocontrol_siocdelmulti,
    &explain_iocontrol_siocdelrt,
    &explain_iocontrol_siocdifaddr,
    &explain_iocontrol_siocdrarp,
    &explain_iocontrol_siocethtool,
    &explain_iocontrol_siocgarp,
    &explain_iocontrol_siocgifaddr,
    &explain_iocontrol_siocgifbr,
    &explain_iocontrol_siocgifbrdaddr,
    &explain_iocontrol_siocgifconf,
    &explain_iocontrol_siocgifcount,
    &explain_iocontrol_siocgifdivert,
    &explain_iocontrol_siocgifdstaddr,
    &explain_iocontrol_siocgifencap,
    &explain_iocontrol_siocgifflags,
    &explain_iocontrol_siocgifhwaddr,
    &explain_iocontrol_siocgifindex,
    &explain_iocontrol_siocgifmap,
    &explain_iocontrol_siocgifmem,
    &explain_iocontrol_siocgifmetric,
    &explain_iocontrol_siocgifmtu,
    &explain_iocontrol_siocgifname,
    &explain_iocontrol_siocgifnetmask,
    &explain_iocontrol_siocgifpflags,
    &explain_iocontrol_siocgifslave,
    &explain_iocontrol_siocgiftxqlen,
    &explain_iocontrol_siocgifvlan,
    &explain_iocontrol_siocgmiiphy,
    &explain_iocontrol_siocgmiireg,
    &explain_iocontrol_siocgpgrp,
    &explain_iocontrol_siocgrarp,
    &explain_iocontrol_siocgstamp,
    &explain_iocontrol_siocgstampns,
    &explain_iocontrol_siocinq,
    &explain_iocontrol_siocoutq,
    &explain_iocontrol_siocrtmsg,
    &explain_iocontrol_siocsarp,
    &explain_iocontrol_siocsifaddr,
    &explain_iocontrol_siocsifbr,
    &explain_iocontrol_siocsifbrdaddr,
    &explain_iocontrol_siocsifdivert,
    &explain_iocontrol_siocsifdstaddr,
    &explain_iocontrol_siocsifencap,
    &explain_iocontrol_siocsifflags,
    &explain_iocontrol_siocsifhwaddr,
    &explain_iocontrol_siocsifhwbroadcast,
    &explain_iocontrol_siocsiflink,
    &explain_iocontrol_siocsifmap,
    &explain_iocontrol_siocsifmem,
    &explain_iocontrol_siocsifmetric,
    &explain_iocontrol_siocsifmtu,
    &explain_iocontrol_siocsifname,
    &explain_iocontrol_siocsifnetmask,
    &explain_iocontrol_siocsifpflags,
    &explain_iocontrol_siocsifslave,
    &explain_iocontrol_siocsiftxqlen,
    &explain_iocontrol_siocsifvlan,
    &explain_iocontrol_siocsmiireg,
    &explain_iocontrol_siocspgrp,
    &explain_iocontrol_siocsrarp,
    &explain_iocontrol_siocwandev,
    &explain_iocontrol_tcflsh,
    &explain_iocontrol_tcgeta,
    &explain_iocontrol_tcgets,
    &explain_iocontrol_tcsbrk,
    &explain_iocontrol_tcsbrkp,
    &explain_iocontrol_tcseta,
    &explain_iocontrol_tcsetaf,
    &explain_iocontrol_tcsetaw,
    &explain_iocontrol_tcsets,
    &explain_iocontrol_tcsetsf,
    &explain_iocontrol_tcsetsw,
    &explain_iocontrol_tcxonc,
    &explain_iocontrol_tioccbrk,
    &explain_iocontrol_tioccons,
    &explain_iocontrol_tiocexcl,
    &explain_iocontrol_tiocgetd,
    &explain_iocontrol_tiocgicount,
    &explain_iocontrol_tiocglcktrmios,
    &explain_iocontrol_tiocgpgrp,
    &explain_iocontrol_tiocgptn,
    &explain_iocontrol_tiocgserial,
    &explain_iocontrol_tiocgsid,
    &explain_iocontrol_tiocgwinsz,
    &explain_iocontrol_tiocinq,
    &explain_iocontrol_tioclinux,
    &explain_iocontrol_tiocmbic,
    &explain_iocontrol_tiocmbis,
    &explain_iocontrol_tiocmget,
    &explain_iocontrol_tiocmiwait,
    &explain_iocontrol_tiocmset,
    &explain_iocontrol_tiocnotty,
    &explain_iocontrol_tiocnxcl,
    &explain_iocontrol_tiocoutq,
    &explain_iocontrol_tiocsbrk,
    &explain_iocontrol_tiocsctty,
    &explain_iocontrol_tiocserconfig,
    &explain_iocontrol_tiocsergetlsr,
    &explain_iocontrol_tiocsergetmulti,
    &explain_iocontrol_tiocsergstruct,
    &explain_iocontrol_tiocsergwild,
    &explain_iocontrol_tiocsersetmulti,
    &explain_iocontrol_tiocserswild,
    &explain_iocontrol_tiocsetd,
    &explain_iocontrol_tiocshayesesp,
    &explain_iocontrol_tiocslcktrmios,
    &explain_iocontrol_tiocspgrp,
    &explain_iocontrol_tiocsptlck,
    &explain_iocontrol_tiocsserial,
    &explain_iocontrol_tiocsti,
    &explain_iocontrol_tiocswinsz,
    &explain_iocontrol_tiocttygstruct,
};

/*
 * The following AWK script may be used to find ioctl requests.
 *
 * /#define.*\<(_IO|_IOC|_IOR|_IOW|_IORW)\(/ {
 *    if (fn != FILENAME)
 *    {
 *        fn = FILENAME;
 *        ifn = fn
 *        sub(/^.usr.include./, "", ifn)
 *        printf("\n    /" "* #include <%s> *" "/\n", ifn);
 *    }
 *    line = $0
 *    sub(/[ \t]*#define./, "", line);
 *    sub(/[ \t].*./, "", line);
 *    printf("#ifdef %s\n", line);
 *    printf("    { \"%s\", %s },\n", line, line);
 *    printf("#endif\n");
 * }
 */

const explain_iocontrol_t *
explain_iocontrol_find_by_number(int fildes, int request,
    const void *data)
{
    const explain_iocontrol_t *const *tpp;

    for (tpp = table; tpp < ENDOF(table); ++tpp)
    {
        const explain_iocontrol_t *tp;

        tp = *tpp;
        if
        (
            request == tp->number
        &&
            (tp->print_name || tp->name)
        &&
            (
                !tp->disambiguate
            ||
                0 == tp->disambiguate(fildes, request, data)
            )
        )
        {
            return tp;
        }
    }
    return &explain_iocontrol_generic;
}


int
explain_parse_ioctl_request_or_die(const char *text)
{
    explain_parse_bits_table_t *tokens;
    size_t          size;
    int             result;

    /*
     * We must build a parse_bits table to pass to the parser.
     */
    tokens = malloc(sizeof(tokens[0]) * (SIZEOF(table) + 2));
    size = 0;
    if (tokens)
    {
        explain_parse_bits_table_t *p;
        const explain_iocontrol_t *const *tpp;

        p = tokens;
        for (tpp = table; tpp < ENDOF(table); ++tpp)
        {
            const explain_iocontrol_t *tp;

            tp = *tpp;
            if (tp->name && tp->number != -1)
            {
                p->name = tp->name;
                p->value = tp->number;
                ++p;
            }
        }

        p->name = "SIOCDEVPRIVATE";
        p->value = SIOCDEVPRIVATE;
        ++p;

        p->name = "SIOCPROTOPRIVATE";
        p->value = SIOCPROTOPRIVATE;
        ++p;

        size = p - tokens;
    }
    result = explain_parse_bits_or_die(text, tokens, size, "ioctl request");
    if (tokens)
        free(tokens);
    return result;
}
