#include "stdafx.h"
#include "CApiHandler.h"

void CApiHandler::Init() {
	STreeView* pTreeView = m_pRoot->FindChildByID2<STreeView>(R.id.tv_allapis);
	if (pTreeView)
	{
		m_pAPIAdapter.Attach(new CAPIAdapter());
		pTreeView->SetAdapter(m_pAPIAdapter);
	}
}
