/***
*cremwlog.cpp
*
*
*/

#include <windows.h>
#include <ole2.h>
#include <oleauto.h>

#include "cremwlog.h"
#include "disphelp.h"
#include "initdisp.h"


//extern BOOL g_fTrace;

#define DIM(X) (sizeof(X) / sizeof((X)[0]))


// method names on the CWlog class.
// 
const OLECHAR * CRemWlogE::m_rgszMethods[] = {
    OLESTR("callsign"),
    OLESTR("callsign"),
     OLESTR("dupe"),
    OLESTR("clearentry"),
     OLESTR("enterqso"),
    OLESTR("setfieldn"),
    OLESTR("getfieldn"),
    OLESTR("getfieldwidth"),
    OLESTR("setvisible"),
    OLESTR("isnewmultiplier"),
    OLESTR("setlogfrequency"),
    OLESTR("mapstation"), 
    OLESTR("activate"),
	OLESTR("setlogfrequencyex"),
	OLESTR("getlogfrequency"),
   OLESTR("currentfieldnumber"),
   OLESTR("setcurrentfield"),
   OLESTR("putbestfield"),
   OLESTR("seenthiscontest"),
   OLESTR("getparent"),
   OLESTR("field"),
   OLESTR("calclinestatus"),
   OLESTR("getband"),
   OLESTR("GetSbRtty"),
   OLESTR("SetXmitPtt"),
   OLESTR("CancelTimedCQ"),
   OLESTR("SetAsReturn"),
   OLESTR("GetLeftRight"),
   OLESTR("DupeCheckList"),
   OLESTR("FieldTitle"),
   OLESTR("PushCall"),
   OLESTR("GetEntryId"),
   OLESTR("NewSpots2"),
};

const OLECHAR * CRemWlog::m_rgszMethods[] = {
    OLESTR("createentry"),
    OLESTR("getcurrententry"),
    OLESTR("getmemmsg"),
    OLESTR("setvisible"),
    OLESTR("setrig"),
    OLESTR("sendprogrammsg"),
   OLESTR("getfocusentry"),
   OLESTR("fileuseropen"),
   OLESTR("optionsregisternet"),
   OLESTR("activate"),
   OLESTR("sortbytime"),
   OLESTR("notifyxmitstart"),
   OLESTR("notifyxmitfinished"),
   OLESTR("GetWlSound"),
   OLESTR("NetLetter"),
   OLESTR("FileOpen"),
   OLESTR("CloseNet"),
   OLESTR("DoSuperCheck"),
   OLESTR("NewSpot"),
   OLESTR("NewSpotEx"),
   OLESTR("SetFKeyMsgDigital"),
   OLESTR("GetFKeyMsgDigital"),
   OLESTR("SetFKeyMsgVoice"),
   OLESTR("GetFKeyMsgVoice"),
   OLESTR("ModifyBlacklist"),
};

static DISPID Put = DISPID_PROPERTYPUT;

struct VariantSetup : public VARIANT
{
    VariantSetup() { ::VariantInit(this);}
    ~VariantSetup() { ::VariantClear(this);}
};


// A useful pre-initialized DISPATCHPARAMS, used on all the methods that
// take 0 arguments.
//
static DISPPARAMS NEAR g_dispparamsNoArgs = {NULL, NULL, 0, 0};

CRemWlogE::CRemWlogE(IDispatch FAR *Dispatch)
{
	 m_pdisp = Dispatch;
	Dispatch->AddRef();
	 InitDisp("WriteLog Entry", IMETH_CREMWLOGE_MAX,
				m_pdisp,
				m_rgszMethods,
				m_rgdispid );
}

CRemWlogE::~CRemWlogE()
{
	 if (m_pdisp)
		  m_pdisp->Release();
	 m_pdisp = 0;
}


//---------------------------------------------------------------------
//                    Introduced methods
//---------------------------------------------------------------------


/*
 * Each of these methods is simply a cover for an IDispatch Invocation
 * of the actual method on the remote CPoly class. This allows CRemWlog
 * to present an interface that looks and acts just like the CPoly
 * object, even though the actual work is being done in another process.
 *
 */
BSTR
CRemWlogE::GetCallsign()
{
    HRESULT hresult;
    VARIANT varResult;
    BSTR    ReturnValue;

	 TestDispId(IMETH_CREMWLOGE_GETCALLSIGN);
     VariantInit(&varResult);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_GETCALLSIGN],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_PROPERTYGET,
      &g_dispparamsNoArgs, &varResult, NULL, NULL);

    if(hresult != NOERROR)
      return 0;

    ReturnValue = V_BSTR(&varResult);

    return ReturnValue;
}

BSTR CRemWlogE::get_Field(short i)
{
	VARIANT pdisp;
	DISPPARAMS dispparams;
	VARIANT	varResult;

	VariantInit(&pdisp);
	VariantInit(&varResult);

	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 0;
	dispparams.rgvarg = &pdisp;
	dispparams.rgdispidNamedArgs = 0;

	V_VT(&pdisp) = VT_I2;
	V_I2(&pdisp) = i;

	TestDispId(IMETH_CREMWLOGE_FIELD);
	m_pdisp->Invoke(
		m_rgdispid[IMETH_CREMWLOGE_FIELD],
		IID_NULL, LOCALE_SYSTEM_DEFAULT,
		DISPATCH_PROPERTYGET,
		&dispparams,
		&varResult, 0, 0);

	if (V_VT(&varResult) == VT_BSTR)
		return (V_BSTR(&varResult));
	return 0;
}

void 
CRemWlogE::put_Field(short i, const char *v)
{
	VARIANT pdisp[2];
#ifdef WIN32
	OLECHAR Wide[500];
	memset(Wide, 0, sizeof(Wide));
	MultiByteToWideChar(CP_ACP, 0, v, -1, Wide, 499);
#else
	const OLECHAR *Wide = v;
#endif
     DISPPARAMS dispparams;

	 VariantInit(&pdisp[0]);
	 VariantInit(&pdisp[1]);

    V_VT(&pdisp[1]) = VT_I2;
    V_I2(&pdisp[1]) = i;
	V_VT(&pdisp[0]) = VT_BSTR;
	V_BSTR(&pdisp[0]) = ::SysAllocString(Wide);

    dispparams.cArgs = 2;
    dispparams.cNamedArgs = 1;
    dispparams.rgvarg = &pdisp[0];
	dispparams.rgdispidNamedArgs = &Put;

	TestDispId(IMETH_CREMWLOGE_FIELD);
	m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_FIELD],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_PROPERTYPUT,
      &dispparams, 0, 0, 0);
	VariantClear(&pdisp[0]);
	VariantClear(&pdisp[1]);
}

long
CRemWlogE::PutCallsign(const char *Call)
{

#if 0
        VARIANTARG varResult;
    DISPPARAMS *pdispparams;
    HRESULT hresult;
    BSTR CallAlloc;
#ifdef WIN32
    OLECHAR CallW[100];
	memset(CallW, 0, sizeof(CallW);
    MultiByteToWideChar(CP_ACP, 0, Call, -1, CallW, 99);
#else
    const OLECHAR *CallW = Call;
#endif
// DISPATCH_PROPERTY_PUT on IMETH_CREMLOG_PUTCALLSIGN
    // with a single argument ought to work, but doesn't.
    CallAlloc = SysAllocString(CallW);
    hresult = DispBuildParams(&pdispparams, 0, NULL, "ib",
        (short) -1,  
        CallAlloc);
    if(hresult != NOERROR)
      return 0;

    VariantInit(&varResult);
 	 TestDispId(IMETH_CREMWLOGE_PUTFIELDN);
   hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_PUTFIELDN],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      pdispparams, &varResult, NULL, NULL);
    DispFreeParams(pdispparams);
    //SysFreeString(CallAlloc); //Empirical testing shows this already done
    return (V_I4(&varResult));
#else
    VARIANTARG varResult;
    DISPPARAMS dispparam;
	VARIANTARG disparg[1];
    HRESULT hresult;
    BSTR CallAlloc;
#ifdef WIN32
    OLECHAR CallW[100];
	memset(CallW, 0, sizeof(CallW));
    MultiByteToWideChar(CP_ACP, 0, Call, -1, CallW, 99);
#else
    const OLECHAR *CallW = Call;
#endif
    // DISPATCH_PROPERTY_PUT on IMETH_CREMLOG_PUTCALLSIGN
    // with a single argument ought to work, but doesn't.
    CallAlloc = SysAllocString(CallW);
	VariantInit(&disparg[0]);
	V_VT(&disparg[0]) = VT_BSTR;
	V_BSTR(&disparg[0]) = CallAlloc;
    VariantInit(&varResult);
    dispparam.rgvarg = &disparg[0];
    dispparam.rgdispidNamedArgs = &Put;
    dispparam.cArgs = 1;
    dispparam.cNamedArgs = 1;


 	 TestDispId(IMETH_CREMWLOGE_PUTCALLSIGN);
   hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_PUTCALLSIGN],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      &dispparam, &varResult, NULL, NULL);
   VariantClear(&disparg[0]);
    return -1;
#endif
}

void
CRemWlogE::CalcLineStatus(const char *Line, unsigned char *s, int s_size)
{
    VARIANTARG varResult;
	VARIANT LineVariant;
    DISPPARAMS dispparams;
    HRESULT hresult;
    BSTR LineAlloc;
	SAFEARRAY *x;
	unsigned char *p;
	long ArraySize = 0;
	int i;
#ifdef WIN32
    OLECHAR LineW[512];
	memset(LineW, 0, sizeof(LineW));
    MultiByteToWideChar(CP_ACP, 0, Line, -1, LineW, 511);
#else
    const OLECHAR *LineW = Line;
#endif

    LineAlloc = SysAllocString(LineW);
	VariantInit(&LineVariant);
	V_VT(&LineVariant) = VT_BSTR;
	V_BSTR(&LineVariant) = LineAlloc;
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    dispparams.rgvarg = &LineVariant;
	dispparams.rgdispidNamedArgs = 0;

    VariantInit(&varResult);
 	 TestDispId(IMETH_CREMWLOGE_CALCLINESTATUS);

   hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_CALCLINESTATUS],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      &dispparams, &varResult, NULL, NULL);

	VariantClear(&LineVariant);

	if (V_VT(&varResult) == (VT_UI1 | VT_ARRAY))
	{
		x = V_ARRAY(&varResult);
		SafeArrayAccessData(x, (void HUGEP* FAR*) &p);
		SafeArrayGetUBound(x, 1, &ArraySize);
		ArraySize += 1;

		for (i = 0; (i < s_size) && (i < ArraySize); i += 1)
			*s++ = *p++;
		SafeArrayUnaccessData(x);
	}
	VariantClear(&varResult);
    return;
}


void
CRemWlogE::SetVisible(short Vis)
{
    DISPPARAMS pdispparams;
    VARIANTARG  pdisparg[1];
    HRESULT hresult;

    pdispparams.rgvarg = pdisparg;
    pdispparams.rgdispidNamedArgs = 0;
    pdispparams.cArgs = 1;
    pdispparams.cNamedArgs = 0;

    VariantInit(&pdisparg[0]);
    V_VT(&pdisparg[0]) = VT_I2;
    V_I2(&pdisparg[0]) = Vis;

  	 TestDispId(IMETH_CREMWLOGE_VISIBLE);
   hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_VISIBLE],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      &pdispparams, 0, NULL, NULL);
    return;
}


void CRemWlogE::SetXmitPtt(short NewVal)
{
    DISPPARAMS pdispparams;
    VARIANTARG  pdisparg[1];

    pdispparams.rgvarg = pdisparg;
    pdispparams.rgdispidNamedArgs = 0;
    pdispparams.cArgs = 1;
    pdispparams.cNamedArgs = 0;

    VariantInit(&pdisparg[0]);
    V_VT(&pdisparg[0]) = VT_I2;
    V_I2(&pdisparg[0]) = NewVal;

  	 TestDispId(IMETH_CREMWLOGE_SETXMITPTT);
	m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_SETXMITPTT],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      &pdispparams, 0, NULL, NULL);
    return;
}


long
CRemWlogE::Dupe()
{
    HRESULT hresult;
    VARIANT varResult;
    long    ReturnValue;

    VariantInit(&varResult);
  	 TestDispId(IMETH_CREMWLOGE_DUPE);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_DUPE],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      &g_dispparamsNoArgs, &varResult, NULL, NULL);

    if(hresult != NOERROR)
      return -1;

    ReturnValue = V_I4(&varResult);

    return ReturnValue;
}


void
CRemWlogE::Clear()
{
  	 TestDispId(IMETH_CREMWLOGE_CLEAR);
    m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_CLEAR],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      &g_dispparamsNoArgs, NULL, NULL, NULL);

}

typedef BOOL  (WINAPI *W32Proc)(DWORD);
static void inline AllowSetForeground()
{
	  HINSTANCE hInst = LoadLibrary("user32.dll");
	  if (hInst)
	  {
		W32Proc	f;
	  //AllowSetForegroundWindow(ASFW_ANY);
		f = (W32Proc)GetProcAddress(hInst, "AllowSetForegroundWindow");
		if (f)
		{
			BOOL r = (*f)((DWORD)(-1));
#ifdef _DEBUG
			if (!r)
			{
				char buf[256];
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, buf, sizeof(buf), 0);
				OutputDebugString(buf);
			}
			else
			{
				OutputDebugString("AllowSetForegroundWindow OK\n");
			}
#endif
		}
		FreeLibrary(hInst);
	  }
}

static void inline ForceReturnForeground(HWND w)
{
	  int c = 10;
	  while (!::SetForegroundWindow(w) && (c-- >= 0))
		  Sleep(1);
}

long
CRemWlogE::Activate()
{
     VARIANT varResult;
     long    ReturnValue = 0;

     VariantInit(&varResult);
 	  TestDispId(IMETH_CREMWLOGE_ACTIVATE);
	AllowSetForeground();
      HRESULT hr = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOGE_ACTIVATE],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &g_dispparamsNoArgs, &varResult, NULL, NULL);
	  VariantChangeType(&varResult, &varResult, 0, VT_I4);
	  ReturnValue = V_I4(&varResult);
	  VariantClear(&varResult);
	  if (SUCCEEDED(hr) && ReturnValue)
	  {
		ForceReturnForeground((HWND)ReturnValue);
	  }
	  return ReturnValue;
}

void
CRemWlogE::EnterQso()
{
  	 TestDispId(IMETH_CREMWLOGE_ENTERQSO);
    m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_ENTERQSO],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      &g_dispparamsNoArgs, NULL, NULL, NULL);
}


long
CRemWlogE::PutFieldN(short n, const char *s)
{
     HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
    VARIANT varResult;
    long    ReturnValue;
    BSTR FieldN;
    OLECHAR FieldW[100];
	memset(FieldW, 0, sizeof(FieldW));
    MultiByteToWideChar(CP_ACP, 0, s, -1, FieldW, 99);
    FieldN = SysAllocString(FieldW);
    hresult = DispBuildParams(&pdispparams, 0, NULL, "ib", n,
        FieldN);
    if(hresult != NOERROR)
        return 0;

    VariantInit(&varResult);
  	 TestDispId(IMETH_CREMWLOGE_PUTFIELDN);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_PUTFIELDN],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
      pdispparams, &varResult, NULL, NULL);

    DispFreeParams(pdispparams);
    ReturnValue = V_I4(&varResult);
     //SysFreeString(FieldN);
     return ReturnValue;
}

BSTR
CRemWlogE::GetFieldN(short n)
{
    HRESULT hresult;
    VARIANT varResult;
    BSTR    ReturnValue;
    VARIANTARG varg;
     DISPPARAMS dispparams;

     V_VT(&varg) = VT_I2;
     V_I2(&varg) = n;

     dispparams.cArgs = 1;
     dispparams.cNamedArgs = 0;
     dispparams.rgvarg = &varg;

     VariantInit(&varResult);
   	 TestDispId(IMETH_CREMWLOGE_GETFIELDN);
   hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_GETFIELDN],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
        &dispparams, &varResult, NULL, NULL);

     if(hresult != NOERROR)
        return 0;

     ReturnValue = V_BSTR(&varResult);

     return ReturnValue;
}

short
CRemWlogE::GetFieldWidth(short n)
{
     HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
    VARIANT varResult;
    short   ReturnValue;

    hresult = DispBuildParams(&pdispparams, 0, NULL, "i", n);
     if(hresult != NOERROR)
        return 0;

    VariantInit(&varResult);
   	 TestDispId(IMETH_CREMWLOGE_GETFIELDWIDTH);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_GETFIELDWIDTH],
        IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      pdispparams, &varResult, NULL, NULL);

    DispFreeParams(pdispparams);
    ReturnValue = V_I2(&varResult);

    return ReturnValue;
}

short
CRemWlogE::IsNewMultiplier(short n)
{
    HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
    VARIANT varResult;
    short   ReturnValue;

    hresult = DispBuildParams(&pdispparams, 0, NULL, "i", n);
     if(hresult != NOERROR)
        return 0;

    VariantInit(&varResult);
   	 TestDispId(IMETH_CREMWLOGE_ISNEWMULT);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_ISNEWMULT],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
        pdispparams, &varResult, NULL, NULL);

     DispFreeParams(pdispparams);
    ReturnValue = V_I2(&varResult);

    return ReturnValue;
}

short
CRemWlogE::SetLogFrequency(short Mode, long Freq)
{
    HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
    VARIANT varResult;
    short   ReturnValue;

    hresult = DispBuildParams(&pdispparams, 0, NULL, "iI", Mode,
        Freq);
     if(hresult != NOERROR)
        return 0;

    VariantInit(&varResult);
   	 TestDispId(IMETH_CREMWLOGE_SETLOGFREQ);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_SETLOGFREQ],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
        pdispparams, &varResult, NULL, NULL);

     DispFreeParams(pdispparams);
    ReturnValue = V_I2(&varResult);

    return ReturnValue;
}
short
CRemWlogE::SetLogFrequencyEx(short Mode, double rFreq,
								double tFreq, short Split)
{
    HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
    VARIANT varResult;
    short   ReturnValue;

    hresult = DispBuildParams(&pdispparams, 0, NULL, "iRRi", Mode,
        rFreq, tFreq, Split);
     if(hresult != NOERROR)
        return 0;

    VariantInit(&varResult);
   	 TestDispId(IMETH_CREMWLOGE_SETLOGFREQEX);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_SETLOGFREQEX],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
        pdispparams, &varResult, NULL, NULL);

     DispFreeParams(pdispparams);
    ReturnValue = V_I2(&varResult);

    return ReturnValue;
}
short
CRemWlogE::GetLogFrequency(short *Mode, double *rFreq,
							double *tFreq, short *Split)
{
    HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
    VARIANT varResult;
    short   ReturnValue;

    hresult = DispBuildParams(&pdispparams, 0, NULL, "&i&R&R&i", Mode,
        rFreq, tFreq, Split);
     if(hresult != NOERROR)
        return 0;

    VariantInit(&varResult);
   	 TestDispId(IMETH_CREMWLOGE_GETLOGFREQ);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_GETLOGFREQ],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
        pdispparams, &varResult, NULL, NULL);

     DispFreeParams(pdispparams);
    ReturnValue = V_I2(&varResult);

    return ReturnValue;
}


void  CRemWlogE::MapStation(long Time, short Stat)
{
     HRESULT hresult;
    DISPPARAMS FAR* pdispparams;

    hresult = DispBuildParams(&pdispparams, 0, NULL, "Ii",
        Time,Stat);

    if(hresult != NOERROR)
        return;

    	 TestDispId(IMETH_CREMWLOGE_MAPSTATION);
   hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_MAPSTATION],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
      pdispparams, 0, NULL, NULL);

    DispFreeParams(pdispparams);
}

void CRemWlogE::NewSpots(bool singleBand,
                            int count, const char**calls,  double *rcvs, 
                          double *xmits,  short *modes, const char **notes,
                          const char **spotters)
{
    static const int NUM_ARGS = 7;
    VariantSetup varResult;
    SAFEARRAYBOUND sab;
    sab.cElements = count;
    sab.lLbound = 0;

    VariantSetup args[NUM_ARGS];
    int argno = NUM_ARGS;

    VARIANT &singleBandVar(args[--argno]); 
    singleBandVar.vt = VT_I2;
    V_I2(&singleBandVar) = singleBand;
    
    VARIANT &Calls(args[--argno]); 
	Calls.vt = VT_BSTR | VT_ARRAY;
	Calls.parray = SafeArrayCreate(VT_BSTR, 1, &sab);

    VARIANT &RxFreqs(args[--argno]); 
	RxFreqs.vt = VT_R8 | VT_ARRAY;
	RxFreqs.parray = SafeArrayCreate(VT_R8, 1, &sab);;

    VARIANT &TxFreqs(args[--argno]); 
	TxFreqs.vt = VT_R8 | VT_ARRAY;
	TxFreqs.parray = SafeArrayCreate(VT_R8, 1, &sab);

    VARIANT &Modes(args[--argno]); 
	Modes.vt = VT_I2 | VT_ARRAY;
	Modes.parray = SafeArrayCreate(VT_I2, 1, &sab);

    VARIANT &Notes(args[--argno]); 
    Notes.vt = VT_BSTR | VT_ARRAY;
    Notes.parray = SafeArrayCreate(VT_BSTR, 1, &sab);

    VARIANT &Spotters(args[--argno]); 
    Spotters.vt = VT_BSTR | VT_ARRAY;
    Spotters.parray = SafeArrayCreate(VT_BSTR, 1, &sab);

    for (long idx = 0; idx < count; idx++)
    {
        OLECHAR MemValueW[256];
        memset(MemValueW, 0, sizeof(MemValueW));
        ::MultiByteToWideChar(CP_ACP, 0, *calls++, -1, MemValueW, sizeof(MemValueW));
        BSTR s = SysAllocString(MemValueW);
        ::SafeArrayPutElement(Calls.parray, &idx, s);
        ::SysFreeString(s);
        ::MultiByteToWideChar(CP_ACP, 0, *notes++, -1, MemValueW, sizeof(MemValueW));
        s = ::SysAllocString(MemValueW);
        ::SafeArrayPutElement(Notes.parray, &idx, s);
        ::SysFreeString(s);
        ::MultiByteToWideChar(CP_ACP, 0, *spotters++, -1, MemValueW, sizeof(MemValueW));
        s = ::SysAllocString(MemValueW);
        ::SafeArrayPutElement(Spotters.parray, &idx, s);
        ::SysFreeString(s);
        ::SafeArrayPutElement(RxFreqs.parray, &idx, rcvs++);
        ::SafeArrayPutElement(TxFreqs.parray, &idx, xmits++);
        ::SafeArrayPutElement(Modes.parray, &idx, modes++);
   }
    
    DISPPARAMS dispparams;
    dispparams.cArgs = NUM_ARGS;
    dispparams.cNamedArgs = 0;
    dispparams.rgvarg = &args[0];

    TestDispId(IMETH_CREMWLOGE_NEWSPOTS);
    HRESULT hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_NEWSPOTS],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      &dispparams, &varResult, NULL, NULL);
}

short CRemWlogE::get_CurrentFieldNumber(void)
{
    HRESULT hresult;
    VARIANT varResult;

    VariantInit(&varResult);
       TestDispId(IMETH_CREMWLOGE_CURRENTFIELDNUMBER);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_CURRENTFIELDNUMBER],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_PROPERTYGET,
      &g_dispparamsNoArgs, &varResult, NULL, NULL);

    if(hresult != NOERROR)
      return -1;


    return V_I2(&varResult);
}

IDispatch *CRemWlogE::GetSbRtty(long Proc)
{
	HRESULT hresult;
	VARIANT varResult;
    DISPPARAMS pdispparam;
    VARIANTARG  pdisparg[1];

	pdispparam.rgvarg = pdisparg;
    pdispparam.rgdispidNamedArgs = 0;
    pdispparam.cArgs = 1;
    pdispparam.cNamedArgs = 0;

    VariantInit(&pdisparg[0]);
    V_VT(&pdisparg[0]) = VT_I4;
    V_I4(&pdisparg[0]) = Proc;


	VariantInit(&varResult);
	TestDispId(IMETH_CREMWLOGE_GETSBRTTY);

	hresult = m_pdisp->Invoke(
			m_rgdispid[IMETH_CREMWLOGE_GETSBRTTY],
			IID_NULL,
			LOCALE_SYSTEM_DEFAULT,
			DISPATCH_METHOD,
			&pdispparam, &varResult, 0, 0);

	return V_DISPATCH(&varResult);
}

void
CRemWlogE::put_CurrentFieldNumber(short NewVal)
{
    DISPPARAMS pdispparam;
    HRESULT hresult;
    VARIANTARG  pdisparg[1];

    pdispparam.rgvarg = pdisparg;
    pdispparam.rgdispidNamedArgs = &Put;
    pdispparam.cArgs = 1;
    pdispparam.cNamedArgs = 1;

    VariantInit(&pdisparg[0]);
    V_VT(&pdisparg[0]) = VT_I2;
    V_I2(&pdisparg[0]) = NewVal;


        TestDispId(IMETH_CREMWLOGE_CURRENTFIELDNUMBER);
   hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_CURRENTFIELDNUMBER],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_PROPERTYPUT,
      &pdispparam, 0, NULL, NULL);
    return;
}

void CRemWlogE::SetCurrentField(const char *s)
{
    HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
    VARIANT varResult;
    BSTR FieldN;
    OLECHAR FieldW[100];
	memset(FieldW, 0, sizeof(FieldW));
    MultiByteToWideChar(CP_ACP, 0, s, -1, FieldW, 99);
    FieldN = SysAllocString(FieldW);
    hresult = DispBuildParams(&pdispparams, 0, NULL, "b", FieldN);
    if(hresult != NOERROR)
        return;

    VariantInit(&varResult);
        TestDispId(IMETH_CREMWLOGE_PUTCURRENTFIELD);
   hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_PUTCURRENTFIELD],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
      pdispparams, &varResult, NULL, NULL);

    DispFreeParams(pdispparams);
}

short 
CRemWlogE::SeenThisContest(void)
{
    VARIANT varResult;
    VariantInit(&varResult);
	V_I2(&varResult) = 0;
        TestDispId(IMETH_CREMWLOGE_SEENTHISCONTEST);
	m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_SEENTHISCONTEST],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
      &g_dispparamsNoArgs, &varResult, NULL, NULL);
	return V_I2(&varResult);
}

short
CRemWlogE::PutBestField(const char *s)
{
    HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
    VARIANT varResult;
    short    ReturnValue;
    BSTR FieldN;
    OLECHAR FieldW[100];
	memset(FieldW, 0, sizeof(FieldW));
    MultiByteToWideChar(CP_ACP, 0, s, -1, FieldW, 99);
    FieldN = SysAllocString(FieldW);
    hresult = DispBuildParams(&pdispparams, 0, NULL, "b", FieldN);
    if(hresult != NOERROR)
        return 0;

    VariantInit(&varResult);
        TestDispId(IMETH_CREMWLOGE_PUTBESTFIELD);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOGE_PUTBESTFIELD],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
      pdispparams, &varResult, NULL, NULL);

    DispFreeParams(pdispparams);
    ReturnValue = V_I2(&varResult);
    return ReturnValue;
}

CRemWlog FAR *CRemWlogE::GetParent(void)
{
    HRESULT hresult;
    VARIANT varResult;
    IDispatch FAR *Dispatch;

    VariantInit(&varResult);
        TestDispId(IMETH_CREMWLOGE_GETPARENT);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOGE_GETPARENT],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &g_dispparamsNoArgs, &varResult, NULL, NULL);

    if(hresult != NOERROR)
      return 0;

     if ((Dispatch = V_DISPATCH(&varResult)) != 0)
     {
        CRemWlog *Parent =  new CRemWlog(Dispatch);
		Dispatch->Release();
		return Parent;
     }
    return 0;
}

short CRemWlogE::GetBand(void)
{
    HRESULT hresult;
    VARIANT varResult;

    VariantInit(&varResult);
    TestDispId(IMETH_CREMWLOGE_GETBAND);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOGE_GETBAND],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &g_dispparamsNoArgs, &varResult, NULL, NULL);

    if ((hresult != NOERROR) ||
			(V_VT(&varResult) != VT_I2))
      return -1;

    return V_I2(&varResult);
}

short CRemWlogE::GetLeftRight(void)
{
    HRESULT hresult;
    VARIANT varResult;

    VariantInit(&varResult);
    TestDispId(IMETH_CREMWLOGE_GETLEFTRIGHT);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOGE_GETLEFTRIGHT],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &g_dispparamsNoArgs, &varResult, NULL, NULL);

    if ((hresult != NOERROR) ||
			(V_VT(&varResult) != VT_I2))
      return -1;

    return V_I2(&varResult);
}


void CRemWlogE::CancelTimedCQ(void)
{
    HRESULT hresult;
    TestDispId(IMETH_CREMWLOGE_CANCELTIMEDCQ);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOGE_CANCELTIMEDCQ],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &g_dispparamsNoArgs, 0, NULL, NULL);

}

void CRemWlogE::SetAsReturn(void)
{
    HRESULT hresult;
    TestDispId(IMETH_CREMWLOGE_SETASRETURN);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOGE_SETASRETURN],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &g_dispparamsNoArgs, 0, NULL, NULL);
}

HRESULT CRemWlogE::DupeCheckList(VARIANT Call,
							VARIANT Rx, VARIANT Tx, VARIANT Mode, VARIANT &res)
{
    HRESULT hresult;
    TestDispId(IMETH_CREMWLOGE_DUPECHECKLIST);

    DISPPARAMS dispparams;
	memset(&dispparams, 0, sizeof(dispparams));

	VARIANT	Args[4];
	Args[3] = Call; Args[2] = Rx; Args[1] = Tx; Args[0] = Mode;
	dispparams.rgvarg = Args;
	dispparams.cArgs = 4;

     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOGE_DUPECHECKLIST],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &dispparams, &res, NULL, NULL);

	 return hresult;
}

short	CRemWlogE::DupeCheck(const char *Call, double Rx, double Tx, short Mode)
{
	HRESULT hr = E_FAIL;
	short Dupe = 0;
	OLECHAR	CallW[100];
	memset(CallW, 0, sizeof(CallW));
    MultiByteToWideChar(CP_ACP, 0, Call, -1, CallW, 99);

	long ArrayIndex = 0;

	VARIANT Callv, Rxv, Txv, Modev, Result;
	VariantInit(&Callv); VariantInit(&Rxv); VariantInit(&Txv); VariantInit(&Modev);
	VariantInit(&Result);
	
	SAFEARRAY *pCallArray = 0, *pRxArray = 0, *pTxArray = 0, *pModeArray = 0;
	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].lLbound = 0;
	rgsabound[0].cElements = 1;

	pCallArray = SafeArrayCreate(VT_BSTR, 1, rgsabound);
	Callv.vt = VT_BSTR | VT_ARRAY;
	Callv.parray = pCallArray;

	pRxArray = SafeArrayCreate(VT_R8, 1, rgsabound);
	Rxv.vt = VT_R8 | VT_ARRAY;
	Rxv.parray = pRxArray;

	pTxArray = SafeArrayCreate(VT_R8, 1, rgsabound);
	Txv.vt = VT_R8 | VT_ARRAY;
	Txv.parray = pTxArray;

	pModeArray = SafeArrayCreate(VT_I2, 1, rgsabound);
	Modev.vt = VT_I2 | VT_ARRAY;
	Modev.parray = pModeArray;

	if (pCallArray && pRxArray && pTxArray && pModeArray)
	{
		BSTR CallBstr = SysAllocString(CallW);
		SafeArrayPutElement(pCallArray, &ArrayIndex, CallBstr);
		SafeArrayPutElement(pRxArray, &ArrayIndex, &Rx);
		SafeArrayPutElement(pTxArray, &ArrayIndex, &Tx);
		SafeArrayPutElement(pModeArray, &ArrayIndex, &Mode);
		hr = DupeCheckList(Callv, Rxv, Txv, Modev, Result);
		SysFreeString(CallBstr);
	}
	if (SUCCEEDED(hr))
	{
		if (Result.vt == (VT_ARRAY | VT_I2))
		{
			SafeArrayGetElement(Result.parray, &ArrayIndex, &Dupe);
		}
	}
	VariantClear(&Callv); VariantClear(&Rxv); VariantClear(&Txv); VariantClear(&Modev);
	VariantClear(&Result);
	return Dupe;

}

BSTR CRemWlogE::GetFieldTitle(short i)
{
	VARIANT pdisp;
	DISPPARAMS dispparams;
	VARIANT	varResult;

	VariantInit(&pdisp);
	VariantInit(&varResult);

	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 0;
	dispparams.rgvarg = &pdisp;
	dispparams.rgdispidNamedArgs = 0;

	V_VT(&pdisp) = VT_I2;
	V_I2(&pdisp) = i;

	TestDispId(IMETH_CREMWLOGE_FIELDTITLE);
	m_pdisp->Invoke(
		m_rgdispid[IMETH_CREMWLOGE_FIELDTITLE],
		IID_NULL, LOCALE_SYSTEM_DEFAULT,
		DISPATCH_PROPERTYGET,
		&dispparams,
		&varResult, 0, 0);

	if (V_VT(&varResult) == VT_BSTR)
		return (V_BSTR(&varResult));
	return 0;
}

short CRemWlogE::GetEntryId()
{
    short ret = -1;

	HRESULT hresult;
	VARIANT varResult;

	VariantInit(&varResult);
	TestDispId(IMETH_CREMWLOGE_GETENTRYID);

	hresult = m_pdisp->Invoke(
			m_rgdispid[IMETH_CREMWLOGE_GETENTRYID],
			IID_NULL,
			LOCALE_SYSTEM_DEFAULT,
			DISPATCH_METHOD,
			&g_dispparamsNoArgs, &varResult, 0, 0);

	if (SUCCEEDED(hresult))
        ret = V_I2(&varResult);

    return ret;
}

void CRemWlogE::PushCall(short entryId)
{
	VARIANT pdisp;
	DISPPARAMS dispparams;
    HRESULT hresult;

	VariantInit(&pdisp);

	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 0;
	dispparams.rgvarg = &pdisp;
	dispparams.rgdispidNamedArgs = 0;

	V_VT(&pdisp) = VT_I2;
	V_I2(&pdisp) = entryId;

    TestDispId(IMETH_CREMWLOGE_PUSHCALL);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOGE_PUSHCALL],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &dispparams, 0, NULL, NULL);
}



IUnknown FAR *CRemWlogE::UnknownInterface()
{
	m_pdisp->AddRef();
	return (m_pdisp);
}

void CRemWlogE::TestDispId(int X)
{
	OLECHAR	MsgBuf[100];
	if (m_rgdispid[X] == -1) 
	{
#ifndef WIN32
		wsprintf(MsgBuf, 
			"Unrecognized member name: %s", m_rgszMethods[X]);
		MessageBox(NULL, MsgBuf, "WriteLog", MB_OK);
#else
		wsprintfW(MsgBuf, 
			L"Unrecognized member name: %s", m_rgszMethods[X]);
		MessageBoxW(NULL, MsgBuf, L"WriteLog", MB_OK);
#endif
	}
}

/***
*HRESULT CRemWlog::Create(clsid, CRemWlog**)
*
*Purpose:
*  This function creates an instance of the CRemWlog class, connects
*  it to the IDispatch interface of the remote CPoly class, and learns
*  the DISPIDs for the members (that we know about) exposed by that
*  class.
*
*Entry:
*  clsid = The CLSID of the CPoly we are to create. (taking this as a
*    param is a bit weird, but allows us to connect to several remote
*    versions.
*
*Exit:
*  return value = HRESULT
*
*  *pprempoly = pointer to the newly created CRemWlog, if successfyl.
*
***********************************************************************/
CRemWlog FAR * CRemWlog::Create(CLSID clsid, int CreateIfNotActive)
{
	 IDispatch FAR* punk;
	 IUnknown    FAR *ActiveObj;
	 CRemWlog FAR* premwlog;
	 HRESULT hresult;

	 premwlog = new FAR CRemWlog;
	 if (premwlog == 0)
		  return 0;
	 premwlog->m_pdisp = 0;

	 if (GetActiveObject(clsid, 0, &ActiveObj) == S_OK)
	 {
		  ActiveObj->QueryInterface(IID_IDispatch, (void FAR* FAR*) &punk);
		  ActiveObj->Release();
	 }
	else if (CreateIfNotActive)
	 {
		  // create an instance of the remote CWlog class.
		  //
		  hresult = CoCreateInstance(
				clsid, NULL,
				CLSCTX_LOCAL_SERVER ,
				IID_IDispatch, (void FAR* FAR*)&punk);
		  if(FAILED(hresult))
		  {
				goto LFreeCRemWlog;
		  }
	 }
	else
		goto LFreeCRemWlog;
	 // were going to talk to this remote instance via IDispatch.
	 //
	 premwlog->m_pdisp = punk;

	 if (!InitDisp("WriteLog File", IMETH_CREMWLOG_MAX,
				premwlog->m_pdisp,
				premwlog->m_rgszMethods,
				premwlog->m_rgdispid ))
		  goto LFreeCRemWlog;

	 return premwlog;

LFreeCRemWlog:;
	 delete premwlog;

	 return 0;
}

CRemWlog::CRemWlog()
{
	 m_pdisp = 0;
}

CRemWlog::CRemWlog(LPDISPATCH Disp)
{
	 m_pdisp = Disp;
	 int res = Disp->AddRef();
     res = res + 0;
	 InitDisp("WriteLog File", IMETH_CREMWLOG_MAX,
					 m_pdisp,
					 m_rgszMethods,
					 m_rgdispid );
}

CRemWlogE FAR *CRemWlog::CreateEntry()
{
	 return Entry(IMETH_CREMWLOG_CREATEENTRY);
}

CRemWlogE FAR *CRemWlog::GetCurrentEntry()
{
	 return Entry(IMETH_CREMWLOG_GETCURENTRY);
}

CRemWlogE FAR *CRemWlog::GetFocusEntry()
{
	 return Entry(IMETH_CREMWLOG_GETFOCENTRY);
}

void CRemWlog::TestDispId(int X)
{
	OLECHAR	MsgBuf[100];
	if (m_rgdispid[X] == -1) 
	{
#ifndef WIN32
		wsprintf(MsgBuf, 
			"Unrecognized member name: %s", m_rgszMethods[X]);
		MessageBox(NULL, MsgBuf, "WriteLog", MB_OK);
#else
		wsprintfW(MsgBuf, 
			L"Unrecognized member name: %s", m_rgszMethods[X]);
		MessageBoxW(NULL, MsgBuf, L"WriteLog", MB_OK);
#endif
	}
}

CRemWlogE FAR *CRemWlog::Entry(int Index)
{
	 HRESULT hresult;
	 VARIANT varResult;
	 IDispatch FAR *Dispatch;
	 TestDispId(Index);
	 VariantInit(&varResult);
	  hresult = m_pdisp->Invoke(
		m_rgdispid[Index],
		IID_NULL,
		LOCALE_SYSTEM_DEFAULT,
		DISPATCH_METHOD,
		&g_dispparamsNoArgs, &varResult, NULL, NULL);

	 if(hresult != NOERROR)
		return 0;
	  CRemWlogE FAR *NewEntry = 0;
	  if ((Dispatch = V_DISPATCH(&varResult)) != 0)
	  {
		  NewEntry = new CRemWlogE(Dispatch);
	  }
	  VariantClear(&varResult);
	  return NewEntry;
}


CRemWlog:: ~CRemWlog()
{
	 if (m_pdisp)
     {
	        int res =  m_pdisp->Release();
            res = res + 0;
     }
    m_pdisp = 0;
}

BSTR
CRemWlog::GetMemMsg(short n)
{
    HRESULT hresult;
    VARIANT varResult;
    BSTR    ReturnValue;
    VARIANTARG varg;
     DISPPARAMS dispparams;

     V_VT(&varg) = VT_I2;
    V_I2(&varg) = n;

    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    dispparams.rgvarg = &varg;

    VariantInit(&varResult);
     TestDispId(IMETH_CREMWLOG_GETMEMMSG);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOG_GETMEMMSG],
      IID_NULL,
      LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      &dispparams, &varResult, NULL, NULL);

    if(hresult != NOERROR)
      return 0;

    ReturnValue = V_BSTR(&varResult);

    return ReturnValue;
}

 BSTR
CRemWlog::GetFKeyMsgDigital(short n)
 {
	 HRESULT hresult;
	 VARIANT varResult;
	 BSTR    ReturnValue;
	 VARIANTARG varg;
     DISPPARAMS dispparams;
	 
     V_VT(&varg) = VT_I2;
	 V_I2(&varg) = n;
	 
	 dispparams.cArgs = 1;
	 dispparams.cNamedArgs = 0;
	 dispparams.rgvarg = &varg;
	 
	 VariantInit(&varResult);
     TestDispId(IMETH_CREMWLOG_GETFKEYMSGDIGITAL);
	 hresult = m_pdisp->Invoke(
		 m_rgdispid[IMETH_CREMWLOG_GETFKEYMSGDIGITAL],
		 IID_NULL,
		 LOCALE_SYSTEM_DEFAULT,
		 DISPATCH_METHOD,
		 &dispparams, &varResult, NULL, NULL);
	 
	 if(hresult != NOERROR)
		 return 0;
	 
	 ReturnValue = V_BSTR(&varResult);
	 
	 return ReturnValue;
 }

void
CRemWlog::SetFKeyMsgDigital(short n, const char *s)
 {
    HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
    VARIANT varResult;
    BSTR bMemValue;
    OLECHAR MemValueW[100];
	memset(MemValueW, 0, sizeof(MemValueW));
    MultiByteToWideChar(CP_ACP, 0, s, -1, MemValueW, 99);
    bMemValue = SysAllocString(MemValueW);
    hresult = DispBuildParams(&pdispparams, 0, NULL, "ib", n,
        bMemValue);
    if(hresult != NOERROR)
        return;

    VariantInit(&varResult);
  	TestDispId(IMETH_CREMWLOG_SETFKEYMSGDIGITAL);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOG_SETFKEYMSGDIGITAL],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
      pdispparams, &varResult, NULL, NULL);

    DispFreeParams(pdispparams);
 }

BSTR
CRemWlog::GetFKeyMsgVoice(short n)
{
    HRESULT hresult;
    VARIANT varResult;
    BSTR    ReturnValue;
    VARIANTARG varg;
	DISPPARAMS dispparams;
	
	V_VT(&varg) = VT_I2;
    V_I2(&varg) = n;
	
    dispparams.cArgs = 1;
    dispparams.cNamedArgs = 0;
    dispparams.rgvarg = &varg;
	
    VariantInit(&varResult);
	TestDispId(IMETH_CREMWLOG_GETFKEYMSGVOICE);
    hresult = m_pdisp->Invoke(
		m_rgdispid[IMETH_CREMWLOG_GETFKEYMSGVOICE],
		IID_NULL,
		LOCALE_SYSTEM_DEFAULT,
		DISPATCH_METHOD,
		&dispparams, &varResult, NULL, NULL);
	
    if(hresult != NOERROR)
		return 0;
	
    ReturnValue = V_BSTR(&varResult);
	
    return ReturnValue;
}

void
CRemWlog::SetFKeyMsgVoice(short n, const char *s)
 {
    HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
    VARIANT varResult;
    BSTR bMemValue;
    OLECHAR MemValueW[100];
	memset(MemValueW, 0, sizeof(MemValueW));
    MultiByteToWideChar(CP_ACP, 0, s, -1, MemValueW, 99);
    bMemValue = SysAllocString(MemValueW);
    hresult = DispBuildParams(&pdispparams, 0, NULL, "ib", n,
        bMemValue);
    if(hresult != NOERROR)
        return;

    VariantInit(&varResult);
  	TestDispId(IMETH_CREMWLOG_SETFKEYMSGVOICE);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOG_SETFKEYMSGVOICE],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
      pdispparams, &varResult, NULL, NULL);

    DispFreeParams(pdispparams);
 }


void
CRemWlog::SetVisible(short Vis)
{
    DISPPARAMS pdispparams;
    VARIANTARG  pdisparg[1];
    HRESULT hresult;
    SCODE result;

    pdispparams.rgvarg = pdisparg;
    pdispparams.rgdispidNamedArgs = 0;
    pdispparams.cArgs = 1;
    pdispparams.cNamedArgs = 0;

    VariantInit(&pdisparg[0]);
    V_VT(&pdisparg[0]) = VT_I2;
    V_I2(&pdisparg[0]) = Vis;

     TestDispId(IMETH_CREMWLOG_VISIBLE);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOG_VISIBLE],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      &pdispparams, 0, NULL, NULL);
    result = GetScode(hresult);
    if (FAILED(hresult))
        MessageBox(0, "Set Visible failed", "Wlog Packet", 0);
    return;
}

void
CRemWlog::SendProgramMsg(short MemoryNumber)
{
    DISPPARAMS pdispparams;
    VARIANTARG  pdisparg[1];
    HRESULT hresult;


    pdispparams.rgvarg = pdisparg;
    pdispparams.rgdispidNamedArgs = 0;
    pdispparams.cArgs = 1;
    pdispparams.cNamedArgs = 0;

    VariantInit(&pdisparg[0]);
    V_VT(&pdisparg[0]) = VT_I2;
    V_I2(&pdisparg[0]) = MemoryNumber;

      TestDispId(IMETH_CREMWLOG_SNDPROGMSG);
   hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOG_SNDPROGMSG],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
      DISPATCH_METHOD,
      &pdispparams, 0, NULL, NULL);
    return;
}

void CRemWlog::SetRig(short CommIndex,
    short Mode, long Xmit, long Rcv, short Fcn, short Split)
{
    HRESULT hresult;
    DISPPARAMS FAR* pdispparams;

    hresult = DispBuildParams(&pdispparams, 0, NULL, "iiIIii",
        CommIndex, Mode,
        Xmit, Rcv, Fcn, Split);
    if(hresult != NOERROR)
        return ;

       TestDispId(IMETH_CREMWLOG_SETRIG);
   hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOG_SETRIG],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
        pdispparams, NULL, NULL, NULL);

    DispFreeParams(pdispparams);
}

IUnknown FAR *CRemWlog::UnknownInterface()
{
	m_pdisp->AddRef();
	return (m_pdisp);
}


IDispatch *CRemWlog::GetWlSound(void)
{
	HRESULT hresult;
	VARIANT varResult;

	VariantInit(&varResult);
	TestDispId(IMETH_CREMWLOG_GETWLSOUND);

	hresult = m_pdisp->Invoke(
			m_rgdispid[IMETH_CREMWLOG_GETWLSOUND],
			IID_NULL,
			LOCALE_SYSTEM_DEFAULT,
			DISPATCH_METHOD,
			&g_dispparamsNoArgs, &varResult, 0, 0);

	return V_DISPATCH(&varResult);
}


short CRemWlog::FileUserOpen(void)
{
    HRESULT hresult;
    VARIANT varResult;

    VariantInit(&varResult);
    TestDispId(IMETH_CREMWLOG_FILEUSEROPEN);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOG_FILEUSEROPEN],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &g_dispparamsNoArgs, &varResult, NULL, NULL);

    if ((hresult != NOERROR) ||
			(V_VT(&varResult) != VT_I2))
      return -1;

    return V_I2(&varResult);
}

short CRemWlog::OptionsRegisterNet(void)
{
    HRESULT hresult;
    VARIANT varResult;

    VariantInit(&varResult);
    TestDispId(IMETH_CREMWLOG_OPTIONSREGISTERNET);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOG_OPTIONSREGISTERNET],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &g_dispparamsNoArgs, &varResult, NULL, NULL);

    if ((hresult != NOERROR) ||
			(V_VT(&varResult) != VT_I2))
      return -1;

    return V_I2(&varResult);
}

long CRemWlog::Activate(void)
{
    HRESULT hresult;
    VARIANT varResult;

    VariantInit(&varResult);
    TestDispId(IMETH_CREMWLOG_ACTIVATE);
	AllowSetForeground();
	hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOG_ACTIVATE],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &g_dispparamsNoArgs, &varResult, NULL, NULL);
	VariantChangeType(&varResult, &varResult, 0, VT_I4);
	long retval = V_I4(&varResult);
	VariantClear(&varResult);
	if (SUCCEEDED(hresult) && retval)
	{
		ForceReturnForeground((HWND) retval);
	}
	return retval;
}

void CRemWlog::SortByTime(void)
{
    HRESULT hresult;
    VARIANT varResult;

    VariantInit(&varResult);
    TestDispId(IMETH_CREMWLOG_SORTBYTIME);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOG_SORTBYTIME],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &g_dispparamsNoArgs, &varResult, NULL, NULL);

}

void CRemWlog::NotifyXmitStart(void)
{
    HRESULT hresult;
    VARIANT varResult;

    VariantInit(&varResult);
    TestDispId(IMETH_CREMWLOG_NOTIFYXMIT);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOG_NOTIFYXMIT],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &g_dispparamsNoArgs, &varResult, NULL, NULL);

}

void CRemWlog::NotifyXmitFinished(void)
{
    HRESULT hresult;
    VARIANT varResult;

    VariantInit(&varResult);
    TestDispId(IMETH_CREMWLOG_NOTIFYXMITFINISHED);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOG_NOTIFYXMITFINISHED],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &g_dispparamsNoArgs, &varResult, NULL, NULL);

}

void CRemWlog::CloseNet(void)
{
    HRESULT hresult;
    TestDispId(IMETH_CREMWLOG_CLOSENET);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOG_CLOSENET],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_METHOD,
		  &g_dispparamsNoArgs, 0, NULL, NULL);

}


short CRemWlog::GetNetLetter(void)
{
    HRESULT hresult;
    VARIANT varResult;
	short ret = -1;

    VariantInit(&varResult);
    TestDispId(IMETH_CREMWLOG_NETLETTER);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOG_NETLETTER],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_PROPERTYGET,
		  &g_dispparamsNoArgs, &varResult, NULL, NULL);
	 if (V_VT(&varResult) == VT_I2)
		ret = V_I2(&varResult);
	 VariantClear(&varResult);
	 return ret;

}

void CRemWlog::SetNetLetter(short v)
{
    HRESULT hresult;
    VARIANT pdisp;
     DISPPARAMS dispparams;


    VariantInit(&pdisp);
	V_VT(&pdisp) = VT_I2;
	V_I2(&pdisp) = v;
	dispparams.cArgs = 1;
	dispparams.cNamedArgs = 1;
	dispparams.rgvarg = &pdisp;
	dispparams.rgdispidNamedArgs = &Put;

    TestDispId(IMETH_CREMWLOG_NETLETTER);
     hresult = m_pdisp->Invoke(
		  m_rgdispid[IMETH_CREMWLOG_NETLETTER],
		  IID_NULL,
		  LOCALE_SYSTEM_DEFAULT,
		  DISPATCH_PROPERTYPUT,
		  &dispparams, 0, NULL, NULL);
	VariantClear(&pdisp);
}

short
CRemWlog::FileOpen(const char *s)
{
    HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
    VARIANT varResult;
    short    ReturnValue;
    BSTR FieldN;
    OLECHAR FieldW[100];
	memset(FieldW, 0, sizeof(FieldW));
    MultiByteToWideChar(CP_ACP, 0, s, -1, FieldW, 99);
    FieldN = SysAllocString(FieldW);
    hresult = DispBuildParams(&pdispparams, 0, NULL, "bi", FieldN, 0);
    if(hresult != NOERROR)
        return 0;

    VariantInit(&varResult);
        TestDispId(IMETH_CREMWLOG_FILEOPEN);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOG_FILEOPEN],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
      pdispparams, &varResult, NULL, NULL);

    DispFreeParams(pdispparams);
    ReturnValue = V_I2(&varResult);
    return ReturnValue;
}


short
CRemWlog::DoSuperCheck(const char *s, unsigned int *Pos, unsigned int *Len)
{
    HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
    VARIANT varResult;
    short    ReturnValue;
    BSTR FieldN;
    OLECHAR FieldW[100];
	memset(FieldW, 0, sizeof(FieldW));
    MultiByteToWideChar(CP_ACP, 0, s, -1, FieldW, 99);
    FieldN = SysAllocString(FieldW);
    hresult = DispBuildParams(&pdispparams, 0, NULL, "b&I&I", FieldN, Pos, Len);
    if(hresult != NOERROR)
        return 0;

    VariantInit(&varResult);
        TestDispId(IMETH_CREMWLOG_DOSUPERCHECK);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOG_DOSUPERCHECK],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
      pdispparams, &varResult, NULL, NULL);

    DispFreeParams(pdispparams);
    ReturnValue = V_I2(&varResult);
    return ReturnValue;
}

void
CRemWlog::NewSpot(const char *s, double Rcv, double Xmit, short Mode)
{
    HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
	VARIANT varResult;
    BSTR FieldN;
    OLECHAR FieldW[100];
	memset(FieldW, 0, sizeof(FieldW));
    MultiByteToWideChar(CP_ACP, 0, s, -1, FieldW, 99);
    FieldN = SysAllocString(FieldW);
    hresult = DispBuildParams(&pdispparams, 0, NULL, "bRRi", FieldN, Rcv, Xmit, Mode);
    if(hresult != NOERROR)
        return ;

	VariantInit(&varResult);
    TestDispId(IMETH_CREMWLOG_NEWSPOT);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOG_NEWSPOT],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
      pdispparams, &varResult, NULL, NULL);
	VariantClear(&varResult);

    DispFreeParams(pdispparams);
}

void
CRemWlog::NewSpotEx(const char *s, double Rcv, double Xmit, short Mode, const char *note)
{
    HRESULT hresult;
    DISPPARAMS FAR* pdispparams;
	VARIANT varResult;
    BSTR FieldN;
	BSTR NoteN;
    OLECHAR FieldW[100];
	OLECHAR NoteW[100];
	memset(FieldW, 0, sizeof(FieldW));
	memset(NoteW, 0, sizeof(NoteW));
    MultiByteToWideChar(CP_ACP, 0, s, -1, FieldW, 99);
	MultiByteToWideChar(CP_ACP, 0, note, -1, NoteW, 99);
    FieldN = SysAllocString(FieldW);
	NoteN = SysAllocString(NoteW);
    hresult = DispBuildParams(&pdispparams, 0, NULL, "bRRib", FieldN, Rcv, Xmit, Mode, NoteN);
    if(hresult != NOERROR)
        return ;

	VariantInit(&varResult);
    TestDispId(IMETH_CREMWLOG_NEWSPOTEX);
    hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOG_NEWSPOTEX],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
      pdispparams, &varResult, NULL, NULL);
	VariantClear(&varResult);

    DispFreeParams(pdispparams);
}

short
CRemWlog::ModifyBlacklist(const char *call, short spotter, short remove)
{
    OLECHAR callW[200];
    MultiByteToWideChar(CP_ACP, 0, call, -1, callW, 200);

    static const int NUMARGS = 3;
    int i = NUMARGS;
    VariantSetup args[NUMARGS];
    args[--i].vt = VT_BSTR;
    args[i].bstrVal = ::SysAllocString(callW);
    args[--i].vt = VT_I2;
    args[i].iVal = spotter;
    args[--i].vt = VT_I2;
    args[i].iVal = remove;

    DISPPARAMS dispparams;
    memset(&dispparams, 0, sizeof(dispparams));
    dispparams.cArgs = NUMARGS;
    dispparams.rgvarg = args;

	VariantSetup varResult;
    TestDispId(IMETH_CREMWLOG_MODIFYBLACKLIST);
    HRESULT hresult = m_pdisp->Invoke(
      m_rgdispid[IMETH_CREMWLOG_MODIFYBLACKLIST],
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,
        DISPATCH_METHOD,
      &dispparams, &varResult, NULL, NULL);
    return varResult.iVal;
}
