#ifndef IWLOGBSM_H
#define IWLOGBSM_H

#ifdef __cplusplus
#include "wl_guid.h"

DECLARE_INTERFACE_(IWlogBandSumm, IUnknown)
{
	// *** IWlogBandSumm methods ***
   STDMETHOD(Reset)(THIS) PURE;
      //Reset to smallest size, and watch for new bands
   STDMETHOD(ZeroSummaries)(THIS) PURE;
      //Clears all the interal counters for a recount, but don't resize
   STDMETHOD(ContributeMultipliers)(THIS_ unsigned short BandIndex,
               short MultiplierCount,
               const short FAR *Multipliers) PURE;
      //Add the ints pointed to by Multipliers into the various columns
	STDMETHOD(SetBandTitle)(THIS_ unsigned short BandIndex, const char FAR *title) PURE;
      //Set the band title for the given index
   STDMETHOD(SetItemTitle)(THIS_ unsigned short ItemIndex, const char FAR *title) PURE;
		//any thing that gets summarized by band also has a title
   STDMETHOD(SetScore)(THIS_ long Score) PURE;
      //Score display
   STDMETHOD(GetEntry)(THIS_ short BandIndex,
                             short ItemIndex,
                             long FAR *value) PURE;
      //recover a value
	STDMETHOD(SetItemMultFlag)(THIS_ unsigned short ItemIndex, unsigned short flag) PURE;
	STDMETHOD(get_SoundEnabled)(THIS_ unsigned short *Enabled) PURE;
	STDMETHOD(put_SoundEnabled)(THIS_ unsigned short Enabled) PURE;
};
#endif

DEFINE_WL_GUID(IWlogBandSumm, "C721214A-7716-101A-AA54-00608C61D0B1");

#endif

