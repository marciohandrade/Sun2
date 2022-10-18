// boundingvolume.h

#ifndef _COMMON_BOUNDINGVOLUME_H_
#define _COMMON_BOUNDINGVOLUME_H_

#include "wzbvtype.h"
#include "wzmath.h"
#include "wzutil.h"

#define NTCT_OUTSIDE    (0x0000)   // �ܺο� ����
#define NTCT_INTERSECT  (0x0001)   // ������
#define NTCT_INSIDE     (0x0002)   // ������ (���� ���� ���� ����)

// World ��ǥ�� ray�� ���� ��ǥ�� �̵�(ray�� ori, dir * inv(worldTM))
WzRay* TransformRay( WzRay* pwrOut, const WzRay* pwrIn, const WzMatrix* pwmWorld );

// ����ü ���� (����)
WzFrustum* Generate_Frustum( WzFrustum* pwfOut, 
                             const WzVector& wvFrom, const WzVector& wvTo, 
                             const WzVector& wvUp, const WzVector& wvRight, 
                             float fFov, float fAspect, 
                             float fNear, float fFar );

// ����ü ���� (����)
WzFrustum* Generate_Frustum_Orthogonal( WzFrustum* pwfOut,
                                        const WzVector& wvFrom, const WzVector& wvTo,
                                        const WzVector& wvUp, const WzVector& wvRight,
                                        float fLeft, float fRight, float fBottom, float fTop,
                                        float fNear, float fFar );

// �ﰢ���� ����ü�� ����
int CutTriangleWithFrustum( WzVector* pwvResult, int* piNumVerInResult, const WzVector* pwvList, const WzFrustum* pwf );

// ���� - �ﰢ�� ���� �˻� (�ܺ�/����)
WORD Intersect_Ray_Triangle( const WzRay* pwr, 
                             const WzVector* pwv0,
                             const WzVector* pwv1,
                             const WzVector* pwv2,
                             float* pfT = NULL,
                             BOOL bFrontFaceOnly = TRUE );

// ���� - �ﰢ�� ����Ʈ ���� �˻�
int Intersect_Ray_TriangleList( const WzRay* pwr,
                                const WzVector* pwvTriangleList,
                                int iNumTriangle,
                                float* pfT = NULL,
                                BOOL bFrontFaceOnly = TRUE,
                                int iDataSize = sizeof( WzVector ) );

// ���� - �ﰢ�� ����Ʈ ���� �˻� (�ε��� ����Ʈ ���)
int Intersect_Ray_TriangleListIndexed( const WzRay* pwr,
                                       const WzVector* pwvTriangleList,
                                       const int* piIndexMap,
                                       int iNumTriangle,
                                       float* pfT = NULL,
                                       BOOL bFrontFaceOnly = TRUE,
                                       int iDataSize = sizeof( WzVector ),
                                       WzVector* pwvNormal = NULL );

// ���� - ��� ���� �˻� (���� - �������� ������ �׻� ������)
WORD Intersect_Ray_Plane( const WzRay* pwr, const WzPlane* pwp, float* pfT = NULL );

// ���� - AABB ���� �˻� (�ܺ�/����)
WORD Intersect_Ray_AABB( const WzRay* pwr, const BvAABB* pbaabb, float* pfT = NULL );
// ���� - �� ���� �˻� (�ܺ�/����)
WORD Intersect_Ray_Sphere( const WzRay* pwr, const BvSphere* pbs, float* pfT = NULL );
// ���� - �ٿ�� ���� ���� �˻� (�ܺ�/����)
WORD Intersect_Ray_Bv( const WzRay* pwr, const WzBoundingVolume* pwbv, float* pfT = NULL );

// ����ü - AABB ���� �˻� (�ܺ�/����/����)
WORD Intersect_Frustum_AABB( const WzFrustum* pwf, const BvAABB* pbaabb );
// ����ü - �� ���� �˻� (�ܺ�/����/����)
WORD Intersect_Frustum_Sphere( const WzFrustum* pwf, const BvSphere* pbs );
// ����ü - �ٿ�� ���� ���� �˻� (�ܺ�/����/����)
WORD Intersect_Frustum_Bv( const WzFrustum* pwf, const WzBoundingVolume* pwbv );

// AABB - AABB ���� �˻� (�ܺ�/����/����)
WORD Intersect_AABB_AABB( const BvAABB* pbaabb1, const BvAABB* pbaabb2 );
// AABB - �� ���� �˻� (�ܺ�/����/����)
WORD Intersect_AABB_Sphere( const BvAABB* pbaabb, const BvSphere* pbs );
// �� - AABB ���� �˻� (�ܺ�/����/����)
WORD Intersect_Sphere_AABB( const BvSphere* pbs, const BvAABB* pbaabb );
// �� - �� ���� �˻� (�ܺ�/����/����)
WORD Intersect_Sphere_Sphere( const BvSphere* pbs1, const BvSphere* pbs2 );
// �ٿ�� ���� - �ٿ�� ���� ���� �˻� (�ܺ�/����/����)
WORD Intersect_Bv_Bv( const WzBoundingVolume* pwbv1, const WzBoundingVolume* pwbv2 );

// AABB - ���ؽ� ���� �˻�
BOOL Contain_AABB_Vertex( const BvAABB* pbaabb, const WzVector& wvVertex );
// �� - ���ؽ� ���� �˻�
BOOL Contain_Sphere_Vertex( const BvSphere* pbs, const WzVector& wvVertex );
// �ٿ�� ���� - ���ؽ� ���� �˻�
BOOL Contain_Bv_Vertex( const WzBoundingVolume* pwbv, const WzVector& wvVertex );

// AABB - AABB ���� �˻�
BOOL Contain_AABB_AABB( const BvAABB* pbaabb1, const BvAABB* pbaabb2 );
// AABB - �� ���� �˻�
BOOL Contain_AABB_Sphere( const BvAABB* pbaabb, const BvSphere* pbs );
// �� - AABB ���� �˻�
BOOL Contain_Sphere_AABB( const BvSphere* pbs, const BvAABB* pbaabb );
// �� - �� ���� �˻�
BOOL Contain_Sphere_Sphere( const BvSphere* pbs1, const BvSphere* pbs2 );
// �ٿ�� ���� - �ٿ�� ���� ���� �˻�
BOOL Contain_Bv_Bv( const WzBoundingVolume* pwbv1, const WzBoundingVolume* pwbv2 );

// ����ü - AABB ���� �˻� (�ܺ�/����,����)
WORD Outside_Frustum_AABB( const WzFrustum* pwf, const BvAABB* pbaabb );
// ����ü - �� ���� �˻� (�ܺ�/����,����)
WORD Outside_Frustum_Sphere( const WzFrustum* pwf, const BvSphere* pbs );
// ����ü - �ٿ�� ���� ���� �˻� (�ܺ�/����,����)
WORD Outside_Frustum_Bv( const WzFrustum* pwf, const WzBoundingVolume* pwbv );
// ����ü - ���ؽ� ���� �˻� (�ܺ�/����,����)
WORD Outside_Frustum_Vertex( const WzFrustum* pwf, const WzVector& wvVertex );

// AABB ����
void Generate_BvAABB( BvAABB* pbaabb, const WzVector* pwvVertices, int iCountVertices, int iSize = sizeof( WzVector ) );
void Generate_BvAABB( BvAABB* pbaabb, const WzVector* pwvVertices, const int* piIndexMap, int iCountVertices, int iSize = sizeof( WzVector ) );

// AABB Ȯ��
void Extend_BvAABB( BvAABB* pbaabb, const WzVector* pwvVertex );

// �ٿ�� �� ����
void Generate_BvSphere( BvSphere* pbs, const WzVector* pwvVertices, int iCountVertices, int iSize = sizeof( WzVector ) );
void Generate_BvSphere( BvSphere* pbs, const WzVector* pwvVertices, const int* piIndexMap, int iCountVertices, int iSize = sizeof( WzVector ) );

//------------------------------------------------------------------------------
/**
    ����ü - �ٿ�� ���� ���� �˻�

    - �ܺ� : �ٿ�� ������ ����ü�� �ܺο� �ִ�.
    - ���� : �ٿ�� ������ ����ü�� �����Ѵ�.
    - ���� : �ٿ�� ������ ����ü�� ���ο� �ִ�.
*/
inline WORD Intersect_Frustum_Bv( const WzFrustum* pwf, const WzBoundingVolume* pwbv )
{
    WzAssert( pwf );
    WzAssert( pwbv );

    switch( pwbv->m_eBvType )
    {
    case BVT_AABB:
        return Intersect_Frustum_AABB( pwf, &pwbv->m_aabb );

    case BVT_SPHERE:
        return Intersect_Frustum_Sphere( pwf, &pwbv->m_BvSphere );
    }

    WZLOG( WZWAR, "Intersect_Frustum_Bv() - Unknown Type(%d)", pwbv->m_eBvType );

    return NTCT_OUTSIDE;
}

//------------------------------------------------------------------------------
/**
    AABB - �� ���� �˻�

    - AABB�� ���� ���� �ϴ���
*/
inline BOOL Contain_AABB_Vertex( const BvAABB* pbaabb, const WzVector& wvVertex )
{
    WzAssert( pbaabb );

    // ���� x��ǥ�� AABB�� min.x���� �۰ų�, max.x���� ū ���
    if( pbaabb->m_wvMin.x > wvVertex.x || wvVertex.x > pbaabb->m_wvMax.x )
    {
        return FALSE;
    }

    // ���� y��ǥ�� AABB�� min.y���� �۰ų�, max.y���� ū ���
    if( pbaabb->m_wvMin.y > wvVertex.y || wvVertex.y > pbaabb->m_wvMax.y )
    {
        return FALSE;
    }

    // ���� z��ǥ�� AABB�� min.z���� �۰ų�, max.z���� ū ���
    if( pbaabb->m_wvMin.z > wvVertex.z || wvVertex.z > pbaabb->m_wvMax.z )
    {
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
    �� - �� ���� �˻�

    - ���� ���� ���� �ϴ���
*/
inline BOOL Contain_Sphere_Vertex( const BvSphere* pbs, const WzVector& wvVertex )
{
    WzAssert( pbs );

    // �� �߽ɿ��� �����ΰ��� ���͸� ���ϰ�
    WzVector wvDist = wvVertex - pbs->m_wvCenter;

    // ������ ����^2�� ���Ѵ�.
    float fDist2 = VectorLength2( &wvDist );

    // ����^2�� �� ������^2 ���� ū ���
    if( fDist2 > pbs->m_fRadius * pbs->m_fRadius )
    {
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
    BV - �� ���� �˻�

    - �ٿ�� ������ ���� ���� �ϴ���
*/
inline BOOL Contain_Bv_Vertex( const WzBoundingVolume* pwbv, const WzVector& wvVertex )
{
    WzAssert( pwbv );

    switch( pwbv->m_eBvType )
    {
    case BVT_AABB:
        return Contain_AABB_Vertex( &pwbv->m_aabb, wvVertex );

    case BVT_SPHERE:
        return Contain_Sphere_Vertex( &pwbv->m_BvSphere, wvVertex );
    }

    WZLOG( WZWAR, "Contain_Bv_Vertex() - Unknown bv type(%d)", pwbv->m_eBvType );

    return FALSE;
}

//------------------------------------------------------------------------------
/**
    ����ü - AABB �ܺ� �˻�

    - ����ü ������ normal�� ����ü �ܺ� ������ (���� ���� �ƴ�)

    - �ܺ� : AABB�� ����ü �ܺο� �ִ�.
    - ���� : AABB�� ����ü ���ο� �ְų� �����Ѵ�.
*/
inline WORD Outside_Frustum_AABB( const WzFrustum* pwf, const BvAABB* pbaabb )
{
    WzAssert( pwf );
    WzAssert( pbaabb );

    // AABB�� min, max ���� �ϴ� ����
    const WzVector* pwvSrcMin = &pbaabb->m_wvMin;
    const WzVector* pwvSrcMax = &pbaabb->m_wvMax;

    // ����ü ��ü ����� ���鼭
    for( int iPlane = 0; iPlane < 6; ++iPlane )
    {
        // ���� ����� ���
        const WzPlane* pwp = &pwf->m_wpPlanes[iPlane];
        WzAssert( pwp );

        // �˻��� �� ����
        static WzVector wvTest;

        // ��� normal�� �� ��ǥ�� 0���� ���� ���, AABB�� max ��ǥ ����
        wvTest.x = ( pwp->m_wvNormal.x < 0.0f) ? pwvSrcMax->x : pwvSrcMin->x;
        wvTest.y = ( pwp->m_wvNormal.y < 0.0f) ? pwvSrcMax->y : pwvSrcMin->y;
        wvTest.z = ( pwp->m_wvNormal.z < 0.0f) ? pwvSrcMax->z : pwvSrcMin->z;

        // ����� ���� �˻� (0���� ũ�ٴ� ���� ��� ��(normal����)�� �ִٴ� ��)
        if( Distance_Plane_Vertex( pwp, wvTest ) > 0.0f )
        {
            return NTCT_OUTSIDE;
        }
    }

    return NTCT_INTERSECT;
}

//------------------------------------------------------------------------------
/**
    ����ü - �� �ܺ� �˻�

    - ����ü ������ normal�� ����ü �ܺ� ������ (���� ���� �ƴ�)

    - �ܺ� : ���� ����ü �ܺο� �ִ�.
    - ���� : ���� ����ü ���ο� �ְų� �����Ѵ�.
*/
inline WORD Outside_Frustum_Sphere( const WzFrustum* pwf, const BvSphere* pbs )
{
    WzAssert( pwf );
    WzAssert( pbs );

    // ����ü ��ü ����� ���鼭
    for( int iPlane = 0; iPlane < 6; ++iPlane )
    {
        // ���� ����� ���
        const WzPlane* pwp = &pwf->m_wpPlanes[iPlane];
        WzAssert( pwp );

        // ���� �߽ɰ� ������ �Ÿ�(��ȣ ����) ���
        float fDist = Distance_Plane_Vertex( pwp, pbs->m_wvCenter );

        // �Ÿ��� ������ ���� ũ�� ��� ��(normal����)�� �ִٴ� ��
        if( fDist > pbs->m_fRadius )
        {
            return NTCT_OUTSIDE; 
        }
    }

    return NTCT_INTERSECT;
}

//------------------------------------------------------------------------------
/**
    ����ü - �ٿ�� ���� �ܺ� �˻�

    - ����ü ������ normal�� ����ü �ܺ� ������ (���� ���� �ƴ�)

    - �ܺ� : �ٿ�� ������ ����ü �ܺο� �ִ�.
    - ���� : �ٿ�� ������ ����ü ���ο� �ְų� �����Ѵ�.
*/
inline WORD Outside_Frustum_Bv( const WzFrustum* pwf, const WzBoundingVolume* pwbv )
{
    WzAssert( pwf );
    WzAssert( pwbv );

    switch( pwbv->m_eBvType )
    {
    case BVT_AABB:
        return Outside_Frustum_AABB( pwf, &pwbv->m_aabb );

    case BVT_SPHERE:
        return Outside_Frustum_Sphere( pwf, &pwbv->m_BvSphere );
    }

    WZLOG( WZWAR, "Outside_Frustum_Bv() - Unknown bv type(%d)", pwbv->m_eBvType );

    return NTCT_OUTSIDE;
}

//------------------------------------------------------------------------------
/**
*/
inline WORD Outside_Frustum_Vertex( const WzFrustum* pwf, const WzVector& wvVertex )
{
    WzAssert( pwf );

    // ����ü ��ü ����� ���鼭
    for( int iPlane = 0; iPlane < 6; ++iPlane )
    {
        // ���� ����� ���
        const WzPlane* pwp = &pwf->m_wpPlanes[iPlane];
        WzAssert( pwp );

        // ���� ������ �Ÿ��� 0 ���� ũ�� ��� ��(normal����)�� �ִٴ� ��
        if( Distance_Plane_Vertex( pwp, wvVertex ) > 0.0f )
        {
            return NTCT_OUTSIDE; 
        }
    }

    return NTCT_INTERSECT;
}

//------------------------------------------------------------------------------
/**
    AABB Ȯ��

    - �־��� ���� �����ϵ��� AABB Ȯ��
*/
inline void Extend_BvAABB( BvAABB* pbaabb, const WzVector* pwvVertex )
{
    WzAssert( pbaabb );
    WzAssert( pwvVertex );

    // x, y, z�� ���鼭
    for( int i = 0; i < 3; ++i )
    {
        // �־��� ���� ��ǥ�� AABB�� min�� ���� ���� ���
        if( pwvVertex->m_Element[i] < pbaabb->m_wvMin.m_Element[i] )
        {
            // AABB�� min�� ����
            pbaabb->m_wvMin.m_Element[i] = pwvVertex->m_Element[i];
        }

        // �־��� ���� ��ǥ�� AABB�� max�� ���� ū ���
        if( pwvVertex->m_Element[i] > pbaabb->m_wvMax.m_Element[i] )
        {
            // AABB�� max�� ����
            pbaabb->m_wvMax.m_Element[i] = pwvVertex->m_Element[i];
        }
    }
}

#endif // _COMMON_BOUNDINGVOLUME_H_
