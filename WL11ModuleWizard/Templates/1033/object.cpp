#include "stdafx.h"
#include "iwritelg.h"
#include "[!output HEADER_FILE]"
#include "Exfsym.h"
#include "bndsmadd.h"
#include "bandsym.h" 
[!if ASK_MODE]
#include "AskModeDlg.h"
[!endif]
[!if !NO_ZONE]
#include "DisplayEntryDefs.h"
[!endif]
[!if !NO_AYGMULT]
#include <algorithm>
[!endif]
#include "[!output DLG_HEADER_FILE]"

#ifdef DIM // avoid compiler warnings
#undef DIM
#endif
#define DIM(x) (sizeof(x) / sizeof(x[0]))

extern HINSTANCE g_hInstance;
[!if !NO_ZONE]

static const struct StateDef_t (g_ZoneList[])=
	{{"1"}, {"2"},  {"3"},  {"4"},  {"5"},  {"6"},  {"7"},  {"8"},  {"9"}, {"10"},
	 {"11"}, {"12"}, {"13"}, {"14"}, {"15"}, {"16"}, {"17"}, {"18"}, {"19"}, {"20"},
	 {"21"}, {"22"}, {"23"}, {"24"}, {"25"}, {"26"}, {"27"}, {"28"}, {"29"}, {"30"},
	 {"31"}, {"32"}, {"33"}, {"34"}, {"35"}, {"36"}, {"37"}, {"38"}, {"39"}, {"40"},     
	};
[!endif]

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

/*order of the enum MUST match that in g_Layout (below) and 
** there MUST be an enum for each row in g_Layout */
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


// EVERY ROW HERE MUST HAVE A CORRESDPONDING ENTRY IN enum ExfOrder_t
// TODO: you may reorder these, as long as you reorder the enum above to match.
// You may add items (removal might not be as easy) as long as you add a
// corresponding entry to the enum above.
// Changing ONLY the WIDTH field(s) is auto-magically handled by the wizard-generated
// code, whether you make such a change before or after users have done
// a File/Save.
//
// If you add/delete/change anything else AFTER users have done File/Save of your module,
// then you have to update Load/Save to to figure out what to do...
// ..or, change your GUID in the project idl file and those old files will
// no longer auto-magically load this module.
//
// A_CALL must appear in EXACTLY ONE of the rows and is the callsign
// A_NUMS only allows digits to be entered
// A_PRMPT makes the column appear in the Entry Window (without it appears only in the Log Window)
// A_RST invokes special processing for RST signal reports (and should be specified with A_NUMS)
// A_OVERSTRIKE and A_TABONLY mean typing overstriks, and typing into the last character
// leaves the cursor in this field. They can be changed later by the user
// A_NOSPACE means typing SPACE moves the cursor to the next field rather than enter a space into the field.
// A_REQUIRED in the absence of A_MULTI prevents a blank entry
// A_REQUIRED in the presence of A_MULTI additional requires a run through MultiCheck with a return value >=0
// A_MULTI causes a change in the field to be notified in MultiCheck
// A_NOTIFY causes any user edit of the QSO to be notified over networked copies of WriteLog
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
    , fMLT(m_qsoFields, NAMEDMULT_IDX)
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
[!if ZONE_SINGLE_BAND]
    , m_ZoneMults(0)
[!endif]
[!if AYGMULT_SINGLE_BAND]
    , m_AygMults(0)
[!endif]
[!if NAMEDMULT_SINGLE_BAND]
	, m_NamedMults(0)
[!endif]
    , m_NumberOfDupeSheetBands(0)
{}

// dtor
[!output MM_CLASS_NAME]::~[!output MM_CLASS_NAME]()
{}

HRESULT [!output MM_CLASS_NAME]::FinalConstruct()
{
    m_qsoFields.StaticInit(g_Layout);
    HRESULT hr = S_OK;
[!if !NO_DXCC || !NO_ZONE]
	/*TODO:  DXCCDOS.DAT maps call into DXCC country list
	**       DXCCWAE.DAT maps call into CQWW country list
	**       CQZONE.DAT maps CALL to CQ zone
	**       ITUZONE.DAT maps CALL to ITU zone.	*/
[!endif]
[!if !NO_DXCC]
    if (SUCCEEDED(hr))
        hr = m_DxContext.Init("DXCCDOS.DAT");
[!endif]
[!if !NO_ZONE]
    if (SUCCEEDED(hr))
        hr = m_ZoneContext.Init("CQZONE.DAT");
[!endif]
[!if !NO_NAMEDMULT]
    if (SUCCEEDED(hr))
        hr = m_pNamedMults.CoCreateInstance(__uuidof(NmdMul),
            0, CLSCTX_SERVER);
	// TODO--you must create an INI file, make sure its in WriteLog's 
	// \programs folder, and make sure the following names it correctly.
    if (SUCCEEDED(hr))
        hr = m_pNamedMults->put_FileName(reinterpret_cast<const unsigned char *>("[!output COCLASS].ini"));
    if (SUCCEEDED(hr))
        m_pNamedMults->Init(reinterpret_cast<const unsigned char *>("[!output COCLASS]"));
	if (SUCCEEDED(hr))
	    hr = m_pNamedMults->get_MultCount(&m_NumNamed);
[!endif]

    return hr;
}

void [!output MM_CLASS_NAME]::FinalRelease()
{
    m_bandSumm.Release();
[!if !NO_DXCC]
    m_DxContext.Release();
[!endif]
[!if !NO_ZONE]
    m_ZoneDisplayEntry.Release();
    m_ZoneContext.Release();
[!endif]
[!if !NO_NAMEDMULT]
    m_NamedDisplayEntry.Release();
    m_NamedDisplay.Release();
    m_pNamedMults.Release();
[!endif]
[!if !NO_AYGMULT]
	if (m_AygDisplayEntry)
		m_AygDisplayEntry->ReleasePage(); // break ref cycle
    m_AygDisplayEntry.Release();
    m_AygDisplay.Release();
[!endif]
}

// IWlogMulti Methods
HRESULT [!output MM_CLASS_NAME]::GetLayout(ConstBandPtr_t * b, ConstExfPtr_t * e, LPCSTR * s)
{
    /* Called by WriteLog on its File/New
    ** Note that it is NOT called on File/Open.
    ** On File/Open, this module must recover its state on its Load() method.
    */
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
    /* The given QSO is being added to the log. Tally its multipliers  */
	int points = 0;
    short   Mult[BAND_SUMMARY_WIDTH];
	memset(Mult, 0, sizeof(Mult));
    int band = q->band; // q->band is always a Dupe Band
    if (band > m_NumberOfDupeSheetBands)
        band = m_NumberOfDupeSheetBands;
	band = DupeBandToMultBand(band);

[!if !NO_DXCC]
    fCMULT(q) = "";
[!endif]
[!if !NO_ZONE]
    fZMULT(q) = "";
[!endif]
[!if PTS_COLUMN]
    fPTS(q) = "";
[!endif]

    if (q->dupe == ' ')
    {
[!if !NO_DXCC]
        //*********************************
        //Process DXCC 
        int dx = m_DxContext.FillQso(q, fCALL, fCPRF, fCOUNTRY, fAMBF);
        if (dx >= 0)
        {
[!if DXCC_SINGLE_BAND]
            int newDxccFlag = !m_Countries[dx]++;
[!endif]
[!if DXCC_MULTI_BAND]
            int newDxccFlag = !m_Countries[band][dx]++;
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
                m_DxccContainer.InvalidateCountry(dx);
            }
        }

[!endif]

[!if !NO_NAMEDMULT]
		//*********************************
		//Processed named multipliers
		int newNamedMultFlag = 0;
		int nm = FindNamed(fRCVD(q));
		if (nm == m_NumNamed)
			fMLT(q) = "?";
		else 
		{
[!if NAMEDMULT_MULTI_BAND]
			newNamedMultFlag = !(m_Named[band][nm]++);
[!endif]
[!if NAMEDMULT_SINGLE_BAND]
			newNamedMultFlag = !(m_Named[nm]++);
[!endif]
		}
		if (newNamedMultFlag)
		{
            char buf[8];
[!if NAMEDMULT_MULTI_BAND]
			_itoa_s(++m_NamedMults[band], buf, 10);
[!endif]
[!if NAMEDMULT_SINGLE_BAND]
			_itoa_s(++m_NamedMults, buf, 10);
[!endif]
            fMLT(q) = buf;
			Mult[BAND_SUMMARY_MUL] += 1;
			if (m_NamedDisplay)
				m_NamedDisplay->Invalidate(nm);
		}

[!endif]
[!if !NO_ZONE]
		//*********************************
		//Process zones
		int			newZoneFlag = 0;
		int z = FindZone(fZN(q));
		if (z == NUMZONES)
            fZMULT(q) = "?";
		else
		{
[!if ZONE_MULTI_BAND]
			newZoneFlag = !(m_Zones[band][z]++);
[!endif]
[!if ZONE_SINGLE_BAND]
			newZoneFlag = !(m_Zones[z]++);
[!endif]
		}
		if (newZoneFlag)
		{
            char buf[8];
[!if ZONE_MULTI_BAND]
            _itoa_s(++m_ZoneMults[band], buf, 10);
[!endif]
[!if ZONE_SINGLE_BAND]
            _itoa_s(++m_ZoneMults, buf, 10);
[!endif]
            fZMULT(q) = buf;
			Mult[BAND_SUMMARY_MUL] += 1;
			if (m_ZoneDisplay)
				m_ZoneDisplay->Invalidate(z);
		}

[!endif]
[!if !NO_AYGMULT]
		//*********************************
		//Process multiplier list built as you go...
		int	newAygFlag = 0;
		int ayg = FindAyg(TRUE, fAYG(q));
		if (ayg < 0)
			fAYGMULT(q) = "?";
		else
		{
[!if AYGMULT_MULTI_BAND]
			newAygFlag = !(m_AygStatus[band][ayg]++);
[!endif]
[!if AYGMULT_SINGLE_BAND]
			newAygFlag = !(m_AygStatus[ayg]++);
[!endif]
		}
		if (newAygFlag)
		{
            char buf[8];
[!if AYGMULT_MULTI_BAND]
            _itoa_s(++m_AygMults[band], buf, 10);
[!endif]
[!if AYGMULT_SINGLE_BAND]
            _itoa_s(++m_AygMults, buf, 10);
[!endif]
            fAYGMULT(q) = buf;
			Mult[BAND_SUMMARY_MUL] += 1;
			if (m_AygDisplayEntry)
				m_AygDisplayEntry->Invalidate(ayg);
		}
[!endif]
        //*********************************
		//Process points and multiplier display
		points = PointsForQso(q);
[!if !MULTI_MODE && PTS_COLUMN]
		Mult[BAND_SUMMARY_PTS] = points;
[!endif]
		m_BandPoints[band] += points;
		m_BandQsos[band] += 1;
[!if PTS_COLUMN]
        if (points)
        {
            char buf[16];
            _itoa_s(points, buf, 10);
            fPTS(q) = buf;
        }
[!endif]
		if (m_bandSumm)
		{
[!if MULTI_MODE]
			if (q->mode == '3')
				Mult[BAND_SUMMARY_CW] = 1;
[!if RTTY]
            else if  (q->mode == '6')
				Mult[BAND_SUMMARY_RTTY] = 1;
[!endif]
			else
				Mult[BAND_SUMMARY_PHONE] = 1;
[!endif]
			m_bandSumm->ContributeMultipliers(
					band,
					BAND_SUMMARY_WIDTH, 
					Mult);
			m_bandSumm->SetScore(Score());
		}
[!if ASK_MODE]
        // record mode based on a qso
        if (q->mode == '3')
            m_ModeSelected = ASK_MODE_CW;
[!if RTTY]
        else if (q->mode == '6')
            m_ModeSelected = ASK_MODE_RY;
[!endif]
        else
            m_ModeSelected = ASK_MODE_PH;
[!endif]
    }

	// If the multiplier module returns S_FALSE, WriteLog completely
	// resorts all QSOs (by calling InitQsoData and QsoAdd for each QSO).
	// So the module will get called again on QsoAdd and it had better NOT
	// return S_FALSE the second time else an infinite recursion results.
    return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::QsoRem(QsoPtr_t q)
{
    /* The given QSO is being removed from the log. remove its contribution to mutlipliers.
    ** When the user edits a QSO already in the log, we get a QsoRem of the old
    ** followed by QsoAdd of the new. Note that this can make multiplier tallies out
    ** of order. For example, editing an early QSO that was not a mult in such a
    ** way that it becomes a mult will cause the log to show an earlier QSO 
    ** with a higher multiplier tally than a later one. This is fixed if the user
    ** does Contest/Recalc. */
	int points = 0;
    short   Mult[BAND_SUMMARY_WIDTH];
	memset(Mult, 0, sizeof(Mult));
    int band = q->band;
    if (band > m_NumberOfDupeSheetBands)
        band = m_NumberOfDupeSheetBands;
	band = DupeBandToMultBand(band);

    if (q->dupe == ' ')
    {
[!if MULTI_MODE]
		if (q->mode == '3')
			Mult[BAND_SUMMARY_CW] = -1;
[!if RTTY]
        else if (q->mode == '6')
			Mult[BAND_SUMMARY_RTTY] = -1;
[!endif]
		else
			Mult[BAND_SUMMARY_PHONE] = -1;

[!endif]
[!if !NO_NAMEDMULT]
		int newNamedMultFlag = 0;
		int nm = FindNamed(fRCVD(q));
		if (nm != m_NumNamed)
		{
[!if NAMEDMULT_MULTI_BAND]
			newNamedMultFlag = !(--m_Named[band][nm]);
[!endif]
[!if NAMEDMULT_SINGLE_BAND]
			newNamedMultFlag = !(--m_Named[nm]);
[!endif]
		}
		if (newNamedMultFlag)
		{
[!if NAMEDMULT_MULTI_BAND]
			m_NamedMults[band] -= 1;
[!endif]
[!if NAMEDMULT_SINGLE_BAND]
			m_NamedMults -= 1;
[!endif]
			Mult[BAND_SUMMARY_MUL] -= 1;
			if (m_NamedDisplay)
				m_NamedDisplay->Invalidate(nm);
		}

[!endif]
[!if !NO_DXCC]
		int dx = m_DxContext.CountryFromQsoPrefix(q, fCPRF);
        if (dx >= 0)
		{
[!if DXCC_SINGLE_BAND]
            int newDxccFlag = !--m_Countries[dx];
[!endif]
[!if DXCC_MULTI_BAND]
            int newDxccFlag = !--m_Countries[band][dx];
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
				m_DxccContainer.InvalidateCountry(dx);
			}
		}

[!endif]
[!if !NO_ZONE]
		int			newZoneFlag = 0;
		int z = FindZone(fZN(q));
		if (z != NUMZONES)
		{
[!if ZONE_MULTI_BAND]
			newZoneFlag = !(--m_Zones[band][z]);
[!endif]
[!if ZONE_SINGLE_BAND]
			newZoneFlag = !(--m_Zones[z]);
[!endif]
		}
		if (newZoneFlag)
		{
[!if ZONE_MULTI_BAND]
			--m_ZoneMults[band];
[!endif]
[!if ZONE_SINGLE_BAND]
			--m_ZoneMults;
[!endif]
			Mult[BAND_SUMMARY_MUL] -= 1;
			if (m_ZoneDisplay)
				m_ZoneDisplay->Invalidate(z);
		}

[!endif]
[!if !NO_AYGMULT]
		//*********************************
		//Process multiplier list built as you go...
		int	newAygFlag = 0;
		int ayg = FindAyg(FALSE, fAYG(q));
		if ((ayg >= 0) && ( ayg < static_cast<int>(m_AygDisplayNames.size())))
		{
[!if AYGMULT_MULTI_BAND]
			newAygFlag = !(--m_AygStatus[band][ayg]);
[!endif]
[!if AYGMULT_SINGLE_BAND]
			newAygFlag = !(--m_AygStatus[ayg]);
[!endif]
		}
		if (newAygFlag)
		{
[!if AYGMULT_MULTI_BAND]
			--m_AygMults[band];
[!endif]
[!if AYGMULT_SINGLE_BAND]
			--m_AygMults;
[!endif]
			Mult[BAND_SUMMARY_MUL] -= 1;
			if (m_AygDisplayEntry)
				m_AygDisplayEntry->Invalidate(ayg);
		}

[!endif]
        points = PointsForQso(q);
[!if !MULTI_MODE && PTS_COLUMN]
		Mult[BAND_SUMMARY_PTS] = -points;
[!endif]
		m_BandPoints[band] -= points;
		m_BandQsos[band] -= 1;
		if (m_bandSumm)
		{
			m_bandSumm->ContributeMultipliers(
					band,
					BAND_SUMMARY_WIDTH, 
					Mult);
			m_bandSumm->SetScore(Score());
		}    
    }
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::InitQsoData()
{
/* Set our state to no multipliers nor QSOs.*/
[!if MULTI_MODE]
	static const char * const band_Title[] =
	{
		"160m",
		"80m",
		"40m",
		"20m",
		"15m",
		"10m",
		"6m",
		"2m"
	};
[!endif]
    m_BandPoints.clear();
    m_BandQsos.clear();
[!if!NO_NAMEDMULT]
    m_Named.clear();
[!if NAMEDMULT_MULTI_BAND]
    m_NamedMults.clear();
[!else]
    m_NamedMults = 0;
[!endif]

[!endif]
[!if !NO_DXCC]
    m_MyCountryIndex = m_DxContext.dxcc_Home();
    m_Countries.clear();
[!if DXCC_MULTI_BAND]
	m_DxccMults.clear();
[!else]
    m_DxccMults = 0;
[!endif]

[!endif]
[!if !NO_ZONE]
    m_Zones.clear();
[!if ZONE_MULTI_BAND]
    m_ZoneMults.clear();
[!else]
    m_ZoneMults = 0;
[!endif]

[!endif]
[!if !NO_AYGMULT]
    m_AygDisplayNames.clear();
    m_AygStatus.clear();
    m_AygIdxToName.clear();
[!if AYGMULT_MULTI_BAND]
    m_AygMults.clear();
[!else]
    m_AygMults = 0;
[!endif]

[!endif]
    m_PageQsos = 0;
    m_PageQsoPoints = 0;
    m_PageMultipliers = 0;
    if (m_bandSumm)
    {
/* The titles we put in the band summary are important 
** The real time score broadcast makes assumptions about
** what titles are multipliers and what are points 
** and what are QSO counts. 
** The same thing with SetBandTitle. If the band's title
** contains a mode (like CW, SSB, PH or RTTY, or RY)
** Then the score broadcast assumes the row applies
** only to one mode. */
[!if !NO_DXCC||!NO_NAMEDMULT||!NO_ZONE||!NO_AYGMULT]
		m_bandSumm->SetItemTitle(BAND_SUMMARY_MUL, "Mul");
[!endif]
[!if MULTI_MODE]
		m_bandSumm->SetItemTitle(BAND_SUMMARY_CW, "CW");
		m_bandSumm->SetItemTitle(BAND_SUMMARY_PHONE, "PH");
[!if RTTY]
		m_bandSumm->SetItemTitle(BAND_SUMMARY_RTTY, "RY");
[!endif]
		for (int i = 0; i < DIM(band_Title); i += 1)
			m_bandSumm->SetBandTitle(i, band_Title[i]);
[!else]
[!if PTS_COLUMN]
		m_bandSumm->SetItemTitle(BAND_SUMMARY_PTS, "Pts");
[!endif]
[!endif]
    }       
    return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::MultiCheck(
    QsoPtr_t q,     // is the QSO being checked
    int p,          // denotes the field in the QSO (its the value in the exfa_stru.pl member)
    int * Result,   // is where to write -1, 0, +1 for Unknown, no mult, new mult
    long RequestMask, // flag whether can write into the QSO
    char * Message  // is a place to overide the message being asked for
    )
{
/* WriteLog calls here a lot. 
** When the user is typing into WriteLog's Entry Window
** When packet spots come in, a hidden qso data structure (i.e. one
** that the user is not aware of) gets offered to the multiplier module here
** so that multiplier checking can be added to packet spots. And such
** checking is called on a per-band basis for spots. That is, when the
** user types, we only get called with q->band set to the one he is logging
** qso's on. But when a packet spot is checked, we get called repeatedly with q->band
** set to all possible values. */

	int ret = -1;
    int band = q->band;
    if (band > m_NumberOfDupeSheetBands)
        band = m_NumberOfDupeSheetBands;
    band = DupeBandToMultBand(band);

    bool canWrite = (RequestMask & WLOG_MULTICHECK_NOWRT) == 0;
    bool msgValid = (RequestMask & WLOG_MULTICHECK_MSGSET) != 0;

	QsoPtr_t OldQ(0);
	if (canWrite)
	{
		//locate any previous QSO with this station...
		unsigned long QsoNumber;
		for (int j = 0; j < m_NumberOfDupeSheetBands; j += 1)
		{
			if (m_Parent->SearchDupeSheet(q, j, 0, &QsoNumber) == S_OK)
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
        int i = m_DxContext.CheckQso(q, canWrite,  fCALL, fCOUNTRY, fCPRF, fAMBF, ambig);
		if (!ambig)
		{   // The DXCC database can map a given call to more than one DXCC country. 
            NewCountry = get_MultWorked(DXCC_MULT_ID, i, band) == S_FALSE ? 1 : 0;
            if (ret <= 0)
                ret = NewCountry ? 1 : 0;
            if ((Message != 0) && NewCountry && msgValid)
                ::LoadString(g_hInstance, IDS_NEWCOUNTRY_MSG, Message, MAX_MULT_MESSAGE_LENGTH);
		}
	}

[!endif]   
[!if !NO_NAMEDMULT]
	//***NAMED MULTIPLIER PROCESSING
	//If its a CALL and the RCVD_POS not filled in...
    bool flagNamed(false);
    if ((fCALL == p) && fRCVD(q).empty()) 
    {
        if (OldQ)
		{
            if (canWrite)    // fill in new field from old qso
                fRCVD(q).assign(fRCVD(OldQ));
            if (!fRCVD(q).empty())
			    flagNamed = true; // and check whether fRCVD is new mult
		}
    }
	if (flagNamed || fRCVD == p)      /*or received*/
    {
       int n = FindNamed(fRCVD(q));
	   if (n != m_NumNamed)
	   {
           int NamedWorked = get_MultWorked(NAMED_MULT_ID, n, band) == S_FALSE ? 1 : 0;
		   if (ret <= 0)
               ret = NamedWorked;
		   if (NamedWorked && msgValid && (Message != 0))
			    LoadString(g_hInstance, IDS_NEWNAMED_MSG, Message, MAX_MULT_MESSAGE_LENGTH);
	   }
	}

[!endif]
[!if !NO_ZONE]
    //Zone processing*************
    bool flagZone = false;
	if (fCALL(q).empty())
	{
        if (canWrite)
            fZN(q) = "";
		ret = 0;
	}
	else if (fZN(q).empty())
	{
		if (fCALL == p)
		{
			if (OldQ)
			{
                if (canWrite) // fill in fZN from OldQ
                    fZN(q).assign(fZN(OldQ));
                flagZone = true;
			}
			else
			{
                int z = m_ZoneContext.CheckQso(q, fCALL);
                if ((z >= 0) && (z < NUMZONES))
                {
                    if (canWrite)
                        fZN(q) = m_ZoneContext.cList()[z].country;
                    flagZone = true;
				}
			}
		}
	}

    if (flagZone || fZN == p)    /*zone*/
	{
        int z = FindZone(fZN(q));
		if (z != NUMZONES)
		{
            int NewZone = get_MultWorked(ZONE_MULT_ID, z, band) == S_FALSE ? 1 : 0;
			if (ret <= 0)
				ret = NewZone;
			if (NewZone && msgValid && (Message != 0))
				LoadString(g_hInstance, IDS_NEWZONE_MSG, Message, MAX_MULT_MESSAGE_LENGTH);
		}
	}

[!endif]
[!if !NO_AYGMULT]
	//As You Go multiplier processing.....
    bool flagAyg(false);
	if (fCALL == p)
    {
        std::string aygVal;
        if (OldQ)
            aygVal = fAYG(OldQ);
        if (fAYG(q).empty() && canWrite) // fill in fAYG from OldQ
            fAYG(q) = aygVal.c_str();
        if (!fAYG(q).empty())
            flagAyg = true;
	}

    if (flagAyg || fAYG == p)    /*this multiplier*/
	{
		int NewAyg = 0;
		int ayg = FindAyg(FALSE, fAYG(q));
[!if AYGMULT_MULTI_BAND]
		NewAyg = m_AygStatus.worked(band, ayg) ? 0 : 1;
[!endif]
[!if AYGMULT_SINGLE_BAND]
		NewAyg = m_AygStatus.worked(ayg) ? 0 : 1;
[!endif]
		if (ret <= 0)
			ret = NewAyg;
		if (NewAyg && msgValid && (Message != 0))
			LoadString(g_hInstance, IDS_AYG_MSG,	Message, MAX_MULT_MESSAGE_LENGTH);
	}

[!endif]
    *Result = ret; // -1, 0, +1 for Unknown, no mult, new mult
    return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::Display(HWND Window)
{
/* There are two very different architectures available here.
** The old style is to simply put up a modal (or modeless)
** dialog box with Window as parent.
** But if WriteLog is to display the multiplier list
** in its dockable windows, then the module must
** CoCreateInstance of progid writelog.multdisp.
*/
[!if !NO_NAMEDMULT||!NO_DXCC||!NO_ZONE||!NO_AYGMULT]
    if (!m_MultDispContainer)
    {
        if (SUCCEEDED(m_MultDispContainer.CoCreateInstance(
            L"writelog.multdisp", 0, CLSCTX_ALL)))
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
            CComObject<CNamedDisplayHelper<[!output MM_CLASS_NAME], NAMED_MULT_ID> > *pTemp(0);
            if (SUCCEEDED(pTemp->CreateInstance(&pTemp)))
            {
                pTemp->Init(m_pNamedMults,
                    m_NamedDisplay,
[!if NAMEDMULT_MULTI_BAND]
                    NumberOfMultBands(),
[!endif]
[!if NAMEDMULT_SINGLE_BAND]
                    1,
[!endif]
                this);
                m_NamedDisplayEntry = pTemp;
            }
        }
    }

[!endif]
[!if !NO_ZONE]
	if (m_ZoneDisplay)
    {
        if (!m_ZoneDisplayEntry)
        {
            m_ZoneDisplay->put_Title("TODO");
            CComObject<CZoneDisplayHelper<[!output MM_CLASS_NAME] , ZONE_MULT_ID> > *pTemp(0);
            if (SUCCEEDED(pTemp->CreateInstance(&pTemp)))
            {
                pTemp->Init(g_ZoneList,
                                DIM(g_ZoneList),
                                m_ZoneDisplay,
[!if ZONE_MULTI_BAND]
								NumberOfMultBands(),
[!endif]
[!if ZONE_SINGLE_BAND]
								1,
[!endif]
								this);
                m_ZoneDisplayEntry = pTemp;
            }
        }
    }
[!endif]
[!if !NO_AYGMULT]
	if (m_AygDisplay)
	{
		if (!m_AygDisplayEntry)
		{
			m_AygDisplay->put_Title("TODO");
            CComObject<CAygDisplayHelper<[!output MM_CLASS_NAME] , AYG_MULT_ID> > *pTemp(0);
            if (SUCCEEDED(pTemp->CreateInstance(&pTemp)))
            {
                pTemp->Init(
                    this,
[!if AYGMULT_MULTI_BAND]
                    NumberOfMultBands(),
[!endif]
[!if AYGMULT_SINGLE_BAND]
                    1,
[!endif]
                    m_AygDisplay);
                m_AygDisplayEntry = pTemp;
            }
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
HRESULT [!output MM_CLASS_NAME]::Score( // not a good name anymore. "ParameterSetup" would be better
    Configuration_Entry_t * Config,     // archaic
    HWND Window,                        // parent window for your dialog
    unsigned QsoNum,                    // number of QSOs in the log now
    const char * TargetDir              // folder containing the current .wl file
    )
{
    /* The name of this function is archaic. It is called from WriteLog on its Contest/Parameter-Setup menu entry.
    ** It is a good place to have code to put up a parameter selection dialog for this module.
    ** The Config, QsoNum and TargetDir parameters are valid in all WL versions up to 12.05, but it is not
    ** especially useful to support them. They were part of an architecture that enabled text substitution by the module
    ** into a txt/rtf file that had the contest summary. Cabrillo has put an end to any need for that.
    */
    // TODO Put up a parameter selection dialog.
    [!output MM_DLG_CLASS_NAME] Dlg;
    Dlg.DoModal(Window);
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::GetModuleData(long * Data)
{
// MODULE_DATA_SCORE   Enables WriteLog's Contest/Parameter-Setup menu item.
// MODULE_DATA_NOBSM   WriteLog has first column QSO count in its bandsummary UNLESS module sets this in GetModuleData
    *Data =
[!if MULTI_MODE]
		MODULE_DATA_NOBSM |
[!endif]
		MODULE_DATA_SCORE;
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::SetMMParent(IWriteLog *p)
{
    m_Parent = p; // NOT refcounted. WriteLog holds a ref on us.
    m_bandSumm.Release();
    wlshr_GetSumm(p, &m_bandSumm);
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::TranslateAccelerator(MSG *, short /* obsolete. always 1 */)
{
    /* WriteLog calls here on all keyboard events.
    ** If the module processes the message (the keyboard event) then it returns S_OK
    ** and WriteLog does NOT process the keyboard event: EVEN IF THE USER HAS SETUP
    ** that key as a keyboard shortcut! This behavior MUST be documented to the user.   */
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::MatchedQso(QsoPtr_t New, QsoPtr_t Old)
{
    // TODO
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::QsoSearch(QsoPtr_t NewQ, QsoPtr_t OldQ, int * IsGood)
{
    if (IsGood)
        *IsGood = 0;
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::SetDupeSheet(QsoPtr_t q, int * DupeSheet)
{
	// WriteLog maintains as many separate dupe sheets as the multiplier
	// module needs. Only QSOs that have matching DupeSheet values are
	// duped against each other.
    // This supports, for example, VHF rovers. The do NOT need to start a
    // new .wl file on changing grid square. Instead, they enter a different
    // transmitted grid square the module tells WL to create a new dupe
    // sheet for that grid square.
    // TODO
   if (DupeSheet)
      *DupeSheet = 0;
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::DupeSheetTitle(int DupeSheet, char * Title, int TitleLength)
{
    return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::TallyPrintQso(QsoPtr_t q)
{
    int Mul=0, Pts = 0;
    if (q->dupe == ' ')
    {
        m_PageQsos += 1;
[!if !NO_NAMEDMULT]
        if (isdigit(*fMLT(q)))
			Mul += 1;
[!endif]
[!if !NO_DXCC]
		if (isdigit(*fCMULT(q)))
			Mul += 1;
[!endif]
[!if !NO_ZONE]
		if (isdigit(*fZMULT(q)))
			Mul += 1;
[!endif]
[!if !NO_AYGMULT]
		if (isdigit(*fAYGMULT(q)))
			Mul += 1;
[!endif]
[!if PTS_COLUMN]
        Pts = atoi(fPTS(q));
[!else]
		Pts = PointsForQso(q);
[!endif]
        m_PageMultipliers += Mul;
        m_PageQsoPoints += Pts;
    }
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::FormatPageSumm(char FAR *Buf, int BufLength)
{
    char    FormatBuf[200];
    char    RscBuf[200];
    RscBuf[0] = 0;
    LoadString(g_hInstance, IDS_FORMAT_PAGE_SUM, RscBuf, sizeof(RscBuf));
    if (RscBuf[0])
    {
        wsprintf(FormatBuf, RscBuf, m_PageQsos, m_PageQsoPoints, m_PageMultipliers);
        strncpy_s(Buf, BufLength, FormatBuf, BufLength-1);
    }
    m_PageQsos = m_PageQsoPoints = m_PageMultipliers = 0;
    return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::GetAdifName(long FieldId, long NameLen, char *Name)
{
	*Name = 0;
[!if RST_IN_EXCHANGE]
	if (fSN == FieldId)
		strncpy_s(Name, NameLen, "RST_SENT", NameLen-1);
	else
	if (fRS == FieldId)
		strncpy_s(Name, NameLen, "RST_RCVD", NameLen-1);
[!endif]
[!if NR_IN_EXCHANGE]
	if (fNR == FieldId)
		strncpy_s(Name, NameLen, "SRX", NameLen-1);
[!endif]
	//TODO: ADIF FIELDS...
	if (*Name)
		return S_OK;
	else
		return S_FALSE;
}

[!if !NO_DXCC || !NO_ZONE || !NO_NAMEDMULT || !NO_AYGMULT]
// multipliers
HRESULT [!output MM_CLASS_NAME]::get_MultWorked(int id, short Mult, short band)
{
    switch (id)
    {
[!if DXCC_SINGLE_BAND]
    case DXCC_MULT_ID:
            return m_Countries.worked(Mult) ? S_OK : S_FALSE;
[!endif]
[!if DXCC_MULTI_BAND]
    case DXCC_MULT_ID:
            return m_Countries.worked(band,Mult) ? S_OK : S_FALSE;
[!endif]
[!if ZONE_SINGLE_BAND]
    case ZONE_MULT_ID:
            return m_Zones.worked(Mult) ? S_OK : S_FALSE;
[!endif]
[!if ZONE_MULTI_BAND]
    case ZONE_MULT_ID:
            return m_Zones.worked(band,Mult) ? S_OK : S_FALSE;
[!endif]
[!if NAMEDMULT_SINGLE_BAND]
    case NAMED_MULT_ID:
            return m_Named.worked(Mult) ? S_OK : S_FALSE;
[!endif]
[!if NAMEDMULT_MULTI_BAND]
    case NAMED_MULT_ID:
            return m_Named.worked(band,Mult) ? S_OK : S_FALSE;
[!endif]
[!if AYGMULT_SINGLE_BAND]
    case AYG_MULT_ID:
            return m_AygStatus.worked(Mult) ? S_OK : S_FALSE;
[!endif]
[!if AYGMULT_MULTI_BAND]
    case AYG_MULT_ID:
            return m_AygStatus.worked(band,Mult) ? S_OK : S_FALSE;
[!endif]
    default:
        break;
    }
    return E_INVALIDARG;
}

[!endif]
[!if !NO_AYGMULT]
HRESULT [!output MM_CLASS_NAME]::get_MultTitle(int ID, short Mult, const char **Title)
{
    if ((Mult >= 0) && (Mult < static_cast<int>(m_AygIdxToName.size())))
    {
        AygNames_t::iterator itor = m_AygIdxToName[Mult];
        if (itor != m_AygDisplayNames.end())
        {
            *Title = itor->first.c_str();
            return S_OK;
        }
    }
    return E_INVALIDARG;
}

[!endif]
long [!output MM_CLASS_NAME]::PointsForQso(QsoPtr_t q)
{
	return 1;	//TODO
}

long [!output MM_CLASS_NAME]::Score()
{
	//TODO
	return 0;
}

[!if !NO_ZONE]
int [!output MM_CLASS_NAME]::FindZone(    /*returns zone index*/
	 const char *c)
{
    int retval=0;
    while (*c)
    {
        retval *= 10; retval += *(c++) - '0'; 
    }
    retval -= 1;    /*reduce to zero index*/
    if ((retval > NUMZONES) || (retval<0)) retval = NUMZONES;
    return retval;
}

[!endif]
[!if !NO_NAMEDMULT]

int [!output MM_CLASS_NAME]::FindNamed(      /*returns index of multiplier*/
	const char *c)
{
	short i;
	if (m_pNamedMults->IndexFromName(reinterpret_cast<const unsigned char *>(c), &i) != S_OK)
		return m_NumNamed;
	return i;
}

[!endif]
[!if !NO_AYGMULT]
int [!output MM_CLASS_NAME]::FindAyg(int AddNew, const char *c)
{
	if (!*c) return m_AygDisplayNames.size();

    AygNames_t::iterator itor = m_AygDisplayNames.find(c);
    if (itor != m_AygDisplayNames.end())
        return itor->second;
    else
    {
        if (AddNew)
        {
            unsigned nextIdx = m_AygIdxToName.size();
            m_AygIdxToName.push_back(
                m_AygDisplayNames.insert(
                AygNames_t::value_type(c, nextIdx)
                ).first);
            return nextIdx;
        }
    }
    return m_AygDisplayNames.size();
}

[!endif]

[!if RTTY]
//RTTY SUPPORT....
HRESULT [!output MM_CLASS_NAME]::WhatsTheBestField(QsoPtr_t q, const char *s, short *Offset)
{
	//TODO
    //		*Offset = CALL_POS;
	//		return NOERROR;

    return E_NOTIMPL;
}

HRESULT  [!output MM_CLASS_NAME]::IsCharOKHere(QsoPtr_t q, char c, short Offset)
{
    // todo
    return S_OK;
}

[!endif]
[!if CABRILLO]
// IWlogCabrillo Methods
// See Iwritelg.h for more detailed documentation
HRESULT [!output MM_CLASS_NAME]::ConfirmFieldsFilled(HWND w)
{
   	//opportunity for multiplier module to put up a box asking for info before Cabrillo export

	if (1					//TODO
[!if !NO_NAMEDMULT]
		|| m_MyMult.empty()
[!endif]
		)	
	{
		if (w)
		{
			char buf[256];
			::LoadString(g_hInstance, IDS_NOFILEYET, buf, sizeof(buf));
			MessageBox(w, buf, "WriteLog", 0); 
		}
	
		return S_FALSE;
	}
	return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::SetCallsign(const char * Call)
{   // We need to put CALL into Cabrillo but we might not have it
    // but WriteLog does. It tells us the call here
    m_MyCallsign = Call;
	return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::GetHeaderLineCount(short * pLines)
{
    // TODO
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::GetHeaderLine(short LineNumber, char * Buf)
{
    // TODO
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::GetContestName(char * Buf)
{
	strcpy_s(Buf, WLOG_CABRILLO_MAXCONTESTNAMELEN,
        "[!output COCLASS]");	//TODO
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::GetClaimedScore(long * pScore)
{
    *pScore = Score();
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::GetTxFieldCount(short * pCount)
{
    // TODO
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::FormatTxField(QsoPtr_t q, short Field, char *Buf)
{
	*Buf = 0;
	switch (Field)
	{
#if 0 // TODO
	case 0:
		wsprintf(Buf, "%-13.13s ", m_MyCallsign.c_str());
		break;
[!if RST_IN_EXCHANGE]
	case 1:
		wsprintf(Buf, "%3.3s ", fSN(q).str());
		break;
[!endif]
[!if !NO_NAMEDMULT]
	case 0:	//TODO
		wsprintf(Buf, "%-6.6s ", m_MyMult.c_str());
		break;
[!endif]
[!if NR_IN_EXCHANGE]
	case 0:	//TODO
		wsprintf(Buf, "%6d ", q->serial);
		break;
[!endif]
     // Sent serial number example
    case 0:
		wsprintf(Buf, "%4d ", q->serial);

#endif
	//TODO case xxxx
    default:
        return E_INVALIDARG;
	}
	return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::GetRxFieldCount(short * pCount)
{
    // TODO
    return E_NOTIMPL;
}
HRESULT [!output MM_CLASS_NAME]::FormatRxField(QsoPtr_t q, short Field, char * Buf)
{
	*Buf = 0;
	switch (Field)
	{
#if 0 // TODO
	case 0:
		wsprintf(Buf, "%-13.13s ", fCALL(q).str());
		break;
[!if RST_IN_EXCHANGE]
	case 1:
		wsprintf(Buf, "%3.3s ", fRS(q).str());
		break;
[!endif]
[!if NR_IN_EXCHANGE]
	case 0:	//TODO
		wsprintf(Buf, "%6.6s ", fNR(q).str());
		break;
[!endif]
[!if !NO_NAMEDMULT]
	case 0:	//TODO
		wsprintf(Buf, "%-6.6s ", fRCVD(q).str());
		break;
[!endif]
#endif
	//TODO case xxxx
    default:
        return E_INVALIDARG;
	}
    return E_NOTIMPL;
}
[!endif]

//IWlogScoreInfo
HRESULT [!output MM_CLASS_NAME]::MultsAndPts(long *pMultCount, long *pPtsCount)
{
	int i;
	//TODO--Check if score really is mults * pts here...
	// *pMultCount = total of mults in the log
	long Mults = 0;
[!if DXCC_MULTI_BAND || NAMEDMULT_MULTI_BAND || ZONE_MULTI_BAND || AYGMULT_MULTI_BAND]
	for (i = 0; i < NumberOfMultBands(); i += 1)
	{
[!if DXCC_MULTI_BAND]
		Mults += m_DxccMults[i];
[!endif]
[!if NAMEDMULT_MULTI_BAND]
		Mults += m_NamedMults[i];
[!endif]
[!if ZONE_MULTI_BAND]
		Mults += m_ZoneMults[i];
[!endif]
[!if AYGMULT_MULTI_BAND]
		Mults += m_AygMults[i];
[!endif]
	}
[!endif]

[!if DXCC_SINGLE_BAND]
	Mults += m_DxccMults;
[!endif]
[!if NAMEDMULT_SINGLE_BAND]
	Mults += m_NamedMults;
[!endif]
[!if ZONE_SINGLE_BAND]
	Mults += m_ZoneMults;
[!endif]
[!if AYGMULT_SINGLE_BAND]
	Mults += m_AygMults;
[!endif]

	// *pPtsCount = total points in the log
	long Points = 0;
	for (i = 0; i < NumberOfMultBands(); i += 1)
	{
		Points += m_BandPoints[i];
	}
	*pPtsCount = Points;
	*pMultCount = Mults;
	return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::PtsForQso(QsoPtr_t q, long *pPtsCount)
{
	*pPtsCount = PointsForQso(q);
	return S_OK;
}

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