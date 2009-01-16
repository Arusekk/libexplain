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

static const libexplain_iocontrol_t *const table[] =
{
    &libexplain_iocontrol_fioasync,
    &libexplain_iocontrol_fioclex,
    &libexplain_iocontrol_fiogetown,
    &libexplain_iocontrol_fionbio,
    &libexplain_iocontrol_fionclex,
    &libexplain_iocontrol_fionread,
    &libexplain_iocontrol_fioqsize,
    &libexplain_iocontrol_fiosetown,
    &libexplain_iocontrol_gio_cmap,
    &libexplain_iocontrol_gio_font,
    &libexplain_iocontrol_gio_fontx,
    &libexplain_iocontrol_gio_scrnmap,
    &libexplain_iocontrol_gio_unimap,
    &libexplain_iocontrol_gio_uniscrnmap,
    &libexplain_iocontrol_kdaddio,
    &libexplain_iocontrol_kddelio,
    &libexplain_iocontrol_kddisabio,
    &libexplain_iocontrol_kdenabio,
    &libexplain_iocontrol_kdfontop,
    &libexplain_iocontrol_kdgetkeycode,
    &libexplain_iocontrol_kdgetled,
    &libexplain_iocontrol_kdgetmode,
    &libexplain_iocontrol_kdgkbdiacr,
    &libexplain_iocontrol_kdgkbdiacruc,
    &libexplain_iocontrol_kdgkbent,
    &libexplain_iocontrol_kdgkbled,
    &libexplain_iocontrol_kdgkbmeta,
    &libexplain_iocontrol_kdgkbmode,
    &libexplain_iocontrol_kdgkbsent,
    &libexplain_iocontrol_kdgkbtype,
    &libexplain_iocontrol_kdkbdrep,
    &libexplain_iocontrol_kdmapdisp,
    &libexplain_iocontrol_kdmktone,
    &libexplain_iocontrol_kdsetkeycode,
    &libexplain_iocontrol_kdsetled,
    &libexplain_iocontrol_kdsetmode,
    &libexplain_iocontrol_kdsigaccept,
    &libexplain_iocontrol_kdskbdiacr,
    &libexplain_iocontrol_kdskbdiacruc,
    &libexplain_iocontrol_kdskbent,
    &libexplain_iocontrol_kdskbled,
    &libexplain_iocontrol_kdskbmeta,
    &libexplain_iocontrol_kdskbmode,
    &libexplain_iocontrol_kdskbsent,
    &libexplain_iocontrol_kdunmapdisp,
    &libexplain_iocontrol_kiocsound,
    &libexplain_iocontrol_mtiocget,
    &libexplain_iocontrol_mtiocgetconfig,
    &libexplain_iocontrol_mtiocpos,
    &libexplain_iocontrol_mtiocsetconfig,
    &libexplain_iocontrol_mtioctop,
    &libexplain_iocontrol_pio_cmap,
    &libexplain_iocontrol_pio_font,
    &libexplain_iocontrol_pio_fontreset,
    &libexplain_iocontrol_pio_fontx,
    &libexplain_iocontrol_pio_scrnmap,
    &libexplain_iocontrol_pio_unimap,
    &libexplain_iocontrol_pio_unimapclr,
    &libexplain_iocontrol_pio_uniscrnmap,
    &libexplain_iocontrol_siocadddlci,
    &libexplain_iocontrol_siocaddmulti,
    &libexplain_iocontrol_siocaddrt,
    &libexplain_iocontrol_siocatmark,
    &libexplain_iocontrol_siocbondchangeactive,
    &libexplain_iocontrol_siocbondenslave,
    &libexplain_iocontrol_siocbondinfoquery,
    &libexplain_iocontrol_siocbondrelease,
    &libexplain_iocontrol_siocbondsethwaddr,
    &libexplain_iocontrol_siocbondslaveinfoquery,
    &libexplain_iocontrol_siocbraddbr,
    &libexplain_iocontrol_siocbraddif,
    &libexplain_iocontrol_siocbrdelbr,
    &libexplain_iocontrol_siocbrdelif,
    &libexplain_iocontrol_siocdarp,
    &libexplain_iocontrol_siocdeldlci,
    &libexplain_iocontrol_siocdelmulti,
    &libexplain_iocontrol_siocdelrt,
    &libexplain_iocontrol_siocdifaddr,
    &libexplain_iocontrol_siocdrarp,
    &libexplain_iocontrol_siocethtool,
    &libexplain_iocontrol_siocgarp,
    &libexplain_iocontrol_siocgifaddr,
    &libexplain_iocontrol_siocgifbr,
    &libexplain_iocontrol_siocgifbrdaddr,
    &libexplain_iocontrol_siocgifconf,
    &libexplain_iocontrol_siocgifcount,
    &libexplain_iocontrol_siocgifdivert,
    &libexplain_iocontrol_siocgifdstaddr,
    &libexplain_iocontrol_siocgifencap,
    &libexplain_iocontrol_siocgifflags,
    &libexplain_iocontrol_siocgifhwaddr,
    &libexplain_iocontrol_siocgifindex,
    &libexplain_iocontrol_siocgifmap,
    &libexplain_iocontrol_siocgifmem,
    &libexplain_iocontrol_siocgifmetric,
    &libexplain_iocontrol_siocgifmtu,
    &libexplain_iocontrol_siocgifname,
    &libexplain_iocontrol_siocgifnetmask,
    &libexplain_iocontrol_siocgifpflags,
    &libexplain_iocontrol_siocgifslave,
    &libexplain_iocontrol_siocgiftxqlen,
    &libexplain_iocontrol_siocgifvlan,
    &libexplain_iocontrol_siocgmiiphy,
    &libexplain_iocontrol_siocgmiireg,
    &libexplain_iocontrol_siocgpgrp,
    &libexplain_iocontrol_siocgrarp,
    &libexplain_iocontrol_siocgstamp,
    &libexplain_iocontrol_siocgstampns,
    &libexplain_iocontrol_siocinq,
    &libexplain_iocontrol_siocoutq,
    &libexplain_iocontrol_siocrtmsg,
    &libexplain_iocontrol_siocsarp,
    &libexplain_iocontrol_siocsifaddr,
    &libexplain_iocontrol_siocsifbr,
    &libexplain_iocontrol_siocsifbrdaddr,
    &libexplain_iocontrol_siocsifdivert,
    &libexplain_iocontrol_siocsifdstaddr,
    &libexplain_iocontrol_siocsifencap,
    &libexplain_iocontrol_siocsifflags,
    &libexplain_iocontrol_siocsifhwaddr,
    &libexplain_iocontrol_siocsifhwbroadcast,
    &libexplain_iocontrol_siocsiflink,
    &libexplain_iocontrol_siocsifmap,
    &libexplain_iocontrol_siocsifmem,
    &libexplain_iocontrol_siocsifmetric,
    &libexplain_iocontrol_siocsifmtu,
    &libexplain_iocontrol_siocsifname,
    &libexplain_iocontrol_siocsifnetmask,
    &libexplain_iocontrol_siocsifpflags,
    &libexplain_iocontrol_siocsifslave,
    &libexplain_iocontrol_siocsiftxqlen,
    &libexplain_iocontrol_siocsifvlan,
    &libexplain_iocontrol_siocsmiireg,
    &libexplain_iocontrol_siocspgrp,
    &libexplain_iocontrol_siocsrarp,
    &libexplain_iocontrol_siocwandev,
    &libexplain_iocontrol_tcflsh,
    &libexplain_iocontrol_tcgeta,
    &libexplain_iocontrol_tcgets,
    &libexplain_iocontrol_tcsbrk,
    &libexplain_iocontrol_tcsbrkp,
    &libexplain_iocontrol_tcseta,
    &libexplain_iocontrol_tcsetaf,
    &libexplain_iocontrol_tcsetaw,
    &libexplain_iocontrol_tcsets,
    &libexplain_iocontrol_tcsetsf,
    &libexplain_iocontrol_tcsetsw,
    &libexplain_iocontrol_tcxonc,
    &libexplain_iocontrol_tioccbrk,
    &libexplain_iocontrol_tioccons,
    &libexplain_iocontrol_tiocexcl,
    &libexplain_iocontrol_tiocgetd,
    &libexplain_iocontrol_tiocgicount,
    &libexplain_iocontrol_tiocglcktrmios,
    &libexplain_iocontrol_tiocgpgrp,
    &libexplain_iocontrol_tiocgptn,
    &libexplain_iocontrol_tiocgserial,
    &libexplain_iocontrol_tiocgsid,
    &libexplain_iocontrol_tiocgwinsz,
    &libexplain_iocontrol_tiocinq,
    &libexplain_iocontrol_tioclinux,
    &libexplain_iocontrol_tiocmbic,
    &libexplain_iocontrol_tiocmbis,
    &libexplain_iocontrol_tiocmget,
    &libexplain_iocontrol_tiocmiwait,
    &libexplain_iocontrol_tiocmset,
    &libexplain_iocontrol_tiocnotty,
    &libexplain_iocontrol_tiocnxcl,
    &libexplain_iocontrol_tiocoutq,
    &libexplain_iocontrol_tiocsbrk,
    &libexplain_iocontrol_tiocsctty,
    &libexplain_iocontrol_tiocserconfig,
    &libexplain_iocontrol_tiocsergetlsr,
    &libexplain_iocontrol_tiocsergetmulti,
    &libexplain_iocontrol_tiocsergstruct,
    &libexplain_iocontrol_tiocsergwild,
    &libexplain_iocontrol_tiocsersetmulti,
    &libexplain_iocontrol_tiocserswild,
    &libexplain_iocontrol_tiocsetd,
    &libexplain_iocontrol_tiocshayesesp,
    &libexplain_iocontrol_tiocslcktrmios,
    &libexplain_iocontrol_tiocspgrp,
    &libexplain_iocontrol_tiocsptlck,
    &libexplain_iocontrol_tiocsserial,
    &libexplain_iocontrol_tiocsti,
    &libexplain_iocontrol_tiocswinsz,
    &libexplain_iocontrol_tiocttygstruct,
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

const libexplain_iocontrol_t *
libexplain_iocontrol_find_by_number(int fildes, int request,
    const void *data)
{
    const libexplain_iocontrol_t *const *tpp;

    for (tpp = table; tpp < ENDOF(table); ++tpp)
    {
        const libexplain_iocontrol_t *tp;

        tp = *tpp;
        if
        (
            request == tp->number
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
    return &libexplain_iocontrol_generic;
}


int
libexplain_parse_ioctl_request_or_die(const char *text)
{
    libexplain_parse_bits_table_t *tokens;
    size_t          size;
    int             result;

    /*
     * We must build a parse_bits table to pass to the parser.
     */
    tokens = malloc(sizeof(tokens[0]) * (SIZEOF(table) + 2));
    size = 0;
    if (tokens)
    {
        libexplain_parse_bits_table_t *p;
        const libexplain_iocontrol_t *const *tpp;

        p = tokens;
        for (tpp = table; tpp < ENDOF(table); ++tpp)
        {
            const libexplain_iocontrol_t *tp;

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
    result = libexplain_parse_bits_or_die(text, tokens, size, "ioctl request");
    if (tokens)
        free(tokens);
    return result;
}
