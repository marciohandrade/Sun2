#pragma once
#include <SLM/CommonSLM.h>

class CStringMethod
{
public:
	CStringMethod();
	virtual ~CStringMethod();

	static void ConvertStringToDateTime(tm& datetime, tstring strdata);
	static void ConvertStringToDateTimeW(tm& datetime, std::wstring strdata);
};
