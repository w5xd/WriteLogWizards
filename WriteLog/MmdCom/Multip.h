/*
** Copyright (c) 1992, 1993 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
/*multip.h
**
*/
#ifndef MULTIP_H
#define MULTIP_H
#ifndef PORT_H
#include "port.h"
#endif

/* MACRO
**
** xprint
**
** DESCRIPTION
**
** Copy the ascii for a country into a QSO "COUNTRY" field.  Null terminate.
*/
#define xprint(a,b,c,d,e,f,g)   {strncpy((a)->var_part+(c), b, (d)-1);\
				(a)->var_part[(c) + (d) -1] = 0;\
		strncpy((a)->var_part+(f), e, (g)-1);\
				(a)->var_part[(f) + (g) -1] = 0; }
#define xprint2(a,b,c,d,e,f,g)   {strncpy((a)->var_part+(c), b, (d)-1);\
				(a)->var_part[(c) + (d) -1] = 0;\
				(a)->var_part[f] = 0;}

#endif
