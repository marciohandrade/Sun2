// wzdmesh.h

#ifndef _3DFILEACCESS_WZDMESH_H_
#define _3DFILEACCESS_WZDMESH_H_

#include "wzdmeshconstructor.h"

//------------------------------------------------------------------------------
/**
    @class CWzdMesh
*/
class CWzdMesh : public CWzdMeshConstructor  
{
public:
    // ������/�Ҹ���
    CWzdMesh();
    virtual ~CWzdMesh();

    // �ı�
    virtual void Destroy();

    // �⺻ �� ��ķ� ���� �� ��� ���ϱ�
    virtual void CalcTransformMatrixD( WzMatrix* pInOutMat, 
                                       const WzTransformParam* pwtp );

    // ��Ű�� ��� ���ϱ�
    virtual void CalcSkinMatrix( WzMatrix* pOutMat, const WzMatrix* pInTformMat );

    // �⺻ �� ����� ����� ��� (��Ű�� ��� ���Ҷ� �ʿ�)
    virtual const WzMatrix* GetInverseOfDefaultTransform() const;

    // �Ƿ翧 ����
    virtual void CreateSilhouette( CWzSilhouette* pwsSilhouette, 
                                   const WzVector& wvRay, 
                                   const WzVector* pwvTransformedVertex );

    // �Ƿ翧 �ı�
    virtual void DestroySilhouette( CWzSilhouette* pwsSilhouette );

    // �ε��� ���� ����
    virtual void CreateShadowVolume( CWzShadowVolume* pwsvShadowVolume, 
                                     const WzVector& wvRay, 
                                     const WzVector* pwvTransformedVertex,
                                     DWORD dwOption = 0 );

    // �ε��� ���� �ı�
    virtual void DestroyShadowVolume( CWzShadowVolume* pwsvShadowVolume );

    // ���ؽ� ���̴� ��� ���� ����
    virtual BOOL CreateVertexShaderConstantInfo( int iMeshGroup, int iNumGroup );
    // ���ؽ� ���̴� ��� ���� ä���
    virtual void FillVertexShaderConstantInfo( int iMeshGroup, int iGroup, int iCountBone, int* piBoneList );

    // ���ؽ� ���� ����/���
    virtual void SetOwnVertexBuffer( int iMeshGroup, HANDLE hOwnVertexBuffer );
    virtual HANDLE GetOwnVertexBuffer( int iMeshGroup ) const;

    // �ε��� ���� ����/���
    virtual void SetOwnIndexBuffer( int iMeshGroup, HANDLE hOwnIndexBuffer );
    virtual HANDLE GetOwnIndexBuffer( int iMeshGroup ) const;

    // �ٿ�� �ڽ� ���
    virtual const BvAABB& GetBvAABB() const;
    // �ٿ�� �� ���
    virtual const BvSphere& GetBvSphere() const;

    // ID ���/�˻�
    virtual void GetIdentity( BYTE byIdentity[4] );
    virtual BOOL CheckIdentity( const BYTE* byIdentity ) const;

protected:
    // Serialize
    virtual BOOL Serialize( CWzArchive* pArchive );

    // ���ؽ� ����Ʈ Serialize
    BOOL SerializeVertexList( CWzArchive* pArchive );
    // �޽� �׷� Serialize
    BOOL SerializeMeshGroup( CWzArchive* pArchive );
    // ��Ű�� ���� Serialize
    BOOL SerializeSkinningInfo( CWzArchive* pArchive );

    // ������ �а� ��ȯ
    BOOL ReadOldVersionAndConvert( CWzArchive* pArchive );

    // �ٿ�� ���� ����
    void UpdateBoundingVolume();

private:
    // ���� �ʱ�ȭ (���� �Լ��� �������� �� ��)
    void InitVariables();

    // �޸� ����
    void Release();

    // �⺻ �� ��� ����
    void ReleaseDefTransformMat();
    // �⺻ �� ��� �ʱ�ȭ
    void InitDefTransformMat();

    // �⺻ �� ��� ���
    void CalcDefBoneTransformMatrix( WORD wNode, WzMatrix* pInOutMat );

    // �⺻ �� ��ķ� ���� �� ��� ���ϱ�
    void CalcBoneTransformMatrixD( WORD wNode, 
                                   WzMatrix* pInOutMat, 
                                   const WzTransformParam* pwtp = NULL );

protected:
    friend class CWzdMeshSorter;

    WzMatrix* m_pDefBoneMat;
    WzMatrix* m_pInvDefTformMat;

    BvAABB m_bvAABB;
    BvSphere m_bvSphere;
};

//------------------------------------------------------------------------------
/**
*/
inline void CWzdMesh::GetIdentity( BYTE byIdentity[4] )
{
    WzAssert( byIdentity );

    memcpy( byIdentity, IDSTR_WZDMESH, 4 );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzdMesh::CheckIdentity( const BYTE* byIdentity ) const
{
    WzAssert( byIdentity );

    return ( memcmp( IDSTR_WZDMESH, byIdentity, 4 ) == 0 );
}

//------------------------------------------------------------------------------
/**
*/
inline const WzMatrix* CWzdMesh::GetInverseOfDefaultTransform() const
{
    return m_pInvDefTformMat; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzdMesh::SetOwnVertexBuffer( int iMeshGroup, HANDLE hOwnVertexBuffer )
{
    WzAssert( iMeshGroup >= 0 && iMeshGroup < m_iNumMeshGroup );
    WzAssert( m_pwmgMeshGroup );

    m_pwmgMeshGroup[iMeshGroup].m_hOwnVertexBuffer = hOwnVertexBuffer; 
}

//------------------------------------------------------------------------------
/**
*/
inline HANDLE CWzdMesh::GetOwnVertexBuffer( int iMeshGroup ) const
{
    WzAssert( iMeshGroup >= 0 && iMeshGroup < m_iNumMeshGroup );
    
    if( iMeshGroup >= 0 && iMeshGroup < m_iNumMeshGroup )
    {
        WzAssert( m_pwmgMeshGroup );

        return m_pwmgMeshGroup[iMeshGroup].m_hOwnVertexBuffer;
    }

    return INVALID_HANDLE_VALUE;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzdMesh::SetOwnIndexBuffer( int iMeshGroup, HANDLE hOwnIndexBuffer )
{
    WzAssert( iMeshGroup >= 0 && iMeshGroup < m_iNumMeshGroup );
    WzAssert( m_pwmgMeshGroup );

    m_pwmgMeshGroup[iMeshGroup].m_hOwnIndexBuffer = hOwnIndexBuffer; 
}

//------------------------------------------------------------------------------
/**
*/
inline HANDLE CWzdMesh::GetOwnIndexBuffer( int iMeshGroup ) const
{ 
    WzAssert( iMeshGroup >= 0 && iMeshGroup < m_iNumMeshGroup );
    
    if( iMeshGroup >= 0 && iMeshGroup < m_iNumMeshGroup )
    {
        WzAssert( m_pwmgMeshGroup );

        return m_pwmgMeshGroup[iMeshGroup].m_hOwnIndexBuffer;
    }

    return INVALID_HANDLE_VALUE;
}

//------------------------------------------------------------------------------
/**
*/
inline const BvAABB& CWzdMesh::GetBvAABB() const
{
    return m_bvAABB;
}

//------------------------------------------------------------------------------
/**
*/
inline const BvSphere& CWzdMesh::GetBvSphere() const
{
    return m_bvSphere;
}

#endif // _3DFILEACCESS_WZDMESH_H_
