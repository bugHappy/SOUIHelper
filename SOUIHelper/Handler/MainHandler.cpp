#include "stdafx.h"
#include "MainHandler.h"

CMainHandler::CMainHandler()
{
}


CMainHandler::~CMainHandler()
{
	

}

void CMainHandler::Init()
{	
	SASSERT(m_pRoot);
	m_pTabBinder.BindHost(m_pRoot->FindChildByID2<STabCtrl>(R.id.tab_main_nav));
	m_pTabBinder.Bind(m_pRoot->FindChildByID(R.id.btn_nav_api), 0);
	m_pTabBinder.Bind(m_pRoot->FindChildByID(R.id.btn_nav_attrib), 1);

	m_PropertyHandler.OnInit(m_pRoot->FindChildByID2<STabCtrl>(R.id.tab_main_nav)->GetPage(L"property"));
	m_ApisHandler.OnInit(m_pRoot->FindChildByID2<STabCtrl>(R.id.tab_main_nav)->GetPage(L"api"));
	
	OnShowNavFull();
}

void CMainHandler::OnShowNavFull()
{
	SWindow* pNavWnd = m_pRoot->FindChildByID(R.id.nav_wnd);
	SASSERT(pNavWnd);
	SWindow *pNavWnd1 = m_pRoot->FindChildByID(8);
	SASSERT(pNavWnd1);
	SWindow *pNavWnd2 = m_pRoot->FindChildByID(9);
	SASSERT(pNavWnd2);
	//userdata==1 ÏÔÊ¾µÄ¼òÂÔ°æµ¼º½
	if (pNavWnd->GetUserData() == 1)
	{
		pNavWnd->SetUserData(2);
		pNavWnd->SetAttribute(L"size", L"160,-2");
		UpDataNav(pNavWnd1->FindChildByID(19), true);
		UpDataNav(pNavWnd2, true);
	}
	else
	{
		pNavWnd->SetUserData(1);
		pNavWnd->SetAttribute(L"size", L"48,-2");
		UpDataNav(pNavWnd1->FindChildByID(19), false);
		UpDataNav(pNavWnd2, false);
	}
}

void CMainHandler::UpDataNav(SWindow *pNav, bool bshow) 
{
	SASSERT(pNav);
	SWindow *pChild= pNav->GetWindow(GSW_FIRSTCHILD);
	while (pChild)
	{
		SWindow * pTitleWnd=pChild->FindChildByID(10);
		if (pTitleWnd)
		{
			pTitleWnd->SetVisible(bshow, FALSE);

		}
		pChild = pChild->GetWindow(GSW_NEXTSIBLING);
	}
	pNav->Invalidate();
}
