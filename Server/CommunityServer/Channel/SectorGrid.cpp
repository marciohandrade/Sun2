#include "WorldServerPch.hxx"
#include "./SectorGrid.h"

#include "Objects/WorldUser.h"

//==================================================================================================

const WzVector ns_world::grid::kNullVector = { 0.0f, 0.0f, 0.0f };

//==================================================================================================

bool ns_world::grid::SectorGridShared::IsNullVector(const WzVector* IN point)
{
    if (::memcmp(point, &ns_world::grid::kNullVector,
            sizeof(ns_world::grid::kNullVector)) == 0)
    {
        return true;
    }
    return false;
};

bool ns_world::grid::SectorGridShared::GetAroundSectors(
    const SECTOR_IDX coordinates,
    SectorGrid3x3* const OUT pOutGrid) const
{
    memset(pOutGrid->Grid, INVALID_SECTOR_INDEX, sizeof(pOutGrid->Grid));
    if ((coordinates == INVALID_SECTOR_INDEX) || !(coordinates < m_SectorCount)) {
        return false;
    };
    const DWORD length_x = m_Length[LEN_X];		// for load to GPR
    const DWORD indexX__ = coordinates % length_x;
    const DWORD __indexY = coordinates / length_x;

    const bool lowerboundX = !indexX__;
    const bool lowerboundY = !__indexY;
    const bool upperboundX = (indexX__ + 1) == m_Length[LEN_X];
    const bool upperboundY = (__indexY + 1) == m_Length[LEN_Y];

    //const DWORD indexX__
    const DWORD indexXm1 = indexX__ - 1;
    const DWORD indexXp1 = indexX__ + 1;

    const DWORD indexY__ = __indexY * length_x;
    const DWORD indexYm1 = indexY__ - length_x;
    const DWORD indexYp1 = indexY__ + length_x;

    SECTOR_IDX* pOffset = (pOutGrid->Grid);

#define GRID_ARRAY_CODE(index, cond, x, y)  \
    if (!(cond)) { *pOffset = (x) + (y); }; \
    ++pOffset

    GRID_ARRAY_CODE(0+0, (lowerboundX || lowerboundY), indexXm1, indexYm1);
    GRID_ARRAY_CODE(0+1, (               lowerboundY), indexX__, indexYm1);
    GRID_ARRAY_CODE(0+2, (upperboundX || lowerboundY), indexXp1, indexYm1);
    GRID_ARRAY_CODE(3+0, (lowerboundX               ), indexXm1, indexY__);
    GRID_ARRAY_CODE(3+1, (0                         ), indexX__, indexY__);
    GRID_ARRAY_CODE(3+2, (upperboundX               ), indexXp1, indexY__);
    GRID_ARRAY_CODE(6+0, (lowerboundX || upperboundY), indexXm1, indexYp1);
    GRID_ARRAY_CODE(6+1, (               upperboundY), indexX__, indexYp1);
    GRID_ARRAY_CODE(6+2, (upperboundX || upperboundY), indexXp1, indexYp1);

#undef GRID_ARRAY_CODE

    return true;
};

void ns_world::grid::SectorGridShared::Build(const SectorGridShared& rSrc)
{   //  이것은 FieldInfo가 가지는 기본 섹터 정보를 복사하기 위함...
    // SectorGridInfo를 복사한다고 생각하자.  이것은 실행 중에 동작하는 함수다.

    m_SectorXYdist = rSrc.m_SectorXYdist; //.GetSectorXYDistance();
    m_SectorCount  = rSrc.SectorCount;
    CopyMemory(m_Boundaries, rSrc.m_Boundaries, sizeof(m_Boundaries));
    CopyMemory(m_Length, rSrc.m_Length, sizeof(m_Length));
}



void ns_world::grid::SectorGridShared::Build(const BvAABB& rSrc, const DWORD sectorDist /*meter*/)
{
    m_SectorXYdist = sectorDist;
    // input X - boundary
    {
        m_Boundaries[SG_X_MIN] = (DWORD)min(rSrc.m_wvMin.x, rSrc.m_wvMax.x);
        m_Boundaries[SG_Y_MIN] = (DWORD)min(rSrc.m_wvMin.y, rSrc.m_wvMax.y);
        m_Boundaries[SG_Z_MIN] = (DWORD)min(rSrc.m_wvMin.z, rSrc.m_wvMax.z);
    };

    DWORD& rLenX = m_Length[LEN_X];
    DWORD& rLenY = m_Length[LEN_Y];
    DWORD& rLenZ = m_Length[LEN_Z];
    {
        rLenX = (DWORD)abs(rSrc.m_wvMax.x - rSrc.m_wvMin.x);
        rLenY = (DWORD)abs(rSrc.m_wvMax.y - rSrc.m_wvMin.y);
        rLenZ = (DWORD)abs(rSrc.m_wvMax.z - rSrc.m_wvMin.z);
    };
    {
        m_Boundaries[SG_X_MAX] = (DWORD)rLenX;
        m_Boundaries[SG_Y_MAX] = (DWORD)rLenY;
        m_Boundaries[SG_Z_MAX] = (DWORD)rLenZ;
    };
    {
        rLenX = (DWORD)(rLenX / m_SectorXYdist) + !!(rLenX % m_SectorXYdist);
        rLenY = (DWORD)(rLenY / m_SectorXYdist) + !!(rLenY % m_SectorXYdist);
        rLenZ = (DWORD)(rLenZ / m_SectorXYdist) + !!(rLenZ % m_SectorXYdist);
    };
    {
        m_SectorCount = rLenX * rLenY;
    };
}

void ns_world::grid::SectorGrid::Init()
{
    if (FlowControl::FCAssert(sector_array_ == NULL && m_SectorCount != 0) == false) {
        __debugbreak(); // NOTE: think more detailed cases
    };
    
    const size_t allocating_size = sizeof(SectorGrid::Node) * m_SectorCount;
    ;{
        void* ptr = ::malloc(allocating_size);
        ZeroMemory(ptr, allocating_size);
        sector_array_ = reinterpret_cast<SectorGrid::Node*>(ptr);
    };
    SectorGrid::Node* sector_it = sector_array_;
    const SectorGrid::Node * const sector_end = &sector_array_[m_SectorCount];
    for (; sector_it != sector_end; ++sector_it)
    {
        util::LList::Init(sector_it);
    };
};

void ns_world::grid::SectorGrid::Release()
{
    if (m_SectorCount == 0) {
        return;
    };
    if (sector_array_ == NULL) {
        return;
    };
    SectorGrid::Node* sector_it = sector_array_;
    const SectorGrid::Node * const sector_end = &sector_array_[m_SectorCount];
    for (; sector_it != sector_end; ++sector_it)
    {
        SectorGrid::Node* const user_node_end = sector_it;
        SectorGrid::Node* user_node_it = sector_it->next;
        while (user_node_it != user_node_end)
        {
            ns_world::WorldUser* user = user_node_it->user;
            __TOUCHED(user);
            user_node_it = util::LList::Delete(user_node_it);
        };
    };

    ::free(sector_array_);
    sector_array_ = NULL;
};

bool ns_world::grid::SectorGrid::Link(Node* const user_node, const WzVector& user_position)
{
    util::LList::Delete(user_node);
    if (m_SectorCount == 0 || sector_array_ == NULL) {
        return false;
    };
    if (IsNullVector(&user_position)) {
        // unset position
        return false;
    };
    const SECTOR_IDX center_index = this->GetSectorIndex(&user_position);
    if (center_index == INVALID_SECTOR_INDEX) {
        return false;
    };
    Node* const sector_root = &sector_array_[center_index];
    util::LList::AddPrev(user_node, sector_root);
    return true;
};
