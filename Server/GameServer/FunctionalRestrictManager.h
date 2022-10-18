#pragma once

//=======================================================================================================================
/// FunctionalRestrictManager
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2006. 5. 7~
	@purpose
			- ���Ӽ����� ����� ������ �Ͽ� ����, Ȥ�� ��ŷ���� ���� ���� ��ó�ϱ� ���� ����
	@history
			- ������ eFUNCTIONALTYPE_ITEM�� eFUNCTIONALRESTRICT_TYPE�� ����
*/
//=======================================================================================================================

class FunctionalRestrictManager
{
public:
	FunctionalRestrictManager(void);
	~FunctionalRestrictManager(void);


	// eFUNCTIONALRESTRICT_TYPE ��� �߰�
	VOID	AddFunctionalRestrict( DWORD FunctionalBit )		{ m_FunctionalType |= FunctionalBit; }
	// eFUNCTIONALRESTRICT_TYPE ��� ����
	VOID	RemoveFunctionalRestrict( DWORD FunctionalBit )		{ m_FunctionalType &= ~FunctionalBit; }
	VOID	SetRestrictFunction( DWORD bits )					{ m_FunctionalType = bits; }

    bool IsPossibleFunction(eFUNCTIONALRESTRICT_TYPE type) const;
    bool IsRestrictFunction(eFUNCTIONALRESTRICT_TYPE type) const;
    DWORD GetRestrictFunction() const;
private:
	DWORD	m_FunctionalType;					//< eFUNCTIONALTYPE_ITEM�� ��Ʈ����ó��
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
