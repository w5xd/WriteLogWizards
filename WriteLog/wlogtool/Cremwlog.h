/***
*cremwlog.h
*
*/
#ifndef CREMWLOG_H
#define CREMWLOG_H

class CRemWlog;

class CRemWlogE {
public:
    CRemWlogE(IDispatch FAR *);
    ~CRemWlogE();
    BSTR GetCallsign(void);
     long PutCallsign(const char *);
    long Dupe(void);
    void Clear(void);
    void EnterQso(void);
    long PutFieldN(short, const char *);
    BSTR GetFieldN(short);
    short GetFieldWidth(short);
    void SetVisible(short);
    short IsNewMultiplier(short);
    short SetLogFrequency(short Mode, long Freq);
    void  MapStation(long Time, short Stat);
    long  Activate(void);
	 short SetLogFrequencyEx(short Mode, double RcvFreq,
							double XmitFreq, short Split);
	 short GetLogFrequency(short *Mode, double *RecvFreq,
								double *XmitFreq, short *Split);
    void put_CurrentFieldNumber(short);
    short get_CurrentFieldNumber(void);
    void SetCurrentField(const char *);
	short PutBestField(const char *);
	short SeenThisContest(void);
	CRemWlog *GetParent(void);
	void put_Field(short, const char *);
	BSTR get_Field(short);
	void CalcLineStatus(const char *, unsigned char *, int);
	short GetBand(void);
	IDispatch *GetSbRtty(long Proc);
	void SetXmitPtt(short);
	void CancelTimedCQ(void);
	void SetAsReturn(void);
	short GetLeftRight(void);
	HRESULT DupeCheckList(VARIANT Call, VARIANT Rx, VARIANT Tx, VARIANT Mode, VARIANT &Result);
	short	DupeCheck(const char *Call, double Rx, double Tx, short Mode);
	BSTR GetFieldTitle(short i);
    void PushCall(short id);
    short GetEntryId();
    void NewSpots(bool singleBand, int count, const char **,  double *,  double *,  short *, const char **,
        const char **);
    void SetKeyboardFocus();

    IUnknown FAR *UnknownInterface();

	static void CvtToChar(BSTR pOleStr, char *Buf, int BufLen)
	{
			WideCharToMultiByte(CP_ACP, 0, pOleStr,
						-1,
						Buf,
						BufLen,
						NULL, NULL);
	}

private:
    enum CREMWLOGE_METHODS {
    IMETH_CREMWLOGE_GETCALLSIGN = 0,
    IMETH_CREMWLOGE_PUTCALLSIGN,
    IMETH_CREMWLOGE_DUPE,
    IMETH_CREMWLOGE_CLEAR,
    IMETH_CREMWLOGE_ENTERQSO,
    IMETH_CREMWLOGE_PUTFIELDN,
    IMETH_CREMWLOGE_GETFIELDN,
    IMETH_CREMWLOGE_GETFIELDWIDTH,
    IMETH_CREMWLOGE_VISIBLE,
    IMETH_CREMWLOGE_ISNEWMULT,
    IMETH_CREMWLOGE_SETLOGFREQ,
    IMETH_CREMWLOGE_MAPSTATION,
    IMETH_CREMWLOGE_ACTIVATE,
	 IMETH_CREMWLOGE_SETLOGFREQEX,
	 IMETH_CREMWLOGE_GETLOGFREQ,
    IMETH_CREMWLOGE_CURRENTFIELDNUMBER,
    IMETH_CREMWLOGE_PUTCURRENTFIELD,
	IMETH_CREMWLOGE_PUTBESTFIELD,
	IMETH_CREMWLOGE_SEENTHISCONTEST,
	IMETH_CREMWLOGE_GETPARENT,
	IMETH_CREMWLOGE_FIELD,
	IMETH_CREMWLOGE_CALCLINESTATUS,
	IMETH_CREMWLOGE_GETBAND,
	IMETH_CREMWLOGE_GETSBRTTY,
	IMETH_CREMWLOGE_SETXMITPTT,
	IMETH_CREMWLOGE_CANCELTIMEDCQ,
	IMETH_CREMWLOGE_SETASRETURN,
	IMETH_CREMWLOGE_GETLEFTRIGHT,
	IMETH_CREMWLOGE_DUPECHECKLIST,
	IMETH_CREMWLOGE_FIELDTITLE,
    IMETH_CREMWLOGE_PUSHCALL,
    IMETH_CREMWLOGE_GETENTRYID,
    IMETH_CREMWLOGE_NEWSPOTS,
    IMETH_CREMWLOGE_SETKEYBOARDFOCUS,
    IMETH_CREMWLOGE_MAX
    };

    IDispatch FAR* m_pdisp;

    // member IDs - these are used by IDispatch::Invoke to identify the
     // method or property on the remote object we accessing.
     //
    DISPID m_rgdispid[IMETH_CREMWLOGE_MAX];
	
	void TestDispId(int);


     // member names - these are used to learn the member IDs when we
     // connect to the remote object.
    //
     static const OLECHAR FAR* m_rgszMethods[IMETH_CREMWLOGE_MAX];
};

class CRemWlog  {
public:
    static CRemWlog FAR *Create(CLSID clsid, int Create);
    CRemWlog();
    CRemWlog(LPDISPATCH);
    ~CRemWlog();
    CRemWlogE FAR *CreateEntry();
    CRemWlogE FAR *GetCurrentEntry();
    CRemWlogE FAR *GetFocusEntry();
    CRemWlogE FAR *GetEntry(short);

    BSTR GetMemMsg(short);
	BSTR GetFKeyMsgDigital(short);
	BSTR GetFKeyMsgVoice(short);
	void SetFKeyMsgDigital(short, const char *);
	void SetFKeyMsgVoice(short, const char *);
    short ModifyBlacklist(const char *Call, short AsSpotter, short RemoveCall);

    void SetVisible(short);
    void SetRig(short CommPort,
        short Mode, long XmitFrequency, long RcvFreq,
        short Fcn, short Split);
    void SendProgramMsg(short MemoryNumber);
	short FileUserOpen(void);
	short OptionsRegisterNet(void);
	long Activate(void);
	void SortByTime(void);
	void NotifyXmitStart(void);
	void NotifyXmitFinished(void);
	IDispatch *GetWlSound(void);
	void SetNetLetter(short);
	short GetNetLetter(void);
	short FileOpen(const char *);
	void CloseNet(void);
	short	DoSuperCheck(const char *, unsigned int *, unsigned int *);
	void NewSpot(const char *, double, double, short);
	void NewSpotEx(const char *, double, double, short, const char *);
    IUnknown FAR *UnknownInterface();
	static void CvtToChar(BSTR pOleStr, char *Buf, int BufLen)
	{
			WideCharToMultiByte(CP_ACP, 0, pOleStr,
						-1,
						Buf,
						BufLen,
						NULL, NULL);
	}

private:
    CRemWlogE FAR *Entry(int);
    enum CREMWLOG_METHODS {
    IMETH_CREMWLOG_CREATEENTRY = 0,
    IMETH_CREMWLOG_GETCURENTRY,
    IMETH_CREMWLOG_GETMEMMSG,
    IMETH_CREMWLOG_VISIBLE,
    IMETH_CREMWLOG_SETRIG,
    IMETH_CREMWLOG_SNDPROGMSG,
    IMETH_CREMWLOG_GETFOCENTRY,
	IMETH_CREMWLOG_FILEUSEROPEN,
	IMETH_CREMWLOG_OPTIONSREGISTERNET,
	IMETH_CREMWLOG_ACTIVATE,
	IMETH_CREMWLOG_SORTBYTIME,
	IMETH_CREMWLOG_NOTIFYXMIT,
	IMETH_CREMWLOG_NOTIFYXMITFINISHED,
	IMETH_CREMWLOG_GETWLSOUND,
	IMETH_CREMWLOG_NETLETTER,
	IMETH_CREMWLOG_FILEOPEN,
	IMETH_CREMWLOG_CLOSENET,
	IMETH_CREMWLOG_DOSUPERCHECK,
	IMETH_CREMWLOG_NEWSPOT,
	IMETH_CREMWLOG_NEWSPOTEX,
	IMETH_CREMWLOG_SETFKEYMSGDIGITAL,
	IMETH_CREMWLOG_GETFKEYMSGDIGITAL,
	IMETH_CREMWLOG_SETFKEYMSGVOICE,
	IMETH_CREMWLOG_GETFKEYMSGVOICE,
    IMETH_CREMWLOG_MODIFYBLACKLIST,
    IMETH_CREMWLOG_GETENTRY,
    IMETH_CREMWLOG_MAX
    };

    IDispatch FAR* m_pdisp;

    // member IDs - these are used by IDispatch::Invoke to identify the
     // method or property on the remote object we accessing.
     //
    DISPID m_rgdispid[IMETH_CREMWLOG_MAX];

 	void TestDispId(int);
    // member names - these are used to learn the member IDs when we
     // connect to the remote object.
    //
     static const OLECHAR FAR* m_rgszMethods[IMETH_CREMWLOG_MAX];
};

#endif
