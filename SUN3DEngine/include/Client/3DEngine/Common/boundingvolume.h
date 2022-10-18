// boundingvolume.h

#ifndef _COMMON_BOUNDINGVOLUME_H_
#define _COMMON_BOUNDINGVOLUME_H_

#include "wzbvtype.h"
#include "wzmath.h"
#include "wzutil.h"

#define NTCT_OUTSIDE    (0x0000)   // 외부에 있음
#define NTCT_INTERSECT  (0x0001)   // 교차함
#define NTCT_INSIDE     (0x0002)   // 포함함 (앞의 것이 뒤의 것을)

// World 좌표의 ray를 로컬 좌표로 이동(ray의 ori, dir * inv(worldTM))
WzRay* TransformRay( WzRay* pwrOut, const WzRay* pwrIn, const WzMatrix* pwmWorld );

// 절두체 생성 (원근)
WzFrustum* Generate_Frustum( WzFrustum* pwfOut, 
                             const WzVector& wvFrom, const WzVector& wvTo, 
                             const WzVector& wvUp, const WzVector& wvRight, 
                             float fFov, float fAspect, 
                             float fNear, float fFar );

// 절두체 생성 (직교)
WzFrustum* Generate_Frustum_Orthogonal( WzFrustum* pwfOut,
                                        const WzVector& wvFrom, const WzVector& wvTo,
                                        const WzVector& wvUp, const WzVector& wvRight,
                                        float fLeft, float fRight, float fBottom, float fTop,
                                        float fNear, float fFar );

// 삼각형을 절두체로 절단
int CutTriangleWithFrustum( WzVector* pwvResult, int* piNumVerInResult, const WzVector* pwvList, const WzFrustum* pwf );

// 광선 - 삼각형 교차 검사 (외부/교차)
WORD Intersect_Ray_Triangle( const WzRay* pwr, 
                             const WzVector* pwv0,
                             const WzVector* pwv1,
                             const WzVector* pwv2,
                             float* pfT = NULL,
                             BOOL bFrontFaceOnly = TRUE );

// 광선 - 삼각형 리스트 교차 검사
int Intersect_Ray_TriangleList( const WzRay* pwr,
                                const WzVector* pwvTriangleList,
                                int iNumTriangle,
                                float* pfT = NULL,
                                BOOL bFrontFaceOnly = TRUE,
                                int iDataSize = sizeof( WzVector ) );

// 광선 - 삼각형 리스트 교차 검사 (인덱스 리스트 사용)
int Intersect_Ray_TriangleListIndexed( const WzRay* pwr,
                                       const WzVector* pwvTriangleList,
                                       const int* piIndexMap,
                                       int iNumTriangle,
                                       float* pfT = NULL,
                                       BOOL bFrontFaceOnly = TRUE,
                                       int iDataSize = sizeof( WzVector ),
                                       WzVector* pwvNormal = NULL );

// 광선 - 평면 교차 검사 (교차 - 평행하지 않으면 항상 교차함)
WORD Intersect_Ray_Plane( const WzRay* pwr, const WzPlane* pwp, float* pfT = NULL );

// 광선 - AABB 교차 검사 (외부/교차)
WORD Intersect_Ray_AABB( const WzRay* pwr, const BvAABB* pbaabb, float* pfT = NULL );
// 광선 - 구 교차 검사 (외부/교차)
WORD Intersect_Ray_Sphere( const WzRay* pwr, const BvSphere* pbs, float* pfT = NULL );
// 광선 - 바운딩 볼륨 교차 검사 (외부/교차)
WORD Intersect_Ray_Bv( const WzRay* pwr, const WzBoundingVolume* pwbv, float* pfT = NULL );

// 절두체 - AABB 교차 검사 (외부/교차/포함)
WORD Intersect_Frustum_AABB( const WzFrustum* pwf, const BvAABB* pbaabb );
// 절두체 - 구 교차 검사 (외부/교차/포함)
WORD Intersect_Frustum_Sphere( const WzFrustum* pwf, const BvSphere* pbs );
// 절두체 - 바운딩 볼륨 교차 검사 (외부/교차/포함)
WORD Intersect_Frustum_Bv( const WzFrustum* pwf, const WzBoundingVolume* pwbv );

// AABB - AABB 교차 검사 (외부/교차/포함)
WORD Intersect_AABB_AABB( const BvAABB* pbaabb1, const BvAABB* pbaabb2 );
// AABB - 구 교차 검사 (외부/교차/포함)
WORD Intersect_AABB_Sphere( const BvAABB* pbaabb, const BvSphere* pbs );
// 구 - AABB 교차 검사 (외부/교차/포함)
WORD Intersect_Sphere_AABB( const BvSphere* pbs, const BvAABB* pbaabb );
// 구 - 구 교차 검사 (외부/교차/포함)
WORD Intersect_Sphere_Sphere( const BvSphere* pbs1, const BvSphere* pbs2 );
// 바운딩 볼륨 - 바운딩 볼륨 교차 검사 (외부/교차/포함)
WORD Intersect_Bv_Bv( const WzBoundingVolume* pwbv1, const WzBoundingVolume* pwbv2 );

// AABB - 버텍스 포함 검사
BOOL Contain_AABB_Vertex( const BvAABB* pbaabb, const WzVector& wvVertex );
// 구 - 버텍스 포함 검사
BOOL Contain_Sphere_Vertex( const BvSphere* pbs, const WzVector& wvVertex );
// 바운딩 볼륨 - 버텍스 포함 검사
BOOL Contain_Bv_Vertex( const WzBoundingVolume* pwbv, const WzVector& wvVertex );

// AABB - AABB 포함 검사
BOOL Contain_AABB_AABB( const BvAABB* pbaabb1, const BvAABB* pbaabb2 );
// AABB - 구 포함 검사
BOOL Contain_AABB_Sphere( const BvAABB* pbaabb, const BvSphere* pbs );
// 구 - AABB 포함 검사
BOOL Contain_Sphere_AABB( const BvSphere* pbs, const BvAABB* pbaabb );
// 구 - 구 포함 검사
BOOL Contain_Sphere_Sphere( const BvSphere* pbs1, const BvSphere* pbs2 );
// 바운딩 볼륨 - 바운딩 볼륨 포함 검사
BOOL Contain_Bv_Bv( const WzBoundingVolume* pwbv1, const WzBoundingVolume* pwbv2 );

// 절두체 - AABB 교차 검사 (외부/교차,포함)
WORD Outside_Frustum_AABB( const WzFrustum* pwf, const BvAABB* pbaabb );
// 절두체 - 구 교차 검사 (외부/교차,포함)
WORD Outside_Frustum_Sphere( const WzFrustum* pwf, const BvSphere* pbs );
// 절두체 - 바운딩 볼륨 교차 검사 (외부/교차,포함)
WORD Outside_Frustum_Bv( const WzFrustum* pwf, const WzBoundingVolume* pwbv );
// 절두체 - 버텍스 교차 검사 (외부/교차,포함)
WORD Outside_Frustum_Vertex( const WzFrustum* pwf, const WzVector& wvVertex );

// AABB 생성
void Generate_BvAABB( BvAABB* pbaabb, const WzVector* pwvVertices, int iCountVertices, int iSize = sizeof( WzVector ) );
void Generate_BvAABB( BvAABB* pbaabb, const WzVector* pwvVertices, const int* piIndexMap, int iCountVertices, int iSize = sizeof( WzVector ) );

// AABB 확장
void Extend_BvAABB( BvAABB* pbaabb, const WzVector* pwvVertex );

// 바운딩 구 생성
void Generate_BvSphere( BvSphere* pbs, const WzVector* pwvVertices, int iCountVertices, int iSize = sizeof( WzVector ) );
void Generate_BvSphere( BvSphere* pbs, const WzVector* pwvVertices, const int* piIndexMap, int iCountVertices, int iSize = sizeof( WzVector ) );

//------------------------------------------------------------------------------
/**
    절두체 - 바운딩 볼륨 교차 검사

    - 외부 : 바운딩 볼륨이 절두체의 외부에 있다.
    - 교차 : 바운딩 볼륨이 절두체와 교차한다.
    - 내부 : 바운딩 볼륨이 절두체의 내부에 있다.
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
    AABB - 점 포함 검사

    - AABB가 점을 포함 하는지
*/
inline BOOL Contain_AABB_Vertex( const BvAABB* pbaabb, const WzVector& wvVertex )
{
    WzAssert( pbaabb );

    // 점의 x좌표가 AABB의 min.x보다 작거나, max.x보다 큰 경우
    if( pbaabb->m_wvMin.x > wvVertex.x || wvVertex.x > pbaabb->m_wvMax.x )
    {
        return FALSE;
    }

    // 점의 y좌표가 AABB의 min.y보다 작거나, max.y보다 큰 경우
    if( pbaabb->m_wvMin.y > wvVertex.y || wvVertex.y > pbaabb->m_wvMax.y )
    {
        return FALSE;
    }

    // 점의 z좌표가 AABB의 min.z보다 작거나, max.z보다 큰 경우
    if( pbaabb->m_wvMin.z > wvVertex.z || wvVertex.z > pbaabb->m_wvMax.z )
    {
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
    구 - 점 포함 검사

    - 구가 점을 포함 하는지
*/
inline BOOL Contain_Sphere_Vertex( const BvSphere* pbs, const WzVector& wvVertex )
{
    WzAssert( pbs );

    // 구 중심에서 점으로가는 벡터를 구하고
    WzVector wvDist = wvVertex - pbs->m_wvCenter;

    // 벡터의 길이^2를 구한다.
    float fDist2 = VectorLength2( &wvDist );

    // 길이^2이 구 반지름^2 보다 큰 경우
    if( fDist2 > pbs->m_fRadius * pbs->m_fRadius )
    {
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
    BV - 점 포함 검사

    - 바운딩 볼륨이 점을 포함 하는지
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
    절두체 - AABB 외부 검사

    - 절두체 평면들의 normal은 절두체 외부 방향임 (내부 방향 아님)

    - 외부 : AABB가 절두체 외부에 있다.
    - 교차 : AABB가 절두체 내부에 있거나 교차한다.
*/
inline WORD Outside_Frustum_AABB( const WzFrustum* pwf, const BvAABB* pbaabb )
{
    WzAssert( pwf );
    WzAssert( pbaabb );

    // AABB의 min, max 점을 일단 저장
    const WzVector* pwvSrcMin = &pbaabb->m_wvMin;
    const WzVector* pwvSrcMax = &pbaabb->m_wvMax;

    // 절두체 전체 평면을 돌면서
    for( int iPlane = 0; iPlane < 6; ++iPlane )
    {
        // 현재 평면을 얻고
        const WzPlane* pwp = &pwf->m_wpPlanes[iPlane];
        WzAssert( pwp );

        // 검사할 점 선정
        static WzVector wvTest;

        // 평면 normal의 각 좌표가 0보다 작은 경우, AABB의 max 좌표 선택
        wvTest.x = ( pwp->m_wvNormal.x < 0.0f) ? pwvSrcMax->x : pwvSrcMin->x;
        wvTest.y = ( pwp->m_wvNormal.y < 0.0f) ? pwvSrcMax->y : pwvSrcMin->y;
        wvTest.z = ( pwp->m_wvNormal.z < 0.0f) ? pwvSrcMax->z : pwvSrcMin->z;

        // 면과의 관계 검사 (0보다 크다는 것은 평면 앞(normal방향)에 있다는 것)
        if( Distance_Plane_Vertex( pwp, wvTest ) > 0.0f )
        {
            return NTCT_OUTSIDE;
        }
    }

    return NTCT_INTERSECT;
}

//------------------------------------------------------------------------------
/**
    절두체 - 구 외부 검사

    - 절두체 평면들의 normal은 절두체 외부 방향임 (내부 방향 아님)

    - 외부 : 구가 절두체 외부에 있다.
    - 교차 : 구가 절두체 내부에 있거나 교차한다.
*/
inline WORD Outside_Frustum_Sphere( const WzFrustum* pwf, const BvSphere* pbs )
{
    WzAssert( pwf );
    WzAssert( pbs );

    // 절두체 전체 평면을 돌면서
    for( int iPlane = 0; iPlane < 6; ++iPlane )
    {
        // 현재 평면을 얻고
        const WzPlane* pwp = &pwf->m_wpPlanes[iPlane];
        WzAssert( pwp );

        // 구의 중심과 평면과의 거리(부호 있음) 계산
        float fDist = Distance_Plane_Vertex( pwp, pbs->m_wvCenter );

        // 거리가 반지름 보다 크면 평면 앞(normal방향)에 있다는 것
        if( fDist > pbs->m_fRadius )
        {
            return NTCT_OUTSIDE; 
        }
    }

    return NTCT_INTERSECT;
}

//------------------------------------------------------------------------------
/**
    절두체 - 바운딩 볼륨 외부 검사

    - 절두체 평면들의 normal은 절두체 외부 방향임 (내부 방향 아님)

    - 외부 : 바운딩 볼륨이 절두체 외부에 있다.
    - 교차 : 바운딩 볼륨이 절두체 내부에 있거나 교차한다.
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

    // 절두체 전체 평면을 돌면서
    for( int iPlane = 0; iPlane < 6; ++iPlane )
    {
        // 현재 평면을 얻고
        const WzPlane* pwp = &pwf->m_wpPlanes[iPlane];
        WzAssert( pwp );

        // 평면과 점과의 거리가 0 보다 크면 평면 앞(normal방향)에 있다는 것
        if( Distance_Plane_Vertex( pwp, wvVertex ) > 0.0f )
        {
            return NTCT_OUTSIDE; 
        }
    }

    return NTCT_INTERSECT;
}

//------------------------------------------------------------------------------
/**
    AABB 확장

    - 주어진 점을 포함하도록 AABB 확장
*/
inline void Extend_BvAABB( BvAABB* pbaabb, const WzVector* pwvVertex )
{
    WzAssert( pbaabb );
    WzAssert( pwvVertex );

    // x, y, z를 돌면서
    for( int i = 0; i < 3; ++i )
    {
        // 주어진 점의 좌표가 AABB의 min값 보다 작은 경우
        if( pwvVertex->m_Element[i] < pbaabb->m_wvMin.m_Element[i] )
        {
            // AABB의 min값 수정
            pbaabb->m_wvMin.m_Element[i] = pwvVertex->m_Element[i];
        }

        // 주어진 점의 좌표가 AABB의 max값 보다 큰 경우
        if( pwvVertex->m_Element[i] > pbaabb->m_wvMax.m_Element[i] )
        {
            // AABB의 max값 수정
            pbaabb->m_wvMax.m_Element[i] = pwvVertex->m_Element[i];
        }
    }
}

#endif // _COMMON_BOUNDINGVOLUME_H_
