
/**************************************************************************************************

��ü ��Ű�� ��� ��ü

iterator�� �̿��Ͽ� ���������� ��Ű�� ��ü�� ������ �� �ִ�.
��Ű�� ��ȣ�� �̿��Ͽ� ��Ű�� ��ü�� ������ �� �ִ�.

**************************************************************************************************/

#pragma once
#include <SLM/CommonSLM.h>
#include <SLM/ShopPackage.h>

class CShopPackageList
{
public:
	CShopPackageList(void);
	~CShopPackageList(void);

	// version 3
	void	AppendVer3(CShopPackage& package);


	// version 3, 6
	int		GetSize();
	void	Clear();

	void	SetFirst();											// ��Ű�� ��Ͽ��� ù ��° ��Ű���� ����Ű�� �Ѵ�.

	bool	GetNext(CShopPackage& package);						// ���� ��Ű�� ��ü�� �ѱ�� ���� ��Ű�� ��ü�� ����Ű�� �Ѵ�.	
	bool	GetValueByKey(int nKey, CShopPackage& package);		// ��Ű�� ��ȣ�� �ش� ��Ű�� ��ü ��������
	bool	GetValueByIndex(int nIndex, CShopPackage& package);	// �ε��� ��ȣ�� �ش� ��Ű�� ��ü ��������

	bool	GetNext(CShopPackage*& package);					// overload : ������ ���
	bool	GetValueByKey(int nKey, CShopPackage*& package);	// overload : ������ ���
	bool	GetValueByIndex(int nIndex, CShopPackage*& package);// overload : ������ ���

	bool	SetPacketLeftCount(int PackageSeq, int nCount);

	// version 6
	void	AppendVer6(CShopPackage& package);


protected:
	std::map<int, CShopPackage>				m_Packages;
	std::map<int, CShopPackage>::iterator	m_iter;
	std::vector<int>						m_PackageIndex;
};
