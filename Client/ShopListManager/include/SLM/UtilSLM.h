#pragma once
#include <SLM/CommonSLM.h>

class CUtilSLM
{
public:
	CUtilSLM(void);
	~CUtilSLM(void);
private:
	CUtilSLM(CUtilSLM& cls);
	CUtilSLM& operator=(CUtilSLM& cls);

public:

	std::string		TrimString(std::string& str, const std::string& drop = std::string("\r\n"));
	tstring			TrimTString(tstring& str, const tstring& drop = tstring(_T("\r\n")));

	std::string		TString2String(tstring& str);
	std::wstring	TString2WString(const tstring& str);
	tstring			WString2TString(std::wstring& str);
};
