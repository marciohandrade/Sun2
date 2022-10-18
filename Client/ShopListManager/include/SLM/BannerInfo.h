#pragma once
#include <SLM/CommonSLM.h>

class CBannerInfo
{
public:
	CBannerInfo();
	virtual ~CBannerInfo();

	bool	SetBanner(tstring& strData, tstring& strDirPath, bool bDonwLoad, tstring& delimiter);
	bool	SetBannerW(std::wstring& strData, std::wstring& strDirPath, bool bDonwLoad, std::wstring& delimiter);

public:
	int		BannerSeq;									//  1. ��� �׷� ����
	TCHAR	BannerName[BANNER_LENGTH_NAME];				//  2. ��� �׷� ��
	WCHAR	BannerNameW[BANNER_LENGTH_NAME];			//  2. ��� �׷� ��		(UseUnicodeString �� ��� ����)
	TCHAR	BannerImageURL[INTERNET_MAX_URL_LENGTH];	//  3. ��� �̹��� URL
	WCHAR	BannerImageURLW[INTERNET_MAX_URL_LENGTH];	//  3. ��� �̹��� URL	(UseUnicodeString �� ��� ����)
	int		BannerOrder;								//  4. ��� ���� ����
	int		BannerDirection;							//  5. ��� ���� ���� * ���� �ڵ� ���� ����
	tm 		BannerStartDate;							//  6. ��� ���� ������
	tm 		BannerEndDate;								//  7. ��� ���� ������
	TCHAR	BannerLinkURL[INTERNET_MAX_URL_LENGTH];		//  8. ��� ��ũ URL
	WCHAR	BannerLinkURLW[INTERNET_MAX_URL_LENGTH];	//  8. ��� ��ũ URL	(UseUnicodeString �� ��� ����)
	TCHAR	BannerImagePath[MAX_PATH];					// ��� �̹��� ���� ���� ���
	WCHAR	BannerImagePathW[MAX_PATH];					// ��� �̹��� ���� ���� ���
};

