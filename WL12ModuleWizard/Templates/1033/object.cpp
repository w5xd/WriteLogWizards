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
[!if AM_ROVER]
    const int MYQTH_WID = 6; // TODO
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
[!if AM_ROVER]
    MYQTH_IDX,
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
    { "SNT", RST_WID, CALC_POS_LATER, A_NOTIFY | A_RST | A_OVERSTRIKE },
    { "RST", RST_WID, CALC_POS_LATER, A_PRMPT | A_NOTIFY | A_RST | A_OVERSTRIKE },
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
[!if AM_ROVER]
    {"MYQTH",   MYQTH_WID, CALC_POS_LATER, A_PRMPT|A_NOSPACE|
                                    A_MULTI|A_REQUIRED|A_NOTIFY|A_OVERSTRIKE}, 
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
    , m_namedMults(NUMBER_OF_REGIONS)
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
    , m_MyCountryIndex(-1)
[!endif]
[!if AM_ROVER]
    , fMYQTH(m_qsoFields, MYQTH_IDX)
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
[!if AM_COUNTYLINE]
    , m_countyLineMode(false)
[!endif]
[!if !AM_ROVER]
[!if !NO_NAMEDMULT || !NO_DXCC || !NO_ZONE || !NO_AYGMULT]
    , m_dupeSheet()
[!endif]
[!else]
    , m_currentDupeSheet(0)
[!endif]
    , m_NumberOfDupeSheetBands(0)
{
 [!if AM_ROVER]
    m_dupeSheets.push_back(std::make_shared<CDupeSheet>());
 [!endif]   
}

[!if !NO_NAMEDMULT]
namespace {
    const char * const IniFileName = "[!output COCLASS].ini";  //FIXME
    const char *SectionNames[[!output MM_CLASS_NAME]::NUMBER_OF_REGIONS] = {
        "[!output COCLASS]", // FIXME
[!if NAMED_MULTI_REGION]
        "[!output COCLASS]", // FIXME
[!endif]
    };
    const char* PageTitles [[!output MM_CLASS_NAME]::NUMBER_OF_REGIONS] = {
        "TODO", // FIXME
[!if NAMED_MULTI_REGION]
        "TODO", // FIXME
[!endif]
    };
    const int MessageResourceIDs[[!output MM_CLASS_NAME]::NUMBER_OF_REGIONS] = {
            IDS_NEWNAMED_MSG, // FIXME
[!if NAMED_MULTI_REGION]
            IDS_NEWNAMED_MSG, // FIXME
[!endif]
    };
}
[!endif]

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
    for (short region = 0; region < NUMBER_OF_REGIONS; region += 1)
    {
        if (SUCCEEDED(hr))
            hr = m_namedMults[region].m_pNamedMults.CoCreateInstance(__uuidof(NmdMul),
                0, CLSCTX_SERVER);
	    // TODO--you must create an INI file, make sure its in WriteLog's 
	    // \programs folder, and make sure the following names it correctly.
        if (SUCCEEDED(hr))
            hr = m_namedMults[region].m_pNamedMults->put_FileName(reinterpret_cast<const unsigned char *>(IniFileName));
        if (SUCCEEDED(hr))
            m_namedMults[region].m_pNamedMults->Init(reinterpret_cast<const unsigned char *>(SectionNames[region]));
	    if (SUCCEEDED(hr))
	        hr = m_namedMults[region].m_pNamedMults->get_MultCount(&m_namedMults[region].m_NumNamed);
    }
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
    for (short region = 0; region < NUMBER_OF_REGIONS; region += 1)
    {
        m_namedMults[region].m_NamedDisplayEntry.Release();
        m_namedMults[region].m_NamedDisplay.Release();
        m_namedMults[region].m_pNamedMults.Release();
    }
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
    /* Called by WriteLog on its File/New with b and e non-zero
    ** Called on File/Open with b and e set to zero
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

[!if AM_ROVER]
[!output MM_CLASS_NAME]::CDupeSheet & [!output MM_CLASS_NAME]::dupeSheetFromQso(QsoPtr_t q)
{   // dupe sheet for QSO when logging as a rover
    if (q->DupeSheet > 0 && q->DupeSheet <= m_dupeSheets.size())
        return *m_dupeSheets[q->DupeSheet - 1]; // is valid
    return *m_dupeSheets[0];
}
[!endif]

HRESULT [!output MM_CLASS_NAME]::QsoAdd(QsoPtr_t q)
{
[!if AM_ROVER]
    if (q->DupeSheet > m_dupeSheets.size())
    {   // invalid DupeSheet
        q->DupeSheet = 0;
        return S_FALSE;
    }
    std::string myQth = fMYQTH(q).str();
    std::string key;
    if (q->DupeSheet > 0 && ((key = m_dupeSheets[q->DupeSheet - 1]->key()), !key.empty()))
    {
        if (myQth.empty())
            fMYQTH(q) = key.c_str();
        else if (key != myQth)
        {   // invalid cuz index doesn't match logged QTH
            q->DupeSheet = 0;
            return S_FALSE;
        }
    } else if (!myQth.empty()) // empty means MYQTH for QSO not initialized
    {
        int qthIdx = FindNamed(0, myQth.c_str());
        if (qthIdx < m_namedMults[0].m_NumNamed)
        {   // found the logged MYQTH is in list of valid QTHs
            if (!m_dupeSheets.front()->key().empty())
            {   // make a new dupe sheet
                m_currentDupeSheet = m_dupeSheets.size();
                m_dupeSheets.push_back(std::make_shared<CDupeSheet>());
            }
            m_dupeSheets.back()->setKey(myQth);
            return S_FALSE; // force redupe
        }
        // non-empty MYQTH in QSO, but not a valid county. leave it at zero DupeSheet
    }
[!endif]

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
[!if !NO_NAMEDMULT]
	fMLT(q) = "";
[!endif]
    if (q->dupe == ' ')
    {
[!if !NO_NAMEDMULT || !NO_DXCC || !NO_ZONE || !NO_AYGMULT]
        CDupeSheet &qDupeSheet = dupeSheetFromQso(q);
[!endif]
[!if !NO_DXCC]
        //*********************************
        //Process DXCC 
        int dx = m_DxContext.FillQso(q, fCALL, fCPRF, fCOUNTRY, fAMBF);
        if (dx >= 0)
        {
            int newDxccFlag = !qDupeSheet.MarkCountryWorked(dx, band);
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
        bool foundNamed = false;
        for (short region = 0; region < NUMBER_OF_REGIONS; region++)
        {
		    int newNamedMultFlag = 0;
		    int nm = FindNamed(region, fRCVD(q));
		    if (nm != m_namedMults[region].m_NumNamed)
            {
			    newNamedMultFlag = !qDupeSheet.MarkNamedMultWorked(nm, band, region);
		        if (newNamedMultFlag)
		        {
                    char buf[8];
[!if NAMEDMULT_MULTI_BAND]
			        _itoa_s(++m_namedMults[region].m_NamedMults[band], buf, 10);
[!endif]
[!if NAMEDMULT_SINGLE_BAND]
			        _itoa_s(++m_namedMults[region].m_NamedMults, buf, 10);
[!endif]
                    fMLT(q) = buf;
			        Mult[BAND_SUMMARY_MUL] += 1;
			        if (m_namedMults[region].m_NamedDisplay)
                        m_namedMults[region].m_NamedDisplay->Invalidate(nm);
		        }
                foundNamed = true;
                break;
            }
        }
        if (!foundNamed)
            fMLT(q) = "?";

[!endif]
[!if !NO_ZONE]
		//*********************************
		//Process zones
		int			newZoneFlag = 0;
		int z = FindZone(fZN(q));
		if (z == NUMZONES)
            fZMULT(q) = "?";
		else
			newZoneFlag = !qDupeSheet.MarkZoneWorked(z,band);
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
		else if (ayg < static_cast<int>(m_AygDisplayNames.size()))
			newAygFlag = !qDupeSheet.MarkAygMultWorked(ayg, band);
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
[!if MULTI_MODE]
        unsigned modeIdx = 0;
        if (q->mode == '3')
            modeIdx = BAND_SUMMARY_CW;
[!if RTTY]
        else if (q->mode == '6')
            modeIdx = BAND_SUMMARY_RTTY;
[!endif]
        else
            modeIdx = BAND_SUMMARY_PHONE;
[!if !NO_NAMEDMULT]
        qDupeSheet.OnWorkedBandModeQso(band, modeIdx, 1);
        qDupeSheet.OnWorkedBandModeQso(band, BAND_SUMMARY_MUL, points);
        qDupeSheet.OnWorkedBandModeQso(band, BAND_SUMMARY_MUL + 1, Mult[BAND_SUMMARY_MUL]);
[!endif]
		Mult[modeIdx] = 1;
[!endif]
		if (m_bandSumm)
		{
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

[!if AM_COUNTYLINE]
    if (m_countyLineMode && q->dupe == ' ')
        m_currentDupeSheet = 0;
[!endif]


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
[!if !NO_NAMEDMULT || !NO_DXCC || !NO_ZONE || !NO_AYGMULT]
    CDupeSheet &qDupeSheet = dupeSheetFromQso(q);
[!endif]

    if (q->dupe == ' ')
    {
        points = PointsForQso(q);
[!if MULTI_MODE]
        unsigned modeIdx = 0;
        if (q->mode == '3')
            modeIdx = BAND_SUMMARY_CW;
[!if RTTY]
        else if (q->mode == '6')
            modeIdx = BAND_SUMMARY_RTTY;
[!endif]
        else
            modeIdx = BAND_SUMMARY_PHONE;
		Mult[modeIdx] = -1;

[!endif]
[!if !NO_NAMEDMULT]
        for (short region = 0; region < NUMBER_OF_REGIONS; region++)
        {
            int newNamedMultFlag = 0;
		    int nm = FindNamed(region, fRCVD(q));
		    if (nm != m_namedMults[region].m_NumNamed)
            {
			    newNamedMultFlag = !qDupeSheet.UnmarkNamedMultWorked(nm,band,region);
		        if (newNamedMultFlag)
		        {
[!if NAMEDMULT_MULTI_BAND]
                    m_namedMults[region].m_NamedMults[band] -= 1;
[!endif]
[!if NAMEDMULT_SINGLE_BAND]
                    m_namedMults[region].m_NamedMults -= 1;
[!endif]
			        Mult[BAND_SUMMARY_MUL] -= 1;
			        if (m_namedMults[region].m_NamedDisplay)
                        m_namedMults[region].m_NamedDisplay->Invalidate(nm);
                }
                break;
            }
		}

[!endif]
[!if !NO_DXCC]
		int dx = m_DxContext.CountryFromQsoPrefix(q, fCPRF);
        if (dx >= 0)
		{
            int newDxccFlag = !qDupeSheet.UnmarkCountryWorked(dx,band);
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
			newZoneFlag = !qDupeSheet.UnmarkZoneWorked(z,band);
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
			newAygFlag = !qDupeSheet.UnmarkAygMultWorked(ayg,band);
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
[!if !MULTI_MODE && PTS_COLUMN]
		Mult[BAND_SUMMARY_PTS] = -points;
[!endif]
[!if MULTI_MODE && !NO_NAMEDMULT]
        qDupeSheet.OnWorkedBandModeQso(band, modeIdx, -1);
        qDupeSheet.OnWorkedBandModeQso(band, BAND_SUMMARY_MUL, -points);
        qDupeSheet.OnWorkedBandModeQso(band, BAND_SUMMARY_MUL + 1, Mult[BAND_SUMMARY_MUL]);
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
	{   // TODO
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
[!if !NO_NAMEDMULT||!NO_DXCC||!NO_ZONE||!NO_AYGMULT]
[!if !AM_ROVER]
    m_dupeSheet.InitQsoData();
[!else]
    for (auto pDupe : m_dupeSheets)
        pDupe->InitQsoData();
[!endif]
[!endif]
[!if !NO_NAMEDMULT]
    for (short reg = 0; reg < NUMBER_OF_REGIONS; reg += 1)
        m_namedMults[reg].clear();
[!endif]
[!if !NO_DXCC]
    m_MyCountryIndex = m_DxContext.dxcc_Home();
[!if DXCC_MULTI_BAND]
	m_DxccMults.clear();
[!else]
    m_DxccMults = 0;
[!endif]

[!endif]
[!if !NO_ZONE]
[!if ZONE_MULTI_BAND]
    m_ZoneMults.clear();
[!else]
    m_ZoneMults = 0;
[!endif]

[!endif]
[!if !NO_AYGMULT]
    m_AygDisplayNames.clear();
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
** contains a mode (like CW, SSB, PH or RTTY, or DIG)
** Then the score broadcast assumes the row applies
** only to one mode. */
[!if !NO_DXCC||!NO_NAMEDMULT||!NO_ZONE||!NO_AYGMULT]
		m_bandSumm->SetItemTitle(BAND_SUMMARY_MUL, "Mul");
[!endif]
[!if MULTI_MODE]
		m_bandSumm->SetItemTitle(BAND_SUMMARY_CW, "CW");
		m_bandSumm->SetItemTitle(BAND_SUMMARY_PHONE, "PH");
[!if RTTY]
		m_bandSumm->SetItemTitle(BAND_SUMMARY_RTTY, "DIG");
[!endif]
		for (int i = 0; i < DIM(band_Title); i += 1)
			m_bandSumm->SetBandTitle(i, band_Title[i]);
[!else]
[!if !MULTI_MODE && PTS_COLUMN]
		m_bandSumm->SetItemTitle(BAND_SUMMARY_PTS, "Pts");
[!endif]
[!endif]
    }       
    return S_OK;
}

[!if !NO_NAMEDMULT||!NO_DXCC||!NO_ZONE||!NO_AYGMULT]
void [!output MM_CLASS_NAME]::CDupeSheet::InitQsoData()
{
[!if !NO_NAMEDMULT]
    m_Named.clear();
[!if MULTI_MODE]
    m_bandModeQsos.clear();
[!endif]
[!endif]
[!if !NO_DXCC]
    m_Countries.clear();
[!endif]
[!if !NO_ZONE]
    m_Zones.clear();
[!endif]
[!if !NO_AYGMULT]
    m_AygStatus.clear();
[!endif]
}

[!if !NO_NAMEDMULT||!NO_DXCC||!NO_ZONE||!NO_AYGMULT]
unsigned [!output MM_CLASS_NAME]::CDupeSheet::totalQsos() const
{   // total number of QSOs logged from this QTH
    unsigned ret = 0;
[!if !NO_NAMEDMULT]
    ret += m_Named.total();
[!endif]
[!if DXCC_SINGLE_BAND]
    for (auto n : m_Countries)
        ret += n.second;
[!endif]
[!if DXCC_MULTI_BAND]
    for (auto n : m_Countries)
        for (auto b : n.second)
            ret += b.second;
[!endif]
[!if ZONE_SINGLE_BAND]
    for (auto n : m_Zones)
        ret += n.second;
[!endif]
[!if ZONE_MULTI_BAND]
    for (auto n : m_Zones)
        for (auto b : n.second)
            ret += b.second;
[!endif]
[!if AYGMULT_SINGLE_BAND]
    for (auto n : m_AygStatus)
        ret += n.second;
[!endif]
[!if AYGMULT_MULTI_BAND]
    for (auto n : m_AygStatus)
        for (auto b : n.second)
            ret += b.second;
[!endif]
    return ret;
}

[!endif]
[!if MULTI_MODE && !NO_NAMEDMULT]
int [!output MM_CLASS_NAME]::CDupeSheet::WorkedBandModeQso(short band, short mode) const
{
    auto itor = m_bandModeQsos.find(band);
    if (itor != m_bandModeQsos.end())
    {
        auto itor2 = itor->second.find(mode);
        if (itor2 != itor->second.end())
            return itor2->second;
    }
    return 0;
}
[!endif]

[!if !NO_DXCC]
int [!output MM_CLASS_NAME]::CDupeSheet::MarkCountryWorked(int dx, int band) {
[!if DXCC_SINGLE_BAND]
            return m_Countries[dx]++;
[!endif]
[!if DXCC_MULTI_BAND]
            return m_Countries[band][dx]++;
[!endif]
}
int [!output MM_CLASS_NAME]::CDupeSheet::UnmarkCountryWorked(int dx, int band) {
[!if DXCC_SINGLE_BAND]
            return --m_Countries[dx];
[!endif]
[!if DXCC_MULTI_BAND]
            return --m_Countries[band][dx];
[!endif]
}
bool [!output MM_CLASS_NAME]::CDupeSheet::countryWorked(short dx, short band) const {
[!if DXCC_SINGLE_BAND]
            return m_Countries.worked(dx);
[!endif]
[!if DXCC_MULTI_BAND]
            return m_Countries.worked(band, dx);
[!endif]
}

[!endif]
[!if !NO_ZONE]
int [!output MM_CLASS_NAME]::CDupeSheet::MarkZoneWorked(int zn, int band) {
[!if ZONE_MULTI_BAND]
			return m_Zones[band][zn]++;
[!endif]
[!if ZONE_SINGLE_BAND]
			return m_Zones[zn]++;
[!endif]
}
int [!output MM_CLASS_NAME]::CDupeSheet::UnmarkZoneWorked(int zn, int band) {
[!if ZONE_MULTI_BAND]
			return --m_Zones[band][zn];
[!endif]
[!if ZONE_SINGLE_BAND]
			return --m_Zones[zn];
[!endif]
}
bool [!output MM_CLASS_NAME]::CDupeSheet::zoneWorked(short zn, short band) const {
[!if ZONE_MULTI_BAND]
			return m_Zones.worked(band,zn);
[!endif]
[!if ZONE_SINGLE_BAND]
			return m_Zones.worked(zn);
[!endif]
}

[!endif]
[!if !NO_AYGMULT]
int [!output MM_CLASS_NAME]::CDupeSheet::MarkAygMultWorked(int ayg, int band) {
[!if AYGMULT_MULTI_BAND]
			return m_AygStatus[band][ayg]++;
[!endif]
[!if AYGMULT_SINGLE_BAND]
			return m_AygStatus[ayg]++;
[!endif]
}
int [!output MM_CLASS_NAME]::CDupeSheet::UnmarkAygMultWorked(int ayg, int band) {
[!if AYGMULT_MULTI_BAND]
			return --m_AygStatus[band][ayg];
[!endif]
[!if AYGMULT_SINGLE_BAND]
			return --m_AygStatus[ayg];
[!endif]
}
bool [!output MM_CLASS_NAME]::CDupeSheet::aygWorked(short ayg, short band) const {
[!if AYGMULT_MULTI_BAND]
			return m_AygStatus.worked(band,ayg);
[!endif]
[!if AYGMULT_SINGLE_BAND]
			return m_AygStatus.worked(ayg);
[!endif]
}
[!endif]

[!endif]
[!if CAN_LOG_ROVER]
// we can log rovers, so prior QSO received exchange might not be what we want to prefill
std::set<std::string> [!output MM_CLASS_NAME]::FindAllPreviousValuesForThisCall(unsigned MaxSize, CQsoField &field, QsoPtr_t q)
{
	unsigned long QsoNumber;
    std::set<std::string> PrevQsoRcvd;
	// Logging rovers. if we find this guy always under the same county,
	// then copy that county
    std::string testCall = fCALL(q);
    CQsoSearchMatchHelper savedQsoMatch(m_qsoSearchMatch,
        [&PrevQsoRcvd, &testCall, &field, this] (QsoPtr_t OldQ, int *IsGood) 
        {   // unless there is recursion replacing m_qsoSearchMatch,testCall should always match fCALL(OldQ).
            if (testCall == fCALL(OldQ).str())
            {
					PrevQsoRcvd.insert(field(OldQ).str());
                    *IsGood = 1; // make writelog gives us all the QSOs with this call
            }
        });
	for (int j = 0; j < m_NumberOfDupeSheetBands; j += 1)
	{
[!if AM_ROVER]
        for (unsigned k = 0; k < m_dupeSheets.size(); k++)
        {
            unsigned dupeSheetIdx = k + 1;    // k+1 cuz q->DupeSheet is offset one above m_dupeSheets idx
[!else]
            unsigned dupeSheetIdx = 0;
            {
[!endif]
			    m_Parent->SearchDupeSheet(q, j, dupeSheetIdx,  &QsoNumber);
				if (PrevQsoRcvd.size() >= MaxSize)
                    return PrevQsoRcvd;
[!if !AM_ROVER]
            }
[!else]
        }
[!endif]
    }
    return PrevQsoRcvd;
}

[!endif]
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
[!if !CAN_LOG_ROVER]
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
[!endif]
   
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
    if (fCALL == p) 
    {
        if (fRCVD(q).empty()) 
        {
            if (!fCALL(q).empty())
            {
                if (canWrite)
                {
[!if !CAN_LOG_ROVER]
                    if (OldQ)
		            {
                        if (canWrite)    // fill in new field from old qso
                            fRCVD(q).assign(fRCVD(OldQ));
                        if (!fRCVD(q).empty())
			                flagNamed = true; // and check whether fRCVD is new mult
		            }
[!else]
                    // Deal with possibility we have logged this guy as a rover
                    auto PrevQsoRcvd = FindAllPreviousValuesForThisCall(2, fRCVD, q);
                    if (PrevQsoRcvd.size() == 1) // only if exactly one RCVD value was found do we set it here
                    {
                        fRCVD(q)= PrevQsoRcvd.begin()->c_str();
                        flagNamed = true;
                    }
[!endif]
                }
            }
            ret = 0;
        }
[!if AM_COUNTYLINE]
        if (canWrite && m_countyLineMode)
        {
            if (!fCALL(q).empty())
            {   // County line--call filled in
                for (unsigned i = 0; i < m_dupeSheets.size(); i++)
                {
                    unsigned long QsoNumber;
                    // dupe sheet idx one higher that m_dupeSheets idx
                    if (m_Parent->SearchDupeSheet(q, q->band, i+1, &QsoNumber) != S_OK)
                    {
                        //switch to first county we have not logged this guy
                        bool diff = i != m_currentDupeSheet;
                        m_currentDupeSheet = i;
                        if (diff)
                        {
[!if !NO_DXCC]
                            m_DxccContainer.InvalidateAll();
[!endif]
[!if !NO_NAMEDMULT]
                            for (short region = 0; region < NUMBER_OF_REGIONS; region += 1)
                                if (m_namedMults[region].m_NamedDisplay)
                                    m_namedMults[region].m_NamedDisplay->Invalidate(-1);
[!endif]
                        }
                        break;                    
                    }
                }
            }
[!if !NO_NAMEDMULT]
            else if (fRCVD(q).empty())
            {   // county line mode. Call empty and fRCVD empty
                // fill in call from previous QSO on this band and for next of our counties
                    unsigned long n = 0;
                    m_Parent->NumberQsos(&n);
                    while (n != 0)
                    {
                        QsoPtr_t lastQ = 0;
                        m_Parent->QsoIth(n-1, &lastQ);
                        if (lastQ && (lastQ->band == q->band))
                        {
                            for (unsigned i = 0; i < m_dupeSheets.size(); i++)
                            {
                                unsigned long QsoNumber;
                                if (m_Parent->SearchDupeSheet(
                                        lastQ, 
                                        q->band, 
                                        i+1, &QsoNumber) != S_OK)
                                {   // previously logged station not logged for this county
                                    bool diff = m_currentDupeSheet != i;
                                    m_currentDupeSheet = i;
                                    fCALL(q) = fCALL(lastQ).str();
                                    fRCVD(q)= fRCVD(lastQ).str();
                                    if (diff)
                                    {
[!if !NO_DXCC]
                                        m_DxccContainer.InvalidateAll();
[!endif]
[!if !NO_NAMEDMULT]
                                        for (short region = 0; region < NUMBER_OF_REGIONS; region += 1)
                                            if (m_namedMults[region].m_NamedDisplay)
                                                m_namedMults[region].m_NamedDisplay->Invalidate(-1);
[!endif]
                                    }
                                    break;
                                }
                            }
                            break;
                        }
                        n -= 1;
                    }
            }
[!endif]
        }
[!endif]
    }
    if (flagNamed || fRCVD == p)      /*or received*/
    {
        for (short region = 0; region < NUMBER_OF_REGIONS; region += 1)
        {
            int n = FindNamed(region, fRCVD(q));
            if (n != m_namedMults[region].m_NumNamed)
            {
                int NamedWorked = get_MultWorked(NAMED_MULT_ID+region, n, band) == S_FALSE ? 1 : 0;
                if (ret <= 0)
                    ret = NamedWorked;
                if (NamedWorked && msgValid && (Message != 0)) // FIXME. different message per region
                    LoadString(g_hInstance, MessageResourceIDs[region], Message, MAX_MULT_MESSAGE_LENGTH);
                break;
            }
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
[!if !CAN_LOG_ROVER]
        std::string aygVal;
        if (OldQ)
            aygVal = fAYG(OldQ);
        if (fAYG(q).empty() && canWrite) // fill in fAYG from OldQ
            fAYG(q) = aygVal.c_str();
        if (!fAYG(q).empty())
            flagAyg = true;
[!else]
        // Deal with possibility we have logged this guy as a rover
        auto PrevQsoRcvd = FindAllPreviousValuesForThisCall(2, fAYG, q);
        if (PrevQsoRcvd.size() == 1) // only if exactly one RCVD value was found do we set it here
        {
            fAYG(q)= PrevQsoRcvd.begin()->c_str();
            flagAyg = true;
        }
[!endif]
	}

    if (flagAyg || fAYG == p)    /*this multiplier*/
	{
		int NewAyg = 0;
		int ayg = FindAyg(FALSE, fAYG(q));
		NewAyg = currentDupeSheet().aygWorked(ayg, band) ? 0 : 1;
		if (ret <= 0)
			ret = NewAyg;
		if (NewAyg && msgValid && (Message != 0))
			LoadString(g_hInstance, IDS_AYG_MSG,	Message, MAX_MULT_MESSAGE_LENGTH);
	}

[!endif]
[!if AM_ROVER]
    // Am logging as rover from multiple QTHs
    std::string myQth = fMYQTH(q).str();
    if (fMYQTH == p)
    {   // The column that sets the QTH we are currently in has been touched.
        ret = -1;
        if (!myQth.empty())
        {   // if this logged-from QTH appears in dupe sheets, then make it current
            unsigned i;
            for (i = 0; i < m_dupeSheets.size(); i++)
            {
                if (m_dupeSheets[i]->key() == myQth)
                {
                    if (canWrite)
                    {
                        bool diff = m_currentDupeSheet != i;
                        m_currentDupeSheet = i;
                        if (diff)
                        {
[!if !NO_DXCC]
                        m_DxccContainer.InvalidateAll();
[!endif]
[!if !NO_NAMEDMULT]
                        for (short region = 0; region < NUMBER_OF_REGIONS; region += 1)
                            if (m_namedMults[region].m_NamedDisplay)
                                m_namedMults[region].m_NamedDisplay->Invalidate(-1);
[!endif]
                        }
                    }
                    ret = 0;
                    break;
                }
            }
            if (ret != 0)
            {   // was not in currently logged. is it some other valid QTH?
                int n = FindNamed(0, myQth.c_str());
                if (n < m_namedMults[0].m_NumNamed)
                    ret = 0;
            }
        }
        else if (canWrite)
        {   // empty MYQTH being checked. invoke dialog
            InvokeQthSelectDlg();
            if (!m_dupeSheets[m_currentDupeSheet]->key().empty())
                ret = 0;
        }
    }
    if (canWrite && myQth.empty())
        fMYQTH(q) = m_dupeSheets[m_currentDupeSheet]->key().c_str();
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
        m_MultDispContainer.CoCreateInstance(
            L"writelog.multdisp", 0, CLSCTX_ALL);

    if (m_MultDispContainer)
	{
[!if !NO_NAMEDMULT]
        for (short region = 0; region < NUMBER_OF_REGIONS; region += 1)
        {
            SetupNamedDisplay(region, PageTitles[region], m_MultDispContainer,
                m_namedMults[region].m_pNamedMults,
[!if NAMEDMULT_MULTI_BAND]
                NumberOfMultBands()
[!endif]
[!if NAMEDMULT_SINGLE_BAND]
                1
[!endif]
            );
        }
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
[!if !NO_ZONE]
        SetupZoneDisplay("TODO", m_MultDispContainer,
[!if ZONE_MULTI_BAND]
								NumberOfMultBands()
[!endif]
[!if ZONE_SINGLE_BAND]
								1
[!endif]
                                );
[!endif]
[!if !NO_AYGMULT]
        SetupAygDisplay("TODO", m_MultDispContainer,
[!if AYGMULT_MULTI_BAND]
                    NumberOfMultBands()
[!endif]
[!if AYGMULT_SINGLE_BAND]
                    1
[!endif]
                    );
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
        m_MultDispContainer->ShowCurrent();
    }
[!endif]
    return S_OK;
}

[!if AM_ROVER]
void [!output MM_CLASS_NAME]::InvokeQthSelectDlg()
{
    [!output MM_ROVERDLG_CLASS_NAME] Dlg(
        [this] ([!output MM_ROVERDLG_CLASS_NAME] &dlg)
        {
            auto allQTHs = dlg.GetDlgItem(IDC_QTHLIST1);
             // TODO fill in the dialog CreateNew
[!if NO_NAMEDMULT]
            allQTHs.SendMessageA(LB_ADDSTRING, 0, (LPARAM) "FIRST QTH"); // FIXME
[!else]
            for (int i = 0; i < m_namedMults[0].m_NumNamed; i++)
            {   // FIXME
                unsigned char *name;
                m_namedMults[0].m_pNamedMults->NameFromIndex(i, &name);
                allQTHs.SendMessageA(LB_ADDSTRING, 0, (LPARAM) name);
            }
[!endif]
            // fill in the UseExisting
            auto usedQTHs = dlg.GetDlgItem(IDC_QTH_OLD_LIST);
            for (auto ds : m_dupeSheets)
                usedQTHs.SendMessageA(LB_ADDSTRING, 0, (LPARAM)ds->title().c_str());
       }
    );

[!if AM_COUNTYLINE]
    Dlg.m_countyLineMode = m_countyLineMode ? 1 : 0;
    for (;;) // keep dialog up in county line mode for multiple "Add" button presses
    {
[!endif]
    auto result = Dlg.DoModal();
    switch (result)
    {
    case IDC_CREATE_NEW:
        for (auto ds : m_dupeSheets)
        {
            if (ds->title() == Dlg.m_result)
                goto UseExisting;
        }
        {
            auto QthDupeSheet = m_dupeSheets[0];
            if (!QthDupeSheet->key().empty())
            {
                QthDupeSheet = std::make_shared<CDupeSheet>();
                m_dupeSheets.push_back(QthDupeSheet);
            }
            QthDupeSheet->setKey(Dlg.m_result);
        }
        // fall through
    case IDC_USE_EXISTING:
    UseExisting:
[!if AM_COUNTYLINE]
        m_countyLineMode = Dlg.m_countyLineMode != 0;
[!endif]
        for (unsigned i = 0; i < m_dupeSheets.size(); i++)
        {
            if (m_dupeSheets[i]->title() == Dlg.m_result)
            {
                bool diff = m_currentDupeSheet != i;
                m_currentDupeSheet = i;
                if (diff)
                {
[!if !NO_DXCC]
                        m_DxccContainer.InvalidateAll();
[!endif]
[!if !NO_NAMEDMULT]
                        for (short region = 0; region < NUMBER_OF_REGIONS; region += 1)
                            if (m_namedMults[region].m_NamedDisplay)
                                m_namedMults[region].m_NamedDisplay->Invalidate(-1);
[!endif]
                }
                break;
            }
        }
        break;
[!if AM_COUNTYLINE]
        default:
            return;
        }

        if (!m_countyLineMode || result == IDC_USE_EXISTING)
            break;
[!endif]
    }
}
[!endif]


[!if !NO_NAMEDMULT]
void [!output MM_CLASS_NAME]::SetupNamedDisplay(short region, const char *title, IMultDisplayContainer*pDispc, IWlNamedMult *pNamedMults, int numMultBands)
{
    if (!m_namedMults[region].m_NamedDisplay && pDispc)
            pDispc->MakeDisplay(1, 0,
                            __uuidof(IMultDisplayPage),
                            (IUnknown **)&m_namedMults[region].m_NamedDisplay);
    if (m_namedMults[region].m_NamedDisplay)
    {
        if (!m_namedMults[region].m_NamedDisplayEntry)
        {
            m_namedMults[region].m_NamedDisplay->put_Title(title);
            CComObject<CNamedDisplayHelperPage<[!output MM_CLASS_NAME]> > *pTemp(0);
            if (SUCCEEDED(pTemp->CreateInstance(&pTemp)))
            {
                pTemp->setRegion(NAMED_MULT_ID + region);
                pTemp->Init(pNamedMults,
                    m_namedMults[region].m_NamedDisplay,
                    numMultBands,
                    this);
                m_namedMults[region].m_NamedDisplayEntry = pTemp;
            }
        }
    }
}
[!endif]

[!if !NO_AYGMULT]
void [!output MM_CLASS_NAME]::SetupAygDisplay(const char *title, IMultDisplayContainer*pDispc, int numMultBands)
{
    if (!m_AygDisplay && pDispc)
			pDispc->MakeDisplay(1, 0,
							__uuidof(IMultDisplayPage),
							(IUnknown **)&m_AygDisplay);

	if (m_AygDisplay)
	{
		if (!m_AygDisplayEntry)
		{
			m_AygDisplay->put_Title(title);
            CComObject<CAygDisplayHelper<[!output MM_CLASS_NAME], AYG_MULT_ID> > *pTemp(0);
            if (SUCCEEDED(pTemp->CreateInstance(&pTemp)))
            {
                pTemp->Init(this,
                    numMultBands,
                    m_AygDisplay);
                m_AygDisplayEntry = pTemp;
            }
		}
        CComQIPtr<IMultDisplayPageOptions> opt = m_AygDisplay;
        opt->put_Sort(1);
    }
}
[!endif]

[!if !NO_ZONE]
void [!output MM_CLASS_NAME]::SetupZoneDisplay(const char *title, IMultDisplayContainer*pDispc, int numMultBands)
{
    if (!m_ZoneDisplay && pDispc)
            pDispc->MakeDisplay(1, 0,
                    __uuidof(IMultDisplayPage),
                    (IUnknown **)&m_ZoneDisplay);

	if (m_ZoneDisplay)
    {
        if (!m_ZoneDisplayEntry)
        {
            m_ZoneDisplay->put_Title(title);
            CComObject<CZoneDisplayHelper<[!output MM_CLASS_NAME], ZONE_MULT_ID> > *pTemp(0);
            if (SUCCEEDED(pTemp->CreateInstance(&pTemp)))
            {
                pTemp->Init(g_ZoneList,
                                DIM(g_ZoneList),
                                m_ZoneDisplay,
                                numMultBands,
								this);
                m_ZoneDisplayEntry = pTemp;
            }
        }
    }}
[!endif]

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

[!if AM_ROVER]
    // InvokeQthSelectDlg(); TODO for Rover. Maybe this is appropriate?
[!endif]

    // TODO Put up a parameter selection dialog.
    [!output MM_DLG_CLASS_NAME] Dlg;
[!if AM_COUNTYLINE]
    Dlg.m_countyLineMode = m_countyLineMode ? 1 : 0;
[!endif]
[!if !NO_NAMEDMULT]
    Dlg.m_myNamed = currentDupeSheet().key();
[!endif]
    if (Dlg.DoModal(Window) == IDOK)
    {
[!if AM_COUNTYLINE]
        m_countyLineMode = Dlg.m_countyLineMode != 0;
[!endif]
[!if !NO_NAMEDMULT]
        currentDupeSheet().setKey(Dlg.m_myNamed);
[!endif]
    }
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::GetModuleData(long * Data)
{
// MODULE_DATA_SCORE   Enables WriteLog's Contest/Parameter-Setup menu item.
// MODULE_DATA_NOBSM   Without this flag, WriteLog does both: (a) sets first column QSO count and (b) creates per-band rows if g_Bands has only one entry
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
/*When a QSO is added to the database, WriteLog matches the
**new QSO's call and band against all those already in the
**database. If it finds a match of those things, it sets the
**dupe member of the New QSO to 'D' and calls the multiplier 
**modules's MatchQso member. The multiplier module is allowed
**to change the 'D' to a space character, which means that
**even though the call and band match between the New and Old
**QSOs, there is some other reason that the New QSO should be
**counted and scored.
**
**If there is more than one QSO in the database that matches
**the new one's call and band, then WL calls here with each
**of the QSOs in the "Old" argument, and WL considers the
**new QSO a dupe if any one of those calls returns a dupe.	*/
HRESULT [!output MM_CLASS_NAME]::MatchedQso(QsoPtr_t New, QsoPtr_t Old)
{
    if (New == 0 || Old == 0)
        return E_POINTER;
[!if CAN_LOG_ROVER]
    int IsGood(0);
    QsoSearch(New, Old, &IsGood);
    if (IsGood)
         New->dupe  = ' ';
[!endif]
    return S_OK;
}

/*It is often necessary to see if a callsign has been worked
**on a given band--whether or not a QSO is about to be logged
**for that call and band. WriteLog searches its QSO database
**by setting up a struct qso_stru with the appropriate call
**and band and other exchange info, and searches through
**its QSO database for matches. For every QSO already in the
**database that matches the canddiate call/band (at NewQ),
**WriteLog offers the multiplier module a chance to say that
**there is some other reason for the QSO to not be considered
**a dupe and sets *IsGood = 1. */
HRESULT [!output MM_CLASS_NAME]::QsoSearch(QsoPtr_t NewQ, QsoPtr_t OldQ, int * IsGood)
{
    if (IsGood==0 || NewQ == 0 || OldQ == 0)
        return E_POINTER;
    *IsGood = 0;
[!if CAN_LOG_ROVER]
[!if !NO_NAMEDMULT && !NO_AYGMULT]
// FIXME Wizard was told to log roving stations as not dupes. But
// Rovers might be in the Named mults or in the AYG mults. 
[!endif]
[!if !NO_NAMEDMULT]
    // Rovers are only allowed in zeroth region
    if ((FindNamed(0, fRCVD(NewQ).str()) != m_namedMults[0].m_NumNamed) &&
        (FindNamed(0, fRCVD(OldQ).str()) != m_namedMults[0].m_NumNamed) &&
        (strcmp(fRCVD(NewQ).str(), fRCVD(OldQ).str()) != 0))
        *IsGood = 1;
[!endif]
[!if !NO_AYGMULT]
   if ((FindAyg(FALSE, fAYG(NewQ).str()) < static_cast<int>(m_AygDisplayNames.size())) &&
        (FindAyg(FALSE, fAYG(OldQ).str()) < static_cast<int>(m_AygDisplayNames.size())) &&
        (strcmp(fAYG(NewQ).str(), fAYG(OldQ).str()) != 0))
			*IsGood = 1;
[!endif]
    if (m_qsoSearchMatch)   // rover has callback during QsoSearch
        m_qsoSearchMatch(OldQ, IsGood);
[!endif]
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::SetDupeSheet(QsoPtr_t q, int * DupeSheet)
{
	// WriteLog maintains as many separate dupe sheets as the multiplier
	// module needs. Only QSOs that have matching DupeSheet values are
	// duped against each other.
   if (!DupeSheet)
       return E_POINTER;
   *DupeSheet = 0;
[!if AM_ROVER]
    // This supports, for example, VHF rovers and county rovers. They do NOT need to start a
    // new .wl file on changing grid square. Instead, they enter a different
    // transmitted grid square the module tells WL to create a new dupe
    // sheet for that grid square.
    // TODO ***********************************************
    if (q->DupeSheet != 0)
        *DupeSheet = q->DupeSheet; // leave nonzero alone
    else
    {   // dupe sheet is not among m_dupeSheets
        unsigned i = m_currentDupeSheet;
        std::string myQth = fMYQTH(q).str();
        if (myQth.empty())  // MYQTH not set
            fMYQTH(q) = m_dupeSheets[i]->title().c_str();
        else
        {   // if fMYQTH matches a dupesheet, then put q in it
            for (i = 0; i < m_dupeSheets.size(); i++)
            {
                if (myQth == m_dupeSheets[i]->key().c_str())
                    break;            
            }
            if (i == m_dupeSheets.size())
                return S_FALSE; // Invalid MYQTH. leave q in zeroth dupe sheet
        }
        *DupeSheet = i + 1; // q->DupeSheet is offset to one more than index into m_dupeSheets
    }
[!endif]
    return S_OK;
}
HRESULT [!output MM_CLASS_NAME]::DupeSheetTitle(int DupeSheet, char * Title, int TitleLength)
{
[!if AM_ROVER]
    unsigned idx = DupeSheet - 1;
    if (idx >= static_cast<int>(m_dupeSheets.size()))
        return E_INVALIDARG;
    strncpy_s(Title, TitleLength, m_dupeSheets[idx]->title().c_str(), TitleLength);
[!else]
[!if !NO_NAMEDMULT || !NO_DXCC || !NO_ZONE || !NO_AYGMULT]
    strncpy_s(Title, TitleLength, currentDupeSheet().title().c_str(), TitleLength);
[!else]
    strncpy_s(Title, TitleLength, "TODO", TitleLength);
[!endif]
[!endif]
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
[!if !NO_NAMEDMULT]
    if (id >= NAMED_MULT_ID && id < NAMED_MULT_ID + NUMBER_OF_REGIONS)
        return currentDupeSheet().namedWorked(Mult, band, id-NAMED_MULT_ID) ? S_OK : S_FALSE;
[!endif]

    switch (id)
    {
[!if !NO_DXCC]
    case DXCC_MULT_ID:
            return currentDupeSheet().countryWorked(Mult,band) ? S_OK : S_FALSE;
[!endif]
[!if !NO_ZONE]
    case ZONE_MULT_ID:
            return currentDupeSheet().zoneWorked(Mult, band) ? S_OK : S_FALSE;
[!endif]
[!if !NO_AYGMULT]
    case AYG_MULT_ID:
            return currentDupeSheet().aygWorked(Mult, band) ? S_OK : S_FALSE;
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
    if (!Title)
        return E_POINTER;
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
    short region,
	const char *c)
{
	short i;
	if (m_namedMults[region].m_pNamedMults->IndexFromName(reinterpret_cast<const unsigned char *>(c), &i) != S_OK)
		return m_namedMults[region].m_NumNamed;
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
    if (q == 0 || s == 0 || Offset == 0)
        return E_POINTER;
	//TODO
    //		*Offset = fCALL.FieldNumber();
	//		return NOERROR;

    return E_NOTIMPL;
}

HRESULT  [!output MM_CLASS_NAME]::IsCharOKHere(QsoPtr_t q, char c, short Offset)
{
    //TODO. Example:
    // if (fRCVD == Offset)
    //{
    //    if (!isalpha(c))
    //        return S_FALSE;
    //}
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
[!if !NO_NAMEDMULT || !NO_AYGMULT]
		|| currentDupeSheet().needInit()
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
		wsprintf(Buf, "%-6.6s ", currentDupeSheet().key().c_str());
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

[!if TQSL_ROVER]
// IWlogTQslQsoLocation
// FIXME. This interface implementation works for USA state QSO parties.
HRESULT [!output MM_CLASS_NAME]::GetNumLocationFields(long* pNumfields)
{
#if GRID_SQUARE_SUPPORT     // FIXME
    *pNumfields = m_dupeSheets.size() > 1 ? 1 : 0;
#else
    *pNumfields = m_dupeSheets.size() > 1 ? 2 : 0;
#endif
    return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::GetFieldGABBIName(long Numfield, char* pFieldName, long FieldNameSize)
{
#if GRID_SQUARE_SUPPORT     // FIXME
    if (Numfield != 0)
        return E_INVALIDARG;
    strncpy_s(pFieldName, FieldNameSize, "GRIDSQUARE", FieldNameSize - 1);
#else
    switch (Numfield)
    {  // order is important. STATE first
    case 0:
        strncpy_s(pFieldName, FieldNameSize, "US_STATE", FieldNameSize - 1);
        break;
    case 1:
        strncpy_s(pFieldName, FieldNameSize, "US_COUNTY", FieldNameSize - 1);
        break;
    default:
        return E_INVALIDARG;
    }
#endif
    return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::GetQsoNumLocations(QsoPtr_t q, long* pNumLocations)
{
    *pNumLocations = 1;
    return S_OK;
}

HRESULT [!output MM_CLASS_NAME]::GetQsoLocationField(QsoPtr_t q, long Location, long FieldNum,
    char* pFieldvalue, long FieldValueSize)
{
    if (Location != 0)
        return E_INVALIDARG;

#if GRID_SQUARE_SUPPORT   // FIXME
    if (FieldNum != 0)
        return E_INVALIDARG;
    strncpy_s(pFieldvalue, FieldValueSize, fMYQTH(q).str(), FieldValueSize - 1);
    return S_OK;
#else
    switch (FieldNum)
    {
    case 0:
        // full spelling of state name StateName
        strncpy_s(pFieldvalue, FieldValueSize, "StateName"/*FIXME*/, FieldValueSize - 1);
        return S_OK;
    case 1:
        {
        std::vector<char> NmdMul(512);
        GetPrivateProfileString("Multipliers",
            "Location",
            "",
            &NmdMul[0],
            NmdMul.size(),
            "WriteLog.ini");
        std::string fn = &NmdMul[0];
        fn += IniFileName;
        // This INI IniSection section must be of the form:
        // county_abbreviation=county_full_spelling
        ::GetPrivateProfileString("IniSection"/*FIXME*/, fMYQTH(q).str(), "", pFieldvalue, FieldValueSize, fn.c_str());
        return S_OK;
        }
    default:
        return E_INVALIDARG;
    }
#endif
}
[!endif]

//IWlogScoreInfo
HRESULT [!output MM_CLASS_NAME]::MultsAndPts(long *pMultCount, long *pPtsCount)
{
	//TODO--Check if score really is mults * pts here...
	// *pMultCount = total of mults in the log
	long Mults = 0;
[!if DXCC_MULTI_BAND]
    for (auto dxm : m_DxccMults)
		Mults += dxm.second;
[!endif]
[!if ZONE_MULTI_BAND]
    for (auto zm : m_ZoneMults)
		Mults += zm.second;
[!endif]
[!if AYGMULT_MULTI_BAND]
    for (auto am : m_AygMults)
        Mults += am.second;
[!endif]
[!if !NO_NAMEDMULT]
    for (short region = 0; region < NUMBER_OF_REGIONS; region += 1)
        Mults += m_namedMults[region].total();
[!endif]

[!if DXCC_SINGLE_BAND]
	Mults += m_DxccMults;
[!endif]
[!if ZONE_SINGLE_BAND]
	Mults += m_ZoneMults;
[!endif]
[!if AYGMULT_SINGLE_BAND]
	Mults += m_AygMults;
[!endif]

	// *pPtsCount = total points in the log
	long Points = 0;
    for (auto bp : m_BandPoints)
        Points += bp.second;
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
[!if !NO_NAMEDMULT || !NO_DXCC || !NO_ZONE || !NO_AYGMULT]
[!if AM_ROVER]

    m_dupeSheets.clear();
    short readlen = 0;
    static_assert(sizeof(readlen) == 2, "use 2 byte type");
    bytesread = 0;
    if (SUCCEEDED(hr))
        hr = pStream->Read(&readlen, sizeof(readlen), &bytesread);
    if (bytesread == sizeof(readlen))
    {
        auto toRead = readlen;
        for (short i = 0; i < toRead && SUCCEEDED(hr); i++)
        {
            hr = pStream->Read(&readlen, sizeof(readlen), &bytesread);
            if (bytesread == sizeof(readlen) && readlen != 0)
            {
                std::vector<char> buf(readlen);
                hr = pStream->Read(&buf[0], buf.size(), &bytesread);
                if (bytesread == buf.size())
                {
                    std::string key;
                    for (auto c : buf)
                        if (c)
                            key.push_back(c);
                        else break;
                    m_dupeSheets.push_back(std::make_shared<CDupeSheet>());
                    m_dupeSheets.back()->setKey(key);
                }
                else
                    break;
            }
            else
                break;
        }
    } 
    if (m_dupeSheets.empty())
        m_dupeSheets.push_back(std::make_shared<CDupeSheet>());
[!else]
[!if !NO_NAMEDMULT]
    short MyMultLen;
    if (SUCCEEDED(hr))
        hr = pStream->Read(&MyMultLen, sizeof(MyMultLen), &bytesread);
    if (SUCCEEDED(hr) && bytesread != 2)
        hr = E_UNEXPECTED;
    if (SUCCEEDED(hr))
    {
        std::string key;
        if (MyMultLen)
        {
            std::vector<char> buf(MyMultLen);
            hr = pStream->Read(&buf[0], MyMultLen, &bytesread);
            if (SUCCEEDED(hr) && bytesread != MyMultLen)
                hr = E_UNEXPECTED;
            key.assign(buf.begin(), buf.end());
        }
        currentDupeSheet().setKey(key);
    }
[!endif]
[!endif]
[!endif]
[!if AM_COUNTYLINE]
    char v = 0;
    if (SUCCEEDED(hr) && (SUCCEEDED(hr = pStream->Read(&v, 1, &bytesread)) && bytesread == 1))
        m_countyLineMode = v != 0;
[!endif]
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
[!if !NO_NAMEDMULT || !NO_DXCC || !NO_ZONE || !NO_AYGMULT]
[!if AM_ROVER]

    short numDupeSheets = static_cast<short>(m_dupeSheets.size());
    static_assert(sizeof(numDupeSheets) == 2, "use 2 byte type");
    short toWrite = 0;
    for (auto ds : m_dupeSheets)
        if (ds->totalQsos() != 0)
            toWrite += 1;
    if (SUCCEEDED(hr))
        hr = pStream->Write(&toWrite, sizeof(toWrite), &written);
    short strlen;
    for (auto ds : m_dupeSheets)
    {
        if (ds->totalQsos() == 0)
            continue;
        std::string key = ds->key();
        strlen = static_cast<short>(key.length());
        if (SUCCEEDED(hr))
            hr = pStream->Write(&strlen, sizeof(strlen), &written);
        if (SUCCEEDED(hr))
            hr = pStream->Write(&key[0], strlen, &written);
    }
[!else]
[!if !NO_NAMEDMULT]
    short MyMultLen = static_cast<short>(currentDupeSheet().key().size());
    static_assert(sizeof(short) == 2, "write 2 bytes size");
    if (SUCCEEDED(hr))
        hr = pStream->Write(&MyMultLen, sizeof(MyMultLen), &written);
    if (SUCCEEDED(hr) && MyMultLen)
        hr = pStream->Write(currentDupeSheet().key().c_str(), MyMultLen, &written);
[!endif]
[!endif]
[!endif]
[!if AM_COUNTYLINE]

    char v = m_countyLineMode ? 1 : 0;
    if (SUCCEEDED(hr))
        hr = pStream->Write(&v, 1, &written);

[!endif]
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
        { 180000,  200000, FSK_MASK, "160M DIG"},
[!endif]
        { 350000,  400000, PHONE_MASK, "80M PH"},
        { 350000,  400000, CW_MASK, "80M CW"},
[!if RTTY]
        { 350000,  400000, FSK_MASK, "80M DIG"},
[!endif]
        { 700000,  730000, PHONE_MASK, "40M PH"},
        { 700000,  730000, CW_MASK, "40M CW"},
[!if RTTY]
        { 700000,  730000, FSK_MASK, "40M DIG"},
[!endif]
        {1400000, 1435000, PHONE_MASK, "20M PH"},
        {1400000, 1435000, CW_MASK, "20M CW"},
[!if RTTY]
        {1400000, 1435000, FSK_MASK, "20M DIG"},
[!endif]
        {2100000, 2145000, PHONE_MASK, "15M PH"},
        {2100000, 2145000, CW_MASK, "15M CW"},
[!if RTTY]
        {2100000, 2145000, FSK_MASK, "15M DIG"},
[!endif]
        {2800000, 2970000, PHONE_MASK|FM_MASK|AM_MASK, "10M PH"},
        {2800000, 2970000, CW_MASK, "10M CW"},
[!if RTTY]
        {2800000, 2970000, FSK_MASK, "10M DIG"},
[!endif]
		{5000000, 5400000, PHONE_MASK|FM_MASK|AM_MASK, "6M PH"},
		{5000000, 5400000, CW_MASK, "6M CW"},
[!if RTTY]
        {5000000, 5400000, FSK_MASK, "6M DIG"},
[!endif]
		{14400000,14800000, PHONE_MASK|FM_MASK|AM_MASK, "2M PH"},
		{14400000,14800000, CW_MASK, "2M CW"},
[!if RTTY]
        {14400000,14800000, FSK_MASK, "2M DIG"},
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