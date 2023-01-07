// [!output DLG_HEADER_FILE] : Declaration of the [!output MM_DLG_CLASS_NAME]

#pragma once

#include "resource.h"       // main symbols
#include <string>
#include <vector>
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
	{}

	~[!output MM_DLG_CLASS_NAME]()
	{}

[!if AM_COUNTYLINE]
	int m_countyLineMode;
[!endif]
[!if !NO_NAMEDMULT]
	std::string m_myNamed;
[!endif]
[!if !NO_AYGMULT]
	std::string m_myAyg;
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
		SetDlgItemTextA(IDC_EDIT_MYAYG, m_myAyg.c_str());
[!endif]
[!if !NO_NAMEDMULT]
		SetDlgItemTextA(IDC_EDIT_MYNAMED, m_myNamed.c_str()); // The edit box to display my transmitted exchange
[!endif]
		bHandled = TRUE;
		return 1;  // Let the system set the focus
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
[!if AM_COUNTYLINE]
		m_countyLineMode = IsDlgButtonChecked(IDC_CHECK_COUNTYLINEMODE);
[!endif]
[!if !NO_AYGMULT]
		auto editAyg = GetDlgItem(IDC_EDIT_MYAYG);
		std::vector<char> bufAyg(1 + editAyg.GetWindowTextLengthA());
		editAyg.GetWindowTextA(&bufAyg[0], bufAyg.size());
		m_myAyg = &bufAyg[0];
[!endif]
[!if !NO_NAMEDMULT]
		auto editNamed = GetDlgItem(IDC_EDIT_MYNAMED);
		std::vector<char> bufNamed(1 + editNamed.GetWindowTextLengthA());
		editNamed.GetWindowTextA(&bufNamed[0], bufNamed.size());
		m_myNamed = &bufNamed[0];
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


