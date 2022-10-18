// wzunit.h

#ifndef _3DFILEACCESS_WZUNIT_H_
#define _3DFILEACCESS_WZUNIT_H_

#include "wzunitsubtype.h"

class WzAnimationInfo;
class CWzArchive;
class CWzdBase;

//------------------------------------------------------------------------------
/**
    @class CWzUnit
*/
class CWzUnit
{
public:
    // 생성자/소멸자
    CWzUnit();
    virtual ~CWzUnit();

    // 저장/로드
    virtual BOOL Save( const char* lpszFilePath );
    virtual BOOL Load( const char* lpszFilePath );

    // Serialize
    virtual BOOL Serialize( CWzArchive* pArchive );

    // 생성/파괴
    virtual BOOL Create();
    virtual void Destroy();

    // 통지
    virtual void OnNotify( int iCode, DWORD dwParam1, DWORD dwParam2 );

    // 타입 설정/얻기
    void SetType( WzID wiType );
    WzID GetType() const;

    // Wzd 수 얻기
    int GetNumWzd() const;

    // 최고 레벨(최고 낮은 품질) 얻기
    int GetMaxWzdLOD() const;

    // LOD 메시 파일명 추가/얻기
    WZ3DA_API void AddWzdFileName( const char* lpszFileName );
    WZ3DA_API void GetWzdFileName( char* lpszDst, int iLOD = 0 );

    // LOD 메시 핸들 설정/얻기
    void SetWzdHandle( HANDLE hWzd, int iLOD = 0 );
    HANDLE GetWzdHandle( int iLOD = 0 ) const;

    // LOD 메시 포인터 얻기
    CWzdBase* GetWzdPtr( int iLOD );
    CWzdBase* GetValidWzdPtr();

    // 애니메이션 스피드 설정/얻기
    void SetAnimationSpeed( float fAnimationSpeed );
    float GetAnimationSpeed() const;

    // 애니메이션 정보 수 얻기
    int GetCountAnimationInfo() const;
    // 애니메이션 정보 추가
    WZ3DA_API void AddAnimationInfo( const WzAnimationInfo& ai );
    // 인덱스로 애니메이션 정보 얻기
    WzAnimationInfo* GetAnimationInfoByIndex( int iIndex );
    // ID로 애니메이션 정보 얻기
    WZ3DA_API WzAnimationInfo* GetAnimationInfo( WzID wiAni );

    // 본 정보 수 얻기
    int GetCountBoneInfo() const;
    // 본 정보 추가
    WZ3DA_API void AddBoneInfo( const WzBoneInfo& bi );
    // 인덱스로 본 정보 얻기
    WzBoneInfo* GetBoneInfoByIndex( int iIndex );
    // ID로 본 정보 얻기
    WZ3DA_API WzBoneInfo* GetBoneInfo( WzID wiBone );

    // 서브 타입 수 얻기
    int GetNumSubType() const;
    // 서브 타입 생성
    WZ3DA_API CWzUnitSubType* CreateSubType();
    // 서브 타입 얻기
    CWzUnitSubType* GetSubType( int iSubType );

    // 메시 그룹 이름 정보 추가
    WZ3DA_API void AddMeshGroupNameInfo( const MeshGroupNameInfo& info );
    // 메시 그룹 이름 정보 얻기
    MeshGroupNameInfo* GetMeshGroupNameInfo( int iIndex );
    // 이름으로 메시 그룹 인덱스 얻기
    WZ3DA_API int GetMeshGroupIndexByName( const char* pszMeshGroupName );

    // 디퓨즈맵 정보 수
    int GetDiffuseMapInfoCount() const;
    // 디퓨즈맵 정보 추가
    WZ3DA_API void AddDiffuseMapInfo( int iIndex, const WzMappingInfo& mi );
    // 디퓨즈맵 정보 얻기
    WzMappingInfo* GetDiffuseMapInfo( int iIndex );

    // 스피큘러맵 정보 수 얻기
    int GetSpecularMapInfoCount() const;
    // 스피큘러맵 정보 추가
    WZ3DA_API void AddSpecularMapInfo( int iIndex, const WzMappingInfo& mi );
    // 스프큘러맵 정보 얻기
    WzMappingInfo* GetSpecularMapInfo( int iIndex );

    // 노말맵 정보 수 얻기
    int GetNormalMapInfoCount() const;
    // 노말맵 정보 추가
    WZ3DA_API void AddNormalMapInfo( int iIndex, const WzMappingInfo& mi );
    // 노말맵 정보 얻기
    WzMappingInfo* GetNormalMapInfo( int iIndex );

    // 글로우맵 정보 수 얻기
    int GetGlowMapInfoCount() const;
    // 글로우맵 정보 추가
    WZ3DA_API void AddGlowMapInfo( int iIndex, const WzMappingInfo& mi );
    // 글로우맵 정보 얻기
    WzMappingInfo* GetGlowMapInfo( int iIndex );

    // 내부맵 정보 수
    int GetInternalMapInfoCount() const;
    // 내부맵 정보 추가
    WZ3DA_API void AddInternalMapInfo( int iIndex, const WzMappingInfo& mi );
    // 내부맵 정보 얻기
    WzMappingInfo* GetInternalMapInfo( int iIndex );

    // 필터 정보 추가
    WZ3DA_API void AddFilterInfo( int iIndex, const WzMappingInfo& fi );
    // 필터 정보 얻기
    WzMappingInfo* GetFilterInfo( int iIndex );

    // 텍스처 애니메이션맵 정보 수
    int GetTexAniMapInfoCount() const;
    // 텍스처 애니메이션맵 정보 추가
    WZ3DA_API void AddTexAniMapInfo( int iIndex, const WzTexAniMappingInfo& tami );
    // 텍스처 애니메이션맵 정보 얻기
    WzTexAniMappingInfo* GetTexAniMapInfo( int iIndex );

    // 텍스처 애니메이션셋 정보 수
    int GetCountTexAniSetInfo() const;
    // 텍스처 애니메이션셋 정보 추가
    WZ3DA_API void AddTexAniSetInfo( const WzTexAniSetInfo& tasi );
    // 인덱스로 텍스처 애니메이션셋 정보 얻기
    WzTexAniSetInfo* GetTexAniSetInfoByIndex( int iIndex );
    // ID로 텍스처 애니메이션셋 정보 얻기
    WZ3DA_API WzTexAniSetInfo* GetTexAniSetInfo( WzID wiID );

    // 바운딩 볼륨 설정
    void SetBoundingVolume( const BvAABB& bvAABB, const BvSphere& bvSphere );
    // 바운딩 박스 얻기
    const BvAABB& GetBvAABB() const;
    // 바운딩 구 얻기
    const BvSphere& GetBvSphere() const;

    // 바운딩 볼륨을 위한 기준 본 설정
    void SetBaseBoneIndexForBV( int iBoneIndex );
    // 바운딩 볼륨을 위한 기준 본 얻기
    int GetBaseBoneIndexForBV() const;
    
    // 자동 바운딩볼륨갱신 상태 얻기
    BOOL GetAutoBvUpdate() const;
    // 자동 바운딩볼륨갱신 상태 셋팅
    void SetAutoBvUpdate( BOOL bFlag );
    // 자동 바운딩볼륨본번호 얻기
    int GetAutoBvBone() const;
    // 자동 바운딩볼륨본번호 셋팅
    void SetAutoBvBone( int iBone );
    
    // 추가 바운딩 볼륨 수 얻기
    int GetCountBVExtraInfo() const;
    // 추가 바운딩 볼륨 추가
    WZ3DA_API void AddBVExtraInfo( const WzBVExtraInfo& bei );
    // 인덱스로 추가 바운딩 볼륨 얻기
    WzBVExtraInfo* GetBVExtraInfoByIndex( int iIndex );
    
    // 위치/회전 오프셋 정보
    WzUnitTransformOffset* GetTransFormOffset();
    
     // 유닛 로딩 상태 얻기
    ENUM_LOAD_STATE GetLoadState();

    // 강제 LOD 적용?
    bool IsLodByForce() const;

private:
    // 변수 초기화
    void InitVariables();
    // 해제
    void Release();

    // WzdBase 찾기
    WZ3DA_API void FindWzdPtr( int iLOD );
    // Wzd 로딩 상태 조사
    WZ3DA_API void CheckLoadState();

    // Serialize Header
    BOOL SerializeHeader( CWzArchive* pArchive );

    // Serialize Default Info
    BOOL SerializeDefaultInfo( CWzArchive* pArchive );
    // Serialize LOD info
    BOOL SerializeLODInfo( CWzArchive* pArchive );
    // xxx: 기존 호환 (Morph Mesh Info)
    BOOL SerializeMorphMeshInfo( CWzArchive* pArchive );
    // Serialize Animation Info
    BOOL SerializeAnimInfo( CWzArchive* pArchive );
    // Serialize Bone Info
    BOOL SerializeBoneInfo( CWzArchive* pArchive );
    // Serialize Mapping Info
    BOOL SerializeMappingInfo( CWzArchive* pArchive );
    // Serialize Combo Info
    BOOL SerializeComboInfo( CWzArchive* pArchive );
    // Serialize Force LOD Info
    BOOL SerializeForceLODInfo( CWzArchive* pArchive );
    // xxx: 기존 호환 (IK Info)
    BOOL SerializeIKInfo( CWzArchive* pArchive );
    // Serialize BV Info
    BOOL SerializeBVInfo( CWzArchive* pArchive );

    // Serialize SubType
    BOOL SerializeSubType( CWzArchive* pArchive );
    // Serialize Mesh Group
    BOOL SerializeMeshGroup( CWzArchive* pArchive );

    // Serialize UnitOffset
    BOOL SerializeUnitOffset( CWzArchive* pArchive );

protected:
    WzID m_wiType;

    int m_iNumWzd;
    char* m_pszWzdFile[WZ_MAX_LOD + 1];

    HANDLE m_hWzd[WZ_MAX_LOD + 1];
    CWzdBase* m_wzdPtr[WZ_MAX_LOD + 1];

    float m_fAnimationSpeed;

    CWzArray<WzAnimationInfo*> m_arAniInfo;
    CWzArray<WzBoneInfo*> m_arBoneInfo;

    CWzArray<CWzUnitSubType*> m_arSubType;
    CWzArray<MeshGroupNameInfo*> m_arMeshGroupName;

    CWzArray<WzMappingInfo*> m_arDiffuseMapInfo;
    CWzArray<WzMappingInfo*> m_arSpecularMapInfo;
    CWzArray<WzMappingInfo*> m_arNormalMapInfo;
    CWzArray<WzMappingInfo*> m_arGlowMapInfo;

    CWzArray<WzMappingInfo*> m_arInternalMapInfo;
    CWzArray<WzMappingInfo*> m_arFilterInfo;

    CWzArray<WzTexAniMappingInfo*> m_arTexAniMapInfo;
    CWzArray<WzTexAniSetInfo*> m_arTexAniSetInfo;

    BOOL m_bHasBVInfo;
    BvAABB m_bvAABB;
    BvSphere m_bvSphere;

    WzUnitTransformOffset m_transformOffset;
    
    int m_iBaseBoneIndex;
    BOOL m_bAutoBvUpdate;
    int  m_nAutoBvBone;

    CWzArray<WzBVExtraInfo*> m_arBVExtraInfo;

    bool m_bLodByForce;

    ENUM_LOAD_STATE m_eLoadState;
};

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnit::SetType( WzID wiType )
{
    m_wiType = wiType; 
}

//------------------------------------------------------------------------------
/**
*/
inline WzID CWzUnit::GetType() const
{
    return m_wiType;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetNumWzd() const
{ 
    return m_iNumWzd;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetMaxWzdLOD() const
{
    return max( 0, m_iNumWzd - 1 );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnit::SetWzdHandle( HANDLE hWzd, int iLOD )
{
    WzAssert( iLOD >= 0 && iLOD <= WZ_MAX_LOD );

    m_hWzd[iLOD] = hWzd;

    FindWzdPtr( iLOD );
}

//------------------------------------------------------------------------------
/**
*/
inline HANDLE CWzUnit::GetWzdHandle( int iLOD ) const
{
    if( iLOD >= 0 && iLOD <= WZ_MAX_LOD )
    {
        return m_hWzd[iLOD];
    }

    return INVALID_HANDLE_VALUE;
}

//------------------------------------------------------------------------------
/**
*/
inline CWzdBase* CWzUnit::GetWzdPtr( int iLOD )
{
    if( iLOD >= 0 && iLOD < m_iNumWzd )
    {
        if( !m_wzdPtr[iLOD] )
        {
            FindWzdPtr( iLOD );
        }

        return m_wzdPtr[iLOD];
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline CWzdBase* CWzUnit::GetValidWzdPtr()
{
    // 일단 가장 퀄리티 좋은 넘을 보고
    CWzdBase* pWzd = GetWzdPtr( 0 );
    if( !pWzd && m_iNumWzd > 1 )
    {
        // 없으면 가장 퀄리티 나쁜 넘으로
        pWzd = GetWzdPtr( m_iNumWzd - 1 );
    }

    return pWzd;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnit::SetAnimationSpeed( float fAnimationSpeed )
{
    WzAssert( fAnimationSpeed > 0.0f );

    m_fAnimationSpeed = fAnimationSpeed;
}

//------------------------------------------------------------------------------
/**
*/
inline float CWzUnit::GetAnimationSpeed() const
{
    return m_fAnimationSpeed;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzUnit::GetAutoBvUpdate() const
{
    return m_bAutoBvUpdate;
}
//------------------------------------------------------------------------------
/**
*/
inline void CWzUnit::SetAutoBvUpdate( BOOL bFlag )
{
    m_bAutoBvUpdate = bFlag;
}
//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetAutoBvBone() const
{
    return m_nAutoBvBone;
}
//------------------------------------------------------------------------------
/**
*/
inline void CWzUnit::SetAutoBvBone( int iBone )
{
    m_nAutoBvBone = iBone;
}
//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetCountAnimationInfo() const
{
    return m_arAniInfo.GetCount();
}

//------------------------------------------------------------------------------
/**
*/
inline WzAnimationInfo* CWzUnit::GetAnimationInfoByIndex( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arAniInfo.GetCount() )
    {
        return m_arAniInfo.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetCountBoneInfo() const
{
    return m_arBoneInfo.GetCount(); 
}

//------------------------------------------------------------------------------
/**
*/
inline WzBoneInfo* CWzUnit::GetBoneInfoByIndex( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arBoneInfo.GetCount() )
    {
        return m_arBoneInfo.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetNumSubType() const
{
    return m_arSubType.GetCount();
}

//------------------------------------------------------------------------------
/**
*/
inline CWzUnitSubType* CWzUnit::GetSubType( int iSubType )
{
    if( iSubType >= 0 && iSubType < m_arSubType.GetCount() )
    {
        return m_arSubType.Get( iSubType );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline MeshGroupNameInfo* CWzUnit::GetMeshGroupNameInfo( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arMeshGroupName.GetCount() )
    {
        return m_arMeshGroupName.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetDiffuseMapInfoCount() const
{
    return m_arDiffuseMapInfo.GetCount();
}

//------------------------------------------------------------------------------
/**
*/
inline WzMappingInfo* CWzUnit::GetDiffuseMapInfo( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arDiffuseMapInfo.GetCount() )
    {
        return m_arDiffuseMapInfo.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetSpecularMapInfoCount() const
{
    return m_arSpecularMapInfo.GetCount();
}

//------------------------------------------------------------------------------
/**
*/
inline WzMappingInfo* CWzUnit::GetSpecularMapInfo( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arSpecularMapInfo.GetCount() )
    {
        return m_arSpecularMapInfo.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetNormalMapInfoCount() const
{
    return m_arNormalMapInfo.GetCount();
}

//------------------------------------------------------------------------------
/**
*/
inline WzMappingInfo* CWzUnit::GetNormalMapInfo( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arNormalMapInfo.GetCount() )
    {
        return m_arNormalMapInfo.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetGlowMapInfoCount() const
{
    return m_arGlowMapInfo.GetCount();
}

//------------------------------------------------------------------------------
/**
*/
inline WzMappingInfo* CWzUnit::GetGlowMapInfo( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arGlowMapInfo.GetCount() )
    {
        return m_arGlowMapInfo.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetInternalMapInfoCount() const
{
    return m_arInternalMapInfo.GetCount();
}

//------------------------------------------------------------------------------
/**
*/
inline WzMappingInfo* CWzUnit::GetInternalMapInfo( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arInternalMapInfo.GetCount() )
    {
        return m_arInternalMapInfo.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMappingInfo* CWzUnit::GetFilterInfo( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arFilterInfo.GetCount() )
    {
        return m_arFilterInfo.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetTexAniMapInfoCount() const
{
    return m_arTexAniMapInfo.GetCount();
}

//------------------------------------------------------------------------------
/**
*/
inline WzTexAniMappingInfo* CWzUnit::GetTexAniMapInfo( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arTexAniMapInfo.GetCount() )
    {
        return m_arTexAniMapInfo.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetCountTexAniSetInfo() const
{
    return m_arTexAniSetInfo.GetCount(); 
}

//------------------------------------------------------------------------------
/**
*/
inline WzTexAniSetInfo* CWzUnit::GetTexAniSetInfoByIndex( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arTexAniSetInfo.GetCount() )
    {
        return m_arTexAniSetInfo.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnit::SetBoundingVolume( const BvAABB& bvAABB, const BvSphere& bvSphere )
{ 
    m_bvAABB = bvAABB; 
    m_bvSphere = bvSphere; 
}

//------------------------------------------------------------------------------
/**
*/
inline const BvAABB& CWzUnit::GetBvAABB() const
{
    return m_bvAABB;
}

//------------------------------------------------------------------------------
/**
*/
inline const BvSphere& CWzUnit::GetBvSphere() const
{
    return m_bvSphere;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzUnit::SetBaseBoneIndexForBV( int iBoneIndex )
{
    m_iBaseBoneIndex = iBoneIndex; 
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetBaseBoneIndexForBV() const
{
    return m_iBaseBoneIndex; 
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzUnit::GetCountBVExtraInfo() const
{
    return m_arBVExtraInfo.GetCount(); 
}

//------------------------------------------------------------------------------
/**
*/
inline WzBVExtraInfo* CWzUnit::GetBVExtraInfoByIndex( int iIndex )
{
    if( iIndex >= 0 && iIndex < m_arBVExtraInfo.GetCount() )
    {
        return m_arBVExtraInfo.Get( iIndex );
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline WzUnitTransformOffset* CWzUnit::GetTransFormOffset()
{
    return &m_transformOffset;
}

//------------------------------------------------------------------------------
/**
*/
inline ENUM_LOAD_STATE CWzUnit::GetLoadState()
{
    if( m_eLoadState != eLOAD_FULL )
    {
        CheckLoadState();
    }

    return m_eLoadState;
}

//------------------------------------------------------------------------------
/**
*/
inline bool CWzUnit::IsLodByForce() const
{
    return m_bLodByForce; 
}

#endif // _3DFILEACCESS_WZUNIT_H_
