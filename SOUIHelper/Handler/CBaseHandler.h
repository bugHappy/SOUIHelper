#pragma once

#define HANDLER_EVENT_MAP_BEGIN()\
EVENT_MAP_BEGIN()\
 EVENT_CHECK_SENDER_ROOT(m_pRoot)

template<typename T>
class CBaseHandler
{
protected:
	SWindow* m_pRoot;
public:
	virtual void Init()=NULL;
	void OnInit(SWindow* pRoot)
	{
		SASSERT(pRoot);
		m_pRoot = pRoot;
		dynamic_cast<T*>(this)->Init();
	}

	virtual ~CBaseHandler() {};
};