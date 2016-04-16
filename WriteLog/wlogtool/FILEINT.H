/*
** Copyright (c) 1992, 1993 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
/* fileint.h
**
*/
/* portability typedefs for ints written to disk files */
#ifndef FILEINT_H
#define FILEINT_H

typedef short     int16;
typedef unsigned short unsigned16;
typedef long    int32;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
	}
#endif


typedef enum
	{
	FIELD_INT,
	FIELD_CHAR,
	FIELD_DUMMY
	}       FieldEnum;

typedef const struct
	{
	int     Size;           /* width of structure */
	char    *Offset;        /* offset into structure. -1 means end of desc*/
	char    *Base;
	FieldEnum       FieldType;
	}
	FieldDesc;


#endif
