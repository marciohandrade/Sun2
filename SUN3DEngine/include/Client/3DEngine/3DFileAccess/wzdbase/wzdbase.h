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

    xxx: 여기에 하위 클래스에서만 쓰이는 함수들도 전부 다 있다.
         이게 과연 좋은 방법인지는 모르겠지만, 외부 코드에서
         CWzdBase 포인터를 가지고 하위 클래스들(ex : CWzdAnimation)
         함수를 호출하고 있기 때문에, 당장 뺄 수도 없다.

         * 본(Bone)하고 노드(Node)하고 같은 의미
*/
class CWzdBase
{
public:
    // 생성자/소멸자
    CWzdBase();
    virtual ~CWzdBase() = 0;

    // 로드
    virtual BOOL Create( const char* lpszFileName, BYTE* pbyBuffer, int iSize );
    virtual BOOL Create( const char* lpszFileName, DA_FILE dfFile );
    virtual BOOL Create( const char* lpszFileName );

    // 저장
    virtual BOOL Save( const char* lpszFileName, BOOL bSaveText = FALSE );
    virtual BOOL Save( const char* lpszFileName, DA_FILE dfFile, BOOL bSaveText = FALSE );

    // 파괴
    virtual void Destroy();

    // -------- CWzdAnimation 관련 --------

    // 본 할당
    virtual void AllocBones( WORD wNumNode ) {}
    // 본 정보 설정
    virtual void SetBoneInfo( WORD wNode, int iParent, const char* lpszName = NULL ) {}

    // 본 수 얻기
    virtual WORD GetNumNode() const { return 0; }

    // 본 리스트 얻기
    virtual WzNode* GetBonesList() const { return NULL; }
    // 추가 본 정보 리스트 얻기
    virtual WzNodeExtra* GetBoneExtraInfoList() const { return NULL; }

    // 본 구조 비교 (완벽하게 일치해야 함)
    virtual BOOL CompareBoneStructure( const CWzdBase* pDest ) { return FALSE; }
    // 본 리스트 비교 (개수가 틀릴 수 있음)
    virtual BOOL CompareBoneList( const CWzdBase* pDest ) { return FALSE; }

    // 키 프레임 할당
    virtual void AllocKeyFrames() {}
    // 키 변환 할당
    virtual void AllocKeyTransforms( WORD wNode, WORD wNumFrame ) {}

    // 실제 프레임 번호 설정
    virtual void SetRealFrame( WORD wNode, WORD wKeyFrame, WORD wRealFrame ) {}

    // 키 변환 정보 설정
    virtual void SetKeyTransformInfo( WORD wNode, 
                                      WORD wFrame, 
                                      const WzVector& wvPos, 
                                      const WzQuaternion& wqRot ) {}

    // 키 프레임 리스트 얻기
    virtual WzKeyFrame* GetKeyFramesList() const { return NULL; }

    // 총 프레임 수 얻기
    virtual WORD GetNumFrame() const { return 0; }
    // 해당 본 프레임 수 얻기
    virtual WORD GetNumBoneFrame( WORD wNode ) const { return 0; }

    // 해당 프레임에서 전체 본 변환 얻기
    virtual void CreateTransformArrayByFrame( WzTransform* pOutTform, float fFrame ) {}
    // 해당 프레임에서 해당 본 변환 얻기
    virtual void CreateTransformByFrame( WzTransform* pOutTform, WORD wNode, float fFrame ) {}

    // 주어진 본 변환으로 최종 본 행렬 구하기
    virtual void CalcTransformMatrixT( WzMatrix* pInOutMat, 
                                       const WzTransform* pInTform, 
                                       const WzTransformParam* pwtp ) {}

    // 애니메이션 보간 (A -> B)
    virtual void AnimationSlerp( WzTransform* pOutTform, 
                                 const WzTransform* pwtA, 
                                 const WzTransform* pwtB, 
                                 float fWeight ) {}

    // 애니메이션 스피드 설정/얻기
    virtual void SetAnimationSpeedRate( float fRate ) {}
    virtual float GetAnimationSpeedRate() const { return 1.0f; }

    // -------- CWzdMeshConstructor 관련 --------

    // 메시 할당
    virtual BOOL AllocForMesh() { return FALSE; }

    // 삼각형 추가
    virtual void AddTriangle( const char* lpszTexture, 
                              const WzSkinningInfo wsi[3],
                              const WzVector wvPosWorld[3],
                              const WzVector wvNormal[3],
                              const WzUVVert wuvVert[3],
                              const char* lpszSpecularTexture, 
                              const DWORD dwColor[3], 
                              const DWORD dwColor2[3] ) {}

    // 버텍스 수 얻기
    virtual int GetNumberOfVertex() const { return 0; }
    // 버텍스 리스트 얻기
    virtual WzMeshVertex* GetVertexList() const { return NULL; }

    // 전체 메시 그룹 수 얻기
    virtual int GetNumberOfMeshGroup() const { return 0; }
    virtual int GetMaxMeshGroup() const { return 0; }

    // 메시 그룹 정보 얻기
    virtual WzMeshGroup* GetMeshGroupInfo( int iMeshGroup ) const { return NULL; }

    // 전체 버텍스팩 수 얻기
    virtual DWORD GetTotalVertPack() const { return 0; }

    // 블랜딩 수 얻기
    virtual int GetBlendedCount() const { return 0; }
    // 스키닝 정보 얻기
    virtual WzSkinningInfo* GetSkinningInfo() const { return NULL; }

    // -------- CWzdMesh 관련 --------

    // 기본 본 행렬로 최종 본 행렬 구하기
    virtual void CalcTransformMatrixD( WzMatrix* pInOutMat, 
                                       const WzTransformParam* pwtp ) {}

    // 스키닝 행렬 구하기
    virtual void CalcSkinMatrix( WzMatrix* pOutMat, const WzMatrix* pInTformMat ) {}

    // 기본 본 행렬의 역행렬 얻기 (스키닝 행렬 구할때 필요)
    virtual const WzMatrix* GetInverseOfDefaultTransform() const { return NULL; }

    // 실루엣 생성
    virtual void CreateSilhouette( CWzSilhouette* pwsSilhouette, 
                                   const WzVector& wvRay, 
                                   const WzVector* pwvTransformedVertex ) {}

    // 실루엣 파괴
    virtual void DestroySilhouette( CWzSilhouette* pwsSilhouette ) {}

    // 셰도우 볼륨 생성
    virtual void CreateShadowVolume( CWzShadowVolume* pwsvShadowVolume, 
                                     const WzVector& wvRay, 
                                     const WzVector* pwvTransformedVertex,
                                     DWORD dwOption = 0 ) {}

    // 셰도우 볼륨 파괴
    virtual void DestroyShadowVolume( CWzShadowVolume* pwsvShadowVolume ) {}

    // 버텍스 셰이더 상수 정보 생성
    virtual BOOL CreateVertexShaderConstantInfo( int iMeshGroup, int iNumGroup ) { return FALSE; }
    // 버텍스 셰이더 상수 정보 채우기
    virtual void FillVertexShaderConstantInfo( int iMeshGroup, int iGroup, int iCountBone, int* piBoneList ) {}

    // 버텍스 버퍼 설정/얻기
    virtual void SetOwnVertexBuffer( int iMeshGroup, HANDLE hOwnVertexBuffer ) {}
    virtual HANDLE GetOwnVertexBuffer( int iMeshGroup ) const { return INVALID_HANDLE_VALUE; }

    // 인덱스 버퍼 설정/얻기
    virtual void SetOwnIndexBuffer( int iMeshGroup, HANDLE hOwnIndexBuffer ) {}
    virtual HANDLE GetOwnIndexBuffer( int iMeshGroup ) const { return INVALID_HANDLE_VALUE; }

    // 바운딩 박스 얻기
    virtual const BvAABB& GetBvAABB() const { static BvAABB dummy; return dummy; }
    // 바운딩 구 얻기
    virtual const BvSphere& GetBvSphere() const { static BvSphere dummy; return dummy; }

    // -------- 기타 --------

    // ID 얻기/검사
    virtual void GetIdentity( BYTE byIdentity[4] );
    virtual BOOL CheckIdentity( const BYTE* byIdentity ) const;

    // 파일명 얻기
    const char* GetFileName() const;
    // 버전 얻기
    WORD GetVersion() const;

    // 플래그 설정/해제/검사
    void SetFlag( DWORD dwFlag );
    void UnsetFlag( DWORD dwFlag );
    BOOL CheckFlag( DWORD dwFlag ) const;

    // DiscAccess 설정
    static void SetDiscAccess( CDiscAccess* pDiscAccess );
    // DiscAccess 존재 여부
    static CDiscAccess* CheckDiscAccessExistence();

    // 스키닝 인덱스 -> 본 인덱스
    static int SkinningIndexToBoneIndex( int iIndex );
    // 본 인덱스 -> 스키닝 인덱스
    static int BoneIndexToSkinningIndex( int iIndex );

protected:
    // Serialize
    virtual BOOL Serialize( CWzArchive* pArchive );

private:
    // 변수 초기화 (가상 함수로 선언하지 말 것)
    void InitVariables();

    // 메모리 해제
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
