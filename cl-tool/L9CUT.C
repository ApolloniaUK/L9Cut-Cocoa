#define PRGNAME "L9Cut"
#define PRGVERSION "3.13"
#define PRGDATE "21.10.2002"

/*
 *  l9cut.c
 *
 *  - extracts Level 9 datafile (V2-4) from most types of input data
 *
 *  Author: Paul David Doherty <42@pdd.de>
 *
 *  with significant fixes provided by:
 *          Andreas Aumayr <anden@DoubleA.at>
 *          David Kinder <D.Kinder@btinternet.com>
 *          Miron Schmidt <miron.schmidt@berlin.de>
 *          Alan Staniforth <alan@apollonia.org>
 *
 *  v1.0:    31 Jul 1996
 *  v1.1:     5 Aug 1996  version database added
 *  v1.2:    12 Mar 1997  faster V3 recognition (Birgit Ahlemeyer)
 *  v1.3:    14 Mar 1997  desperate mode added
 *  v1.4-10:              database updates (see l9data_d.h)
 *  v2.0:     9 Sep 1997  buffered I/O; CRCs; Z80/PRG detection
 *  v2.01:   12 Sep 1997  Z80 decompression added
 *  v2.02:   13 Sep 1997  V1 trap; Z80 bug removed; decomp sped up
 *  v2.03:   15 Sep 1997  fixes too small to mention
 *  v2.04:   17 Sep 1997  small fix for PC versions
 *  v2.05:   18 Sep 1997  fix for header detection (desperate mode)
 *  v2.06:   17 Oct 1997  V2 fix (for SecDAdMole pt. 4/C-64)
 *  v2.07:   19 Nov 1997  added "split V4" warning
 *  v2.08:    2 Dec 1997  handles very small files (Raj II)
 *  v2.09:    8 Dec 1997  fix for Speccy/Mole I pt.3
 *  v2.50:   15 Apr 1999  added patch detection and protection removal,
 *                        allow override for C-64 compressed files,
 *                        fix for false V3 detection
 *  v2.51:   27 May 1999  small fixes for SAS-C (Andreas Aumayr)
 *  v2.52:   14 Jun 1999  fgetchar() fix for DJGPP, >64k address space
 *                        for Amiga/DJGPP
 *  v2.53:   20 Jul 1999  more info for V2 boundary crossing, even
 *                        smaller files allowed now (Raj II)
 *  v2.60:    5 Mar 2000  reorganized databases, separated patches
 *                        (fixing patch bug reported by Achim Haertel)
 *  v2.61:    7 Mar 2000  added -mp option
 *  v3.00:   14 May 2001  cleanups for portability
 *                        version number jump for the new millennium
 *  v3.01:   22 May 2001  portability fixes for GCC
 *  v3.02:    4 Jul 2001  removed "hassle Glen Summers" message
 *  v3.03:   31 Dec 2001  modified to build for Mac OS (Alan Staniforth)
 *                        2 memory leaks fixed (Alan Staniforth)
 *  v3.04:    3 Jan 2002  very minor cosmetic changes
 *  v3.10:   25 Feb 2002  added -md and -ld options 
 *                        code slightly rearranged (Alan Staniforth)
 *  v3.11:   26 Feb 2002  better V1 detection
 *  v3.12:    4 Mar 2002  small fixes
 *  v3.13:   21 Oct 2002  small Mac fix
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#ifdef macintosh		/* Mac specific includes */
#include <Memory.h>
#include <Sioux.h>
#include <console.h>
#include <unix.h>
#include <setjmp.h>
#endif

#ifdef __TURBOC__
#include <io.h>
#endif

/* various defines */

/* 
   #define DEBUG
   #define DKPATCH for David Kinder's split-V4-merging patch
   #define LARGE for Amiga, DJGPP, Macintosh etc.
 */
#if defined(__DJGPP__) || defined(macintosh)
#define LARGE
#endif

/* typedefs needed by l9data_d.h & l9data_p.h */

typedef unsigned char type8;
typedef unsigned short type16;
typedef unsigned long int type32;

#ifdef LARGE
typedef type32 type16_32;
#define CUTOFF 100000L
#else
typedef type16 type16_32;
#define CUTOFF 64000L
#endif

#include "l9data_p.h"
#include "l9data_d.h"
#include "l9crc.h"

/* other global stuff */
#if TARGET_API_MAC_CARBON	/* header clash with Mac Carbon target */
#undef TRUE
#undef FALSE
#endif
#define TRUE 1
#define FALSE 0

FILE *fdi, *fdo;
FILE *fzin, *fzout;
type8 *infiledata;
type32 snalength, l9offset;
type16 l9length;
type8 l9check;

/* Mac specific globals */
#ifdef macintosh
jmp_buf env;
#endif

/* arbitrary V3/V4 distinction based on Archers/Adrian Mole */
#define V3LIMIT 0x8500

/* arbitrary minimum filelength to prevent false detection */
#define MINLIMIT 0x0fd0

/* Amiga version string */
char *amiverstag = "\0$VER: " PRGNAME " " PRGVERSION " (" PRGDATE ")";

/* prototypes */
void ex (char *error);
void about (void);
void usage (void);
char checkc64 (void);
char check_z80 (char *infilename);
void z80_block (type16 length, type16 blsize);
void trans_block_z80 (type32 seed, type8 blocknum, type8 mem);
void dec_open_z80 (char *infilename);
type16 calcword (type32 i);
char v1_check (void);
char v2_recognition (void);
char v3_recognition (int type_of_check);
type16 database_version (type16 length, type8 chk, type16 crc, type8 no_crc);
type16 patchbase_version (type16 length, type8 chk, type16 crc);
type16 patchbase_patch (type16 length, type8 chk, type16 crc);
void all_games_list (void);
void missing_games_list (void);
void missing_patch_list (void);
#ifdef macintosh
short call_l9cut (char *inSourcePathName, char *inDestPathName);
int l9cut_main (int argc, char **argv);
#endif


/*>>>>>>>>>>>>>>>>>>>>> various routines <<<<<<<<<<<<<<<<<<<<< */

/* leave the program due to error */
void
ex (char *error)
{
  fprintf (stderr, PRGNAME ": %s\n", error);
#ifdef macintosh
/*
   In the Mac build l9cut runs as a function of a program
   so return, not quit
 */
  longjmp (env, 1);
#endif
  exit (1);
}

/* print info about the program */
void
about (void)
{
  fprintf (stderr, PRGNAME);
  fprintf (stderr, " version " PRGVERSION);
#ifdef LARGE
  fprintf (stderr, "L");
#endif
#ifdef DEBUG
  fprintf (stderr, " [DEBUG]");
#endif
  data_usage ();
  fprintf (stderr,
	   "Copyleft 1996-2002 by Paul David Doherty <42@pdd.de>\n");
#ifdef macintosh
  fprintf (stderr,
	   "Macintosh port by Alan Staniforth <alan@apollonia.org>\n");
#endif
  fprintf (stderr, "\n");
  fflush (stderr);
}

/* called with too few/too many parameters */
void
usage (void)
{
  about ();
  fprintf (stderr,
	   "Usage: " PRGNAME " infile outfile :  extract datafile\n");
  fprintf (stderr,
      "       " PRGNAME " -ld            :  list all detected datafiles\n");
  fprintf (stderr,
   "       " PRGNAME " -md            :  list (known) missing datafiles\n");
  fprintf (stderr,
	   "       " PRGNAME " -mp            :  list missing patches\n");
  exit (1);
}

/* check for C-64 PRG file */
char
checkc64 (void)
{
  char returnvalue = FALSE;
  char yesno;

/* binary PRGs */
  if ((infiledata[0] == 0x01) && (infiledata[1] == 0x08)
      && (infiledata[3] == 0x08) && (infiledata[2] > 0x0a)
      && (infiledata[2] < 0x30))
    returnvalue = TRUE;
/* T64 files */
  if ((infiledata[0] == 'C') && (infiledata[1] == '6')
      && (infiledata[3] == 'S') && (infiledata[0x0401] == 0x08)
      && (infiledata[0x0400] > 0x0a) && (infiledata[0x0400] < 0x30))
    returnvalue = TRUE;
/* P00 files */
  if ((infiledata[0] == 'C') && (infiledata[1] == '6')
      && (infiledata[3] == 'F') && (infiledata[0x1a] == 0x01)
      && (infiledata[0x1b] == 0x08) && (infiledata[0x1d] == 0x08)
      && (infiledata[0x1c] > 0x0a) && (infiledata[0x1c] < 0x30))
    returnvalue = TRUE;

  if (returnvalue == TRUE)
    {
      fprintf (stderr, "This is a C-64 program file. ");
      fprintf (stderr, "Since these files are usually compressed,\n");
      fprintf (stderr, "you may have to convert it to a memory snapshot ");
      fprintf (stderr, "before running " PRGNAME ".\n");
      fprintf (stderr, "Do you want to continue anyway (y/N)? ");
      fflush (stderr);
      yesno = fgetc (stdin);
#ifdef macintosh		/* please, just don't ask... */
      (void) fgetc (stdin);
#endif
      fflush (stdin);
      if ((yesno == 'y') || (yesno == 'Y'))
	returnvalue = FALSE;
    }
  return (returnvalue);
}

/* check for "Z80" extension */
char
check_z80 (char *infilename)
{
  char *filename;
  type8 i, j;
  char ret = FALSE;

  if ((filename = (char *) malloc ((strlen (infilename) + 1))) == NULL)
    ex ("not enough memory");

  strcpy (filename, infilename);
  for (i = strlen (filename); i > 0; i--)
    if (filename[i] == '.')
      break;
  i++;
  for (j = i; j <= strlen (filename); j++)
    filename[j - i] = filename[j];

  if ((strcmp (filename, "Z80") == 0) || (strcmp (filename, "z80") == 0))
    ret = (TRUE);
  else
    ret = (FALSE);

  free (filename);
  return (ret);
}

/* copy compressed Z80 block from fzin to fzout */
void
z80_block (type16 length, type16 blsize)
{
#ifdef DEBUG
  type32 curpos;
#endif
  int count;
  type8 i;
  type16 x, y;
  type8 *inblock, *outblock;

#ifdef DEBUG
  curpos = ftell (fzin);
  printf ("block: %06lx, ", curpos);
  printf ("len: %04x \n", length);
#endif

  if ((inblock = (type8 *) malloc ((length == 0xffff) ? 16384 : length)) == NULL)
    ex ("not enough memory");
  if ((outblock = (type8 *) malloc (blsize)) == NULL)
    {
      free (inblock);
      ex ("not enough memory");
    }
  if (fread (inblock, ((length == 0xffff) ? 16384 : length), 1, fzin) != 1)
    {
      free (inblock);
      free (outblock);
      ex ("couldn't finish read\n");
    }

/*
   careful with the next one: I originally had a length of
   0x4000 as "always uncompressed", but that's not correct:
   ed ed ed -> ed ed 03 ed  (+1)
   but
   00 00 00 00 00 -> ed ed 05 00 (-1)
   or
   ed 00 00 00 00 00 00 -> ed 00 ed ed 05 00 (-1)
   (The Pawn has a compressed/uncompressed block of 0x4000)
 */
  if (length == 0xffff)
    fwrite (inblock, 16384, 1, fzout);
  else
    {
      x = 0;
      y = 0;
/*
   Decode a 16k/48k block
   (Contrary to popular belief, putting ++ and -- statements on lines
   by themselves does not increase compilation or execution time...)
 */
      while (length > x)
	{
	  if (inblock[x] != 237)
	    {
	      outblock[y] = inblock[x];
	      y++;
	    }
	  else
	    {
	      x++;
	      if (inblock[x] != 237)
		{
		  outblock[y] = 237;
		  y++;
		  outblock[y] = inblock[x];
		  y++;
		}
	      else
		{
		  x++;
		  count = inblock[x];
		  x++;
		  for (i = 0; i < count; i++)
		    {
		      outblock[y] = inblock[x];
		      y++;
		    }
		}
	    }
	  x++;
	}
      fwrite (outblock, blsize, 1, fzout);
    }
  free (inblock);
  free (outblock);
}

void
trans_block_z80 (type32 seed, type8 blocknum, type8 mem)
{
  type8 bltype, i;
  type32 length = 0L, end;

#ifdef DEBUG
  printf ("trans: %02lx, ", seed);
  printf ("#%02x\n", blocknum);
#endif

  fseek (fzin, seed, SEEK_SET);
  for (i = 3; i <= 10; i++)
    {
      length = fgetc (fzin);
      length += 256 * fgetc (fzin);
      bltype = fgetc (fzin);

      if ((bltype != i) && (mem == 128))
	fprintf (stderr, "block numbers don't match");
      if (bltype == blocknum)
	break;
      else
	{
	  end = ftell (fzin) + length;
	  fseek (fzin, end, SEEK_SET);
	}
      if ((mem == 48) && (i == 5))
	break;
    }
  z80_block (length, 16384);
}

/* decode Z80 into tempfile, open tempfile as fdi */
void
dec_open_z80 (char *infilename)
{
  char *dumpfilename;
  type32 length, seekseed;
  type8 sntype, bltype, i;

  if ((dumpfilename = (char *) malloc ((strlen (infilename) + 1))) == NULL)
    ex ("not enough memory");
  strcpy (dumpfilename, infilename);
  for (i = strlen (dumpfilename); i > 0; i--)
    if (dumpfilename[i] == '.')
      break;
  dumpfilename[i] = 0;
  strcat (dumpfilename, ".dmp");

  if ((fzin = fopen (infilename, "rb")) == NULL)
    ex ("infile doesn't exist");
  if ((fzout = fopen (dumpfilename, "wb")) == NULL)
    ex ("can't create tempfile");

  fprintf (stderr, "Decompressing Z80 ");

/* Z80 decoding */
  fseek (fzin, 30L, SEEK_SET);
  switch (fgetc (fzin))
    {
    case 23:
      fprintf (stderr, "v2.01 ");
      sntype = 2;
      seekseed = 55L;
      break;
    case 54:
      fprintf (stderr, "v3.0x ");
      sntype = 2;
      seekseed = 86L;
      break;
    default:
      fprintf (stderr, "v1.45 ");
      sntype = 1;
      seekseed = 30L;
      break;
    }

/*
   trivia: a decoded 48k snapshot will not be identical to its
   SNA equivalent, since the SNA format requires the PC to be
   pushed on the stack
 */
  if (sntype == 1)
    {
      fprintf (stderr, "48k snapshot...\n");
/* all 48k in one block */
      fseek (fzin, 0L, SEEK_END);
      length = ftell (fzin) - 34;
      fseek (fzin, seekseed, SEEK_SET);
      z80_block (length, (type16) 49152L);
    }
  else
    {
      fseek (fzin, seekseed, SEEK_SET);
      length = fgetc (fzin);
      length += 256 * fgetc (fzin);
      bltype = fgetc (fzin);

      if (bltype == 4)
	{
	  fprintf (stderr, "48k snapshot...\n");
/* for 48k: order is 5-1-2 (or in Z80 terms, 8-4-5) */
	  trans_block_z80 (seekseed, 8, 48);
	  trans_block_z80 (seekseed, 4, 48);
	  trans_block_z80 (seekseed, 5, 48);
	}
      else if (bltype == 3)
	{
	  fprintf (stderr, "128k snapshot...\n");
/* for 128k: order is 5-2-0 (or in Z80 terms, 8-5-3) */
	  trans_block_z80 (seekseed, 8, 128);
	  trans_block_z80 (seekseed, 5, 128);
	  trans_block_z80 (seekseed, 3, 128);
#ifdef DKPATCH
	  /*DK */
	  trans_block_z80 (seekseed, 4, 128);
	  /*DK */
#endif
	}
      else
	ex ("unknown snapshot type");
    }

/* tempfile will be infile from now on; it will not be deleted */
  fclose (fzin);
  fclose (fzout);
  if ((fdi = fopen (dumpfilename, "rb")) == NULL)
    ex ("tempfile got lost");

  free (dumpfilename);
}

/* calculate word starting at byte position */
type16
calcword (type32 i)
{
  type16 calc;

  calc = infiledata[i];
  calc += 256 * (infiledata[i + 1]);
  return (calc);
}

/*>>>>>>>>>>>>>>>>>>>>> V1 trap hack <<<<<<<<<<<<<<<<<<<<< */

char
v1_check (void)
{
/*
   V2 has an unencrypted dictionary too, so v2check must be performed
   before this one
 */
  type32 i, vocstart;
  type8 attval = 0xff, bunval = 0xff;
  type32 attoffset = 0, bunoffset = 0;

#ifdef DEBUG
  fprintf (stderr, "V1 check...\n");
#endif

/* check 1: ATTACK */
  for (i = 0; i < (snalength - 20); i++)
    if (infiledata[i] == 'A')
      if ((infiledata[i + 1] == 'T')
	  && (infiledata[i + 2] == 'T')
	  && (infiledata[i + 3] == 'A')
	  && (infiledata[i + 4] == 'C')
	  && (infiledata[i + 5] == 0xcb))
	{
	  attoffset = i;
	  attval = infiledata[attoffset + 6];
	  break;
	}

/* check 2: BUNCH */
  for (i = attoffset; i < (snalength - 20); i++)
    if (infiledata[i] == 'B')
      if ((infiledata[i + 1] == 'U')
	  && (infiledata[i + 2] == 'N')
	  && (infiledata[i + 3] == 'C')
	  && (infiledata[i + 4] == 0xc8))
	{
	  bunoffset = i;
	  bunval = infiledata[bunoffset + 5];
	  break;
	}

/* evaluation */
  if ((attval == 0xff) && (bunval == 0xff))
    return (FALSE);
  else
    {
      fprintf (stderr, "Detected V1 datafile (");

      if ((attval == 0x14) && (bunval == 0xff))
	{
	  fprintf (stderr, "Dungeon Adventure");
	  vocstart = attoffset - 206L;
	}
      else if ((attval == 0x15) && (bunval == 0x5d))
	{
	  fprintf (stderr, "Lords of Time");
	  vocstart = attoffset - 252L;
	}
      else if ((attval == 0x15) && (bunval == 0x6c))
	{
	  fprintf (stderr, "Snowball");
	  vocstart = attoffset - 284L;
	}
      else if ((attval == 0x1a) && (bunval == 0x24))
	{
	  fprintf (stderr, "Colossal Adventure");
/* 
   Colossal version A has both "jewelry" and "jewellery",
   version B just "jewelry"
 */
	  if ((bunoffset - attoffset) == 0x53)
	    {
	      fprintf (stderr, " [A]");
	      vocstart = attoffset - 301L;
	    }
	  else if ((bunoffset - attoffset) == 0x63)
	    {
	      fprintf (stderr, " [B]");
	      vocstart = attoffset - 301L;
	    }
	  else
	    {
	      fprintf (stderr, " [unknown version]");
	      vocstart = 0L;
	    }
	}
      else if ((attval == 0x20) && (bunval == 0x3b))
	{
	  fprintf (stderr, "Adventure Quest");
	  vocstart = attoffset - 283L;
	}
      else
	{
	  fprintf (stderr, "unknown game %02x/%02x", attval, bunval);
	  vocstart = 0L;
	}
    }
  fprintf (stderr, ")...\n");

  if (vocstart > snalength)
    {
      vocstart = 0L;
      fprintf (stderr, "Dictionary start not found; file incomplete...\n");
    }
  else
    fprintf (stderr, "Dictionary start at offset $%04lx...\n", vocstart);

#ifdef DEBUG
  fprintf (stderr, "ATT: $%04lx   $%04lx\n", attoffset, (attoffset - vocstart));
  fprintf (stderr, "BUN: $%04lx   $%04lx\n", bunoffset, (bunoffset - vocstart));
  fprintf (stderr, "B-A: $%04lx   \n", (bunoffset - attoffset));
#endif

#ifdef DEBUG
  i = vocstart;
  while (i < (((vocstart + 5000L) < snalength) ? (vocstart + 5000L) : snalength))
    {
      type8 tempo;
      tempo = infiledata[i];
      if (tempo < 0x80)
	{
	  fprintf (stdout, "%c", infiledata[i]);
	  i++;
	}
      else
	{
	  fprintf (stdout, "%c %02x\n", (infiledata[i] - 0x80), infiledata[i + 1]);
	  i = i + 2;
	}
    }
#endif

  return (TRUE);
}

/*>>>>>>>>>>>>>>>>>>>>> V2 recognition <<<<<<<<<<<<<<<<<<<<< */

char
v2_recognition (void)
{
  type32 i, j;
  type8 found = 0;

#ifdef DEBUG
  fprintf (stderr, "V2 check...\n");
#endif

  for (i = 0; i < (snalength - 20); i++)
    if ((calcword (i + 4) == 0x0020) &&
	(calcword (i + 0x0a) == 0x8000) &&
	(calcword (i + 0x14) == calcword (i + 0x16)))
/*
   condition 3 is a hack based on currently known V2 datafiles; there
   may be a better solution: e.g. byte (i+0x19) must be 0x80...0x82
 */
      if ((l9length = calcword (i + 0x1c)) != 0)
	{
	  if ((l9length + i) >= snalength)
	    {
	      fprintf (stderr, "Spurious data at $%04lx ", i);
	      fprintf (stderr, "with length $%04x...\n", l9length);
	      ex ("file crosses boundary; cannot extract");
	    }
/* calculate checksum */
	  l9check = 0;
	  for (j = i; j <= (l9length + i); j++)
	    l9check += infiledata[j];
	  l9offset = i;
	  found = 1;
	}
  if (found == 1)
    return (TRUE);
  else
    return (FALSE);
}

/*>>>>>>>>>>>>>>>>>>>>> V3 (V4) recognition <<<<<<<<<<<<<<<<<<<<< */

char
v3_recognition (int type_of_check)
{
  type32 end;
  type32 i, j;
  type8 checksum, realcheck, found;

#ifdef DEBUG
  fprintf (stderr, "V3 check #%d...\n", type_of_check);
#endif

  found = 0;
  for (i = 0; i < (snalength - 20); i++)
    {
      if (found == 0)
	{
	  checksum = 0;
	  l9length = calcword (i);
	  end = (type32) l9length + i;

	  if (type_of_check != 3)
	    {
	      if (end <= (snalength - 2))
		if (((type_of_check == 2) ||
		     (((infiledata[((type16_32) end) - 1] == 0) &&
		       (infiledata[((type16_32) end) - 2] == 0)) ||
		      ((infiledata[((type16_32) end) + 1] == 0) &&
		       (infiledata[((type16_32) end) + 2] == 0))))
		    && (l9length > 0x4000) && (l9length <= 0xdb00))
		  if ((l9length != 0) && (infiledata[i + 0x0d] == 0))
		    /* byte 13 = highbyte of dictdatalength must be 0! */
		    for (j = i; j < (i + 16); j = j + 2)
		      {
			if (((calcword (j) + calcword (j + 2)) == calcword (j + 4))
			    && ((calcword (j) + calcword (j + 2)) != 0))
			  found++;
		      }
	    }
	  else
	    {
	      if ((l9length > MINLIMIT) && (end <= (snalength - 2)))
		if (((calcword (i + 2) + calcword (i + 4)) == calcword (i + 6))
		    && (calcword (i + 2) != 0)
		    && (calcword (i + 4)) != 0)
		  if (((calcword (i + 6) + calcword (i + 8)) == calcword (i + 10))
		      && ((infiledata[i + 18] == 0x2a) || (infiledata[i + 18] == 0x2c))
		      && (infiledata[i + 19] == 0)
		      && (infiledata[i + 20] == 0)
		      && (infiledata[i + 21] == 0))
		    found = 2;
	    }

	  if (found >= 2)
	    {
	      if (type_of_check == 3)
		fprintf (stderr, "Entering desperate mode...\n");

	      l9offset = i;
	      l9check = infiledata[(type16_32) end];
	      for (j = i; j <= end; j++)
		checksum += infiledata[j];
	      if ((checksum == 0) || (type_of_check == 3))
		found = 1;
	      else
		found = 0;

	      if ((checksum != 0) && (type_of_check == 3))
		{
		  realcheck = (type8) (l9check - checksum);
		  fprintf (stderr,
			   "Wrong checksum ($%02x), datafile is corrupt...\n", checksum);
		  fprintf (stderr,
			   "Change checkbyte from $%02x to $%02x to run it...\n", l9check, realcheck);
		}
	    }
	  else
	    found = 0;
	}
    }
  if (found == 1)
    return (TRUE);
  else
    return (FALSE);
}

/*>>>>>>>>>>>>>>>>>>>>> database lookups <<<<<<<<<<<<<<<<<<<<< */

type16
database_version (type16 length, type8 chk, type16 crc, type8 no_crc)
{
  type16 count = 0;

  while (version_table[count].version != 0)
    {
      if ((version_table[count].version < WANTED) &&
	  (length == version_table[count].length) &&
	  (chk == version_table[count].checksum) &&
	  ((no_crc == 1) || (crc == version_table[count].crcvalue)))
	break;
      count++;
    }
  if (version_table[count].version == 0)
    count = 0xffff;
  return (count);
}

type16
patchbase_version (type16 length, type8 chk, type16 crc)
{
  type16 count = 0;

  while (patch_table[count].length != 0x0000)
    {
      if ((length == patch_table[count].length) &&
	  (chk == patch_table[count].patchchk) &&
	  (crc == patch_table[count].patchcrc))
	break;
      count++;
    }
  if (patch_table[count].length == 0x0000)
    count = 0xffff;
  return (count);
}

type16
patchbase_patch (type16 length, type8 chk, type16 crc)
{
  type16 count = 0;

  while (patch_table[count].length != 0x0000)
    {
      if ((length == patch_table[count].length) &&
	  (chk == patch_table[count].origchk) &&
	  (crc == patch_table[count].origcrc))
	break;
      count++;
    }
  if (patch_table[count].length == 0x0000)
    count = 0xffff;
  return (count);
}

/*>>>>>>>>>>>>>>>>>>>>> database lists <<<<<<<<<<<<<<<<<<<<< */

void
all_games_list (void)
{
  type16 count = 0, itemcount = 0;
  fprintf (stdout, "Detected games:\n");
  while (version_table[count].version != 0)
    {
      if (version_table[count].version < WANTED)
	{
	  itemcount++;
	  fprintf (stdout, " %3d. %s", itemcount, version_table[count].game);
	  fprintf (stdout, " (V%d%s)\n",
		   ((version_table[count].version == 5) ? 3 :
		    version_table[count].version),
		   ((version_table[count].version == 5) ? "/SplitV4" : ""));
	}
      count++;
    }
}

void
missing_games_list (void)
{
  type16 count = 0, itemcount = 0;
  fprintf (stdout, "Missing games:\n");
  while (version_table[count].version != 0)
    {
      if (version_table[count].version > WANTED)
	{
	  itemcount++;
	  fprintf (stdout, " %3d. %s", itemcount, version_table[count].game);
	  fprintf (stdout, " (V%d%s)\n",
		   ((version_table[count].version == WANTED + 5) ? 3 :
		    (version_table[count].version - WANTED)),
	  ((version_table[count].version == WANTED + 5) ? "/SplitV4" : ""));
	}
      count++;
    }
}

void
missing_patch_list (void)
{
  type16 count = 0, itemcount = 0;
  fprintf (stdout, "Missing patches:\n");
  while (version_table[count].version != 0)
    {
      /*
         checks whether *any* patch is present
         (assumes that first patch is also applicable!)
       */
      if (version_table[count].version < WANTED)
	if (version_table[count].patchstate != 0)
	  if (patchbase_patch (version_table[count].length,
			       version_table[count].checksum,
			       version_table[count].crcvalue) == 0xffff)
	    {
	      itemcount++;
	      fprintf (stdout, " %3d. %s", itemcount,
		       version_table[count].game);
	      fprintf (stdout, " (V%d%s)\n",
		       ((version_table[count].version == 5) ? 3 :
			version_table[count].version),
		   ((version_table[count].version == 5) ? "/SplitV4" : ""));
	    }
      count++;
    }
}

/*>>>>>>>>>>>>>>>>>>>>> Macintosh interface routines <<<<<<<<<<<<<<<<<<<<< */

#ifdef macintosh
short
call_l9cut (char *inSourcePathName, char *inDestPathName)
{
  char appPath[256] = ":";
  char *argArray[3];
  int theResult;

  fdi = NULL;
  fdo = NULL;
  fzin = NULL;
  fzout = NULL;
  infiledata = NULL;
  snalength = 0;
  l9offset = 0;
  l9length = 0;
  l9check = 0;

  argArray[0] = appPath;
  argArray[1] = inSourcePathName;
  argArray[2] = inDestPathName;

  if (setjmp (env) != 0)
    {
/*
   If ex() is subsequently called then we return here with the
   Result set to 1
 */
      if (NULL != infiledata)
	free (infiledata);
      if (fdi != NULL)
	fclose (fdi);
      if (fdo != NULL)
	fclose (fdo);
      if (fzin != NULL)
	fclose (fzin);
      if (fzout != NULL)
	fclose (fzout);
      return (1);
    }

  theResult = l9cut_main (3, argArray);
  if (theResult)
    remove (inDestPathName);

  return (theResult);
}
#endif

/*>>>>>>>>>>>>>>>>>>>>> main <<<<<<<<<<<<<<<<<<<<< */

#ifdef macintosh
int
l9cut_main (int argc, char **argv)
#else
int
main (int argc, char **argv)
#endif
{
  type32 infilelength;
  type8 patchflag = 0;
  type16 orig_id = 0, patch_id = 0;
  type16 i, crc_checksum;

#ifdef macintosh
  /* temporary storage for creator types */
  unsigned long tempCreator, tempType;
#endif

  if ((argc != 2) && (argc != 3))
    usage ();

  if (argc == 2)
    {
      if (strcmp (argv[1], "-ld") == 0)
	all_games_list ();
      else if (strcmp (argv[1], "-md") == 0)
	missing_games_list ();
      else if (strcmp (argv[1], "-mp") == 0)
	missing_patch_list ();
      else
	usage ();
      exit (0);
    }

/* open I/O streams */
#ifdef __TURBOC__
  _fmode = O_BINARY;
#endif
  if (check_z80 (argv[argc - 2]) == TRUE)
    dec_open_z80 (argv[argc - 2]);
  else if ((fdi = fopen (argv[argc - 2], "rb")) == NULL)
    ex ("infile not found");

#ifdef macintosh
/*
   store current creator/type and change output creator/type
   to match Alan's port of Level9 interpreter
 */
  tempCreator = _fcreator;
  tempType = _ftype;
  _fcreator = 'LVL9';
  _ftype = 'L9GM';
#endif

  if ((fdo = fopen (argv[argc - 1], "wb")) == NULL)
    ex ("could not create outfile");

#ifdef macintosh
  /* restore original current creator/type */
  _fcreator = tempCreator;
  _ftype = tempType;
#endif

/* get length of input file */
  fseek (fdi, 0L, SEEK_END);
  infilelength = ftell (fdi);
  fseek (fdi, 0L, SEEK_SET);

/* kludgy stuff for DOS */
  if (infilelength > CUTOFF)
    {
      snalength = CUTOFF;
      fprintf (stderr, "Infile bigger than %ld; padding discarded...\n", CUTOFF);
    }
  else
    snalength = infilelength;

/* next kludge */
  if (snalength < 2L)
    ex ("file too short");

/* load complete input file into infiledata */
  if ((infiledata = (type8 *) malloc ((type16_32) (snalength + 2))) == NULL)
    ex ("not enough memory");
  if (fread (infiledata, (type16_32) snalength, 1, fdi) != 1)
    ex ("couldn't finish read");

/* 2 bytes of padding for exact length datafiles (PC) */
  infiledata[(type16_32) snalength] = infiledata[(type16_32) snalength + 1] = 0;
  snalength = snalength + 2;

/* check if this is compressed file */
  if (checkc64 () == TRUE)
    ex ("can't extract from compressed data");

/* scanning */
  fprintf (stderr, "Scanning for Level 9 data...\n");
  if (v2_recognition () == TRUE)
    fprintf (stderr, "V2 data found");
  else if (v3_recognition (1) == TRUE)
    fprintf (stderr, "%s data found", (l9length >= V3LIMIT) ? "V4" : "V3");
  else if (v1_check () == TRUE)
    ex ("V1 datafile extraction not supported");
  else if (v3_recognition (2) == TRUE)
    fprintf (stderr, "%s data found", (l9length >= V3LIMIT) ? "V4" : "V3");
  else if (v3_recognition (3) == TRUE)
    fprintf (stderr, "%s data found", (l9length >= V3LIMIT) ? "V4" : "V3");
  else
    ex ("no data found");
  fprintf (stderr, " at offset $%04lx...\n", l9offset);

/* block move datafile to start of infiledata */
  for (i = 0; i < (l9length + 1); i++)
    infiledata[i] = infiledata[i + (type16_32) l9offset];
  infiledata[l9length + 1] = 0;
  infiledata[l9length + 2] = 0;

/* calculate CRC16 */
  crc_checksum = crc16 (infiledata, (l9length + 3));

/* datafile version lookup */

/* is this a patched version? */
  patch_id = patchbase_version (l9length, l9check, crc_checksum);
  if (patch_id != 0xffff)
    {
/* yes it is */
      patchflag = 1;
      orig_id = database_version (l9length, patch_table[patch_id].origchk,
				  patch_table[patch_id].origcrc, 0);
      if (orig_id == 0xffff)
	ex ("patch backtrack error (serious!)");
    }
  else
    {
/* no it ain't -- maybe original? */
      orig_id = database_version (l9length, l9check, crc_checksum, 0);
    }

  if (orig_id == 0xffff)
    {
/* lookup without CRC */
      orig_id = database_version (l9length, l9check, 0xffff, 1);
      if (orig_id != 0xffff)
	{
	  fprintf (stderr, "Seems to be \"%s\" (V%d%s)...\n",
		   version_table[orig_id].game,
		   ((version_table[orig_id].version == 5) ? 3 :
		    version_table[orig_id].version),
		 ((version_table[orig_id].version == 5) ? "/SplitV4" : ""));
	  fprintf (stderr, "Wrong CRC (%04x); datafile is corrupt...\n",
		   crc_checksum);
	}
      else
	{
	  fprintf (stderr, "Unknown game or version ");
	  fprintf (stderr, "[Length: %04x", l9length);
	  fprintf (stderr, "; Checksum: %02x", l9check);
	  fprintf (stderr, "; CRC16: %04x", crc_checksum);
	  fprintf (stderr, "]...\n");
	}
      fprintf (stderr,
	       "Please report the problem to <42@pdd.de>...\n");
    }
  else
    {
/* everything okay */
      fprintf (stderr, "Detected \"%s\" (V%d%s%s)...\n",
	       version_table[orig_id].game,
	       ((version_table[orig_id].version == 5) ? 3 :
		version_table[orig_id].version),
	       ((version_table[orig_id].version == 5) ? "/SplitV4" : ""),
	       ((patchflag == 1) ? " *patched*" : ""));

/* split V4 handling */

#ifdef DKPATCH
      /*DK */
      /* join split V4 datafiles */
      if (version_table[orig_id].version == 5)
	{
	  /* get acode length */
	  type16 acodelen = calcword (0xc000) - 3;
	  l9length -= 2;

	  /* copy acode to the end of the data */
	  for (i = 0; i <= acodelen; i++)
	    infiledata[l9length + i] = infiledata[0xc002 + i];

	  /* adjust length */
	  l9length += acodelen;
	  infiledata[l9length + 2] = 0;
	  infiledata[l9length + 3] = 0;

	  /* update the length */
	  infiledata[0] = (l9length + 1) % 256;
	  infiledata[1] = (l9length + 1) / 256;

	  /* update the checksum */
	  l9check = 0;
	  for (i = 0; i <= l9length; i++)
	    l9check += infiledata[i];
	  infiledata[l9length + 1] = 0x100 - l9check;
	}
      /*DK */

#else
/* warn about split V4 datafiles */
      if (version_table[orig_id].version == 5)
	fprintf (stderr, "\
*****************************************************************\n\
NOTE: This is a \"split datafile\". Support for this type of data\n\
is currently being investigated. Stay tuned!\n\
*****************************************************************\n\
");
#endif

/* warn about old patches */
      if ((patchflag == 1) &&
	  (patch_id != patchbase_patch (l9length,
					version_table[orig_id].checksum,
					version_table[orig_id].crcvalue)))
	fprintf (stderr, "\
*****************************************************************\n\
NOTE: This is a suboptimal patch, possibly created with an older\n\
version of " PRGNAME ". Please reapply the patch to your unpatched\n\
original datafile using the current version of " PRGNAME ".\n\
*****************************************************************\n\
");

/* patch removal option */
      if ((patchflag == 0) && (version_table[orig_id].patchstate != 0))
	{
	  char yesno;

	  fprintf (stderr, "Datafile is copy-protected (%s)...\n",
		   (version_table[orig_id].patchstate == 2) ?
		   "Lenslok" : "manual lookup");

	  patch_id = patchbase_patch (l9length, l9check, crc_checksum);
	  if ((patch_id != 0xffff) &&
	      (patch_table[patch_id].patchloc1 != 0xffff))
	    {
	      fprintf (stderr,
		       "Do you want the protection removed (y/N)? ");
	      fflush (stderr);
	      yesno = fgetc (stdin);
#ifdef macintosh		/* please, just don't ask... */
	      (void) fgetc (stdin);
#endif
	      fflush (stdin);
	      if ((yesno == 'y') || (yesno == 'Y'))
		{
		  fprintf (stderr, "Removing protection...\n");
		  infiledata[patch_table[patch_id].patchloc1] =
		    patch_table[patch_id].patch1;
		  if (patch_table[patch_id].patchloc2 != 0xffff)
		    {
		      infiledata[patch_table[patch_id].patchloc2] =
			patch_table[patch_id].patch2;
		      if (patch_table[patch_id].patchloc3 != 0xffff)
			infiledata[patch_table[patch_id].patchloc3] =
			  patch_table[patch_id].patch3;
		      if (patch_table[patch_id].patchloc4 != 0xffff)
			infiledata[patch_table[patch_id].patchloc4] =
			  patch_table[patch_id].patch4;
		    }
		  infiledata[patch_table[patch_id].length] =
		    patch_table[patch_id].patchchk;
		}
	    }
	  else
	    fprintf (stderr, "(Let me know if you want a deprotection patch!)\n");
	}
    }

/* write datafile to outfile */
  fprintf (stderr, "Writing data...\n");
  fwrite (infiledata, (l9length + 3), 1, fdo);

/* cleanup */
  free (infiledata);
  fclose (fdi);
  fclose (fdo);
  return (0);
}
