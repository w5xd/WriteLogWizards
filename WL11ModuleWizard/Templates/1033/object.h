#pragma once
#include "resource.h"       // main symbols

#include "[!output MIDL_H_FILENAME]"
#include "Iwritelg.h"
#include "iwlogbsm.h"
#include "QsoFieldMgr.h"

using namespace ATL;

// [!output MM_CLASS_NAME]

class ATL_NO_VTABLE [!output MM_CLASS_NAME] :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<[!output MM_CLASS_NAME], &CLSID_[!output COCLASS]>
    , public IWlogMulti
[!if CABRILLO]
    , public IWlogCabrillo
[!endif]
    , public IPersistStorage
{
public:
    [!output MM_CLASS_NAME]();

    DECLARE_REGISTRY_RESOURCEID([!output RGS_ID])

    DECLARE_NOT_AGGREGATABLE([!output MM_CLASS_NAME])

    BEGIN_COM_MAP([!output MM_CLASS_NAME])
        COM_INTERFACE_ENTRY(IWlogMulti)
[!if CABRILLO]
        COM_INTERFACE_ENTRY(IWlogCabrillo)
[!endif]
        COM_INTERFACE_ENTRY(IPersist)
        COM_INTERFACE_ENTRY(IPersistStorage)
    END_COM_MAP()
    
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct();
    void FinalRelease();

    // IWlogMulti Methods
public:
    STDMETHOD(GetLayout)(ConstBandPtr_t * b, ConstExfPtr_t * e, LPCSTR * s);
    STDMETHOD(QsoAdd)(QsoPtr_t q);
    STDMETHOD(QsoRem)(QsoPtr_t q);
    STDMETHOD(InitQsoData)();
    STDMETHOD(MultiCheck)(QsoPtr_t q, int Offset, int * Result, long RequestMask, char * Message);
    STDMETHOD(Display)(HWND Window);
    STDMETHOD(Score)(Configuration_Entry_t * Config, HWND Window, unsigned QsoNum, const char * TargetDir);
    STDMETHOD(GetModuleData)(long * Data);
    STDMETHOD(SetMMParent)(IWriteLog *);
    STDMETHOD(TranslateAccelerator)(MSG *, short);
    STDMETHOD(MatchedQso)(QsoPtr_t New, QsoPtr_t Old);
    STDMETHOD(QsoSearch)(QsoPtr_t NewQ, QsoPtr_t OldQ, int * IsGood);
    STDMETHOD(DupeSheetTitle)(int DupeSheet, char * Title, int TitleLength);
    STDMETHOD(TallyPrintQso)(QsoPtr_t q);
    STDMETHOD(FormatPageSumm)(char * Buf, int BufLength);
    STDMETHOD(SetDupeSheet)(QsoPtr_t q, int * DupeSheet);
    STDMETHOD(GetAdifName)(long, long, char * pName);

[!if CABRILLO]
    // IWlogCabrillo Methods
public:
    STDMETHOD(ConfirmFieldsFilled)(HWND w);
    STDMETHOD(SetCallsign)(const char * Call);
    STDMETHOD(GetHeaderLineCount)(short * pLines);
    STDMETHOD(GetHeaderLine)(short LineNumber, char * Buf);
    STDMETHOD(GetContestName)(char * Buf);
    STDMETHOD(GetClaimedScore)(long * Score);
    STDMETHOD(GetTxFieldCount)(short * pCount);
    STDMETHOD(FormatTxField)(QsoPtr_t q, short Field, char * Buf);
    STDMETHOD(GetRxFieldCount)(short * pCount);
    STDMETHOD(FormatRxField)(QsoPtr_t q, short Field, char * Buf);
[!endif]

public:
    //IPersist
    STDMETHOD(GetClassID)(CLSID *pClassID);
    // IPersistStorage
    STDMETHOD(IsDirty)(void);
    STDMETHOD(InitNew)(IStorage *pStg);
    STDMETHOD(Load)(IStorage *pStg);
    STDMETHOD(Save)(IStorage *pStgSave, BOOL fSameAsLoad);
    STDMETHOD(SaveCompleted)(IStorage *pStgNew);
    STDMETHOD(HandsOffStorage)(void);

protected:
    static const struct exfa_stru g_Layout[];
    static const struct band_stru g_BandsPh[];
    static const struct band_stru g_BandsCw[];
    static const unsigned gArchiveVersion;
    IWriteLog *m_Parent; // NOT REF COUNTED
    CComPtr<IWlogBandSumm> m_bandSumm;
    CQsoFieldMgr    m_qsoFields; // must construct before CQsoField's
    // order of the enum must match that in Layout
    enum { CALL_IDX, SNT_IDX, RST_IDX, NR_IDX };
    CQsoCallField   CALL;
    CQsoField    SNT;
    CQsoField    RST;
    CQsoField    NR;
};

OBJECT_ENTRY_AUTO(__uuidof([!output COCLASS]), [!output MM_CLASS_NAME])
