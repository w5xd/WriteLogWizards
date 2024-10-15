// Declaration of the [!output MM_ROVERDLG_CLASS_NAME]
#pragma once

#include "resource.h"       // main symbols
#include <atlhost.h>
#include <functional>

// [!output MM_ROVERDLG_CLASS_NAME]

class [!output MM_ROVERDLG_CLASS_NAME] : 
	public CAxDialogImpl<[!output MM_ROVERDLG_CLASS_NAME]>
{
public:
	typedef std::function<void([!output MM_ROVERDLG_CLASS_NAME] &)> InitDialogFcn_t;
	[!output MM_ROVERDLG_CLASS_NAME] (const InitDialogFcn_t& f) : m_FocusState(0)
		, m_OnInit(f)
[!if AM_COUNTYLINE]
		, m_countyLineMode(0)
[!endif]
	{}

	~[!output MM_ROVERDLG_CLASS_NAME]()
	{}

	std::string m_result;
[!if AM_COUNTYLINE]
	int m_countyLineMode;
[!endif]

	enum { IDD = [!output IDD_ROVERSELECT_DIALOGID] };
	enum { FOCUS_STATE_NONE, FOCUS_STATE_ADD, FOCUS_STATE_EXISTING };

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
[!if AM_COUNTYLINE]

	COMMAND_HANDLER(IDC_CHECK_COUNTYLINEMODE, BN_CLICKED, OnClickedCountyLineMode)
[!endif]

	CHAIN_MSG_MAP(CAxDialogImpl<[!output MM_ROVERDLG_CLASS_NAME]>)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
	{
		CAxDialogImpl<[!output MM_ROVERDLG_CLASS_NAME]>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		if (m_OnInit)
			m_OnInit(*this);
[!if AM_COUNTYLINE]
		CheckDlgButton(IDC_CHECK_COUNTYLINEMODE, m_countyLineMode);
[!endif]
		bHandled = TRUE;
		return 1;  // Let the system set the focus
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
	{
		switch (m_FocusState)
		{
		case static_cast<int>(FOCUS_STATE_ADD):
			OnCreateNew();
			return 0;
		case static_cast<int>(FOCUS_STATE_EXISTING):
			OnUseExisting();
			return 0;
		}
		MessageBeep(MB_ICONQUESTION);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
	{
		EndDialog(IDCANCEL);
		return 0;
	}

	LRESULT OnQthListDblClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
	{
		OnCreateNew();
		return 0;
	}
	LRESULT OnQthListSetFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
	{
		m_FocusState = FOCUS_STATE_ADD;
		return 0;
	}
	LRESULT OnQthListKillFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
	{
		m_FocusState = FOCUS_STATE_NONE;
		return 0;
	}

	LRESULT OnQthOldDblClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
	{
		OnUseExisting();
		return 0;
	}
	LRESULT OnQthOldSetFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
	{
		m_FocusState = FOCUS_STATE_EXISTING;
		return 0;
	}
	LRESULT OnQthOldKillFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
	{
		m_FocusState = FOCUS_STATE_NONE;
		return 0;
	}

	LRESULT OnClickedCreateNew(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
	{
		OnCreateNew();
		return 0;
	}

	LRESULT OnClickedUseExisting(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
	{
		OnUseExisting();
		return 0;
	}
[!if AM_COUNTYLINE]

	LRESULT OnClickedCountyLineMode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
	{
		m_countyLineMode = IsDlgButtonChecked(wID);
		return 0;
	}
[!endif]

	void getListItem(ATL::CWindow list, int item, std::string & ret, const std::function<void()>&f = std::function<void()>())
	{
		auto nchars = list.SendMessageA(LB_GETTEXTLEN, item);
		if (nchars > 0)
		{
			if (f)
				f();
			std::vector<char> buf(nchars + 1);
			list.SendMessageA(LB_GETTEXT, item, (LPARAM)&buf[0]);
			for (auto c : buf)
				if (!c)
					break;
				else
					ret.push_back(c);
		}
	}

	std::string GetListText(unsigned idc)
	{
		std::string ret;
		auto list = GetDlgItem(idc);
		auto cursel = list.SendMessageA(LB_GETCURSEL);
		if (cursel != LB_ERR)
			getListItem(list, cursel, ret);
		return ret;
	}

[!if MULTIPLE_NAMED_IN_QSO_TX]
	std::string GetListTextM(unsigned idc)
	{
		std::string ret;
		auto list = GetDlgItem(idc);
		static const int MAX_SELECTIONS = 4;
		auto selcount = list.SendMessageA(LB_GETSELCOUNT);
		if ((selcount > 0) && (selcount <= MAX_SELECTIONS))
		{
			auto f = std::function<void()>();
			std::vector<int> selections(selcount);
			list.SendMessageA(LB_GETSELITEMS, selcount, reinterpret_cast<LPARAM>(&selections[0]));
			for (auto idc : selections)
			{
				getListItem(list, idc, ret, f);
				f = [&ret]() {
					ret.push_back('/'); };
			}
		}
		return ret;
	}
[!else]
	std::string GetListTextM(unsigned idc) {return GetListText(idc);}
[!endif]

	void OnCreateNew()
	{
		m_result = GetListTextM(IDC_QTHLIST1);
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


