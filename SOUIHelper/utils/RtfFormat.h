#pragma once

class CRtfForamt
{
public:	
	static SStringA Format2Rtf(const char* RtfFormatStr, size_t FormatStrLen=0);	
	static void SetRitchEditRtf(SRichEdit* pRe, const char* RtfBuf, size_t bufsize);
	static void SetRitchEditRtf(SRichEdit* pRe, const SStringA& rtf);
};