#include "stdafx.h"
#include "Iwritelg.h"
#include "[!output HEADER_FILE]"
#include "Exfsym.h"
#include "bndsmadd.h"
#include "bandsym.h" 

// [!output MM_CLASS_NAME]
[!output MM_CLASS_NAME]::[!output MM_CLASS_NAME]()
    : m_Parent(0)
    , CALL(m_qsoFields, CALL_IDX)
    , SNT(m_qsoFields, SNT_IDX)
    , RST(m_qsoFields, RST_IDX)
    , NR(m_qsoFields, NR_IDX)
{
}

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
        *b = g_BandsCw;
    if (s)
        *s = "Custom";  
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::QsoAdd(QsoPtr_t q)
{
    // Add your function implementation here.
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

static const int CALC_POS_LATER = -1;
static const int CALL_POS = -1;
static const int RST_WID = 4;
static const int NR_WID = 5;
const struct exfa_stru [!output MM_CLASS_NAME]::g_Layout[] =	//todo, Reorder, add/delete, etc.
{
    { "CALL", CALL_WID, CALL_POS, A_CALL | A_PRMPT | A_MULTI | A_NOTIFY },
    { "SNT", RST_WID, CALC_POS_LATER, A_NUMS | A_NOTIFY | A_RST | A_OVERSTRIKE },
    { "RST", RST_WID, CALC_POS_LATER, A_PRMPT | A_NUMS | A_NOTIFY | A_RST | A_OVERSTRIKE },
    { "NR", NR_WID, CALC_POS_LATER, A_NUMS | A_NOTIFY | A_PRMPT | A_TABONLY | A_REQUIRED |
            A_DUPE | A_OVERSTRIKE },
    { 0 }
 };

const struct band_stru [!output MM_CLASS_NAME]::g_BandsPh[] =	//todo
{
    { 180000, 200000, PHONE_MASK, "160M" },
    { 350000, 400000, PHONE_MASK, "80M" },
    { 700000, 730000, PHONE_MASK, "40M" },
    { 1400000, 1435000, PHONE_MASK, "20M" },
    { 2100000, 2145000, PHONE_MASK, "15M" },
    { 2800000, 2970000, PHONE_MASK | FM_MASK | AM_MASK, "10M" },
    { -1, -1, -1 }
};

const struct band_stru [!output MM_CLASS_NAME]::g_BandsCw[] =	//todo
{
    { 180000, 200000, CW_MASK, "160M" },
    { 350000, 400000, CW_MASK, "80M" },
    { 700000, 730000, CW_MASK, "40M" },
    { 1400000, 1435000, CW_MASK, "20M" },
    { 2100000, 2145000, CW_MASK, "15M" },
    { 2800000, 2970000, CW_MASK, "10M" },
    { -1, -1, -1 }
};
