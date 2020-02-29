#pragma once
namespace SOUI
{
	class STabCtrlHeaderBinder
	{
	public:
		STabCtrlHeaderBinder();
		STabCtrlHeaderBinder(STabCtrl *phost);
		~STabCtrlHeaderBinder();
		void BindHost(STabCtrl*);
		void Bind(SWindow *pWnd, int idx, DWORD evtcode = EVT_CMD);
	private:
		STabCtrl *m_pTabHost;
	protected:
		SMap<SWindow*, int> m_lstPages;
		bool EvtHander(EventArgs *e);
		bool TabSelChanged(EventTabSelChanged *evt_sel);
	};
}