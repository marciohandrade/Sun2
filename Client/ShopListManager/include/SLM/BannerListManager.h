
/*
2009.10.06, ÁøÇıÁø
*/

#pragma once
#include <SLM/CommonSLM.h>
#include <SLM/ListManager.h>

class CBannerInfoList;

class CBannerListManager : public CListManager
{
public:
	CBannerListManager();	
	virtual ~CBannerListManager();

	CBannerInfoList*	GetListPtr() {return m_BannerInfoList.get();};

private:
	std::auto_ptr<CBannerInfoList> m_BannerInfoList;

	void				RegistScriptFiles();
	WZResult			LoadScript(bool bDonwLoad);
};
