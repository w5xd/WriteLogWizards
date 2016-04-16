#ifndef WLREGSTR_H
#define WLREGSTR_H

#ifdef __cplusplus
extern "C" {
#endif

	extern void FAR PASCAL wl_RegisterClass(HANDLE, REFCLSID, int);
	extern void FAR PASCAL wl_RemoveProgId(const char *);
	extern void FAR PASCAL wl_UnregisterClass(REFCLSID);

#ifdef __cplusplus
}
#endif
#endif    
