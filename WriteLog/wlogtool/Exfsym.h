/*
** Copyright (c) 1992, 1993 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
/*exfsym.h
**Wayne E. Wright
**September 11, 1988
**Round Rock, Texas
**attribute bit definitions
*/

#ifndef EXFSYM_H
#define EXFSYM_H

#define A_CALL  1       /*this exf is call sign field*/
#define A_NUMS  2       /*this exf only includes digits*/
#define A_PRMPT 4       /*this exf is included in qso entry prompt*/
#define A_DUMMY 8       /*this one is only to be printed*/
#define A_DUPE 0x10     /*this one is to print on dupe check*/
#define A_MULTI 0x20 /*this one determines multipliers*/
#define A_NOHRD 0x40    /*this one is not to be printed in final copy*/
#define A_NOED 0x80     /* don't edit this one */
#define A_LOWER 0x100   /*Allow lower case letters */
#define A_TABONLY 0X200	/*except for TAB, any typed character keeps field current*/
#define A_OVERSTRIKE 0X400 /* overstrike mode rather than insert */
#define A_NOSPACE 0X800   /* Don't allow space characters*/
#define A_REQUIRED 0X1000   /* field is required at entry time */
#define A_NOTIFY   0X2000     /* net propogate changes in this field*/
#define A_RST      0X4000		/* this field is an RST */

			/* A field with A_REQUIRED without A_MULTI is simply required
			** to be non-empty for QSO entry.
			**
			** A_REQUIRED with A_MULTI generates a call to:
			**			IWlogMulti::MultiCheck(,, &ret,WLOG_MULTICHECK_NOWRT,0)
			** as a QSO is being entered from the Entry Window.
			** Entry is refused if the "ret" argument is zero.
			** It is also expected by the client that the QsoPtr_t NOT be
			** modified in this call to IWlogMulti::MultiCheck()
			**
			** A_MULTI also generates these calls:
			**		IWlogMulti::QsoRem() followed by IWlogMulti::QsoAdd()
			**		for an editing of that field in the QSO.
			**	
			**		IWlogMulti::MultiCheck(,, &ret, WLOG_MULTICHECK_MSGSET, Message)
			**			at many different times to check whether a QSO might
			**			be a new multiplier
			**	
			*/

#define Q_PDATE 1       /*arg to qso_print--print date*/
#define Q_HRDCP 2       /*arg to qso_print--print for hardcopy*/

/*
** arguments for mul_qedit when something in a QSO has been edited but
** is a permanent part of all QSOs and therefore has no "position"
** in ->var_part 
*/
#define EX_DUPE_POS -1

#ifndef PORT_H
#include "port.h"
#endif

#define MAX_EXF_WID 28  /*maximum width of any EXF*/
struct exfa_stru        /*argument passing convention between parent/child*/
	{
	char    *hdr;   /*header for this exf*/
	int             wid;    /*print width of this exf*/
	int             pl;             /*offset into qso structure*/
	int             at;             /*attributes*/
	};

typedef struct exfa_stru FAR* ExfPtr_t;
typedef const struct exfa_stru FAR* ConstExfPtr_t;

#endif
