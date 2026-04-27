#pragma once

#if !defined(BUILDING_WLOGSH32)
#pragma comment(lib, "wlogsh32.lib")
#endif


#ifndef CONDEFS_H
#include "condefs.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern char * __stdcall qso_ptime(QsoPtr_t, char *);
extern char * __stdcall qso_arldate(QsoPtr_t, char *);
extern char * __stdcall qso_pdate(QsoPtr_t, char *);
extern char * __stdcall qso_months(int);
extern char * __stdcall qso_amode(QsoPtr_t);
extern char * __stdcall qso_aband(QsoPtr_t);
extern char * __stdcall qso_abndTitle(int);
extern int    __stdcall qso_abndindex(long);
#ifdef __cplusplus
	}
#endif


