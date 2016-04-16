/*
** Copyright (c) 1992, 1993 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
/*condefs.h
**Wayne E. Wright
**Round Rock, Texas
*/

#ifndef CONDEFS_H
#define CONDEFS_H

#include <windows.h>

#define CALL_WID 14

#ifndef FILEINT_H
#include "fileint.h"
#endif

#define NUMBANDS 60             /*number of bands we will support*/


/*type definitions*/

    typedef struct qso_stru FAR * QsoPtr_t;
    typedef char FAR * VarPart_t;

struct qso_stru 
{
    long   freq;   /*frequency in units of 10 Hz*/
    long   rfreq;  /*receive frequency in 10Hz*/
    unsigned long        serial;         /*serial number of this qso*/
    VarPart_t var_part;     /*pointer to variable part of qso*/
    unsigned char    band;  /*index into band listhead*/
    unsigned char   DupeSheet;
    char    stn[CALL_WID];  /*call sign*/
    char    dupe;           /*set to space, or 'D' for dupe qso*/
    char    parfl;          /*0 when parent via next/ 1 when via prev*/
    char    mode;           /*'1' for LSB, '2' USB, '3' CW*/
	FILETIME			m_time;	/* time of qso */

    /*following fields are NOT part of the public interface...*/
    long	 VectorIndex;/*sort pointer to one that sorts earlier*/
    void    *Note;                  /* OLE Object containing note for this*/
    unsigned long  ExtraHeight;    /* Scrolling height of this guy w/Note */
    unsigned long    LocalIndex;     /* which QSO this one is in its block*/
    unsigned long    GlobalIndex;    /* which QSO this one is in world */
    };

/* Bits in parfl */
#define PARFL 1         /* we are prev of parent (0 means we're next) */
#define SENTQSL 2       /* Bit set means a QSL has been printed for this */
#define RECVQSL 4       /* bit means we've received a QSL */
#define WSQSLREC 8      /* means need to write out a Sent QSL record */
#define WRQSLREC 16 /* means need to write out a Received QSL record */

#endif


