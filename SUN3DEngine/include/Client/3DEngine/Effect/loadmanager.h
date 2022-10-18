// loadmanager.h

#ifndef _EFFECT_LOADMANAGER_H_
#define _EFFECT_LOADMANAGER_H_

#include <string>
#include <hash_map>

class CDiscAccess;

//------------------------------------------------------------------------------
/**
    @class CLoadBaseManager
*/
class CLoadBaseManager
{
public:
    typedef stdext::hash_map <WzID, std::string> STD_HM_ID;

    // 생성자/소멸자
    CLoadBaseManager();
    ~CLoadBaseManager();

    // 생성
    BOOL CreateFromFile( const char* pszFilePath,
                         CDiscAccess* pDisc,
                         EENUM_LOAD_TYPE eLoadType = EELT_NEW );

    // 초기화
    void Clear();
    // 비었는가?
    BOOL IsEmpty() const;

    // WzID로 경로 얻기
    BOOL GetPath( WzID wzID, char* szPath ) const;

    // 맵 바로 얻기(순차 검색시 필요)
    const STD_HM_ID& GetMap() const;

    // 합치기
    void Merge( const CLoadBaseManager& rhs );
    // 복사
    void Copy( const CLoadBaseManager& rhs );

private:
    STD_HM_ID m_stdHashMapID;
};

//------------------------------------------------------------------------------
/**
*/
inline void CLoadBaseManager::Clear()
{
    m_stdHashMapID.clear();
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CLoadBaseManager::IsEmpty() const
{
    return m_stdHashMapID.empty();
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CLoadBaseManager::GetPath( WzID wzID, char* szPath ) const
{
    WzAssert( szPath );

    STD_HM_ID::const_iterator it = m_stdHashMapID.find( wzID );
    if( it != m_stdHashMapID.end() )
    {
        strcpy( szPath, it->second.c_str() );

        return TRUE;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
inline const CLoadBaseManager::STD_HM_ID& CLoadBaseManager::GetMap() const
{
    return m_stdHashMapID;
}

#endif // _EFFECT_LOADMANAGER_H_
