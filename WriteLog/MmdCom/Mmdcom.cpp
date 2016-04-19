#include <windows.h>
#include <ole2.h>
#include "mmdcom.h"
#include "iwlogbsm.h"
#include "bndsmadd.h"


MmdCom::MmdCom()
{
	Parent = 0;
   BsmInterface = 0;
	ObjectDestroyed = 0;
}

MmdCom::~MmdCom()
{
   if (BsmInterface)
      BsmInterface->Release();
   BsmInterface = 0;
	if (ObjectDestroyed)
		(*ObjectDestroyed)();
}

QsoPtr_t MmdCom::GetQsoIth(long Index)
{
	QsoPtr_t TestQ;
	if (Parent && Index >= 0)
			Parent->QsoIth((unsigned long)Index, &TestQ);
	else
			TestQ = 0;
	return TestQ;
}


const char *MmdCom::BandNames(int i)
{
	const char *Title;
	if (SUCCEEDED(Parent->BandTitle(i, &Title)))
		return Title;
	else
		return "";
}



STDMETHODIMP MmdCom::SetMMParent(IWriteLog FAR *p)
{
	Parent = p;
    if (BsmInterface)
        BsmInterface->Release();
    BsmInterface = 0;
    wlshr_GetSumm(p, &BsmInterface);
	return NOERROR;
}


STDMETHODIMP MmdCom::TranslateAccelerator(MSG FAR *, short)
{
	return E_NOTIMPL;
}


STDMETHODIMP MmdCom::Score( Configuration_Entry_t FAR *Config, HWND Window,
		unsigned QsoNum, const char FAR *TargetDir)
{
	return ResultFromScode(E_FAIL);
}

STDMETHODIMP MmdCom::MatchedQso(QsoPtr_t New, QsoPtr_t Old)
{
	return NOERROR;
}

STDMETHODIMP MmdCom::QsoSearch(QsoPtr_t Nq, QsoPtr_t Oq, int FAR *IsGood)
{
   if (IsGood)
      *IsGood = 0;
	return NOERROR;
}

STDMETHODIMP MmdCom::DupeSheetTitle(int DupeSheet, char FAR *Title, int TitleLength)
{
	return NOERROR;
}

STDMETHODIMP MmdCom::TallyPrintQso(QsoPtr_t)
{
return NOERROR;
}

STDMETHODIMP MmdCom::FormatPageSumm(char FAR *Buf, int BufLength)
{
if (BufLength)
	Buf[0] = 0;
return NOERROR;
}

STDMETHODIMP MmdCom::SetDupeSheet(QsoPtr_t q, int FAR *d)
{
   if (d)
      *d = 0;
return NOERROR;
}

STDMETHODIMP MmdCom::GetAdifName(long, long, char FAR *)
{
	return E_NOTIMPL;
}


STDMETHODIMP MmdCom::WhatsTheBestField(QsoPtr_t ,const char *, short *)
{
	return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP MmdCom::IsCharOKHere(QsoPtr_t q, char c,	short Offset)
{
	return S_OK;
}

void MmdCom::CvtLongToAscii(long Value, char *pValue, long MaxChars)
{
	// render the LEAST significant MaxChars-1 digits into *pValue
	char Buf[16];
	_itoa_s(Value,  Buf,  10);
	int i = strlen(Buf);
	int j = 0;
	if (i > MaxChars-1)
		j = 1 + i - MaxChars;
	strcpy_s(pValue, MaxChars, Buf+j);
}





