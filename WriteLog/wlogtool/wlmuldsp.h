#ifndef WLMULDSP_H
#define WLMULDSP_H

#ifdef __cplusplus
#include "wl_guid.h"

DECLARE_INTERFACE_(IMultDisplayContainer, IUnknown)
{
    STDMETHOD(MakeDisplay)(short MultiBand,
							short FixedColumns,
							REFIID iid,
							IUnknown **)=0;
    STDMETHOD(ShowCurrent)(void)=0;
    STDMETHOD(InvalidateAll)(void)=0;
	STDMETHOD(TranslateAccelerator)(MSG FAR *, short)=0;
};

DECLARE_INTERFACE_(IMultDisplayPage, IUnknown)
{
    STDMETHOD(put_Title)(const char *Title)=0;
    STDMETHOD(put_MultCount)(short Count)=0;
    STDMETHOD(put_BandCount)(short Count)=0;
    STDMETHOD(put_LookupFcn)(IUnknown *)=0;
    STDMETHOD(put_ColumnCount)(short Count)=0;
    STDMETHOD(Invalidate)(short Mult)=0;
};

DECLARE_INTERFACE_(IMultDisplayEntry, IUnknown)
{
    STDMETHOD(get_MultWorked)(short Mult, short band)=0;
    STDMETHOD(get_MultTitle)(short Mult, const char **Title)=0;
    STDMETHOD(get_Column)(short Mult, short *Column)=0;
    STDMETHOD(get_ColumnTitle)(short Column, const char **Title)=0;
};


DEFINE_WL_GUID(IMultDisplayContainer,"C7212133-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IMultDisplayPage,"C7212131-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IMultDisplayEntry,"C7212132-7716-101A-AA54-00608C61D0B1");
#endif
#endif
