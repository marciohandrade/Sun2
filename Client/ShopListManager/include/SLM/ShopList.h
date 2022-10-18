/**************************************************************************************************

스크립트 목록 최 상위 객체

카테고리 목록, 패키지 목록, 상품(속성) 목록을 가지고 있다.

**************************************************************************************************/

#pragma once
#include <SLM/CommonSLM.h>


class CShopCategoryList;
class CShopPackageList;
class CShopProductList;

class CShopPropertyList;
class CShopProductLinkList;

class CShopList  
{
public:
	CShopList();
	virtual ~CShopList();

	WZResult LoadCategroy		(tstring& filepath, CListManagerInfo& listManagerInfo);
	WZResult LoadPackage		(tstring& filepath, CListManagerInfo& listManagerInfo);
	WZResult LoadProduct		(tstring& filepath, CListManagerInfo& listManagerInfo);

	WZResult LoadPackageLink	(tstring& filepath, CListManagerInfo& listManagerInfo);
	WZResult LoadProperty		(tstring& filepath, CListManagerInfo& listManagerInfo);
	WZResult LoadProductLink	(tstring& filepath, CListManagerInfo& listManagerInfo);


	WZResult LoadCategroyW		(std::wstring& filepath, CListManagerInfo& listManagerInfo);
	WZResult LoadPackageW		(std::wstring& filepath, CListManagerInfo& listManagerInfo);
	WZResult LoadProductW		(std::wstring& filepath, CListManagerInfo& listManagerInfo);

	WZResult LoadPackageLinkW	(std::wstring& filepath, CListManagerInfo& listManagerInfo);
	WZResult LoadPropertyW		(std::wstring& filepath, CListManagerInfo& listManagerInfo);
	WZResult LoadProductLinkW	(std::wstring& filepath, CListManagerInfo& listManagerInfo);


	CShopCategoryList*		GetCategoryListPtr() {return m_CategoryListPtr.get();};			// 카테고리 목록 가져온다.
	CShopPackageList*		GetPackageListPtr()  {return m_PackageListPtr.get();};			// 패키지 목록 가져온다.
	CShopProductList*		GetProductListPtr()  {return m_ProductListPtr.get();};			// 상품(속성) 목록 가져온다.

	CShopPropertyList*		GetPropertyListPtr()	{return m_PropertyListPtr.get();};		// 속성 목록 가져온다.
	CShopProductLinkList*	GetProductLinkListPtr() {return m_ProductListLinkPtr.get();};	// 상품 링크 목록 가져온다.


private:	
	std::auto_ptr<CShopCategoryList>	m_CategoryListPtr;
	std::auto_ptr<CShopPackageList>		m_PackageListPtr;
	std::auto_ptr<CShopProductList>		m_ProductListPtr;

	std::auto_ptr<CShopPropertyList>	m_PropertyListPtr;
	std::auto_ptr<CShopProductLinkList>	m_ProductListLinkPtr;
};