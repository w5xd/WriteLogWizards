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
    // Add your function implementation here.
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
    return hr; 
}

HRESULT [!output MM_CLASS_NAME]::Save(IStorage *p, BOOL fSameAsLoad)  {
    CComPtr<IStream> pStream;
    HRESULT hr = p->CreateStream(StreamName,
        STGM_DIRECT | STGM_READWRITE |
        STGM_CREATE | STGM_SHARE_EXCLUSIVE, 0, 0, &pStream);
    if (SUCCEEDED(hr))
        hr = m_qsoFields.Save(gArchiveVersion, pStream);
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
[!if RTTY]
    {
        { 350000,  400000, FSK_MASK, "80M"},
        { 700000,  730000, FSK_MASK, "40M"},
        {1400000, 1435000, FSK_MASK, "20M"},
        {2100000, 2145000, FSK_MASK, "15M"},
        {2800000, 2970000, FSK_MASK, "10M"},
        {-1, -1, -1}
    };
[!else]
    {
        { 180000,  200000, PHONE_MASK, "160M PH"},
        { 180000,  200000, CW_MASK, "160M CW"},
        { 350000,  400000, PHONE_MASK, "80M PH"},
        { 350000,  400000, CW_MASK, "80M CW"},
        { 700000,  730000, PHONE_MASK, "40M PH"},
        { 700000,  730000, CW_MASK, "40M CW"},
        {1400000, 1435000, PHONE_MASK, "20M PH"},
        {1400000, 1435000, CW_MASK, "20M CW"},
        {2100000, 2145000, PHONE_MASK, "15M PH"},
        {2100000, 2145000, CW_MASK, "15M CW"},
        {2800000, 2970000, PHONE_MASK|FM_MASK|AM_MASK, "10M PH"},
        {2800000, 2970000, CW_MASK, "10M CW"},
		{5000000, 5400000, PHONE_MASK|FM_MASK|AM_MASK, "6M PH"},
		{5000000, 5400000, CW_MASK, "6M CW"},
		{14400000,14800000, PHONE_MASK|FM_MASK|AM_MASK, "2M PH"},
		{14400000,14800000, CW_MASK, "2M CW"},
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