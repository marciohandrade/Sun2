// wzanistate.h

#ifndef _3DFILEACCESS_WZANISTATE_H_
#define _3DFILEACCESS_WZANISTATE_H_

#include "wzevent.h"
#include "wzanimationinfo.h"
#include <programcommon/wzflag.h>

class CWzAniStateCB;
class CWzdBase;
struct WzNode;
struct WzTransform;
struct WzKeyFrame;
struct WzTransformParam;

//------------------------------------------------------------------------------
/**
    @class CWzAniState
*/
class CWzAniState
{
public:
    // 생성자/소멸자
    CWzAniState();
    virtual ~CWzAniState();

    // 초기화
    void Initialize();

    // 애니메이션 정보 설정
    void SetAnimationInfo( WzAnimationInfo* pAnimationInfo, 
                           bool bLoopAni,
                           bool bBlendAni,
                           int nInterpolationFrame,
                           bool bReset );

    // 애니메이션 정보 얻기
    WzAnimationInfo* GetAnimationInfo() const;
    // 애니메이션 있는지 여부
    bool HasAnimation() const;

    // 애니메이션 데이타 얻기
    CWzdBase* GetAnimationData() const;

    // 프레임 진행
    ENUM_ANI_FRAME_STATUS ProgressFrame( float fFrameAdd, 
                                         WzEvent* pEvent,
                                         float fUnitAniSpeed,
                                         WzAnimationInfo* pPrevAni );

    // 단순 애니메이션
    bool AnimateSimple( WzTransform* pOutTform );
    // 일반 애니메이션
    bool Animate( WzTransform* pOutTform, const WzTransform* pInTform );
    // 블랜딩 애니메이션
    bool AnimateBlend( WzTransform* pOutTform );

    // 변환 행렬 계산
    void CalcTransformMatrix( WzMatrix* pOutMat,
                              const WzTransform* pInTform,
                              const WzTransformParam* pwtp );

    // 반복 여부 설정
    void SetAniLoop( bool b );
	bool GetAniLoop() const;

    // speed 비율 설정
    void SetSpeedRatio( float r );
	float GetSpeedRatio() const;

    // 블랜딩 본 설정
    void SetBlendBoneIndex( int idx );
	int GetBlendBoneIndex() const;

    // 델타 블랜딩 여부 설정
    void SetDeltaBlend( bool b );
	bool GetDeltaBlend() const;

    // 최대 프레임 얻기
    float GetMaxFrame() const;

    // 현재 프레임 설정/얻기
    void SetCurFrame( float fFrame );
    float GetCurFrame() const;

    // 보간 프레임 설정
    void SetStartIpolFrame( int nFrame );
    void SetEndIpolFrame( int nFrame );

    // 보간 중인가?
    bool IsInterpolating() const;

    // 콜백 클래스 등록
    void SetCallback( CWzAniStateCB* pCB );

private:
    // 변수 초기화
    void InitVariables();
    // 해제
    void Release();

    // 애니메이션 데이타 검사
    bool CheckAnimationData();

    // 이벤트 재생 여부 초기화
    void InitEventPlayFlags();
    // 이벤트 설정
    void SetEvents( WzEvent* pEvent );

    // 블랜딩 본 리스트 재설정
    void ResetBlendBoneList();
    // 블랜딩 본 추가
    void AddBlendBone( const WzNode* pBoneList, int nBoneIdx );

    // 해당 본 블랜딩 변환
    void BlendTransform_typeS( WzTransform* pOutTform, int iNode, float fWeight );
    void BlendTransform_typeSD( WzTransform* pOutTform, int iNode, float fWeight );

    void BlendTransform_typeE( WzTransform* pOutTform, int iNode, float fWeight );
    void BlendTransform_typeED( WzTransform* pOutTform, int iNode, float fWeight );

    void BlendTransform_typeN( WzTransform* pOutTform, int iNode );
    void BlendTransform_typeND( WzTransform* pOutTform, int iNode );

private:
    typedef std::vector<int> VECTOR_BLENDBONE;

    CWzAniStateCB* m_pCallback;

    WzAnimationInfo* m_pAnimationInfo;
    CWzdBase* m_pWzdAnimation;

    int m_nNumBone;
    WzKeyFrame* m_wzKeyFrameList;

    WzTransform* m_pTempTransformBuf;
    CWzFlag<DWORD> m_calcTempTformFlag;

    float m_fMaxFrame;      // 최대 프레임
    float m_fCurFrame;      // 현재 프레임

    int m_nIpolFrame;       // 지정된 보간 프레임 수
    int m_nStartIpolFrame;  // 앞 보간 프레임 수
    int m_nEndIpolFrame;    // 뒤 보간 프레임 수

    float m_fTotIpolTime;   // 총 보간 시간 (단위: 초)
    float m_fCurIpolTime;   // 현재 보간 시간 (단위: 초)

    ENUM_IPOL_STATE m_ipolState;

    float m_fSpeedRatio;    // 애니메이션 속도 비율
    bool m_bLoopAni;        // 반복 애니메이션?

    bool m_bBlendAni;       // 블랜딩 애니메이션?
    bool m_bDeltaBlend;     // 델타 블랜딩?

    int m_nBlendBoneIdx;    // 블랜딩 본 인덱스
    VECTOR_BLENDBONE m_blendBoneList;

    DWORD m_dwEventPlayFlag[3];
    int m_nEventPlayNum[3];
};

//------------------------------------------------------------------------------
/**
*/
inline void CWzAniState::InitEventPlayFlags()
{
    memset( m_dwEventPlayFlag, 0, sizeof( m_dwEventPlayFlag ) );
    memset( m_nEventPlayNum, 0, sizeof( m_nEventPlayNum ) );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzAniState::SetAniLoop( bool b )
{
    m_bLoopAni = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzAniState::SetSpeedRatio( float r )
{
    m_fSpeedRatio = r;
}

//------------------------------------------------------------------------------
/**
*/
inline WzAnimationInfo* CWzAniState::GetAnimationInfo() const
{
    return m_pAnimationInfo; 
}

//------------------------------------------------------------------------------
/**
*/
inline bool CWzAniState::HasAnimation() const
{
    return ( m_pAnimationInfo != NULL );
}

//------------------------------------------------------------------------------
/**
*/
inline CWzdBase* CWzAniState::GetAnimationData() const
{
    return m_pWzdAnimation; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzAniState::SetDeltaBlend( bool b )
{
    m_bDeltaBlend = b;
}

//------------------------------------------------------------------------------
/**
*/
inline float CWzAniState::GetMaxFrame() const
{
    return m_fMaxFrame;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzAniState::SetCurFrame( float fFrame )
{
    WzAssert( fFrame >= 0.0f );

    m_fCurFrame = max( 0.0f, fFrame );
}

//------------------------------------------------------------------------------
/**
*/
inline float CWzAniState::GetSpeedRatio() const
{
	return m_fSpeedRatio;
}

//------------------------------------------------------------------------------
/**
*/
inline bool CWzAniState::GetAniLoop() const
{
	return m_bLoopAni;
}

//------------------------------------------------------------------------------
/**
*/
inline float CWzAniState::GetCurFrame() const
{
    return m_fCurFrame;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzAniState::GetBlendBoneIndex() const
{
	return m_nBlendBoneIdx;
}

//------------------------------------------------------------------------------
/**
*/
inline bool CWzAniState::GetDeltaBlend() const
{
	return m_bDeltaBlend;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzAniState::SetStartIpolFrame( int nFrame )
{
    // xxx: SetAnimationInfo() 후, ProgressFrame() 전에 호출되어야 함

    if( m_pAnimationInfo )
    {
        m_nIpolFrame = nFrame;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzAniState::SetEndIpolFrame( int nFrame )
{
    if( m_pAnimationInfo )
    {
        m_nEndIpolFrame = max( 0, nFrame );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline bool CWzAniState::IsInterpolating() const
{
    return (m_ipolState == IS_IPOL_ING);
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzAniState::SetCallback( CWzAniStateCB* pCB )
{
    m_pCallback = pCB;
}

#endif // _3DFILEACCESS_WZANISTATE_H_