/**************************************************************************************************

상품(속성) 객체

현재 상품의 기본 정보와 한 가지 속성 정보를 가지고 있다.
상품 번호가 동일한 여러 속성을 조합하여 한 가지 상품을 표현해야 한다.

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
	int		ProductSeq;											//  1. 상품 번호
	TCHAR	ProductName[SHOPLIST_LENGTH_PRODUCTNAME];			//  2. 상품 명
	WCHAR	ProductNameW[SHOPLIST_LENGTH_PRODUCTNAME];			//  2. 상품 명			(UseUnicodeString 인 경우 사용됨)
	TCHAR	PropertyName[SHOPLIST_LENGTH_PRODUCTPROPERTYNAME];	//  3. 속성 명
	WCHAR	PropertyNameW[SHOPLIST_LENGTH_PRODUCTPROPERTYNAME];	//  3. 속성 명			(UseUnicodeString 인 경우 사용됨)
	TCHAR	Value[SHOPLIST_LENGTH_PRODUCTVALUE];				//  4. 속성 값
	WCHAR	ValueW[SHOPLIST_LENGTH_PRODUCTVALUE];				//  4. 속성 값			(UseUnicodeString 인 경우 사용됨)
	TCHAR	UnitName[SHOPLIST_LENGTH_PRODUCTUNITNAME];			//  5. 속성 단위 명
	WCHAR	UnitNameW[SHOPLIST_LENGTH_PRODUCTUNITNAME];			//  5. 속성 단위 명		(UseUnicodeString 인 경우 사용됨)
	int		Price;												//  6. 상품 가격
	int		PriceSeq;											//  7. 상품 가격 번호
	int		PropertyType;										//  8. 속성 유형 (141:아이템 속성, 142:가격 속성)
	int		MustFlag;											//  9. 필수 여부 (145:필수, 146:선택)
	int		vOrder;												// 10. 메인 속성 구분 (1:메인 속성, 9:서브 속성)
	int		DeleteFlag;											// 11. 삭제 여부 (143: 삭제, 144: 활성)
	int		StorageGroup;										// 12. 보관함 그룹 유형
	int		ShareFlag;											// 13. ServerType(서버 유형) 별 보관함 노출 공유 항목 여부
	TCHAR	InGamePackageID[SHOPLIST_LENGTH_INGAMEPACKAGEID];	// 14. 아이템 코드
	WCHAR	InGamePackageIDW[SHOPLIST_LENGTH_INGAMEPACKAGEID];	// 14. 아이템 코드		(UseUnicodeString 인 경우 사용됨)
	int		PropertySeq;										// 15. 속성 코드
	int		ProductType;										// 16. 상품 유형 코드
	int		UnitType;											// 17. 단위 코드

	// version 6 : version 3과 중복 멤버 주석처리
	//int		ProductSeq;										//  1. 상품 번호
	//TCHAR	ProductName[SHOPLIST_LENGTH_PRODUCTNAME];			//  2. 상품 명
	//int		ProductType;									//  3. 상품 유형 코드
	//int		DeleteFlag;										//  4. 삭제 여부 (143: 삭제, 144: 활성)
	//int		StorageGroup;									//  5. 보관함 그룹 유형
	//int		ShareFlag;										//  6. ServerType(서버 유형) 별 보관함 노출 공유 항목 여부
	TCHAR	Description[SHOPLIST_LENGTH_PRODUCTDESCRIPTION];	//  7. 상품 설명
	WCHAR	DescriptionW[SHOPLIST_LENGTH_PRODUCTDESCRIPTION];	//  7. 상품 설명

};
