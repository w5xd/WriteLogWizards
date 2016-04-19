#include "stdafx.h"
#include "iwritelg.h"
#include "[!output HEADER_FILE]"
#include "Exfsym.h"
#include "bndsmadd.h"
#include "bandsym.h" 
[!if ASK_MODE]
#include "AskModeDlg.h"
[!endif]

#define DIM(x) (sizeof(x) / sizeof(x[0])

extern HINSTANCE g_hInstance;

// [!output MM_CLASS_NAME]
namespace {
    const int CALC_POS_LATER = -1;
    const int CALL_POS = -1;
	/* TODO change the xyz_WID, as needed.
	** Serialization support allows changing
	** them even after a File/Save of old
	** widths. */
[!if RST_IN_EXCHANGE]
    const int RST_WID = 4;
[!endif]
[!if NR_IN_EXCHANGE]
    const int NR_WID = 5; // TODO
[!endif]
[!if !NO_NAMEDMULT]
    const int   RCVD_WID =	5;// TODO
    const int   MLT_WID  =   3;// TODO
[!endif]
[!if !NO_ZONE]
    const int ZN_WID = 3;
    const int ZMULT_WID = 3;
[!endif]
[!if !NO_AYGMULT]
    const int AYG_WID = 3;// TODO
    const int AYGMULT_WID = 3;// TODO
[!endif]
[!if !NO_DXCC]
    const int  COUNTRY_WID = 15;
    const int  AMBF_WID = 2;
    const int  CPRF_WID = 6;
    const int  CMULT_WID = 3;
[!endif]
[!if PTS_COLUMN]
    const int PTS_WID = 2; // TODO
[!endif]
}

// order of the enum MUST match that in g_Layout (below)
enum ExfOrder_t { CALL_IDX, 
[!if RST_IN_EXCHANGE]
    SNT_IDX, RST_IDX,
[!endif]
[!if NR_IN_EXCHANGE]
    NR_IDX,
[!endif]
[!if !NO_NAMEDMULT]
    RCVD_IDX,
[!endif]
[!if !NO_ZONE]
    ZN_IDX,
[!endif]
[!if !NO_AYGMULT]
	AYG_IDX,
[!endif]
[!if !NO_DXCC]
    COUNTRY_IDX, COUNTRY_C_IDX, PREF_IDX,
[!endif]
[!if !NO_NAMEDMULT]
	NAMEDMULT_IDX,
[!endif]
[!if !NO_DXCC]
	COUNTRY_CM_IDX,
[!endif]
[!if !NO_ZONE]
	ZM_IDX,
[!endif]
[!if !NO_AYGMULT]
	AYG_M_IDX,
[!endif]
[!if PTS_COLUMN]
	PTS_IDX,
[!endif]    
    };

//TODO: Reorder, add/delete, etc.
// EVERY ROW HERE MUST HAVE A CORRESDPONDING ENTRY IN enum ExfOrder_t
const struct exfa_stru [!output MM_CLASS_NAME]::g_Layout[] =	
{
    { "CALL", CALL_WID, CALL_POS, A_CALL | A_PRMPT | A_MULTI | A_NOTIFY },
[!if RST_IN_EXCHANGE]
    { "SNT", RST_WID, CALC_POS_LATER, A_NUMS | A_NOTIFY | A_RST | A_OVERSTRIKE },
    { "RST", RST_WID, CALC_POS_LATER, A_PRMPT | A_NUMS | A_NOTIFY | A_RST | A_OVERSTRIKE },
[!endif]
[!if NR_IN_EXCHANGE]
    { "NR", NR_WID, CALC_POS_LATER, A_NUMS | A_NOTIFY | A_PRMPT | A_TABONLY | A_REQUIRED |
            A_DUPE | A_OVERSTRIKE },
[!endif]
[!if !NO_NAMEDMULT]
    {"RCVD",          RCVD_WID, CALC_POS_LATER, A_DUPE|A_PRMPT| A_NOSPACE |
                                    A_MULTI|A_REQUIRED|A_NOTIFY|A_OVERSTRIKE}, 
[!endif]
[!if !NO_ZONE]
    {"ZN",          ZN_WID, CALC_POS_LATER, A_DUPE|A_NUMS|A_PRMPT|
                                    A_MULTI|A_REQUIRED|A_NOTIFY}, 
[!endif]
[!if !NO_AYGMULT]
	{"A?",			AYG_WID, CALC_POS_LATER, A_DUPE|A_PRMPT|A_MULTI|A_REQUIRED|
										A_NOTIFY},	//TODO: check name, size and flags
[!endif]
[!if !NO_DXCC]
    {"COUNTRY",     COUNTRY_WID, CALC_POS_LATER,
                                    A_PRMPT|A_LOWER|A_NOSPACE},
    {"C",   AMBF_WID, CALC_POS_LATER,     
			A_PRMPT|A_MULTI|A_NOHRD|A_TABONLY|A_NOTIFY|A_NOSPACE},
    {"PREF",        CPRF_WID, CALC_POS_LATER, A_MULTI|A_NOTIFY},
[!endif]
[!if !NO_NAMEDMULT]
	{"ML", MLT_WID, CALC_POS_LATER, A_NOED},
[!endif]
[!if !NO_DXCC]
	{"CM", CMULT_WID, CALC_POS_LATER, A_NOED},
[!endif]
[!if !NO_ZONE]
	{"ZM", ZMULT_WID, CALC_POS_LATER, A_NOED},
[!endif]
[!if !NO_AYGMULT]
	{"?M",	AYGMULT_WID, CALC_POS_LATER, A_NOED},	//todo: NAME OF COLUMN
[!endif]
[!if PTS_COLUMN]
	{"P", PTS_WID, CALC_POS_LATER, A_NOED},
[!endif]
    { 0 }
 };

// ctor
[!output MM_CLASS_NAME]::[!output MM_CLASS_NAME]()
    : m_Parent(0)
    , m_PageQsos(0)
    , m_PageQsoPoints(0)
    , m_PageMultipliers(0)
    , fCALL(m_qsoFields, CALL_IDX)
[!if RST_IN_EXCHANGE]
    , fSN(m_qsoFields, SNT_IDX)
    , fRS(m_qsoFields, RST_IDX)
[!endif]
[!if NR_IN_EXCHANGE]
    , fNR(m_qsoFields, NR_IDX)
[!endif]
[!if !NO_NAMEDMULT]
    , fRCVD(m_qsoFields,RCVD_IDX)
[!endif]
[!if !NO_ZONE]
    , fZN(m_qsoFields,ZN_IDX)
[!endif]
[!if !NO_AYGMULT]
    , fAYG(m_qsoFields,AYG_IDX)
[!endif]
[!if !NO_DXCC]
    , fCOUNTRY(m_qsoFields, COUNTRY_IDX)
    , fAMBF(m_qsoFields, COUNTRY_C_IDX)
    , fCPRF(m_qsoFields, PREF_IDX)
[!endif]
[!if !NO_NAMEDMULT]
    , fMULT(m_qsoFields, NAMEDMULT_IDX)
[!endif]
[!if !NO_DXCC]
    , fCMULT(m_qsoFields, COUNTRY_CM_IDX)
[!endif]
[!if !NO_ZONE]
    , fZMULT(m_qsoFields, ZM_IDX)
[!endif]
[!if !NO_AYGMULT]
    , fAYGMULT(m_qsoFields, AYG_M_IDX)
[!endif]
[!if PTS_COLUMN]
    , fPTS(m_qsoFields, PTS_IDX)
[!endif]
[!if ASK_MODE]
    , m_ModeSelected(ASK_MODE_CW)
[!endif]
[!if DXCC_SINGLE_BAND]
    , m_DxccMults(0)
[!endif]
    , m_NumberOfDupeSheetBands(0)
{}

// dtor
[!output MM_CLASS_NAME]::~[!output MM_CLASS_NAME]()
{}

HRESULT [!output MM_CLASS_NAME]::FinalConstruct()
{
    m_qsoFields.StaticInit(g_Layout);
[!if !NO_DXCC]
    HRESULT hr = m_DxContext.Init("DXCCDOS.DAT");
	/*TODO:  DXCCDOS.DAT is DXCC country list
	**       DXCCWAE.DAT is CQWW country list
	**       CQZONE.DAT maps CALL to CQ zone
	**       ITUZONE.DAT maps CALL to ITU zone.	*/
    if (FAILED(hr))
        return hr;
[!endif]
    return S_OK;
}

void [!output MM_CLASS_NAME]::FinalRelease()
{
    m_bandSumm.Release();
[!if !NO_DXCC]
    m_DxContext.Release();
[!endif]
}

// IWlogMulti Methods
HRESULT [!output MM_CLASS_NAME]::GetLayout(ConstBandPtr_t * b, ConstExfPtr_t * e, LPCSTR * s)
{
    if (e)
        *e = m_qsoFields.GetLayout();
    if (b)
    {
[!if ASK_MODE]
        CAskModeDlg dlg;
        dlg.DoModal();
        if (dlg.m_mode == IDC_MODESELCW)
        {
            *b = g_BandsCw;
            m_ModeSelected = ASK_MODE_CW;
        }
        else
        {
            *b = g_BandsPh;
            m_ModeSelected = ASK_MODE_PH;
        }
[!else]
        * b = g_Bands;
[!endif]
        m_NumberOfDupeSheetBands = 0;
        for (ConstBandPtr_t bandCountPtr = *b;
            bandCountPtr->low >= 0;
            bandCountPtr += 1)
            m_NumberOfDupeSheetBands += 1;
    }
    if (s)
        *s = "Custom";  
    return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::QsoAdd(QsoPtr_t q)
{
	int points = 0;
    short   Mult[BAND_SUMMARY_WIDTH];
	memset(Mult, 0, sizeof(Mult));
    int band = q->band; // q->band is always a Dupe Band
    if (band > m_NumberOfDupeSheetBands)
        band = m_NumberOfDupeSheetBands;
	band = DupeBandToMultBand(band);

[!if !NO_NAMEDMULT||!NO_DXCC||!NO_ZONE||!NO_AYGMULT]
	int i;
[!endif]
[!if !NO_DXCC]
    int  countryIndex = -1, newDxccFlag = 0;
[!endif]
[!if !NODXCC]
    fCMULT(q) = "";
[!endif]
[!if PTS_COLUMN]
    fPTS(q) = "";
[!endif]

    if (q->dupe == ' ')
    {
[!if !NO_DXCC]
        //*********************************
        //Process DXCC 
        i = m_DxContext.FillQso(q, fCALL, fCPRF, fCOUNTRY, fAMBF);
        if (i >= 0)
        {
            countryIndex = i;
[!if DXCC_SINGLE_BAND]
            Countries_t::iterator itor = m_Countries.find(countryIndex);
            if (itor == m_Countries.end())
                itor = m_Countries.insert(Countries_t::value_type(countryIndex, 0)).first;
            newDxccFlag = !itor->second++;
[!endif]
[!if DXCC_MULTI_BAND]
            Countries_t::iterator itor1 = m_Countries.find(band);
            if (itor1 == m_Countries.end())
                itor1 = m_Countries.insert(Countries_t::value_type(band, std::map<short,int>())).first;
            std::map<short, int>::iterator itor2 = itor1->second.find(countryIndex);
            if (itor2 == itor1->second.end())
                itor2 = itor1->second.insert(std::map<short, int>::value_type(countryIndex, 0)).first;
            newDxccFlag = !itor2->second++;
[!endif]
            if (newDxccFlag)
            {
	            char Buf[16];
[!if DXCC_SINGLE_BAND]
	            _itoa_s(++m_DxccMults,  Buf,  10);
[!endif]
[!if DXCC_MULTI_BAND]
	            _itoa_s(++m_DxccMults[band],  Buf,  10);
[!endif]
                fCMULT(q) = Buf;
                Mult[BAND_SUMMARY_MUL] += 1;
                m_DxccContainer.InvalidateCountry(countryIndex);
            }
        }
[!endif]
    }

[!if ASK_MODE]
    if (q->dupe == ' ')
    {   // record mode based on a qso
        if (q->mode == '3')
            m_ModeSelected = ASK_MODE_CW;
[!if RTTY]
        else if (q->mode == '6')
            m_ModeSelected = ASK_MODE_RY;
[!endif]
        else
            m_ModeSelected = ASK_MODE_PH;
    }
[!endif]
    return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::QsoRem(QsoPtr_t q)
{
	int points = 0;
    short   Mult[BAND_SUMMARY_WIDTH];
	memset(Mult, 0, sizeof(Mult));
    int band = q->band;
    if (band > m_NumberOfDupeSheetBands)
        band = m_NumberOfDupeSheetBands;
	band = DupeBandToMultBand(band);

[!if !NO_NAMEDMULT||!NO_DXCC||!NO_ZONE||!NO_AYGMULT]
	int i;
[!endif]
[!if !NO_DXCC]
    int countryIndex = -1, newDxccFlag = 0;
[!endif]

    if (q->dupe == ' ')
    {
[!if !NO_DXCC]
		i = m_DxContext.CountryFromQsoPrefix(q, fCPRF);
        if (i >= 0)
		{
			countryIndex = i;
[!if DXCC_SINGLE_BAND]
            Countries_t::iterator itor = m_Countries.find(countryIndex);
            if (itor != m_Countries.end())
                newDxccFlag = ! --itor->second;
[!endif]
[!if DXCC_MULTI_BAND]
            Countries_t::iterator itor1 = m_Countries.find(band);
            if (itor1 != m_Countries.end())
            {
                std::map<short, int>::iterator itor2 = itor1->second.find(countryIndex);
                if (itor2 != itor1->second.end())
                    newDxccFlag = ! --itor2->second;
            }
[!endif]
			if (newDxccFlag)
			{
[!if DXCC_SINGLE_BAND]
				m_DxccMults -= 1;
[!endif]
[!if DXCC_MULTI_BAND]
				m_DxccMults[band] -= 1;
[!endif]
				Mult[BAND_SUMMARY_MUL] -= 1;
				m_DxccContainer.InvalidateCountry(countryIndex);
			}
		}
[!endif]
    }
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::InitQsoData()
{
[!if !NO_DXCC]
    m_MyCountryIndex = m_DxContext.dxcc_Home();
    m_Countries.clear();
[!if DXCC_MULTI_BAND]
	m_DxccMults.clear();
[!else]
    m_DxccMults = 0;
[!endif]
[!endif]
    return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::MultiCheck(QsoPtr_t q, int p, int * Result, long RequestMask, char * Message)
{
	int ret = -1;
    int band = q->band;
    if (band > m_NumberOfDupeSheetBands)
        band = m_NumberOfDupeSheetBands;
    band = DupeBandToMultBand(band);

	QsoPtr_t OldQ = 0;
	if (!(RequestMask & WLOG_MULTICHECK_NOWRT))
	{
		//locate any previous QSO with this station...
		unsigned long QsoNumber;
		for (int j = 0; j < m_NumberOfDupeSheetBands; j += 1)
		{
			if (m_Parent->SearchDupeSheet(q, j, 
						0,
						&QsoNumber) == S_OK)
			{
				OldQ = GetQsoIth(QsoNumber);
				break;
			}
		}
	}
    
[!if !NO_DXCC]
	//DXCC country processing
    if ((fCALL == p) ||  /*call sign*/
        (fAMBF == p))    /*or country tag*/
    {
	    bool ambig(false);
		bool NewCountry(false);
        int i = m_DxContext.CheckQso(q, !(RequestMask & WLOG_MULTICHECK_NOWRT),
            fCALL, fCOUNTRY, fCPRF, fAMBF, ambig);
		if (!ambig)
		{
            NewCountry = get_MultWorked(DXCC_MULT_ID, i, band) == S_FALSE ? 1 : 0;
            if (ret <= 0)
                ret = NewCountry ? 1 : 0;
		}
        if (NewCountry && (RequestMask == WLOG_MULTICHECK_MSGSET))
            ::LoadString(g_hInstance, IDS_NEWCOUNTRY_MSG, Message, MAX_MULT_MESSAGE_LENGTH);
	}

[!endif]    
    return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::Display(HWND Window)
{
[!if !NO_NAMEDMULT||!NO_DXCC||!NO_ZONE||!NO_AYGMULT]
    if (!m_MultDispContainer)
    {
        if (SUCCEEDED(m_MultDispContainer.CoCreateInstance(
            OLESTR("writelog.multdisp"), 0, CLSCTX_ALL)))
		{
[!if !NO_NAMEDMULT]
            m_MultDispContainer->MakeDisplay(1, 0,
                            __uuidof(IMultDisplayPage),
                            (IUnknown **)&m_NamedDisplay);
[!endif]
[!if !NO_ZONE]
            m_MultDispContainer->MakeDisplay(1, 0,
                            __uuidof(IMultDisplayPage),
                            (IUnknown **)&m_ZoneDisplay);
[!endif]
[!if !NO_AYGMULT]
			m_MultDispContainer->MakeDisplay(1, 0,
							__uuidof(IMultDisplayPage),
							(IUnknown **)&m_AygDisplay);
[!endif]
[!if !NO_DXCC]
            m_DxccContainer.MakeDxccDisplays(m_MultDispContainer, 
[!if DXCC_MULTI_BAND]
												1,
[!endif]
[!if DXCC_SINGLE_BAND]
												0,
[!endif]
												0);
[!endif]
        }
    }
[!if !NO_NAMEDMULT]
    if (m_NamedDisplay)
	{
		if (!m_NamedDisplayEntry)
		{
			m_NamedDisplay->put_Title("TODO");
			m_NamedDisplayEntry = 
[!if NAMEDMULT_MULTI_BAND]
					new $$MM_CLASS_NAME$$NamedDispEntry(
								m_pNamedMults,
								m_NamedDisplay,
								NumberOfMultBands(),
								this);
[!endif]
[!if NAMEDMULT_SINGLE_BAND]
                    new CNamedMultDisplaySingleBand(
                                m_sctCntArr,
                                m_pNamedMults,
                                m_NamedDisplay);
[!endif]
		}
	}

[!endif]
[!if !NO_ZONE]
	if (m_ZoneDisplay)
    {
        if (!m_ZoneDisplayEntry)
        {
            m_ZoneDisplay->put_Title("TODO");
[!if ZONE_MULTI_BAND]
            m_ZoneDisplayEntry = 
                    new $$MM_CLASS_NAME$$ZoneDispEntry(
                                g_ZoneList,
                                DIM(g_ZoneList),
                                m_ZoneDisplay,
								NumberOfMultBands(),
								this);
[!endif]
[!if ZONE_SINGLE_BAND]
            m_ZoneDisplayEntry = 
                    new $$MM_CLASS_NAME$$ZoneDispEntry(
                                g_ZoneList,
                                DIM(g_ZoneList),
                                m_ZoneDisplay,
								1,
								this);
[!endif]
        }
    }
[!endif]
[!if !NO_AYGMULT]
	if (m_AygDisplay)
	{
		if (!m_AygDisplayEntry)
		{
			m_AygDisplay->put_Title("TODO");
			m_AygDisplayEntry = new
				$$MM_CLASS_NAME$$AygDispEntry(this,
[!if AYGMULT_MULTI_BAND]
								NumberOfMultBands(),
[!endif]
[!if AYGMULT_SINGLE_BAND]
								1,
[!endif]
								m_AygDisplay);
		}
	}

[!endif]
[!if !NO_DXCC]
    m_DxccContainer.InitializeEntry(m_DxContext.cList());
[!if DXCC_MULTI_BAND]
	m_DxccContainer.SetMults(this, NumberOfMultBands());
[!endif]
[!if DXCC_SINGLE_BAND]
	m_DxccContainer.SetMults(this);
[!endif]
[!endif]
    if (m_MultDispContainer)
        m_MultDispContainer->ShowCurrent();
[!endif]
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::Score(Configuration_Entry_t * Config, HWND Window, unsigned QsoNum, const char * TargetDir)
{
    // Add your function implementation here.
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::GetModuleData(long * Data)
{
    *Data = MODULE_DATA_NOBSM;
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::SetMMParent(IWriteLog *p)
{
    m_Parent = p;
    m_bandSumm.Release();
    wlshr_GetSumm(p, &m_bandSumm);
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::TranslateAccelerator(MSG *, short)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::MatchedQso(QsoPtr_t New, QsoPtr_t Old)
{
    // Add your function implementation here.
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::QsoSearch(QsoPtr_t NewQ, QsoPtr_t OldQ, int * IsGood)
{
    // Add your function implementation here.
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::DupeSheetTitle(int DupeSheet, char * Title, int TitleLength)
{
    // Add your function implementation here.
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::TallyPrintQso(QsoPtr_t q)
{
    int Mul=0, Pts = 0;
    if (q->dupe == ' ')
    {
        m_PageQsos += 1;
[!if !NO_DXCC]
		if (isdigit(*fCMULT(q)))
			Mul += 1;
[!endif]
        m_PageMultipliers += Mul;
        m_PageQsoPoints += Pts;
    }
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::FormatPageSumm(char * Buf, int BufLength)
{
    // Add your function implementation here.
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::SetDupeSheet(QsoPtr_t q, int * DupeSheet)
{
    // Add your function implementation here.
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::GetAdifName(long, long, char * pName)
{
    *pName = 0;
    return S_FALSE;
}

[!if !NO_DXCC]
// multipliers
HRESULT [!output MM_CLASS_NAME]::get_MultWorked(int id, short Mult, short band)
{
    switch (id)
    {
[!if DXCC_SINGLE_BAND]
    case DXCC_MULT_ID:
        {
            std::map<short, int>::iterator itor = m_Countries.find(Mult);
            if (itor == m_Countries.end())
                return S_FALSE;
            return itor->second != 0 ? S_OK : S_FALSE;
        }
[!endif]
[!if DXCC_MULTI_BAND]
    case DXCC_MULT_ID:
        {
            std::map<short, std::map<short, int> >::iterator itor1 = m_Countries.find(band);
            if (itor1 == m_Countries.end())
                return S_FALSE;
            std::map<short, int>::iterator itor2 = itor1->second.find(Mult);
            if (itor2 == itor1->second.end())
                return S_FALSE;
            return itor2->second != 0 ? S_OK : S_FALSE;
        }
[!endif]
    default:
        break;
    }
    return E_INVALIDARG;
}
[!endif]


[!if CABRILLO]
// IWlogCabrillo Methods
HRESULT [!output MM_CLASS_NAME]::ConfirmFieldsFilled(HWND w)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::SetCallsign(const char * Call)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::GetHeaderLineCount(short * pLines)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::GetHeaderLine(short LineNumber, char * Buf)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::GetContestName(char * Buf)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::GetClaimedScore(long * Score)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::GetTxFieldCount(short * pCount)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::FormatTxField(QsoPtr_t q, short Field, char * Buf)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::GetRxFieldCount(short * pCount)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::FormatRxField(QsoPtr_t q, short Field, char * Buf)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
[!endif]

//IPersist
HRESULT [!output MM_CLASS_NAME]::GetClassID(CLSID *pClassID)  { *pClassID = __uuidof([!output COCLASS]);  return S_OK; }

static const OLECHAR StreamName[] = L"[!output COCLASS]";
const unsigned [!output MM_CLASS_NAME]::gArchiveVersion = 1;

// IPersistStorage

HRESULT [!output MM_CLASS_NAME]::Load(IStorage *p)  {
    CComPtr<IStream> pStream;
    HRESULT  hr = p->OpenStream(StreamName, 0,
        STGM_DIRECT | STGM_READ | STGM_SHARE_EXCLUSIVE,
        0, &pStream);
    if (SUCCEEDED(hr))
        hr = m_qsoFields.Load(pStream);
    if (SUCCEEDED(hr) && m_qsoFields.VersionLoaded() > gArchiveVersion)
        hr = E_UNEXPECTED; // Loaded a version higher than we know of.
    ULONG bytesread;
#if _MSC_VER >= 1800
    static_assert(sizeof(m_NumberOfDupeSheetBands) == 4, "use 4 byte type");
#endif
    if (SUCCEEDED(hr))
        hr = pStream->Read(&m_NumberOfDupeSheetBands, sizeof(m_NumberOfDupeSheetBands), &bytesread);
    return hr; 
}

HRESULT [!output MM_CLASS_NAME]::Save(IStorage *p, BOOL fSameAsLoad)  {
    CComPtr<IStream> pStream;
    HRESULT hr = p->CreateStream(StreamName,
        STGM_DIRECT | STGM_READWRITE |
        STGM_CREATE | STGM_SHARE_EXCLUSIVE, 0, 0, &pStream);
    if (SUCCEEDED(hr))
        hr = m_qsoFields.Save(gArchiveVersion, pStream);
    ULONG written;
    if (SUCCEEDED(hr))
        hr = pStream->Write(&m_NumberOfDupeSheetBands, sizeof(m_NumberOfDupeSheetBands), &written);
    if (SUCCEEDED(hr))
        hr = p->SetClass(__uuidof([!output COCLASS]));
    if (SUCCEEDED(hr))
        hr = p->Commit(STGC_DEFAULT);
    return hr;
}

// unused
HRESULT [!output MM_CLASS_NAME]::SaveCompleted(IStorage *pStgNew)  { return S_OK; }
HRESULT [!output MM_CLASS_NAME]::HandsOffStorage(void)  { return S_OK; }
HRESULT [!output MM_CLASS_NAME]::IsDirty(void)  { return S_OK; }
HRESULT [!output MM_CLASS_NAME]::InitNew(IStorage *pStg)  { return S_OK; }

[!if !ASK_MODE]
const struct band_stru 
[!output MM_CLASS_NAME]::g_Bands[] =	//todo
[!if !MULTI_MODE]
[!if !RTTY]
    {
        { 180000, 200000, CW_MASK | PHONE_MASK, "160M"},
        { 350000, 400000, CW_MASK | PHONE_MASK, "80M" },
        { 700000, 730000, CW_MASK | PHONE_MASK, "40M" },
        { 1400000, 1435000, CW_MASK | PHONE_MASK, "20M" },
        { 2100000, 2145000, CW_MASK | PHONE_MASK, "15M" },
        { 2800000, 2970000, CW_MASK | PHONE_MASK | FM_MASK | AM_MASK, "10M" },
        { 5000000, 5400000, CW_MASK | PHONE_MASK | FM_MASK | AM_MASK, "6M" },
        { 14400000, 14800000, CW_MASK | PHONE_MASK | FM_MASK | AM_MASK, "2M" },
        { -1, -1, -1 }
    };
[!else]
    {
        { 350000,  400000, FSK_MASK, "80M"},
        { 700000,  730000, FSK_MASK, "40M"},
        {1400000, 1435000, FSK_MASK, "20M"},
        {2100000, 2145000, FSK_MASK, "15M"},
        {2800000, 2970000, FSK_MASK, "10M"},
        {-1, -1, -1}
    };
[!endif]
[!else]
    {   // The ordering of the modes here must match ModesPerBandEnum
        // TODO: which bands for this contest?
        { 180000,  200000, PHONE_MASK, "160M PH"},
        { 180000,  200000, CW_MASK, "160M CW"},
[!if RTTY]
        { 180000,  200000, FSK_MASK, "160M RY"},
[!endif]
        { 350000,  400000, PHONE_MASK, "80M PH"},
        { 350000,  400000, CW_MASK, "80M CW"},
[!if RTTY]
        { 350000,  400000, FSK_MASK, "80M RY"},
[!endif]
        { 700000,  730000, PHONE_MASK, "40M PH"},
        { 700000,  730000, CW_MASK, "40M CW"},
[!if RTTY]
        { 700000,  730000, FSK_MASK, "40M RY"},
[!endif]
        {1400000, 1435000, PHONE_MASK, "20M PH"},
        {1400000, 1435000, CW_MASK, "20M CW"},
[!if RTTY]
        {1400000, 1435000, FSK_MASK, "20M RY"},
[!endif]
        {2100000, 2145000, PHONE_MASK, "15M PH"},
        {2100000, 2145000, CW_MASK, "15M CW"},
[!if RTTY]
        {2100000, 2145000, FSK_MASK, "15M RY"},
[!endif]
        {2800000, 2970000, PHONE_MASK|FM_MASK|AM_MASK, "10M PH"},
        {2800000, 2970000, CW_MASK, "10M CW"},
[!if RTTY]
        {2800000, 2970000, FSK_MASK, "10M RY"},
[!endif]
		{5000000, 5400000, PHONE_MASK|FM_MASK|AM_MASK, "6M PH"},
		{5000000, 5400000, CW_MASK, "6M CW"},
[!if RTTY]
        {5000000, 5400000, FSK_MASK, "6M RY"},
[!endif]
		{14400000,14800000, PHONE_MASK|FM_MASK|AM_MASK, "2M PH"},
		{14400000,14800000, CW_MASK, "2M CW"},
[!if RTTY]
        {14400000,14800000, FSK_MASK, "2M RY"},
[!endif]
        {-1, -1, -1}
    };
[!endif]
[!else]
const struct band_stru
[!output MM_CLASS_NAME]::g_BandsPh[] =	//todo
    {
        { 180000,  200000, PHONE_MASK, "160M"},
        { 350000,  400000, PHONE_MASK, "80M"},
        { 700000,  730000, PHONE_MASK, "40M"},
        {1400000, 1435000, PHONE_MASK, "20M"},
        {2100000, 2145000, PHONE_MASK, "15M"},
        {2800000, 2970000, PHONE_MASK|FM_MASK|AM_MASK, "10M"},
		{5000000, 5400000, PHONE_MASK|FM_MASK|AM_MASK, "6M"},
		{14400000,14800000, PHONE_MASK|FM_MASK|AM_MASK, "2M"},
        {-1, -1, -1}
    };
const struct band_stru
[!output MM_CLASS_NAME]::g_BandsCw[] =	//todo
    {
        { 180000,  200000, CW_MASK, "160M"},
        { 350000,  400000, CW_MASK, "80M"},
        { 700000,  730000, CW_MASK, "40M"},
        {1400000, 1435000, CW_MASK, "20M"},
        {2100000, 2145000, CW_MASK, "15M"},
        {2800000, 2970000, CW_MASK, "10M"},
		{5000000, 5400000, CW_MASK, "6M"},
		{14400000,14800000, CW_MASK, "2M"},
        {-1, -1, -1}
    };
[!if RTTY]
const struct band_stru
[!output MM_CLASS_NAME]::g_BandsRy[] =	//todo
{
        { 350000,  400000, FSK_MASK, "80M"},
        { 700000,  730000, FSK_MASK, "40M"},
        {1400000, 1435000, FSK_MASK, "20M"},
        {2100000, 2145000, FSK_MASK, "15M"},
        {2800000, 2970000, FSK_MASK, "10M"},
        {-1, -1, -1}
};
[!endif]
[!endif]