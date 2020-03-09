#include "stdafx.h"
#include "RtfFormat.h"
#include <helper/SAutoBuf.h>
#include <winscard.h>

struct MemBlock {
	LPCBYTE  pBuf;
	LONG     nRemains;
};
DWORD CALLBACK EditStreamInCallback_MemBlock(
	DWORD_PTR dwCookie,
	LPBYTE pbBuff,
	LONG cb,
	LONG* pcb
)
{
	MemBlock* pmb = (MemBlock*)dwCookie;
	if (pmb->nRemains >= cb)
	{
		memcpy(pbBuff, pmb->pBuf, cb);
		pmb->pBuf += cb;
		pmb->nRemains -= cb;
		if (pcb) *pcb = cb;
		return 0;
	}
	else
	{
		memcpy(pbBuff, pmb->pBuf, pmb->nRemains);
		pmb->pBuf += pmb->nRemains;
		if (pcb) *pcb = pmb->nRemains;
		pmb->nRemains = 0;
		return 0;
	}
}

bool char2rtfchar(const char* srcChar, char** rftChar)
{
	if (srcChar == NULL || strlen(srcChar) == 0)
	{
		*rftChar = new char[1];
		ZeroMemory(*rftChar, 1);
		return false;
	}
	size_t bufsize = strlen(srcChar) * 5 + 1;
	*rftChar = new char[bufsize];
	int pos = 0;
	for (int i = 0; i < strlen(srcChar); i++)
	{
		if (srcChar[i] < 0)
		{
			sprintf_s(*rftChar + pos, bufsize - pos, R"(\'%x)", BYTE(srcChar[i++]));
			pos += 4;
			sprintf_s(*rftChar + pos, bufsize - pos, R"(\'%x)", BYTE(srcChar[i]));
			pos += 4;
		}
		else
		{
			switch (srcChar[i])
			{
				case '\t':
					memcpy(*rftChar + pos, R"(\tab )", 5);
					pos += 5;
					break;
				case '\n':
					memcpy(*rftChar + pos, R"(\par )", 5);
					pos += 5;
					break;
				case '\r':
					memcpy(*rftChar + pos, R"(\tab )", 5);
					pos += 5;
					break;
				default:
					memcpy(*rftChar + pos, srcChar + i, 1);
					++pos;
					break;
			}
		}
	}
	ZeroMemory(*rftChar + pos, 1);
	return true;
}

SStringA CRtfForamt::Format2Rtf(const char *RtfFormatStr,size_t FormatStrLen)
{
	size_t dwSize = FormatStrLen;
	if((dwSize==0)&& RtfFormatStr)
		dwSize = strlen(RtfFormatStr);
	SStringA retString;
	if (dwSize)
	{
		char* rtfStr = NULL;
		char2rtfchar(RtfFormatStr, &rtfStr);
		retString = rtfStr;
	}
	return retString;
}

void CRtfForamt::SetRitchEditRtf(SRichEdit* pRe,const char* RtfBuf,size_t bufsize)
{
	EDITSTREAM es;
	MemBlock mb = { NULL,0 };
	SAutoBuf mybuf;

	mb.pBuf =(LPCBYTE) mybuf.Allocate(bufsize);
	mb.nRemains = bufsize;
	memcpy(mybuf, RtfBuf, bufsize);

	es.dwCookie = (DWORD_PTR)&mb;
	es.pfnCallback = EditStreamInCallback_MemBlock;
	pRe->SSendMessage(EM_STREAMIN, SF_RTF, (LPARAM)&es);
}

void CRtfForamt::SetRitchEditRtf(SRichEdit* pRe, const SStringA& rtf)
{
	SetRitchEditRtf(pRe, rtf.c_str(),(size_t)(rtf.GetLength()));
}