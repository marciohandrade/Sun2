// wzdanimation.h

#ifndef _3DFILEACCESS_WZDANIMATION_H_
#define _3DFILEACCESS_WZDANIMATION_H_

#include "wzdbase.h"

//------------------------------------------------------------------------------
/**
    @class CWzdAnimation

    * ��(Bone)�ϰ� ���(Node)�ϰ� ���� �ǹ�
*/
class CWzdAnimation : public CWzdBase  
{
public:
    // ������/�Ҹ���
    CWzdAnimation();
    virtual ~CWzdAnimation();

    // �ı�
    virtual void Destroy();

    // �� �Ҵ�
    virtual void AllocBones( WORD wNumNode );
    // �� ���� ����
    virtual void SetBoneInfo( WORD wNode, int iParent, const char* lpszName = NULL );

    // �� �� ���
    virtual WORD GetNumNode() const;

    // �� ����Ʈ ���
    virtual WzNode* GetBonesList() const;
    // �߰� �� ���� ����Ʈ ���
    virtual WzNodeExtra* GetBoneExtraInfoList() const;

    // �� ���� �� (�Ϻ��ϰ� ��ġ�ؾ� ��)
    virtual BOOL CompareBoneStructure( const CWzdBase* pDest );
    // �� ����Ʈ �� (������ Ʋ�� �� ����)
    virtual BOOL CompareBoneList( const CWzdBase* pDest );

    // Ű ������ �Ҵ�
    virtual void AllocKeyFrames();
    // Ű ��ȯ �Ҵ�
    virtual void AllocKeyTransforms( WORD wNode, WORD wNumFrame );

    // ���� ������ ��ȣ ����
    virtual void SetRealFrame( WORD wNode, WORD wKeyFrame, WORD wRealFrame );

    // Ű ��ȯ ���� ����
    virtual void SetKeyTransformInfo( WORD wNode, 
                                      WORD wFrame, 
                                      const WzVector& wvPos, 
                                      const WzQuaternion& wqRot );

    // Ű ������ ����Ʈ ���
    virtual WzKeyFrame* GetKeyFramesList() const;

    // �� ������ �� ���
    virtual WORD GetNumFrame() const;
    // �ش� �� ������ �� ���
    virtual WORD GetNumBoneFrame( WORD wNode ) const;

    // �ش� �����ӿ��� ��ü �� ��ȯ ���
    virtual void CreateTransformArrayByFrame( WzTransform* pOutTform, float fFrame );
    // �ش� �����ӿ��� �ش� �� ��ȯ ���
    virtual void CreateTransformByFrame( WzTransform* pOutTform, WORD wNode, float fFrame );

    // �־��� �� ��ȯ���� ���� �� ��� ���ϱ�
    virtual void CalcTransformMatrixT( WzMatrix* pInOutMat, 
                                       const WzTransform* pInTform, 
                                       const WzTransformParam* pwtp );

    // �ִϸ��̼� ���� (A -> B)
    virtual void AnimationSlerp( WzTransform* pOutTform, 
                                 const WzTransform* pwtA, 
                                 const WzTransform* pwtB, 
                                 float fWeight );

    // �ִϸ��̼� ���ǵ� ����/���
    virtual void SetAnimationSpeedRate( float fRate );
    virtual float GetAnimationSpeedRate() const;

    // ID ���/�˻�
    virtual void GetIdentity( BYTE byIdentity[4] );
    virtual BOOL CheckIdentity( const BYTE* byIdentity ) const;

protected:
    // Serialize
    virtual BOOL Serialize( CWzArchive* pArchive );

    // Save (�ؽ�Ʈ ����)
    BOOL Save_Text( CWzArchive* pArchive );

private:
    // ���� �ʱ�ȭ (���� �Լ��� �������� �� ��)
    void InitVariables();

    // �޸� ����
    void Release();

    // �־��� �� ��ȯ���� ���� �� ��� ���ϱ�
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

    // 0�� ���� ������ �������� Key(���� ������ ��ȣ) + 1
    // (��� ���� ������ ���� ������ ��ȣ�� ����)
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
