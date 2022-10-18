#pragma once
#ifndef WORLDSERVER_SECTOR_GRID_H
#define WORLDSERVER_SECTOR_GRID_H

#pragma warning(push)
#pragma warning(disable : 4100)
#include <WzDataType.h>
#include <client/3DEngine/Common/BoundingVolume.h>
#pragma warning(pop)

namespace ns_world { namespace grid {
;

typedef DWORD      SECTOR_IDX;
typedef SECTOR_IDX SECTOR_VRIDX;   // SectorGridVR에서 사용하는 인덱스
#define INVALID_SECTOR_INDEX static_cast<ns_world::grid::SECTOR_IDX>(ULONG_MAX)
extern const WzVector kNullVector;

//==================================================================================================
//  <SectorGrid3x3>
struct SectorGrid3x3
{
    enum ePOS
    {
        LEFT_UP     = 0,        UP          ,           RIGHT_UP    ,
        LEFT        ,           CENTER      ,           RIGHT       ,
        LEFT_DOWN   ,           DOWN        ,           RIGHT_DOWN  ,
        MAX_SIZE    ,
    };
    typedef util::ARRAY_TYPEINFO<SECTOR_IDX, 0, MAX_SIZE> GridArray;
    // SectorGridVR에서는 SECTOR_IDX를 SECTOR_VRIDX로 처리한다.

    GridArray::array_type Grid;
};

//==================================================================================================
//
class SectorGridInfo;
class SectorGrid;
class SectorGridVR;

//==================================================================================================
//  <SectorGridShared>
//
class SectorGridShared
{
public:
    enum eCoord   { SG_X_MIN, SG_X_MAX, SG_Y_MIN, SG_Y_MAX, SG_Z_MIN, SG_Z_MAX, SG_XYZ_MAX };
    enum eXYZ_LEN { LEN_X, LEN_Y, LEN_Z, LEN_XYZ_MAX };
    //
    SectorGridShared();

    const DWORD* GetBoundaries() const { return m_Boundaries; }
    const DWORD* GetLengths() const { return m_Length; }

    void Build(const SectorGridShared& rSrc);
    void Build(const BvAABB& rSrc, const DWORD sectorDist /*meter*/);

    bool IsValidSector(const SECTOR_IDX coordinates) const;
    bool IsValidCoordinate(const WzVector* IN point) const;
    static bool IsNullVector(const WzVector* IN point);
    //
    SECTOR_IDX GetSectorIndex(const WzVector* IN point) const;
    bool GetCenterPosInSector(const SECTOR_IDX coordinates, WzVector& OUT result_center) const;

    bool GetAroundSectors(const SECTOR_IDX coordinates,
                          SectorGrid3x3* const OUT result_grid) const;
    bool ChangeSector(const SECTOR_IDX prevSectorXY, const SECTOR_IDX newSectorXY) const;

protected:
    bool _IsValidCoordinate(const DWORD x, const DWORD y, const DWORD z) const;
    // fields
public:
    const DWORD& SectorCount;
    const DWORD& SectorXYDistance;
protected:
    DWORD  m_SectorXYdist; // (x==y), 각 변 길이 (1.0f == 1meter == 1)
    DWORD  m_SectorCount;
    DWORD  m_Boundaries[SG_XYZ_MAX];
    DWORD  m_Length[LEN_XYZ_MAX];
    //
    friend class SectorGridInfo;
    friend class SectorGrid;
    friend class SectorGridVR;
    __DISABLE_COPY(SectorGridShared);
};
//  end SectorGridShared
//==================================================================================================
//  <DEFINITIONs>
//  <SectorGridBase>
template<typename _SUBCLASS>
class SectorGridBase : public SectorGridShared
{
public:
    ~SectorGridBase() { Destroy();  }

    void Destroy();
    void Build(const SectorGridShared& rSrc);
    void Build(const BvAABB& rSrc, const DWORD sectorDist /*meter*/);
    void ReBuild(const SectorGridShared& rSrc, const DWORD sectorDist);
};
//  end SectorGridBase
//==================================================================================================
//  <SectorGridInfo>
class SectorGridInfo : public SectorGridBase<SectorGridInfo>
{
    typedef SectorGridBase<SectorGridInfo> base_type;
public:
    SectorGridInfo() {}

private:
    void Init() {}  //  SectorGridBase::Build에 의해 자동 호출된다.
    void Release() {}
    //
    friend class SectorGridBase<SectorGridInfo>;
    __DISABLE_COPY(SectorGridInfo);
};

//==================================================================================================


//==================================================================================================
//  <SectorGrid>
class SectorGrid : public SectorGridBase<SectorGrid>
{
    typedef SectorGridBase<SectorGridInfo> base_type;
public:
    struct Node : public ::util::__LList<Node>
    {
        ns_world::WorldUser* user; // if the object is root node, the user pointer is NULL.
    };
    //
    SectorGrid() : sector_array_(0) {}
    //
    Node* GetSector(const SECTOR_IDX coordinates) const;
    bool Link(Node* const user_node, const WzVector& user_position);
private:
    void Init();     //  SectorGridBase::Build에 의해 자동 호출된다.
    void Release();
    //
    Node* sector_array_;

    friend class SectorGridBase<SectorGrid>;
    friend class SectorGridVR;
};

}}; //end of namespace
//==================================================================================================
//==================================================================================================
//==================================================================================================

//  Around계열에서 Center를 먼저 처리할 경우만 사용할 것.
#define GRID3x3_CENTER_PRIOR_SWAP(_grid_) \
    ;{ \
        const SECTOR_IDX __center_index__       = (_grid_).Grid[ SectorGrid3x3::CENTER ];  \
        (_grid_).Grid[ SectorGrid3x3::CENTER ]  = (_grid_).Grid[ SectorGrid3x3::LEFT_UP ]; \
        (_grid_).Grid[ SectorGrid3x3::LEFT_UP ] = __center_index__; \
    };

inline ns_world::grid::SectorGridShared::SectorGridShared()
    : SectorCount(m_SectorCount)
    , SectorXYDistance(m_SectorXYdist)
    , m_SectorXYdist(0)
    , m_SectorCount(0)
{
    ZeroMemory(m_Boundaries, sizeof(m_Boundaries));
    ZeroMemory(m_Length, sizeof(m_Length));
}

inline bool ns_world::grid::SectorGridShared::_IsValidCoordinate(
    const DWORD x, const DWORD y, const DWORD z) const
{
    __UNUSED(z);
    return (0 <= x && x < m_Boundaries[SG_X_MAX]) &&
           (0 <= y && y < m_Boundaries[SG_Y_MAX]);
}

inline ns_world::grid::SECTOR_IDX
ns_world::grid::SectorGridShared::GetSectorIndex(const WzVector* IN point) const
{
    ulong cur_pos_x = static_cast<ulong>(point->x) - m_Boundaries[SG_X_MIN];
    ulong cur_pos_y = static_cast<ulong>(point->y) - m_Boundaries[SG_Y_MIN];

    if (_IsValidCoordinate(cur_pos_x, cur_pos_y, 0))
    {
        ulong index_x = static_cast<ulong>(cur_pos_x / m_SectorXYdist);
        ulong index_y = static_cast<ulong>(cur_pos_y / m_SectorXYdist);
        return index_x + index_y * m_Length[LEN_X];
    }
    return INVALID_SECTOR_INDEX;
}

template<typename _SUBCLASS>
void ns_world::grid::SectorGridBase<_SUBCLASS>::Destroy()
{
    static_cast<_SUBCLASS*>(this)->Release();
    m_SectorXYdist  = 0;
    m_SectorCount   = 0;
    ZeroMemory(m_Boundaries, sizeof(m_Boundaries));
    ZeroMemory(m_Length, sizeof(m_Length));
}

template<typename _SUBCLASS>
inline void ns_world::grid::SectorGridBase<_SUBCLASS>::Build(const SectorGridShared& rSrc)
{
    SectorGridShared::Build(rSrc);
    static_cast<_SUBCLASS*>(this)->Init();
}

template<typename _SUBCLASS>
inline void ns_world::grid::SectorGridBase<_SUBCLASS>::Build(
    const BvAABB& rSrc, const DWORD sectorDist)
{
    SectorGridShared::Build(rSrc, sectorDist);
    static_cast<_SUBCLASS*>(this)->Init();
}

template<typename _SUBCLASS>
inline void ns_world::grid::SectorGridBase<_SUBCLASS>::ReBuild(
    const SectorGridShared& rSrc, const DWORD sectorDist)
{
    const DWORD* const pBoundaries = rSrc.GetBoundaries();
    BvAABB aabb;
    aabb.m_wvMin.x = (float)(int)pBoundaries[SG_X_MIN];
    aabb.m_wvMin.y = (float)(int)pBoundaries[SG_Y_MIN];
    aabb.m_wvMin.z = (float)(int)pBoundaries[SG_Z_MIN];
    aabb.m_wvMax.x = aabb.m_wvMin.x + (float)(int)pBoundaries[SG_X_MAX];
    aabb.m_wvMax.y = aabb.m_wvMin.y + (float)(int)pBoundaries[SG_Y_MAX];
    aabb.m_wvMax.z = aabb.m_wvMin.z + (float)(int)pBoundaries[SG_Z_MAX];
    Build(aabb, sectorDist);
}

//==================================================================================================
//  <SectorGrid>
inline ns_world::grid::SectorGrid::Node*
ns_world::grid::SectorGrid::GetSector(const SECTOR_IDX coordinates) const
{
    if (sector_array_ && coordinates < m_SectorCount) {
        return &sector_array_[coordinates];
    }
    return NULL;
}

#endif //WORLDSERVER_SECTOR_GRID_H