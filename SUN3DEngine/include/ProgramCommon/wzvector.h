// wzvector.h

#ifndef _PROGRAMCOMMON_WZVECTOR_H_
#define _PROGRAMCOMMON_WZVECTOR_H_

#include <windows.h>
#include <math.h>
#include "WzAssert.h"

//------------------------------------------------------------------------------
/**
    @struct WzVector
*/
struct WzVector
{
#pragma warning(push)
#pragma warning(disable:4201)
    union
    {
        struct
        {
            float x, y, z;
        };

        float m_Element[3];
    };
#pragma warning(pop)
    WzVector operator + ( void ) const;
    WzVector operator - ( void ) const;
    WzVector& operator += ( const WzVector& );
    WzVector& operator -= ( const WzVector& );
    WzVector& operator *= ( float );
    WzVector operator + ( const WzVector& ) const;
    WzVector operator - ( const WzVector& ) const;
    WzVector operator * ( float ) const;
    BOOL operator == ( const WzVector& ) const;
    BOOL operator != ( const WzVector& ) const;
};

//------------------------------------------------------------------------------
/**
*/
inline WzVector WzVector::operator + ( void ) const
{
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector WzVector::operator - ( void ) const
{
    WzVector out;

    out.x = -x;
    out.y = -y;
    out.z = -z;

    return out;
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector& WzVector::operator += ( const WzVector& v )
{
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector& WzVector::operator -= ( const WzVector& v )
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector& WzVector::operator *= ( float val )
{
    x *= val;
    y *= val;
    z *= val;

    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector WzVector::operator + ( const WzVector& v ) const
{
    WzVector out;

    out.x = x + v.x;
    out.y = y + v.y;
    out.z = z + v.z;

    return out;
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector WzVector::operator - ( const WzVector& v ) const
{
    WzVector out;

    out.x = x - v.x;
    out.y = y - v.y;
    out.z = z - v.z;

    return out;
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector WzVector::operator * ( float val ) const
{
    WzVector out;

    out.x = x * val;
    out.y = y * val;
    out.z = z * val;

    return out;
}

//------------------------------------------------------------------------------
/**
    xxx : ���� ���� vector �񱳽�(ex : �ͽ���Ʈ ��)

    ���� �Լ� ��� �ɼ��� ���� �ִ� ���
      - �� 1�� �� 2���� ���� ���� ����

    ���� �Լ� ��� �ɼ��� ���� �ִ� ���
      - �� 1�� ���� �� 2�� ����� ��û���� ���� �ɸ�
      - fabs�� �Լ��� inlineȭ ���� ���ϰ� �Լ� ȣ���� �Ǳ� ����
*/
inline BOOL WzVector::operator == ( const WzVector& v ) const
{
    // �� 1
    // if( x != v.x || y != v.y || z != v.z )

    // �� 2
    if( fabs( x - v.x ) > 0.000001f ||
        fabs( y - v.y ) > 0.000001f ||
        fabs( z - v.z ) > 0.000001f )
    {
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL WzVector::operator != ( const WzVector& v ) const
{
    return !( operator == ( v ) );
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector& SetVector( WzVector* out, float x, float y, float z )
{
    WzAssert( out );

    out->x = x;
    out->y = y;
    out->z = z;

    return *out;
}

//------------------------------------------------------------------------------
/**
    v2 -> v1���� t��ŭ ���� (�� �̷��� �ߴ����� ������, ��������� �׷���.)

    ��) v1t + v2(1 - t)
         = v1t + v2 - v2t
         = v2 + (v1 - v2)t
*/
inline WzVector* InterpolateVector( WzVector* out, const WzVector* v1, const WzVector* v2, float t )
{
    WzAssert( out );
    WzAssert( v1 );
    WzAssert( v2 );

    float invTime = 1.0f - t;

    out->x = ( v1->x * t ) + ( v2->x * invTime );
    out->y = ( v1->y * t ) + ( v2->y * invTime );
    out->z = ( v1->z * t ) + ( v2->z * invTime );

    return out;
}

//------------------------------------------------------------------------------
/**
*/
inline float VectorLength2( const WzVector* v )
{
    WzAssert( v );

    return ( ( v->x * v->x ) + ( v->y * v->y ) + ( v->z * v->z ) );
}

//------------------------------------------------------------------------------
/**
*/
inline float VectorLength( const WzVector* v )
{
    WzAssert( v );

    return ( (float)sqrt( VectorLength2( v ) ) );
}

//------------------------------------------------------------------------------
/**
*/
inline float VectorDotProduct( const WzVector* v1, const WzVector* v2 )
{
    WzAssert( v1 );
    WzAssert( v2 );

    return ( ( v1->x * v2->x ) + ( v1->y * v2->y ) + ( v1->z * v2->z ) );
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector* VectorCrossProduct( WzVector* out, const WzVector* v1, const WzVector* v2 )
{
    WzAssert( out );
    WzAssert( v1 );
    WzAssert( v2 );

    out->x = ( v1->y * v2->z ) - ( v1->z * v2->y );
    out->y = ( v1->z * v2->x ) - ( v1->x * v2->z );
    out->z = ( v1->x * v2->y ) - ( v1->y * v2->x );

    return out;
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector* VectorNormalize( WzVector* out, const WzVector* v )
{
    WzAssert( out );
    WzAssert( v );

    float len = VectorLength( v );
    if( len > 0.0f )
    {
        out->x = v->x / len;
        out->y = v->y / len;
        out->z = v->z / len;
    }
    else
    {
        *out = *v;
    }

    return out;
}

#endif // _PROGRAMCOMMON_WZVECTOR_H_