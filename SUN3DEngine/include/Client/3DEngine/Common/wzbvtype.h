// wzbvtype.h

#ifndef _COMMON_WZBVTYPE_H_
#define _COMMON_WZBVTYPE_H_

#include "wzplane.h"

// �ٿ�� ���� Ÿ��
// �������� �������� ���� ���� ���ؼ� �ȵȴ�.
enum ENUM_BVTYPE
{
    BVT_AABB = 0,
    BVT_SPHERE = 1,
    BVT_CYLINDER = 2,       // ��� �� ��
    BVT_PATH_TILE_INDEX = 3,
    BVT_TILE_ATT = 4,
};

// ����ü ���
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

// ����
struct WzRay
{
    WzVector m_wvOrigin;    // ������
    WzVector m_wvDirection; // ���� ( unit vector �̾�� �Ѵ�.)
};

// ����ü
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

// ��
struct BvSphere
{
    WzVector m_wvCenter;    // �߽�
    float m_fRadius;        // ������
};

// �н�Ÿ�� �ε���
struct BvPathTileIndex
{
    DWORD m_nCount;         // �ε��� ����
    DWORD* m_pdwData;       // �ε��� ����Ÿ
};

// �ٿ�� ����
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