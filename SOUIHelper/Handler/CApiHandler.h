#pragma once
#include "CBaseHandler.h"
#include "CAPIAdapter.h"

class CApiHandler:public CBaseHandler<CApiHandler>
{
public:
	virtual void Init()override;
protected:

	HANDLER_EVENT_MAP_BEGIN()

	EVENT_MAP_BREAK()
private:
	CAutoRefPtr<CAPIAdapter>	m_pAPIAdapter;
};

