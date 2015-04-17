#define BASEVERSION "050"

/*
 *  l9data_d.h
 *
 *  - Level 9 version database for l9cut.c
 *
 *  Author: Paul David Doherty <42@pdd.de>
 *
 *  001:   5 Aug 1996                                               (1.1)
 *  002:  13 Apr 1997  5 new versions  (Donald Kruse)               (1.4)
 *  003:  16 Apr 1997  1 new version   (Toine de Greef)             (1.5)
 *  004:  24 Apr 1997  1 new version   (Speccy CD 96)               (1.6)
 *  005:  26 Apr 1997  1 new version   (Donald Kruse)               (1.7)
 *  006:  28 Jun 1997  3 new versions  (DoomSooth)                  (1.8)
 *  007:   5 Jul 1997  4 new versions  (Manuel Schulz)              (1.9)
 *  008:   6 Sep 1997  3 new versions  (Stuart "Dark Fiber" George) (1.10)
 *  009:   9 Sep 1997 11 new versions  (PDD), CRC16 checksums
 *  010:  12 Sep 1997 15 new versions  (PDD)
 *  011:  13 Sep 1997  7 new versions  (PDD)
 *  012:  18 Sep 1997  3 new versions  (PDD)
 *  013:  29 Sep 1997  5 new versions  (Peter Schoen, Miron Schmidt,
 *                                      Garry Lancaster)
 *  014:   2 Oct 1997  6 new versions  (Dorothy O'Donnell, WWW)
 *  015:  12 Oct 1997  6 new versions  (Peter Schoen)
 *  016:  17 Oct 1997  4 new versions  (PDD)
 *  017:  18 Nov 1997  4 new versions  (Ken Russell, Gunther Schmidl)
 *  018:  19 Nov 1997  3 new versions  (Gunther Schmidl)
 *  019:   2 Dec 1997  4 new versions  (PDD, Paul Welbourn, Peter Schoen)
 *  020:   8 Dec 1997  2 new versions  (Lee Tonks)
 *  021:  21 Aug 1998 10 new versions  (Steffen Einsle, WoS)
 *  022:  24 Jan 1999 12 new versions  (Miron Schmidt & David Gorst)
 *  023:  11 Apr 1999  3 new versions  (Dimitris Kiminas)
 *  024:  15 Apr 1999  1 new version   (Dimitris Kiminas)
 *                    11 new patches   (PDD), deprotection added
 *  025:  16 Apr 1999  1 new version   (PDD)
 *                    10 new patches   (PDD)
 *  026:  18 Apr 1999  7 new versions  (PDD, Dimitris Kiminas)
 *                    11 new patches   (PDD)
 *  027:  24 Apr 1999  4 new versions  (Chris Link, PDD)
 *                    22 new patches   (PDD)
 *  028:  26 Apr 1999  4 new versions  (Dimitris Kiminas, PDD)
 *                     5 new patches   (PDD)
 *  029:  27 May 1999  6 new versions  (Dimitris Kiminas, Sarinee/Underdogs)
 *                    15 new patches   ("Mok" via Sarinee/Underdogs)
 *  030:  28 May 1999 13 new patches   (PDD)
 *  031:  14 Jun 1999  4 new versions  (Mike Mee, Sarinee/Underdogs)
 *  032:  20 Jul 1999 18 new versions  (PDD, Dimitris Kiminas)
 *                    24 new patches   (PDD)
 *  033:   5 Mar 2000    new database format (patches outsourced)
 *                     3 new versions  (PDD)
 *  034:   7 Mar 2000    reformatted again (sorry Miron...)
 *  035:  15 Mar 2000  1 new version   (Achim Haertel)
 *  036:  17 Mar 2000  1 new version   (Dimitris Kiminas)
 *  037:   9 Apr 2000  1 new version   (Dimitris Kiminas)
 *  038:  20 Jul 2000    small fixes   (Dimitris Kiminas)
 *  039:  22 May 2001    some comments added/changed
 *  040:   8 Jun 2001  4 new versions  (David Kinder, PDD)
 *  041:  18 Jun 2001    corrections, format improved (Miron Schmidt)
 *  042:   5 Jul 2001  6 new versions  (WWW -> David Batty)
 *                     1 checksum error corrected
 *  043:   9 Dec 2001  3 new versions  (TZXvault -> David Kinder)
 *  044:  12 Dec 2001    reordered Speccy PoM versions
 *  045:   3 Jan 2002  7 new versions  (Alan Staniforth)
 *  046:  25 Feb 2002 10 new versions  (StairwayToHell, TZXvault ->
 *                                      thanks to David Kinder)
 *  047:  26 Feb 2002  4 new versions  (StairwayToHell)
 *  048:   4 Mar 2002  5 new versions  (StairwayToHell, PDD)
 *  049:  21 Oct 2002  3 new versions  (TZX Vault, PDD)
 *  050:  22 Oct 2002  4 new versions  (PDD)
 */

#define WANTED 9

struct versionbase
  {
    type8 version;		/* V2/V3/V4, V5 is split V4 */
    type16 length;		/* datafile length, bytes */
    type8 checksum;		/* 8-bit checksum, last datafile byte */
    type16 crcvalue;		/* 16-bit CRC, l9cut-internal */
    type8 patchstate;		/* 0=unprotected original,
				   1=patched, ---- REMOVED
				   2=original with Lenslok protection,
				   3=original with manual protection */
    char *game;			/* game title and platform */
  }
version_table[] =
{
/*
 *  Chronology:
 *  ----------
 *  (1st date: announcement; 2nd: approximate release)
 *
 *  Missile Defence            ??
 *  rqForth (BBC)              <=83
 *  Forth Toolkit (BBC)        <=83
 *  Compass (Assembler, Lynx)  <=83
 *
 *  Colossal Adventure         <=5/83
 *  Adventure Quest            <=6/83
 *  Dungeon Adventure          <=6/83
 *  Snowball                   ~83-84
 *  Lords of Time              ~83-84
 *  Return to Eden      08/84  12/84  (L9FS has 10/84)
 *  Erik the Viking     11/84  01/85
 *  Emerald Isle        01/85  05/85
 *  Red Moon            08/84  09/85  (L9FS has 07/85) = last V2
 *  Adrian Mole I       06/85  12/85
 *  Worm in Paradise    01/85  02/86  (L9FS has 12/85)
 *  Price of Magik      01/85  07/86  (L9FS has 05/86)
 *  Jewels of Darkness  07/86  10/86
 *  The Archers         05/86  01/87  (L9FS has 1985 - huh?)
 *  Silicon Dreams      12/86  02/87  (L9FS has 12/86)
 *  Adrian Mole II      05/86  04/87                        = last V3
 *  Knight Orc          07/86  12/87  (L9FS has 07/87)
 *  Gnome Ranger         ----  01/88  (L9FS has 09/87)
 *  Time & Magik        02/88  06/88
 *  Lancelot            07/88  12/88
 *  Ingrid's Back       09/88  03/89
 *  Scapeghost/Spook    06/89  01/90
 *  Gnome Free          ca.89   ----
 *  Champion of the Raj           91?
 *
 *  It Came From the Desert (PC)  91?
 *
 *
 *  Datafile naming scheme:
 *  ----------------------
 *  _a: Atari        _j: Macintosh      _s: Spectrum
 *  _b: BBC          _l: Apple II       _t: Atari ST
 *  _c: C-64         _m: Amiga          _x: MSX
 *  _d: DOS (PC)     _p: CPC
 *
 *  .1/.2/.3/.4/.5 (.5 for split V4/GD)
 *
 *  *.?c: corrupt     *.?[pqr]: Patched     *.4g: C-64 Gfx
 *  *.?cf: fixed      *.3s: Speccy48/CPC64  *.4t: C-64 TO
 *  *.?[vw]: variant  *.3l: Speccy/CPC128   *.4u: USA (Datasoft)
 *
 *  Platforms:
 *  ---------
 *  Covered (fully, more or less):
 *           Amiga, Atari ST, PC;
 *           C-64, Sinclair Spectrum, Atari 8bit, Amstrad CPC, BBC
 *  Partially covered:
 *           Apple ][, Macintosh, MSX
 *  Completely missing:
 *           Lynx 48k             [found nothing -> only V1?]
 *           NASCOM               [found nothing -> only V1?]
 *           Oric-1               [found nothing -> only V1?]
 *           Enterprise 64        [found only V1 Colossal, but some
 *                                 V2 were also released]
 *           Memotech MTX512      [V1 & V2; Colossal, Snowball, Eden:
 *                                 not convertable yet -> per@active.se]
 *           Sinclair QL          [found nothing, but at least JoD and
 *                                 SD were released]
 *           Spectravideo SVI-328 [found nothing -> really released?]
 *           Nimbus               [found nothing -> really released?]
 *           Electron             [were they ported?]
 *           Dragon               [were they ported?]
 *
 *  (Notable omissions are marked WANTED in this database.)
 *
 *  Identifiers:
 *  -----------
 *  C-64: Gfx means version with graphics; TO means text-only version
 *        (V4 games were released both in complete text-only and
 *        shortened text-plus-gfx versions)
 *
 */

/*---------------------------> V2 <-----------------------*/

/*
 *  V2 Adventure Quest, (c) ? Level 9
 */

  2, 0x5323, 0xb7, 0x8af7, 0, "Adventure Quest (CPC/Speccy)",
    WANTED + 2, 0x0000, 0x00, 0x0000, 0, "Adventure Quest (MSX)",

/*
 *  V2 Dungeon Adventure, (c) ? Level 9
 */

/*
   next two are totally different: compare shows 0x01 in CPC with
   various other values in MSX
 */
    2, 0x630e, 0x8d, 0x7d7d, 0, "Dungeon Adventure (CPC)",
    2, 0x630e, 0xbe, 0x3374, 0, "Dungeon Adventure (MSX)",
    WANTED + 2, 0x0000, 0x00, 0x0000, 0, "Dungeon Adventure (Speccy)",

/*
 *  V2 Lords of Time, (c) ? Level 9
 */

/*
   next three are different: compare shows 0x01 in CPC with
   various other values in Speccy/MSX -> Speccy is not corrupted,
   as it's from TAP file *and* older SNA!
   (MSX & Speccy: only one byte different)
 */
    2, 0x5eb9, 0x30, 0xe99a, 0, "Lords of Time (CPC)",
    2, 0x5eb9, 0x5d, 0xc098, 0, "Lords of Time (MSX)",
    2, 0x5eb9, 0x6e, 0xc689, 0, "Lords of Time (Speccy)",

/*
 *  V2 Snowball, (c) 83 Level 9
 */

/*
   next two are totally different: compare shows 0x01 in CPC with
   various other values in MSX
 */
    2, 0x5fab, 0x5c, 0xa309, 0, "Snowball (CPC)",
    2, 0x5fab, 0x2f, 0x8aa2, 0, "Snowball (MSX)",
    WANTED + 2, 0x0000, 0x00, 0x0000, 0, "Snowball (Speccy)",

/*
 *  V2 Return to Eden, (c) 84 Level 9
 */

    2, 0x60c4, 0x28, 0x0154, 0, "Return to Eden (CPC/C-64[v1])",
/*
   next two have only 9 bytes difference
 */
    2, 0x6064, 0x01, 0x5b3c, 0, "Return to Eden (BBC[v1])",
    2, 0x6064, 0x95, 0x510c, 0, "Return to Eden (C-64[v2])",
/*
   next one has a 3-byte difference to the "normal" C-64[v2]; however,
   the GS interpreter refuses to run it
 */
    2, 0x6064, 0xda, 0xe610, 0, "Return to Eden (C-64[v2] *corrupt*)",
/*
   next one is very different; may also be corrupted; again the GS
   interpreter does not run it
 */

    WANTED + 2, 0x6064, 0x00, 0x0000, 0, "Return to Eden (Atari)",
    2, 0x6064, 0xbd, 0x73ec, 0, "Return to Eden (Atari *corrupt*)",
    2, 0x6047, 0x6c, 0x17ab, 0, "Return to Eden (BBC[v2])",
    2, 0x5ca1, 0x33, 0x1c43, 0, "Return to Eden (Speccy[v1])",
/* next two have only 4 bytes difference */
    2, 0x5cb7, 0x64, 0x0790, 0, "Return to Eden (Speccy[v2])",
    2, 0x5cb7, 0xfe, 0x3533, 0, "Return to Eden (MSX)",

/*
 *  V2 The Saga of Erik the Viking, (c) ? Level 9
 */

/* next two only one byte difference -> Speccy corrupted? */
    2, 0x34b3, 0x20, 0xccda, 0, "Erik the Viking (BBC/C-64)",
    2, 0x34b3, 0x53, 0x8f00, 0, "Erik the Viking (Speccy)",
/*
   rather different: always various values where others have 0x01
   -> CPC probably corrupted
 */
    2, 0x34b3, 0xc7, 0x9058, 0, "Erik the Viking (CPC)",
    WANTED + 2, 0x0000, 0x00, 0x0000, 0, "Erik the Viking (Atari)",
    WANTED + 2, 0x0000, 0x00, 0x0000, 0, "Erik the Viking (MSX)",

/*
 *  V2 Emerald Isle, (c) 85 Level 9
 */

    2, 0x63be, 0xd6, 0xcf5d, 0, "Emerald Isle (Atari/C-64/CPC/Speccy)",
    WANTED + 2, 0x63be, 0x00, 0x0000, 0, "Emerald Isle (MSX)",
/* next one: longish difference with first, probably corruption */
    2, 0x63be, 0x0a, 0x21ed, 0, "Emerald Isle (MSX *corrupt*)",
    2, 0x378c, 0x8d, 0x3a21, 0, "Emerald Isle (BBC)",

/*
 *  V2 Red Moon, (c) 85 Level 9
 */

    2, 0x506c, 0xf0, 0xba72, 0, "Red Moon (BBC/C-64/CPC/MSX)",
    2, 0x505d, 0x32, 0x2dcf, 0, "Red Moon (Speccy)",
    WANTED + 2, 0x0000, 0x00, 0x0000, 0, "Red Moon (Atari)",


/*---------------------------> V3 <-----------------------*/

/*
 *  V3 Worm in Paradise, (c) 85 Level 9
 */

/* next two Speccy versions on same tape... */
    3, 0x772b, 0xcd, 0xa503, 0, "Worm in Paradise (Speccy128)",
    3, 0x546c, 0xb7, 0x9420, 0, "Worm in Paradise (Speccy48)",
/*
   this version, although corrupt, seems to have been released by
   Level 9 as is (David Kinder has original tape which displays the
   same errors); therefore it's likely that there is no uncorrupted
   version of this datafile
 */
    3, 0x6d84, 0xf9, 0x49ae, 0, "Worm in Paradise (C-64 *corrupt*)",
/* next one is fixed from the corrupt version! */
    3, 0x6d84, 0xc8, 0x943f, 0, "Worm in Paradise (C-64 *fixed*)",
    3, 0x6030, 0x47, 0x46ad, 0, "Worm in Paradise (CPC)",
    3, 0x5828, 0xbd, 0xe7cb, 0, "Worm in Paradise (BBC)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "Worm in Paradise (MSX)",

/*
 *  V3 Price of Magik, (c) 86 Level 9
 */

/*
   L9DIS fails on the Speccy128 and C-64 versions (but not
   on Speccy48, CPC and BBC) -> most messages get numbered
   incorrectly...
 */

/* next two Speccy versions on same tape... */
    3, 0x7410, 0x5e, 0x60be, 2, "Price of Magik (Speccy128)",
    3, 0x5aa4, 0xc1, 0x10a0, 2, "Price of Magik (Speccy48[v1])",
/* next one has two byte difference to first one -- earlier version? */
    3, 0x5aa4, 0xc1, 0xeda4, 2, "Price of Magik (Speccy48[v2])",
    WANTED + 3, 0x7334, 0x87, 0x0000, 2, "Price of Magik (MSX)",
    3, 0x6fc6, 0x14, 0xf9b6, 2, "Price of Magik (C-64)",
/* next one: 5 bytes different from S48var, 3 bytes from S48 */
    3, 0x5aa4, 0xc1, 0xbbf4, 2, "Price of Magik (CPC)",
    3, 0x5671, 0xbc, 0xff35, 0, "Price of Magik (BBC)",

/*
 *  V3 Jewels of Darkness, (c) 86 Level 9
 */

    3, 0x76f4, 0x5e, 0x1fe5, 3, "Colossal Adventure /JoD (Amiga/PC)",
    3, 0x76f4, 0x5a, 0xcf4b, 3, "Colossal Adventure /JoD (ST)",
    3, 0x6e60, 0x83, 0x18e0, 3, "Adventure Quest /JoD (Amiga/PC)",
    3, 0x6e5c, 0xf6, 0xd356, 3, "Adventure Quest /JoD (ST)",
    3, 0x6f0c, 0x95, 0x1f64, 3, "Dungeon Adventure /JoD (Amiga/PC/ST)",

    WANTED + 3, 0x0000, 0x00, 0x0000, 3, "Colossal Adventure /JoD (Mac)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 3, "Adventure Quest /JoD (Mac)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 3, "Dungeon Adventure /JoD (Mac)",

    3, 0x6f70, 0x40, 0xbd91, 2, "Colossal Adventure /JoD (MSX)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 2, "Adventure Quest /JoD (MSX)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 2, "Dungeon Adventure /JoD (MSX)",

    3, 0x6f6e, 0x78, 0x28cd, 2, "Colossal Adventure /JoD (Speccy128)",
    3, 0x6970, 0xd6, 0xa820, 2, "Adventure Quest /JoD (Speccy128)",
    3, 0x6de8, 0x4c, 0xd795, 2, "Dungeon Adventure /JoD (Speccy128)",

    3, 0x6f4d, 0xcb, 0xe8f2, 2, "Colossal Adventure /JoD (CPC128[v1]/Speccy+3)",
    3, 0x6f6a, 0xa5, 0x8dd2, 2, "Colossal Adventure /JoD (CPC128[v2])",
    3, 0x6968, 0x32, 0x0c01, 2, "Adventure Quest /JoD (CPC128/Speccy+3)",
    3, 0x6dc0, 0x63, 0x5d95, 2, "Dungeon Adventure /JoD (CPC128/Speccy+3)",

    3, 0x5e31, 0x7c, 0xaa54, 2, "Colossal Adventure /JoD (CPC64)",
    3, 0x5b50, 0x66, 0x1800, 2, "Adventure Quest /JoD (CPC64)",
    3, 0x58a6, 0x24, 0xb50f, 2, "Dungeon Adventure /JoD (CPC64)",

    3, 0x6c8e, 0xb6, 0x9be3, 2, "Colossal Adventure /JoD (C-64)",
    3, 0x63b6, 0x2e, 0xef38, 2, "Adventure Quest /JoD (C-64)",
    3, 0x6bd2, 0x65, 0xa41f, 2, "Dungeon Adventure /JoD (C-64)",

    3, 0x5b16, 0x3b, 0xe2aa, 2, "Colossal Adventure /JoD (Atari)",
    3, 0x5b58, 0x50, 0x332e, 2, "Adventure Quest /JoD (Atari)",
    3, 0x593a, 0x80, 0x7a34, 2, "Dungeon Adventure /JoD (Atari)",

    3, 0x5a8e, 0xf2, 0x7cca, 2, "Colossal Adventure /JoD (Speccy48)",
    3, 0x5ace, 0x11, 0xdc12, 2, "Adventure Quest /JoD (Speccy48)",
    3, 0x58a3, 0x38, 0x8ce4, 2, "Dungeon Adventure /JoD (Speccy48)",

/*
 *  V3 Silicon Dreams, (c) 86 Level 9
 */

/* is there a C-64 TO? */

    3, 0x7b31, 0x6e, 0x2e2b, 3, "Snowball /SD (Amiga/ST)",
    3, 0x7d16, 0xe6, 0x5438, 3, "Return to Eden /SD (Amiga/ST)",
    3, 0x7cd9, 0x0c, 0x4df1, 3, "Worm in Paradise /SD (Amiga/ST)",

    3, 0x7b2f, 0x70, 0x6955, 3, "Snowball /SD (Mac/PC/Speccy128)",
/* next one only 4-byte difference -> not corrupt! */
    3, 0x7b2f, 0x70, 0x6f6c, 3, "Snowball /SD (CPC/Speccy+3)",
    3, 0x7d14, 0xe8, 0xfbab, 3, "Return to Eden /SD (PC)",
    3, 0x7cff, 0xf8, 0x6044, 3, "Return to Eden /SD (CPC/Speccy+3)",
    3, 0x7cf8, 0x24, 0x9c1c, 3, "Return to Eden /SD (Mac)",
    3, 0x7c55, 0x18, 0xdaee, 3, "Return to Eden /SD (Speccy128)",
    3, 0x7cd7, 0x0e, 0x4feb, 3, "Worm in Paradise /SD (CPC/Mac/PC/Speccy128/Speccy+3)",

    3, 0x7363, 0x65, 0xa0ab, 3, "Snowball /SD (C-64)",
    3, 0x772f, 0xca, 0x8602, 3, "Return to Eden /SD (C-64)",
    3, 0x788d, 0x72, 0x888a, 3, "Worm in Paradise /SD (C-64)",

    3, 0x6bf8, 0x3f, 0xc9f7, 3, "Snowball /SD (Atari)",
    3, 0x60f7, 0x68, 0xc2bc, 3, "Return to Eden /SD (Atari)",
    3, 0x6161, 0xf3, 0xe6d7, 3, "Worm in Paradise /SD (Atari)",

    3, 0x67a3, 0x9d, 0x1d05, 3, "Snowball /SD (Apple ][)",
    3, 0x639c, 0x8b, 0x06e2, 3, "Return to Eden /SD (Apple ][)",
    3, 0x60dd, 0xf2, 0x5bb8, 3, "Worm in Paradise /SD (Apple ][)",

    3, 0x6541, 0x02, 0x2e6c, 3, "Snowball /SD (Speccy48)",
    3, 0x5f43, 0xca, 0x828c, 3, "Return to Eden /SD (Speccy48)",
    3, 0x5ebb, 0xf1, 0x4dec, 3, "Worm in Paradise /SD (Speccy48)",

/*
 *  V3 Secret Diary of Adrian Mole, (c) ? Level 9/Mosaic
 */

    3, 0x8333, 0xb7, 0xe2ac, 0, "Adrian Mole I, pt. 1 (C-64)",
    3, 0x844d, 0x50, 0x5353, 0, "Adrian Mole I, pt. 2 (C-64)",
    3, 0x8251, 0x5f, 0x862a, 0, "Adrian Mole I, pt. 3 (C-64)",
    3, 0x7a78, 0x5e, 0x6ea3, 0, "Adrian Mole I, pt. 4 (C-64)",

    WANTED + 3, 0x8324, 0x87, 0x0000, 0, "Adrian Mole I, pt. 1 (MSX)",
    WANTED + 3, 0x8442, 0x0f, 0x0000, 0, "Adrian Mole I, pt. 2 (MSX)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "Adrian Mole I, pt. 3 (MSX)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "Adrian Mole I, pt. 4 (MSX)",

    3, 0x7c6f, 0x0f, 0xba24, 0, "Adrian Mole I, pt. 1 (CPC)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "Adrian Mole I, pt. 2 (CPC)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "Adrian Mole I, pt. 3 (CPC)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "Adrian Mole I, pt. 4 (CPC)",

    3, 0x72fa, 0x8b, 0x6f12, 0, "Adrian Mole I, pt. 1 (Speccy)",
    3, 0x738e, 0x5b, 0x7e3d, 0, "Adrian Mole I, pt. 2 (Speccy)",
    3, 0x7375, 0xe5, 0x3f3e, 0, "Adrian Mole I, pt. 3 (Speccy)",
    3, 0x78d5, 0xe3, 0xcd7d, 0, "Adrian Mole I, pt. 4 (Speccy)",

    3, 0x3a31, 0xe5, 0x0bdb, 0, "Adrian Mole I, pt. 1 (BBC)",
    3, 0x37f1, 0x77, 0xd231, 0, "Adrian Mole I, pt. 2 (BBC)",
    3, 0x3900, 0x1c, 0x5d9a, 0, "Adrian Mole I, pt. 3 (BBC)",
    3, 0x3910, 0xac, 0x07f9, 0, "Adrian Mole I, pt. 4 (BBC)",
    3, 0x3ad6, 0xa7, 0x95d2, 0, "Adrian Mole I, pt. 5 (BBC)",
    3, 0x38a5, 0x0f, 0xdefc, 0, "Adrian Mole I, pt. 6 (BBC)",
    3, 0x361e, 0x7e, 0xfd9f, 0, "Adrian Mole I, pt. 7 (BBC)",
    3, 0x3934, 0x75, 0xe141, 0, "Adrian Mole I, pt. 8 (BBC)",
    3, 0x3511, 0xcc, 0xd829, 0, "Adrian Mole I, pt. 9 (BBC)",
    3, 0x38dd, 0x31, 0x2534, 0, "Adrian Mole I, pt. 10 (BBC)",
    3, 0x39c0, 0x44, 0x89df, 0, "Adrian Mole I, pt. 11 (BBC)",
    3, 0x3a12, 0x8f, 0xc2bd, 0, "Adrian Mole I, pt. 12 (BBC)",

/*
 *  V3 Growing Pains of Adrian Mole, (c) 86 Level 9/Virgin Games
 */

    3, 0x7931, 0xb9, 0xc51b, 0, "Adrian Mole II, pt. 1 (C-64/CPC)",
    3, 0x7cdf, 0xa5, 0x43e3, 0, "Adrian Mole II, pt. 2 (C-64/CPC)",
    3, 0x7a0c, 0x97, 0x4bea, 0, "Adrian Mole II, pt. 3 (C-64/CPC)",
    3, 0x7883, 0xe2, 0xee0e, 0, "Adrian Mole II, pt. 4 (C-64/CPC)",

    3, 0x6841, 0x4a, 0x94e7, 0, "Adrian Mole II, pt. 1 (Speccy)",
    3, 0x6bc0, 0x62, 0xab3d, 0, "Adrian Mole II, pt. 2 (Speccy)",
    3, 0x692c, 0x21, 0x2015, 0, "Adrian Mole II, pt. 3 (Speccy)",
    3, 0x670a, 0x94, 0xa2a6, 0, "Adrian Mole II, pt. 4 (Speccy)",

    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "Adrian Mole II, pt. 1 (MSX)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "Adrian Mole II, pt. 2 (MSX)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "Adrian Mole II, pt. 3 (MSX)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "Adrian Mole II, pt. 4 (MSX)",

    3, 0x593a, 0xaf, 0x30e9, 0, "Adrian Mole II, pt. 1 (BBC)",
    3, 0x57e6, 0x8a, 0xc41a, 0, "Adrian Mole II, pt. 2 (BBC)",
    3, 0x5819, 0xcd, 0x1ba0, 0, "Adrian Mole II, pt. 3 (BBC)",
    3, 0x579b, 0xad, 0xa723, 0, "Adrian Mole II, pt. 4 (BBC)",

/*
 *  V3 The Archers, (c) ? Level 9/Mosaic
 */

    3, 0x765d, 0xcd, 0xfc02, 0, "The Archers, pt. 1 (C-64)",
    3, 0x6e58, 0x07, 0xbffc, 0, "The Archers, pt. 2 (C-64)",
    3, 0x7e98, 0x6a, 0x95e5, 0, "The Archers, pt. 3 (C-64)",
    3, 0x81e2, 0xd5, 0xb278, 0, "The Archers, pt. 4 (C-64)",

    3, 0x6ce5, 0x58, 0x46de, 0, "The Archers, pt. 1 (Speccy)",
    3, 0x68da, 0xc1, 0x3b8e, 0, "The Archers, pt. 2 (Speccy)",
    3, 0x6c67, 0x9a, 0x9a6a, 0, "The Archers, pt. 3 (Speccy)",
    3, 0x6d91, 0xb9, 0x12a7, 0, "The Archers, pt. 4 (Speccy)",

    3, 0x5834, 0x42, 0xcc9d, 0, "The Archers, pt. 1 (BBC)",
    3, 0x56dd, 0x51, 0xe582, 0, "The Archers, pt. 2 (BBC)",
    3, 0x5801, 0x53, 0xf2ef, 0, "The Archers, pt. 3 (BBC)",
    3, 0x54a4, 0x01, 0xc0ab, 0, "The Archers, pt. 4 (BBC)",

    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "The Archers, pt. 1 (CPC)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "The Archers, pt. 2 (CPC)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "The Archers, pt. 3 (CPC)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "The Archers, pt. 4 (CPC)",

    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "The Archers, pt. 1 (MSX)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "The Archers, pt. 2 (MSX)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "The Archers, pt. 3 (MSX)",
    WANTED + 3, 0x0000, 0x00, 0x0000, 0, "The Archers, pt. 4 (MSX)",


/*---------------------------> V3a ("V5") <-----------------------*/
/*
   These are the famous "split" (two-part) datafiles. In its current
   configuration L9Cut only extracts the GAMEDATA part, not the ACODE
   part. They can be glued together by hand, and there is also a
   compile option DKPATCH provided by David Kinder which does the job
   automatically for Spectrum snapshots.
 */
/* GD=game data, AC=acode (AC not yet included) */

/* two-part V3 (BBC) */

    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Knight Orc, pt. 1 GD (BBC)",
    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Knight Orc, pt. 2 GD (BBC)",
    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Knight Orc, pt. 3 GD (BBC)",

    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Gnome Ranger, pt. 1 GD (BBC)",
    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Gnome Ranger, pt. 2 GD (BBC)",
    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Gnome Ranger, pt. 3 GD (BBC)",

    5, 0x579e, 0x97, 0x9faa, 0, "Lords of Time /T&M GD (BBC)",
    5, 0x5500, 0x50, 0xca75, 0, "Red Moon /T&M GD (BBC)",
    5, 0x579a, 0x2a, 0x9373, 0, "Price of Magik /T&M GD (BBC)",

    5, 0x4fd2, 0x9d, 0x799a, 0, "Lancelot, pt. 1 GD (BBC)",
    5, 0x4dac, 0xa8, 0x86ed, 0, "Lancelot, pt. 2 GD (BBC)",
    5, 0x4f96, 0x22, 0x30f8, 0, "Lancelot, pt. 3 GD (BBC)",

    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Ingrid's Back, pt. 1 GD (BBC)",
    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Ingrid's Back, pt. 2 GD (BBC)",
    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Ingrid's Back, pt. 3 GD (BBC)",

    5, 0x55ce, 0xa1, 0xba12, 0, "Scapeghost, pt. 1 GD (BBC)",
    5, 0x54a6, 0xa9, 0xc9f3, 0, "Scapeghost, pt. 2 GD (BBC)",
    5, 0x51bc, 0xe3, 0x89c3, 0, "Scapeghost, pt. 3 GD (BBC)",

/* two-part V3 (CPC/Speccy+3) */

/* (Knight Orc is from Speccy+3 disk, but CPC should be the same) */
    5, 0x46ec, 0x64, 0x2300, 0, "Knight Orc, pt. 1 GD (CPC/Speccy+3)",
    5, 0x6140, 0x18, 0x4f66, 0, "Knight Orc, pt. 2 GD (CPC/Speccy+3)",
    5, 0x640e, 0xc1, 0xfc69, 0, "Knight Orc, pt. 3 GD (CPC/Speccy+3)",

/* (Gnome Ranger is CPC+PCW version) */
    5, 0x5ff0, 0xf8, 0x3a13, 0, "Gnome Ranger, pt. 1 GD (CPC/Speccy+3)",
    5, 0x6024, 0x01, 0xaaa9, 0, "Gnome Ranger, pt. 2 GD (CPC/Speccy+3)",
    5, 0x6036, 0x3d, 0x6c6c, 0, "Gnome Ranger, pt. 3 GD (CPC/Speccy+3)",

    5, 0x69fe, 0x56, 0xecfb, 0, "Lords of Time /T&M GD (CPC/Speccy+3)",
    5, 0x6888, 0x8d, 0x7f6a, 0, "Red Moon /T&M GD (CPC/Speccy+3)",
    5, 0x5a50, 0xa9, 0xa5fa, 0, "Price of Magik /T&M GD (CPC/Speccy+3)",

/* (Lancelot is from Speccy+3 disk, but CPC should be the same) */
    5, 0x5c7a, 0x44, 0x460e, 0, "Lancelot, pt. 1 GD (CPC/Speccy+3)",
    5, 0x53a2, 0x1e, 0x2fae, 0, "Lancelot, pt. 2 GD (CPC/Speccy+3)",
    5, 0x5914, 0x22, 0x4a31, 0, "Lancelot, pt. 3 GD (CPC/Speccy+3)",

    5, 0x5a38, 0xf7, 0x876e, 0, "Ingrid's Back, pt. 1 GD (CPC/Speccy+3)",
    5, 0x531a, 0xed, 0xcf3f, 0, "Ingrid's Back, pt. 2 GD (CPC/Speccy+3)",
    5, 0x57e4, 0x19, 0xb354, 0, "Ingrid's Back, pt. 3 GD (CPC/Speccy+3)",

    5, 0x5cbc, 0xa5, 0x0dbe, 0, "Scapeghost, pt. 1 GD (CPC/Speccy+3)",
    5, 0x5932, 0x4e, 0xb2f5, 0, "Scapeghost, pt. 2 GD (CPC/Speccy+3)",
    5, 0x5860, 0x95, 0x3227, 0, "Scapeghost, pt. 3 GD (CPC/Speccy+3)",

/* two-part V3 (Spectrum 128) */

    5, 0x74e0, 0x92, 0x885e, 0, "Knight Orc, pt. 1 GD (Speccy128)",
    5, 0x6dbc, 0x97, 0x6f55, 0, "Knight Orc, pt. 2 GD (Speccy128)",
    5, 0x7402, 0x07, 0x385f, 0, "Knight Orc, pt. 3 GD (Speccy128)",

    5, 0x52aa, 0xdf, 0x7b5b, 0, "Gnome Ranger, pt. 1 GD (Speccy128)",
    5, 0x6ffa, 0xdb, 0xdde2, 0, "Gnome Ranger, pt. 2 GD (Speccy128)",
    5, 0x723a, 0x69, 0x039b, 0, "Gnome Ranger, pt. 3 GD (Speccy128)",

    5, 0x6f1e, 0xda, 0x2ce0, 0, "Lords of Time /T&M GD (Speccy128)",
    5, 0x6da0, 0xb8, 0x3802, 0, "Red Moon /T&M GD (Speccy128)",
    5, 0x6108, 0xdd, 0xefe7, 0, "Price of Magik /T&M GD (Speccy128)",

    5, 0x768c, 0xe8, 0x8fc6, 0, "Lancelot, pt. 1 GD (Speccy128)",
    5, 0x76b0, 0x1d, 0x0fcd, 0, "Lancelot, pt. 2 GD (Speccy128)",
    5, 0x765e, 0x4f, 0x3b73, 0, "Lancelot, pt. 3 GD (Speccy128)",

    5, 0x76a0, 0x3a, 0xb803, 0, "Ingrid's Back, pt. 1 GD (Speccy128)",
    5, 0x7674, 0x0b, 0xe92f, 0, "Ingrid's Back, pt. 2 GD (Speccy128)",
    5, 0x765e, 0xba, 0x086d, 0, "Ingrid's Back, pt. 3 GD (Speccy128)",

    5, 0x762e, 0x82, 0x8848, 0, "Scapeghost, pt. 1 GD (Speccy128)",
    5, 0x5bd6, 0x35, 0x79ef, 0, "Scapeghost, pt. 2 GD (Speccy128)",
    5, 0x6fa8, 0xa4, 0x62c2, 0, "Scapeghost, pt. 3 GD (Speccy128)",

/*---------------------------> V4 <-----------------------*/

/*
 *  V4 Knight Orc UK version, (c) 87 Level 9/Rainbird
 */

    4, 0xbb93, 0x36, 0x6a05, 3, "Knight Orc, pt. 1 (Amiga)",
    4, 0xbb6e, 0xad, 0x4d40, 3, "Knight Orc, pt. 1 (ST)",
    4, 0xc58e, 0x4a, 0x4e9d, 3, "Knight Orc, pt. 2 (Amiga/ST)",
    4, 0xcb9a, 0x0f, 0x0804, 3, "Knight Orc, pt. 3 (Amiga/ST)",

    4, 0xbb6e, 0xa6, 0x9753, 3, "Knight Orc, pt. 1 (PC)",
    4, 0xc58e, 0x43, 0xe9ce, 3, "Knight Orc, pt. 2 (PC)",
    4, 0xcb9a, 0x08, 0x6c36, 3, "Knight Orc, pt. 3 (PC)",

/* Apple ][: only part 1 protected */
    4, 0x898a, 0x43, 0xfc8b, 3, "Knight Orc, pt. 1 (Apple ][)",
    4, 0x8b9f, 0x61, 0x7288, 0, "Knight Orc, pt. 2 (Apple ][)",
    4, 0x8af9, 0x61, 0x7542, 0, "Knight Orc, pt. 3 (Apple ][)",

    WANTED + 4, 0x0000, 0x00, 0x0000, 0, "Knight Orc, pt. 1 (C-64 TO)",
    WANTED + 4, 0x0000, 0x00, 0x0000, 0, "Knight Orc, pt. 2 (C-64 TO)",
    WANTED + 4, 0x0000, 0x00, 0x0000, 0, "Knight Orc, pt. 3 (C-64 TO)",

/* C-64 Gfx: only part 1 protected */
    4, 0x8970, 0x6b, 0x3c7b, 3, "Knight Orc, pt. 1 (C-64 Gfx)",
    4, 0x8b90, 0x4e, 0x098c, 0, "Knight Orc, pt. 2 (C-64 Gfx)",
    4, 0x8aea, 0x4e, 0xca54, 0, "Knight Orc, pt. 3 (C-64 Gfx)",

/* Speccy48: only part 1 protected */
    4, 0x86d0, 0xb7, 0xadbd, 3, "Knight Orc, pt. 1 (Speccy48)",
    4, 0x8885, 0x22, 0xe293, 0, "Knight Orc, pt. 2 (Speccy48)",
    4, 0x87e5, 0x0e, 0xdc33, 0, "Knight Orc, pt. 3 (Speccy48)",

/*
 *  V4 Gnome Ranger UK version, (c) 87 Level 9
 */

    4, 0xb1a9, 0x80, 0x5fb7, 3, "Gnome Ranger, pt. 1 (Amiga/ST)",
    4, 0xab9d, 0x31, 0xbe6d, 3, "Gnome Ranger, pt. 2 (Amiga/ST)",
    4, 0xae28, 0x87, 0xb6b6, 3, "Gnome Ranger, pt. 3 (Amiga/ST)",

    4, 0xb0ec, 0xc2, 0x0053, 3, "Gnome Ranger, pt. 1 (ST[v1])",
    4, 0xaf82, 0x83, 0x19f7, 3, "Gnome Ranger, pt. 2 (ST[v1])",
/*
   only have corrupt version of next one (unfixable: total
   corruption in jumptable 7800-788f, plus at the end)
   [Source: ST-CD #4, GAU_077]
 */
    WANTED + 4, 0xb3e6, 0xab, 0x0000, 3, "Gnome Ranger, pt. 3 (ST[v1])",

    4, 0xb1aa, 0xad, 0xaf47, 3, "Gnome Ranger, pt. 1 (PC)",
    4, 0xb19e, 0x92, 0x8f96, 3, "Gnome Ranger, pt. 1 (ST[v2])",
    4, 0xab8b, 0xbf, 0x31e6, 3, "Gnome Ranger, pt. 2 (PC/ST[v2])",
    4, 0xae16, 0x81, 0x8741, 3, "Gnome Ranger, pt. 3 (PC/ST[v2])",

    WANTED + 4, 0x0000, 0x00, 0x0000, 3, "Gnome Ranger, pt. 1 (Mac)",
    WANTED + 4, 0x0000, 0x00, 0x0000, 3, "Gnome Ranger, pt. 2 (Mac)",
    WANTED + 4, 0x0000, 0x00, 0x0000, 3, "Gnome Ranger, pt. 3 (Mac)",

    4, 0xad41, 0xa8, 0x42c5, 3, "Gnome Ranger, pt. 1 (C-64 TO)",
    4, 0xa735, 0xf7, 0x2e08, 3, "Gnome Ranger, pt. 2 (C-64 TO)",
    4, 0xa9c0, 0x9e, 0x0d70, 3, "Gnome Ranger, pt. 3 (C-64 TO)",

    4, 0x908e, 0x0d, 0x58a7, 3, "Gnome Ranger, pt. 1 (C-64 Gfx)",
    4, 0x8f6f, 0x0a, 0x411a, 3, "Gnome Ranger, pt. 2 (C-64 Gfx)",
    4, 0x9060, 0xbb, 0xe75d, 3, "Gnome Ranger, pt. 3 (C-64 Gfx)",

    4, 0x8aab, 0xc0, 0xde5f, 3, "Gnome Ranger, pt. 1 (Speccy48)",
    4, 0x8ac8, 0x9a, 0xc89b, 3, "Gnome Ranger, pt. 2 (Speccy48)",
    4, 0x8a93, 0x4f, 0x10cc, 3, "Gnome Ranger, pt. 3 (Speccy48)",

/*
 *  V4 Time & Magik UK version, (c) 88 Level 9/Mandarin
 *     [PC: EGA/MGA, 8 gfx modes]
 */

/*
   The disk-based T&M (UK or USA) versions (all except C-64 TO,
   Speccy48) use a certain opcode during SAVE which crashes old
   versions of Glen Summers' Level9 interpreter after the "insert
   your formatted save position disc and press a key" message (or
   in the case of the Mac version *just before* the "insert [...]"
   message).

   David Kinder's version 3.0 of the interpreter fixes this bug.
 */

    4, 0xb57c, 0x44, 0x7779, 3, "Lords of Time /T&M (PC)",
    4, 0xa69e, 0x6c, 0xb268, 3, "Red Moon /T&M (PC)",
    4, 0xbac7, 0x7f, 0xddb2, 3, "Price of Magik /T&M (PC)",

/*
   next one: fixed corrupt version of part 1 (41f8:19->39)
 */
    4, 0xb579, 0x89, 0x3e89, 3, "Lords of Time /T&M (ST)",
    4, 0xa698, 0x41, 0xcaca, 3, "Red Moon /T&M (ST)",
    4, 0xbac4, 0x80, 0xa750, 3, "Price of Magik /T&M (ST)",

    4, 0xb576, 0x2a, 0x7239, 3, "Lords of Time /T&M (Amiga)",
    4, 0xa692, 0xd1, 0x6a99, 3, "Red Moon /T&M (Amiga)",
    4, 0xbaca, 0x3a, 0x221b, 3, "Price of Magik /T&M (Amiga)",

    4, 0xb563, 0x6a, 0x0c5c, 3, "Lords of Time /T&M (Mac)",
    4, 0xa67c, 0xb8, 0xff41, 3, "Red Moon /T&M (Mac)",
    4, 0xbab2, 0x87, 0x09f5, 3, "Price of Magik /T&M (Mac)",

    4, 0xb38c, 0x37, 0x9f8e, 3, "Lords of Time /T&M (C-64 TO)",
    4, 0xa4e2, 0xa6, 0x016d, 3, "Red Moon /T&M (C-64 TO)",
    4, 0xb451, 0xa8, 0x2682, 3, "Price of Magik /T&M (C-64 TO)",

    4, 0x9070, 0x43, 0x45d4, 3, "Lords of Time /T&M (C-64 Gfx)",
    4, 0x903f, 0x6b, 0x603e, 3, "Red Moon /T&M (C-64 Gfx)",
    4, 0x8f51, 0xb2, 0x6c9a, 3, "Price of Magik /T&M (C-64 Gfx)",

    4, 0x8950, 0xa1, 0xbb16, 3, "Lords of Time /T&M (Speccy48)",
    4, 0x8813, 0x11, 0x22de, 3, "Red Moon /T&M (Speccy48)",
    4, 0x8a60, 0x2a, 0x29ed, 3, "Price of Magik /T&M (Speccy48)",

/*
 *  V4 Time & Magik USA version, (c) 88 Datasoft
 *     [PC: EGA/CGA/MGA, 23 gfx modes]
 */

    4, 0xb260, 0xe5, 0xc5b2, 0, "Lords of Time /T&M (PC/ST *USA*)",
    4, 0xa3a4, 0xdf, 0x6732, 0, "Red Moon /T&M (PC/ST *USA*)",
    4, 0xb7a0, 0x7e, 0x2226, 0, "Price of Magik /T&M (PC/ST *USA*)",

    4, 0xb257, 0xf8, 0xfbd5, 0, "Lords of Time /T&M (Amiga *USA*)",
    4, 0xa398, 0x82, 0xd031, 0, "Red Moon /T&M (Amiga *USA*)",
    4, 0xb797, 0x1f, 0x84a9, 0, "Price of Magik /T&M (Amiga *USA*)",

    4, 0x8d78, 0x3a, 0xba6e, 0, "Lords of Time /T&M (C-64 Gfx *USA*)",
    4, 0x8d56, 0xd3, 0x146a, 0, "Red Moon /T&M (C-64 Gfx *USA*)",
    4, 0x8c46, 0xf0, 0xcaf6, 0, "Price of Magik /T&M (C-64 Gfx *USA*)",

/*
 *  V4 Lancelot UK version, (c) 88 Level 9/Mandarin
 */

    4, 0xc0cf, 0x4e, 0xb7fa, 3, "Lancelot, pt. 1 (Amiga/PC/ST)",
    4, 0xd5e9, 0x6a, 0x4192, 3, "Lancelot, pt. 2 (Amiga/PC/ST)",
    4, 0xbb8f, 0x1a, 0x7487, 3, "Lancelot, pt. 3 (Amiga/PC/ST)",

    4, 0xc0bd, 0x57, 0x6ef1, 3, "Lancelot, pt. 1 (Mac)",
    4, 0xd5d7, 0x99, 0x770b, 3, "Lancelot, pt. 2 (Mac)",
    4, 0xbb7d, 0x17, 0xbc42, 3, "Lancelot, pt. 3 (Mac)",

    4, 0xb4c9, 0x94, 0xd784, 3, "Lancelot, pt. 1 (C-64 TO)",
    4, 0xb729, 0x51, 0x8ee5, 3, "Lancelot, pt. 2 (C-64 TO)",
    4, 0xb702, 0xe4, 0x1809, 3, "Lancelot, pt. 3 (C-64 TO)",

    4, 0x8feb, 0xba, 0xa800, 3, "Lancelot, pt. 1 (C-64 Gfx)",
    4, 0x8f6b, 0xfa, 0x0f7e, 3, "Lancelot, pt. 2 (C-64 Gfx)",
    4, 0x8f71, 0x2f, 0x0ddc, 3, "Lancelot, pt. 3 (C-64 Gfx)",

    4, 0x8ade, 0xf2, 0xfffb, 0, "Lancelot, pt. 1 (Speccy48)",
    4, 0x8b0e, 0xfb, 0x0bab, 0, "Lancelot, pt. 2 (Speccy48)",
    4, 0x8ab3, 0xc1, 0xcb62, 0, "Lancelot, pt. 3 (Speccy48)",

/*
 *  V4 Lancelot USA version, (c) 88 Datasoft
 */

    4, 0xbba4, 0x94, 0x0871, 0, "Lancelot, pt. 1 (Amiga/PC *USA*)",
    4, 0xd0c0, 0x56, 0x8c48, 0, "Lancelot, pt. 2 (Amiga/PC *USA*)",
    4, 0xb6ac, 0xc6, 0xaea0, 0, "Lancelot, pt. 3 (Amiga/PC *USA*)",

    WANTED + 4, 0x0000, 0x00, 0x0000, 0, "Lancelot, pt. 1 (ST *USA*)",
    WANTED + 4, 0x0000, 0x00, 0x0000, 0, "Lancelot, pt. 2 (ST *USA*)",
    WANTED + 4, 0x0000, 0x00, 0x0000, 0, "Lancelot, pt. 3 (ST *USA*)",

    4, 0x8afc, 0x07, 0x8321, 0, "Lancelot, pt. 1 (C-64 Gfx *USA*)",
    4, 0x8aec, 0x13, 0x6791, 0, "Lancelot, pt. 2 (C-64 Gfx *USA*)",
    4, 0x8aba, 0x0d, 0x5602, 0, "Lancelot, pt. 3 (C-64 Gfx *USA*)",

/*
 *  V4 Ingrid's Back UK version, (c) 88 Level 9
 */

    4, 0xd19b, 0xad, 0x306d, 3, "Ingrid's Back, pt. 1 (PC)",
    4, 0xc5a5, 0xfe, 0x3c98, 3, "Ingrid's Back, pt. 2 (PC)",
    4, 0xd7ae, 0x9e, 0x1878, 3, "Ingrid's Back, pt. 3 (PC)",

    4, 0xd188, 0x13, 0xdc60, 3, "Ingrid's Back, pt. 1 (Amiga)",
    4, 0xc594, 0x03, 0xea95, 3, "Ingrid's Back, pt. 2 (Amiga)",
    4, 0xd79f, 0xb5, 0x1661, 3, "Ingrid's Back, pt. 3 (Amiga)",

    4, 0xd183, 0x83, 0xef72, 3, "Ingrid's Back, pt. 1 (ST)",
    4, 0xc58f, 0x65, 0xf337, 3, "Ingrid's Back, pt. 2 (ST)",
    4, 0xd79a, 0x57, 0x49c5, 3, "Ingrid's Back, pt. 3 (ST)",

    4, 0xb770, 0x03, 0x9a03, 3, "Ingrid's Back, pt. 1 (C-64 TO)",
    4, 0xb741, 0xb6, 0x2aa5, 3, "Ingrid's Back, pt. 2 (C-64 TO)",
    4, 0xb791, 0xa1, 0xd065, 3, "Ingrid's Back, pt. 3 (C-64 TO)",

    4, 0x9089, 0xce, 0xc5e2, 3, "Ingrid's Back, pt. 1 (C-64 Gfx)",
    4, 0x908d, 0x80, 0x30c7, 3, "Ingrid's Back, pt. 2 (C-64 Gfx)",
    4, 0x909e, 0x9f, 0xdecc, 3, "Ingrid's Back, pt. 3 (C-64 Gfx)",

    4, 0x8ab7, 0x68, 0xee57, 0, "Ingrid's Back, pt. 1 (Speccy48)",
    4, 0x8b1e, 0x84, 0x2538, 0, "Ingrid's Back, pt. 2 (Speccy48)",
    4, 0x8b1c, 0xa8, 0x9262, 0, "Ingrid's Back, pt. 3 (Speccy48)",

/*
 *  V4 Scapeghost UK version, (c) 89 Level 9
 */

/*
   Version 2.0 of Glen Summers' Level9 interpreter doesn't allow game
   part selection in the PC/ST, Amiga *bak* and C-64 Gfx versions, 
   due to incorrect V4 detection.

   This problem is fixed in David Kinder's version 3.0 of the
   interpreter.
 */

    4, 0xbeab, 0x2d, 0x94d9, 0, "Scapeghost, pt. 1 (Amiga)",
    4, 0xc132, 0x14, 0x7adc, 0, "Scapeghost, pt. 1 (Amiga *bak*)",
    4, 0xbe94, 0xcc, 0x04b8, 0, "Scapeghost, pt. 1 (PC/ST)",
    4, 0x99bd, 0x65, 0x032e, 0, "Scapeghost, pt. 2 (Amiga/PC/ST)",
    4, 0xbcb6, 0x7a, 0x7d4f, 0, "Scapeghost, pt. 3 (Amiga/PC/ST)",

    WANTED + 4, 0x0000, 0x00, 0x0000, 0, "Scapeghost, pt. 1 (C-64 TO)",
    WANTED + 4, 0x0000, 0x00, 0x0000, 0, "Scapeghost, pt. 2 (C-64 TO)",
    WANTED + 4, 0x0000, 0x00, 0x0000, 0, "Scapeghost, pt. 3 (C-64 TO)",

    4, 0x9058, 0xcf, 0x9748, 0, "Scapeghost, pt. 1 (C-64 Gfx)",
    4, 0x8f43, 0xc9, 0xeefd, 0, "Scapeghost, pt. 2 (C-64 Gfx)",
    4, 0x90ac, 0x68, 0xb4a8, 0, "Scapeghost, pt. 3 (C-64 Gfx)",

    4, 0x8a21, 0xf4, 0xd9e4, 0, "Scapeghost, pt. 1 (Speccy48)",
    4, 0x8a12, 0xe3, 0xc2ff, 0, "Scapeghost, pt. 2 (Speccy48)",
    4, 0x8a16, 0xcc, 0x4f3b, 0, "Scapeghost, pt. 3 (Speccy48)",

/*
 *  Split V4(?) Champion of the Raj [English], (c) 90 Mirrorsoft
 */

    5, 0x3ebb, 0x00, 0xf6dc, 0, "Champion of the Raj (English) 1/2 GD (Amiga)",
    5, 0x0fd8, 0x00, 0xf250, 0, "Champion of the Raj (English) 2/2 GD (Amiga)",

    5, 0x3e8f, 0x00, 0x5599, 0, "Champion of the Raj (English) 1/2 GD (ST)",
    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Champion of the Raj (English) 2/2 GD (ST)",

    5, 0x3e4f, 0x00, 0xb202, 0, "Champion of the Raj (English) 1/2 GD (PC)",
    5, 0x14a3, 0x00, 0xa288, 0, "Champion of the Raj (English) 2/2 GD (PC)",

/* the ST demo was on "ST Format" #19 cover disk */
    5, 0x1929, 0x00, 0xd4b2, 0, "Champion of the Raj (demo), 1/2 GD (ST)",
    5, 0x40e0, 0x02, 0x080d, 0, "Champion of the Raj (demo), 2/2 GD (ST)",

/*
 *  Split V4(?) Champion of the Raj [German], (c) 90 Mirrorsoft
 */

    5, 0x4872, 0x00, 0x9515, 0, "Champion of the Raj (German) 1/2 GD (Amiga)",
    5, 0x11f5, 0x00, 0xbf39, 0, "Champion of the Raj (German) 2/2 GD (Amiga)",

    5, 0x4846, 0x00, 0xd9c1, 0, "Champion of the Raj (German) 1/2 GD (ST)",
    5, 0x11f5, 0x00, 0x7aa4, 0, "Champion of the Raj (German) 2/2 GD (ST)",

    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Champion of the Raj (German) 1/2 GD (PC)",
    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Champion of the Raj (German) 2/2 GD (PC)",

/*
 *  Split V4(?) Champion of the Raj [French], (c) 90 Mirrorsoft
 */

    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Champion of the Raj (French) 1/2 GD (Amiga)",
    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Champion of the Raj (French) 2/2 GD (Amiga)",

    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Champion of the Raj (French) 1/2 GD (ST)",
    5, 0x110f, 0x00, 0x4b57, 0, "Champion of the Raj (French) 2/2 GD (ST)",

    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Champion of the Raj (French) 1/2 GD (PC)",
    WANTED + 5, 0x0000, 0x00, 0x0000, 0, "Champion of the Raj (French) 2/2 GD (PC)",

    0, 0x0000, 0x00, 0x0000, 0, "*",
};

void
data_usage (void)
{
  type16 count = 0, countwant = 0, patchcount = 0;

  while (version_table[count].version != 0)
    {
      if (version_table[count].version > WANTED)
	countwant++;
      count++;
    }
  count = (sizeof (version_table) / sizeof (struct versionbase)) - 1;
  patchcount = patch_usage ();

  fprintf (stderr,
	   " (database " BASEVERSION ", patchbase " PBASEVERSION "):\n\
Extracts Level 9 datafile (V2-4) from most types of sequential input data.\n\
Current version detects %d original and %d patched datafile versions.\n",
	   count - countwant, patchcount);
}
