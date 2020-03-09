#pragma once
#include <helper/SAdapterBase.h>
#include "..\utils\CDataMgr.h"
enum class PropertyNodeType
{
	CLASS, PROPERTY
};

struct PropertyNodeInfo {
	PropertyNodeType type;
	SStringT name;
};


class CPropertyAdapter :public STreeAdapterBase<PropertyNodeInfo>
{
public:

	CPropertyAdapter()
	{
	}

	void Init()
	{
		const CDataMgr* datamgr = CDataMgr::GetInstance();
		const std::map<SStringW, CtrlInfo>& ctrlData = datamgr->GetCtrlInfo();
		for (auto& ite : ctrlData)
		{
			PropertyNodeInfo nodeinfo;
			nodeinfo.type = PropertyNodeType::CLASS;
			nodeinfo.name = ite.first;
			SOUI::HTREEITEM hRoot = InsertItem(nodeinfo);
			SetItemExpanded(hRoot, TRUE);
			for (auto& ite2 : ite.second.m_prolist)
			{
				nodeinfo.type = PropertyNodeType::PROPERTY;
				nodeinfo.name = ite2.name;
				InsertItem(nodeinfo, hRoot);
			}
		}
	}

	int GetItemCount()const
	{
		return m_tree.GetChildrenCount(ITEM_ROOT);
	}

	const ItemInfo& GetData(HSTREEITEM item)
	{
		return m_tree.GetItemRef(item);
	}

	const ItemInfo& GetParentData(HSTREEITEM item)
	{
		return m_tree.GetItemRef(m_tree.GetParentItem(item));
	}

	HSTREEITEM GetFirstTreeItem()
	{
		HSTREEITEM hFistItem = m_tree.GetChildItem(ITEM_ROOT);
		if (hFistItem)
			return m_tree.GetChildItem(hFistItem);
		return NULL;
	}

	virtual void getView(SOUI::HTREEITEM loc, SWindow* pItem, pugi::xml_node xmlTemplate)
	{
		ItemInfo& ii = m_tree.GetItemRef((HSTREEITEM)loc);
		int itemType = getViewType(loc);
		if (pItem->GetChildrenCount() == 0)
		{
			switch (itemType)
			{
				case 0:xmlTemplate = xmlTemplate.child(L"item_class");
					break;
				case 1:xmlTemplate = xmlTemplate.child(L"item_fun");
					break;
			}
			pItem->InitFromXml(xmlTemplate);
		}
		SWindow* pName = pItem->FindChildByID(R.id.lable_name);
		if (itemType == 0)
		{
		}
		pName->SetWindowText(ii.data.name);
	}

	bool OnGroupItemPanelClick(EventArgs* pEvt)
	{
		SItemPanel* pItemPanel = sobj_cast<SItemPanel>(pEvt->sender);
		SASSERT(pItemPanel);
		SOUI::HTREEITEM hItem = (SOUI::HTREEITEM)pItemPanel->GetItemIndex();
		BOOL bExpanded = IsItemExpanded(hItem);
		SetItemExpanded(hItem, !bExpanded);
		notifyBranchExpandChanged(hItem, bExpanded, !bExpanded);
		pEvt->bubbleUp = false;
		return true;
	}

	virtual int getViewType(SOUI::HTREEITEM hItem) const
	{
		ItemInfo& ii = m_tree.GetItemRef((HSTREEITEM)hItem);
		int ret = 0;
		switch (ii.data.type)
		{
			case PropertyNodeType::CLASS:ret = 0; break;
			case PropertyNodeType::PROPERTY:ret = 1; break;
		}
		return ret;
	}

	virtual int getViewTypeCount() const
	{
		return 2;
	}
protected:

private:
};