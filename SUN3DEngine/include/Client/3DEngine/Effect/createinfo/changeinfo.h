// changeinfo.h

#ifndef _EFFECT_CHANGEINFO_H_
#define _EFFECT_CHANGEINFO_H_

#include "../typedef.h"

// �׷��� ����.
enum ENUM_MAP_KIND
{
    // ũ��
    EM_SCALEX,
    EM_SCALEY,
    EM_SCALEZ,

    // RGBA
    EM_RED,
    EM_GREEN,
    EM_BLUE,
    EM_ALPHA,

    // �ӵ�
    EM_ACCELERATIONX,
    EM_ACCELERATIONY,
    EM_ACCELERATIONZ,

    // �ʱ� ��ġ ȸ��
    EM_POSROTATIONX,    
    EM_POSROTATIONY,
    EM_POSROTATIONZ,

    // ���� ȸ��.
    EM_ROTATIONX,
    EM_ROTATIONY,
    EM_ROTATIONZ,
};

//------------------------------------------------------------------------------
/**
    @struct St_Change_Info

    - ��° �̷��� © ������ ������?;
    - �� ���� ���� ������ �Ʒ� �������� �ٷ� ���� ������
      ����μ� ��ġ�Ⱑ �뷫 �����̴�. �ϴ��� �׳� ����.

      * std_KeyMap�� <������, ��> ���̴�.
*/
struct St_Change_Info
{
    // ũ��
    std_KeyMap m_stdMapKeyScaleX;
    std_KeyMap m_stdMapKeyScaleY;
    std_KeyMap m_stdMapKeyScaleZ;
    
    // RGBA
    std_KeyMap m_stdMapKeyRed;
    std_KeyMap m_stdMapKeyGreen;
    std_KeyMap m_stdMapKeyBlue;
    std_KeyMap m_stdMapKeyAlpha;

    // ���ӵ�
    std_KeyMap m_stdMapKeyAccelerationX;
    std_KeyMap m_stdMapKeyAccelerationY;
    std_KeyMap m_stdMapKeyAccelerationZ;

    // �ʱ� ��ġ ȸ��
    std_KeyMap m_stdMapKeyPosRotationX;
    std_KeyMap m_stdMapKeyPosRotationY;
    std_KeyMap m_stdMapKeyPosRotationZ;

    // ���� ȸ��
    std_KeyMap m_stdMapKeyRotationX;
    std_KeyMap m_stdMapKeyRotationY;
    std_KeyMap m_stdMapKeyRotationZ;

    // �׷��� Ÿ��
    ENUM_GRAPH_TYPE m_eGraphTypeScaleX;
    ENUM_GRAPH_TYPE m_eGraphTypeScaleY;
    ENUM_GRAPH_TYPE m_eGraphTypeScaleZ;
    
    ENUM_GRAPH_TYPE m_eGraphTypeRed;
    ENUM_GRAPH_TYPE m_eGraphTypeGreen;
    ENUM_GRAPH_TYPE m_eGraphTypeBlue;
    ENUM_GRAPH_TYPE m_eGraphTypeAlpha;

    ENUM_GRAPH_TYPE m_eGraphTypeAccelX;
    ENUM_GRAPH_TYPE m_eGraphTypeAccelY;
    ENUM_GRAPH_TYPE m_eGraphTypeAccelZ;

    ENUM_GRAPH_TYPE m_eGraphTypePosRotX;
    ENUM_GRAPH_TYPE m_eGraphTypePosRotY;
    ENUM_GRAPH_TYPE m_eGraphTypePosRotZ;

    ENUM_GRAPH_TYPE m_eGraphTypeRotX;
    ENUM_GRAPH_TYPE m_eGraphTypeRotY;
    ENUM_GRAPH_TYPE m_eGraphTypeRotZ;

    // ���� ���ӵ� ����?
    bool m_bRatioAccel;

    // ������
    St_Change_Info()    
    {
        Initialize();
    }

    // �ʱ�ȭ
    void Initialize()
    {
        // memset���� �ʱ�ȭ �ϸ� �� ��

        Release();

        // �׷��� Ÿ��
        m_eGraphTypeScaleX = EGT_LINE;
        m_eGraphTypeScaleY = EGT_LINE;
        m_eGraphTypeScaleZ = EGT_LINE;

        m_eGraphTypeRed = EGT_LINE;
        m_eGraphTypeGreen = EGT_LINE;
        m_eGraphTypeBlue = EGT_LINE;
        m_eGraphTypeAlpha = EGT_LINE;

        m_eGraphTypeAccelX = EGT_LINE;
        m_eGraphTypeAccelY = EGT_LINE;
        m_eGraphTypeAccelZ = EGT_LINE;

        m_eGraphTypePosRotX = EGT_LINE;
        m_eGraphTypePosRotY = EGT_LINE;
        m_eGraphTypePosRotZ = EGT_LINE;

        m_eGraphTypeRotX = EGT_LINE;
        m_eGraphTypeRotY = EGT_LINE;
        m_eGraphTypeRotZ = EGT_LINE;

        m_bRatioAccel = false;
    }

    // ����Ÿ ����
    void Release()
    {
        // ���� ȸ��
        m_stdMapKeyRotationX.clear();
        m_stdMapKeyRotationY.clear();
        m_stdMapKeyRotationZ.clear();

        // �ʱ� ��ġ ȸ��
        m_stdMapKeyPosRotationX.clear();
        m_stdMapKeyPosRotationY.clear();
        m_stdMapKeyPosRotationZ.clear();

        // ���ӵ�
        m_stdMapKeyAccelerationX.clear();
        m_stdMapKeyAccelerationY.clear();
        m_stdMapKeyAccelerationZ.clear();

        // RGBA
        m_stdMapKeyRed.clear();
        m_stdMapKeyGreen.clear();
        m_stdMapKeyBlue.clear();
        m_stdMapKeyAlpha.clear();

        // ũ��
        m_stdMapKeyScaleX.clear();
        m_stdMapKeyScaleY.clear();
        m_stdMapKeyScaleZ.clear();
    }

    // �ش� �ʿ� ����Ÿ �߰�
    void Add( int t, float val, ENUM_MAP_KIND eKind )
    {
        WzAssert( t >= 0 );

        switch( eKind )
        {
        case EM_SCALEX:
            m_stdMapKeyScaleX[t] = val;
            return;

        case EM_SCALEY:
            m_stdMapKeyScaleY[t] = val;
            return;

        case EM_SCALEZ:
            m_stdMapKeyScaleZ[t] = val;
            return;

        case EM_RED:
            m_stdMapKeyRed[t] = val;
            return;

        case EM_GREEN:
            m_stdMapKeyGreen[t] = val;
            return;

        case EM_BLUE:
            m_stdMapKeyBlue[t] = val;
            return;

        case EM_ALPHA:
            m_stdMapKeyAlpha[t] = val;
            return;

        case EM_ACCELERATIONX:
            m_stdMapKeyAccelerationX[t] = val;
            return;

        case EM_ACCELERATIONY:
            m_stdMapKeyAccelerationY[t] = val;
            return;

        case EM_ACCELERATIONZ:
            m_stdMapKeyAccelerationZ[t] = val;
            return;

        case EM_POSROTATIONX:
            m_stdMapKeyPosRotationX[t] = val;
            return;

        case EM_POSROTATIONY:
            m_stdMapKeyPosRotationY[t] = val;
            return;

        case EM_POSROTATIONZ:           
            m_stdMapKeyPosRotationZ[t] = val;
            return;

        case EM_ROTATIONX:
            m_stdMapKeyRotationX[t] = val;
            return;

        case EM_ROTATIONY:
            m_stdMapKeyRotationY[t] = val;
            return;

        case EM_ROTATIONZ:
            m_stdMapKeyRotationZ[t] = val;
            return;
        }
    }

    // �ش� �ʿ��� ����Ÿ ����
    void RemoveAt( int t, ENUM_MAP_KIND eKind )
    {
        WzAssert( t >= 0 );

        switch( eKind )
        {
        case EM_SCALEX:
            {
                std_KeyMap::iterator it = m_stdMapKeyScaleX.find( t );
                if( it != m_stdMapKeyScaleX.end() )
                {                               
                    m_stdMapKeyScaleX.erase( it );
                }
            }
            return;

        case EM_SCALEY:
            {
                std_KeyMap::iterator it = m_stdMapKeyScaleY.find( t );
                if( it != m_stdMapKeyScaleY.end() )
                {                               
                    m_stdMapKeyScaleY.erase( it );
                }
            }
            return;

        case EM_SCALEZ:
            {
                std_KeyMap::iterator it = m_stdMapKeyScaleZ.find( t );
                if( it != m_stdMapKeyScaleZ.end() )
                {                               
                    m_stdMapKeyScaleZ.erase( it );
                }
            }
            return;

        case EM_RED:
            {
                std_KeyMap::iterator it = m_stdMapKeyRed.find( t );
                if( it != m_stdMapKeyRed.end() )
                {                       
                    m_stdMapKeyRed.erase( it );
                }
            }
            return;

        case EM_GREEN:
            {
                std_KeyMap::iterator it = m_stdMapKeyGreen.find( t );
                if( it != m_stdMapKeyGreen.end() )
                {                       
                    m_stdMapKeyGreen.erase( it );
                }
            }
            return;

        case EM_BLUE:
            {
                std_KeyMap::iterator it = m_stdMapKeyBlue.find( t );
                if( it != m_stdMapKeyBlue.end() )
                {                               
                    m_stdMapKeyBlue.erase( it );
                }
            }
            return;

        case EM_ALPHA:
            {
                std_KeyMap::iterator it = m_stdMapKeyAlpha.find( t );
                if( it != m_stdMapKeyAlpha.end() )
                {
                    m_stdMapKeyAlpha.erase( it );
                }
            }
            return;

        case EM_ACCELERATIONX:
            {
                std_KeyMap::iterator it = m_stdMapKeyAccelerationX.find( t );
                if( it != m_stdMapKeyAccelerationX.end() )
                {               
                    m_stdMapKeyAccelerationX.erase( it );
                }
            }
            return;

        case EM_ACCELERATIONY:
            {
                std_KeyMap::iterator it = m_stdMapKeyAccelerationY.find( t );
                if( it != m_stdMapKeyAccelerationY.end() )
                {               
                    m_stdMapKeyAccelerationY.erase( it );
                }
            }
            return;

        case EM_ACCELERATIONZ:
            {
                std_KeyMap::iterator it = m_stdMapKeyAccelerationZ.find( t );
                if( it != m_stdMapKeyAccelerationZ.end() )
                {               
                    m_stdMapKeyAccelerationZ.erase( it );
                }
            }
            return;

        case EM_POSROTATIONX:
            {
                std_KeyMap::iterator it = m_stdMapKeyPosRotationX.find( t );
                if( it != m_stdMapKeyPosRotationX.end() )
                {               
                    m_stdMapKeyPosRotationX.erase( it );
                }
            }
            return;

        case EM_POSROTATIONY:
            {
                std_KeyMap::iterator it = m_stdMapKeyPosRotationY.find( t );
                if( it != m_stdMapKeyPosRotationY.end() )
                {               
                    m_stdMapKeyPosRotationY.erase( it );
                }
            }
            return;

        case EM_POSROTATIONZ:
            {
                std_KeyMap::iterator it = m_stdMapKeyPosRotationZ.find( t );
                if( it != m_stdMapKeyPosRotationZ.end() )
                {               
                    m_stdMapKeyPosRotationZ.erase( it );
                }
            }
            return;

        case EM_ROTATIONX:
            {
                std_KeyMap::iterator it = m_stdMapKeyRotationX.find( t );
                if( it != m_stdMapKeyRotationX.end() )
                {               
                    m_stdMapKeyRotationX.erase( it );
                }
            }
            return;

        case EM_ROTATIONY:
            {
                std_KeyMap::iterator it = m_stdMapKeyRotationY.find( t );
                if( it != m_stdMapKeyRotationY.end() )
                {               
                    m_stdMapKeyRotationY.erase( it );
                }
            }
            return;

        case EM_ROTATIONZ:
            {
                std_KeyMap::iterator it = m_stdMapKeyRotationZ.find( t );
                if( it != m_stdMapKeyRotationZ.end() )
                {               
                    m_stdMapKeyRotationZ.erase( it );
                }
            }
            return;
        }       
    }

    // operator = 
    St_Change_Info& operator = ( const St_Change_Info& rhs )
    {
        if( &rhs == this )
        {
            return *this;
        }

        // memcpy�� ���� �ϸ� �� ��
        // std::map�� �ֱ� ������ ������

        // ũ��
        m_stdMapKeyScaleX = rhs.m_stdMapKeyScaleX;
        m_stdMapKeyScaleY = rhs.m_stdMapKeyScaleY;
        m_stdMapKeyScaleZ = rhs.m_stdMapKeyScaleZ;
        
        // RGBA
        m_stdMapKeyRed = rhs.m_stdMapKeyRed;
        m_stdMapKeyGreen = rhs.m_stdMapKeyGreen;
        m_stdMapKeyBlue = rhs.m_stdMapKeyBlue;
        m_stdMapKeyAlpha = rhs.m_stdMapKeyAlpha;

        // ���ӵ�
        m_stdMapKeyAccelerationX = rhs.m_stdMapKeyAccelerationX;
        m_stdMapKeyAccelerationY = rhs.m_stdMapKeyAccelerationY;
        m_stdMapKeyAccelerationZ = rhs.m_stdMapKeyAccelerationZ;

        // �ʱ� ��ġ ȸ��
        m_stdMapKeyPosRotationX = rhs.m_stdMapKeyPosRotationX;
        m_stdMapKeyPosRotationY = rhs.m_stdMapKeyPosRotationY;
        m_stdMapKeyPosRotationZ = rhs.m_stdMapKeyPosRotationZ;

        // ���� ȸ��
        m_stdMapKeyRotationX = rhs.m_stdMapKeyRotationX;
        m_stdMapKeyRotationY = rhs.m_stdMapKeyRotationY;
        m_stdMapKeyRotationZ = rhs.m_stdMapKeyRotationZ;

        // �׷��� Ÿ��
        m_eGraphTypeScaleX = rhs.m_eGraphTypeScaleX;
        m_eGraphTypeScaleY = rhs.m_eGraphTypeScaleY;
        m_eGraphTypeScaleZ = rhs.m_eGraphTypeScaleZ;
        
        m_eGraphTypeRed = rhs.m_eGraphTypeRed;
        m_eGraphTypeGreen = rhs.m_eGraphTypeGreen;
        m_eGraphTypeBlue = rhs.m_eGraphTypeBlue;
        m_eGraphTypeAlpha = rhs.m_eGraphTypeAlpha;

        m_eGraphTypeAccelX = rhs.m_eGraphTypeAccelX;
        m_eGraphTypeAccelY = rhs.m_eGraphTypeAccelY;
        m_eGraphTypeAccelZ = rhs.m_eGraphTypeAccelZ;

        m_eGraphTypePosRotX = rhs.m_eGraphTypePosRotX;
        m_eGraphTypePosRotY = rhs.m_eGraphTypePosRotY;
        m_eGraphTypePosRotZ = rhs.m_eGraphTypePosRotZ;

        m_eGraphTypeRotX = rhs.m_eGraphTypeRotX;
        m_eGraphTypeRotY = rhs.m_eGraphTypeRotY;
        m_eGraphTypeRotZ = rhs.m_eGraphTypeRotZ;

        // ���� ���ӵ� ����?
        m_bRatioAccel = rhs.m_bRatioAccel;

        return *this;
    }
};

#endif // _EFFECT_CHANGEINFO_H_