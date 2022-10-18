#pragma once

#include <SLM/CommonSLM.h>
#include <SLM/BannerInfo.h>

class CBannerInfoList
{
public:
	CBannerInfoList(void);
	~CBannerInfoList(void);

	WZResult	LoadBanner(tstring& filepath, tstring& strDirPath, bool bDonwLoad, CListManagerInfo& listManagerInfo);
	WZResult	LoadBannerW(std::wstring& filepath, std::wstring& strDirPath, bool bDonwLoad, CListManagerInfo& listManagerInfo);

	void Clear();	
	int GetSize();	

	virtual void Append(CBannerInfo banner);

	void SetFirst();
	bool GetNext(CBannerInfo& banner);	

protected:	
	std::multimap<int, CBannerInfo> m_BannerInfos;	
	std::multimap<int, CBannerInfo>::iterator m_BannerInfoIter;

};
