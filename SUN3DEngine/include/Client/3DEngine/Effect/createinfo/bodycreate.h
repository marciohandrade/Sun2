// bodycreate.h

#ifndef _EFFECT_BODYCREATE_H_
#define _EFFECT_BODYCREATE_H_

#include "createinfo.h"
#include "moveinfo.h"
#include "changeinfo.h"
#include "visualinfo.h"
#include "soundinfo.h"
#include "elenodeinfo.h"
#include <client/3dengine/common/wznaturalcubicsplinedyn.h>
#include <client/3dengine/common/boundingvolume.h>

//------------------------------------------------------------------------------
/**
    * �⺻ ����

    - BodyCreate�� element ������ �ʿ��� ��� ������ �ִ�.
    - BodyCreate�� CreateInfo, MoveInfo, ChangeInfo, VisualInfo�� �������� ���� �� �ִ�.
    - ElementCreate�� �ε����� �̿��Ͽ� BodyCreate�� �ִ� ������ ��ũ�ȴ�.

    - �̷��� �ؼ� ������ ������ �� �ְ�, �ε��� ���ո����� �������� ���� Ÿ����
      ���� �� �ִٴ� ������ �ִµ�, ���� �׷��� �������� �ʰ� �ִ�.
*/

//------------------------------------------------------------------------------
/**
    @struct St_ElementCreate
*/
struct St_ElementCreate
{
    // ���� ���� ������ (index�� �̿��� BodyCreate�� �ִ� ���� ������ ��ũ)
    St_Create_Info* m_pCreate;
    St_Move_Info* m_pMove;
    St_Change_Info* m_pChange;
    St_Visual_Info* m_pVisual;  

    // ���� ���� �ε��� (BodyCreate�� �ִ� ���� ���� index)
    short m_nIndexCreate;
    short m_nIndexMove;
    short m_nIndexChange;
    short m_nIndexVisual; 
    
    // ���� ����
    St_SoundInfo m_sSoundInfo;

    // ũ��
    WzNaturalCubicSplineDyn* m_pcSplineScaleX;
    WzNaturalCubicSplineDyn* m_pcSplineScaleY;
    WzNaturalCubicSplineDyn* m_pcSplineScaleZ;

    // RGBA
    WzNaturalCubicSplineDyn* m_pcSplineRed;  
    WzNaturalCubicSplineDyn* m_pcSplineGreen;    
    WzNaturalCubicSplineDyn* m_pcSplineBlue;
    WzNaturalCubicSplineDyn* m_pcSplineAlpha;

    // ���ӵ�
    WzNaturalCubicSplineDyn* m_pcSplineAccelerationX;
    WzNaturalCubicSplineDyn* m_pcSplineAccelerationY;
    WzNaturalCubicSplineDyn* m_pcSplineAccelerationZ;

    // �ʱ� ��ġ ȸ��
    WzNaturalCubicSplineDyn* m_pcSplinePosRotationX;
    WzNaturalCubicSplineDyn* m_pcSplinePosRotationY;
    WzNaturalCubicSplineDyn* m_pcSplinePosRotationZ;

    // ���� ȸ��
    WzNaturalCubicSplineDyn* m_pcSplineRotationX;
    WzNaturalCubicSplineDyn* m_pcSplineRotationY;
    WzNaturalCubicSplineDyn* m_pcSplineRotationZ;

    // �ڽ� ����
    WzNaturalCubicSplineDyn* m_pcSplineCreateCustomX;
    WzNaturalCubicSplineDyn* m_pcSplineCreateCustomY;
    WzNaturalCubicSplineDyn* m_pcSplineCreateCustomZ;

    // �ڽ� �̵�
    WzNaturalCubicSplineDyn* m_pcSplineCourseX;
    WzNaturalCubicSplineDyn* m_pcSplineCourseY;
    WzNaturalCubicSplineDyn* m_pcSplineCourseZ;

    // ������
    St_ElementCreate()
    {
        Initialize();
    }

    // �ʱ�ȭ
    void Initialize()
    {
        // ��������� memset���� �ʱ�ȭ �ص� �����ϴ�.
        // ����, ���Ӱ� ��� ������ �߰��Ѵٰų� �ϴ� ����
        // memset�� �ص� �Ǵ��� ���� ���� �Ѵ�.

        memset( this, 0, sizeof( St_ElementCreate ) );
    }

private:
    // operator = (����)
    St_ElementCreate& operator = ( const St_ElementCreate& rhs );
};

//------------------------------------------------------------------------------
/**
    @struct St_BodyCreate

    ��¼�ٰ� �̷� �������� ����ü��;;
    �ϴ� �̷��� ���� ���߿� �ѹ� �� ���� �غ���
*/
struct St_BodyCreate
{
    WzID m_wzBodyID;            // BodyID
    float m_fSpeedToSecond;     // �ʴ� ���ǵ�

    bool m_blDeadBeforeFade;    // ���� �� FadeOut ����?
    float m_fFadeOfSec;         // ���� ���� ����

    BvAABB m_bbox;              // �ٿ�� �ڽ�

    // Elememt ���� ����
    std_list m_stdList_Element;

    // ���� ���� (0���� Body ������)
    std_list m_stdList_Create;
    std_list m_stdList_Move;
    std_list m_stdList_Change;
    std_list m_stdList_Visual;   

    float m_fAccmulateScale;    // ���� ������ ��
    int m_nLevel;               // ����Ʈ ����

#ifdef _DEBUG
    char m_FilePath[2048];
#endif //_DEBUG

    // ������
    St_BodyCreate()
    : m_wzBodyID( 0 )
    , m_fSpeedToSecond( 1.0f )
    , m_blDeadBeforeFade( false )
    , m_fFadeOfSec( 1.0f )
    , m_fAccmulateScale( 1.0f )
    , m_nLevel( 0 )
    {
        SetVector( &m_bbox.m_wvMin, -0.5f, -0.5f, -0.5f );
        SetVector( &m_bbox.m_wvMax, 0.5f, 0.5f, 0.5f );
    }

    // �Ҹ���
    ~St_BodyCreate()
    {
    }

    // ����
#ifdef _USE_ENGINE_MEMORY_POOL_
    void ReleaseAll();

    // operator =
    St_BodyCreate& St_BodyCreate::operator = ( const St_BodyCreate& rhs );

    // Element ���� ���� ����
    void St_BodyCreate::SetupAllElementCreates();

    // Element ���� ������ ���ö��� ����
    void St_BodyCreate::CreateSpline( St_ElementCreate* ec );

    // ���ö��� ����
    void DestroySpline( St_ElementCreate* ec );
#else
    void ReleaseAll()
    {
        std_list::iterator it;

        // Visual ���� ����
        for( it = m_stdList_Visual.begin(); it != m_stdList_Visual.end(); ++it )
        {
            St_Visual_Info* info = (St_Visual_Info*)(*it);
            SAFE_DELETE( info );
        }

        m_stdList_Visual.clear();

        // Change ���� ����
        for( it = m_stdList_Change.begin(); it != m_stdList_Change.end(); ++it )
        {
            St_Change_Info* info = (St_Change_Info*)(*it);
            SAFE_DELETE( info );
        }

        m_stdList_Change.clear();

        // Move ���� ����
        for( it = m_stdList_Move.begin(); it != m_stdList_Move.end(); ++it )
        {
            St_Move_Info* info = (St_Move_Info*)(*it);
            SAFE_DELETE( info );
        }

        m_stdList_Move.clear();

        // Create ���� ����
        for( it = m_stdList_Create.begin(); it != m_stdList_Create.end(); ++it )
        {
            St_Create_Info* info = (St_Create_Info*)(*it);
            SAFE_DELETE( info );
        }

        m_stdList_Create.clear();

        // Element ���� ���� ����
        for( it = m_stdList_Element.begin(); it != m_stdList_Element.end(); ++it )
        {           
            St_ElementCreate* elem = (St_ElementCreate*)(*it);
            if( elem )
            {
                // ���ö��� ����
                DestroySpline( elem );

                delete elem;
            }
        }

        m_stdList_Element.clear();
    }

    // operator =
    St_BodyCreate& St_BodyCreate::operator = ( const St_BodyCreate& rhs )
    {
        if( &rhs == this )
        {
            return *this;
        }

        // �ϴ� ���� ����Ÿ ���� ����
        ReleaseAll();

        // memcpy�� ����ϸ� �� ��

        m_wzBodyID = rhs.m_wzBodyID;
        m_fSpeedToSecond = rhs.m_fSpeedToSecond;

        m_blDeadBeforeFade = rhs.m_blDeadBeforeFade;
        m_fFadeOfSec = rhs.m_fFadeOfSec;

        m_bbox = rhs.m_bbox;

        m_fAccmulateScale = rhs.m_fAccmulateScale;
        m_nLevel = rhs.m_nLevel;

        // Create, Move, Change, Visual ������ ���� ����Ʈ��
        // NULL���� �ִٰ� �ϴ��� ������ �°� ������ ��� �Ѵ�.
        // ElementCreate���� �ε����� �����Ǳ� �����̴�.

        // �ݸ�, ElementCreate�� ���� ������ �ִ� ��츸, �� ������ �����Ѵ�.

        std_list::const_iterator it;

        // Element ���� ����
        for( it = rhs.m_stdList_Element.begin(); it != rhs.m_stdList_Element.end(); ++it )
        {
            St_ElementCreate* elem = (St_ElementCreate*)(*it);
            WzAssert( elem );

            // Element ���� ������ ���� ������ �ִ� ��츸 ����
            if( elem )
            {
                St_ElementCreate* newElem = new St_ElementCreate;
                WzAssert( newElem );

                newElem->m_nIndexCreate = elem->m_nIndexCreate;
                newElem->m_nIndexMove = elem->m_nIndexMove;
                newElem->m_nIndexChange = elem->m_nIndexChange;
                newElem->m_nIndexVisual = elem->m_nIndexVisual; 

                // ���� ����
                newElem->m_sSoundInfo = elem->m_sSoundInfo;

                m_stdList_Element.push_back( newElem );
            }
        }

        // Create ����
        for( it = rhs.m_stdList_Create.begin(); it != rhs.m_stdList_Create.end(); ++it )
        {
            St_Create_Info* newInfo = new St_Create_Info;
            WzAssert( newInfo );

            St_Create_Info* info = (St_Create_Info*)(*it);
            WzAssert( info );

            if( info )
            {
                *newInfo = *info;
            }

            m_stdList_Create.push_back( newInfo );
        }

        // Move ����
        for( it = rhs.m_stdList_Move.begin(); it != rhs.m_stdList_Move.end(); ++it )
        {
            St_Move_Info* newInfo = new St_Move_Info;
            WzAssert( newInfo );

            St_Move_Info* info = (St_Move_Info*)(*it);
            WzAssert( info );

            if( info )
            {
                *newInfo = *info;
            }

            m_stdList_Move.push_back( newInfo );
        }

        // Change ����
        for( it = rhs.m_stdList_Change.begin(); it != rhs.m_stdList_Change.end(); ++it )
        {
            St_Change_Info* newInfo = new St_Change_Info;
            WzAssert( newInfo );

            St_Change_Info* info = (St_Change_Info*)(*it);
            WzAssert( info );

            if( info )
            {
                *newInfo = *info;
            }

            m_stdList_Change.push_back( newInfo );
        }

        // Visual ����
        for( it = rhs.m_stdList_Visual.begin(); it != rhs.m_stdList_Visual.end(); ++it )
        {
            St_Visual_Info* newInfo = new St_Visual_Info;
            WzAssert( newInfo );

            St_Visual_Info* info = (St_Visual_Info*)(*it);
            WzAssert( info );

            if( info )
            {
                *newInfo = *info;
            }

            m_stdList_Visual.push_back( newInfo );
        }

        // ElementCreate ����
        SetupAllElementCreates();

        return *this;
    }

    // Element ���� ���� ����
    void St_BodyCreate::SetupAllElementCreates()
    {
        std_list::iterator it = m_stdList_Element.begin();
        for( ; it != m_stdList_Element.end(); ++it )
        {           
            St_ElementCreate* ec = (St_ElementCreate*)(*it);
            WzAssert( ec );

            // Create ����
            WzAssert( ec->m_nIndexCreate >= 0 && (size_t)ec->m_nIndexCreate < m_stdList_Create.size() );

            ec->m_pCreate = (St_Create_Info*)m_stdList_Create[ec->m_nIndexCreate];
            WzAssert( ec->m_pCreate );

            // Move ����
            WzAssert( ec->m_nIndexMove >= 0 && (size_t)ec->m_nIndexMove < m_stdList_Move.size() );

            ec->m_pMove = (St_Move_Info*)m_stdList_Move[ec->m_nIndexMove];
            WzAssert( ec->m_pMove );

            // Visual ����
            WzAssert( ec->m_nIndexVisual >= 0 && (size_t)ec->m_nIndexVisual < m_stdList_Visual.size() );

            ec->m_pVisual = (St_Visual_Info*)m_stdList_Visual[ec->m_nIndexVisual];
            WzAssert( ec->m_pVisual );

            // Change ����
            WzAssert( ec->m_nIndexChange >= 0 && (size_t)ec->m_nIndexChange < m_stdList_Change.size() );

            ec->m_pChange = (St_Change_Info*)m_stdList_Change[ec->m_nIndexChange];
            WzAssert( ec->m_pChange );

            // ���ö��� ����
            CreateSpline( ec );
        }
    }

    // Element ���� ������ ���ö��� ����
    void St_BodyCreate::CreateSpline( St_ElementCreate* ec )
    {
        WzAssert( ec );
        WzAssert( ec->m_pCreate );
        WzAssert( ec->m_pMove );
        WzAssert( ec->m_pChange );

        // �����ϰ� ���⼭�� ����
        DestroySpline( ec );

        // Spline ����

        // ũ��
        ec->m_pcSplineScaleX = new WzNaturalCubicSplineDyn;
        ec->m_pcSplineScaleY = new WzNaturalCubicSplineDyn;
        ec->m_pcSplineScaleZ = new WzNaturalCubicSplineDyn;

        // RGBA
        ec->m_pcSplineRed = new WzNaturalCubicSplineDyn;
        ec->m_pcSplineGreen = new WzNaturalCubicSplineDyn;
        ec->m_pcSplineBlue = new WzNaturalCubicSplineDyn;
        ec->m_pcSplineAlpha = new WzNaturalCubicSplineDyn;

        // ���ӵ�
        ec->m_pcSplineAccelerationX = new WzNaturalCubicSplineDyn;
        ec->m_pcSplineAccelerationY = new WzNaturalCubicSplineDyn;
        ec->m_pcSplineAccelerationZ = new WzNaturalCubicSplineDyn; 

        // �ʱ� ��ġ ȸ��
        ec->m_pcSplinePosRotationX = new WzNaturalCubicSplineDyn;
        ec->m_pcSplinePosRotationY = new WzNaturalCubicSplineDyn;
        ec->m_pcSplinePosRotationZ = new WzNaturalCubicSplineDyn;

        // ���� ȸ��
        ec->m_pcSplineRotationX = new WzNaturalCubicSplineDyn;
        ec->m_pcSplineRotationY = new WzNaturalCubicSplineDyn;
        ec->m_pcSplineRotationZ = new WzNaturalCubicSplineDyn;

        // Spline ����

        std_KeyMap::iterator i;

        // ũ�� X
        WzAssert( ec->m_pcSplineScaleX );
        for( i = ec->m_pChange->m_stdMapKeyScaleX.begin(); i != ec->m_pChange->m_stdMapKeyScaleX.end(); ++i )
        {
            ec->m_pcSplineScaleX->Add( (float)i->first, i->second );    
        }

        ec->m_pcSplineScaleX->Create( ec->m_pChange->m_eGraphTypeScaleX );

        // ũ�� Y
        WzAssert( ec->m_pcSplineScaleY );
        for( i = ec->m_pChange->m_stdMapKeyScaleY.begin(); i != ec->m_pChange->m_stdMapKeyScaleY.end(); ++i )
        {
            ec->m_pcSplineScaleY->Add( (float)i->first, i->second );    
        }

        ec->m_pcSplineScaleY->Create( ec->m_pChange->m_eGraphTypeScaleY );

        // ũ�� Z
        WzAssert( ec->m_pcSplineScaleZ );
        for( i = ec->m_pChange->m_stdMapKeyScaleZ.begin(); i != ec->m_pChange->m_stdMapKeyScaleZ.end(); ++i )
        {
            ec->m_pcSplineScaleZ->Add( (float)i->first, i->second );    
        }

        ec->m_pcSplineScaleZ->Create( ec->m_pChange->m_eGraphTypeScaleZ );

        // R
        WzAssert( ec->m_pcSplineRed );
        for( i = ec->m_pChange->m_stdMapKeyRed.begin(); i != ec->m_pChange->m_stdMapKeyRed.end(); ++i )
        {
            ec->m_pcSplineRed->Add( (float)i->first, i->second );
        }

        ec->m_pcSplineRed->Create( ec->m_pChange->m_eGraphTypeRed );  

        // G
        WzAssert( ec->m_pcSplineGreen );
        for( i = ec->m_pChange->m_stdMapKeyGreen.begin(); i != ec->m_pChange->m_stdMapKeyGreen.end(); ++i )
        {
            ec->m_pcSplineGreen->Add( (float)i->first, i->second ); 
        }

        ec->m_pcSplineGreen->Create( ec->m_pChange->m_eGraphTypeGreen );

        // B
        WzAssert( ec->m_pcSplineBlue );
        for( i = ec->m_pChange->m_stdMapKeyBlue.begin(); i != ec->m_pChange->m_stdMapKeyBlue.end(); ++i )
        {
            ec->m_pcSplineBlue->Add( (float)i->first, i->second );  
        }

        ec->m_pcSplineBlue->Create(ec->m_pChange->m_eGraphTypeBlue);

        // A
        WzAssert( ec->m_pcSplineAlpha );
        for( i = ec->m_pChange->m_stdMapKeyAlpha.begin(); i != ec->m_pChange->m_stdMapKeyAlpha.end(); ++i )
        {
            ec->m_pcSplineAlpha->Add( (float)i->first, i->second ); 
        }

        ec->m_pcSplineAlpha->Create( ec->m_pChange->m_eGraphTypeAlpha );

        // ���ӵ� X
        WzAssert( ec->m_pcSplineAccelerationX );
        for( i = ec->m_pChange->m_stdMapKeyAccelerationX.begin(); i != ec->m_pChange->m_stdMapKeyAccelerationX.end(); ++i )
        {
            ec->m_pcSplineAccelerationX->Add( (float)i->first, i->second ); 
        }

        ec->m_pcSplineAccelerationX->Create( ec->m_pChange->m_eGraphTypeAccelX );

        // ���ӵ� Y
        WzAssert( ec->m_pcSplineAccelerationY );
        for( i = ec->m_pChange->m_stdMapKeyAccelerationY.begin(); i != ec->m_pChange->m_stdMapKeyAccelerationY.end(); ++i )
        {
            ec->m_pcSplineAccelerationY->Add( (float)i->first, i->second ); 
        }

        ec->m_pcSplineAccelerationY->Create( ec->m_pChange->m_eGraphTypeAccelY );

        // ���ӵ� Z
        WzAssert( ec->m_pcSplineAccelerationZ );
        for( i = ec->m_pChange->m_stdMapKeyAccelerationZ.begin(); i != ec->m_pChange->m_stdMapKeyAccelerationZ.end(); ++i )
        {
            ec->m_pcSplineAccelerationZ->Add( (float)i->first, i->second ); 
        }

        ec->m_pcSplineAccelerationZ->Create( ec->m_pChange->m_eGraphTypeAccelZ );

        // �ʱ� ��ġ ȸ�� X
        WzAssert( ec->m_pcSplinePosRotationX );
        for( i = ec->m_pChange->m_stdMapKeyPosRotationX.begin(); i != ec->m_pChange->m_stdMapKeyPosRotationX.end(); ++i )
        {
            ec->m_pcSplinePosRotationX->Add( (float)i->first, i->second );
        }

        ec->m_pcSplinePosRotationX->Create( ec->m_pChange->m_eGraphTypePosRotX );

        // �ʱ� ��ġ ȸ�� Y
        WzAssert( ec->m_pcSplinePosRotationY );
        for( i = ec->m_pChange->m_stdMapKeyPosRotationY.begin(); i != ec->m_pChange->m_stdMapKeyPosRotationY.end(); ++i )
        {
            ec->m_pcSplinePosRotationY->Add( (float)i->first, i->second );      
        }

        ec->m_pcSplinePosRotationY->Create( ec->m_pChange->m_eGraphTypePosRotY );

        // �ʱ� ��ġ ȸ�� Z
        WzAssert( ec->m_pcSplinePosRotationZ );
        for( i = ec->m_pChange->m_stdMapKeyPosRotationZ.begin(); i != ec->m_pChange->m_stdMapKeyPosRotationZ.end(); ++i )
        {
            ec->m_pcSplinePosRotationZ->Add( (float)i->first, i->second );      
        }

        ec->m_pcSplinePosRotationZ->Create( ec->m_pChange->m_eGraphTypePosRotZ );

        // ���� ȸ�� X
        WzAssert( ec->m_pcSplineRotationX );
        for( i = ec->m_pChange->m_stdMapKeyRotationX.begin(); i != ec->m_pChange->m_stdMapKeyRotationX.end(); ++i )
        {
            ec->m_pcSplineRotationX->Add( (float)i->first, i->second );     
        }

        ec->m_pcSplineRotationX->Create( ec->m_pChange->m_eGraphTypeRotX );

        // ���� ȸ�� Y
        WzAssert( ec->m_pcSplineRotationY );
        for( i = ec->m_pChange->m_stdMapKeyRotationY.begin(); i != ec->m_pChange->m_stdMapKeyRotationY.end(); ++i )
        {
            ec->m_pcSplineRotationY->Add( (float)i->first, i->second );     
        }

        ec->m_pcSplineRotationY->Create( ec->m_pChange->m_eGraphTypeRotY );

        // ���� ȸ�� Z
        WzAssert( ec->m_pcSplineRotationZ );
        for( i = ec->m_pChange->m_stdMapKeyRotationZ.begin(); i != ec->m_pChange->m_stdMapKeyRotationZ.end(); ++i )
        {
            ec->m_pcSplineRotationZ->Add( (float)i->first, i->second );     
        }

        ec->m_pcSplineRotationZ->Create( ec->m_pChange->m_eGraphTypeRotZ );

        // Custom (Ŀ���� ���� ������)
        if( ec->m_pCreate->m_ePosType == EPOS_CUSTOM )
        {
            // xxx: �ɸ��� ���� �ʹ� ���Ƽ� �ϴ� �ּ� ó��
            // WzAssert( ec->m_pCreate->m_stdMapVector.size() > 1 );

            if( ec->m_pCreate->m_stdMapVector.size() > 1 )
            {
                ec->m_pcSplineCreateCustomX = new WzNaturalCubicSplineDyn;
                ec->m_pcSplineCreateCustomY = new WzNaturalCubicSplineDyn;
                ec->m_pcSplineCreateCustomZ = new WzNaturalCubicSplineDyn;

                WzAssert( ec->m_pcSplineCreateCustomX );
                WzAssert( ec->m_pcSplineCreateCustomY );
                WzAssert( ec->m_pcSplineCreateCustomZ );

                std_CourseMap::iterator j = ec->m_pCreate->m_stdMapVector.begin();
                for( ; j != ec->m_pCreate->m_stdMapVector.end(); ++j )
                {
                    float t = (float)j->first;
                    const WzVector& wzPos = j->second;

                    ec->m_pcSplineCreateCustomX->Add( t, wzPos.x );
                    ec->m_pcSplineCreateCustomY->Add( t, wzPos.y );
                    ec->m_pcSplineCreateCustomZ->Add( t, wzPos.z );
                }

                ec->m_pcSplineCreateCustomX->Create();
                ec->m_pcSplineCreateCustomY->Create();
                ec->m_pcSplineCreateCustomZ->Create();
            }
            else
            {
                WZLOG( WZWAR, "St_BodyCreate::CreateSpline() - ���� �ڽ� ����" );
            }
        }

        // ���� �̵��� �ƴ� ���
        if( !ec->m_pMove->m_blPhysicsMove )
        {
            // ���� �̵��� �ƴ϶�� �ؼ� �ݵ�� �ڽ� �̵��� �ƴϴ�.
            // ���ڸ����� �������� �ʴ� ���, ������ �ڽ��� �ƴ� �� �ִ�.

            // Course
            if( ec->m_pMove->m_stdMapCourse.size() > 1 )
            {
                ec->m_pcSplineCourseX = new WzNaturalCubicSplineDyn;
                ec->m_pcSplineCourseY = new WzNaturalCubicSplineDyn;
                ec->m_pcSplineCourseZ = new WzNaturalCubicSplineDyn;

                WzAssert( ec->m_pcSplineCourseX );
                WzAssert( ec->m_pcSplineCourseY );
                WzAssert( ec->m_pcSplineCourseZ );

                std_CourseMap::iterator j = ec->m_pMove->m_stdMapCourse.begin();
                for( ; j != ec->m_pMove->m_stdMapCourse.end(); ++j )
                {
                    float t = (float)j->first;
                    const WzVector& wzPos = j->second;

                    ec->m_pcSplineCourseX->Add( t, wzPos.x );
                    ec->m_pcSplineCourseY->Add( t, wzPos.y );
                    ec->m_pcSplineCourseZ->Add( t, wzPos.z );
                }

                ec->m_pcSplineCourseX->Create();
                ec->m_pcSplineCourseY->Create();
                ec->m_pcSplineCourseZ->Create();
            }
        }
    }

    // ���ö��� ����
    void DestroySpline( St_ElementCreate* ec )
    {
        WzAssert( ec );

        if( !ec )
        {
            return;
        }

        // Course
        SAFE_DELETE( ec->m_pcSplineCourseX );
        SAFE_DELETE( ec->m_pcSplineCourseY );
        SAFE_DELETE( ec->m_pcSplineCourseZ );

        // Custom
        SAFE_DELETE( ec->m_pcSplineCreateCustomX );
        SAFE_DELETE( ec->m_pcSplineCreateCustomY );
        SAFE_DELETE( ec->m_pcSplineCreateCustomZ );

        // ���� ȸ��
        SAFE_DELETE( ec->m_pcSplineRotationX );
        SAFE_DELETE( ec->m_pcSplineRotationY );
        SAFE_DELETE( ec->m_pcSplineRotationZ );

        // �ʱ� ��ġ ȸ��
        SAFE_DELETE( ec->m_pcSplinePosRotationX );
        SAFE_DELETE( ec->m_pcSplinePosRotationY );
        SAFE_DELETE( ec->m_pcSplinePosRotationZ );

        // ���ӵ�
        SAFE_DELETE( ec->m_pcSplineAccelerationX );
        SAFE_DELETE( ec->m_pcSplineAccelerationY );
        SAFE_DELETE( ec->m_pcSplineAccelerationZ );

        // RGBA
        SAFE_DELETE( ec->m_pcSplineRed );
        SAFE_DELETE( ec->m_pcSplineGreen );
        SAFE_DELETE( ec->m_pcSplineBlue );
        SAFE_DELETE( ec->m_pcSplineAlpha );

        // ũ��
        SAFE_DELETE( ec->m_pcSplineScaleX );
        SAFE_DELETE( ec->m_pcSplineScaleY );
        SAFE_DELETE( ec->m_pcSplineScaleZ );
    }
#endif //_USE_ENGINE_MEMORY_POOL_
};

#endif // _EFFECT_BODYCREATE_H_