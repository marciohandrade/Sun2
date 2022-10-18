#include "stdafx.h"
#include "./FieldInfo.h"

//==================================================================================================
// FieldInfo class 
//
FieldInfo::FieldInfo(SharedFieldInfo* shared_field_info)
    : shared_field_info_(shared_field_info)
    , SectorGridInfoRef(sector_grid_info_)
    , SectorGridVResolutions(sector_grid_various_resolutions_)
{
}

FieldInfo::~FieldInfo()
{
}

int FieldInfo::Init()
{
#ifdef _DEV_TAIYO_PATHENGINE_CODE
    int index1 = GetWorldBase()->x_pRoomManager->DuplicateRoomInfo();

    if (shared_field_info_->IsNewPathEngine())
    {
        int index2 = GetWorldBase()->x_pPathFinder->DuplicateDynamicCollision();
        if (index1 != index2)
        {
            SUNLOG(eCRITICAL_LOG, "[FieldInfo::Init] index1[%d] != index2[%d]", index1, index2);
        }
    }

    return index1;
#else
    CRoomManager* room_manager = GetWorldBase()->x_pRoomManager;
    return room_manager->DuplicateRoomInfo();
#endif
}


void FieldInfo::Release(int index)
{
#ifdef _DEV_TAIYO_PATHENGINE_CODE
    if (shared_field_info_->IsNewPathEngine())
    {
        BOOL rt1 = GetWorldBase()->x_pPathFinder->DestroyDynamicCollision(index);
        ASSERT(rt1);
    }
#endif
    CRoomManager* room_manager = GetWorldBase()->x_pRoomManager;
    BOOL destroy_result = room_manager->DestroyRoomInfo(index);
    ASSERT(destroy_result);
}


///< FieldInfo를 읽은 다음,  SectorSize에 따른 SectorInfo를 셋팅
void FieldInfo::EstablishSectorInfo(DWORD sector_size)
{
    ASSERT(sector_size >= 30);
    BOOST_STATIC_ASSERT(VRES_MAX == VRES_800_PERCENT + 1);
    // sector_grid_info_은 기본 섹터 형태를 보존하기 위함... 따라서 ASSERT는 유지하기로 한다.
    const BvAABB bvAABB = GetWorldBase()->GetTerrainSize();
    sector_grid_info_.Build(bvAABB, sector_size);
    //
#define FORMULA_SECTOR_GRID_RESOLUTION(_sector_distance, _resolution) \
    DWORD((_sector_distance) * 100 / (_resolution))

    // CHANGES: f110415.3L, prevent constructor usage to solve sun:r53320 case
#define BUILD_SECTOR_GRID_RESOLUTION(_index, _resolution) \
    ;{ \
        SectorGridVR* _sector_grid_ = new (&sector_grid_various_resolutions_[_index]) \
            SectorGridVR(SectorGridVR::ProjectionGridWrapper(&sector_grid_info_)); \
        _sector_grid_->Rebuild(FORMULA_SECTOR_GRID_RESOLUTION(sector_size, _resolution)); \
    };

    BUILD_SECTOR_GRID_RESOLUTION(VRES_100_PERCENT, 100);
    BUILD_SECTOR_GRID_RESOLUTION(VRES_200_PERCENT, 200);
    BUILD_SECTOR_GRID_RESOLUTION(VRES_400_PERCENT, 400);
    // NOTE: f110308.1L, added new resolution to use more closed ranges
    BUILD_SECTOR_GRID_RESOLUTION(VRES_800_PERCENT, 800);
    //
}

// NOTE: f110321.2L, this method is used only "regen_pos.txt" parser that reserved deletion.
// this method don't used anymore on run-time service.
// if we want kind of this function, do use it of GameField.
bool FieldInfo::IsMovableCoordinatesRaw(const WzVector* position)
{
    // X,Y좌표상에서 최대값보다 큰 값을 클라이언트에서 보낸 경우
    const SECTOR_IDX coordinates = sector_grid_info_.GetSectorIndex(position);
    if (coordinates == INVALID_SECTOR_INDEX)
    {
        const DWORD* const boundary = sector_grid_info_.GetBoundaries();
        SUNLOG(eFULL_LOG,
               _T("["__FUNCTION__"] [Xmin, Xmax] = [%d, %d], [Ymin, Ymax] = [%d, %d] - ")
               _T("trying pos {%.f,%.f,%.f}"),
               boundary[SectorGridInfo::SG_X_MIN], boundary[SectorGridInfo::SG_X_MAX],
               boundary[SectorGridInfo::SG_Y_MIN], boundary[SectorGridInfo::SG_Y_MAX],
               position->x, position->y, position->z);
        return false;
    }

    // 맵상에서 이동 가능한 좌표인지 검사
    CPathFinderBase* const path_finder = GetWorldBase()->x_pPathFinder;
    const int result_tile = path_finder->GetAllowTileToStand(*position, NULL, -1.0f, 100.0f);
    return (result_tile != -1);
}

// (f100623.1L) change an interface to support a multi-checksum checker
bool FieldInfo::CheckMapVersion(EngineFileChecksum* stored_checksums,
                                const EngineFileChecksum& inputed_checksums) const
{
    const EngineFileChecksum& engine_checksums = this->shared_field_info_->GetFileChecksum();
    const bool passed_map_check = inputed_checksums.checksum_map == engine_checksums.checksum_map;
    const bool passed_wmo_check = inputed_checksums.checksum_wmo == engine_checksums.checksum_wmo;
    if (stored_checksums) {
        *stored_checksums = engine_checksums;
    }
    if (passed_map_check == false)
    {
        stored_checksums->pass_result = stored_checksums->Failed;
        return false;
    }
    const bool fail_proc = (g_pGameServer->GetMode() != eSERVER_DEV);
    if (passed_wmo_check == false)
    {
        stored_checksums->pass_result = fail_proc
                                      ?     stored_checksums->Failed
                                      :     stored_checksums->Passed_But_NotMatchedWmo;
        return (fail_proc ? false : true);
    }
    stored_checksums->pass_result = stored_checksums->Passed;
    return true;
}
