#ifndef MMDCOM_H
#define MMDCOM_H

#ifndef PORT_H
#include "port.h"
#endif

#ifndef IWRITELG_H
#include "iwritelg.h"
#endif

#ifndef MMDCF_H
#include "mmdcf.h"
#endif

#include <iwlogbsm.h>

#ifdef __cplusplus

template <class T>
class CWlogMultUnk : public T
{
public:
	CWlogMultUnk(){m_RefCount = 0;};

	// *** IUnknown methods ***
	STDMETHOD_(ULONG,Release) ()
	{
		if (--m_RefCount == 0)
		{
			delete this;
			return 0;
		}
		else
			return m_RefCount;
	}
	STDMETHOD_(ULONG,AddRef) () { return ++m_RefCount;}

protected:
	virtual ~CWlogMultUnk(){};
	unsigned long	m_RefCount;
};


class MmdCom : public CWlogMultUnk<IWlogMulti2>
{
protected:
   IWlogBandSumm FAR *BsmInterface;
	~MmdCom();
	void CvtLongToAscii(long Value, char *pValue, long MaxChars);
public:
	MmdCom();
	DestroyFunction ObjectDestroyed;

	IWriteLog FAR *Parent;

	// *** IWlogMulti methods
	STDMETHOD(SetMMParent)(IWriteLog FAR *);
	STDMETHOD(TranslateAccelerator)(MSG FAR *msg, short AccFlag);
	STDMETHOD(Score)( Configuration_Entry_t FAR *Config, HWND Window,
		unsigned QsoNum, const char FAR *TargetDir) ;
	STDMETHOD(MatchedQso)(QsoPtr_t New, QsoPtr_t Old);
	STDMETHOD(QsoSearch)(THIS_ QsoPtr_t New, QsoPtr_t Old, int FAR *IsGood);
	STDMETHOD(DupeSheetTitle)(THIS_ int DupeSheet, char FAR *Title, int TitleLength);
	STDMETHOD(TallyPrintQso)(THIS_ QsoPtr_t q);
	STDMETHOD(FormatPageSumm)(THIS_ char FAR *Buf, int BufLength);
	STDMETHOD(SetDupeSheet)(THIS_ QsoPtr_t q, int *Sheet);
	STDMETHOD(GetAdifName)(THIS_ long, long, char FAR *pName);

	//IWlogMulti2 methods
	STDMETHOD(WhatsTheBestField)(QsoPtr_t q, const char *, short *);
	STDMETHOD(IsCharOKHere)(QsoPtr_t q, char c,	short Offset);

	// helpful functions
	const char *BandNames(int i);
	QsoPtr_t GetQsoIth(long Index);

};

template <class BASE, class PARENT>
class CWlogEmbeddable : public BASE
{
public:
	CWlogEmbeddable(PARENT *p){m_Parent = p;};

	//IUnknown
    STDMETHODIMP QueryInterface ( REFIID riid, LPVOID FAR* ppvObj)
	{
			return m_Parent->QueryInterface(riid, ppvObj);
	}
	STDMETHOD_(ULONG,Release) (){return m_Parent->Release();}
	STDMETHOD_(ULONG,AddRef) (){return m_Parent->AddRef();}
protected:
	PARENT *m_Parent;
};

template <class T>
class CWlogSaveSummary : public CWlogEmbeddable<IWlogSaveSummary, T>
{
public:
	CWlogSaveSummary(T *p) : CWlogEmbeddable<IWlogSaveSummary, T>(p){};
	//IWlogSaveSummary
	STDMETHOD(SaveTxt)(Configuration_Entry_t  *Config,const char *p){return m_Parent->SaveTxt(Config,p);}
	STDMETHOD(SaveRtf)(Configuration_Entry_t  *Config,const char *p){return m_Parent->SaveRtf(Config,p);}

};

template <class T>
class CWlogContestCommandBase : public CWlogEmbeddable<IWlogContestCommand, T>
{
public:
	CWlogContestCommandBase(T *p) : CWlogEmbeddable<IWlogContestCommand, T>(p){};
	//IWlogContestCommand
	STDMETHOD(GetCommandName)(char *s, short i){return m_Parent->GetCommandName(s,i);}
	STDMETHOD(ExecuteCommand)(const char *s) {return m_Parent->ExecuteCommand(s);}
};

template <class T>
class CWlogContestCommand : public CWlogContestCommandBase<T>
{
public:
	CWlogContestCommand(T *p) : CWlogContestCommandBase<T>(p){}
	STDMETHOD(StartFileSelection)(char *s, short i) {return m_Parent->StartFileSelection(s,i);}
};

template <class T>
class CWlogArrlContestCommand : public CWlogContestCommandBase<T>
{
public:
	CWlogArrlContestCommand(T *p) : CWlogContestCommandBase<T>(p){}
	STDMETHODIMP StartFileSelection(char *s, short i)
	{
		char *Slash = strrchr(s, '\\');
		if (!Slash)
		{
			Slash = s;
		}
		else
		{
			Slash += 1;
		}
		GetPrivateProfileString("REPORT",
								"CALL",
								"",
								Slash,
								i - (Slash - s),
								"Writelog.ini");
		strcat_s(s, i, ".LOG");
		return S_OK;
	}
};

class CTimeOffExf
{
protected:
	IWlogExf *m_Exf;
	char	m_Output[13];
public:
	CTimeOffExf(MmdCom *p)
	{
		char Header[128];
		m_Exf = 0;	
		if (p->Parent)
		{
			p->Parent->GetExf(&m_Exf);
			if (m_Exf)
			{
				if (m_Exf->GoFirst() == S_OK)
				{
					for (;;)
					{
						m_Exf->GetHeader(Header, sizeof(Header));
						if (strcmp(Header,
								"  TIME-OFF   ") == 0)
						{
							break;
						}
						if (m_Exf->GoNext() != S_OK)
						{
							m_Exf->Release();
							m_Exf = 0;
							break;
						}
					}
				}
			}
		}
	}

	~CTimeOffExf()
	{
		if (m_Exf)
			m_Exf->Release();
	}

	const char *GetSpot(QsoPtr_t q)
	{
		int i;
		VarPart_t t = 0;
		if (m_Exf)
		{
			m_Exf->Place(q, &t);
		}
		m_Output[0] = 0;
		if (t)
			strncpy_s(m_Output, t, sizeof(m_Output)-1);
		i = strlen(m_Output);
		while (i < sizeof(m_Output) - 1)
		{
			m_Output[i++] = ' ';
		}
		m_Output[i] = 0;
		return m_Output;
	}
};


template <class T>
class CWlogCabrilloImpl : public CWlogEmbeddable<IWlogCabrillo, T>
{
public:
	CWlogCabrilloImpl(T *p) : CWlogEmbeddable<IWlogCabrillo, T>(p){};

	STDMETHOD(ConfirmFieldsFilled)( HWND w)
	{
		return m_Parent->ConfirmFieldsFilled(w);
	}
	STDMETHOD(GetHeaderLineCount)( short *pLines)
	{
		return m_Parent->GetHeaderLineCount(pLines);
	}
	STDMETHOD(GetHeaderLine)( short LineNumber, char *Buf)
	{
		return m_Parent->GetHeaderLine(LineNumber, Buf);
	}
	STDMETHOD(SetCallsign)( const char *Buf)
	{
		return m_Parent->SetCallsign(Buf);
	}
	STDMETHOD(GetContestName)( char *Buf  )
	{
		return m_Parent->GetContestName(Buf);
	}
	STDMETHOD(GetClaimedScore)( long *Score)
	{
		return m_Parent->GetClaimedScore(Score);
	}
	STDMETHOD(GetTxFieldCount)( short *pCount) 
	{
		return m_Parent->GetTxFieldCount(pCount);
	}
	STDMETHOD(FormatTxField)( QsoPtr_t q, short Field, char *Buf) 
	{
		return m_Parent->FormatTxField(q, Field, Buf);
	}
	STDMETHOD(GetRxFieldCount)( short *pCount)
	{
		return m_Parent->GetRxFieldCount(pCount);
	}
	STDMETHOD(FormatRxField)( QsoPtr_t q, short Field, char *Buf) 
	{
		return m_Parent->FormatRxField(q, Field, Buf);
	}
};

template <class T>
class CWlogCabrillo2Impl : public CWlogEmbeddable<IWlogCabrillo2, T>
{
public:
	CWlogCabrillo2Impl(T *p) : CWlogEmbeddable<IWlogCabrillo2, T>(p){};
	STDMETHOD(LinesForQSO)(QsoPtr_t q, short *pCount)
	{
		return m_Parent->LinesForQSO(q, pCount);
	}
	STDMETHOD(SetCurrentLineNumber)(short Line) 
	{
		return m_Parent->SetCurrentLineNumber(Line);
	}

};

template <class T>
class CWlogCabrillo3Impl : public CWlogCabrillo2Impl<T>
{
public:
	CWlogCabrillo3Impl(T *p) : CWlogCabrillo2Impl<T>(p){};
	STDMETHOD(AdditionalRowsForThisLine)(short *pRows)
	{
		return m_Parent->AdditionalRowsForThisLine(pRows);
	}
	STDMETHOD(GetAdditionalRow)(short RowNumber, IWlogCabrilloHelper *pHelper)
	{
		return m_Parent->GetAdditionalRow(RowNumber, pHelper);
	}
};

template <class T>
class CWlogCabrillo4Impl : public CWlogEmbeddable<IWlogCabrillo4, T>
{
public:
	CWlogCabrillo4Impl(T *p) : CWlogEmbeddable<IWlogCabrillo4, T>(p){};

	STDMETHOD(FormatFreqField)( QsoPtr_t q, short RowNumber, short len, char *pBuf) 
	{
		return m_Parent->FormatFreqField(q, RowNumber, len, pBuf);
	}
	STDMETHOD(FormatModeField)( QsoPtr_t q, short RowNumber, short len, char *pBuf) 
	{
		return m_Parent->FormatModeField(q, RowNumber, len, pBuf);
	}
	STDMETHOD(FormatDateField)( QsoPtr_t q, short RowNumber, short len, char *pBuf) 
	{
		return m_Parent->FormatDateField(q, RowNumber, len, pBuf);
	}
	STDMETHOD(FormatTimeField)( QsoPtr_t q, short RowNumber, short len, char *pBuf) 
	{
		return m_Parent->FormatTimeField(q, RowNumber, len, pBuf);
	}
};


template <class T>
class CWlogCabrilloOptions : public CWlogEmbeddable<IWlogCabrilloOptions, T>
{
public:
	CWlogCabrilloOptions(T *p) : CWlogEmbeddable<IWlogCabrilloOptions, T>(p){};
	STDMETHOD(ItemsForThisCategory)(const char *pCategoryName, long *Items) 
	{
		return m_Parent->ItemsForThisCategory(pCategoryName, Items);
	}
	STDMETHOD(GetItemForCategory)(const char *pCategoryName, long ItemNumber, 
						char *DisplayString, long DisplayStringBufSize,
						char *ValueString, long ValueStringBufSize
						) 
	{
		return m_Parent->GetItemForCategory(pCategoryName, ItemNumber, 
						DisplayString,						DisplayStringBufSize,
						ValueString, ValueStringBufSize);
	}

	STDMETHOD(GetSponsorEmailAddress)(char *pAddr, long AddrSize)
	{
		return m_Parent->GetSponsorEmailAddress(pAddr, AddrSize);
	}

	STDMETHOD(GetCabrilloEmailSubjectLine)(char *pSubject, long SubjectSize)
	{
		return m_Parent->GetCabrilloEmailSubjectLine(pSubject, SubjectSize);
	}

};


template <class T>
class CWlogMultiADIFImpl : public CWlogEmbeddable<IWlogMultiADIF, T>
{
public:
	CWlogMultiADIFImpl(T *p) : CWlogEmbeddable<IWlogMultiADIF, T>(p){};
	STDMETHOD(TranslateToAdif)(QsoPtr_t q, long Offset, char *Buf, long BufLen)
	{
		return m_Parent->TranslateToAdif(q, Offset, Buf, BufLen);
	}
	STDMETHOD(TranslateFromAdif)(QsoPtr_t q, long Offset, const char *Buf)
	{
		return m_Parent->TranslateFromAdif(q, Offset, Buf);
	}
};

template <class T>
class CWlogMultiSingleMinutes : public CWlogEmbeddable<IWlogMultiSingleMinutes, T>
{
public:
	CWlogMultiSingleMinutes(T *p) : CWlogEmbeddable<IWlogMultiSingleMinutes, T>(p){};
	STDMETHOD(SecondsOnBandRequired)(long *Seconds)
	{
		return m_Parent->SecondsOnBandRequired(Seconds);
	}
	STDMETHOD(IsMultiplier)(QsoPtr_t q, long *MultCount)
	{
		return m_Parent->IsMultiplier(q, MultCount);
	}
};

template <class T>
class CWlogMultiSingleBandChangeThrottle : public CWlogEmbeddable<IWlogMultiSingleBandChangeThrottle, T>
{
public:
	CWlogMultiSingleBandChangeThrottle(T *p) : CWlogEmbeddable<IWlogMultiSingleBandChangeThrottle, T>(p){};
	STDMETHOD(GetBandChangeLimits)(long *pSeconds, long *pMaxChanges, long *pPerHour)
	{
		return m_Parent->GetBandChangeLimits(pSeconds, pMaxChanges, pPerHour);
	}
};

template <class T>
class CWlogScoreInfo : public CWlogEmbeddable<IWlogScoreInfo, T>
{
public:
	CWlogScoreInfo(T *p) : CWlogEmbeddable<IWlogScoreInfo, T>(p){};
	STDMETHOD(MultsAndPts)(long *pMultCount, long *pPtsCount)
	{
		return m_Parent->MultsAndPts(pMultCount, pPtsCount);
	}
	STDMETHOD(PtsForQso)(QsoPtr_t q, long *pPtsCount)
	{
		return m_Parent->PtsForQso(q, pPtsCount);
	}
};

template <class T>
class CWlogMultiGridSquare : public CWlogEmbeddable<IWlogMultiGridSquare2, T>
{
public:
	CWlogMultiGridSquare(T *p) : CWlogEmbeddable<IWlogMultiGridSquare2, T>(p){};
	STDMETHOD(GetGridSquares)(QsoPtr_t q, char *pHisSquare, char *pMySquare)
	{
		return m_Parent->GetGridSquares(q, pHisSquare, pMySquare);
	}

	STDMETHOD(AdviseGridSquare)(QsoPtr_t q, char *pHisSquare) 
	{
		return m_Parent->AdviseGridSquare(q, pHisSquare);
	}
};

template <class T>
class CWlogMultiBandCalc : public CWlogEmbeddable<IWlogMultiBandCalc, T>
{
public:
	CWlogMultiBandCalc(T *p) : CWlogEmbeddable<IWlogMultiBandCalc, T>(p){};
	STDMETHOD(BandCalc)(QsoPtr_t q, unsigned char *pBand)
	{
		return m_Parent->BandCalc(q, pBand);
	}

};


template <class T>
class CWlogTQslQsoLocation : public CWlogEmbeddable<IWlogTQslQsoLocation, T>
{
public:
	CWlogTQslQsoLocation(T *p) : CWlogEmbeddable<IWlogTQslQsoLocation, T>(p){};
	STDMETHOD(GetNumLocationFields)( long *pNumfields)
	{ 
		return m_Parent->GetNumLocationFields(pNumfields) ;
	}
	STDMETHOD(GetFieldGABBIName)(long Numfield, char *pFieldName, long FieldNameSize)
	{ 
		return m_Parent->GetFieldGABBIName(Numfield, pFieldName, FieldNameSize) ;
	}
	STDMETHOD(GetQsoNumLocations)( QsoPtr_t q, long *pNumLocations)
	{ 
		return m_Parent->GetQsoNumLocations(q, pNumLocations) ;
	}
	STDMETHOD(GetQsoLocationField)( QsoPtr_t q, long Location, long FieldNum,
		char *pFieldvalue, long FieldValueSize)
	{ 
		return m_Parent->GetQsoLocationField(q, Location, FieldNum, 
			pFieldvalue, FieldValueSize) ;
	}
};

template <class T>
class CWlogMultiOffTime : public CWlogEmbeddable<IWlogMultiOffTime, T>
{
public:
	CWlogMultiOffTime(T *p) : CWlogEmbeddable<IWlogMultiOffTime, T>(p){};
    STDMETHOD(GetMinOffTime)(long *pOffTime)
	{
		return m_Parent->GetMinOffTime(pOffTime);
	}

};

template <class T>
class CWlogMulFeatureEnable : public CWlogEmbeddable<IWlogMulFeatureEnable, T>
{
public:
	CWlogMulFeatureEnable(T *p) : CWlogEmbeddable<IWlogMulFeatureEnable, T>(p){};
    STDMETHOD(SupercheckPartial)() 
    {
		return m_Parent->SupercheckPartial();
	}
    STDMETHOD(LoadFromADIF)() 
    {
		return m_Parent->LoadFromADIF();
	}
    STDMETHOD(CwDecoder)() 
    {
		return m_Parent->CwDecoder();
	}
    STDMETHOD(StartUdpPostingAutomatically)(short *timeMinutes) 
    {
		return m_Parent->StartUdpPostingAutomatically(timeMinutes);
	}
    STDMETHOD(GetPostingUrl)(char *url)
    {
        return m_Parent->GetPostingUrl(url);
    }
    STDMETHOD(GetPostingPortNumber)(short *portnum)
    {
        return m_Parent->GetPostingPortNumber(portnum);
    }
    STDMETHOD(GetPostingEnableBandBreakdown)( short *enableBandBreakdown)
    {
        return m_Parent->GetPostingEnableBandBreakdown(enableBandBreakdown);
    }
    STDMETHOD(GetPostingContestName)(char *name)
    {
        return m_Parent->GetPostingContestName(name);
    }

};

template <class T>
class CWlogCabrilloSettings : public CWlogEmbeddable<IWlogCabrilloSettings, T>
{
public:
	CWlogCabrilloSettings(T *p) : CWlogEmbeddable<IWlogCabrilloSettings, T>(p){};
    STDMETHOD(SelectionForThisCategory)( HWND w, const char *pCategoryName, const char *pSelection)
    {
        return m_Parent->SelectionForThisCategory(w, pCategoryName, pSelection);
    }
};


template <class T>
class CWlogHighlightNetFrequencyDisplay : public CWlogEmbeddable<IWlogHighlightNetFrequencyDisplay, T>
{
public:
    CWlogHighlightNetFrequencyDisplay(T *p) : CWlogEmbeddable<IWlogHighlightNetFrequencyDisplay, T>(p) {};
    STDMETHOD(OnNetFrequencyDisplay)(short StationId, short RigNumber, long freq, short mode, short band,
        short MyId, short idPos, short rigNumPos, char *pMarker, int *pHighlight)
    {
        return m_Parent->OnNetFrequencyDisplay( StationId,  RigNumber,  freq,  mode, band,
             MyId,  idPos,  rigNumPos, pMarker, pHighlight);
    }
};

#endif
#endif                               
