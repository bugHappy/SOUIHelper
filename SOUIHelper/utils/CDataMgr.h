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
	SStringW	valueType;
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

	//��xml�ļ����ؿؼ���Ϣ
	bool LoadCtrlInfoFormXml(LPCWSTR xmlpath = NULL)
	{
		const WCHAR* ctrlxml = LR"(\soui�����б�.xml)";

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
					//<Attribute name = "trCtx" default = "" type = "STRING" comment = "�����Է���ʱ��Ϊcontextʹ�� xml����" / >
					prolist.push_back({ attrib.attribute(L"name").value(), attrib.attribute(L"comment").value(),attrib.attribute(L"type").value() });
				}
			}
			
			return true;
		}
		return false;
	}
	
};

