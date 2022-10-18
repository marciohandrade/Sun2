// wzunitsubtype.h

#ifndef _3DFILEACCESS_WZUNITSUBTYPE_H_
#define _3DFILEACCESS_WZUNITSUBTYPE_H_

#include "wzunittypedef.h"
#include <programcommon/wzarray.h>

class CWzArchive;

//------------------------------------------------------------------------------
/**
    @class CWzUnitSubType
*/
class CWzUnitSubType
{
public:
    // 생성자/소멸자
    CWzUnitSubType();
    virtual ~CWzUnitSubType();

    // 생성
    virtual BOOL Create();
    // 파괴
    virtual void Destroy();

    // Serialize
    virtual BOOL Serialize( CWzArchive* pArchive );

    // 본 크기 정보 추가/얻기
    WZ3DA_API void AddBoneScaling( const WzBoneScaling* pBoneScaling );
    WzBoneScaling* GetBoneScaling( int iArray );
    // 본 크기 정보 수 얻기
    int GetNumBoneScaling() const;

    // 장식 정보 추가/얻기
    WZ3DA_API void AddDecoration( const WzDecoration* pDecoration );
    WzDecoration* GetDecoration( int iArray );
    // 장식 정보 수 얻기
    int GetNumDecoration() const;

    // 렌더링 정보 추가/얻기
    WZ3DA_API void AddRenderInfo( const WzRenderInfo* pRenderInfo );
    WzRenderInfo* GetRenderInfo( int iArray );
    // 렌더링 정보 수 얻기
    int GetNumRenderInfo() const;

    // 크기 설정/얻기
    void SetScale( float fScale );
    void SetScale( const WzVector& wvScale );
    void GetScale( WzVector* out );

    // LOD 거리 활성 여부 설정/얻기
    void EnableLODDistance( BOOL bEnable );
    BOOL IsEnableLODDistance() const;

    // LOD 거리 설정/얻기
    void SetLODDistance( int iIndex, float fDist );
    float* GetLODDistance();

    // LOD 거리 수 설정/얻기
    void SetNumLODDistance( int iNum );
    int GetNumLODDistance() const;

    // 노말맵 거리 활성 여부 설정/얻기
    void EnableNormalMapDistance( BOOL bEnable );
    BOOL IsEnableNormalMapDistance() const;

    // 노말맵 거리 설정/얻기
    void SetNormalMapDistance( int iIndex, float fDist );
    float* GetNormalMapDistance();

    // 복사
    void Copy( const CWzUnitSubType& rhs );

private:
    // 변수 초기화
    void InitVariables();
    // 해제
    void Release();

protected:
    CWzArray<WzBoneScaling*> m_arBoneScaling;
    CWzArray<WzDecoration*> m_arDecoration;
    CWzArray<WzRenderInfo*> m_arRenderInfo;

    WzVector m_wvScale;

    BOOL m_bEnableLODDist;
    float m_fLODDist[NUM_LOD_DISTANCE_VER129];
    int m_iNumLODDistance;
    
    BOOL m_bEnableNormalMapDist;
    float m_fNormalMapDist[NUM_NORMAMAP_DISTANCE_VER126];
};

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSubType::SetScale( float fScale )
{
    SetVector( &m_wvScale, fScale, fScale, fScale );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSubType::SetScale( const WzVector& wvScale )
{
    m_wvScale = wvScale;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSubType::GetScale( WzVector* out )
{
    WzAssert( out );

    *out = m_wvScale;
}

//------------------------------------------------------------------------------
/**
*/
inline WzBoneScaling* CWzUnitSubType::GetBoneScaling( int iArray )
{
    WzAssert( iArray >= 0 && iArray < m_arBoneScaling.GetCount() );

    return m_arBoneScaling.Get( iArray );
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitSubType::GetNumBoneScaling() const
{
    return m_arBoneScaling.GetCount();
}

//------------------------------------------------------------------------------
/**
*/
inline WzDecoration* CWzUnitSubType::GetDecoration( int iArray )
{
    WzAssert( iArray >= 0 && iArray < m_arDecoration.GetCount() );

    return m_arDecoration.Get( iArray );
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitSubType::GetNumDecoration() const
{
    return m_arDecoration.GetCount();
}

//------------------------------------------------------------------------------
/**
*/
inline WzRenderInfo* CWzUnitSubType::GetRenderInfo( int iArray )
{
    WzAssert( iArray >= 0 && iArray < m_arRenderInfo.GetCount() );

    return m_arRenderInfo.Get( iArray );
}
//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitSubType::GetNumRenderInfo() const
{
    return m_arRenderInfo.GetCount();
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSubType::EnableLODDistance( BOOL bEnable )
{
    m_bEnableLODDist = bEnable;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitSubType::IsEnableLODDistance() const
{
    return m_bEnableLODDist;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSubType::SetLODDistance( int iIndex, float fDist )
{
    WzAssert( 0 <= iIndex && iIndex < NUM_LOD_DISTANCE_VER129 );

    m_fLODDist[iIndex] = fDist;
}

//------------------------------------------------------------------------------
/**
*/
inline float* CWzUnitSubType::GetLODDistance()
{
    return m_fLODDist;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSubType::SetNumLODDistance( int iNum )
{
    m_iNumLODDistance = iNum;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnitSubType::GetNumLODDistance() const
{
    return m_iNumLODDistance;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSubType::EnableNormalMapDistance( BOOL bEnable )
{
    m_bEnableNormalMapDist = bEnable;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnitSubType::IsEnableNormalMapDistance() const
{
    return m_bEnableNormalMapDist;
}

//------------------------------------------------------------------------------
/**
*/
inline float* CWzUnitSubType::GetNormalMapDistance()
{
    return m_fNormalMapDist;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnitSubType::SetNormalMapDistance( int iIndex, float fDist )
{
    WzAssert( 0 <= iIndex && iIndex < NUM_NORMAMAP_DISTANCE_VER126 );

    m_fNormalMapDist[iIndex] = fDist;
}

#endif // _3DFILEACCESS_WZUNITSUBTYPE_H_
