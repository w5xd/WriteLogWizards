#ifndef IWRITELG_H
#define IWRITELG_H

#include "condefs.h"
#include "config.h"
#include "wl_guid.h"

typedef const struct band_stru FAR * ConstBandPtr_t;
typedef const struct exfa_stru FAR* ConstExfPtr_t;

#define MAX_MULT_MESSAGE_LENGTH 100

#ifdef __cplusplus

DECLARE_INTERFACE_(IWlogExf, IUnknown)
{
   // *** IWlogExf
   STDMETHOD(GoFirst)(THIS) PURE;
   STDMETHOD(GoFirstn)(THIS) PURE;
   STDMETHOD(GoLastn)(THIS) PURE;
   STDMETHOD(GoDupe)(THIS) PURE;
   STDMETHOD(GoNext)(THIS) PURE;
   STDMETHOD(GoNextn)(THIS) PURE;
   STDMETHOD(GoPrev)(THIS) PURE;
   STDMETHOD(GoPrevn)(THIS) PURE;

   STDMETHOD(AddNew)(THIS_ const char *, short , short, short) PURE;
   STDMETHOD(FinishedNew)(THIS) PURE;
   STDMETHOD(Widths)(THIS_ short *, short *, short *) PURE;
   STDMETHOD(Count)(THIS_ short *)PURE;

   STDMETHOD(Attributes)(THIS_ short *) PURE;
   STDMETHOD(Place)(THIS_ QsoPtr_t, VarPart_t *) PURE;
   STDMETHOD(Width)(THIS_ short *) PURE;
   STDMETHOD(Offset)(THIS_ short *) PURE;
   STDMETHOD(GetHeader)(THIS_ char *, long) PURE;
};

DECLARE_INTERFACE_(IWlogExf2, IWlogExf)
{
   // *** IWlogExf2
   STDMETHOD(GetADIFName)(THIS_ char *, long) PURE;
};

DECLARE_INTERFACE_(IWlogExf3, IWlogExf2)
{
   // *** IWlogExf3
   STDMETHOD(SetAttributes)(THIS_ short attr) PURE;
	// attr is an attribute mask defined in exfsym.h
   STDMETHOD(SetHeader)(THIS_ char *pHeader) PURE;
    // pHeader is a new header string for the field. 
};

#define MODULE_DATA_SCORE 1 // Enables WriteLog's Contest/Parameter-Setup menu item.
#define MODULE_DATA_NOBSM 2 // WriteLog has first column QSO count in its bandsummary UNLESS module sets this in GetModuleData

DECLARE_INTERFACE_(IWriteLog, IUnknown)
{

    // *** IWriteLog methods ***
    STDMETHOD(QsoHighlight)(THIS_ unsigned long QsoIndex, unsigned long Length)  PURE;
    STDMETHOD(Message)(THIS_ struct CwMsgStru FAR *Message, short Mode) PURE;
    STDMETHOD(QsoForceDraw)(THIS_ unsigned long QsoIndex) PURE;
    STDMETHOD(QsoIth)(THIS_ unsigned long QsoIndex, QsoPtr_t *Qso) PURE;
    STDMETHOD(SetQsoValues)(THIS_ unsigned long QsoIndex, QsoPtr_t q) PURE;
    STDMETHOD(NumberQsos)(THIS_ unsigned long *Number) PURE;
    STDMETHOD(BandTitle)(THIS_ short Index, const char FAR * FAR * Title) PURE;
    STDMETHOD(GetBandCount)(THIS_ short *Number) PURE;
    STDMETHOD(SearchDupeSheet)(THIS_ QsoPtr_t Qso, short Band,
        short DupeSheet, unsigned long *QsoNumber) PURE;
    STDMETHOD(AddQso)(THIS_ QsoPtr_t Qso) PURE;
    STDMETHOD(GetExf)(THIS_ IWlogExf FAR* FAR* ppvObj) PURE;
	STDMETHOD(AbortMessage)(THIS_) PURE;
	STDMETHOD(GetNet)(THIS_ REFIID iid, IUnknown **ppvObj) PURE;
};

DECLARE_INTERFACE_(IWriteLog2, IUnknown)
{
	STDMETHOD(SetUpdateFlag)(THIS_ long UpdateFlag, long *pPrevFlag) PURE;
	STDMETHOD(DoRedrawNow)(THIS_) PURE;
};

/*Bit flags for RequestMask argument to MultiCheck member */
#define WLOG_MULTICHECK_MSGSET 1L  /*Means Message buffer is valid */
#define WLOG_MULTICHECK_NOWRT  2L  /*Means MultCheck may NOT alter the qso */
/* ALL of the following bits MUST be CLEAR */
#define WLOG_MULTICHECK_RESERVED (~(WLOG_MULTICHECK_MSGSET | \
                                    WLOG_MULTICHECK_NOWRT))

DECLARE_INTERFACE_(IWlogMulti, IUnknown)
{
    // *** IWlogMulti methods ***
    STDMETHOD(GetLayout)(THIS_ ConstBandPtr_t *b, ConstExfPtr_t *e, LPCSTR FAR*s) PURE;

	// If the multiplier module returns S_FALSE, WriteLog completely
	// resorts all QSOs (by calling InitQsoData and QsoAdd for each QSO).
	// So the module will get called again on QsoAdd and it had better NOT
	// return S_FALSE the second time else an infinite recursion results.
    STDMETHOD(QsoAdd)(THIS_ QsoPtr_t q) PURE;
    STDMETHOD(QsoRem)(THIS_ QsoPtr_t q) PURE;
    STDMETHOD(InitQsoData)(THIS) PURE;

    STDMETHOD(MultiCheck)(THIS_ QsoPtr_t q, int Offset, int FAR *Result,
                long RequestMask, char FAR *Message) PURE;
	/*
	** q			is the QSO being checked
	** Offset		is the field in the QSO
	** Result		is where to write -1, 0, +1 for Unknown, no mult, new mult
	** RequestMask	is described above
	** Message		is a place to overide the message being asked for
	*/

    STDMETHOD(Display)(THIS_ HWND Window) PURE;

    // Score is called by Writelog when the user clicks Contest/Parameter Setup...
    STDMETHOD(Score)(THIS_ Configuration_Entry_t FAR *Config, HWND Window,
        unsigned QsoNum, const char FAR *TargetDir) PURE;
    STDMETHOD(GetModuleData)(THIS_ long *Data) PURE;
    STDMETHOD(SetMMParent)(THIS_ IWriteLog FAR *) PURE;
    STDMETHOD(TranslateAccelerator)(THIS_ MSG FAR *, short) PURE;

	//When a QSO is added to the database, WriteLog matches the
	//new QSO's call and band against all those already in the
	//database. If it finds a match of those things, it sets the
	//dupe member of the New QSO to 'D' and calls the multiplier 
	//modules's MatchQso member. The multiplier module is allowed
	//to change the 'D' to a space character, which means that
	//even though the call and band match between the New and Old
	//QSOs, there is some other reason that the New QSO should be
	//counted and scored.
	//
	//If there is more than one QSO in the database that matches
	//the new one's call and band, then WL calls here with each
	//of the QSOs in the "Old" argument, and WL considers the
	//new QSO a dupe if any one of those calls returns a dupe.
	//
    STDMETHOD(MatchedQso)(THIS_ QsoPtr_t New, QsoPtr_t Old) PURE;

	//It is often necessary to see if a callsign has been worked
	//on a given band--whether or not a QSO is about to be logged
	//for that call and band. WriteLog searches its QSO database
	//by setting up a struct qso_stru with the appropriate call
	//and band and other exchange info, and searches through
	//its QSO database for matches. For every QSO already in the
	//database that matches the canddiate call/band (at NewQ),
	//WriteLog offers the multiplier module a chance to say that
	//there is some other reason for the QSO to not be considered
	//a dupe and sets *IsGood = 1.
    STDMETHOD(QsoSearch)(THIS_ QsoPtr_t NewQ, QsoPtr_t OldQ,
         int FAR *IsGood) PURE;

    STDMETHOD(DupeSheetTitle)(THIS_ int DupeSheet, char FAR *Title, int TitleLength) PURE;
    STDMETHOD(TallyPrintQso)(THIS_ QsoPtr_t q) PURE;
    STDMETHOD(FormatPageSumm)(THIS_ char FAR *Buf, int BufLength) PURE;

	// WriteLog maintains as many separate dupe sheets as the multiplier
	// module needs. Only QSOs that have matching DupeSheet values are
	// duped against each other.
    STDMETHOD(SetDupeSheet)(THIS_ QsoPtr_t q, int *DupeSheet) PURE;

	//GetAdifName--returning S_OK and *pName to NULL causes this field to NOT be written to ADIF
	STDMETHOD(GetAdifName)(THIS_ long, long, char FAR *pName) PURE;
};

#define S_CHAR_OVERIDE MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, 2)
DECLARE_INTERFACE_(IWlogMulti2, IWlogMulti)
{
	//Take the QSO being edited and a string, where does the
	//the string best fit? return the value in Offset
	STDMETHOD(WhatsTheBestField)(QsoPtr_t q, const char *s, short *Offset) PURE;
	STDMETHOD(IsCharOKHere)(QsoPtr_t q, char c,	short Offset) PURE;
};


DECLARE_INTERFACE_(IWlogLineStatus, IUnknown)
{
	STDMETHOD(SetLineStatus)(IUnknown FAR *, const char *, unsigned char *) PURE;
};

/* IWlogSaveSummary is deprecated. It was used generate txt and rtf
** files. Pretty much all contests expect cabrillo now.
*/
DECLARE_INTERFACE_(IWlogSaveSummary, IUnknown)
{
	STDMETHOD(SaveTxt)( Configuration_Entry_t  *Config, const char *) PURE;
	STDMETHOD(SaveRtf)( Configuration_Entry_t  *Config, const char *) PURE;
};

/*
** Support for the one menu item the module can add to WriteLog's menu:
** the bottom entry in the "Contest" menu.
*/
DECLARE_INTERFACE_(IWlogContestCommand, IUnknown)
{
	/* WriteLog calls GetCommandName to ask the multiplier module
	** what string to put as the bottom entry WriteLog's "Contest" menu.
	*/
	STDMETHOD(GetCommandName)(char *s, short length) PURE;

	/*
	** When the user selects the bottom entry in the Contest menu,
	** WriteLog first calls StartFileSelection, WriteLog passes the argument
	** pInitialFileName set to the full path name of the current .WL file
	** that WriteLog has last saved.
	**
	** If StartFileSelection returns a SUCCEEDED status, then WL
	** uses pInitialFileName (which the multiplier module is allowed to overwrite
	** as long as it writes no more than "length" characters) on a call
	** to a file selection dialog box.
	*/
	STDMETHOD(StartFileSelection)(char *pInitialFileName, short length) PURE;

	/*
	** WriteLog calls ExecuteCommand after the call to StartFileSelection.
	** There are two ways for the multiplier module to arrange for a call
	** to ExecuateCommand:
	** 
	** a) if the module returns a SUCCEEDED status to StartFileSelection, then
	** WL displays to the user a FileSave dialog box, and the dialog box is i
	** initialized to the string in pInitialFileName (which MAY have been updated
	** by the multiplier module in the call to StartFileSelection). 
	** The user might	CANCEL the resulting dialog box. If he does, the multiplier
	** module does NOT receive a call to ExecuteCommand.
	**
	** b) if the module does NOT return a SUCEEDED status to StartFileSelection,
	** then WL calls ExecuteCommand with pInitialFileName
	*/
	STDMETHOD(ExecuteCommand)(const char *) PURE;
};

#define WLOG_CABRILLO_MAXCONTESTNAMELEN 80
#define WLOG_CABRILLO_MAXFIELDLENGTH 128
DECLARE_INTERFACE_(IWlogCabrillo, IUnknown)
{
	//opportunity for multiplier module to put up a box asking for info
	STDMETHOD(ConfirmFieldsFilled)(THIS_ HWND w) PURE;
	STDMETHOD(SetCallsign)(THIS_ const char *Call) PURE;
	//opportunity for multiplier module to format additional header lines for output
	//128 character MAX
	STDMETHOD(GetHeaderLineCount)(THIS_ short *pLines) PURE;
	STDMETHOD(GetHeaderLine)(THIS_ short LineNumber, char *Buf) PURE;

	//for CONTEST: 80 chars max
	STDMETHOD(GetContestName)(THIS_ char *Buf) PURE;
	//for CLAIMED-SCORE:
	STDMETHOD(GetClaimedScore)(THIS_ long *Score) PURE;

	//multiplier module has to format each field its going to put in the QSO:
	//FIELD WIDTHS ARE LIMITED TO 128 bytes
	STDMETHOD(GetTxFieldCount)(THIS_ short *pCount) PURE;
	STDMETHOD(FormatTxField)(THIS_ QsoPtr_t q, short Field, char *Buf) PURE;
	STDMETHOD(GetRxFieldCount)(THIS_ short *pCount) PURE;
	STDMETHOD(FormatRxField)(THIS_ QsoPtr_t q, short Field, char *Buf) PURE;
};

// WriteLog offers the module the opportunity to write Additional Rows to
// the Cabrillo file after the QSO: row (or rows)
DECLARE_INTERFACE_(IWlogCabrilloHelper, IUnknown)
{
	// The first four of these methods add a space character at the end of the
	// data they write
	STDMETHOD(WriteFrequency)(long freq) PURE; // Writes a Cabrillo formatted frequency
	STDMETHOD(WriteMode)(short mode) PURE;		// Writes a Cabrillo formatted mode
	STDMETHOD(WriteDate)(FILETIME *ft) PURE;	// Writes a Cabrillo formatted Date
	STDMETHOD(WriteTime)(FILETIME *ft) PURE; // Writes a Cabrillo formatted Time
	STDMETHOD(WriteString)(char *pLine) PURE;	// Writes string of characters.
};

DECLARE_INTERFACE_(IWlogCabrillo2, IUnknown)
{
	// The module can instruct WriteLog to write additional QSO: lines to
	// the Cabrillo files for each QSO. LinesForQSO tells WriteLog how many
	// QSO: lines to write for this qso.
	STDMETHOD(LinesForQSO)(THIS_ QsoPtr_t q, short *pLines) PURE;
	// WriteLog tells the module which of the QSO: lines it is working on.
	// The qso is the one last named in LinesForQSO
	STDMETHOD(SetCurrentLineNumber)(short LineNo) PURE;
};

DECLARE_INTERFACE_(IWlogCabrillo3, IWlogCabrillo2)
{
	// The module can instruct WriteLog to write additional rows after the QSO:
	// row (or rows--see IWlogCabrillo2) for each QSO. WriteLog does not format
	// these additional rows. The module is responsible for their formatting.
	// All the calls on this interface refer to the QSO named in the LinesForQSO
	// method in the base interface.
	// pMaxLength is the size buffer that WriteLog will allocate.
	// pRows is the number of additional rows to be written for this line.
	STDMETHOD(AdditionalRowsForThisLine)(short *pRows) PURE;
	// Allows the module to write whatever it wants to the Cabrillo file. THe
	// module should NOT write <cr> or <lf> characters. WriteLog will add one
	// after calling GetAdditionalRow
    // Note that if the module also implements IWlogCabrillo4, that any calls it
    // makes to WriteFreq, WriteMode, WriteDate, WriteTime will include 
    // a callback to the corresponding method on IWlogCabrillo4.
	STDMETHOD(GetAdditionalRow)(short RowNumber, IWlogCabrilloHelper *pHelper) PURE;
};

DECLARE_INTERFACE_(IWlogCabrillo4, IUnknown)
{
	// The module can override the way WriteLog formats the standard
    // Cabrillo fields, Frequency, Mode, Date and Time.
    // WriteLog formats the field as it plans to write it to the file,
    // then calls the module with pBuf pointing to the default formated
    // field, and "len" the number of characters allowed to be formated.
    // "len" includes a trailing space and a trailing null character.
    // The module may override (and return S_OK), or just leave it alone
    // (and return S_FALSE).
    // Note that RowNumber is -1 for the original row and that is the only
    // call the module will receive unless it also supports IWlogCabrillo3. In 
    // that case, and only if the module makes calls on its pHelper, it
    // will see RowNumber from 0 to the number of additional rows for this QSO
	STDMETHOD(FormatFreqField)(THIS_ QsoPtr_t q, short RowNumber, short len, char *pBuf) PURE;
	STDMETHOD(FormatModeField)(THIS_ QsoPtr_t q, short RowNumber, short len, char *pBuf) PURE;
	STDMETHOD(FormatDateField)(THIS_ QsoPtr_t q, short RowNumber, short len, char *pBuf) PURE;
	STDMETHOD(FormatTimeField)(THIS_ QsoPtr_t q, short RowNumber, short len, char *pBuf) PURE;
};

DECLARE_INTERFACE_(IWlogCabrilloOptions, IUnknown)
{
	// The module can control the entries in WL's drop down boxes for 
	// the Cabrillo output. As of this writing, the categories that
	// WL will use are:
	//
	//	OperatorCategories
	//	BAND
	//	POWER
	//	CATEGORYOVERLAY
	//	MODE
	//	ARRLSECTIONS
	// (which are also the sections in LogSubmission.ini)
	//  As of WL version 10.68, The "ARRLSECTIONS" entry is handled specially.
	//  If the module returns S_OK for ItemsForThisCategory for "LOCATIONS", 
	//  then that CategoryName is used to fill in the Cabrillo Version 3.0 
	//  "LOCATION" tag. Otherwise, WriteLog  uses CategoryName "ARRLSECTIONS" to 
	//  fill in the Cabrillo LOCATIONS tag, including a call to ItemsForThisCategory
	//  (which means WriteLog is backwards compatible with modules expecting to
	//  be called with "ARRLSECTIONS", but new modules should expect "LOCATIONS")
	//  Also added in 10.68 are:
	//  LOCATIONS
	//  STATION
	//  TIME
	//  TRANSMITTER
	//  ASSISTED
	//

	// Module should return S_OK to this if it wants to overide WL's drop down
	// entries in Cabrillo for the given category, or S_FALSE if it wants WL's defaults.
	// If the module wants the drop down box empty, then it should return S_OK, and *Items = 0;
	STDMETHOD(ItemsForThisCategory)(THIS_ const char *pCategoryName, long *Items) PURE;
	// If the module returns S_OK to ItemsForThisCategory, WL will call back
	// Items times on GetItemForCategory
	STDMETHOD(GetItemForCategory)(THIS_ const char *pCategoryName, long ItemNumber, 
						// WL will put the DisplayString in the drop down,
						char *DisplayString, long DisplayStringBufSize,
						// and this corresponding value into the Cabrillo file.
						char *ValueString, long ValueStringBufSize
						) PURE;

	// WL looks up the Cabrillo contest ID in LogSubmissions.ini (by
	// calling IWlogCabrillo::GetContestName first)
	// If the lookup in LogSubmissions.ini gives no match, then WL calls here
	STDMETHOD(GetSponsorEmailAddress)(THIS_ char *pAddr, long AddrSize) PURE;

	// If the module returns S_OK to this, then it will set the
	// Subject line of the email to this. Otherwise, WriteLog sets the 
	// the subject line to be the CALL (and nothing else)
	STDMETHOD(GetCabrilloEmailSubjectLine)(THIS_ char *pSubject, long SubjectSize) PURE;
};

DECLARE_INTERFACE_(IWlogCabrilloSettings, IUnknown)
{
	// WriteLog queries for and calls this interface after the user clicks
	// Create cabrillo.
	// WriteLog iterates through all the Cabrillo categories (almost) like
	// in IWlogCabrilloOptions.
	// This time, WriteLog tells the module what the choice in the combo box is
	//
	// "LOCATION"
	// "CALLSIGN"
	//	"OPERATOR"
	//	"BAND"
	//	"POWER"
	//	"OVERLAY"
	//	"MODE"
	//	"STATION"
	//	"TIME"
	//	"TRANSMITTER"
	//	"ASSISTED"

	STDMETHOD(SelectionForThisCategory)(THIS_ HWND hWnd, 
			const char *pCategoryName,	const char *pSelectedValue) PURE;
	// if the module returns S_OK, WL continues
	// if the module returns S_FALSE, WL does MessageBeep and places the focus on
	// the control that produced the "problem" result, and does not write the file
	// if the module returns any failure code, the writing of the cabrillo does NOT
	// happen and the module is reponsible for putting up a dialog box on parent hWnd
	// telling the user why.
};

DECLARE_INTERFACE_(IWlogMultiADIF, IUnknown)
{
	// WriteLog calls here to allow the multiplier module the opportunity to format the ADIF output
	// for a given QSO. Return anything other than S_OK, and WL will just copy the QSO field to the
	// ADIF file.
	STDMETHOD(TranslateToAdif)(THIS_ QsoPtr_t q, long Offset, char *Buf, long BufLen) PURE;
	// WriteLog calls here to allow the multiplier module to translate the ADIF text into
	// the QSO. Return anything other than S_OK, and WL parses the ADIF text using the same
	// rules as if the characters were typed in.
	STDMETHOD(TranslateFromAdif)(THIS_ QsoPtr_t q, long Offset, const char *pIn) PURE;
};

// Only contests that have a 10minute rule should support this interface
DECLARE_INTERFACE_(IWlogMultiSingleMinutes, IUnknown)
{
	// Return the number of seconds that a station must remain on a band (usually 600)
	STDMETHOD(SecondsOnBandRequired)(THIS_ long *pSeconds) PURE;
	// set *pMultCount to the number of mults for q. *pMultCount = 2, for example is a double mult
	STDMETHOD(IsMultiplier)(THIS_ QsoPtr_t q, long *pMultCount) PURE;
};

// Only contests that support ARRL-style band-change-per-hour rules should support this
DECLARE_INTERFACE_(IWlogMultiSingleBandChangeThrottle, IUnknown)
{
	// ARRL-style mult-single contest
	STDMETHOD(GetBandChangeLimits)(THIS_ long *pSeconds, long *pMaxChanges, 
			long *pPerClockHour	// return 0 on this to count band changes per elapsed hour
								// return 1 on this to count band changes per zulu hour
								// any other return is illegal and treated as an error.
			) PURE;
};

// IWlogScoreInfo helps WriteLog build a rate meter
DECLARE_INTERFACE_(IWlogScoreInfo, IUnknown)
{
	STDMETHOD(MultsAndPts)(THIS_ long *pMultCount, long *pPtsCount) PURE;
	STDMETHOD(PtsForQso)(THIS_ QsoPtr_t q, long *pPtsCount) PURE;
};

DECLARE_INTERFACE_(IWlogMultiGridSquare, IUnknown)
{
	// The multiplier module returns the grid squares corresponding to a given QSO.
	// Both pHisSquare and pMySquare point to buffers at least 7 characters long.
	// The module may return success (S_OK) only if it can give at least a 4 character
	// grid square for both HisSquare and MySquare. It should return 6 character 
	// squares if it has them.
	STDMETHOD(GetGridSquares)(THIS_ QsoPtr_t q, char *pHisSquare, char *pMySquare) PURE;
};

DECLARE_INTERFACE_(IWlogMultiGridSquare2, IWlogMultiGridSquare)
{
	// WriteLog calls the module here if it looks up a grid square in a callsign
	// database and finds it. The module may or may not choose to use this information.
	STDMETHOD(AdviseGridSquare)(THIS_ QsoPtr_t q, char *pHisSquare) PURE;
};

DECLARE_INTERFACE_(IWlogMultiBandCalc, IUnknown)
{
	// The multiplier module is allowed to calculate a new band for the QSO.
	STDMETHOD(BandCalc)(THIS_ QsoPtr_t q, unsigned char *pNewBand) PURE;
};

DECLARE_INTERFACE_(IWlogMultiOffTime, IUnknown)
{
	// The multiplier module can set the minimum off time for the 
    // off-time calculator in WriteLog. WL defaults to 30 minutes.
    // If the contest requires shorter/longer off times, fix it here
	STDMETHOD(GetMinOffTime)(THIS_ long *pOffTime) PURE;
};


// When WriteLog exports to TrustedQSL, it first asks the module
// if it supports IWlogTQslQsoLocation. if not, then all QSOs in
// the export are logged against a single location, which WriteLog gets
// from the user in a selection dialog and the multiplie module is not
// involved.
// But if the module supports the following interface, then each QSO in the
// exported log can have a different location, AND each QSO in the log
// can be logged under multiple locations--again, only if the module supports
// this interface.
//
// As a general rule, a contest that supports rovers needs to support this
// interface in order to enable WriteLog to put the right StationLocation
// information in the LOTW entry for the QSOs logged in each county.
// The same thing happens for LOTW export for grid square contest with rovers.
// Of course, only the rover itself needs this support--stations that 
// contact the rover 
//
// One not-so-obvious feature of this interface is that the LOTW fields the
// module choses to control (which it enumerates in GetFieldGABBIName) will
// make a lot of difference in how the module behaves. 
// WriteLog asks the user to select an existing StationLocation from the
// Trusted QSL data store before ever starting the export. And if the
// station stays in one place the whole contest, then there is nothing
// more the module needs to do. Buta  county QSO party module that supports 
// rovers must certainly control the  US_COUNTY GABBI field.
// (What's a GABBI field? Look here: http://www.trustedqsl.org/tQSL-documents/licence.html
// and follow the GABBI specification link).
//
// But the module MIGHT want to also control the US_STATE GABBI field. Why?
// If the user selected an existing StationLocation that has the US_STATE
// field already set, and its set to the wrong state, then when WriteLog
// attempts to set the US_COUNTY field, the LOTW libraries will complain
// and WriteLog will put up a box saying that "LOTW won't accept XYZ county
// in the US_COUNTY field".
// The module can also control the US_STATE field if it likes, so it could
// prevent that error by returning US_STATE->California (or whatever) for
// every single QSO and the LOTW export will happen successfully even if
// the user starts the export with a station location with the wrong state
// or no state at all.
//
// There's no way to avoid the next confusing thing: Legal values for 
// the US_COUNTY field depend on the value already set in the US_STATE
// field. The only way to make this work is that the module , for
// the GABBI fields that are on earlier pages, must return them first
// (at lower Numfield numbers). That way, WriteLog will tell LOTW that
// the "state is California", followed by the "county is Alpine" and
// that will work while doing so in the reverse order won't.
DECLARE_INTERFACE_(IWlogTQslQsoLocation, IUnknown)
{
	// The TrustedQSL export format is organized by GABBI fields.
	// At file export time, 
	// WriteLog asks the module how many of those GABBI fields it wants to
	// customize, and what is the name of each of those fields.
	// By definition in this interface, all QSOs in the log have the same number of GABBI fields to setup.
	STDMETHOD(GetNumLocationFields)(THIS_ long *pNumfields) PURE;
	// Once the module has said its going to update GABBI fields 
	// (by returning nonzero to GetNumLocationFields, it is committed to
	// returning a set of names to GetFieldGABBIName AND it
	// is committed to returning at least one value for that GABBI field for EVERY
	// QSO in the log.
	STDMETHOD(GetFieldGABBIName)(THIS_ long Numfield, char *pFieldName, long FieldNameSize) PURE;

	// For each QSO in the log, WriteLog asks how many locations apply to the
	// the QSO. Each QSO in the log can be logged into a different number of locations.
	// Zero locations is an error and aborts the export.
	// This supports county line operations--such a log would return 2 on
	// GetQsoNumLocations and then WL will call back twice on each QSO to query
	// the two counties, one at a time.
	STDMETHOD(GetQsoNumLocations)(THIS_ QsoPtr_t q, long *pNumLocations) PURE;
	// ...and then, for each location in *pNumLocations, WriteLog asks
	// for the module to specify the location in terms of values of the GABBI
	// fields, so it runs FieldNum from zero to GetNumLocationFields.
	// WriteLog writes NumLocations copies of the QSO into the LOTW export
	// --each one with a different tStation GABBI field, and each one logged
	// one second later than the previous one (because the LOTW database counts
	// a subsequent QSO on the same time and frequency as an overwrite of the
	// previous--the one second difference makes the second entry different)
	STDMETHOD(GetQsoLocationField)(THIS_ QsoPtr_t q, long Location, long FieldNum,
		char *pFieldvalue, long FieldValueSize) PURE;
};

DECLARE_INTERFACE_(IWlogMulFeatureEnable, IUnknown)
{
    STDMETHOD(SupercheckPartial)(THIS_) PURE;   // S_OK means enable, S_FALSE means no
    STDMETHOD(LoadFromADIF)(THIS_) PURE;        // S_OK means enable, S_FALSE means no
    STDMETHOD(CwDecoder)(THIS_) PURE;
    STDMETHOD(StartUdpPostingAutomatically)(THIS_ short *timeMinutes) PURE; /* CAN BE NULLPTR */
    STDMETHOD(GetPostingUrl)(THIS_ char *url) PURE;/* CAN BE NULLPTR */
    STDMETHOD(GetPostingPortNumber)(THIS_ short *portnum) PURE;/* CAN BE NULLPTR */
    STDMETHOD(GetPostingEnableBandBreakdown)(THIS_  short *enableBandBreakdown) PURE;/* CAN BE NULLPTR */
    STDMETHOD(GetPostingContestName)(THIS_ char *name) PURE;/* CAN BE NULLPTR */
};


DEFINE_WL_GUID(IWriteLog, "C7212144-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogMulti, "C721220C-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogMulti2, "C7212201-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogExf, "C721220E-7716-101A-AA54-00608C61D0B1" );
DEFINE_WL_GUID(IWlogLineStatus, "C7212162-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogSaveSummary, "C7212223-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogContestCommand, "C7212224-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogCabrillo, "28CAE760-575C-11D3-9B84-00105AA43DDF");
DEFINE_WL_GUID(IWlogCabrillo2, "193c1231-2844-4bba-a6bf-2822d3a4c6e0");
DEFINE_WL_GUID(IWlogCabrillo3, "C721217B-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogCabrillo4, "C7212180-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogCabrilloOptions, "C721218D-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogCabrilloSettings, "C7212197-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogCabrilloHelper, "C721217C-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogMultiADIF, "C7212174-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogMultiSingleMinutes, "C7212176-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogMultiSingleBandChangeThrottle, "C721217D-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogExf2, "C7212178-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogScoreInfo, "C721218A-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogExf3, "C721218B-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogMultiGridSquare, "C721218C-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogMultiGridSquare2, "C7212190-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogMultiBandCalc, "C7212191-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogTQslQsoLocation, "C721218E-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWriteLog2, "C721218F-7716-101A-AA54-00608C61D0B1");
DEFINE_WL_GUID(IWlogMultiOffTime, "76e5ada3-fbc0-4368-b881-1d9d3fc2240a");
DEFINE_WL_GUID(IWlogMulFeatureEnable, "4b6006be-67c4-4654-a70a-42b1e7da8f21");

DEFINE_GUID(CATID_IWlogMultiCategory, 0xC7212159, 0x7716, 0x101A,
    0xAA, 0x54, 0x00, 0x60, 0x8C, 0x61, 0xD0, 0xB1);

// WARNING. This IID is deprecated. WriteLog does not support it any more.
// As of WL 10.35, the signature of the function on this interface changed.
// You will only notice this change if you recompile a module that supported
// the old interface. It won't compile any more because you need to add a
// a parameter to the GetBandChangeLimits method.
DEFINE_GUID(IID_IWlogMultiSingleBandChangeThrottleOld, 0xC7212177, 0x7716, 0x101A,
    0xAA, 0x54, 0x00, 0x60, 0x8C, 0x61, 0xD0, 0xB1);
/* {C7212177-7716-101A-AA54-00608C61D0B1}			*/

#endif // __cplusplus

#endif
