/*
** Copyright (c) 1992, 1993 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
#ifndef ARRLSUB_H
#define ARRLSUB_H

#ifndef CONDEFS_H
#include "condefs.h"
#endif
typedef	const char * (*MultName)(int, long);
#ifdef __cplusplus
extern "C" {
#endif

extern int WriteField(int, int, FILE *, QsoPtr_t);
extern int WriteRST(int, int, FILE *, QsoPtr_t);
extern int WriteArrlCommon(FILE *, QsoPtr_t);
extern char * ListSections(int	NumSec, int *SectionArray, MultName NameFunction,
					char *CurPos, int MulPrintWidth, int NumPrintColumns,
					int CompareSense, long User);
extern int WriteArrlCall(QsoPtr_t q, FILE *ArrlFile);

#ifdef __cplusplus
}
#endif

#endif         
