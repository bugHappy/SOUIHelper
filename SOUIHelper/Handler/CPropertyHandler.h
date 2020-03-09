#pragma once
#include "CBaseHandler.h"
#include "CPropertyAdapter.h"
class CPropertyHandler :public CBaseHandler<CPropertyHandler>
{
	void OnTVSelChanged(EventArgs* pEArg);
public:
	virtual void Init()override;
protected:
	HANDLER_EVENT_MAP_BEGIN()
		EVENT_ID_HANDLER(R.id.tv_property, EventTVSelChanged::EventID, OnTVSelChanged)
	EVENT_MAP_BREAK()

private:
	CAutoRefPtr<CPropertyAdapter>	m_pPropertyAdapter;
};

