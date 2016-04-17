// AskMode.h : Declaration of the CAskModeDlg

#pragma once
#include <atlhost.h>
#include "resource.h"
using namespace ATL;

// CAskModeDlg

class CAskModeDlg : public CAxDialogImpl<CAskModeDlg>
{
public:
	CAskModeDlg() : m_mode(IDC_MODESELCW)
	{	}

	enum { IDD = IDD_MODESELECT };

BEGIN_MSG_MAP(CAskModeDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDC_MODESELCW, BN_CLICKED, OnClickedModeButton)
	COMMAND_HANDLER(IDC_MODESELPHONE, BN_CLICKED, OnClickedModeButton)
	CHAIN_MSG_MAP(CAxDialogImpl<CAskModeDlg>)
END_MSG_MAP()

LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CAskModeDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);
        CheckRadioButton(IDC_MODESELCW, IDC_MODESELPHONE, m_mode);
		bHandled = TRUE;
		return 1;  // Let the system set the focus
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

    LRESULT OnClickedModeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {       
        m_mode = wID;    
        return 0;
    }

    WORD m_mode;
};


