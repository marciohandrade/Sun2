// wzdbase.h

#ifndef _3DFILEACCESS_WZDBASE_H_
#define _3DFILEACCESS_WZDBASE_H_

#include "wzdtypedef.h"
#include "wzdshareinfo.h"
#include <programcommon/discaccess.h>
#include <client/3dengine/common/wzbvtype.h>

class CWzArchive;
class CWzSilhouette;
class CWzShadowVolume;

//------------------------------------------------------------------------------
/**
    @class CWzdBase

    xxx: ���⿡ ���� Ŭ���������� ���̴� �Լ��鵵 ���� �� �ִ�.
         �̰� ���� ���� ��������� �𸣰�����, �ܺ� �ڵ忡��
         CWzdBase �����͸� ������ ���� Ŭ������(ex : CWzdAnimation)
         �Լ��� ȣ���ϰ� �ֱ� ������, ���� �� ���� ����.

         * ��(Bone)�ϰ� ���(Node)�ϰ� ���� �ǹ�
*/
class CWzdBase
{
public:
    // ������/�Ҹ���
    CWzdBase();
    virtual ~CWzdBase() = 0;

    // �ε�
    virtual BOOL Create( const char* lpszFileName, BYTE* pbyBuffer, int iSize );
    virtual BOOL Create( const char* lpszFileName, DA_FILE dfFile );
    virtual BOOL Create( const char* lpszFileName );

    // ����
    virtual BOOL Save( const char* lpszFileName, BOOL bSaveText = FALSE );
    virtual BOOL Save( const char* lpszFileName, DA_FILE dfFile, BOOL bSaveText = FALSE );

    // �ı�
    virtual void Destroy();

    // -------- CWzdAnimation ���� --------

    // �� �Ҵ�
    virtual void AllocBones( WORD wNumNode ) {}
    // �� ���� ����
    virtual void SetBoneInfo( WORD wNode, int iParent, const char* lpszName = NULL ) {}

    // �� �� ���
    virtual WORD GetNumNode() const { return 0; }

    // �� ����Ʈ ���
    virtual WzNode* GetBonesList() const { return NULL; }
    // �߰� �� ���� ����Ʈ ���
    virtual WzNodeExtra* GetBoneExtraInfoList() const { return NULL; }

    // �� ���� �� (�Ϻ��ϰ� ��ġ�ؾ� ��)
    virtual BOOL CompareBoneStructure( const CWzdBase* pDest ) { return FALSE; }
    // �� ����Ʈ �� (������ Ʋ�� �� ����)
    virtual BOOL CompareBoneList( const CWzdBase* pDest ) { return FALSE; }

    // Ű ������ �Ҵ�
    virtual void AllocKeyFrames() {}
    // Ű ��ȯ �Ҵ�
    virtual void AllocKeyTransforms( WORD wNode, WORD wNumFrame ) {}

    // ���� ������ ��ȣ ����
    virtual void SetRealFrame( WORD wNode, WORD wKeyFrame, WORD wRealFrame ) {}

    // Ű ��ȯ ���� ����
    virtual void SetKeyTransformInfo( WORD wNode, 
                                      WORD wFrame, 
                                      const WzVector& wvPos, 
                                      const WzQuaternion& wqRot ) {}

    // Ű ������ ����Ʈ ���
    virtual WzKeyFrame* GetKeyFramesList() const { return NULL; }

    // �� ������ �� ���
    virtual WORD GetNumFrame() const { return 0; }
    // �ش� �� ������ �� ���
    virtual WORD GetNumBoneFrame( WORD wNode ) const { return 0; }

    // �ش� �����ӿ��� ��ü �� ��ȯ ���
    virtual void CreateTransformArrayByFrame( WzTransform* pOutTform, float fFrame ) {}
    // �ش� �����ӿ��� �ش� �� ��ȯ ���
    virtual void CreateTransformByFrame( WzTransform* pOutTform, WORD wNode, float fFrame ) {}

    // �־��� �� ��ȯ���� ���� �� ��� ���ϱ�
    virtual void CalcTransformMatrixT( WzMatrix* pInOutMat, 
                                       const WzTransform* pInTform, 
                                       const WzTransformParam* pwtp ) {}

    // �ִϸ��̼� ���� (A -> B)
    virtual void AnimationSlerp( WzTransform* pOutTform, 
                                 const WzTransform* pwtA, 
                                 const WzTransform* pwtB, 
                                 float fWeight ) {}

    // �ִϸ��̼� ���ǵ� ����/���
    virtual void SetAnimationSpeedRate( float fRate ) {}
    virtual float GetAnimationSpeedRate() const { return 1.0f; }

    // -------- CWzdMeshConstructor ���� --------

    // �޽� �Ҵ�
    virtual BOOL AllocForMesh() { return FALSE; }

    // �ﰢ�� �߰�
    virtual void AddTriangle( const char* lpszTexture, 
                              const WzSkinningInfo wsi[3],
                              const WzVector wvPosWorld[3],
                              const WzVector wvNormal[3],
                              const WzUVVert wuvVert[3],
                              const char* lpszSpecularTexture, 
                              const DWORD dwColor[3], 
                              const DWORD dwColor2[3] ) {}

    // ���ؽ� �� ���
    virtual int GetNumberOfVertex() const { return 0; }
    // ���ؽ� ����Ʈ ���
    virtual WzMeshVertex* GetVertexList() const { return NULL; }

    // ��ü �޽� �׷� �� ���
    virtual int GetNumberOfMeshGroup() const { return 0; }
    virtual int GetMaxMeshGroup() const { return 0; }

    // �޽� �׷� ���� ���
    virtual WzMeshGroup* GetMeshGroupInfo( int iMeshGroup ) const { return NULL; }

    // ��ü ���ؽ��� �� ���
    virtual DWORD GetTotalVertPack() const { return 0; }

    // ���� �� ���
    virtual int GetBlendedCount() const { return 0; }
    // ��Ű�� ���� ���
    virtual WzSkinningInfo* GetSkinningInfo() const { return NULL; }

    // -------- CWzdMesh ���� --------

    // �⺻ �� ��ķ� ���� �� ��� ���ϱ�
    virtual void CalcTransformMatrixD( WzMatrix* pInOutMat, 
                                       const WzTransformParam* pwtp ) {}

    // ��Ű�� ��� ���ϱ�
    virtual void CalcSkinMatrix( WzMatrix* pOutMat, const WzMatrix* pInTformMat ) {}

    // �⺻ �� ����� ����� ��� (��Ű�� ��� ���Ҷ� �ʿ�)
    virtual const WzMatrix* GetInverseOfDefaultTransform() const { return NULL; }

    // �Ƿ翧 ����
    virtual void CreateSilhouette( CWzSilhouette* pwsSilhouette, 
                                   const WzVector& wvRay, 
                                   const WzVector* pwvTransformedVertex ) {}

    // �Ƿ翧 �ı�
    virtual void DestroySilhouette( CWzSilhouette* pwsSilhouette ) {}

    // �ε��� ���� ����
    virtual void CreateShadowVolume( CWzShadowVolume* pwsvShadowVolume, 
                                     const WzVector& wvRay, 
                                     const WzVector* pwvTransformedVertex,
                                     DWORD dwOption = 0 ) {}

    // �ε��� ���� �ı�
    virtual void DestroyShadowVolume( CWzShadowVolume* pwsvShadowVolume ) {}

    // ���ؽ� ���̴� ��� ���� ����
    virtual BOOL CreateVertexShaderConstantInfo( int iMeshGroup, int iNumGroup ) { return FALSE; }
    // ���ؽ� ���̴� ��� ���� ä���
    virtual void FillVertexShaderConstantInfo( int iMeshGroup, int iGroup, int iCountBone, int* piBoneList ) {}

    // ���ؽ� ���� ����/���
    virtual void SetOwnVertexBuffer( int iMeshGroup, HANDLE hOwnVertexBuffer ) {}
    virtual HANDLE GetOwnVertexBuffer( int iMeshGroup ) const { return INVALID_HANDLE_VALUE; }

    // �ε��� ���� ����/���
    virtual void SetOwnIndexBuffer( int iMeshGroup, HANDLE hOwnIndexBuffer ) {}
    virtual HANDLE GetOwnIndexBuffer( int iMeshGroup ) const { return INVALID_HANDLE_VALUE; }

    // �ٿ�� �ڽ� ���
    virtual const BvAABB& GetBvAABB() const { static BvAABB dummy; return dummy; }
    // �ٿ�� �� ���
    virtual const BvSphere& GetBvSphere() const { static BvSphere dummy; return dummy; }

    // -------- ��Ÿ --------

    // ID ���/�˻�
    virtual void GetIdentity( BYTE byIdentity[4] );
    virtual BOOL CheckIdentity( const BYTE* byIdentity ) const;

    // ���ϸ� ���
    const char* GetFileName() const;
    // ���� ���
    WORD GetVersion() const;

    // �÷��� ����/����/�˻�
    void SetFlag( DWORD dwFlag );
    void UnsetFlag( DWORD dwFlag );
    BOOL CheckFlag( DWORD dwFlag ) const;

    // DiscAccess ����
    static void SetDiscAccess( CDiscAccess* pDiscAccess );
    // DiscAccess ���� ����
    static CDiscAccess* CheckDiscAccessExistence();

    // ��Ű�� �ε��� -> �� �ε���
    static int SkinningIndexToBoneIndex( int iIndex );
    // �� �ε��� -> ��Ű�� �ε���
    static int BoneIndexToSkinningIndex( int iIndex );

protected:
    // Serialize
    virtual BOOL Serialize( CWzArchive* pArchive );

private:
    // ���� �ʱ�ȭ (���� �Լ��� �������� �� ��)
    void InitVariables();

    // �޸� ����
    void Release();

protected:
    static CDiscAccess* s_pDiscAccess;

    char m_szFileName[MAX_PATH];

    WORD m_wVersion;
    DWORD m_dwFlags;

    BOOL m_bSaveText;
};

//------------------------------------------------------------------------------
/**
*/
inline void CWzdBase::GetIdentity( BYTE byIdentity[4] )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzdBase::CheckIdentity( const BYTE* byIdentity ) const
{
    return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
inline const char* CWzdBase::GetFileName() const
{
    return &m_szFileName[0];
}

//------------------------------------------------------------------------------
/**
*/
inline WORD CWzdBase::GetVersion() const
{
    return m_wVersion; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzdBase::SetFlag( DWORD dwFlag )
{
    m_dwFlags |= dwFlag; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzdBase::UnsetFlag( DWORD dwFlag )
{
    m_dwFlags &= ~dwFlag;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzdBase::CheckFlag( DWORD dwFlag ) const
{ 
    return ( ( m_dwFlags & dwFlag ) == dwFlag ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzdBase::SetDiscAccess( CDiscAccess* pDiscAccess )
{
    s_pDiscAccess = pDiscAccess;
}

//------------------------------------------------------------------------------
/**
*/
inline CDiscAccess* CWzdBase::CheckDiscAccessExistence()
{
    if( !s_pDiscAccess )
    {
        static CDiscAccess s_discAccess;

        s_pDiscAccess = &s_discAccess;
    }

    return s_pDiscAccess;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzdBase::SkinningIndexToBoneIndex( int iIndex )
{ 
    return ( -iIndex - 1 );
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzdBase::BoneIndexToSkinningIndex( int iIndex )
{ 
    return ( -iIndex - 1 );
}

#endif // _3DFILEACCESS_WZDBASE_H_
