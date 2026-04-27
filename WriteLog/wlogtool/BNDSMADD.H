#pragma once

#if !defined(BUILDING_WLOGSH32)
#pragma comment(lib, "wlogsh32.lib")
#endif

extern "C" void __stdcall wlshr_GetSumm(IUnknown  *Parent,
            IWlogBandSumm  *  *BndSum);


