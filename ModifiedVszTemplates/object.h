#pragma once
#include <vector>
[!if CAN_LOG_ROVER]
#include <functional>
#include <set>
[!endif]
#include "resource.h"       // main symbols

#include "[!output MIDL_H_FILENAME]"
/* Iwritelg.h documents the interfaces that contest modules
** can optionally implement. The wizard does not generate code for all of
** them. */
#include "Iwritelg.h"
#include "iwlogbsm.h"
#include "QsoFieldMgr.h"

[!if !NO_DXCC||!NO_ZONE]
#include "dxcclist.h"
#include "dxpref.h"
#include "CountryLookupHelper.h"
[!endif]
[!if !NO_DXCC || !NO_ZONE || !NO_AYGMULT || !NO_NAMEDMULT]
#include "MultDisplayEntryImpl.h"
#include "MultiplierMap.h"
[!endif]

[!if AM_ROVER]
#include "[!output MM_ROVERDLG_CLASS_FILENAME]"
[!endif]

// [!output MM_CLASS_NAME]

class ATL_NO_VTABLE [!output MM_CLASS_NAME] :
	public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
	public ATL::CComCoClass<[!output MM_CLASS_NAME], &CLSID_[!output COCLASS]>
[!if RTTY]
    , public IWlogMulti2
[!else]
    , public IWlogMulti
[!endif]
    , public IWlogScoreInfo
[!if CABRILLO]
    , public IWlogCabrillo
[!if MULTIPLE_NAMED_IN_QSO && !NO_NAMEDMULT]
    , public IWlogCabrilloFileIterator
[!endif]
[!endif]
[!if TQSL_ROVER]
    , public IWlogTQslQsoLocation
[!endif]
    , public IPersistStorage
{
[!if !NO_DXCC || !NO_ZONE || !NO_AYGMULT || !NO_NAMEDMULT]
    typedef CMultiplierMap MultiplierMap_t;
[!endif]
public:
    // ctor/dtor
    [!output MM_CLASS_NAME]();
    ~[!output MM_CLASS_NAME]();

    DECLARE_REGISTRY_RESOURCEID([!output RGS_ID])
    DECLARE_NOT_AGGREGATABLE([!output MM_CLASS_NAME])

    BEGIN_COM_MAP([!output MM_CLASS_NAME])
        COM_INTERFACE_ENTRY(IWlogMulti)
[!if RTTY]
        COM_INTERFACE_ENTRY(IWlogMulti2)
[!endif]
        COM_INTERFACE_ENTRY(IWlogScoreInfo)
[!if CABRILLO]
        COM_INTERFACE_ENTRY(IWlogCabrillo)
[!if MULTIPLE_NAMED_IN_QSO && !NO_NAMEDMULT]
        COM_INTERFACE_ENTRY(IWlogCabrilloFileIterator)
[!endif]
[!endif]
[!if TQSL_ROVER]
        COM_INTERFACE_ENTRY(IWlogTQslQsoLocation)
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
[!if !MULTI_MODE && PTS_COLUMN]
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
[!endif]

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

	//IWlogScoreInfo--rate display support
	STDMETHOD(MultsAndPts)(long *pMultCount, long *pPtsCount);
	STDMETHOD(PtsForQso)(QsoPtr_t q, long *pPtsCount);

[!if RTTY]
    // IWlogMulti2  -- for RTTY
    STDMETHOD(WhatsTheBestField)(QsoPtr_t q, const char *s, short *Offset);
    STDMETHOD(IsCharOKHere)(QsoPtr_t q, char c, short Offset);

[!endif]
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
[!if MULTIPLE_NAMED_IN_QSO]
    // Inherited via IWlogCabrilloFileIterator
    friend class [!output MM_CABRILLOITERATOR_CLASS_NAME];
    virtual HRESULT __stdcall GetIterator(IWlogCabrillo * *pIterator) override;
[!endif]

[!endif]
[!if TQSL_ROVER]
    // IWlogTQslQsoLocation
    STDMETHOD(GetNumLocationFields)(long* pNumfields);
    STDMETHOD(GetFieldGABBIName)(long Numfield, char* pFieldName, long FieldNameSize);
    STDMETHOD(GetQsoNumLocations)(QsoPtr_t q, long* pNumLocations);
    STDMETHOD(GetQsoLocationField)(QsoPtr_t q, long Location, long FieldNum,
        char* pFieldvalue, long FieldValueSize);

[!endif]
[!if !NO_NAMEDMULT]
[!if NAMED_MULTI_REGION]
    enum { REGION_NAME_FIXME1, REGION_NAME_FIXME2, NUMBER_OF_REGIONS };
[!else]
    enum { REGION_NAME_FIXME1, NUMBER_OF_REGIONS = 1 };
[!endif]
[!endif]
[!if !NO_NAMEDMULT || !NO_DXCC || !NO_ZONE || !NO_AYGMULT]
[!if NAMEDMULT_SINGLE_BAND]
[!if NAMED_MULTI_REGION]
    typedef CMultiplierMapByRegion NamedMult_t;
[!else]
    typedef CMultiplierMap2 NamedMult_t;
[!endif]
[!endif]
[!if NAMEDMULT_MULTI_BAND]
[!if NAMED_MULTI_REGION]
    typedef CMultiplierMapByRegionByBand NamedMult_t;
[!else]
    typedef CMultiplierMapByBand2 NamedMult_t;
[!endif]
[!endif]
[!if AYGMULT_MULTI_BAND]
    typedef CMultiplierMapByBand AygStatus_t;
[!endif]
[!if AYGMULT_SINGLE_BAND]
    typedef MultiplierMap_t AygStatus_t;
[!endif]
[!if ZONE_MULTI_BAND]
    typedef CMultiplierMapByBand Zones_t;
[!endif]
[!if ZONE_SINGLE_BAND]
    typedef MultiplierMap_t Zones_t;
[!endif]
[!if DXCC_SINGLE_BAND]
    typedef MultiplierMap_t Countries_t;
[!endif]
[!if DXCC_MULTI_BAND]
    typedef CMultiplierMapByBand Countries_t;
 [!endif]
    enum {  // ID's to distinguish the IMultDisplayEntry implementations
         [!if !NO_DXCC]DXCC_MULT_ID,[!endif][!if !NO_ZONE]ZONE_MULT_ID,[!endif][!if !NO_NAMEDMULT] NAMED_MULT_ID,[!endif]
         [!if NAMED_MULTI_REGION]LAST_NAMED_ID=NAMED_MULT_ID+NUMBER_OF_REGIONS-1,[!endif]
         [!if !NO_AYGMULT] AYG_MULT_ID,[!endif]
    };
    HRESULT get_MultWorked(int id, short Mult, short band);
// class CDupeSheet. ********************************************
[!if AM_ROVER]
// We support roving. There is one CDupeSheet per roving QTH.
[!else]
// Only one dupe sheet
[!endif]
class CDupeSheet {
public:
    CDupeSheet()
    {}
    void InitQsoData();
protected:
	std::string					    m_MyMult;
public:
    bool needInit() const { return m_MyMult.empty();}
    const std::string &title() const { return m_MyMult;}
    const std::string &key() const { return m_MyMult;}
    void setKey(const std::string &k) {m_MyMult = k;}
    unsigned totalQsos() const;
[!if !NO_NAMEDMULT]
protected:
	[!output MM_CLASS_NAME]::NamedMult_t		            m_Named;		//Number of times we've worked each multiplier
public:
    int MarkNamedMultWorked(int nm, int band, short region) { return m_Named.mark(nm, band, region);}
    int UnmarkNamedMultWorked(int nm, int band, short region){ return m_Named.unmark(nm, band, region); }
    bool namedWorked(short nm, short band, short region) const{ return m_Named.worked(nm, band, region); }
[!if NAMEDMULT_SINGLE_BAND]
    const [!output MM_CLASS_NAME]::NamedMult_t::MapMultToWorked_t& getWorked(short region) const { return m_Named.worked(region); }
[!endif]
[!if MULTI_MODE]
    void OnWorkedBandModeQso(short band, short mode, int inc) { m_bandModeQsos[band][mode] += inc;}
    int WorkedBandModeQso(short band, short mode) const;
protected:
    std::map<short, std::map<short, int>> m_bandModeQsos;
[!endif]
[!endif]
[!if !NO_DXCC]
public:
    int MarkCountryWorked(int dx, int band);
    int UnmarkCountryWorked(int dx, int band);
    bool countryWorked(short dx, short band) const;
protected:
	[!output MM_CLASS_NAME]::Countries_t	                m_Countries;
[!endif]
[!if !NO_ZONE]
public:
    int MarkZoneWorked(int zn, int band);
    int UnmarkZoneWorked(int zn, int band);
    bool zoneWorked(short zn, short band) const;
protected:
	[!output MM_CLASS_NAME]::Zones_t   	 	                m_Zones;
[!endif]
[!if !NO_AYGMULT]
public:
    int MarkAygMultWorked(int ayg, int band);
    int UnmarkAygMultWorked(int ayg, int band);
    bool aygWorked(short ayg, short band) const;
protected:
	[!output MM_CLASS_NAME]::AygStatus_t		            m_AygStatus;
[!endif]
};
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

[!if !NO_DXCC || !NO_ZONE || !NO_NAMEDMULT ||!NO_AYGMULT]
public: 
	// query multiplier worked
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
    IWriteLog                     *m_Parent; // NOT REF COUNTED
    CComPtr<IWlogBandSumm>         m_bandSumm;

    // *******************************************
    // Qso exchange field support ****************
    CQsoFieldMgr                   m_qsoFields; // here to construct BEFORE CQsoField's below
	/* Each CQsoField MUST have an entry in enum ExfOrder_t in [!output CPP_FILE]
	** The converse is not true, but any entries in g_Layout without a 
	** corresponding CQsoField cannot be manipulated by this module. */
    CQsoCallField                  fCALL;
[!if RST_IN_EXCHANGE]
    CQsoField                      fSN;
    CQsoField                      fRS;
[!endif]
[!if NR_IN_EXCHANGE]
    CQsoField                      fNR;
[!endif]
[!if PTS_COLUMN]
    CQsoField                      fPTS;
[!endif]
    // *******************************************
[!if ASK_MODE]
enum {	// identical rules, but different modes on different weekends
    ASK_MODE_CW, ASK_MODE_PH [!if RTTY], ASK_MODE_RY[!endif]

    }                              m_ModeSelected;
[!endif]

[!if !NO_NAMEDMULT||!NO_DXCC||!NO_ZONE||!NO_AYGMULT]
    /************
    Multiplier support
    ************/
	CComPtr<IMultDisplayContainer>	m_MultDispContainer;
 [!if !AM_ROVER]
    CDupeSheet             m_dupeSheet;
    CDupeSheet &currentDupeSheet() { return m_dupeSheet; }
    CDupeSheet &dupeSheetFromQso(QsoPtr_t) { return m_dupeSheet;}
[!else]
    std::vector<std::shared_ptr<CDupeSheet>> m_dupeSheets;
    CDupeSheet &currentDupeSheet() { return *m_dupeSheets[m_currentDupeSheet].get(); }
    CDupeSheet &dupeSheetFromQso(QsoPtr_t);
    unsigned m_currentDupeSheet;

[!endif]
[!endif]
[!if !NO_NAMEDMULT]

	//Named multiplier support...
    CQsoField                       fRCVD;
    CQsoField                       fMLT;

    void SetupNamedDisplay(short region, const char* title, IMultDisplayContainer*, IWlNamedMult*, int numMultBands);
    struct NamedMultsPage {
	    short							m_NumNamed;
	    CComPtr<IWlNamedMult>			m_pNamedMults;
	    CComPtr<IMultDisplayPage>		m_NamedDisplay;
	    CComPtr<IMultDisplayEntry>		m_NamedDisplayEntry;
[!if NAMEDMULT_MULTI_BAND]
	    [!output MM_CLASS_NAME]::MultiplierMap_t		            m_NamedMults;
        void clear() { m_NamedMults.clear(); }
        unsigned total() const {
            unsigned ret(0);
            for (auto m : m_NamedMults)
                ret += m.second;
            return ret;
        }
[!endif]
[!if NAMEDMULT_SINGLE_BAND]
	    int	    m_NamedMults;
        void clear() { m_NamedMults = 0; }
        unsigned total() const { return m_NamedMults;}
[!endif]
[!if !NAMED_MULTI_REGION]
        NamedMultsPage & operator[] (int) { return *this;}
        size_t size() const { return 1; }
[!endif]
        NamedMultsPage(int = 0)
            : m_NumNamed(0)
[!if NAMEDMULT_SINGLE_BAND]
            , m_NamedMults(0)
[!endif]
        {}
    };
[!if !NAMED_MULTI_REGION]
    NamedMultsPage m_namedMults;
[!else]
    std::vector<NamedMultsPage> m_namedMults;
[!endif]
[!if MULTIPLE_NAMED_IN_QSO]
    typedef std::set<short> Named_t;
[!else]
    typedef int Named_t;
[!endif]
    Named_t FindNamed(short region, const char *c);
    bool validNamed(short region, const Named_t &);
    // end named

[!endif]
[!if !NO_DXCC]
	//DXCC Multiplier
	CCountryLookupHelper			m_DxContext;
    int								m_MyCountryIndex;
    CQsoField                       fCOUNTRY;
    CQsoField                       fAMBF;
    CQsoField                       fCPRF;
    CQsoField                       fCMULT;
    CDxDispContainerHelper<[!output MM_CLASS_NAME],
        CDxccDisplayHelper<[!output MM_CLASS_NAME], DXCC_MULT_ID> >
        m_DxccContainer;
[!if DXCC_SINGLE_BAND]
	int                             m_DxccMults;
[!endif]
[!if DXCC_MULTI_BAND]
    typedef MultiplierMap_t DxccMults_t;
	DxccMults_t	                    m_DxccMults;
[!endif]
    // end DXCC

[!endif]
[!if !NO_ZONE]
	//Zone multiplier support
    enum { NUMZONES = 40 };    // TODO
	CCountryLookupHelper			m_ZoneContext;
    CQsoField                       fZN;
    CQsoField                       fZMULT;
    void SetupZoneDisplay(const char *title, IMultDisplayContainer*,int numMultBands);
	CComPtr<IMultDisplayPage>		m_ZoneDisplay;
	CComPtr<IMultDisplayEntry>		m_ZoneDisplayEntry;
[!if ZONE_MULTI_BAND]
    typedef MultiplierMap_t ZoneMults_t;
	ZoneMults_t				        m_ZoneMults;
[!endif]
[!if ZONE_SINGLE_BAND]
	int				                m_ZoneMults;
[!endif]
	static int FindZone(const char *c);
	//end Zone

[!endif]
[!if !NO_AYGMULT]
	//As You Go (AYG) multiplier support
    typedef std::map<std::string, short> AygNames_t;
	AygNames_t	                    m_AygDisplayNames; 
    std::vector<AygNames_t::iterator>   m_AygIdxToName;
    CQsoField                       fAYG;
    CQsoField                       fAYGMULT;
    void SetupAygDisplay(const char *title, IMultDisplayContainer*,int numMultBands);
	CComPtr<IMultDisplayPage>		m_AygDisplay;
	CComPtr<CAygDisplayHelper<[!output MM_CLASS_NAME], AYG_MULT_ID> >   
                                    m_AygDisplayEntry;
[!if AYGMULT_MULTI_BAND]
	MultiplierMap_t		            m_AygMults;
[!endif]
[!if AYGMULT_SINGLE_BAND]
	int		                        m_AygMults;
[!endif]
	int		FindAyg(int, const char *c);

public:
    int     AygCount() const {        return m_AygDisplayNames.size();    }
    HRESULT get_MultTitle(int ID, short Mult, const char **Title);
    // end AYG

[!endif]
[!if AM_ROVER]
    CQsoField                       fMYQTH;
[!endif]
    /************
    End Multiplier support
    ************/

[!if AM_COUNTYLINE]
    char GetCountyLineMode() { return m_countyLineMode ? 1 : 0; }
    void SetCountyLineMode(char v) { m_countyLineMode = (v != 0); }
[!endif]

protected:
    std::string                     m_MyCallsign;
    std::map<short, int>		    m_BandPoints;
	std::map<short, int>		    m_BandQsos;
    int						        m_PageQsos;
    int						        m_PageQsoPoints;
    int						        m_PageMultipliers;
    int                             m_NumberOfDupeSheetBands; // count of g_Bands entries at File/New time.

    // Methods
    long PointsForQso(QsoPtr_t q);
    long Score();

	// other helper functions
	// The DupeBand's are the frequency+mode ranges where you can work the station
    int DupeBandToMultBand(int band) {
        return band / static_cast<int>(NUMBER_OF_MODES_PER_MULT_BAND);
    }
	// The MultBand's are the frequency+mode ranges where they count as a mult 
    int  NumberOfMultBands() const {
        return m_NumberOfDupeSheetBands / static_cast<int>(NUMBER_OF_MODES_PER_MULT_BAND);
    }
    QsoPtr_t GetQsoIth(unsigned long Index) const {
        QsoPtr_t TestQ;
        if (m_Parent && Index >= 0)
            m_Parent->QsoIth(Index, &TestQ);
        else
            TestQ = 0;
        return TestQ;
    }
[!if AM_ROVER]
    void InvokeQthSelectDlg();
[!endif]
[!if CAN_LOG_ROVER]
    typedef std::function<void(QsoPtr_t, int*)> QsoSearchMatch_t;
    QsoSearchMatch_t m_qsoSearchMatch;
    class CQsoSearchMatchHelper {
    public:
        CQsoSearchMatchHelper(QsoSearchMatch_t &toUpdate, const QsoSearchMatch_t &useInScope) 
            : toRestore(toUpdate), stored(toUpdate)
        {  toUpdate = useInScope;   }
        ~CQsoSearchMatchHelper()
        { toRestore = stored;  }
    protected:
        QsoSearchMatch_t stored;
        QsoSearchMatch_t &toRestore;
    };
    // take into account previously logged rover will have multiple QTHs in our log.
    std::set<std::string> FindAllPreviousValuesForThisCall(unsigned MaxSize, CQsoField &field, QsoPtr_t q);
[!endif]
[!if AM_COUNTYLINE]
    bool m_countyLineMode;
[!endif]
};

OBJECT_ENTRY_AUTO(__uuidof([!output COCLASS]), [!output MM_CLASS_NAME])
