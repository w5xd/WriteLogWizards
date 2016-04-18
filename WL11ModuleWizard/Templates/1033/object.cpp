#include "stdafx.h"
#include "iwritelg.h"
#include "[!output HEADER_FILE]"
#include "Exfsym.h"
#include "bndsmadd.h"
#include "bandsym.h" 
[!if ASK_MODE]
#include "AskModeDlg.h"
[!endif]

// [!output MM_CLASS_NAME]
namespace {
    const int CALC_POS_LATER = -1;
    const int CALL_POS = -1;
[!if RST_IN_EXCHANGE]
    const int RST_WID = 4;
[!endif]
[!if NR_IN_EXCHANGE]
    const int NR_WID = 5;
[!endif]
[!if PTS_COLUMN]
    const int PTS_WID = 2;
[!endif]
}

// order of the enum MUST match that in g_Layout
    enum { CALL_IDX, 
[!if RST_IN_EXCHANGE]
        SNT_IDX, RST_IDX, 
[!endif]
[!if NR_IN_EXCHANGE]
        NR_IDX
[!endif]
[!if PTS_COLUMN]
        PTS_IDX
[!endif]
    };

const struct exfa_stru [!output MM_CLASS_NAME]::g_Layout[] =	//todo, Reorder, add/delete, etc.
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
    , CALL(m_qsoFields, CALL_IDX)
[!if RST_IN_EXCHANGE]
    , SNT(m_qsoFields, SNT_IDX)
    , RST(m_qsoFields, RST_IDX)
[!endif]
[!if NR_IN_EXCHANGE]
    , NR(m_qsoFields, NR_IDX)
[!endif]
[!if PTS_COLUMN]
    , PTS(m_qsoFields, PTS_IDX)
[!endif]
[!if ASK_MODE]
    , m_ModeSelected(ASK_MODE_CW)
[!endif]
[!if DXCC_SINGLE_BAND]
    , m_DxccMults(0)
[!endif]
[!if !NO_DXCC]
    , m_DxContext(0)
[!endif]
    , m_NumberOfMultBands(0)
{}

// dtor
[!output MM_CLASS_NAME]::~[!output MM_CLASS_NAME]()
{}


HRESULT [!output MM_CLASS_NAME]::FinalConstruct()
{
    m_qsoFields.StaticInit(g_Layout);
    return S_OK;
}

void [!output MM_CLASS_NAME]::FinalRelease()
{
    m_bandSumm.Release();
[!if !NO_DXCC]
    if (m_DxContext)
        pref_free(m_DxContext);
    m_DxContext = 0;
[!endif]
}

// IWlogMulti Methods
HRESULT [!output MM_CLASS_NAME]::GetLayout(ConstBandPtr_t * b, ConstExfPtr_t * e, LPCSTR * s)
{
    // Add your function implementation here.
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
        m_NumberOfMultBands = 0;
        for (ConstBandPtr_t bandCountPtr = g_Bands;
            bandCountPtr->low >= 0;
            bandCountPtr += 1)
            m_NumberOfMultBands += 1;
[!if MULTI_MODE]
        m_NumberOfMultBands /= static_cast<int>(NUMBER_OF_MODES_PER_MULT_BAND);
[!endif]
    }
    if (s)
        *s = "Custom";  
    return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::QsoAdd(QsoPtr_t q)
{
    // Add your function implementation here.
[!if PTS_COLUMN]
    PTS(q) = "";
[!endif]
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
    // Add your function implementation here.
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::InitQsoData()
{
    // Add your function implementation here.
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::MultiCheck(QsoPtr_t q, int Offset, int * Result, long RequestMask, char * Message)
{
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
								m_NumberOfMultBands,
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
								m_NumberOfMultBands,
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
								m_NumberOfMultBands,
[!endif]
[!if AYGMULT_SINGLE_BAND]
								1,
[!endif]
								m_AygDisplay);
		}
	}
[!endif]

[!if !NO_DXCC]
    m_DxccContainer.InitializeEntry(m_cList);
[!if DXCC_MULTI_BAND]
	m_DxccContainer.SetMults(this, m_NumberOfMultBands);
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
    // Add your function implementation here.
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
    static_assert(sizeof(m_NumberOfMultBands) == 4, "fix serialize");
#endif
    if (SUCCEEDED(hr))
        hr = pStream->Read(&m_NumberOfMultBands, sizeof(m_NumberOfMultBands), &bytesread);
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
        hr = pStream->Write(&m_NumberOfMultBands, sizeof(m_NumberOfMultBands), &written);
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