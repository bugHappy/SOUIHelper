#pragma once
#include <helper/SAdapterBase.h>
#include "..\utils\CDataMgr.h"
#include <algorithm>

enum class PropertyNodeType
{
	CLASS, PROPERTY
};

struct PropertyNodeInfo {
	PropertyNodeType type;
	SStringT name;
};

class SSearchAdapter : public SAdapterBase
{
	friend class CPropertyAdapter;

public:
	struct SearchInfo
	{
		SStringT strText;
		HSTREEITEM itemindex;
	};

	SearchInfo GetItem(int i) { return m_searchResult[i]; }
protected:
	virtual int getCount()
	{
		return m_searchResult.size();
	}

	virtual void getView(int position, SWindow* pItem, pugi::xml_node xmlTemplate)
	{
		if (pItem->GetChildrenCount() == 0)
		{
			pItem->InitFromXml(xmlTemplate);
		}
		pItem->FindChildByID(R.id.txt_key)->SetWindowText(m_searchResult[position].strText);
	}

protected:
	std::vector<SearchInfo> m_searchResult;
};
class CPropertyAdapter :public STreeAdapterBase<PropertyNodeInfo>
{
public:

	CPropertyAdapter()
	{
	}

	struct {
		SStringW strSearchKey;
		bool operator() (SSearchAdapter::SearchInfo first, SSearchAdapter::SearchInfo sec) 
		{ 
			int i = sec.strText.Find(strSearchKey);
			int j = first.strText.Find(strSearchKey);
			return (i < j);
		}
	} mycmp;
	int Search(const SStringT& strKey, SSearchAdapter* pSearchAdapter)
	{
		if (strKey.IsEmpty()) return 0;

		HSTREEITEM hNext = m_tree.GetNextItem(STVI_ROOT);
		while (hNext)
		{
			const auto data= m_tree.GetItemPt(hNext)->data;
			if (data.name.Find(strKey) != -1)
			{
				SStringW showText = data.name;
				switch (data.type)
				{
					case PropertyNodeType::CLASS:
						{
							showText += L"(¿Ø¼þ)";
						}break;
					case PropertyNodeType::PROPERTY:
						{
							const CPropertyAdapter::ItemInfo& ParentData = GetParentData(hNext);
							showText += L"(";
							showText += ParentData.data.name;
							showText += L")";
						}break;
				}
				SSearchAdapter::SearchInfo searchRet{ showText ,hNext };
				pSearchAdapter->m_searchResult.push_back(searchRet);
			}
			hNext = m_tree.GetNextItem(hNext);
		}
		mycmp.strSearchKey = strKey;
		std::sort(pSearchAdapter->m_searchResult.begin(), pSearchAdapter->m_searchResult.end(), mycmp);
		return pSearchAdapter->m_searchResult.size();
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