#pragma once
#include "../UIExtend/STabCtrlHeaderBinder.h"

#include "CBaseHandler.h"
#include "CPropertyHandler.h"
#include "CApiHandler.h"

class CMainHandler:public CBaseHandler<CMainHandler>
{
	friend class CMainDlg;

public:
	CMainHandler();
	~CMainHandler();
	virtual void Init()override;
	
protected:
	void OnShowNavFull();
	void UpDataNav(SWindow* pNav, bool bshow);
	EVENT_MAP_BEGIN()
		//EVENT_CHECK_SENDER_ROOT(m_pRoot)
		EVENT_NAME_COMMAND(L"btn_show_nav_full", OnShowNavFull)
		CHAIN_EVENT_MAP_MEMBER(m_PropertyHandler)
		CHAIN_EVENT_MAP_MEMBER(m_ApisHandler)
		//CHAIN_EVENT_MAP_MEMBER(m_mainHandler)
	EVENT_MAP_BREAK()
private:
	STabCtrlHeaderBinder		m_pTabBinder;

	CPropertyHandler			m_PropertyHandler;
	CApiHandler					m_ApisHandler;
};