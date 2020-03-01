#pragma once
#include <helper/SAdapterBase.h>

enum class NodeType
{
	CLASS, FUN
};

struct NodeInfo {
	NodeType type;
	SStringT name;
};

class CAPIAdapter :public STreeAdapterBase<NodeInfo>
{
public:

	CAPIAdapter()
	{
		for (int m = 1; m < 6; m++)
		{
			NodeInfo item;
			item.type = NodeType::CLASS;
			item.name.Format(L"CLASS%d", m);
			SOUI::HTREEITEM hRoot = InsertItem(item);
			SetItemExpanded(hRoot, TRUE);
			item.type = NodeType::FUN;

			for (int i = 1; i < 6; i++)
			{
				item.name.Format(L"FUN%d", i);
				InsertItem(item, hRoot);
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


	HSTREEITEM GetFirstTreeItem()
	{
		HSTREEITEM hFistItem = m_tree.GetChildItem(ITEM_ROOT);
		if (hFistItem)
			return m_tree.GetChildItem(hFistItem);
		return NULL;
	}

	~CAPIAdapter() {}

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

			SWindow* pName = pItem->FindChildByID(R.id.lable_name);

			if (itemType == 0)
			{

			}

			pName->SetWindowText(ii.data.name);
		}
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
			case NodeType::CLASS:ret = 0; break;
			case NodeType::FUN:ret = 1; break;
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