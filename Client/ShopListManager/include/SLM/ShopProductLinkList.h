/*
	��ü ��ǰ ��ũ ��� ��ü

	iterator�� �̿��Ͽ� ���������� ��ǰ ��ũ ��ü�� ������ �� �ִ�.
	��ǰ ��ȣ�� ���� ��ȣ�� �������� ��ǰ ��ũ ��ü�� ������ �� �ִ�.

	(��Ű���� ������ ���� ��ȣ�� �� ���� ��� ���� ��ȣ 0���� �ص� �ȴ�.)
	(��Ű���� ������ ���� ��ȣ�� ���� ���� ��� ���� ��ȣ�� ��Ȯ�� �����ؾ� �Ѵ�.)

	��ǰ ��ȣ�� ���� ��ȣ�� �ش��ϴ� ��ũ ��ü�� ���� �� �̴�.
	���� ��ũ ��ü�� �̿��Ͽ� �ϳ��� ��ǰ�� ǥ�� �ؾ� �Ѵ�.
	��ǰ ��ũ ��ü���� ��ǰ�� �Ӽ��� �� �Ӽ��� �ش��ϴ� Value ���� ��ũ �� ���� ��ü �̴�.


	(ex) A ��Ű���� ���� ��ǰ��ȣ 100�� ���� ��ȣ 10���� ��ǰ�� ǥ�� �ϴ� ���.

	1. ProductList ���� 100�� Product �������� : �̸�, �⺻ ��ǰ ���� ǥ�� 
	2. ProductLinkList ���� ��ǰ��ȣ 100�� ���� ��ȣ 10���� ��ũ ���� �д´�.
	   5�� �Ӽ��� ���� 123 �̴� -> 5�� �Ӽ��� ���� ������ �Ӽ� ��Ͽ��� �о�´�.
	   6�� �Ӽ��� ���� 234 �̴� -> 6�� �Ӽ��� ���� ������ �Ӽ� ��Ͽ��� �о�´�.
*/

#pragma once
#include <SLM/CommonSLM.h>
#include <SLM/ShopProductLink.h>


class CShopProductLinkList
{
public:
	CShopProductLinkList(void);
	~CShopProductLinkList(void);

	void Clear();	

	virtual void AppendVer6(CShopProductLink link);

	// Interface --------------------------------------------------
	int GetSize();													// ��ǰ ��ũ ����

	void SetProductFirst(int nProductSeq, int nPriceSeq = 0);		// ��ǰ ��ȣ�� ���� ��ȣ�� �ش��ϴ� ù ��° ��ũ ��ü�� ����Ű�� �Ѵ�.
																	
	void SetFirst();												// ��Ͽ��� ù ��° ��ǰ ��ũ ��ü�� ����Ű�� �Ѵ�.
	bool GetNext(CShopProductLink& link);							// ���� ��ǰ ��ũ ��ü�� �ѱ�� ���� ��ǰ ��ũ ��ü�� ����Ű�� �Ѵ�.
	bool GetProductNext(CShopProductLink& link);					// ���� ��ǰ ��ũ ��ü�� �ѱ�� ���� ��ǰ ��ũ ��ü�� ����Ű�� �Ѵ�.

	bool GetNext(CShopProductLink*& link);							// overload : ������ ���
	bool GetProductNext(CShopProductLink*& link);					// overload : ������ ���

	// ------------------------------------------------------------

protected:
	int PriceSeqKey;
	std::multimap<int, CShopProductLink> m_ProductLinks;	
	std::multimap<int, CShopProductLink>::iterator m_AllProductLinkIter;
	std::multimap<int, CShopProductLink>::iterator m_CurrentProductIter;
	std::pair<std::multimap<int, CShopProductLink>::iterator, std::multimap<int, CShopProductLink>::iterator> m_ProductRange;
};
