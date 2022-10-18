
/**************************************************************************************************

ī�װ� ��ü

���� ī�װ� ������ ������ �ִ�.
���� ī�װ� ������ "ī�װ� ��ȣ"�� ������� ������ �ִ�.
���� ī�װ��� �� ���� ī�װ���� "��Ű�� ��ȣ" ����� ������ �ִ�.

**************************************************************************************************/

#pragma once
#include <SLM/CommonSLM.h>


class CShopCategory
{
public:
	CShopCategory();
	virtual ~CShopCategory();
	
	// version 3
	bool SetCategoryVer3(tstring& strData, const tstring& delimiter);
	bool SetCategoryVer3W(std::wstring& strData, const std::wstring& delimiter);

	void SetCategoryFirst();							// ���� ī�װ� ����� ù ��° �׷��� ����Ű���� �����Ѵ�.
	bool GetCategoryNext(int& CategorySeq);				// ���� ī�װ� ��ȣ�� �����ϰ� ���� ���� ī�װ� ��ȣ�� ����Ų��.

	void SetPackagSeqFirst();							// ī�װ��� ��ϵǾ� �ִ� ��Ű�� ����� ù ��° �׸��� ����Ű���� �����Ѵ�.
	bool GetPackagSeqNext(int& PackagSeq);				// ��Ű�� ��ȣ�� �����ϰ� ���� ��Ű�� ��ȣ�� ����Ų��.

	void AddPackageSeq(int PackageSeq);
	void ClearPackageSeq();


	// version 6
	bool SetCategoryVer6(tstring& strData, const tstring& delimiter);
	bool SetCategoryVer6W(std::wstring& strData, const std::wstring& delimiter);

	void ClearChildCategorySeqList();
	void ClearChildPackageSeqList();
	void ClearAllChildPackageSeqList();

	void AddChildCategorySeqList(int CategorySeq);
	void SetChildCategorySeqFirst();					// ���� ī�װ� ����� ù ��° �׷��� ����Ű���� �����Ѵ�.
	bool GetChildCategorySeqNext(int& CategorySeq);		// ���� ī�װ� ��ȣ�� �����ϰ� ���� ���� ī�װ� ��ȣ�� ����Ų��.

	void AddChildPackageSeqList(int PackageSeq);
	void SetChildPackagSeqFirst();						// ī�װ��� ��ϵǾ� �ִ� ��Ű�� ����� ù ��° �׸��� ����Ű���� �����Ѵ�.
	bool GetChildPackagSeqNext(int& PackageSeq);		// ��Ű�� ��ȣ�� �����ϰ� ���� ��Ű�� ��ȣ�� ����Ų��.
	bool ExistChildPackageSeq(int PackageSeq);			// PackageSeq�� �ִ��� Ȯ��

	void AddAllChildPackageSeqList(int PackageSeq);
	void SetAllChildPackageSeqFirst();					// ī�װ� �� ��� ���� ī�װ��� ��ϵǾ� �ִ� ��Ű�� ����� ù ��° �׸��� ����Ű���� �����Ѵ�.
	bool GetAllChildPackageSeqNext(int& PackageSeq);	// ��Ű�� ��ȣ�� �����ϰ� ���� ��Ű�� ��ȣ�� ����Ų��.
	std::vector<int>& GetAllChildPackageSeq();

public:
	int		ProductDisplaySeq;								// 1. ī�װ� ��ȣ
	TCHAR	CategroyName[SHOPLIST_LENGTH_CATEGORYNAME];		// 2. ī�װ� �̸�
	WCHAR	CategroyNameW[SHOPLIST_LENGTH_CATEGORYNAME];	// 2. ī�װ� �̸�		(UseUnicodeString �� ��� ����)
	int		EventFlag;										// 3. �̺�Ʈ ī�װ� ���� (199:�̺�Ʈ, 200:�Ϲ�)	
	int		OpenFlag;										// 4. ���� ���� (201:����, 202: �����)
	int		ParentProductDisplaySeq;						// 5. �θ� ī�װ� ��ȣ
	int		DisplayOrder;									// 6. ���� ����
	int		Root;											// 7. �ֻ��� ī�װ� ���� (1: �ֻ���, 0: ����)

	// version 3
	std::vector<int> CategoryList;						// ���� ī�װ��� ���� "ī�װ� ��ȣ" ���
	std::vector<int>::iterator Categoryiter;

	std::vector<int> PackageList;						// ���� ī�װ��� ���Ե� "��Ű�� ��ȣ" ���
	std::vector<int>::iterator Packageiter;

	// version 6
	std::vector<int> ChildCategorySeqList;				// ���� ī�װ��� ���� "ī�װ� ��ȣ" ���
	std::vector<int>::iterator ChildCategorySeqIter;

	std::vector<int> ChildPackageSeqList;				// ���� ī�װ��� ���Ե� "��Ű�� ��ȣ" ���
	std::vector<int>::iterator ChildPackageSeqIter;

	std::vector<int> AllChildPackageSeqList;			// ���� ī�װ��� ���Ե� "��Ű�� ��ȣ" ��� �� ��� ���� ī�װ��� ���� �ִ� "��Ű�� ��ȣ" ���
	std::vector<int>::iterator AllChildPackageSeqIter;

};
