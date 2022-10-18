// wzutil.h

#ifndef _COMMON_WZUTIL_H_
#define _COMMON_WZUTIL_H_

#include "wzplane.h"

// x, y �� normal, z �� k  (a*x + b*y + k = 0)
typedef WzVector Wz2DLine;

// v1v2, v1v3 �� ������ ��� ���� ���ϱ�
WzVector* CalculateNormal( WzVector* pwvOut, const WzVector* pwv1, const WzVector* pwv2, const WzVector* pwv3 );

// �� ������ ��� ���ϱ�
WzPlane* GetPlaneFromPoints( WzPlane* pwpOut, const WzVector& wv1, const WzVector& wv2, const WzVector& wv3 );

// ���� ������ �Ÿ� ���ϱ�
float Distance_Plane_Vertex( const WzPlane* pwpPlane, const WzVector& wvVertex );

// �������� ������� �ڸ���
BOOL CutPolygonWithPlane( WzVector* pwvResult, int* piNumResult, const WzVector* pwvPolygon, int iNumVertInPolygon, const WzPlane* pwp );

// 2D - �� ���� ���� ���ϱ�
float As2D_DotProduct( const WzVector* pwv1, const WzVector* pwv2 );

// 2D - �� ���� �մ� ������ ���� ���ϱ�
WzVector* As2D_GetNormal( WzVector* pwvOut, const WzVector* pwv1, const WzVector* pwv2 );

// 2D - �� ���� �մ� ���� ���ϱ�
Wz2DLine* As2D_GetLine( Wz2DLine* pw2lOut, const WzVector* pwv1, const WzVector* pwv2 );

// 2D - ������ ������ �Ÿ� ���ϱ�
float As2D_Distance_Line_Vertex( const Wz2DLine* pw2lLine, const WzVector& wvVertex );

// 2D - �� ������ ���� ����
BOOL As2D_TestParallel( const Wz2DLine* pw2lLine1, const Wz2DLine* pw2lLine2 );

// 2D - �� ������ ������ ���ϱ�
WzVector* As2D_GetCrossPointOfTwoLine( WzVector* pwvOut, const Wz2DLine* pw2lLine1, const Wz2DLine* pw2lLine2 );

// 2D - �� ����(���� �ƴ�)�� ���� ����
BOOL As2D_TestCrossTwoSegment( const WzVector* pwv1, const WzVector* pwv2, const WzVector* pwv3, const WzVector* pwv4, BOOL* pbOnOtherSegment = NULL );

//------------------------------------------------------------------------------
/**
*/
inline WzVector* CalculateNormal( WzVector* pwvOut, const WzVector* pwv1, const WzVector* pwv2, const WzVector* pwv3 )
{
    WzAssert( pwvOut );
    WzAssert( pwv1 );
    WzAssert( pwv2 );
    WzAssert( pwv3 );

    WzVector wvD1 = *pwv2 - *pwv1;
    WzVector wvD2 = *pwv3 - *pwv1;

    WzVector wvCrossProduct;

    VectorCrossProduct( &wvCrossProduct, &wvD1, &wvD2 );
    VectorNormalize( pwvOut, &wvCrossProduct );

    return pwvOut;
}

//------------------------------------------------------------------------------
/**
*/
inline WzPlane* GetPlaneFromPoints( WzPlane* pwpOut, const WzVector& wv1, const WzVector& wv2, const WzVector& wv3 )
{
    WzAssert( pwpOut );

    // normal ���ϱ�
    CalculateNormal( &pwpOut->m_wvNormal, &wv1, &wv2, &wv3 );

    // d ���ϱ�
    pwpOut->m_fD = -VectorDotProduct( &pwpOut->m_wvNormal, &wv1 );

    return pwpOut;
}

//------------------------------------------------------------------------------
/**
    > 0 : ��� ���� (normal ����)
    < 0 : ��� ���� (normal �ݴ� ����)
    = 0 : ��鿡 ����
*/
inline float Distance_Plane_Vertex( const WzPlane* pwpPlane, const WzVector& wvVertex )
{
    WzAssert( pwpPlane );

    return ( pwpPlane->a * wvVertex.x + pwpPlane->b * wvVertex.y + pwpPlane->c * wvVertex.z + pwpPlane->d );
}

//------------------------------------------------------------------------------
/**
*/
inline float As2D_DotProduct( const WzVector* pwv1, const WzVector* pwv2 )
{
    WzAssert( pwv1 );
    WzAssert( pwv2 );

    return ( pwv1->x * pwv2->x + pwv1->y * pwv2->y );
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector* As2D_GetNormal( WzVector* pwvOut, const WzVector* pwv1, const WzVector* pwv2 )
{
    WzAssert( pwvOut );
    WzAssert( pwv1 );
    WzAssert( pwv2 );

    pwvOut->x = -( pwv2->y - pwv1->y);
    pwvOut->y = pwv2->x - pwv1->x;

    float fScale = 1.0f / (float)sqrt( pwvOut->x * pwvOut->x + pwvOut->y * pwvOut->y );

    pwvOut->x *= fScale;
    pwvOut->y *= fScale;
    pwvOut->z = 0.0f;

    return pwvOut;
}

//------------------------------------------------------------------------------
/**
*/
inline Wz2DLine* As2D_GetLine( Wz2DLine* pw2lOut, const WzVector* pwv1, const WzVector* pwv2 )
{
    WzAssert( pw2lOut );
    WzAssert( pwv1 );
    WzAssert( pwv2 );

    pw2lOut->x = -( pwv2->y - pwv1->y);
    pw2lOut->y = pwv2->x - pwv1->x;

    float fScale = 1.0f / (float)sqrt( pw2lOut->x * pw2lOut->x + pw2lOut->y * pw2lOut->y );

    pw2lOut->x *= fScale;
    pw2lOut->y *= fScale;
    pw2lOut->z = -As2D_DotProduct( pwv1, pw2lOut );

    return pw2lOut;
}

//------------------------------------------------------------------------------
/**
    > 0 : normal ����
    < 0 : normal �ݴ� ����
*/
inline float As2D_Distance_Line_Vertex( const Wz2DLine* pw2lLine, const WzVector& wvVertex )
{
    WzAssert( pw2lLine );

    return ( pw2lLine->x * wvVertex.x + pw2lLine->y * wvVertex.y + pw2lLine->z );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL As2D_TestParallel( const Wz2DLine* pw2lLine1, const Wz2DLine* pw2lLine2 )
{
    WzAssert( pw2lLine1 );
    WzAssert( pw2lLine2 );

    float fTest = (float)fabs( pw2lLine1->x * pw2lLine2->y - pw2lLine1->y * pw2lLine2->x );

    return ( fTest < WZ_EPSILON );
}

//------------------------------------------------------------------------------
/**
*/
namespace wzutil
{
    template <class T>
    inline void swap( T &a, T &b )
    {
        T c = a;
        a = b;
        b = c;
    }
}

#endif // _COMMON_WZUTIL_H_
