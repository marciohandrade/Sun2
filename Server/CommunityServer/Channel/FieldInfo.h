#pragma once
#ifndef WORLDSERVER_FIELD_INFO_H
#define WORLDSERVER_FIELD_INFO_H

#include "Channel/SectorGrid.h"

class CDiscAccess;// 3d engine class

namespace ns_world { namespace community {
;

class FieldInfo
{
public:
    FieldInfo();
    ~FieldInfo();

    bool Create(const sFIELDINFO * field_info, bool load_terrain, CDiscAccess* disc_access);
    void Destroy();
    FIELDCODE GetFieldCode() const;
    const ns_world::grid::SectorGridInfo& GetSectorGridInfo() const;

private:
    // data fields...
    const sFIELDINFO* field_info_;
    ns_world::grid::SectorGridInfo sector_grid_info_;
    //
    __DISABLE_COPY(FieldInfo);
};

}}; //end of namespace

//==================================================================================================

inline FIELDCODE ns_world::community::FieldInfo::GetFieldCode() const
{
    return (field_info_) ? field_info_->FieldCode : 0;
}

inline const ns_world::grid::SectorGridInfo&
ns_world::community::FieldInfo::GetSectorGridInfo() const
{
    return sector_grid_info_;
}

#endif //WORLDSERVER_FIELD_INFO_H