/*
	전체 상품(속성) 목록 객체

	iterator를 이용하여 순차적으로 상품(속성) 객체를 가져올 수 있다.
	상품 번호를 이용하여 상품(속성) 객체를 가져올 수 있다.
	상품 번호와 가격 번호를 이용하여 상품(속성) 객체를 가져올 수 있다.
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
	void	SetProductSeqFirst(int ProductSeq);				// 해당 상품 번호의 상품(속성) 목록 첫 번째 위치
	bool	GetProductSeqNext(CShopProduct& product);		// 상품(속성) 객체를 가져오고 다음 상품(속성)에 위치

	void	SetPriceSeqFirst(int ProductSeq, int PriceSeq);	// 해당 상품 번호와 가격 번호의 조합으로 상품(속성) 목록 첫 번째 위치
	bool	GetPriceSeqNext(CShopProduct& product);			// 상품(속성) 객체를 가져오고 다음 상품(속성)에 위치

	void	AppendVer3(CShopProduct& product);


	// version 3, 6
	void	Clear();							
	int		GetSize();										// 상품 개수

	void	SetFirst();
	bool	GetNext(CShopProduct& product);
	bool	GetNext(CShopProduct*& product);				// overload : 포인터 사용


	// version 6
	void	AppendVer6(CShopProduct& product);				// 상품 추가

	bool	GetValueByKey(int nProductSeq, CShopProduct& Product);	// 상품 번호로 상품 객체를 가져온다.
	bool	GetValueByIndex(int nIndex, CShopProduct& Product);		// 인덱스 번호로 상품 객체를 가져온다. 

	bool	GetValueByKey(int nProductSeq, CShopProduct*& Product);	// overload : 포인터 사용
	bool	GetValueByIndex(int nIndex, CShopProduct*& Product);	// overload : 포인터 사용



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
	std::map<int, CShopProduct>				m_ProductsVer6;			// 상품 객체 맵
	std::map<int, CShopProduct>::iterator	m_ProductIterVer6;		// 상품 iterator
	std::vector<int> m_ProductIndex;								// 상품 번호 인덱스 목록

};
