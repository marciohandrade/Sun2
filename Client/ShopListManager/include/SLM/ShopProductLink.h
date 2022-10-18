
/*
	��ǰ ��ũ ��ü

	�ϳ��� ��ǰ ��ũ ������ ������ �ִ�.
	��ǰ�� ���� �ϳ��� �Ӽ��� �� �Ӽ��� Value�� ������ ��ũ �� ��ü �̴�.

	��ǰ ��ȣ�� ������ ���� ��ũ ��ü�� �����Ͽ� �� ���� ��ǰ�� ǥ���ؾ� �Ѵ�.
*/
#pragma once
#include <SLM/CommonSLM.h>


class CShopProductLink
{
public:
	CShopProductLink();
	virtual ~CShopProductLink();

	bool SetProductLinkVer6(tstring& strData, const tstring& delimiter);
	bool SetProductLinkVer6W(std::wstring& strData, const std::wstring& delimiter);

	int		ProductSeq;								// 1. ��ǰ ����
	TCHAR	Value[SHOPLIST_LENGTH_PROPERTYVALUE];	// 2. �Ӽ� ��
	WCHAR	ValueW[SHOPLIST_LENGTH_PROPERTYVALUE];	// 2. �Ӽ� ��
	int 	Price;									// 3. ���� ��ǰ ����
	int 	PriceSeq;								// 4. ���� ��ǰ ���� ����
	int 	vOrder;									// 5. ���� �Ӽ� ���� (1: ����, 9: ����)
	int 	InGameProductID;						// 6. ������ �ڵ� (���°�� 0)
	int 	PropertySeq;							// 7. �Ӽ� �ڵ�
	int		PropertyType;							// 8. �Ӽ� ���� (141 : ������ �Ӽ�, 142 : ���� �Ӽ�)
};
