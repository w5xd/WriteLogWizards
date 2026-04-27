#pragma once
#if !defined(BUILDING_WLOGSH32)
#pragma comment(lib, "wlogsh32.lib")
#endif
#ifdef __cplusplus
extern "C" {
#endif

typedef struct LogFontWithPointSize_tag {
	LOGFONTA lf;
	unsigned pointsizeX10;
} LogFontWithPointSize;

#ifdef WIN32
	typedef long WindowSize_t;
#else
	typedef int WindowSize_t;
#endif
extern void  __stdcall   GetWindowFromProfile(const char *,
             WindowSize_t *,  WindowSize_t *,  WindowSize_t *,  WindowSize_t *,int);
extern void  __stdcall   SetWindowToProfile(const char *, int, int, int, int);
extern int __stdcall RestoreFontFromProfile(LOGFONTA *, const char *);
extern int __stdcall RestoreFontFromProfile2(LogFontWithPointSize*,  const char *);
extern void __stdcall SaveFontToProfile(const LOGFONTA *lf,     const char *Section);
extern void __stdcall SaveFontToProfile2(const LogFontWithPointSize*lf, const char *);
extern int __stdcall GetProfileNumber(const char *Section, const char *Entry);
extern void __stdcall SetProfileNumber(const char *Section, const char *Entry, int number);
extern int __stdcall wl_GetDpiForHWND(HWND);

#ifndef WM_DPICHANGED_BEFOREPARENT // only defined in later Windows SDKs. But we dynamically handle it if it appears in a message queue
#define WM_DPICHANGED_BEFOREPARENT 0x02E2
#endif

#ifdef __cplusplus
	}
#endif


