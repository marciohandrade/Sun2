// wzbvtype.h

#ifndef _COMMON_WZBVTYPE_H_
#define _COMMON_WZBVTYPE_H_

#include "wzplane.h"

// 바운딩 볼륨 타입
// 맵파일이 변함으로 절대 값이 변해선 안된다.
enum ENUM_BVTYPE
{
    BVT_AABB = 0,
    BVT_SPHERE = 1,
    BVT_CYLINDER = 2,       // 사용 안 함
    BVT_PATH_TILE_INDEX = 3,
    BVT_TILE_ATT = 4,
};

// 절두체 평면
enum ENUM_FP
{
    FP_LEFT = 0,
    FP_RIGHT,
    FP_BOTTOM,
    FP_TOP,
    FP_NEAR,
    FP_FAR,
    NUM_FP
};

// 광선
struct WzRay
{
    WzVector m_wvOrigin;    // 시작점
    WzVector m_wvDirection; // 방향 ( unit vector 이어야 한다.)
};

// 절두체
struct WzFrustum
{
    // left, right, bottom, top, near, far
    WzPlane m_wpPlanes[NUM_FP];
};

// AABB
struct BvAABB
{
    union
    {
        struct
        {
            WzVector m_wvMin;
            WzVector m_wvMax;
        };

        WzVector m_wvElement[2];
    };
};

// 구
struct BvSphere
{
    WzVector m_wvCenter;    // 중심
    float m_fRadius;        // 반지름
};

// 패스타일 인덱스
struct BvPathTileIndex
{
    DWORD m_nCount;         // 인덱스 개수
    DWORD* m_pdwData;       // 인덱스 데이타
};

// 바운딩 볼륨
struct WzBoundingVolume
{
    ENUM_BVTYPE m_eBvType;

    union
    {
        BvAABB m_aabb;
        BvSphere m_BvSphere;
        BvPathTileIndex m_BvPathTileIndex;
    };
};

#endif // _COMMON_WZBVTYPE_H_