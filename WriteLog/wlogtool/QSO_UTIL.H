#ifndef QSO_UTIL_H
#define QSO_UTIL_H

#ifndef CONDEFS_H
#include "condefs.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern char * WINAPI qso_ptime(QsoPtr_t, char *);
extern char * WINAPI qso_arldate(QsoPtr_t, char *);
extern char * WINAPI qso_pdate(QsoPtr_t, char *);
extern char * WINAPI qso_months(int);
extern char * WINAPI qso_amode(QsoPtr_t);
extern char * WINAPI qso_aband(QsoPtr_t);
extern char * WINAPI qso_abndTitle(int);
extern int    WINAPI qso_abndindex(long);
#ifdef __cplusplus
	}
#endif

#endif
