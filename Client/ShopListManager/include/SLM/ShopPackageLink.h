/**************************************************************************************************

��Ű�� ��ũ ��ü

ī�װ��� �����ִ� ��Ű���� �˷��ִ� ��ü�̴�.
�ϳ��� ��Ű���� ���� ī�װ��� ���� ���� �� �ִ�.

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

	int ProductDisplaySeq;							// 1. ī�װ� ��ȣ
	int ViewOrder;									// 2. ���� ����
	int PackageProductSeq;							// 3. ��Ű�� ��ȣ
};

// ���� �����ϱ� ���� �� ����
class CompareViewOrder
{
public:
	BOOL operator()(CShopPackageLink x, CShopPackageLink y) const
	{
		return x.ViewOrder < y.ViewOrder;
	}	
};
