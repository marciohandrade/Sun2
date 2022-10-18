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
    // ������/�Ҹ���
    CWzAniState();
    virtual ~CWzAniState();

    // �ʱ�ȭ
    void Initialize();

    // �ִϸ��̼� ���� ����
    void SetAnimationInfo( WzAnimationInfo* pAnimationInfo, 
                           bool bLoopAni,
                           bool bBlendAni,
                           int nInterpolationFrame,
                           bool bReset );

    // �ִϸ��̼� ���� ���
    WzAnimationInfo* GetAnimationInfo() const;
    // �ִϸ��̼� �ִ��� ����
    bool HasAnimation() const;

    // �ִϸ��̼� ����Ÿ ���
    CWzdBase* GetAnimationData() const;

    // ������ ����
    ENUM_ANI_FRAME_STATUS ProgressFrame( float fFrameAdd, 
                                         WzEvent* pEvent,
                                         float fUnitAniSpeed,
                                         WzAnimationInfo* pPrevAni );

    // �ܼ� �ִϸ��̼�
    bool AnimateSimple( WzTransform* pOutTform );
    // �Ϲ� �ִϸ��̼�
    bool Animate( WzTransform* pOutTform, const WzTransform* pInTform );
    // ���� �ִϸ��̼�
    bool AnimateBlend( WzTransform* pOutTform );

    // ��ȯ ��� ���
    void CalcTransformMatrix( WzMatrix* pOutMat,
                              const WzTransform* pInTform,
                              const WzTransformParam* pwtp );

    // �ݺ� ���� ����
    void SetAniLoop( bool b );
	bool GetAniLoop() const;

    // speed ���� ����
    void SetSpeedRatio( float r );
	float GetSpeedRatio() const;

    // ���� �� ����
    void SetBlendBoneIndex( int idx );
	int GetBlendBoneIndex() const;

    // ��Ÿ ���� ���� ����
    void SetDeltaBlend( bool b );
	bool GetDeltaBlend() const;

    // �ִ� ������ ���
    float GetMaxFrame() const;

    // ���� ������ ����/���
    void SetCurFrame( float fFrame );
    float GetCurFrame() const;

    // ���� ������ ����
    void SetStartIpolFrame( int nFrame );
    void SetEndIpolFrame( int nFrame );

    // ���� ���ΰ�?
    bool IsInterpolating() const;

    // �ݹ� Ŭ���� ���
    void SetCallback( CWzAniStateCB* pCB );

private:
    // ���� �ʱ�ȭ
    void InitVariables();
    // ����
    void Release();

    // �ִϸ��̼� ����Ÿ �˻�
    bool CheckAnimationData();

    // �̺�Ʈ ��� ���� �ʱ�ȭ
    void InitEventPlayFlags();
    // �̺�Ʈ ����
    void SetEvents( WzEvent* pEvent );

    // ���� �� ����Ʈ �缳��
    void ResetBlendBoneList();
    // ���� �� �߰�
    void AddBlendBone( const WzNode* pBoneList, int nBoneIdx );

    // �ش� �� ���� ��ȯ
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

    float m_fMaxFrame;      // �ִ� ������
    float m_fCurFrame;      // ���� ������

    int m_nIpolFrame;       // ������ ���� ������ ��
    int m_nStartIpolFrame;  // �� ���� ������ ��
    int m_nEndIpolFrame;    // �� ���� ������ ��

    float m_fTotIpolTime;   // �� ���� �ð� (����: ��)
    float m_fCurIpolTime;   // ���� ���� �ð� (����: ��)

    ENUM_IPOL_STATE m_ipolState;

    float m_fSpeedRatio;    // �ִϸ��̼� �ӵ� ����
    bool m_bLoopAni;        // �ݺ� �ִϸ��̼�?

    bool m_bBlendAni;       // ���� �ִϸ��̼�?
    bool m_bDeltaBlend;     // ��Ÿ ����?

    int m_nBlendBoneIdx;    // ���� �� �ε���
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
    // xxx: SetAnimationInfo() ��, ProgressFrame() ���� ȣ��Ǿ�� ��

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