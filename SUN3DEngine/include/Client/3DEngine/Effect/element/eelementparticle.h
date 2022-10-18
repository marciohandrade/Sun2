// eelementparticle.h

#ifndef _EFFECT_EELEMENTPARTICLE_H_
#define _EFFECT_EELEMENTPARTICLE_H_

#include "eelementbase.h"

//------------------------------------------------------------------------------
/**
    @class CEElementParticle
*/
class CEElementParticle : public CEElementBase
{
public:
    // ������/�Ҹ���
    CEElementParticle();
    virtual ~CEElementParticle();

    // �ʱ�ȭ
    virtual void Clear();

    // ����/�ݺ�
    virtual bool Create( bool bFirst );
    virtual void Loop();

    // Move (�ùķ��̼�)
    virtual _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate );

    // ���� ��(��ġ, ȸ��, ũ��) ���� - �ܺο��� ȣ��
    virtual void SetResultMatrix( const WzVector& wvPos, const WzVector& wvRot, const WzVector& wvScale );
    virtual void SetResultMatrix( const WzMatrix* pwmPos, const WzMatrix* pwmRot, const WzMatrix* pwmScale );

    // ũ�� ���
    const WzVector& GetScale() const;
    // ȸ�� ���
    const WzVector& GetRot() const;
    // ��ġ ���
    const WzVector& GetPos() const;
    // ���� ��ġ ���
    const WzVector& GetRealPos() const;


protected:
    // �¾ �� ó��
    virtual void OnBeingBorn();

    // Į�� ���
    void CalculateToColor( DWORD dwTime );
    // ������ ���
    void CalculateToScale( DWORD dwTime );
    // ���� ȸ�� ���
    void CalculateToRotation( DWORD dwTime );
    // ȸ�� ��� (����Ʈ ��ǥ�� �߽�)
    void CalculateToPosRotation( DWORD dwTime );
    // ��ġ ���
    void CalculateToPos( DWORD dwTime, DWORD dwTick );

    // ������ ����
    void SetScale( const WzVector& wvScale );
    // ȸ�� ����
    void SetRot( const WzVector& wvRot );
    // ��ġ ����
    void SetPos( const WzVector& wvPos );

    // �ܺ� ��ġ ���� ��������
    bool IsEnableSetMatrix();

protected:
    // ��ġ ����
    WzVector m_wvInitPos;           // �ʱ� ��ġ (����Ʈ ��ǥ��)
    WzVector m_wvLocalPos;          // ���� ��ġ (����Ʈ ��ǥ��)

    WzVector m_wvPos;               // ���� ��ġ = ���� ��ġ + �ʱ� ��ġ (����Ʈ ��ǥ��) 
    WzVector m_wvPrevPos;           // ���� ���� ��ġ

    WzVector m_wvRealPos;           // ���� ��ǥ (���� ��ǥ��)
    WzVector m_wvPrevRealPos;       // ���� ���� ��ǥ

    WzVector m_wvPrevCoursePos;     // �ڽ� �̵��� ���� ��ǥ
    WzVector m_wvPrevParentPos;     // �θ��� ���� ��ġ

    // ȸ�� ����
    WzVector m_wvRotInit;           // �ʱ� ȸ��

    WzVector m_wvRot;               // ȸ����  
    WzVector m_wvPrevRot;           // ���� ȸ����

    WzVector m_wvPosRot;            // �ʱ� ��ġ ȸ��
    WzVector m_wvPrevParentRot;     // �θ��� ���� ȸ����

    // ������ ����
    WzVector m_wvScale;             // ������ ��
    
    // �ӵ�/���ӵ� ����
    WzVector m_wvVelocity;          // �ӵ� (m/s)
    float m_fPhysicsForce;          // ������

    WzVector m_wvGravity;           // �߷� ����
    float m_fGravityForce;          // �߷���
    bool m_bGravity;                // �߷� ����?

    // ��Ÿ
    float m_fRandomLifeRate;        // ���� ���� ����

    // ��� ��� ����
    bool m_bSetResultMat;

    WzMatrix m_wmScale;
    WzMatrix m_wmRot;
    WzMatrix m_wmPos;
    WzMatrix m_wmResult;
};

//------------------------------------------------------------------------------
/**
*/
inline bool CEElementParticle::IsEnableSetMatrix()
{
    WzAssert( m_ec );

    // ���� �̵��� �ƴѵ�, �̹� �¾�� ������� ������ ���¸� ����
    // �̴� �ݴ��, ���� �̵��� ���, �¾�� ��, ��� ���� ������
    // ������ �� ������ �� �ִٴ� ��Ⱑ �ȴ�.
    if( !m_ec->m_pMove->m_blSubordinateToParent && m_bBorn && m_bSetResultMat )
    {
        return false;
    }
    
    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementParticle::Loop()
{
    CEElementBase::Loop();

    // xxx: ���⼭ �̰� �� ��� ���� �����ӿ��� SetResultMatrix()�� ������.
    m_bSetResultMat = false;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementParticle::SetScale( const WzVector& wvScale )
{
    m_wvScale = wvScale;
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CEElementParticle::GetScale() const
{
    return m_wvScale;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementParticle::SetRot( const WzVector& wvRot )
{
    m_wvPrevRot = m_wvRot;
    m_wvRot = wvRot;
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CEElementParticle::GetRot() const
{
    return m_wvRot;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementParticle::SetPos( const WzVector& wvPos )
{
    m_wvPrevPos = m_wvPos;
    m_wvPos = wvPos;
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CEElementParticle::GetPos() const
{
    return m_wvPos;
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CEElementParticle::GetRealPos() const
{
    return m_wvRealPos;
}

#endif // _EFFECT_EELEMENTPARTICLE_H_