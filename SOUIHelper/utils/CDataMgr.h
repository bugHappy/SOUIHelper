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
public:
	const std::map<SStringW, CtrlInfo>& GetCtrlInfo()const
	{
		return m_ctrls;
	}
	//从xml文件加载控件信息
	bool LoadCtrlInfoFormXml(LPCWSTR xmlpath = NULL)
	{
		const WCHAR* ctrlxml = LR"(\ctrls.dat)";
		const WCHAR* ctrlpro = LR"(\ctrlspro.dat)";
		const WCHAR* proinfoxml = LR"(\provalue.dat)";
		SStringW ctrlXmlPath = ctrlxml, ctrlProXmlPath = ctrlpro, proinfoXmlPath = proinfoxml;
		if (xmlpath)
		{
			ctrlXmlPath = xmlpath;
			ctrlXmlPath += ctrlxml;
			ctrlProXmlPath = xmlpath;
			ctrlProXmlPath += ctrlpro;
			proinfoXmlPath = xmlpath;
			proinfoXmlPath += proinfoxml;
		}
		pugi::xml_document ctrldoc, prodoc, proinfodoc;
		pugi::xml_parse_result ret = ctrldoc.load_file(ctrlXmlPath);
		if (ret)
		{
			pugi::xml_node ctrlsnode = ctrldoc.child(L"root").child(L"ctrls");

			for (auto& ctrlnode : ctrlsnode)
			{
				//<SOUI des = "SOUI真窗口" parent="***"/ >
				m_ctrls[ctrlnode.name()]=(CtrlInfo(ctrlnode.attribute(L"des").value(), ctrlnode.attribute(L"parent").value()));
			}
			ret = prodoc.load_file(ctrlProXmlPath);
			if (ret)
			{
				pugi::xml_node ctrlsproperty = prodoc.child(L"root").child(L"ctrls_property");
				for (auto& ite : m_ctrls)
				{
					pugi::xml_node ctrlpronode= ctrlsproperty.child(ite.first);
					if (ctrlpronode)
					{
						for (auto& pro : ctrlpronode)
						{
							// <cueColor des="无内容显示的提示文字的颜色&#10;string&#10;无&#10;请在这里输入您的姓名" />
							ite.second.m_prolist.push_back({ pro.name(), pro.attribute(L"des").value() });
						}
					}
				}
			}
			ret = proinfodoc.load_file(proinfoXmlPath);
			if (ret)
			{
				//proinfodoc 本意是添加参数提示
			}
			return true;
		}
		return false;
	}
	
};

