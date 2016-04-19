#pragma once
#include "resource.h"       // main symbols

#include "[!output MIDL_H_FILENAME]"
#include "Iwritelg.h"
#include "iwlogbsm.h"
#include "QsoFieldMgr.h"

[!if !NO_DXCC||!NO_ZONE]
#include "dxcclist.h"
#include "dxpref.h"
#include "CountryLookupHelper.h"
[!endif]
[!if !NO_DXCC]
#include "MultDisplayEntryImpl.h"
[!endif]

// [!output MM_CLASS_NAME]

class ATL_NO_VTABLE [!output MM_CLASS_NAME] :
	public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
	public ATL::CComCoClass<[!output MM_CLASS_NAME], &CLSID_[!output COCLASS]>
    , public IWlogMulti
[!if CABRILLO]
    , public IWlogCabrillo
[!endif]
    , public IPersistStorage
{
public:
    // ctor/dtor
    [!output MM_CLASS_NAME]();
    ~[!output MM_CLASS_NAME]();

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

    enum {
[!if MULTI_MODE]
		BAND_SUMMARY_CW,
		BAND_SUMMARY_PHONE,
[!if RTTY]
		BAND_SUMMARY_RTTY,
[!endif]
		//TODO--if there are more modes....
[!else ]
		BAND_SUMMARY_QSO,
[!if PTS_COLUMN]
		BAND_SUMMARY_PTS,
[!endif ]
[!endif ]
[!if !NO_NAMEDMULT||!NO_DXCC||!NO_ZONE||!NO_AYGMULT]
		BAND_SUMMARY_MUL,
[!endif ]
		BAND_SUMMARY_WIDTH};

[!if MULTI_MODE]
    //TODO--If there are more modes
    enum ModesPerBandEnum {MULT_MODE_PHONE, MULT_MODE_CW,[!if RTTY] MULT_MODE_RTTY,[!endif] NUMBER_OF_MODES_PER_MULT_BAND};		
[!else]
    enum ModesPerBandEnum {NUMBER_OF_MODES_PER_MULT_BAND = 1};		
[!endif ]

public:
    // IWlogMulti Methods
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

[!if !NO_DXCC]
public: 
	// query multiplier worked
    HRESULT get_MultWorked(int id, short Mult, short band);
[!endif]
protected:
	// static contest data structure defintions
    static const struct exfa_stru g_Layout[];
[!if !ASK_MODE]
    static const struct band_stru g_Bands[];
[!else]
    static const struct band_stru g_BandsPh[];
    static const struct band_stru g_BandsCw[];
[!if RTTY]
    static const struct band_stru g_BandsRy[];
[!endif]
[!endif]
    static const unsigned gArchiveVersion;

	// connections to main WriteLog
    IWriteLog *m_Parent; // NOT REF COUNTED
    CComPtr<IWlogBandSumm> m_bandSumm;

    // *******************************************
    // Qso exchange field support ****************
    CQsoFieldMgr    m_qsoFields; // here to construct BEFORE CQsoField's below
	/* Each CQsoField MUST have an entry in enum ExfOrder_t in [!output CPP_FILE]
	** The converse is not true, but any entries in g_Layout without a 
	** corresponding CQsoField cannot be manipulated by this module. */
    CQsoCallField   fCALL;
[!if RST_IN_EXCHANGE]
    CQsoField    fSN;
    CQsoField    fRS;
[!endif]
[!if NR_IN_EXCHANGE]
    CQsoField    fNR;
[!endif]
[!if !NO_NAMEDMULT]
    CQsoField    fRCVD;
[!endif]
[!if !NO_ZONE]
    CQsoField    fZN;
[!endif]
[!if !NO_AYGMULT]
    CQsoField    fAYG;
[!endif]
[!if !NO_DXCC]
    CQsoField    fCOUNTRY;
    CQsoField    fAMBF;
    CQsoField    fCPRF;
[!endif]
[!if !NO_NAMEDMULT]
    CQsoField    fMLT;
[!endif]
[!if !NO_DXCC]
    CQsoField    fCMULT;
[!endif]
[!if !NO_ZONE]
    CQsoField    fZMULT;
[!endif]
[!if !NO_AYGMULT]
    CQsoField    fAYGMULT;
[!endif]
[!if PTS_COLUMN]
    CQsoField    fPTS;
[!endif]
    // *******************************************
[!if ASK_MODE]
enum {	// otherwise identical single-mode contests
    ASK_MODE_CW, ASK_MODE_PH
    [!if RTTY]
    , ASK_MODE_RY
    [!endif]
    } m_ModeSelected;
[!endif]

    /************
    Multiplier support
    ************/
[!if !NO_NAMEDMULT||!NO_DXCC||!NO_ZONE||!NO_AYGMULT]
	CComPtr<IMultDisplayContainer>	m_MultDispContainer;
    enum {
[!if !NO_DXCC]
            DXCC_MULT_ID,
[!endif]
    };
[!endif]
[!if !NO_DXCC]

	//DXCC Multiplier
	CCountryLookupHelper			m_DxContext;
    int								m_MyCountryIndex;
	CDxDispContainerHelper<[!output MM_CLASS_NAME],
        CDxccDisplayHelper<[!output MM_CLASS_NAME], DXCC_MULT_ID> >
        m_DxccContainer;
[!if DXCC_SINGLE_BAND]
    typedef std::map<short, int> Countries_t;
	Countries_t	 m_Countries;
	int          m_DxccMults;
[!endif]
[!if DXCC_MULTI_BAND]
    typedef std::map<short, std::map<short, int> > Countries_t;
	Countries_t	m_Countries;
    typedef std::map<short, int> DxccMults_t;
	DxccMults_t	m_DxccMults;
[!endif]
[!endif]
    /************
    End Multiplier support
    ************/

    int						m_PageQsos;
    int						m_PageQsoPoints;
    int						m_PageMultipliers;
    int                     m_NumberOfDupeSheetBands;

	// helper functions
	// The DupeBand's are the frequency+mode ranges where you can work the station
    int DupeBandToMultBand(int band) {
        return band / static_cast<int>(NUMBER_OF_MODES_PER_MULT_BAND);
    }
	// The MultBand's are the frequency+mode ranges where they count as a mult 
    int  NumberOfMultBands() const {
        return m_NumberOfDupeSheetBands * static_cast<int>(NUMBER_OF_MODES_PER_MULT_BAND);
    }
    QsoPtr_t GetQsoIth(unsigned long Index) const
        {
            QsoPtr_t TestQ;
            if (m_Parent && Index >= 0)
                m_Parent->QsoIth(Index, &TestQ);
            else
                TestQ = 0;
            return TestQ;
        }

};

OBJECT_ENTRY_AUTO(__uuidof([!output COCLASS]), [!output MM_CLASS_NAME])
