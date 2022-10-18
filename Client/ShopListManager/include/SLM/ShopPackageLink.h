/**************************************************************************************************

패키지 링크 객체

카테고리에 속해있는 패키지를 알려주는 객체이다.
하나의 패키지는 여러 카테고리에 속해 있을 수 있다.

**************************************************************************************************/

#pragma once
#include <SLM/CommonSLM.h>

class CShopPackageLink
{
public:
	CShopPackageLink();
	virtual ~CShopPackageLink();

	bool SetPackageLinkVer6(tstring& strData, const tstring& delimiter);
	bool SetPackageLinkVer6W(std::wstring& strData, const std::wstring& delimiter);

	int ProductDisplaySeq;							// 1. 카테고리 번호
	int ViewOrder;									// 2. 노출 순서
	int PackageProductSeq;							// 3. 패키지 번호
};

// 백터 소팅하기 위해 비교 연산
class CompareViewOrder
{
public:
	BOOL operator()(CShopPackageLink x, CShopPackageLink y) const
	{
		return x.ViewOrder < y.ViewOrder;
	}	
};
