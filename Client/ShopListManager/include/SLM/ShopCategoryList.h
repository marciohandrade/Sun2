
/**************************************************************************************************

��ü ī�װ� ��� ��ü

iterator�� �̿��Ͽ� ���������� ī�װ� ��ü�� ������ �� �ִ�.
ī�װ� ��ȣ�� �̿��Ͽ� ī�װ� ��ü�� ������ �� �ִ�.

**************************************************************************************************/

#pragma once
#include <SLM/CommonSLM.h>

#include <SLM/ShopCategory.h>
#include <SLM/ShopPackageLink.h>


class CShopCategoryList
{
public:
	CShopCategoryList(void);
	~CShopCategoryList(void);

	// version 3
	void			Clear();
	virtual void	AppendVer3(CShopCategory& category);
	bool			InsertPackageVer3(int Category, int Package);
	bool			RefreshPackageSeq(int Category, int PackageSeqs[], int PackageCount);


	// version 3, 6
	int				GetSize();												// ī�װ� ����
	void			SetFirst();												// ī�װ� ��Ͽ��� ù ��° ī�װ��� ����Ű�� �Ѵ�.

	bool			GetNext(CShopCategory& category);						// ���� ī�װ� ��ü�� �ѱ�� ���� ī�װ� ��ü�� ����Ű�� �Ѵ�.
	bool			GetValueByKey(int nKey, CShopCategory& category);		// ī�װ� �������� ī�װ� ��ü�� �����´�.
	bool			GetValueByIndex(int nIndex, CShopCategory& category);	// �ε��� ��ȣ�� ī�װ� ��ü�� �����´�. 

	bool			GetNext(CShopCategory*& category);						// overload : ������ ���
	bool			GetValueByKey(int nKey, CShopCategory*& category);		// overload : ������ ���
	bool			GetValueByIndex(int nIndex, CShopCategory*& category);	// overload : ������ ���


	// version 6
	void			ClearCategory();
	void			ClearPackageLink();
	void			AppendPackageLinkVer6(CShopPackageLink& link);				// ��Ű�� ��ũ �߰�
	void			AppendVer6(CShopCategory& category);
	void			SortPackageLinkList();


	// version 6 : �߰� ���
	void			SetRootPackageSeq();						// root ī�װ��� ���� ī�װ��� ��� PackageSeq�� ����Ѵ�.
	void			SetAllChildPackageSeq();

private:
	std::map<int, CShopCategory>::iterator	FindRootCategory(const std::map<int, CShopCategory>::iterator& it);	// root ī�װ��� iterator�� ã���ִ� ��� �Լ�
	void FindAllChildPackageSeq(int ProductDisplaySeq, std::vector<int>& vec);

protected:
	// version 3, 6	
	std::map<int, CShopCategory> m_Categorys;				// ī�װ� ��ü ��
	std::map<int, CShopCategory>::iterator m_Categoryiter;	// ī�װ� iterator
	std::vector<int> m_CategoryIndex;						// ī�װ� ��ȣ �ε��� ���

	// version 6
	std::vector<CShopPackageLink> m_PackageLinks;			// ��Ű�� ��ũ ���
};
