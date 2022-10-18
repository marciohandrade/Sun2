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
    // ������/�Ҹ���
    CWzUnit();
    virtual ~CWzUnit();

    // ����/�ε�
    virtual BOOL Save( const char* lpszFilePath );
    virtual BOOL Load( const char* lpszFilePath );

    // Serialize
    virtual BOOL Serialize( CWzArchive* pArchive );

    // ����/�ı�
    virtual BOOL Create();
    virtual void Destroy();

    // ����
    virtual void OnNotify( int iCode, DWORD dwParam1, DWORD dwParam2 );

    // Ÿ�� ����/���
    void SetType( WzID wiType );
    WzID GetType() const;

    // Wzd �� ���
    int GetNumWzd() const;

    // �ְ� ����(�ְ� ���� ǰ��) ���
    int GetMaxWzdLOD() const;

    // LOD �޽� ���ϸ� �߰�/���
    WZ3DA_API void AddWzdFileName( const char* lpszFileName );
    WZ3DA_API void GetWzdFileName( char* lpszDst, int iLOD = 0 );

    // LOD �޽� �ڵ� ����/���
    void SetWzdHandle( HANDLE hWzd, int iLOD = 0 );
    HANDLE GetWzdHandle( int iLOD = 0 ) const;

    // LOD �޽� ������ ���
    CWzdBase* GetWzdPtr( int iLOD );
    CWzdBase* GetValidWzdPtr();

    // �ִϸ��̼� ���ǵ� ����/���
    void SetAnimationSpeed( float fAnimationSpeed );
    float GetAnimationSpeed() const;

    // �ִϸ��̼� ���� �� ���
    int GetCountAnimationInfo() const;
    // �ִϸ��̼� ���� �߰�
    WZ3DA_API void AddAnimationInfo( const WzAnimationInfo& ai );
    // �ε����� �ִϸ��̼� ���� ���
    WzAnimationInfo* GetAnimationInfoByIndex( int iIndex );
    // ID�� �ִϸ��̼� ���� ���
    WZ3DA_API WzAnimationInfo* GetAnimationInfo( WzID wiAni );

    // �� ���� �� ���
    int GetCountBoneInfo() const;
    // �� ���� �߰�
    WZ3DA_API void AddBoneInfo( const WzBoneInfo& bi );
    // �ε����� �� ���� ���
    WzBoneInfo* GetBoneInfoByIndex( int iIndex );
    // ID�� �� ���� ���
    WZ3DA_API WzBoneInfo* GetBoneInfo( WzID wiBone );

    // ���� Ÿ�� �� ���
    int GetNumSubType() const;
    // ���� Ÿ�� ����
    WZ3DA_API CWzUnitSubType* CreateSubType();
    // ���� Ÿ�� ���
    CWzUnitSubType* GetSubType( int iSubType );

    // �޽� �׷� �̸� ���� �߰�
    WZ3DA_API void AddMeshGroupNameInfo( const MeshGroupNameInfo& info );
    // �޽� �׷� �̸� ���� ���
    MeshGroupNameInfo* GetMeshGroupNameInfo( int iIndex );
    // �̸����� �޽� �׷� �ε��� ���
    WZ3DA_API int GetMeshGroupIndexByName( const char* pszMeshGroupName );

    // ��ǻ��� ���� ��
    int GetDiffuseMapInfoCount() const;
    // ��ǻ��� ���� �߰�
    WZ3DA_API void AddDiffuseMapInfo( int iIndex, const WzMappingInfo& mi );
    // ��ǻ��� ���� ���
    WzMappingInfo* GetDiffuseMapInfo( int iIndex );

    // ����ŧ���� ���� �� ���
    int GetSpecularMapInfoCount() const;
    // ����ŧ���� ���� �߰�
    WZ3DA_API void AddSpecularMapInfo( int iIndex, const WzMappingInfo& mi );
    // ����ŧ���� ���� ���
    WzMappingInfo* GetSpecularMapInfo( int iIndex );

    // �븻�� ���� �� ���
    int GetNormalMapInfoCount() const;
    // �븻�� ���� �߰�
    WZ3DA_API void AddNormalMapInfo( int iIndex, const WzMappingInfo& mi );
    // �븻�� ���� ���
    WzMappingInfo* GetNormalMapInfo( int iIndex );

    // �۷ο�� ���� �� ���
    int GetGlowMapInfoCount() const;
    // �۷ο�� ���� �߰�
    WZ3DA_API void AddGlowMapInfo( int iIndex, const WzMappingInfo& mi );
    // �۷ο�� ���� ���
    WzMappingInfo* GetGlowMapInfo( int iIndex );

    // ���θ� ���� ��
    int GetInternalMapInfoCount() const;
    // ���θ� ���� �߰�
    WZ3DA_API void AddInternalMapInfo( int iIndex, const WzMappingInfo& mi );
    // ���θ� ���� ���
    WzMappingInfo* GetInternalMapInfo( int iIndex );

    // ���� ���� �߰�
    WZ3DA_API void AddFilterInfo( int iIndex, const WzMappingInfo& fi );
    // ���� ���� ���
    WzMappingInfo* GetFilterInfo( int iIndex );

    // �ؽ�ó �ִϸ��̼Ǹ� ���� ��
    int GetTexAniMapInfoCount() const;
    // �ؽ�ó �ִϸ��̼Ǹ� ���� �߰�
    WZ3DA_API void AddTexAniMapInfo( int iIndex, const WzTexAniMappingInfo& tami );
    // �ؽ�ó �ִϸ��̼Ǹ� ���� ���
    WzTexAniMappingInfo* GetTexAniMapInfo( int iIndex );

    // �ؽ�ó �ִϸ��̼Ǽ� ���� ��
    int GetCountTexAniSetInfo() const;
    // �ؽ�ó �ִϸ��̼Ǽ� ���� �߰�
    WZ3DA_API void AddTexAniSetInfo( const WzTexAniSetInfo& tasi );
    // �ε����� �ؽ�ó �ִϸ��̼Ǽ� ���� ���
    WzTexAniSetInfo* GetTexAniSetInfoByIndex( int iIndex );
    // ID�� �ؽ�ó �ִϸ��̼Ǽ� ���� ���
    WZ3DA_API WzTexAniSetInfo* GetTexAniSetInfo( WzID wiID );

    // �ٿ�� ���� ����
    void SetBoundingVolume( const BvAABB& bvAABB, const BvSphere& bvSphere );
    // �ٿ�� �ڽ� ���
    const BvAABB& GetBvAABB() const;
    // �ٿ�� �� ���
    const BvSphere& GetBvSphere() const;

    // �ٿ�� ������ ���� ���� �� ����
    void SetBaseBoneIndexForBV( int iBoneIndex );
    // �ٿ�� ������ ���� ���� �� ���
    int GetBaseBoneIndexForBV() const;
    
    // �ڵ� �ٿ���������� ���� ���
    BOOL GetAutoBvUpdate() const;
    // �ڵ� �ٿ���������� ���� ����
    void SetAutoBvUpdate( BOOL bFlag );
    // �ڵ� �ٿ����������ȣ ���
    int GetAutoBvBone() const;
    // �ڵ� �ٿ����������ȣ ����
    void SetAutoBvBone( int iBone );
    
    // �߰� �ٿ�� ���� �� ���
    int GetCountBVExtraInfo() const;
    // �߰� �ٿ�� ���� �߰�
    WZ3DA_API void AddBVExtraInfo( const WzBVExtraInfo& bei );
    // �ε����� �߰� �ٿ�� ���� ���
    WzBVExtraInfo* GetBVExtraInfoByIndex( int iIndex );
    
    // ��ġ/ȸ�� ������ ����
    WzUnitTransformOffset* GetTransFormOffset();
    
     // ���� �ε� ���� ���
    ENUM_LOAD_STATE GetLoadState();

    // ���� LOD ����?
    bool IsLodByForce() const;

private:
    // ���� �ʱ�ȭ
    void InitVariables();
    // ����
    void Release();

    // WzdBase ã��
    WZ3DA_API void FindWzdPtr( int iLOD );
    // Wzd �ε� ���� ����
    WZ3DA_API void CheckLoadState();

    // Serialize Header
    BOOL SerializeHeader( CWzArchive* pArchive );

    // Serialize Default Info
    BOOL SerializeDefaultInfo( CWzArchive* pArchive );
    // Serialize LOD info
    BOOL SerializeLODInfo( CWzArchive* pArchive );
    // xxx: ���� ȣȯ (Morph Mesh Info)
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
    // xxx: ���� ȣȯ (IK Info)
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
    // �ϴ� ���� ����Ƽ ���� ���� ����
    CWzdBase* pWzd = GetWzdPtr( 0 );
    if( !pWzd && m_iNumWzd > 1 )
    {
        // ������ ���� ����Ƽ ���� ������
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
