// wzdanimation.h

#ifndef _3DFILEACCESS_WZDANIMATION_H_
#define _3DFILEACCESS_WZDANIMATION_H_

#include "wzdbase.h"

//------------------------------------------------------------------------------
/**
    @class CWzdAnimation

    * 본(Bone)하고 노드(Node)하고 같은 의미
*/
class CWzdAnimation : public CWzdBase  
{
public:
    // 생성자/소멸자
    CWzdAnimation();
    virtual ~CWzdAnimation();

    // 파괴
    virtual void Destroy();

    // 본 할당
    virtual void AllocBones( WORD wNumNode );
    // 본 정보 설정
    virtual void SetBoneInfo( WORD wNode, int iParent, const char* lpszName = NULL );

    // 본 수 얻기
    virtual WORD GetNumNode() const;

    // 본 리스트 얻기
    virtual WzNode* GetBonesList() const;
    // 추가 본 정보 리스트 얻기
    virtual WzNodeExtra* GetBoneExtraInfoList() const;

    // 본 구조 비교 (완벽하게 일치해야 함)
    virtual BOOL CompareBoneStructure( const CWzdBase* pDest );
    // 본 리스트 비교 (개수가 틀릴 수 있음)
    virtual BOOL CompareBoneList( const CWzdBase* pDest );

    // 키 프레임 할당
    virtual void AllocKeyFrames();
    // 키 변환 할당
    virtual void AllocKeyTransforms( WORD wNode, WORD wNumFrame );

    // 실제 프레임 번호 설정
    virtual void SetRealFrame( WORD wNode, WORD wKeyFrame, WORD wRealFrame );

    // 키 변환 정보 설정
    virtual void SetKeyTransformInfo( WORD wNode, 
                                      WORD wFrame, 
                                      const WzVector& wvPos, 
                                      const WzQuaternion& wqRot );

    // 키 프레임 리스트 얻기
    virtual WzKeyFrame* GetKeyFramesList() const;

    // 총 프레임 수 얻기
    virtual WORD GetNumFrame() const;
    // 해당 본 프레임 수 얻기
    virtual WORD GetNumBoneFrame( WORD wNode ) const;

    // 해당 프레임에서 전체 본 변환 얻기
    virtual void CreateTransformArrayByFrame( WzTransform* pOutTform, float fFrame );
    // 해당 프레임에서 해당 본 변환 얻기
    virtual void CreateTransformByFrame( WzTransform* pOutTform, WORD wNode, float fFrame );

    // 주어진 본 변환으로 최종 본 행렬 구하기
    virtual void CalcTransformMatrixT( WzMatrix* pInOutMat, 
                                       const WzTransform* pInTform, 
                                       const WzTransformParam* pwtp );

    // 애니메이션 보간 (A -> B)
    virtual void AnimationSlerp( WzTransform* pOutTform, 
                                 const WzTransform* pwtA, 
                                 const WzTransform* pwtB, 
                                 float fWeight );

    // 애니메이션 스피드 설정/얻기
    virtual void SetAnimationSpeedRate( float fRate );
    virtual float GetAnimationSpeedRate() const;

    // ID 얻기/검사
    virtual void GetIdentity( BYTE byIdentity[4] );
    virtual BOOL CheckIdentity( const BYTE* byIdentity ) const;

protected:
    // Serialize
    virtual BOOL Serialize( CWzArchive* pArchive );

    // Save (텍스트 버전)
    BOOL Save_Text( CWzArchive* pArchive );

private:
    // 변수 초기화 (가상 함수로 선언하지 말 것)
    void InitVariables();

    // 메모리 해제
    void Release();

    // 주어진 본 변환으로 최종 본 행렬 구하기
    void CalcBoneTransformMatrixT( WORD wNode, 
                                   WzMatrix* pInOutMat, 
                                   const WzTransform* pInTform, 
                                   const WzTransformParam* pwtp = NULL );

protected:
    WORD m_wNumNode;

    WzNode* m_pNodeList;
    WzNodeExtra* m_pNodeExList;
    WzKeyFrame* m_pKeyFrameList;
    bool* m_pbNodeTMCalc;

    float m_fAniSpeedRate;
};

//------------------------------------------------------------------------------
/**
*/
inline void CWzdAnimation::GetIdentity( BYTE byIdentity[4] )
{
    WzAssert( byIdentity );

    memcpy( byIdentity, IDSTR_WZDANI, 4 );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzdAnimation::CheckIdentity( const BYTE* byIdentity ) const
{
    WzAssert( byIdentity );

    return ( memcmp( IDSTR_WZDANI, byIdentity, 4 ) == 0  );
}

//------------------------------------------------------------------------------
/**
*/
inline WORD CWzdAnimation::GetNumNode() const
{ 
    return m_wNumNode; 
}

//------------------------------------------------------------------------------
/**
*/
inline WzNode* CWzdAnimation::GetBonesList() const
{
    return m_pNodeList;
}

//------------------------------------------------------------------------------
/**
*/
inline WzNodeExtra* CWzdAnimation::GetBoneExtraInfoList() const
{
    return m_pNodeExList; 
}

//------------------------------------------------------------------------------
/**
*/
inline WzKeyFrame* CWzdAnimation::GetKeyFramesList() const
{
    return m_pKeyFrameList;
}

//------------------------------------------------------------------------------
/**
*/
inline WORD CWzdAnimation::GetNumFrame() const
{
    WzAssert( m_pKeyFrameList );

    // 0번 본의 마지막 프레임의 Key(실제 프레임 번호) + 1
    // (모든 본의 마지막 실제 프레임 번호는 같다)
    return m_pKeyFrameList[0].m_pKeyTransforms[m_pKeyFrameList[0].m_wNumKey - 1].m_wKey + 1;
}

//------------------------------------------------------------------------------
/**
*/
inline WORD CWzdAnimation::GetNumBoneFrame( WORD wNode ) const 
{
    WzAssert( wNode < m_wNumNode );
    WzAssert( m_pKeyFrameList );

    return m_pKeyFrameList[wNode].m_wNumKey;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzdAnimation::SetAnimationSpeedRate( float fRate )
{
    m_fAniSpeedRate = fRate; 
}

//------------------------------------------------------------------------------
/**
*/
inline float CWzdAnimation::GetAnimationSpeedRate() const
{
    return m_fAniSpeedRate;
}

#endif // _3DFILEACCESS_WZDANIMATION_H_
