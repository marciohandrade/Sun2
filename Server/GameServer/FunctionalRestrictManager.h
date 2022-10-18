#pragma once

//=======================================================================================================================
/// FunctionalRestrictManager
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2006. 5. 7~
	@purpose
			- 게임서버의 기능적 제한을 하여 버그, 혹은 해킹으로 부터 조기 대처하기 위해 제작
	@history
			- 기존의 eFUNCTIONALTYPE_ITEM을 eFUNCTIONALRESTRICT_TYPE로 변경
*/
//=======================================================================================================================

class FunctionalRestrictManager
{
public:
	FunctionalRestrictManager(void);
	~FunctionalRestrictManager(void);


	// eFUNCTIONALRESTRICT_TYPE 기능 추가
	VOID	AddFunctionalRestrict( DWORD FunctionalBit )		{ m_FunctionalType |= FunctionalBit; }
	// eFUNCTIONALRESTRICT_TYPE 기능 제거
	VOID	RemoveFunctionalRestrict( DWORD FunctionalBit )		{ m_FunctionalType &= ~FunctionalBit; }
	VOID	SetRestrictFunction( DWORD bits )					{ m_FunctionalType = bits; }

    bool IsPossibleFunction(eFUNCTIONALRESTRICT_TYPE type) const;
    bool IsRestrictFunction(eFUNCTIONALRESTRICT_TYPE type) const;
    DWORD GetRestrictFunction() const;
private:
	DWORD	m_FunctionalType;					//< eFUNCTIONALTYPE_ITEM의 비트연산처리
};

//==================================================================================================

inline bool FunctionalRestrictManager::IsPossibleFunction(eFUNCTIONALRESTRICT_TYPE type) const {
    return (m_FunctionalType & type) == 0;
}

inline bool FunctionalRestrictManager::IsRestrictFunction(eFUNCTIONALRESTRICT_TYPE type) const {
    return (m_FunctionalType & type) != 0;
}

inline DWORD FunctionalRestrictManager::GetRestrictFunction() const {
    return m_FunctionalType;
}

//==================================================================================================
extern FunctionalRestrictManager g_FunctionalRestrictMgr;
//==================================================================================================
