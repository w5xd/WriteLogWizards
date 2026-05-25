/*
** Copyright (c) 1992, 1993 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
/*misc.h
*/
#pragma once

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

extern char * __stdcall cvt_char(char *, int , unsigned long, int);
extern unsigned long __stdcall cvt_long(const char *, int);
extern void __stdcall build_fname(char *, char *, char *);
extern void __stdcall set_path(char *, char *);
extern void __stdcall set_fsel(char *, char *);
extern void __stdcall pr_number(char *, char *, long, int, int);
extern int __stdcall SubString(const char *, const char *, int);
extern int __stdcall xl_makeStr( char *, const char *);
extern int __stdcall xl_makeInt( char *, int);
extern int __stdcall xl_makeDouble(char *, double);
extern int __stdcall xl_makeLong(char *, unsigned long, int);
extern int __stdcall xl_Header(char *, int , int );
extern int __stdcall wk1_makeStr(     char *, const char *,  int ,        int );
extern int __stdcall wk1_makeInt(     char *, int,   int ,        int );
extern int __stdcall wk1_makeDouble(char *, double, int, int);
extern int __stdcall wk1_makeLong(    char *, unsigned long , int,  int,  int );
extern int __stdcall wk1_Header(char *, int , unsigned);
extern void __stdcall SetChildWindowFonts(HWND Dialog, HFONT Font);
extern void __stdcall SetDriveAndDir(char *fpath);
#ifdef __cplusplus
}
#endif


