#include "stdafx.h"
#include "CSQLite3.h"

int initsqlite3= sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);

CSQLite3::operator sqlite3* ()
{
	return m_sqlfile;
}

CSQLite3::CSQLite3()
{	
}

CSQLite3::~CSQLite3()
{
	Close();
}

void CSQLite3::Close()
{
	if (m_sqlfile)
	{
		sqlite3_close_v2(m_sqlfile);
		m_sqlfile = nullptr;
	}
}

bool CSQLite3::BeginTransaction(char** errmsg)
{
	return Exec("begin transaction;", nullptr, nullptr, errmsg);
}

bool CSQLite3::CommitTransaction(char** errmsg)
{
	return Exec("commit transaction;", nullptr, nullptr, errmsg);
}

bool CSQLite3::Open(LPCWSTR filepath, int flags)
{
	Close();
	SStringA u8filepath = S_CW2A(filepath, CP_UTF8);
	return SQLITE_OK== sqlite3_open_v2(u8filepath, &m_sqlfile, flags,nullptr);
}

bool CSQLite3::Exec(const char* sql, sqlite_callback cbfun, void* ptr, char** errmsg)
{
	if(m_sqlfile)
		return SQLITE_OK == sqlite3_exec(m_sqlfile,sql, cbfun,ptr,errmsg);
	return false;
}

