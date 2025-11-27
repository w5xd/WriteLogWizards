$if$ (0) /*Copyright (c) 2025 by Wayne E. Wright, W5XD
This template was converted from the version used for the old vsz template technology.
The old style used [!if ]. The new one uses the "if" that commands the processor to skip this very commentary.
The difference between the two is (a) that the old tech supported nested "if" clauses while the new one does not,
and (b) the old style supported && and || operators and the new one only supports string compare for "if" clauses.
The way I made this work was to extract all the compounded conditional expressions that in the old tech were
evaluated by the template processor, and make the wizard here evaluate the expressions and write a separate
for each expression.*/$endif$// $DLG_HEADER_FILE$ : Declaration of the $MM_DLG_CLASS_NAME$

#pragma once

#include "resource.h"       // main symbols
#include <string>
#include <vector>
#include <atlhost.h>

// $MM_DLG_CLASS_NAME$

class $MM_DLG_CLASS_NAME$ : 
	public CAxDialogImpl<$MM_DLG_CLASS_NAME$>
{
public:
	$MM_DLG_CLASS_NAME$()
	$if$ ($AM_COUNTYLINE$ == 1)		: m_countyLineMode(0)
	$endif$	{}

	~$MM_DLG_CLASS_NAME$()
	{}

$if$ ($AM_COUNTYLINE$ == 1)	int m_countyLineMode;
$endif$$if$ ($NO_NAMEDMULT$ == 0)	std::string m_myNamed;
$endif$$if$ ($NO_AYGMULT$ == 0)	std::string m_myAyg;
$endif$
	enum { IDD = $IDD_DIALOGID$ };

BEGIN_MSG_MAP($MM_DLG_CLASS_NAME$)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<$MM_DLG_CLASS_NAME$>)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<$MM_DLG_CLASS_NAME$>::OnInitDialog(uMsg, wParam, lParam, bHandled);
$if$ ($AM_COUNTYLINE$ == 1)		CheckDlgButton(IDC_CHECK_COUNTYLINEMODE, m_countyLineMode);
$endif$$if$ ($NO_AYGMULT$ == 0)		SetDlgItemTextA(IDC_EDIT_MYAYG, m_myAyg.c_str());
$endif$$if$ ($NO_NAMEDMULT$ == 0)		SetDlgItemTextA(IDC_EDIT_MYNAMED, m_myNamed.c_str()); // The edit box to display my transmitted exchange
$endif$		bHandled = TRUE;
		return 1;  // Let the system set the focus
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
$if$ ($AM_COUNTYLINE$ == 1)		m_countyLineMode = IsDlgButtonChecked(IDC_CHECK_COUNTYLINEMODE);
$endif$$if$ ($NO_AYGMULT$ == 0)		auto editAyg = GetDlgItem(IDC_EDIT_MYAYG);
		std::vector<char> bufAyg(1 + editAyg.GetWindowTextLengthA());
		editAyg.GetWindowTextA(&bufAyg[0], bufAyg.size());
		m_myAyg = &bufAyg[0];
$endif$$if$ ($NO_NAMEDMULT$ == 0)		auto editNamed = GetDlgItem(IDC_EDIT_MYNAMED);
		std::vector<char> bufNamed(1 + editNamed.GetWindowTextLengthA());
		editNamed.GetWindowTextA(&bufNamed[0], bufNamed.size());
		m_myNamed = &bufNamed[0];
$endif$		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
};


