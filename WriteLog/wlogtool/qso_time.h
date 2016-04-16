#ifndef QSO_TIME_H
#define QSO_TIME_H

#if 0
typedef long	QsoTimeUnpack_t;

#define QSO_TIME_UNPACK(x, y) *(x) = *(y)
#define QSO_TIME_ADDR(q) (&(q)->time)

#define QSO_YEAR(x) ((((*(x))>>25) & 0x7F) + 1980L)	//decimal years
#define QSO_MONTH(x) ((*(x)>>21) & 0xF)				//months-- 1 through 12
#define QSO_DAY(x)   ((*(x)>>16) & 0x1F)			//days  -- 1 through 31
#define QSO_HOUR(x)  ((*(x)>>11) & 0x1F)			// 0 through 23
#define QSO_MINUTE(x) ((*(x)>>5) & 0x3F)			// 0 through 59
#define QSO_SECOND(x) (((*(x)) & 0x1F) << 1)			// 0 through 59


#else
typedef SYSTEMTIME	QsoTimeUnpack_t;

#define QSO_TIME_UNPACK(x, y) FileTimeToSystemTime(y, x)
#define QSO_TIME_ADDR(q) (&(q)->m_time)

#define QSO_YEAR(x) ((x)->wYear)	//decimal years
#define QSO_MONTH(x) ((x)->wMonth)				//months-- 1 through 12
#define QSO_DAY(x)   ((x)->wDay)			//days  -- 1 through 31
#define QSO_HOUR(x)  ((x)->wHour)			// 0 through 23
#define QSO_MINUTE(x) ((x)->wMinute)			// 0 through 59
#define QSO_SECOND(x) ((x)->wSecond)			// 0 through 59

#endif

#endif