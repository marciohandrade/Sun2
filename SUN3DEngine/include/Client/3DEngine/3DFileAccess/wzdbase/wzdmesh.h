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
    // 생성자/소멸자
    CWzdMesh();
    virtual ~CWzdMesh();

    // 파괴
    virtual void Destroy();

    // 기본 본 행렬로 최종 본 행렬 구하기
    virtual void CalcTransformMatrixD( WzMatrix* pInOutMat, 
                                       const WzTransformParam* pwtp );

    // 스키닝 행렬 구하기
    virtual void CalcSkinMatrix( WzMatrix* pOutMat, const WzMatrix* pInTformMat );

    // 기본 본 행렬의 역행렬 얻기 (스키닝 행렬 구할때 필요)
    virtual const WzMatrix* GetInverseOfDefaultTransform() const;

    // 실루엣 생성
    virtual void CreateSilhouette( CWzSilhouette* pwsSilhouette, 
                                   const WzVector& wvRay, 
                                   const WzVector* pwvTransformedVertex );

    // 실루엣 파괴
    virtual void DestroySilhouette( CWzSilhouette* pwsSilhouette );

    // 셰도우 볼륨 생성
    virtual void CreateShadowVolume( CWzShadowVolume* pwsvShadowVolume, 
                                     const WzVector& wvRay, 
                                     const WzVector* pwvTransformedVertex,
                                     DWORD dwOption = 0 );

    // 셰도우 볼륨 파괴
    virtual void DestroyShadowVolume( CWzShadowVolume* pwsvShadowVolume );

    // 버텍스 셰이더 상수 정보 생성
    virtual BOOL CreateVertexShaderConstantInfo( int iMeshGroup, int iNumGroup );
    // 버텍스 셰이더 상수 정보 채우기
    virtual void FillVertexShaderConstantInfo( int iMeshGroup, int iGroup, int iCountBone, int* piBoneList );

    // 버텍스 버퍼 설정/얻기
    virtual void SetOwnVertexBuffer( int iMeshGroup, HANDLE hOwnVertexBuffer );
    virtual HANDLE GetOwnVertexBuffer( int iMeshGroup ) const;

    // 인덱스 버퍼 설정/얻기
    virtual void SetOwnIndexBuffer( int iMeshGroup, HANDLE hOwnIndexBuffer );
    virtual HANDLE GetOwnIndexBuffer( int iMeshGroup ) const;

    // 바운딩 박스 얻기
    virtual const BvAABB& GetBvAABB() const;
    // 바운딩 구 얻기
    virtual const BvSphere& GetBvSphere() const;

    // ID 얻기/검사
    virtual void GetIdentity( BYTE byIdentity[4] );
    virtual BOOL CheckIdentity( const BYTE* byIdentity ) const;

protected:
    // Serialize
    virtual BOOL Serialize( CWzArchive* pArchive );

    // 버텍스 리스트 Serialize
    BOOL SerializeVertexList( CWzArchive* pArchive );
    // 메시 그룹 Serialize
    BOOL SerializeMeshGroup( CWzArchive* pArchive );
    // 스키닝 정보 Serialize
    BOOL SerializeSkinningInfo( CWzArchive* pArchive );

    // 구버전 읽고 변환
    BOOL ReadOldVersionAndConvert( CWzArchive* pArchive );

    // 바운딩 볼륨 갱신
    void UpdateBoundingVolume();

private:
    // 변수 초기화 (가상 함수로 선언하지 말 것)
    void InitVariables();

    // 메모리 해제
    void Release();

    // 기본 본 행렬 제거
    void ReleaseDefTransformMat();
    // 기본 본 행렬 초기화
    void InitDefTransformMat();

    // 기본 본 행렬 계산
    void CalcDefBoneTransformMatrix( WORD wNode, WzMatrix* pInOutMat );

    // 기본 본 행렬로 최종 본 행렬 구하기
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
