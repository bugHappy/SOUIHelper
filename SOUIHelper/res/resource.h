//stamp:092da5f4f33a9e54
/*<------------------------------------------------------------------------------------------------->*/
/*该文件由uiresbuilder生成，请不要手动修改*/
/*<------------------------------------------------------------------------------------------------->*/
#pragma once
#include <res.mgr/snamedvalue.h>
#define ROBJ_IN_CPP \
namespace SOUI \
{\
    const _R R;\
    const _UIRES UIRES;\
}
namespace SOUI
{
	class _UIRES{
		public:
		class _UIDEF{
			public:
			_UIDEF(){
				XML_INIT = _T("UIDEF:XML_INIT");
			}
			const TCHAR * XML_INIT;
		}UIDEF;
		class _LAYOUT{
			public:
			_LAYOUT(){
				XML_MAINWND = _T("LAYOUT:XML_MAINWND");
			}
			const TCHAR * XML_MAINWND;
		}LAYOUT;
		class _values{
			public:
			_values(){
				string = _T("values:string");
				color = _T("values:color");
				skin = _T("values:skin");
				template_cpp = _T("values:template");
			}
			const TCHAR * string;
			const TCHAR * color;
			const TCHAR * skin;
			const TCHAR * template_cpp;
		}values;
		class _IMG{
			public:
			_IMG(){
				traymenu_icons = _T("IMG:traymenu_icons");
				icon_api = _T("IMG:icon_api");
				icon_showfull = _T("IMG:icon_showfull");
				png_search = _T("IMG:png_search");
				png_search_box = _T("IMG:png_search_box");
				png_search_bk = _T("IMG:png_search_bk");
				png_about = _T("IMG:png_about");
				png_attrib = _T("IMG:png_attrib");
			}
			const TCHAR * traymenu_icons;
			const TCHAR * icon_api;
			const TCHAR * icon_showfull;
			const TCHAR * png_search;
			const TCHAR * png_search_box;
			const TCHAR * png_search_bk;
			const TCHAR * png_about;
			const TCHAR * png_attrib;
		}IMG;
		class _ICON{
			public:
			_ICON(){
				ICON_LOGO = _T("ICON:ICON_LOGO");
			}
			const TCHAR * ICON_LOGO;
		}ICON;
		class _SMENU{
			public:
			_SMENU(){
				menu_tray = _T("SMENU:menu_tray");
			}
			const TCHAR * menu_tray;
		}SMENU;
	};
	const SNamedID::NAMEDVALUE namedXmlID[]={
		{L"_name_start",65535},
		{L"bkwnd",65545},
		{L"btn_about",65540},
		{L"btn_close",65547},
		{L"btn_max",65548},
		{L"btn_min",65550},
		{L"btn_nav_api",65538},
		{L"btn_nav_attrib",65539},
		{L"btn_restore",65549},
		{L"btn_show_nav_full",65537},
		{L"et_search",65542},
		{L"lable_name",65544},
		{L"nav_dev_cmd",65546},
		{L"nav_wnd",65536},
		{L"tab_main_nav",65541},
		{L"tv_allapis",65543}		};
	class _R{
	public:
		class _name{
		public:
		_name(){
			_name_start = namedXmlID[0].strName;
			bkwnd = namedXmlID[1].strName;
			btn_about = namedXmlID[2].strName;
			btn_close = namedXmlID[3].strName;
			btn_max = namedXmlID[4].strName;
			btn_min = namedXmlID[5].strName;
			btn_nav_api = namedXmlID[6].strName;
			btn_nav_attrib = namedXmlID[7].strName;
			btn_restore = namedXmlID[8].strName;
			btn_show_nav_full = namedXmlID[9].strName;
			et_search = namedXmlID[10].strName;
			lable_name = namedXmlID[11].strName;
			nav_dev_cmd = namedXmlID[12].strName;
			nav_wnd = namedXmlID[13].strName;
			tab_main_nav = namedXmlID[14].strName;
			tv_allapis = namedXmlID[15].strName;
		}
		 const wchar_t * _name_start;
		 const wchar_t * bkwnd;
		 const wchar_t * btn_about;
		 const wchar_t * btn_close;
		 const wchar_t * btn_max;
		 const wchar_t * btn_min;
		 const wchar_t * btn_nav_api;
		 const wchar_t * btn_nav_attrib;
		 const wchar_t * btn_restore;
		 const wchar_t * btn_show_nav_full;
		 const wchar_t * et_search;
		 const wchar_t * lable_name;
		 const wchar_t * nav_dev_cmd;
		 const wchar_t * nav_wnd;
		 const wchar_t * tab_main_nav;
		 const wchar_t * tv_allapis;
		}name;

		class _id{
		public:
		const static int _name_start	=	65535;
		const static int bkwnd	=	65545;
		const static int btn_about	=	65540;
		const static int btn_close	=	65547;
		const static int btn_max	=	65548;
		const static int btn_min	=	65550;
		const static int btn_nav_api	=	65538;
		const static int btn_nav_attrib	=	65539;
		const static int btn_restore	=	65549;
		const static int btn_show_nav_full	=	65537;
		const static int et_search	=	65542;
		const static int lable_name	=	65544;
		const static int nav_dev_cmd	=	65546;
		const static int nav_wnd	=	65536;
		const static int tab_main_nav	=	65541;
		const static int tv_allapis	=	65543;
		}id;

		class _string{
		public:
		const static int title	=	0;
		const static int ver	=	1;
		}string;

		class _color{
		public:
		const static int blue	=	0;
		const static int gray	=	1;
		const static int gray2	=	2;
		const static int gray3	=	3;
		const static int gray4	=	4;
		const static int green	=	5;
		const static int red	=	6;
		const static int white	=	7;
		}color;

	};

#ifdef R_IN_CPP
	 extern const _R R;
	 extern const _UIRES UIRES;
#else
	 extern const __declspec(selectany) _R & R = _R();
	 extern const __declspec(selectany) _UIRES & UIRES = _UIRES();
#endif//R_IN_CPP
}
