#pragma once
#include <set>
#include <vector>
#include <stdexcept>
#include <map>
//һ������
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
	SStringW    name;//������
	SStringW    des;//����˵��

	bool operator ==(SStringW _name)const
	{
		return name == _name;
	}
};
//�ؼ���Ϣ
struct CtrlInfo
{
	std::set<SStringW>          m_parent;//���ؼ�
	SStringW					m_des;//�ؼ�����
	std::vector<PropertyInfo>   m_prolist;//˽�������б����������ؼ������ԣ�
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
//���ݹ������
class CDataMgr :public Singleton<CDataMgr>
{
private:
	std::map<SStringW,CtrlInfo> m_ctrls;
public:
	const std::map<SStringW, CtrlInfo>& GetCtrlInfo()const
	{
		return m_ctrls;
	}
	//��xml�ļ����ؿؼ���Ϣ
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
				//<SOUI des = "SOUI�洰��" parent="***"/ >
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
							// <cueColor des="��������ʾ����ʾ���ֵ���ɫ&#10;string&#10;��&#10;��������������������" />
							ite.second.m_prolist.push_back({ pro.name(), pro.attribute(L"des").value() });
						}
					}
				}
			}
			ret = proinfodoc.load_file(proinfoXmlPath);
			if (ret)
			{
				//proinfodoc ��������Ӳ�����ʾ
			}
			return true;
		}
		return false;
	}
	
};

