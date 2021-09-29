// Declaration of the [!output MM_ROVERDLG_CLASS_NAME]
#pragma once

#include "resource.h"       // main symbols
#include <atlhost.h>
#include <functional>

// [!output MM_ROVERDLG_CLASS_NAME]
//
// When the wizard puts this file in your project, it also does a song-and-dance to get the corresponding
// DIALOG object into the RC file. The wizard in its standard add-a-DIALOG processing (as far as I can figure out) 
// will NOT allow use of symbols from Resource.h in the template.
// 
// So...this wizard adds an empty rgs file to the .rc file, but in such a way that the following steps
// result in the desired dialog in the .rc file:
// 
// Since you are reading this .h file, you are far enough along that the wizard has added an empty
// rgs file to your project and to the REGISTRY section in the .rc, and it has added a .dlg file
// with the right DIALOG, but the .rc file does not reference the .dlg, while it does reference the
// useless .rgs.
// 
// Edit the .rc file AS TEXT.
// Track down this line:
// [!output IDD_ROVERSELECT_DIALOGID]    REGISTRY                "[!output ROVER_SELECT_RGSFILE]"
//
// Remove that line and replace it with this:
// #include "[!output ROVER_SELECT_DLGFILE]"
//
// Later, you may remove and delete both [!output ROVER_SELECT_DLGFILE] and  [!output ROVER_SELECT_RGSFILE]
// From the VC++ project, but NOT before you have brought up the Visual Studio editor on your .rc file
// and made some change, and then saved it. VS save of the .rc will conver the #include above into an in-line copy.
//

class [!output MM_ROVERDLG_CLASS_NAME] : 
	public CAxDialogImpl<[!output MM_ROVERDLG_CLASS_NAME]>
{
public:
	typedef std::function<void([!output MM_ROVERDLG_CLASS_NAME] &)> InitDialogFcn_t;
	[!output MM_ROVERDLG_CLASS_NAME](const InitDialogFcn_t &f) : m_FocusState(0)
		, m_OnInit(f)
	{}

	~[!output MM_ROVERDLG_CLASS_NAME]()
	{}

	std::string m_result;

	enum { IDD = [!output IDD_ROVERSELECT_DIALOGID] };

BEGIN_MSG_MAP([!output MM_ROVERDLG_CLASS_NAME])
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)

	COMMAND_HANDLER(IDC_CREATE_NEW, BN_CLICKED, OnClickedCreateNew)
	COMMAND_HANDLER(IDC_USE_EXISTING, BN_CLICKED, OnClickedUseExisting)

	COMMAND_HANDLER(IDC_QTHLIST1, LBN_DBLCLK, OnQthListDblClick)
	COMMAND_HANDLER(IDC_QTHLIST1, LBN_SETFOCUS, OnQthListSetFocus)
	COMMAND_HANDLER(IDC_QTHLIST1, LBN_KILLFOCUS, OnQthListKillFocus)

	COMMAND_HANDLER(IDC_QTH_OLD_LIST, LBN_DBLCLK, OnQthOldDblClick)
	COMMAND_HANDLER(IDC_QTH_OLD_LIST, LBN_SETFOCUS, OnQthOldSetFocus)
	COMMAND_HANDLER(IDC_QTH_OLD_LIST, LBN_KILLFOCUS, OnQthOldKillFocus)
	
	CHAIN_MSG_MAP(CAxDialogImpl<[!output MM_ROVERDLG_CLASS_NAME]>)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<[!output MM_ROVERDLG_CLASS_NAME]>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		if (m_OnInit)
			m_OnInit(*this);
		bHandled = TRUE;
		return 1;  // Let the system set the focus
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		switch (m_FocusState)
		{
		case 1:
			OnCreateNew();
			return 0;
		case 2:
			OnUseExisting();
			return 0;
		}
		MessageBeep(MB_ICONQUESTION);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(IDCANCEL);
		return 0;
	}

	LRESULT OnQthListDblClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		OnCreateNew();
		return 0;
	}
	LRESULT OnQthListSetFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_FocusState = 1;
		return 0;
	}
	LRESULT OnQthListKillFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_FocusState = 0;
		return 0;
	}

	LRESULT OnQthOldDblClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		OnUseExisting();
		return 0;
	}
	LRESULT OnQthOldSetFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_FocusState = 2;
		return 0;
	}
	LRESULT OnQthOldKillFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_FocusState = 0;
		return 0;
	}

	LRESULT OnClickedCreateNew(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		OnCreateNew();
		return 0;
	}

	LRESULT OnClickedUseExisting(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		OnUseExisting();
		return 0;
	}

	std::string GetListText(unsigned idc)
	{
		std::string ret;
		auto list = GetDlgItem(idc);
		auto cursel = list.SendMessageA(LB_GETCURSEL);
		if (cursel != LB_ERR)
		{
			auto nchars = list.SendMessageA(LB_GETTEXTLEN, cursel);
			if (nchars > 0)
			{
					std::vector<char> buf(nchars+1);
					list.SendMessageA(LB_GETTEXT, cursel, (LPARAM)&buf[0]);
					for (auto c : buf)
						if (!c)
							break;
						else
							ret.push_back(c);
			}
		}
		return ret;
	}

	void OnCreateNew()
	{
		m_result = GetListText(IDC_QTHLIST1);
		if (!m_result.empty())
				EndDialog(IDC_CREATE_NEW);
	}

	void OnUseExisting()
	{
		m_result = GetListText(IDC_QTH_OLD_LIST);
		if (!m_result.empty())
				EndDialog(IDC_USE_EXISTING);
	}

protected:
	int m_FocusState;
	InitDialogFcn_t m_OnInit;
};


