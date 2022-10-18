// moveinfo.h

#ifndef _EFFECT_MOVEINFO_H_
#define _EFFECT_MOVEINFO_H_

#include "../typedef.h"

//------------------------------------------------------------------------------
/**
    @struct St_Move_Info
*/
struct St_Move_Info
{
    bool m_blSubordinateToParent;       // 부모 종속 이동?

    float m_fXAngleRange;               // 초기 속도 벡터 랜덤 회전 각
    float m_fYAngleRange;
    float m_fZAngleRange;

    bool m_blGravity;                   // 중력 적용?
    bool m_blPhysicsMove;               // 물리 이동?

    WzVector m_wvInitVelocity;          // 초기 속도
    float m_fPhysicsForce;              // 물리힘

    WzVector m_wvGravityVelocity;       // 중력 방향
    float m_fGravityForce;              // 중력힘

    ENUM_MOVE_TARRAIN m_eTarrainAttr;   // 지형 체크 속성

    WzVector m_wvVelocityAngle;         // 각 축별 초기 속도 회전각
    ENUM_DIR_TYPE m_eDirAttr;           // 방향 속성 (안으로, 밖으로 등)

    std_CourseMap m_stdMapCourse;       // 이동 코스 맵 <시간, 위치>쌍

    WzVector m_wvDirection;             // xxx: 사용 안 함
    std_CourseMap m_stdMapPos2;         // xxx: 사용 안 함

    St_RandomInfo m_sRandForcePhy;      // 랜덤 물리힘 정보
    St_RandomInfo m_sRandForceGravity;  // 랜덤 중력힘 정보
    St_RandomInfo m_sRandLifeTime;      // 랜덤 수명 정보

    St_RandomInfo m_sRandAccelX;        // 랜덤 가속도 정보
    St_RandomInfo m_sRandAccelY;          
    St_RandomInfo m_sRandAccelZ;

    St_RandomInfo m_sRandReverseForce;  // 랜덤 역방향 힘 정보

    // 생성자
    St_Move_Info()
    {
        Initialize();
    }

    // 초기화
    void Initialize()
    {
        // memset으로 초기화 하면 안 됨!!

        m_blSubordinateToParent = false;

        m_fXAngleRange = 0.0f;
        m_fYAngleRange = 0.0f;
        m_fZAngleRange = 0.0f;

        m_blGravity = false;
        m_blPhysicsMove = true;

        SetVector( &m_wvInitVelocity, 0.0f, 0.0f, 0.0f );
        m_fPhysicsForce = 0.0f;

        SetVector( &m_wvGravityVelocity, 0.0f, 0.0f, -1.0f );
        m_fGravityForce = 9.8f;

        m_eTarrainAttr = EMT_NONE;

        SetVector( &m_wvVelocityAngle, 0.0f, 0.0f, 0.0f );
        m_eDirAttr = EDT_DIR_NO;

        m_stdMapCourse.clear();

        SetVector( &m_wvDirection, 0.0f, 0.0f, 0.0f );
        m_stdMapPos2.clear();

        m_sRandForcePhy.Clear();
        m_sRandForceGravity.Clear();
        m_sRandLifeTime.Clear();

        m_sRandAccelX.Clear();
        m_sRandAccelY.Clear();          
        m_sRandAccelZ.Clear();

        m_sRandReverseForce.Clear();    
    }

    // 데이타 추가
    // xxx: 동일 키가 들어오면 해당 키 데이타를 덮어 쓴다. (기존 호환성)
    // cf) insert() 함수를 쓰면 덮어쓰지 않는다.
    void Add( int t, const WzVector& pos, ENUM_MOVE_KIND eKind = EM_POS1 )
    {
        switch( eKind )
        {
        case EM_POS1:
            m_stdMapCourse[t] = pos;
            return;

        case EM_POS2:
            m_stdMapPos2[t] = pos;
            return;
        }
    }

    // 데이타 제거
    void RemoveAt( int t, ENUM_MOVE_KIND eKind = EM_POS1 )
    {
        switch( eKind )
        {
        case EM_POS1:
            {
                std_CourseMap::iterator it = m_stdMapCourse.find( t );
                if( it != m_stdMapCourse.end() )
                {                           
                    m_stdMapCourse.erase( it );
                }
            }
            return;

        case EM_POS2:
            {
                std_CourseMap::iterator it = m_stdMapPos2.find( t );
                if( it != m_stdMapPos2.end() )
                {                           
                    m_stdMapPos2.erase( it );
                }
            }
            return;
        }
    }

    // 데이타 전부 제거
    void RemoveAll()
    {
        m_stdMapCourse.clear();
        m_stdMapPos2.clear();
    }

    // operator =
    St_Move_Info& operator = ( const St_Move_Info& rhs )
    {
        if( &rhs == this )
        {
            return *this;
        }

        // memcpy로 복사 하면 안 됨

        m_blSubordinateToParent = rhs.m_blSubordinateToParent;

        m_fXAngleRange = rhs.m_fXAngleRange;
        m_fYAngleRange = rhs.m_fYAngleRange;
        m_fZAngleRange = rhs.m_fZAngleRange;

        m_blGravity = rhs.m_blGravity;
        m_blPhysicsMove = rhs.m_blPhysicsMove;

        m_wvInitVelocity = rhs.m_wvInitVelocity;
        m_fPhysicsForce = rhs.m_fPhysicsForce;

        m_wvGravityVelocity = rhs.m_wvGravityVelocity;
        m_fGravityForce = rhs.m_fGravityForce;

        m_eTarrainAttr = rhs.m_eTarrainAttr;

        m_wvVelocityAngle = rhs.m_wvVelocityAngle;
        m_eDirAttr = rhs.m_eDirAttr;

        m_stdMapCourse = rhs.m_stdMapCourse;

        m_wvDirection = rhs.m_wvDirection;
        m_stdMapPos2 = rhs.m_stdMapPos2;

        m_sRandForcePhy = rhs.m_sRandForcePhy;
        m_sRandForceGravity = rhs.m_sRandForceGravity;
        m_sRandLifeTime = rhs.m_sRandLifeTime;

        m_sRandAccelX = rhs.m_sRandAccelX;
        m_sRandAccelY = rhs.m_sRandAccelY;
        m_sRandAccelZ = rhs.m_sRandAccelZ;

        m_sRandReverseForce = rhs.m_sRandReverseForce;

        return *this;
    }
};

#endif // _EFFECT_MOVEINFO_H_