#pragma once
#include "CBaseHandler.h"
#include "CPropertyAdapter.h"
#include "../UIExtend/SSearchDropdownList.h"

class CPropertyHandler :public CBaseHandler<CPropertyHandler>
{
protected:
	void OnTVSelChanged(EventArgs* pEArg);
	void OnSearchFillList(EventArgs* e);
	void OnSearchValue(EventArgs* e);
public:
	virtual void Init()override;
	
protected:
	HANDLER_EVENT_MAP_BEGIN()
		EVENT_ID_HANDLER(R.id.et_search, EventDropdownListSelected::EventID, OnSearchValue)
		EVENT_ID_HANDLER(R.id.et_search, EventFillSearchDropdownList::EventID, OnSearchFillList)
		EVENT_ID_HANDLER(R.id.tv_property, EventTVSelChanged::EventID, OnTVSelChanged)
	EVENT_MAP_BREAK()

private:
	CAutoRefPtr<CPropertyAdapter>	m_pPropertyAdapter;
};

