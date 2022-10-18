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
    bool m_blSubordinateToParent;       // �θ� ���� �̵�?

    float m_fXAngleRange;               // �ʱ� �ӵ� ���� ���� ȸ�� ��
    float m_fYAngleRange;
    float m_fZAngleRange;

    bool m_blGravity;                   // �߷� ����?
    bool m_blPhysicsMove;               // ���� �̵�?

    WzVector m_wvInitVelocity;          // �ʱ� �ӵ�
    float m_fPhysicsForce;              // ������

    WzVector m_wvGravityVelocity;       // �߷� ����
    float m_fGravityForce;              // �߷���

    ENUM_MOVE_TARRAIN m_eTarrainAttr;   // ���� üũ �Ӽ�

    WzVector m_wvVelocityAngle;         // �� �ະ �ʱ� �ӵ� ȸ����
    ENUM_DIR_TYPE m_eDirAttr;           // ���� �Ӽ� (������, ������ ��)

    std_CourseMap m_stdMapCourse;       // �̵� �ڽ� �� <�ð�, ��ġ>��

    WzVector m_wvDirection;             // xxx: ��� �� ��
    std_CourseMap m_stdMapPos2;         // xxx: ��� �� ��

    St_RandomInfo m_sRandForcePhy;      // ���� ������ ����
    St_RandomInfo m_sRandForceGravity;  // ���� �߷��� ����
    St_RandomInfo m_sRandLifeTime;      // ���� ���� ����

    St_RandomInfo m_sRandAccelX;        // ���� ���ӵ� ����
    St_RandomInfo m_sRandAccelY;          
    St_RandomInfo m_sRandAccelZ;

    St_RandomInfo m_sRandReverseForce;  // ���� ������ �� ����

    // ������
    St_Move_Info()
    {
        Initialize();
    }

    // �ʱ�ȭ
    void Initialize()
    {
        // memset���� �ʱ�ȭ �ϸ� �� ��!!

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

    // ����Ÿ �߰�
    // xxx: ���� Ű�� ������ �ش� Ű ����Ÿ�� ���� ����. (���� ȣȯ��)
    // cf) insert() �Լ��� ���� ����� �ʴ´�.
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

    // ����Ÿ ����
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

    // ����Ÿ ���� ����
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

        // memcpy�� ���� �ϸ� �� ��

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