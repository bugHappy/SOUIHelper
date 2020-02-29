#pragma once
#include "../UIExtend/STabCtrlHeaderBinder.h"
#include "CAPIAdapter.h"

class CMainHandler
{
	friend class CMainDlg;
	SOUI::SWindow *m_pRoot;
public:
	CMainHandler();
	~CMainHandler();
	void OnInit(SOUI::SWindow *pRoot);
	
protected:
	void OnShowNavFull();
	void UpDataNav(SWindow* pNav, bool bshow);
	EVENT_MAP_BEGIN()
		//EVENT_CHECK_SENDER_ROOT(m_pRoot)
		EVENT_NAME_COMMAND(L"btn_show_nav_full", OnShowNavFull)
	EVENT_MAP_BREAK()
private:
	STabCtrlHeaderBinder		m_pTabBinder;
	CAutoRefPtr<CAPIAdapter>	m_pTreeViewAdapter;
};