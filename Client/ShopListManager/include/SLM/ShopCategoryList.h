
/**************************************************************************************************

전체 카테고리 목록 객체

iterator를 이용하여 순차적으로 카테고리 객체를 가져올 수 있다.
카테고리 번호를 이용하여 카테고리 객체를 가져올 수 있다.

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
	int				GetSize();												// 카테고리 개수
	void			SetFirst();												// 카테고리 목록에서 첫 번째 카테고리를 가리키게 한다.

	bool			GetNext(CShopCategory& category);						// 현재 카테고리 객체를 넘기고 다음 카테고리 객체를 가리키게 한다.
	bool			GetValueByKey(int nKey, CShopCategory& category);		// 카테고리 순번으로 카테고리 객체를 가져온다.
	bool			GetValueByIndex(int nIndex, CShopCategory& category);	// 인덱스 번호로 카테고리 객체를 가져온다. 

	bool			GetNext(CShopCategory*& category);						// overload : 포인터 사용
	bool			GetValueByKey(int nKey, CShopCategory*& category);		// overload : 포인터 사용
	bool			GetValueByIndex(int nIndex, CShopCategory*& category);	// overload : 포인터 사용


	// version 6
	void			ClearCategory();
	void			ClearPackageLink();
	void			AppendPackageLinkVer6(CShopPackageLink& link);				// 패키지 링크 추가
	void			AppendVer6(CShopCategory& category);
	void			SortPackageLinkList();


	// version 6 : 추가 기능
	void			SetRootPackageSeq();						// root 카테고리에 하위 카테고리의 모든 PackageSeq를 등록한다.
	void			SetAllChildPackageSeq();

private:
	std::map<int, CShopCategory>::iterator	FindRootCategory(const std::map<int, CShopCategory>::iterator& it);	// root 카테고리의 iterator를 찾아주는 재귀 함수
	void FindAllChildPackageSeq(int ProductDisplaySeq, std::vector<int>& vec);

protected:
	// version 3, 6	
	std::map<int, CShopCategory> m_Categorys;				// 카테고리 객체 맵
	std::map<int, CShopCategory>::iterator m_Categoryiter;	// 카테고리 iterator
	std::vector<int> m_CategoryIndex;						// 카테고리 번호 인덱스 목록

	// version 6
	std::vector<CShopPackageLink> m_PackageLinks;			// 패키지 링크 목록
};
