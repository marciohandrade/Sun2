/*
	�Ӽ� ��ü
	�ϳ��� �Ӽ� ������ ������ �ִ�.
*/

#pragma once
#include <SLM/CommonSLM.h>

class CShopProperty
{
public:
	CShopProperty();
	virtual ~CShopProperty();

	bool SetPropertyVer6(tstring& strData, const tstring& delimiter);
	bool SetPropertyVer6W(std::wstring& strData, const std::wstring& delimiter);

public:	
	int		PropertySeq;										//  1. �Ӽ� ��ȣ
	TCHAR	PropertyName[SHOPLIST_LENGTH_PROPERTYNAME];			//  2. �Ӽ� ��
	WCHAR	PropertyNameW[SHOPLIST_LENGTH_PROPERTYNAME];		//  2. �Ӽ� ��
	int		PropertyType;										//  3. �Ӽ� ���� (141:������ �Ӽ�, 142:���� �Ӽ�)
	int		MustFlag;											//  4. �ʼ� ���� (145:�ʼ�, 146:����)
	int		UnitType;											//  5. ���� �ڵ�
	TCHAR	UnitName[SHOPLIST_LENGTH_PROPERTYUNITNAME];			//  6. �Ӽ� ���� ��
	WCHAR	UnitNameW[SHOPLIST_LENGTH_PROPERTYUNITNAME];		//  6. �Ӽ� ���� ��
};
