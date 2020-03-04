#pragma once
#include <..\third-part\sqlite3\sqlite3.h>

#ifdef  _DEBUG
#pragma comment(lib,"sqlite3d.lib")
#else
#pragma comment(lib,"sqlite3.lib")
#endif //  _DEBUG


class CSQliteErrMsg
{
public:
	~CSQliteErrMsg()
	{
		if (m_errmsg)
			sqlite3_free(m_errmsg);
	}
	operator char** ()
	{
		if (m_errmsg)
		{
			sqlite3_free(m_errmsg);
			m_errmsg = nullptr;
		}
		return &m_errmsg;
	}
	SStringW GetErrorMsg()
	{
		if (m_errmsg)
		{
			SStringW outmsg;
			outmsg = S_CA2W(m_errmsg);
			return outmsg;
		}
		return SStringW();
	}
	operator LPCSTR()
	{
		return m_errmsg;
	}
	CSQliteErrMsg& operator =(LPSTR msg)
	{
		if (m_errmsg)
			sqlite3_free(m_errmsg);
		m_errmsg = msg;
		return *this;
	}
private:
	LPSTR m_errmsg = nullptr;
};

class CSQLite3
{
	typedef int (*sqlite_callback)(void*, int, char**, char**);

public:
	operator sqlite3* ();
	CSQLite3();
	~CSQLite3();
	bool Open(LPCWSTR filepath, int flags);
	bool Exec(const char* sql, sqlite_callback, void*, char** errmsg);
	void Close();
	bool BeginTransaction(char** errmsg);
	bool CommitTransaction(char** errmsg);
private:
	sqlite3* m_sqlfile = nullptr;
};

class CDoxygenFileName {
public:
	static SStringW makeXmlFileName(LPCWSTR classname)
	{
		if (classname)
		{
			SStringW strXml;

			const WCHAR* p = classname;
			while (*p != L'\0')
			{
				if (iswupper(*p))
				{
					strXml += L"_";
					strXml += towlower(*p);
				}
				else
				{
					strXml += *p;
				}
				p++;
			}
			strXml = L"class" + strXml;
			strXml += L".xml";
			return strXml;
		}
		return L"";
	}
};

class CSOUIDBHelper
{
public:
	//����һ����ʹ���ݿ�
	static bool init(LPCWSTR dbpath, LPCWSTR xmlpath)
	{
		CSQLite3 sqldb;
		if (!sqldb.Open(dbpath, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE))
			return false;
		const WCHAR* index = LR"(index.xml)";
		pugi::xml_document doc;
		pugi::xml_parse_result ret = doc.load_file(SStringW(xmlpath) + LR"(\)" + index);
		if (!ret)
			return false;

		const char* CreateTable = R"(CREATE TABLE IF NOT EXISTS 'Class'('name' varchar(256),'description' varchar(256),'valid' INTEGER))";
		//����һ����Class�ı��¼���е���
		CSQliteErrMsg msg;
		sqldb.Exec(CreateTable, nullptr, nullptr, msg);

		

		if (sqldb.BeginTransaction(msg))
		{
			for (auto& node : doc.first_child())
			{
				SStringW strKind = node.attribute(L"kind").value();
				if (/*(strKind == L"struct") ||*/ (strKind == L"class"))
				{
					//��������Ϣ
					const char* strinsert = R"(INSERT INTO 'Class' VALUES ('%s','%s','%d'))";
					SStringA strSQL;
					SStringW nodetext = node.child(L"name").text().as_string();
					strSQL.Format(strinsert, S_CW2A(nodetext, CP_UTF8),u8"�����˵��",TRUE);
					sqldb.Exec(strSQL, nullptr, nullptr, nullptr);
					//������-����-����(������ʽ ���ƣ����Σ�˵��)
					const char* CreateClassTable = R"(CREATE TABLE IF NOT EXISTS '%s'('name' varchar(256),'description' varchar(2048),'vardes' varchar(2048)))";					
					strSQL.Format(CreateClassTable, S_CW2A(nodetext, CP_UTF8));

					//����һ������ͬ���ı��¼������Ϣ
					sqldb.Exec(strSQL, nullptr, nullptr, nullptr);
					//�����ຯ����Ϣ
					/*for (auto& funnode : node)
					{
						const SStringT strmember = L"member";
						const SStringT strfunction = L"function";						
						if ((strmember ==funnode.name())&&(strfunction== funnode.attribute(L"kind").value()))
						{
							const char* strinsertfun = R"(INSERT INTO '%s' VALUES ('%s','%s','%s'))";
							strSQL.Format(\
								strinsertfun, S_CW2A(nodetext, CP_UTF8),\
								S_CW2A(funnode.child(L"name").text().as_string(), CP_UTF8),\
								u8"�����˵��",\
								u8"�����˵��");
							sqldb.Exec(strSQL, nullptr, nullptr, nullptr);
						}
					}*/
					//ͨ����������ϸxml
					//SStringT xmlpath=CDoxygenFileName::makeXmlFileName(nodetext);
					SStringW strRefid = node.attribute(L"refid").value();					
					if (strRefid.IsEmpty())
						continue;
					strRefid += L".xml";

					pugi::xml_document classdoc;
					ret = classdoc.load_file(SStringW(xmlpath) + LR"(\)" + strRefid);
					if (ret)
					{
						for (auto& nodeiter : classdoc.first_child().first_child())
						{
							const SStringT sectiondef = L"sectiondef";
							const SStringT public_func = L"public-func";

							SStringT nodename = nodeiter.name();
							SStringT kind = nodeiter.attribute(L"kind").value();
							if ((sectiondef == nodeiter.name()) && (public_func == nodeiter.attribute(L"kind").value()))
							{
								for (auto& public_funcnode : nodeiter)
								{
									const char* strinsertfun = R"(INSERT INTO '%s' VALUES ('%s','%s','%s'))";
									strSQL.Format(\
										strinsertfun, S_CW2A(nodetext, CP_UTF8), \
										S_CW2A(public_funcnode.child(L"definition").text().as_string(), CP_UTF8), \
										u8"�����˵��", \
										u8"�����˵��");
									sqldb.Exec(strSQL, nullptr, nullptr, nullptr);
								}
								//ֻ��һ��public_func�Ľ��
								break;
							}
						}
					}
				}
			}
			sqldb.CommitTransaction(msg);
		}
		return true;
	}
};

//
#define CreateDB(xmlpath) CSOUIDBHelper::init(L"SOUIDB.db3",xmlpath);