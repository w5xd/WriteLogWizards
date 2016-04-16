/*
** Copyright (c) 1992, 1993 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
#include <stdio.h>
#include <string.h>
#include "arrlsub.h"
#include "misc.h"
#include "qso_util.h"


/* returns EOF on failure */
int WriteField(int Offset, int Length, FILE *f, QsoPtr_t q)
{
	int i;
	int RetVal = 0;
	i = strlen(q->var_part + Offset);
	if (i) RetVal = fputs(q->var_part + Offset, f);
	while (i < Length)
	{
		RetVal = fputc(' ', f);
		i += 1;
	}
	return RetVal;
}

/* returns EOF on failure */
int WriteRST(int Offset, int Length, FILE *f, QsoPtr_t q)
{
	int i;
	int RetVal = 0;
	i = strlen(q->var_part + Offset);
	if (i)
		RetVal = fputs(q->var_part + Offset, f);
	else
	{
		if (q->mode == '3' || q->mode == '6') /* CW or RTTY ? */
			fputs("599", f);
		else
			fputs("59 ", f);
		i = 3;	
	}
	while (i < Length)
	{
		RetVal = fputc(' ', f);
		i += 1;
	}
	return RetVal;
}


/* returns EOF on failure */
int WriteArrlCommon(FILE *ArrlFile, QsoPtr_t FirstQ)
{
	char    StringBuffer[100];
	char    *CurPos;
						fputs(qso_aband(FirstQ), ArrlFile);
						fputc(' ', ArrlFile);
						fputs(qso_amode(FirstQ), ArrlFile);
						CurPos = StringBuffer;
						*CurPos++ = ' ';
						CurPos = qso_arldate(FirstQ, CurPos);
						*CurPos++ = ' ';
						CurPos = qso_ptime(FirstQ, CurPos);
						*CurPos++ = ' ';
						CurPos = cvt_char(CurPos, 5, FirstQ->serial, 0);
						*CurPos++ = ' ';
						*CurPos++ = 0; 
						return fputs(StringBuffer, ArrlFile);
}

char * ListSections(int	NumSec, int *SectionArray, MultName NameFunction,
					char *CurPos, int MulPrintWidth, int NumPrintColumns,
					int		CompareSense,
					long User)
{
	int i, k, l, j;
	const char 	*SecName;
	j = 0;
	for (i = 0; i < NumSec; i += 1)
	{
		SecName = (*NameFunction)(i, User);
		if (!SecName)
			break;
		if (!CompareSense ^ !SectionArray[i])
		{
			for (k = l = 0; k < MulPrintWidth; k += 1)
			{
				if (SecName[l])
					*CurPos++ = SecName[l++];
				else
					*CurPos++ = ' ';
			}
			j += 1;
			if (j >= NumPrintColumns)
			{
				*CurPos++ =  '^'; *CurPos++ = 'p'; 
				j = 0;
			}
		}               
	}
	if (j != 0)
	{
		*CurPos++ = '^'; *CurPos++ = 'p';
	}
	*CurPos = 0;
	return CurPos;
}

int WriteArrlCall(QsoPtr_t q, FILE *ArrlFile)
{
	int i;
	int RetVal;
	i = strlen(q->stn);
	if (i)
		RetVal = fputs(q->stn, ArrlFile);
	while (i < 16)
		{RetVal = fputc(' ', ArrlFile); i += 1;}
	return RetVal;
}