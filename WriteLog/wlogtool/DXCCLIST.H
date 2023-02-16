/*
** Copyright (c) 1992 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
/*dxcclist.h
**Wayne E. Wright
**1805 Mayfield
**Round Rock, Texas
**October 3, 1988
**
**table that maps call sign prefix to DXCC country.
*/

#ifndef DXCCLIST_H
#define DXCCLIST_H
struct country_stru
	{
	char *usual_prefix;
	char *country;	/*ascii description--null terminate*/
	char continent[4];	/*ascii--null terminate*/
#ifdef __cplusplus
	country_stru() : usual_prefix(0), country(0) 
	{memset(&continent, 0, sizeof(continent));}
#endif
	};

struct country_location
{
    float Latitude;         // >0 is north
    float Longitude;        // >0 is West
    float UtcOffset;        // >0 is West
#ifdef __cplusplus
	country_location() : Latitude(0), Longitude(0), UtcOffset(0){}
#endif
};
#endif
