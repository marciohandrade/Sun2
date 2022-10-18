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
    // 생성자/소멸자
    CEElementParticle();
    virtual ~CEElementParticle();

    // 초기화
    virtual void Clear();

    // 생성/반복
    virtual bool Create( bool bFirst );
    virtual void Loop();

    // Move (시뮬레이션)
    virtual _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate );

    // 월드 값(위치, 회전, 크기) 설정 - 외부에서 호출
    virtual void SetResultMatrix( const WzVector& wvPos, const WzVector& wvRot, const WzVector& wvScale );
    virtual void SetResultMatrix( const WzMatrix* pwmPos, const WzMatrix* pwmRot, const WzMatrix* pwmScale );

    // 크기 얻기
    const WzVector& GetScale() const;
    // 회전 얻기
    const WzVector& GetRot() const;
    // 위치 얻기
    const WzVector& GetPos() const;
    // 월드 위치 얻기
    const WzVector& GetRealPos() const;


protected:
    // 태어날 때 처리
    virtual void OnBeingBorn();

    // 칼라 계산
    void CalculateToColor( DWORD dwTime );
    // 스케일 계산
    void CalculateToScale( DWORD dwTime );
    // 로컬 회전 계산
    void CalculateToRotation( DWORD dwTime );
    // 회전 계산 (이펙트 좌표계 중심)
    void CalculateToPosRotation( DWORD dwTime );
    // 위치 계산
    void CalculateToPos( DWORD dwTime, DWORD dwTick );

    // 스케일 설정
    void SetScale( const WzVector& wvScale );
    // 회전 설정
    void SetRot( const WzVector& wvRot );
    // 위치 설정
    void SetPos( const WzVector& wvPos );

    // 외부 위치 설정 가능한지
    bool IsEnableSetMatrix();

protected:
    // 위치 관련
    WzVector m_wvInitPos;           // 초기 위치 (이펙트 좌표계)
    WzVector m_wvLocalPos;          // 로컬 위치 (이펙트 좌표계)

    WzVector m_wvPos;               // 최종 위치 = 로컬 위치 + 초기 위치 (이펙트 좌표계) 
    WzVector m_wvPrevPos;           // 이전 최종 위치

    WzVector m_wvRealPos;           // 월드 좌표 (월드 좌표계)
    WzVector m_wvPrevRealPos;       // 이전 월드 좌표

    WzVector m_wvPrevCoursePos;     // 코스 이동시 이전 좌표
    WzVector m_wvPrevParentPos;     // 부모의 이전 위치

    // 회전 관련
    WzVector m_wvRotInit;           // 초기 회전

    WzVector m_wvRot;               // 회전값  
    WzVector m_wvPrevRot;           // 이전 회전값

    WzVector m_wvPosRot;            // 초기 위치 회전
    WzVector m_wvPrevParentRot;     // 부모의 이전 회전값

    // 스케일 관련
    WzVector m_wvScale;             // 스케일 값
    
    // 속도/가속도 관련
    WzVector m_wvVelocity;          // 속도 (m/s)
    float m_fPhysicsForce;          // 물리힘

    WzVector m_wvGravity;           // 중력 벡터
    float m_fGravityForce;          // 중력힘
    bool m_bGravity;                // 중력 적용?

    // 기타
    float m_fRandomLifeRate;        // 랜덤 수명 비율

    // 결과 행렬 관련
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

    // 종속 이동이 아닌데, 이미 태어났고 결과까지 설정된 상태면 리턴
    // 이는 반대로, 종속 이동인 경우, 태어나기 전, 결과 설정 전에는
    // 언제든 재 설정할 수 있다는 얘기가 된다.
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

    // xxx: 여기서 이걸 해 줘야 다음 프레임에서 SetResultMatrix()가 먹힌다.
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