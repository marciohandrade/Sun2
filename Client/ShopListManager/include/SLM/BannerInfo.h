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
	int		BannerSeq;									//  1. 배너 그룹 순번
	TCHAR	BannerName[BANNER_LENGTH_NAME];				//  2. 배너 그룹 명
	WCHAR	BannerNameW[BANNER_LENGTH_NAME];			//  2. 배너 그룹 명		(UseUnicodeString 인 경우 사용됨)
	TCHAR	BannerImageURL[INTERNET_MAX_URL_LENGTH];	//  3. 배너 이미지 URL
	WCHAR	BannerImageURLW[INTERNET_MAX_URL_LENGTH];	//  3. 배너 이미지 URL	(UseUnicodeString 인 경우 사용됨)
	int		BannerOrder;								//  4. 배너 노출 순서
	int		BannerDirection;							//  5. 배너 노출 방향 * 공통 코드 정의 참고
	tm 		BannerStartDate;							//  6. 배너 노출 시작일
	tm 		BannerEndDate;								//  7. 배너 노출 종료일
	TCHAR	BannerLinkURL[INTERNET_MAX_URL_LENGTH];		//  8. 배너 링크 URL
	WCHAR	BannerLinkURLW[INTERNET_MAX_URL_LENGTH];	//  8. 배너 링크 URL	(UseUnicodeString 인 경우 사용됨)
	TCHAR	BannerImagePath[MAX_PATH];					// 배너 이미지 파일 로컬 경로
	WCHAR	BannerImagePathW[MAX_PATH];					// 배너 이미지 파일 로컬 경로
};

