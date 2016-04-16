/*
** Copyright (c) 1992 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
/*dxpref.h*/

#ifndef DXPREF_H
#define DXPREF_H

#ifndef DXCCLIST_H
#include "dxcclist.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DxContext *DxContext_t;

extern DxContext_t FAR PASCAL pref_init(const char *Fname);
extern void FAR PASCAL pref_free(DxContext_t);
extern void FAR PASCAL pref_calc(DxContext_t, char *,	const char *);
extern int FAR PASCAL pref_calc2(DxContext_t, char *,  const char *);
extern int FAR PASCAL pref_find(DxContext_t, const char *);
extern int FAR PASCAL pref_reso(DxContext_t, int,	char,	int *);
extern int FAR PASCAL pref_cvt(DxContext_t, const char *);
extern void FAR PASCAL pref_ambig(DxContext_t, int, char *);
extern struct country_stru *  FAR PASCAL pref_list(DxContext_t);
extern struct country_location * FAR PASCAL pref_locate(DxContext_t);

#ifdef __cplusplus
	}
#endif

#endif

