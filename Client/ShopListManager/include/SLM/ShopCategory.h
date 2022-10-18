
/**************************************************************************************************

카테고리 객체

현재 카테고리 정보를 가지고 있다.
현재 카테고리 하위의 "카테고리 번호"를 목록으로 가지고 있다.
현재 카테고리가 최 하위 카테고리라면 "패키지 번호" 목록을 가지고 있다.

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

	void SetCategoryFirst();							// 하위 카테고리 목록의 첫 번째 항록을 가리키도록 설정한다.
	bool GetCategoryNext(int& CategorySeq);				// 하위 카테고리 번호를 리턴하고 다음 하위 카테고리 번호를 가리킨다.

	void SetPackagSeqFirst();							// 카테고리에 등록되어 있는 페키지 목록의 첫 번째 항목을 가리키도록 설정한다.
	bool GetPackagSeqNext(int& PackagSeq);				// 페키지 번호를 리턴하고 다음 페키지 번호를 가리킨다.

	void AddPackageSeq(int PackageSeq);
	void ClearPackageSeq();


	// version 6
	bool SetCategoryVer6(tstring& strData, const tstring& delimiter);
	bool SetCategoryVer6W(std::wstring& strData, const std::wstring& delimiter);

	void ClearChildCategorySeqList();
	void ClearChildPackageSeqList();
	void ClearAllChildPackageSeqList();

	void AddChildCategorySeqList(int CategorySeq);
	void SetChildCategorySeqFirst();					// 하위 카테고리 목록의 첫 번째 항록을 가리키도록 설정한다.
	bool GetChildCategorySeqNext(int& CategorySeq);		// 하위 카테고리 번호를 리턴하고 다음 하위 카테고리 번호를 가리킨다.

	void AddChildPackageSeqList(int PackageSeq);
	void SetChildPackagSeqFirst();						// 카테고리에 등록되어 있는 패키지 목록의 첫 번째 항목을 가리키도록 설정한다.
	bool GetChildPackagSeqNext(int& PackageSeq);		// 패키지 번호를 리턴하고 다음 패키지 번호를 가리킨다.
	bool ExistChildPackageSeq(int PackageSeq);			// PackageSeq가 있는지 확인

	void AddAllChildPackageSeqList(int PackageSeq);
	void SetAllChildPackageSeqFirst();					// 카테고리 및 모든 하위 카테고리에 등록되어 있는 패키지 목록의 첫 번째 항목을 가리키도록 설정한다.
	bool GetAllChildPackageSeqNext(int& PackageSeq);	// 패키지 번호를 리턴하고 다음 패키지 번호를 가리킨다.
	std::vector<int>& GetAllChildPackageSeq();

public:
	int		ProductDisplaySeq;								// 1. 카테고리 번호
	TCHAR	CategroyName[SHOPLIST_LENGTH_CATEGORYNAME];		// 2. 카테고리 이름
	WCHAR	CategroyNameW[SHOPLIST_LENGTH_CATEGORYNAME];	// 2. 카테고리 이름		(UseUnicodeString 인 경우 사용됨)
	int		EventFlag;										// 3. 이벤트 카테고리 여부 (199:이벤트, 200:일반)	
	int		OpenFlag;										// 4. 공개 여부 (201:공개, 202: 비공개)
	int		ParentProductDisplaySeq;						// 5. 부모 카테고리 번호
	int		DisplayOrder;									// 6. 노출 순서
	int		Root;											// 7. 최상위 카테고리 구분 (1: 최상위, 0: 서브)

	// version 3
	std::vector<int> CategoryList;						// 현재 카테고리의 하위 "카테고리 번호" 목록
	std::vector<int>::iterator Categoryiter;

	std::vector<int> PackageList;						// 현재 카테고리에 포함된 "페키지 번호" 목록
	std::vector<int>::iterator Packageiter;

	// version 6
	std::vector<int> ChildCategorySeqList;				// 현재 카테고리의 하위 "카테고리 번호" 목록
	std::vector<int>::iterator ChildCategorySeqIter;

	std::vector<int> ChildPackageSeqList;				// 현재 카테고리에 포함된 "패키지 번호" 목록
	std::vector<int>::iterator ChildPackageSeqIter;

	std::vector<int> AllChildPackageSeqList;			// 현재 카테고리에 포함된 "패키지 번호" 목록 및 모든 하위 카테고리가 갖고 있는 "패키지 번호" 목록
	std::vector<int>::iterator AllChildPackageSeqIter;

};
