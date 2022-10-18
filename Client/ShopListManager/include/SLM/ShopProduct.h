/**************************************************************************************************

��ǰ(�Ӽ�) ��ü

���� ��ǰ�� �⺻ ������ �� ���� �Ӽ� ������ ������ �ִ�.
��ǰ ��ȣ�� ������ ���� �Ӽ��� �����Ͽ� �� ���� ��ǰ�� ǥ���ؾ� �Ѵ�.

**************************************************************************************************/

#pragma once
#include <SLM/CommonSLM.h>


class CShopProduct
{
public:
	CShopProduct();
	virtual ~CShopProduct();

	// version 3
	bool SetProductVer3(tstring& strData, const tstring& delimiter);
	bool SetProductVer3W(std::wstring& strData, const std::wstring& delimiter);

	// version 6
	bool SetProductVer6(tstring& strData, const tstring& delimiter);
	bool SetProductVer6W(std::wstring& strData, const std::wstring& delimiter);


public:	
	// version 3
	int		ProductSeq;											//  1. ��ǰ ��ȣ
	TCHAR	ProductName[SHOPLIST_LENGTH_PRODUCTNAME];			//  2. ��ǰ ��
	WCHAR	ProductNameW[SHOPLIST_LENGTH_PRODUCTNAME];			//  2. ��ǰ ��			(UseUnicodeString �� ��� ����)
	TCHAR	PropertyName[SHOPLIST_LENGTH_PRODUCTPROPERTYNAME];	//  3. �Ӽ� ��
	WCHAR	PropertyNameW[SHOPLIST_LENGTH_PRODUCTPROPERTYNAME];	//  3. �Ӽ� ��			(UseUnicodeString �� ��� ����)
	TCHAR	Value[SHOPLIST_LENGTH_PRODUCTVALUE];				//  4. �Ӽ� ��
	WCHAR	ValueW[SHOPLIST_LENGTH_PRODUCTVALUE];				//  4. �Ӽ� ��			(UseUnicodeString �� ��� ����)
	TCHAR	UnitName[SHOPLIST_LENGTH_PRODUCTUNITNAME];			//  5. �Ӽ� ���� ��
	WCHAR	UnitNameW[SHOPLIST_LENGTH_PRODUCTUNITNAME];			//  5. �Ӽ� ���� ��		(UseUnicodeString �� ��� ����)
	int		Price;												//  6. ��ǰ ����
	int		PriceSeq;											//  7. ��ǰ ���� ��ȣ
	int		PropertyType;										//  8. �Ӽ� ���� (141:������ �Ӽ�, 142:���� �Ӽ�)
	int		MustFlag;											//  9. �ʼ� ���� (145:�ʼ�, 146:����)
	int		vOrder;												// 10. ���� �Ӽ� ���� (1:���� �Ӽ�, 9:���� �Ӽ�)
	int		DeleteFlag;											// 11. ���� ���� (143: ����, 144: Ȱ��)
	int		StorageGroup;										// 12. ������ �׷� ����
	int		ShareFlag;											// 13. ServerType(���� ����) �� ������ ���� ���� �׸� ����
	TCHAR	InGamePackageID[SHOPLIST_LENGTH_INGAMEPACKAGEID];	// 14. ������ �ڵ�
	WCHAR	InGamePackageIDW[SHOPLIST_LENGTH_INGAMEPACKAGEID];	// 14. ������ �ڵ�		(UseUnicodeString �� ��� ����)
	int		PropertySeq;										// 15. �Ӽ� �ڵ�
	int		ProductType;										// 16. ��ǰ ���� �ڵ�
	int		UnitType;											// 17. ���� �ڵ�

	// version 6 : version 3�� �ߺ� ��� �ּ�ó��
	//int		ProductSeq;										//  1. ��ǰ ��ȣ
	//TCHAR	ProductName[SHOPLIST_LENGTH_PRODUCTNAME];			//  2. ��ǰ ��
	//int		ProductType;									//  3. ��ǰ ���� �ڵ�
	//int		DeleteFlag;										//  4. ���� ���� (143: ����, 144: Ȱ��)
	//int		StorageGroup;									//  5. ������ �׷� ����
	//int		ShareFlag;										//  6. ServerType(���� ����) �� ������ ���� ���� �׸� ����
	TCHAR	Description[SHOPLIST_LENGTH_PRODUCTDESCRIPTION];	//  7. ��ǰ ����
	WCHAR	DescriptionW[SHOPLIST_LENGTH_PRODUCTDESCRIPTION];	//  7. ��ǰ ����

};
