/*
** Copyright (c) 1992 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
#ifndef DXCCHOME_H
#define DXCCHOME_H
#ifndef DXPREF_H
#include "dxpref.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

extern int FAR PASCAL dxcc_Home(DxContext_t, HANDLE); // HANDLE is deprecated--can be zero
#ifdef __cplusplus
	}
inline int dxcc_Home(DxContext_t d) { return dxcc_Home(d, HANDLE(0)); } 
#endif

#endif
