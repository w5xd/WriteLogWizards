// [!output DLG_HEADER_FILE] : Declaration of the [!output MM_DLG_CLASS_NAME]

#pragma once

#include "resource.h"       // main symbols
#include <atlhost.h>

// [!output MM_DLG_CLASS_NAME]

class [!output MM_DLG_CLASS_NAME] : 
	public CAxDialogImpl<[!output MM_DLG_CLASS_NAME]>
{
public:
	[!output MM_DLG_CLASS_NAME]()
	[!if AM_COUNTYLINE]
		: m_countyLineMode(0)
	[!endif]
	{
	}

	~[!output MM_DLG_CLASS_NAME]()
	{
	}

	[!if AM_COUNTYLINE]
	int m_countyLineMode;
	[!endif]

	enum { IDD = [!output IDD_DIALOGID] };

BEGIN_MSG_MAP([!output MM_DLG_CLASS_NAME])
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<[!output MM_DLG_CLASS_NAME]>)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<[!output MM_DLG_CLASS_NAME]>::OnInitDialog(uMsg, wParam, lParam, bHandled);
[!if AM_COUNTYLINE]
		CheckDlgButton(IDC_CHECK_COUNTYLINEMODE, m_countyLineMode);
[!endif]
[!if !NO_AYGMULT]
		//FIXME
		GetDlgItem(IDC_EDIT_MYAYG); // The edit box to display my transmitted exchange
[!endif]
[!if !NO_NAMEDMULT]
		//FIXME
		GetDlgItem(IDC_EDIT_MYNAMED); // The edit box to display my transmitted exchange
[!endif]
		bHandled = TRUE;
		return 1;  // Let the system set the focus
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
[!if AM_COUNTYLINE]
		m_countyLineMode = IsDlgButtonChecked(IDC_CHECK_COUNTYLINEMODE);
[!endif]
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
};


