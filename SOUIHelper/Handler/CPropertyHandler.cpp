#include "stdafx.h"
#include "CPropertyHandler.h"
#include "..\utils\RtfFormat.h"

void CPropertyHandler::OnTVSelChanged(EventArgs* pEArg)
{
	EventTVSelChanged* pE2 = sobj_cast<EventTVSelChanged>(pEArg);
	SASSERT(pE2);
	const CPropertyAdapter::ItemInfo& data = m_pPropertyAdapter->GetData(pE2->hNewSel);
	const char* ProRtfFormatString = R"({\rtf1\ansi\ansicpg936\deff0\nouicompat\deflang1033\deflangfe2052{\fonttbl{\f0\fnil\fcharset134 \'cb\'ce\'cc\'e5;}}
{\*\generator Riched20 10.0.18362}\viewkind4\uc1 
\pard\sa200\sl276\slmult1\b\f0\fs40\lang2052\'bf\'d8\'bc\'fe\'a3\'ba%s\par
\'ca\'f4\'d0\'d4\'a3\'ba%s\par
\b0\fs36 %s\par
\b\fs40\'b2\'ce\'ca\'fd\par
\i\fs36 %s\tab\b0\i0 %s\b\i\par
\b0\i0\fs22\par
})";
	const char* CtrlRtfFormatString = R"({\rtf1\ansi\ansicpg936\deff0\nouicompat\deflang1033\deflangfe2052{\fonttbl{\f0\fnil\fcharset134 \'cb\'ce\'cc\'e5;}}
{\*\generator Riched20 10.0.18362}\viewkind4\uc1 
\pard\sa200\sl276\slmult1\b\f0\fs40\lang2052\'bf\'d8\'bc\'fe\'a3\'ba%s\par
\b0\fs36 %s\b\i\par
\b0\i0\fs22\par
})";
	static const CDataMgr* datamgr = CDataMgr::GetInstance();
	const std::map<SStringW, CtrlInfo>& ctrlData = datamgr->GetCtrlInfo();

	SRichEdit* pre_prodetail = m_pRoot->FindChildByID2<SRichEdit>(R.id.re_prodetail);
	switch (data.data.type)
	{
		case PropertyNodeType::CLASS:
			{
				auto ctrlinfo = ctrlData.find(data.data.name);
				if (ctrlinfo != ctrlData.end())
				{
					SStringA ctrlName = S_CW2A(ctrlinfo->first);
					SStringA ctrlDes = S_CW2A(ctrlinfo->second.m_des);
					ctrlName = CRtfForamt::Format2Rtf(ctrlName);
					ctrlDes = CRtfForamt::Format2Rtf(ctrlDes);
					SStringA RtfString;
					RtfString.Format(CtrlRtfFormatString, ctrlName.c_str(), ctrlDes.c_str());
					CRtfForamt::SetRitchEditRtf(pre_prodetail, RtfString);
				}
			}break;

		case PropertyNodeType::PROPERTY:
			{
				const CPropertyAdapter::ItemInfo& ParentData = m_pPropertyAdapter->GetParentData(pE2->hNewSel);

				auto ctrlinfo = ctrlData.find(ParentData.data.name);
				if (ctrlinfo != ctrlData.end())
				{
					SStringA ctrlName = S_CW2A(ctrlinfo->first);
					ctrlName = CRtfForamt::Format2Rtf(ctrlName);

					SStringA proName = S_CW2A(data.data.name);
					proName = CRtfForamt::Format2Rtf(proName);

					auto proinfo = std::find(ctrlinfo->second.m_prolist.begin(), ctrlinfo->second.m_prolist.end(), data.data.name);
					SStringA proDes;
					if (proinfo != ctrlinfo->second.m_prolist.end())
					{
						proDes = S_CW2A(proinfo->des);
						proDes = CRtfForamt::Format2Rtf(proDes);
					}

					SStringA proVar;
					SStringA proVarDes;

					SStringA RtfString;
					RtfString.Format(ProRtfFormatString, ctrlName.c_str(), proName.c_str(), proDes.c_str(), proVar.c_str(), proVarDes.c_str());
					CRtfForamt::SetRitchEditRtf(pre_prodetail, RtfString);
				}
			}break;
	}


}

void CPropertyHandler::Init()
{
	STreeView* pTreeView = m_pRoot->FindChildByID2<STreeView>(R.id.tv_property);
	if (pTreeView)
	{
		m_pPropertyAdapter.Attach(new CPropertyAdapter());
		m_pPropertyAdapter->Init();
		pTreeView->SetAdapter(m_pPropertyAdapter);
	}
	SRichEdit* pre_prodetail = m_pRoot->FindChildByID2<SRichEdit>(R.id.re_prodetail);
	HWND hWnd = m_pRoot->GetContainer()->GetHostHwnd();
	HDC hdc = GetDC(hWnd);
	pre_prodetail->SSendMessage(EM_SETTARGETDEVICE,(WPARAM)hdc);
	ReleaseDC(hWnd, hdc);
}
