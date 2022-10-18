#include "WorldServerPch.hxx"
#include "./FieldInfo.h"

#include <io.h>
#pragma warning(push)
#pragma warning(disable : 4100)
#include <client/3DEngine/3DTerrain/WorldBase.h>
#pragma warning(pop)
#include "WorldServer.h"

//==================================================================================================

ns_world::community::FieldInfo::FieldInfo()
    : field_info_(NULL)
{
}

ns_world::community::FieldInfo::~FieldInfo()
{
};

bool ns_world::community::FieldInfo::Create(
    const sFIELDINFO* field_info, bool load_terrain, CDiscAccess* disc_access)
{
    if (FlowControl::FCAssert(field_info_ == NULL) == false) {
        return false;
    };
    field_info_ = field_info;
    if (load_terrain == false)
    {
        sector_grid_info_.Destroy();
        return true;
    };

    //const ns_world::WorldServer::Engine& engine = ns_world::WorldServer::Instance()->GetEngine();
    //CWorldBase* const world_base = (CWorldBase*)engine.instance(CT_NEW_WORLD, 0);
    //world_base->Create();
    //
    struct ResouceReleaser {
        ResouceReleaser(CDiscAccess* disc_access)
        {
            const ns_world::WorldServer::Engine& engine = \
                ns_world::WorldServer::Instance()->GetEngine();
            world_base_ = (CWorldBase*)engine.instance(CT_NEW_WORLD, 0);
            world_base_->Create(false);
            world_base_->SetDiscAccess(disc_access);
        }
        ~ResouceReleaser()
        {
            archive_.Close();
            world_base_->Destroy(NULL);
            const ns_world::WorldServer::Engine& engine = \
                ns_world::WorldServer::Instance()->GetEngine();
            engine.instance(CT_DELETE_WORLD, world_base_);
        };
        CWorldBase* world_base_;
        CWzArchive archive_;
    } helper(disc_access);

    ASSERT(_access(field_info->pszPath, 0) != -1);

    if (helper.archive_.LoadFile(const_cast<char *>(field_info->pszPath)) == false)
    {
        SUNLOG(eCRITICAL_LOG, "|["__FUNCTION__"]|Msg=field load failed, path='%s'",
               field_info->pszPath);
        return false;
    }
    if (helper.world_base_->Serialize(&helper.archive_) == false)
    {
        SUNLOG(eCRITICAL_LOG, "|["__FUNCTION__"]|Msg=World serializing failed, path='%s'",
               field_info->pszPath);
        return false;
    };
    //
    BOOST_STATIC_ASSERT(VILLAGE_SECTOR_SIZE == FIELD_SECTOR_SIZE &&
                        VILLAGE_SECTOR_SIZE == ROOM_SECTOR_SIZE &&
                        VILLAGE_SECTOR_SIZE == DOMINATION_SECTOR_SIZE);
    BvAABB world_ranges = helper.world_base_->GetTerrainSize();
    sector_grid_info_.Build(world_ranges, VILLAGE_SECTOR_SIZE);

    return true;
}

void ns_world::community::FieldInfo::Destroy()
{
}



