/*
** Copyright (c) 1992, 1993 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
#ifndef BCDARITH_H
#define BCDARITH_H

#ifndef WIN32
#ifndef FAR
#define FAR _far
#endif
#ifndef PASCAL
#define PASCAL _pascal
#endif
#else
//#define FAR
//#define PASCAL
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void FAR PASCAL incBCD(char *, int);
extern void FAR PASCAL decBCD(char *, int);
extern void FAR PASCAL zeroBCD(char *, int);
#define INCBCD(a) incBCD(a, sizeof(a) - 2)
#define DECBCD(a) decBCD(a, sizeof(a) - 2)
#define ZEROBCD(a) zeroBCD(a, sizeof(a) - 2);
#ifdef __cplusplus
	}
#endif
#endif


