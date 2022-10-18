/*
	��ü ��ǰ(�Ӽ�) ��� ��ü

	iterator�� �̿��Ͽ� ���������� ��ǰ(�Ӽ�) ��ü�� ������ �� �ִ�.
	��ǰ ��ȣ�� �̿��Ͽ� ��ǰ(�Ӽ�) ��ü�� ������ �� �ִ�.
	��ǰ ��ȣ�� ���� ��ȣ�� �̿��Ͽ� ��ǰ(�Ӽ�) ��ü�� ������ �� �ִ�.
*/

#pragma once
#include <SLM/CommonSLM.h>
#include <SLM/ShopProduct.h>


class CShopProductList
{
public:
	CShopProductList(void);
	~CShopProductList(void);

	// version 3
	void	SetProductSeqFirst(int ProductSeq);				// �ش� ��ǰ ��ȣ�� ��ǰ(�Ӽ�) ��� ù ��° ��ġ
	bool	GetProductSeqNext(CShopProduct& product);		// ��ǰ(�Ӽ�) ��ü�� �������� ���� ��ǰ(�Ӽ�)�� ��ġ

	void	SetPriceSeqFirst(int ProductSeq, int PriceSeq);	// �ش� ��ǰ ��ȣ�� ���� ��ȣ�� �������� ��ǰ(�Ӽ�) ��� ù ��° ��ġ
	bool	GetPriceSeqNext(CShopProduct& product);			// ��ǰ(�Ӽ�) ��ü�� �������� ���� ��ǰ(�Ӽ�)�� ��ġ

	void	AppendVer3(CShopProduct& product);


	// version 3, 6
	void	Clear();							
	int		GetSize();										// ��ǰ ����

	void	SetFirst();
	bool	GetNext(CShopProduct& product);
	bool	GetNext(CShopProduct*& product);				// overload : ������ ���


	// version 6
	void	AppendVer6(CShopProduct& product);				// ��ǰ �߰�

	bool	GetValueByKey(int nProductSeq, CShopProduct& Product);	// ��ǰ ��ȣ�� ��ǰ ��ü�� �����´�.
	bool	GetValueByIndex(int nIndex, CShopProduct& Product);		// �ε��� ��ȣ�� ��ǰ ��ü�� �����´�. 

	bool	GetValueByKey(int nProductSeq, CShopProduct*& Product);	// overload : ������ ���
	bool	GetValueByIndex(int nIndex, CShopProduct*& Product);	// overload : ������ ���



protected:
	// versoin 3
	int	PriceSeqKey;
	std::multimap<int, CShopProduct>			m_Products;	
	std::multimap<int, CShopProduct>::iterator	m_ProductIter;
	std::multimap<int, CShopProduct>::iterator	m_ProductSeqIter;
	std::multimap<int, CShopProduct>::iterator	m_PriceSeqIter;	
	std::pair<std::multimap<int, CShopProduct>::iterator, std::multimap<int, CShopProduct>::iterator> m_ProductRange;
	std::pair<std::multimap<int, CShopProduct>::iterator, std::multimap<int, CShopProduct>::iterator> m_PriceRange;

	// version 6
	std::map<int, CShopProduct>				m_ProductsVer6;			// ��ǰ ��ü ��
	std::map<int, CShopProduct>::iterator	m_ProductIterVer6;		// ��ǰ iterator
	std::vector<int> m_ProductIndex;								// ��ǰ ��ȣ �ε��� ���

};
