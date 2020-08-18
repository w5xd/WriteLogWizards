/*
** Copyright (c) 1992, 1993 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
/*freq.h
**Wayne E. Wright
**December 22, 1987
**Round Rock, Texas
**
**Define the data structures necessary to save tranceiver state
*
*/
#ifndef FREQ_H
#define FREQ_H

struct freq_stru 
	{
	long    xmit;   /*transmit frequency in units of 10Hz*/
	long    rcv;    /*receive frequency in units of 10Hz*/
	char    mode;   /*mode:
					** '1' LSB, '2' USB, '3' CW,
					** '4' FM , '5' AM , '6' FSK
					*/
	char    fcn;    /*function '0' is A '1' is B */
	char    split;  /*'0' is xcv, '1' split*/
#ifdef __cplusplus
    bool operator == (const freq_stru &rhs) const
    {
        return xmit == rhs.xmit && rcv == rhs.rcv &&
            mode == rhs.mode && mode == rhs.mode;
    }
    freq_stru & operator =(const freq_stru &o)
    {
        xmit = o.xmit;
        rcv = o.rcv;
        mode = o.mode;
        fcn = o.fcn;
        split = o.split;
        return *this;
    }
    static long long toLongLong(unsigned int f) // units of 10Hz
    {
        static const int Fboundary = 1100000000;
        static const long long Fscale = 100ll;
        if (f <= Fboundary)
            return f;
        return Fscale * (f - Fboundary);
    }
    static unsigned  fromLongLong(long long f) // units of 10Hz
    {
        static const int Fboundary = 1100000000;
        static const long long Fscale = 100ll;
        if (f <= Fboundary)
            return static_cast<unsigned>(f);
        else
            return static_cast<unsigned>(Fboundary + f/Fscale);
    }
#endif
	};
#endif


