/*
** Copyright (c) 1992, 1993 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
/*misc.h
*/

#ifndef MISC_H
#define MISC_H

#ifndef PORT_H
#include "port.h"
#endif

/* union
****************
**
** fpConvert
**
** DESCRIPTION
**
** This is used to write out floating point numbers in Lotus.  The floating
** point is assigned to the "fp" entry, and the "output" entry
*/
union fpConvert
	{
	double fp;
	unsigned char output[8];
	};


#ifdef __cplusplus
extern "C" {
#endif

extern char * FAR PASCAL cvt_char(char FAR *, int , unsigned long, int);
extern unsigned long FAR PASCAL cvt_long(const char FAR *, int);
extern void FAR PASCAL build_fname(char *, char *, char *);
extern void FAR PASCAL set_path(char *, char *);
extern void FAR PASCAL set_fsel(char *, char *);
extern void FAR PASCAL pr_number(char *, char *, long, int, int);
extern int FAR PASCAL SubString(const char *, const char *, int);
extern int FAR PASCAL xl_makeStr( char FAR *, const char FAR *);
extern int FAR PASCAL xl_makeInt( char FAR *, int);
extern int FAR PASCAL xl_makeDouble(char FAR *, double);
extern int FAR PASCAL xl_makeLong(char FAR *, unsigned long, int);
extern int FAR PASCAL xl_Header(char FAR *, int , int );
extern int FAR PASCAL wk1_makeStr(     char FAR *, const char FAR *,  int ,        int );
extern int FAR PASCAL wk1_makeInt(     char FAR *, int,   int ,        int );
extern int FAR PASCAL wk1_makeDouble(char FAR *, double, int, int);
extern int FAR PASCAL wk1_makeLong(    char FAR *, unsigned long , int,  int,  int );
extern int FAR PASCAL wk1_Header(char FAR *, int , unsigned);
extern void FAR PASCAL SetChildWindowFonts(HWND Dialog, HFONT Font);
extern void FAR PASCAL SetDriveAndDir(char FAR *fpath);
#ifdef __cplusplus
}
#endif

#endif
