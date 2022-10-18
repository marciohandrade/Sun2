#pragma once
#ifndef GAMESERVER_FIELDINFO_H
#define GAMESERVER_FIELDINFO_H
//==================================================================================================
/// FieldInfo class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2004. 10. 24
    @purpose
    - SharedFieldInfo를 기반으로 Zone의 GameField의 종류마다 공유되는 클래스
    @history
    - 2006.2.20 ~ 2006.2.24 : 리펙토링
    - 2011.3.21 : waverix, code rearrangement
*/
//==================================================================================================

#include "MapDefine.h"
#include "SharedFieldInfo.h"
#include "SectorGrid.h"

//==================================================================================================
//
class MAPOBJECT_INFO
{
public:
    DWORD dwMapObjectID;
    WzID  wzId;
    DWORD dwAttribute;
    WzVector wvPos;
    WzVector wzRot;
    WzVector wzScale;
    WzBoundingVolume wzBV;
    DWORD dwExtraInfos[eMAPOBJECT_MAX];
    WzID  CollectionID; ///< 유저가 입력한 WZID.
};

//==================================================================================================
// FieldInfo
//
class FieldInfo
{
public:
    FieldInfo(SharedFieldInfo* shared_field_info);
    ~FieldInfo();

    // 각 GameField마다 호출되어야 하는 함수
    int Init();
    void Release(int index);
    void EstablishSectorInfo(DWORD sector_size);
    //
    CWorldBase* GetWorldBase();
    MAPCODE GetFieldCode() const;
    SharedFieldInfo* GetSharedFieldInfo();
    bool IsValidCoordinates(const WzVector* position) const;
    // NOTE: f110321.2L, this method is used only "regen_pos.txt" parser that reserved deletion.
    // this method don't used anymore on run-time service.
    // if we want kind of this function, do use it of GameField.
    bool IsMovableCoordinatesRaw(const WzVector* position);

    // (f100623.1L) change an interface to support a multi-checksum checker
    bool CheckMapVersion(EngineFileChecksum* stored_checksums,
                         const EngineFileChecksum& inputed_checksums) const;

    DWORD GetRandomStartingTileIndex(BYTE class_code);
    TriggerGroupInfo* GetTriggerGroupInfo(const TCHAR* group_name);
    WzSpecialArea* GetAreaInfo(WzID area_id);
    WzSpecialArea* GetAreaInfoByIndex(int index);

    // fields...
    // layouts : shared, resoultions...
private:
    SharedFieldInfo* shared_field_info_;

public:
    enum eResolutions
    {
        VRES_100_PERCENT,
        VRES_200_PERCENT, VRES_400_PERCENT,
        // NOTE: f110308.1L, added new resolution to use more closed ranges.
        VRES_800_PERCENT,
        VRES_MAX
    };
    typedef util::ARRAY_TYPEINFO< SectorGridVR, VRES_100_PERCENT, VRES_MAX >  Resolutions;
    const SectorGridInfo& SectorGridInfoRef;
    const Resolutions::array_type& SectorGridVResolutions;
private:
    SectorGridInfo sector_grid_info_; //  기본 해당 필드가 가지는 정규 섹터 정보
    Resolutions::array_type sector_grid_various_resolutions_; // 가변 해상도를 가지는 BuiltinSectorGrids
    //
    __DISABLE_COPY(FieldInfo);
};


//==================================================================================================
//

inline CWorldBase* FieldInfo::GetWorldBase() {
    return shared_field_info_->GetWorldBase();
}

inline MAPCODE FieldInfo::GetFieldCode() const {
    return shared_field_info_->GetCode();
}

inline SharedFieldInfo* FieldInfo::GetSharedFieldInfo() {
    return shared_field_info_;
}

inline bool FieldInfo::IsValidCoordinates(const WzVector* position) const
{
    return sector_grid_info_.IsValidCoordinate(position);
}

inline DWORD FieldInfo::GetRandomStartingTileIndex(BYTE class_code) {
    return shared_field_info_->GetRandomStartingTileIndex(class_code);
}

inline TriggerGroupInfo* FieldInfo::GetTriggerGroupInfo(const TCHAR* group_name) {
    return shared_field_info_->GetTriggerGroupInfo(group_name);
}

inline WzSpecialArea* FieldInfo::GetAreaInfo(WzID area_id)
{
    if (CWorldBase* world_base = shared_field_info_->GetWorldBase()) {
        return world_base->GetSpecialArea(area_id);
    }
    return NULL;
}

inline WzSpecialArea* FieldInfo::GetAreaInfoByIndex(int index)
{
    CWorldBase* world_base = shared_field_info_->GetWorldBase();
    // CHANGES: f110928.3L
    const int number_of_areas = world_base->GetNumberOfSpecialArea();
    if (index < 0 || index >= number_of_areas) {
        return NULL;
    };
    return world_base->GetSpecialAreaByIndex(index);
}

#endif //GAMESERVER_FIELDINFO_H