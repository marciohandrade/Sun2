#pragma once
#ifndef GAMESERVER_SECTOR_GRID_H
#define GAMESERVER_SECTOR_GRID_H

#include <WzDataType.h>
#include <3DEngine/Common/BoundingVolume.h>
#include <LinkedListCS.h>

class Sector;
class GameField;

//==================================================================================================
// namespace
namespace nsGrid {
;

typedef DWORD                   SECTOR_IDX;
typedef SECTOR_IDX              SECTOR_VRIDX;   // SectorGridVR���� ����ϴ� �ε���
#define INVALID_SECTOR_INDEX    static_cast<SECTOR_IDX>(ULONG_MAX)

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
    // SectorGridVR������ SECTOR_IDX�� SECTOR_VRIDX�� ó���Ѵ�.

    GridArray::array_type Grid;
};

//==================================================================================================
//  <SectorGrid3x3_ChangeSector>
struct SectorGrid3x3_ChangeSector
{
    SectorGrid3x3 OUT CurrentSectors;
    SectorGrid3x3 OUT EnterSectors;
    SectorGrid3x3 OUT LeaveSectors;
};

//==================================================================================================
//  <ViewBoxCubic>
//  -   Bounding Volume for Variable Broadcast
struct ViewBoxCubic
{
    BvAABB Cubic;
    void SetInit2D(const WzVector* const center_pos, const WzVector* const radius_2d);
    void SetInit3D(const WzVector* const center_pos, const WzVector* const radius_3d);
    void SetInit3D_UpsideZ(const WzVector* const center_pos, const WzVector* const radius_3d);
    void SetInit3D_DownsideZ(const WzVector* const center_pos, const WzVector* const radius_3d);
    void SetInit3D_Radian(const WzVector* const center_pos, const WzVector* const radius_3d,
                          const WzVector* const direction_and_weight);
    void SetInit3D_Radian_UpsideZ(const WzVector* const center_pos,
                                  const WzVector* const radius_3d,
                                  const WzVector* const direction_and_weight);
    bool IsValidCoordinateXY(const WzVector* const point) const;
#ifdef _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE
    bool IsValidCoordinateXY(const WzVector* const point, const WzVector* const center_point) const;
#endif  // _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE
    bool IsValidCoordinateXYZ(const WzVector* const point) const;

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
    //
    SECTOR_IDX GetSectorIndex(const WzVector* IN point) const;
    bool GetCenterPosInSector(const SECTOR_IDX coordinates, WzVector& OUT result_center) const;

    bool GetAroundSectors(const SECTOR_IDX coordinates,
                          SectorGrid3x3* const OUT result_grid) const;
    bool ChangeSector(const SECTOR_IDX prevSectorXY, const SECTOR_IDX newSectorXY,
                      SectorGrid3x3_ChangeSector& OUT info) const;

protected:
    bool _IsValidCoordinate(const DWORD x, const DWORD y, const DWORD z) const;
    // fields
public:
    const DWORD& SectorCount;
    const DWORD& SectorXYDistance;
protected:
    DWORD  m_SectorXYdist; // (x==y), �� �� ���� (1.0f == 1meter == 1)
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
    void Init() {}  //  SectorGridBase::Build�� ���� �ڵ� ȣ��ȴ�.
    void Release() {}
    //
    friend class SectorGridBase<SectorGridInfo>;
    friend class ::Sector;
    __DISABLE_COPY(SectorGridInfo);
};
//
//==================================================================================================
// SYNC_MSG_LINKEDLIST
//  NOTE: f110112.5L, fix to byte alignment to enhance system architecture address calculator.
struct SYNC_MSG_LINKEDLIST : public util::__LList<SYNC_MSG_LINKEDLIST>
{
    typedef SYNC_MSG_LINKEDLIST this_type;

    // internal method
    this_type* SerializeTo(WAR_MSG_HDR* const IN header,
        const WAR_TAG_V* war_msg_in_buffer, uint16_t war_msg_in_buffer_size) const;
    //
    const WAR_TAG_V* war_msg;
    uint16_t war_msg_size;
};
//
//==================================================================================================
//  <SectorGrid>
class SectorGrid : public SectorGridBase<SectorGrid>
{
    typedef SectorGridBase<SectorGridInfo> base_type;
    struct Node
    {
        Sector*             pSector;
        SYNC_MSG_LINKEDLIST m_SectorRoot;
    };
public:
    SectorGrid() : m_pSectorNodes(0) {}
    //
    void Update(GameField* const game_field) const;
    Sector* GetSector(const SECTOR_IDX coordinates) const;
    Sector* AddSectorMsgNode(WAR_MSG_HDR* const header, const SECTOR_IDX coordinates,
                             const WAR_TAG_V* war_msg_in_buffer, uint16_t war_msg_size) const;
private:
    void Init();     //  SectorGridBase::Build�� ���� �ڵ� ȣ��ȴ�.
    void Release();
    //
    Node* m_pSectorNodes;
    //SYNC_MSG_LINKEDLIST m_FieldRoot;

    friend class SectorGridBase<SectorGrid>;
    friend class ::Sector;
    friend class SectorGridVR;
};
//
//==================================================================================================
//  <SectorGridVR>
//  -   SectorGrid for Variable-Resolution
//  -   SectorGridVR�� 1���� SectorVRIndex�� ó���Ѵ�. SectorGrid�� 9�� �����ϴ���
//      �ٸ� �並 ������ Ư�� Sector�鸸 ó���� �� �ְ� �ȴ�. (��� ���� ����)
//  -   (WARNING)
//  -       �� ���� ����� �ÿ��� m_pProjectionGrid�� not NULL�̶�� ���� ���� ������ �����Ѵ�.
//  -       �ݵ�� Rebuild�� ȣ���Ѵ�. 
class SectorGridVR : public SectorGridBase<SectorGridVR>
{
    typedef SectorGridBase<SectorGridVR> base_type;
public:
    // CHANGES: f110415.3L, prevent constructor usage to solve sun:r53320 case
    struct ProjectionGridWrapper {
        ProjectionGridWrapper(const SectorGridShared* projection_grid)
            : projection_grid_(projection_grid) {}
        const SectorGridShared* projection_grid_;
    };
    SectorGridVR() : m_pProjectionGrid(NULL) {} // don't use it
    SectorGridVR(const ProjectionGridWrapper& projection)
        : m_pProjectionGrid(projection.projection_grid_) {}
    //
    void Rebuild(const DWORD sectorDist);  // Resolution ����
    bool GetAroundSectors(const WzVector* IN point, //  ������ �Ǵ� ��ǥ
                          SectorGrid3x3* const OUT result_grid, //  (��ǥ ����) �� VRGrid�� �´� ProjectionGrid ���� �ε��� ��� ��ȯ
                          ViewBoxCubic* const OUT pOutBoundary) const; //  (��ǥ ����) �� VRGrid�� ������ ViewBox
private:
    void Init() {} //  SectorGridBase::Build�� ���� �ڵ� ȣ��ȴ�.
    void Release() {}

    struct Boundaries { int minX, minY, maxX, maxY; }; // (WAVERIX) (090403) type change
    bool CheckAllowMinMaxBoundary(const SECTOR_IDX coordinates,
                                  const SectorGridShared& rSrc,
                                  const Boundaries* const IN pBoundary) const;
    void Get3x3MinMaxBoundary(const SECTOR_IDX coordinates,
                              const SectorGridVR& rSrc, Boundaries* const OUT pOut) const;
    //
    const SectorGridShared* m_pProjectionGrid;
    //
    friend class SectorGridBase<SectorGridVR>;
    friend class ::Sector;
};
//
//==================================================================================================
//  <SectorGridProxy>
//  -   SectorGrid based user-specific control
//  -   SectorGrid ������� ������ �ڷᱸ���� ������ �ʿ䰡 ���� �� ���.
template<typename _SUBTYPE>
class SectorGridProxy : public SectorGridBase<_SUBTYPE>
{
    typedef SectorGridBase<_SUBTYPE> base_type;
public:
    SectorGridProxy() : m_pProjectionGrid(0) {}

    void Build(const DWORD sectorDist);
    //  ���� �Լ����� _SUBTYPE���� ������ Init, Release�� ó���� ��.
    //void Init();    //  SectorGridBase::Build�� ���� �ڵ� ȣ��ȴ�.
    //void Release(); //  SectorGridBase::Destroy�� ���� �ڵ� ȣ��ȴ�.
protected:
    SectorGrid* m_pProjectionGrid;
private:
    //
    friend class SectorGridBase<_SUBTYPE>;
    friend class ::Sector;
    __DISABLE_COPY(SectorGridProxy<_SUBTYPE>);
};

}; //end of namespace 'nsGrid'
//==================================================================================================
//==================================================================================================
//==================================================================================================

//  Around�迭���� Center�� ���� ó���� ��츸 ����� ��.
#define GRID3x3_CENTER_PRIOR_SWAP(_grid_) \
    ;{ \
        const SECTOR_IDX __center_index__       = (_grid_).Grid[ SectorGrid3x3::CENTER ];  \
        (_grid_).Grid[ SectorGrid3x3::CENTER ]  = (_grid_).Grid[ SectorGrid3x3::LEFT_UP ]; \
        (_grid_).Grid[ SectorGrid3x3::LEFT_UP ] = __center_index__; \
    };

//==================================================================================================
//  <ViewBoxCubic>
//  -   Bounding Volume for Variable Broadcast
inline void nsGrid::ViewBoxCubic::SetInit2D(
    const WzVector* const center_pos, const WzVector* const radius_2d)
{
    //  ASSERT(pRadius.x > .0f && pRadius.y > .0f && pRadius.z == .0f);
    Cubic.m_wvMin.x = center_pos->x - radius_2d->x;
    Cubic.m_wvMin.y = center_pos->y - radius_2d->y;
    Cubic.m_wvMin.z = -5.0f;
    Cubic.m_wvMax.x = center_pos->x + radius_2d->x;
    Cubic.m_wvMax.y = center_pos->y + radius_2d->y;
    Cubic.m_wvMax.z = +5.0f;
}

inline void nsGrid::ViewBoxCubic::SetInit3D(
    const WzVector* const center_pos, const WzVector* const radius_3d)
{
    //  ASSERT(pRadius.x > .0f && pRadius.y > .0f && pRadius.z > .0f);
    Cubic.m_wvMin.x = center_pos->x - radius_3d->x;
    Cubic.m_wvMin.y = center_pos->y - radius_3d->y;
    Cubic.m_wvMin.z = center_pos->z - radius_3d->z - 5.0f;
    Cubic.m_wvMax.x = center_pos->x + radius_3d->x;
    Cubic.m_wvMax.y = center_pos->y + radius_3d->y;
    Cubic.m_wvMax.z = center_pos->z + radius_3d->z + 5.0f;
}

inline void nsGrid::ViewBoxCubic::SetInit3D_UpsideZ(
    const WzVector* const center_pos, const WzVector* const radius_3d)
{   //  �� ������ �ִ� '��ž'���� ������ �����ϰ� ����� ������ ���̴�.
    //  ASSERT(pRadius.x > .0f && pRadius.y > .0f && pRadius.z > .0f);
    Cubic.m_wvMin.x = center_pos->x - radius_3d->x;
    Cubic.m_wvMin.y = center_pos->y - radius_3d->y;
    Cubic.m_wvMin.z = center_pos->z - 5.f; // ����� ��, ��ž ���� ������ �̹��� ������ �߱��� �� �����Ƿ�...
    Cubic.m_wvMax.x = center_pos->x + radius_3d->x;
    Cubic.m_wvMax.y = center_pos->y + radius_3d->y;
    Cubic.m_wvMax.z = center_pos->z + radius_3d->z + 5.0f;
}

inline void nsGrid::ViewBoxCubic::SetInit3D_DownsideZ(
    const WzVector* const center_pos, const WzVector* const radius_3d)
{   //  �� ������ �ִ� '��ž'���� ������ �����ϰ� ����� ������ ���̴�. (��ܿ����� �� ��� ��� ���� ���� ����)
    //  ASSERT(pRadius.x > .0f && pRadius.y > .0f && pRadius.z > .0f);
    Cubic.m_wvMin.x = center_pos->x - radius_3d->x;
    Cubic.m_wvMin.y = center_pos->y - radius_3d->y;
    Cubic.m_wvMin.z = center_pos->z - radius_3d->z - 5.f;  // ����� ��, ��ž ���� ������ �̹��� ������ �߱��� �� �����Ƿ�...
    Cubic.m_wvMax.x = center_pos->x + radius_3d->x;
    Cubic.m_wvMax.y = center_pos->y + radius_3d->y;
    Cubic.m_wvMax.z = center_pos->z + 5.0f;
}

//  �־��� �������� ����ġ ���� (�־��� �������� �� ���� �Ÿ��� �Ҵ��Ѵ�.)
//  XXXXX       XXXXX
//  XXXXX       XXXXX
//  XXOXX   =>  XXXXX
//  XXXXX       XOXXX
//  XXXXX       XXXXX
inline void nsGrid::ViewBoxCubic::SetInit3D_Radian(
    const WzVector* const center_pos, const WzVector* const radius_3d,
    const WzVector* const direction_and_weight)
{
    SetInit3D(center_pos, radius_3d);
    WzVector clone  = *direction_and_weight;
    Cubic.m_wvMax   += clone;
}

inline void nsGrid::ViewBoxCubic::SetInit3D_Radian_UpsideZ(
    const WzVector* const center_pos, const WzVector* const radius_3d,
    const WzVector* const direction_and_weight)
{
    SetInit3D_UpsideZ(center_pos, radius_3d);
    WzVector clone  = *direction_and_weight;
    clone.z         = 0.f;  // ���̷� ����ġ �ִ� ���� ����ùķ��̼ǿ����� �ϸ� �ȴ�.
    Cubic.m_wvMax   += clone;
}

inline bool nsGrid::ViewBoxCubic::IsValidCoordinateXY(const WzVector* const point) const
{
    const float fx = point->x;
    const float fy = point->y;

    if (fx < Cubic.m_wvMin.x || Cubic.m_wvMax.x < fx ||
        fy < Cubic.m_wvMin.y || Cubic.m_wvMax.y < fy)
    {
        return false;
    }

    return true;
}
#ifdef _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE
inline bool nsGrid::ViewBoxCubic::IsValidCoordinateXY(const WzVector* const point, 
                                                      const WzVector* const center_point) const
{
    // �簢���� �����ϴ� ������ �浹üũ �ϴ� �Լ�( �ڽ��� ��ġ, �簢���� ����)

    // ĳ������ ��ġ..
    const float fx = point->x;
    const float fy = point->y;
   
    // �� ���� �Ÿ� ���ϴ� ����..
    WzVector vector_distance;
    vector_distance.x = fx - center_point->x;
    vector_distance.y = fy - center_point->y;
    vector_distance.z = 0.0f;

    const float distance = VectorLength2(&vector_distance);

    // �������� ���ϴ� ����..
    const float radius = (center_point->x - Cubic.m_wvMin.x);

    //�浹 üũ..
    if (distance > (radius*radius))
    {
        return false;
    }

    return true;
}
#endif //_NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE

inline bool nsGrid::ViewBoxCubic::IsValidCoordinateXYZ(const WzVector* const point) const
{   // IsValidCoordinateXY ���ٴ� �̰��� ����ϴ� ���� �� ���� ��. ������ ���� ó�� �ʿ� �����Ƿ�...
    const float fx = point->x;
    const float fy = point->y;
    const float fz = point->z;

    if (fx < Cubic.m_wvMin.x || Cubic.m_wvMax.x < fx ||
        fy < Cubic.m_wvMin.y || Cubic.m_wvMax.y < fy ||
        fz < Cubic.m_wvMin.z || Cubic.m_wvMax.z < fz)
    {
        return false;
    }

    return true;
}

//==================================================================================================
//  <SectorGridShared>
//
inline nsGrid::SectorGridShared::SectorGridShared()
    : SectorCount(m_SectorCount)
    , SectorXYDistance(m_SectorXYdist)
    , m_SectorXYdist(0)
    , m_SectorCount(0)
{
    ZeroMemory(m_Boundaries, sizeof(m_Boundaries));
    ZeroMemory(m_Length, sizeof(m_Length));
}


inline bool nsGrid::SectorGridShared::IsValidSector(const SECTOR_IDX coordinates) const
{
    if ((coordinates < m_SectorCount) == false) {
        return false;
    }

    const DWORD indexX = coordinates % m_Length[LEN_X];
    const DWORD indexY = coordinates / m_Length[LEN_X];

    if (indexX >= m_Length[LEN_X] ||
        indexY >= m_Length[LEN_Y])
    {
        return false;
    }
    return true;
}

inline bool nsGrid::SectorGridShared::IsValidCoordinate(const WzVector* IN point) const
{
    const DWORD x = (DWORD)point->x - m_Boundaries[SG_X_MIN];
    const DWORD y = (DWORD)point->y - m_Boundaries[SG_Y_MIN];
    return (0 <= x && x < m_Boundaries[SG_X_MAX]) &&
           (0 <= y && y < m_Boundaries[SG_Y_MAX]);
}

inline bool nsGrid::SectorGridShared::_IsValidCoordinate(
    const DWORD x, const DWORD y, const DWORD z) const
{
    __UNUSED(z);
    return (0 <= x && x < m_Boundaries[SG_X_MAX]) &&
           (0 <= y && y < m_Boundaries[SG_Y_MAX]);
}

inline nsGrid::SECTOR_IDX nsGrid::SectorGridShared::GetSectorIndex(const WzVector* IN point) const
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

inline bool nsGrid::SectorGridShared::GetCenterPosInSector(
    const SECTOR_IDX coordinates, WzVector& OUT result_center) const
{
    if ((coordinates < m_SectorCount) == false) {
        return false;
    }
    const DWORD indexX = coordinates%m_Length[LEN_X];
    const DWORD indexY = coordinates/m_Length[LEN_X];
    result_center.x = (float)((indexX*m_SectorXYdist) + m_Boundaries[SG_X_MIN]);
    result_center.y = (float)((indexY*m_SectorXYdist) + m_Boundaries[SG_Y_MIN]);
    result_center.z = 0.f;
    return true;
}


//==================================================================================================
//  SectorGridBase<_SUBCLASS>
//
template<typename _SUBCLASS>
void nsGrid::SectorGridBase<_SUBCLASS>::Destroy()
{
    static_cast<_SUBCLASS*>(this)->Release();
    m_SectorXYdist  = 0;
    m_SectorCount   = 0;
    ZeroMemory(m_Boundaries, sizeof(m_Boundaries));
    ZeroMemory(m_Length, sizeof(m_Length));
}

template<typename _SUBCLASS>
inline void nsGrid::SectorGridBase<_SUBCLASS>::Build(const SectorGridShared& rSrc)
{
    SectorGridShared::Build(rSrc);
    static_cast<_SUBCLASS*>(this)->Init();
}

template<typename _SUBCLASS>
inline void nsGrid::SectorGridBase<_SUBCLASS>::Build(const BvAABB& rSrc, const DWORD sectorDist)
{
    SectorGridShared::Build(rSrc, sectorDist);
    static_cast<_SUBCLASS*>(this)->Init();
}

template<typename _SUBCLASS>
inline void nsGrid::SectorGridBase<_SUBCLASS>::ReBuild(
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
inline Sector* nsGrid::SectorGrid::GetSector(const SECTOR_IDX coordinates) const
{
    if (m_pSectorNodes && coordinates < m_SectorCount) {
        return m_pSectorNodes[coordinates].pSector;
    }
    return NULL;
}

//==================================================================================================
//  <SectorGridVR>
inline void nsGrid::SectorGridVR::Rebuild(const DWORD sectorDist)
{
    base_type::ReBuild(*m_pProjectionGrid, sectorDist);
}

template<typename _SUBTYPE>
inline void nsGrid::SectorGridProxy<_SUBTYPE>::Build(const DWORD sectorDist)
{
    if (m_pProjectionGrid) {
        base_type::ReBuild(*m_pProjectionGrid, sectorDist);
    }
}

#endif //GAMESERVER_SECTOR_GRID_H