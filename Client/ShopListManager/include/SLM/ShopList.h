/**************************************************************************************************

��ũ��Ʈ ��� �� ���� ��ü

ī�װ� ���, ��Ű�� ���, ��ǰ(�Ӽ�) ����� ������ �ִ�.

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


	CShopCategoryList*		GetCategoryListPtr() {return m_CategoryListPtr.get();};			// ī�װ� ��� �����´�.
	CShopPackageList*		GetPackageListPtr()  {return m_PackageListPtr.get();};			// ��Ű�� ��� �����´�.
	CShopProductList*		GetProductListPtr()  {return m_ProductListPtr.get();};			// ��ǰ(�Ӽ�) ��� �����´�.

	CShopPropertyList*		GetPropertyListPtr()	{return m_PropertyListPtr.get();};		// �Ӽ� ��� �����´�.
	CShopProductLinkList*	GetProductLinkListPtr() {return m_ProductListLinkPtr.get();};	// ��ǰ ��ũ ��� �����´�.


private:	
	std::auto_ptr<CShopCategoryList>	m_CategoryListPtr;
	std::auto_ptr<CShopPackageList>		m_PackageListPtr;
	std::auto_ptr<CShopProductList>		m_ProductListPtr;

	std::auto_ptr<CShopPropertyList>	m_PropertyListPtr;
	std::auto_ptr<CShopProductLinkList>	m_ProductListLinkPtr;
};