#ifndef WLMULDSP_H
#define WLMULDSP_H

#ifdef __cplusplus
#include "wl_guid.h"

namespace wlmuldsp {
    namespace gridloc {
        const short DISPLAY_FLAG = static_cast<short>(18 * 18 * 10 * 10); // Flag for MakeDisplay
        const short NUM_BANDS = 12; // number of bands the GRID_DISPLAY_FLAG has on it.
    }
}

DECLARE_INTERFACE_(IMultDisplayContainer, IUnknown)
{
    STDMETHOD(MakeDisplay)(short MultiBand,
							short FixedColumns,
							REFIID iid,
							IUnknown **)=0;
    /* Special case for MakeDisplay:
    ** MultiBand = 1 and FixedColumns = wlmuldsp::gridloc::DISPLAY_FLAG
    **   .. gives a GridLocator display that (a) is scrollable and (b) only
    **   allows one such Page on a Display
    */
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

DECLARE_INTERFACE_(IMultDisplayPage2, IMultDisplayPage)
{
    STDMETHOD(put_BandColorOffset)(short Band)=0;
    STDMETHOD(put_DefaultScrollMultPos)(short Mult) = 0;
};

DECLARE_INTERFACE_(IMultDisplayPageOptions, IUnknown)
{
    STDMETHOD(put_Sort)(unsigned short sort) = 0;
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
DEFINE_WL_GUID(IMultDisplayPage2,"C7212136-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IMultDisplayEntry,"C7212132-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IMultDisplayPageOptions, "C7212101-7716-101A-AA54-00608C61D0B1");
#endif
#endif
