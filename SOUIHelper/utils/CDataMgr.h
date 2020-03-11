#pragma once
#include <set>
#include <vector>
#include <stdexcept>
#include <map>
//一个单列
template <typename T>
class Singleton
{
public:
	template<typename... Args>
	static T* InitInstance(Args&&... args)
	{
		if (m_pInstance == nullptr)
			m_pInstance = new T(std::forward<Args>(args)...);
		return m_pInstance;
	}
	static T* GetInstance()
	{
		if (m_pInstance == nullptr)
			throw std::logic_error("the instance is not init, please call the InitInstance first");
		return m_pInstance;
	}
	static void DestroyInstance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

protected:
	Singleton(void) {};
	virtual ~Singleton(void) {};
	Singleton(const Singleton&) =delete;
	Singleton& operator = (const Singleton&) = delete;
private:
	static T* m_pInstance;
};

template <class T> T* Singleton<T>::m_pInstance = nullptr;

struct PropertyInfo
{
	SStringW    name;//属性名
	SStringW    des;//属性说明
	SStringW	valueType;
	bool operator ==(SStringW _name)const
	{
		return name == _name;
	}
};
//控件信息
struct CtrlInfo
{
	std::set<SStringW>          m_parent;//父控件
	SStringW					m_des;//控件属性
	std::vector<PropertyInfo>   m_prolist;//私有属性列表（不包括父控件的属性）
	CtrlInfo() {}
	CtrlInfo(LPCWSTR des, LPCWSTR parent)
	{
		m_des = des;
		if (parent)
		{
			SStringWList parentlist;
			int i= SplitString(SStringW(parent), L',', parentlist);
			for (int m = 0; m < i; m++)
			{
				m_parent.insert(parentlist[m]);
			}
		}
	}
};
//数据管理对象
class CDataMgr :public Singleton<CDataMgr>
{
private:
	std::map<SStringW,CtrlInfo> m_ctrls;
	SStringW m_verinfo;
public:
	const std::map<SStringW, CtrlInfo>& GetCtrlInfo()const
	{
		return m_ctrls;
	}
	const SStringW& GetProDataVerInfo()
	{
		return m_verinfo;
	}

	//从xml文件加载控件信息
	bool LoadCtrlInfoFormXml(LPCWSTR xmlpath = NULL)
	{
		const WCHAR* ctrlxml = LR"(\soui属性列表.xml)";

		SStringW ctrlXmlPath = ctrlxml;
		if (xmlpath)
		{
			ctrlXmlPath = xmlpath;
			ctrlXmlPath += ctrlxml;
		}
		pugi::xml_document ctrldoc, prodoc, proinfodoc;
		pugi::xml_parse_result ret = ctrldoc.load_file(ctrlXmlPath);
		if (ret)
		{
			pugi::xml_node ctrlsnode = ctrldoc.first_child();

			m_verinfo.Format(GETSTRING(L"@string/dataverformat"),ctrlsnode.attribute(L"author").value()
				, ctrlsnode.attribute(L"ver").value()
				, ctrlsnode.attribute(L"date").value()
			);

			for (auto& ctrlnode : ctrlsnode)
			{				
				m_ctrls[ctrlnode.name()]=(CtrlInfo(ctrlnode.attribute(L"comment").value(), ctrlnode.attribute(L"parent").value()));
				auto &prolist = m_ctrls[ctrlnode.name()].m_prolist;
				for (auto& attrib : ctrlnode)
				{
					//<Attribute name = "trCtx" default = "" type = "STRING" comment = "在语言翻译时作为context使用 xml名称" / >
					prolist.push_back({ attrib.attribute(L"name").value(), attrib.attribute(L"comment").value(),attrib.attribute(L"type").value() });
				}
			}
			
			return true;
		}
		return false;
	}
	
};

