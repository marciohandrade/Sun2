#include "stdafx.h"
#include "./MapInfoParser.h"

#include <algorithm>
#include <iostream>

#include "Struct.h"

//==================================================================================================
//==================================================================================================
//==================================================================================================

inline MapGroupBase::MapGroupBase()
{
}

MapGroupBase::~MapGroupBase()
{
    map_infos_by_index_.clear();
    map_infos_by_code_.clear();
}

void MapGroupBase::AddMapInfoIndex(sMAPINFO* map_info, WORD index/*0/1/2*/)
{
    map_infos_by_index_[index] = map_info;
}

void MapGroupBase::AddMapInfoCode(sMAPINFO* map_info, MAPCODE map_code)
{
    map_infos_by_code_[map_code] = map_info;
}

sMAPINFO* MapGroupBase::GetMapInfoIndex(WORD index) const
{ 
    sMAPINFO_HASH::const_iterator it = map_infos_by_index_.find(index);
    if (it == map_infos_by_index_.end()) {
        return NULL;
    };
    sMAPINFO* map_info = it->second;
    return map_info;
}

sMAPINFO* MapGroupBase::GetMapInfoCode(MAPCODE map_code) const
{
    sMAPINFO_HASH::const_iterator it = map_infos_by_code_.find(map_code);
    if (it == map_infos_by_code_.end()) {
        return NULL;
    };
    sMAPINFO* map_info = it->second;
    return map_info;
}

//==================================================================================================

namespace util { namespace internal {
;

struct MapSubInfo
{
    MAPCODE     MapCode;
    int         FIndex;
    MAPCODE     FCode;
    char        FGCode[sizeof(WzID) + 1];
    FIELDCODE   FWMOCode;
    FIELDCODE   FWUTCode;
    FIELDCODE   MMCode; //MiniMap Code
    char        pszDebugName[MAX_MAPNAME_LENGTH];
};

// debugging facitlities
typedef sMAPINFO sMAPINFO_ARRAY[300];
typedef sFIELDINFO sFIELDINFO_ARRAY[300];
typedef sWMOINFO sWMOINFO_ARRAY[300];
typedef sWUTINFO sWUTINFO_ARRAY[300];
typedef sMAP_ENTERENCE_PORTAL sMAP_ENTERENCE_PORTAL_ARRAY[300];
typedef MapSubInfo sMAPSUBINFO_ARRAY[300];

}}; //end of namespace

//==================================================================================================

struct MapInfoParser::MapInfoParserWorkingContainer
{
    void Initialize();
    void Release();
    //
    int number_of_subinfos_;
    util::internal::MapSubInfo* subinfo_array_;
};

void MapInfoParser::MapInfoParserWorkingContainer::Initialize()
{
    ZeroMemory(this, sizeof(*this));
};

void MapInfoParser::MapInfoParserWorkingContainer::Release()
{
    if (subinfo_array_) {
        ::free(subinfo_array_);
        subinfo_array_ = NULL;
    };
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

MapInfoParser* MapInfoParser::instance_static_ = NULL;

MapInfoParser* MapInfoParser::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(MapInfoParser** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        MapInfoParser** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    MapInfoParser* provider = new MapInfoParser;
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

// old util::Singleton support interface
void MapInfoParser::DestroyInstance()
{
    SAFE_DELETE(instance_static_);
}

//==================================================================================================

MapInfoParser::MapInfoParser()
    : number_of_loaded_(0)
    , instance_map_start_index_(0)
    , map_info_array_(NULL)
    , field_info_array_(NULL)
    , wmo_info_array_(NULL)
    , wut_info_array_(NULL)
    , portal_info_array_(NULL)
    , working_container_(new MapInfoParserWorkingContainer)
{
    working_container_->Initialize();
}

MapInfoParser::~MapInfoParser()
{
    Release();
}

void MapInfoParser::Init(DWORD unused)
{
    __UNUSED(unused);
}

void MapInfoParser::Release()
{
    Unload();
}

BOOL MapInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    __UNUSED(reloaded);
    bool result = false;
    switch (script_code)
    {
    case SCRIPT_FIELD:
        result = _LoadField();
        break;
    case SCRIPT_FIELDWMO:
        result = _LoadFieldWMO();
        break;
    case SCRIPT_FIELDWUT:
        result = _LoadFieldWUT();
        break;
    case SCRIPT_WORLD:
        result = _LoadWorld();
        break;
    case SCRIPT_MAPENTERANCEPORTAL:
        result = _LoadMapEnterencePortal();
        break;
    case SCRIPT_WORLD_SUB_INFO:
        result = _LoadWorldSubInfo();
        break;
    }
    if (result != false) {
        ASSERT(number_of_loaded_ < kNumberOfTotalLoaded);
        ++number_of_loaded_;
    };
    return result;
}

void MapInfoParser::Unload()
{
    if (number_of_loaded_ == 0) {
        return;
    };
    number_of_loaded_ = 0; // prevent to unload six times in the parser releasing sequence
    //
    map_info_table_.clear();
    field_info_table_.clear();
    FOREACH_CONTAINER(const MAPGROUP_HASH::value_type& node, map_group_table_, MAPGROUP_HASH) {
        MapGroupBase* map_group = const_cast<MapGroupBase*>(node.second);
        delete map_group;
    };
    map_group_table_.clear();
    //
    map_portal_table_.clear();
    wmo_info_table_.clear();
    wut_info_table_.clear();

    if (map_info_array_) {
        ::free(map_info_array_);
        map_info_array_ = NULL;
    };
    if (field_info_array_) {
        ::free(field_info_array_);
        field_info_array_ = NULL;
    };
    if (wmo_info_array_) {
        ::free(wmo_info_array_);
        wmo_info_array_ = NULL;
    };
    if (wut_info_array_) {
        ::free(wut_info_array_);
        wut_info_array_ = NULL;
    };
    if (portal_info_array_) {
        ::free(portal_info_array_);
        portal_info_array_ = NULL;
    };
    if (working_container_)
    {
        working_container_->Release();
        delete working_container_; // f110513.4L, fixed invalid releasing
        working_container_ = NULL;
    };
}

//==================================================================================================

sFIELDINFO* MapInfoParser::GetFieldInfo(DWORD field_code) const {
    if (field_code == 0 || field_code > USHRT_MAX) {
        return NULL;
    };
    sFIELDINFO_HASH::const_iterator found = field_info_table_.find(field_code);
    if (found == field_info_table_.end()) {
        return NULL;
    };
    sFIELDINFO* field_info = found->second;
    return field_info;
}

MapGroup* MapInfoParser::GetMapGroup(CODETYPE map_kind) const {
    if (map_kind == 0 || map_kind > USHRT_MAX) {
        return NULL;
    };
    MAPGROUP_HASH::const_iterator found = map_group_table_.find(map_kind);
    if (found == map_group_table_.end()) {
        return NULL;
    };
    MapGroup* map_group = found->second;
    return map_group;
}

sMAP_ENTERENCE_PORTAL* MapInfoParser::GetMapPortal(CODETYPE index) const {
    if (index == 0 || index > USHRT_MAX) {
        return NULL;
    };
    sMAPPORTAL_HASH::const_iterator found = map_portal_table_.find(index);
    if (found == map_portal_table_.end()) {
        return NULL;
    };
    sMAP_ENTERENCE_PORTAL* portal_info = found->second;
    return portal_info;
}

const sWMOINFO* MapInfoParser::FindWMOInfo(FIELDCODE wmocode) const
{
    sWMOINFO_HASH::const_iterator it = wmo_info_table_.find(wmocode);
    if (it == wmo_info_table_.end()) {
        return NULL;
    };
    sWMOINFO* wmo_info = it->second;
    return wmo_info;
}

const sWUTINFO* MapInfoParser::FindWUTInfo(FIELDCODE wmocode) const
{
    sWUTINFO_HASH::const_iterator it = wut_info_table_.find(wmocode);
    if (it == wut_info_table_.end()) {
        return NULL;
    };
    sWUTINFO* wut_info = it->second;
    return wut_info;
}

const sMAPINFO* MapInfoParser::FindMapInfo(MAPCODE mapcode) const
{
    sMAPINFO_HASH::const_iterator it = map_info_table_.find(mapcode);
    if (it == map_info_table_.end()) {
        return NULL;
    };
    sMAPINFO* map_info = it->second;
    return map_info;
}

//==================================================================================================

void MapInfoParser::AddMapGroup(MapGroup* map_group, WORD map_kind)
{
    MAPGROUP_HASH::_Pairib result = map_group_table_.insert(
        MAPGROUP_HASH::value_type(map_kind, map_group));
    ASSERT(result.second == true);
}

void MapInfoParser::AddMapInfo(sMAPINFO* map_info, MAPCODE map_code)
{
    sMAPINFO_HASH_BASE::_Pairib result = map_info_table_.insert(
        sMAPINFO_HASH_BASE::value_type(map_code, map_info));
    ASSERT(result.second == true);
}

void MapInfoParser::AddMapPortal(sMAP_ENTERENCE_PORTAL* portal_info, WORD index)
{
    sMAPPORTAL_HASH::_Pairib result = map_portal_table_.insert(
        sMAPPORTAL_HASH::value_type(index, portal_info));
    ASSERT(result.second == true);
}

//==================================================================================================
// parse 'Worldsubinfo.txt'
bool MapInfoParser::_LoadWorldSubInfo()
{
    const int max_rows = GetRowSize();
    if (max_rows == 0 || working_container_->number_of_subinfos_) {
        return false;
    };
    typedef util::internal::MapSubInfo MapSubInfo;
    const size_t buffer_size = sizeof(MapSubInfo) * max_rows;
    MapSubInfo* const subinfo_begin = reinterpret_cast<MapSubInfo*>(::malloc(buffer_size))
    ;{
        ZeroMemory(subinfo_begin, buffer_size);
        working_container_->number_of_subinfos_ = max_rows;
        working_container_->subinfo_array_ = subinfo_begin;
    };
#ifdef _DEBUG
    const util::internal::sMAPSUBINFO_ARRAY& memory_map_for_debug =
        *(util::internal::sMAPSUBINFO_ARRAY*)(
            working_container_->subinfo_array_);
    __TOUCHED(memory_map_for_debug);
#endif

    for (int row = 0; row < max_rows; ++row)
    {
        MapSubInfo* const subinfo = &subinfo_begin[row];
        DWORD map_code = GetDataDWORD("MapCode", row);
        ASSERT(map_code != 0 && (SAFE_NUMERIC_TYPECAST(DWORD, map_code, MAPCODE)));

        subinfo->MapCode = static_cast<MAPCODE>(map_code);
        subinfo->FIndex  = GetDataInt("Findex", row);
        subinfo->FCode   = GetDataWORD("FCode", row);
        subinfo->FWMOCode = GetDataWORD("WMOCode", row);
        subinfo->FWUTCode = GetDataWORD("WUTCode", row);
        subinfo->MMCode = GetDataWORD("MMCode", row);
        strncpy(subinfo->FGCode, GetDataString("FGCode", row), _countof(subinfo->FGCode));
        subinfo->FGCode[_countof(subinfo->FGCode) - 1] = '\0';
        strncpy(subinfo->pszDebugName,  GetDataString("MName", row), _countof(subinfo->pszDebugName));
        subinfo->pszDebugName[_countof(subinfo->pszDebugName) - 1] = '\0';
    }

    return true;

}

//==================================================================================================
// parse 'MapEnterencePortal.txt'
bool MapInfoParser::_LoadMapEnterencePortal()
{
    const int max_rows = GetRowSize();
    if (!FlowControl::FCAssert(max_rows != 0 && portal_info_array_ == NULL)) {
        return false;
    };
    ;{
        const size_t buffer_size = sizeof(*portal_info_array_) * max_rows;
        portal_info_array_ = reinterpret_cast<sMAP_ENTERENCE_PORTAL_BASE*>(::malloc(buffer_size));
        ZeroMemory(portal_info_array_, buffer_size);
    };
#ifdef _DEBUG
    const util::internal::sMAP_ENTERENCE_PORTAL_ARRAY& memory_map_for_debug =
        *(util::internal::sMAP_ENTERENCE_PORTAL_ARRAY*)(portal_info_array_);
    __TOUCHED(memory_map_for_debug);
#endif

    for (int row = 0; row < max_rows; ++row)
    {
        sMAP_ENTERENCE_PORTAL_BASE* const portal_info = &portal_info_array_[row];
        portal_info->wIndex = GetDataWORD("wIndex", row);
        portal_info->byMapType = GetDataBYTE("bMapType", row);
        portal_info->bMoveType = GetDataBYTE("bMoveType", row);
    #ifdef _NA_000816_20101025_AIRSHIP_SYSTEM
        portal_info->npc_id_ = GetDataWORD(_T("NpcID"), row);
    #endif //
        portal_info->wWorldFrom = GetDataWORD("wWorldFrom", row);
        portal_info->wFieldFrom = GetDataWORD("wFieldFrom", row);
        portal_info->wWorldTo = GetDataWORD("wWorldTo", row);
        portal_info->wFieldTo = GetDataWORD("wFieldTo", row);
        portal_info->byMin_Level = GetDataBYTE("bMin_Level", row);
        portal_info->byMax_Level = GetDataBYTE("bMax_Level", row);
        portal_info->byMissionCode = GetDataBYTE("wMissionCode", row);
        portal_info->wQuestCode = GetDataWORD("wQuestCode", row);
        portal_info->wItemCode = GetDataWORD("wItemCode", row);
        portal_info->wItemNum = GetDataWORD("bItemNum", row);
        portal_info->bWaste_Item = GetDataBYTE("dWaste_Item", row);
        portal_info->dwHeim = GetDataDWORD("dHeim", row);

        strncpy(portal_info->sAreaFromID, GetDataString("sAreaFromID", row),
                _countof(portal_info->sAreaFromID));
        portal_info->sAreaFromID[_countof(portal_info->sAreaFromID) - 1] = '\0';
        if (portal_info->sAreaFromID[0] != 0) {
            portal_info->m_dwFromAreaID = StrToWzID(portal_info->sAreaFromID);
        };

        strncpy(portal_info->sAreaToID, GetDataString("sAreaToID", row),
                _countof(portal_info->sAreaToID));
        portal_info->sAreaToID[_countof(portal_info->sAreaToID) - 1] = '\0';

        if (portal_info->sAreaToID[0] != 0) {
            portal_info->m_dwToAreaID = StrToWzID(portal_info->sAreaToID);
        };

    #ifdef _NA_20100307_BETAKEY_SYSTEM
        portal_info->nBeytaKey = GetDataInt("BetaKeyCode", row);
        portal_info->fPayApp = GetDataFloat("PayApp", row);
    #endif

        FASSERT(portal_info->byMapType < sMAP_ENTERENCE_PORTAL::MAP_T_UPPERBOUND);
        FASSERT(portal_info->bMoveType < sMAP_ENTERENCE_PORTAL::MOVE_TYPE_UPPER);
        FASSERT(portal_info->byMin_Level< sMAP_ENTERENCE_PORTAL::LEVEL_UPPERBOUND);
        FASSERT(portal_info->byMax_Level< sMAP_ENTERENCE_PORTAL::LEVEL_UPPERBOUND);
        FASSERT(portal_info->byMissionCode< sMAP_ENTERENCE_PORTAL::MISSION_CODE_UPPERBOUND);
        FASSERT(portal_info->wQuestCode < sMAP_ENTERENCE_PORTAL::QUEST_CODE_UPPERBOUND);
        FASSERT(portal_info->bWaste_Item < sMAP_ENTERENCE_PORTAL::WASTE_UPPERBOUND);

        FASSERT(NULL == GetMapPortal(portal_info->wIndex));
        AddMapPortal(portal_info, portal_info->wIndex);
    }
    return true;
}

//==================================================================================================
// parse 'fieldwmo.txt'
bool MapInfoParser::_LoadFieldWMO()
{
    const int max_rows = GetRowSize();
    if (!FlowControl::FCAssert(max_rows != 0 && wmo_info_array_ == NULL)) {
        return false;
    };
    ;{
        const size_t buffer_size = sizeof(*wmo_info_array_) * max_rows;
        wmo_info_array_ = reinterpret_cast<sWMOINFO_BASE*>(::malloc(buffer_size));
        ZeroMemory(wmo_info_array_, buffer_size);
    };
#ifdef _DEBUG
    const util::internal::sWMOINFO_ARRAY& memory_map_for_debug =
        *(util::internal::sWMOINFO_ARRAY*)(wmo_info_array_);
    __TOUCHED(memory_map_for_debug);
#endif

    char temp_str[MAX_PATH + 1];
    for (int row = 0; row < max_rows; ++row)
    {
        sWMOINFO_BASE* const wmo_info = &wmo_info_array_[row];
        wmo_info->Code = GetDataWORD("FieldCode", row);

        strncpy(temp_str, GetDataString("FieldMapDir", row), _countof(temp_str));
        temp_str[_countof(temp_str) - 1] = '\0';
        //절대경로일 경우 경로 조합을 안하겠단 코드 입니다.
        if (NULL == strchr(temp_str, ':'))
        {
            _snprintf(wmo_info->pszPath, _countof(wmo_info->pszPath),
                      "data\\%s", temp_str);
            wmo_info->pszPath[_countof(wmo_info->pszPath) - 1] = '\0';
        }
        else
        {
            _snprintf(wmo_info->pszPath, _countof(wmo_info->pszPath), "%s", temp_str);
            wmo_info->pszPath[_countof(wmo_info->pszPath) - 1] = '\0';
        }

        if (FindWMOInfo(wmo_info->Code) == NULL)
        {
            wmo_info_table_[wmo_info->Code] = wmo_info;
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "Script Data Fail, File = FieldWMO.txt, Code = %d",
                   wmo_info->Code);
            ASSERT(0);
        }
    }

    return true;
}

//==================================================================================================
// parse 'fieldwut.txt'
bool MapInfoParser::_LoadFieldWUT()
{
    const int max_rows = GetRowSize();
    if (!FlowControl::FCAssert(max_rows != 0 && wut_info_array_ == NULL)) {
        return false;
    };
    ;{
        const size_t buffer_size = sizeof(*wut_info_array_) * max_rows;
        wut_info_array_ = reinterpret_cast<sWUTINFO_BASE*>(::malloc(buffer_size));
        ZeroMemory(wut_info_array_, buffer_size);
    };
#ifdef _DEBUG
    const util::internal::sWUTINFO_ARRAY& memory_map_for_debug =
        *(util::internal::sWUTINFO_ARRAY*)(wut_info_array_);
    __TOUCHED(memory_map_for_debug);
#endif

    char temp_str[MAX_PATH + 1];
    for (int row = 0; row < max_rows; ++row)
    {
        sWUTINFO_BASE* const wut_info = &wut_info_array_[row];
        wut_info->Code = GetDataWORD("FieldCode", row);

        strncpy(temp_str, GetDataString("FieldMapDir", row), _countof(temp_str));
        temp_str[_countof(temp_str) - 1] = '\0';
        //절대경로일 경우 경로 조합을 안하겠단 코드 입니다.
        if (NULL == strchr(temp_str, ':'))
        {
            _snprintf(wut_info->pszPath, _countof(wut_info->pszPath),
                      "data\\%s", temp_str);
            wut_info->pszPath[_countof(wut_info->pszPath) - 1] = '\0';
        }
        else
        {
            _snprintf(wut_info->pszPath, _countof(wut_info->pszPath), "%s", temp_str);
            wut_info->pszPath[_countof(wut_info->pszPath) - 1] = '\0';
        }

        if (FindWUTInfo(wut_info->Code) == NULL)
        {
            wut_info_table_[wut_info->Code] = wut_info;
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "Script Data Fail, File = FieldWUT.txt, Code = %d", wut_info->Code);
            ASSERT(0);
        }
    }

    return true;
}

//==================================================================================================
// parse 'field.txt'
bool MapInfoParser::_LoadField()
{
    const int max_rows = GetRowSize();
    if (!FlowControl::FCAssert(max_rows != 0 && field_info_array_ == NULL)) {
        return false;
    };
    ;{
        const size_t buffer_size = sizeof(*field_info_array_) * max_rows;
        field_info_array_ = reinterpret_cast<sFIELDINFO_BASE*>(::malloc(buffer_size));
        ZeroMemory(field_info_array_, buffer_size);
    };
#ifdef _DEBUG
    const util::internal::sFIELDINFO_ARRAY& memory_map_for_debug =
        *(util::internal::sFIELDINFO_ARRAY*)(field_info_array_);
    __TOUCHED(memory_map_for_debug);
#endif

    char temp_str[MAX_PATH + 1];
    for (int row = 0; row < max_rows; ++row)
    {
        sFIELDINFO_BASE* const field_info = &field_info_array_[row];
        field_info->FieldCode = GetDataWORD("FieldCode", row);

        strncpy(temp_str, GetDataString("FieldMapDir", row), _countof(temp_str));
        temp_str[_countof(temp_str) - 1] = '\0';
        //절대경로일 경우 경로 조합을 안하겠단 코드 입니다.
        if (NULL == strchr(temp_str, ':'))
        {
            _snprintf(field_info->pszPath, _countof(field_info->pszPath),
                      "data\\%s", temp_str);
            field_info->pszPath[_countof(field_info->pszPath) - 1] = '\0';
        }
        else
        {
            _snprintf(field_info->pszPath, _countof(field_info->pszPath), "%s", temp_str);
            field_info->pszPath[_countof(field_info->pszPath) - 1] = '\0';
        }

        field_info->byServerType = GetDataBYTE("BySeverType", row);
        field_info->bNewPathEngine = GetDataBYTE("UseNewPathEngine", row);
        field_info->bWarCastle= GetDataBYTE("IsWarCastleMap", row);

        if (GetFieldInfo(field_info->FieldCode) == NULL)
        {
            field_info_table_[field_info->FieldCode] = field_info;
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "Script Data Fail, File =  Field.txt, FieldCode = %d", field_info->FieldCode);
            FASSERT(0);
        }
    }

    return true;
}


//==================================================================================================
// parse 'world.txt'
bool MapInfoParser::_LoadWorld()
{
    // NOTE: we must loaded worldsubinfo.txt first before world.txt script loading.
    FASSERT(this->working_container_ != NULL &&
            working_container_->subinfo_array_ != NULL);
    //
    const int max_rows = GetRowSize();
    if (!FlowControl::FCAssert(max_rows != 0 && map_info_array_ == NULL)) {
        return false;
    };
    ;{
        const size_t buffer_size = sizeof(*map_info_array_) * max_rows;
        map_info_array_ = reinterpret_cast<sMAPINFO_BASE*>(::malloc(buffer_size));
        ZeroMemory(map_info_array_, buffer_size);
        BOOST_STATIC_ASSERT(eZONE_RULESET::ZONE_RULE_EMPTY == 0);
    };
#ifdef _DEBUG
    const util::internal::sMAPSUBINFO_ARRAY& subinfo_mmap_for_debug =
        *(util::internal::sMAPSUBINFO_ARRAY*)(
            working_container_->subinfo_array_);
    __TOUCHED(subinfo_mmap_for_debug);
    const util::internal::sMAP_ENTERENCE_PORTAL_ARRAY& portal_info_mmap_for_debug =
        *(util::internal::sMAP_ENTERENCE_PORTAL_ARRAY*)(portal_info_array_);
    __TOUCHED(portal_info_mmap_for_debug);
    const util::internal::sWMOINFO_ARRAY& wmo_info_mmap_for_debug =
        *(util::internal::sWMOINFO_ARRAY*)(wmo_info_array_);
    __TOUCHED(wmo_info_mmap_for_debug);
    const util::internal::sWUTINFO_ARRAY& wut_info_mmap_for_debug =
        *(util::internal::sWUTINFO_ARRAY*)(wut_info_array_);
    __TOUCHED(wut_info_mmap_for_debug);
    const util::internal::sFIELDINFO_ARRAY& field_info_mmap_for_debug =
        *(util::internal::sFIELDINFO_ARRAY*)(field_info_array_);
    __TOUCHED(field_info_mmap_for_debug);
    const util::internal::sMAPINFO_ARRAY& map_info_mmap_for_debug =
        *(util::internal::sMAPINFO_ARRAY*)(map_info_array_);
    __TOUCHED(map_info_mmap_for_debug);
#endif

    for (int row = 0; row < max_rows; ++row)
    {
        sMAPINFO_BASE* const map_info = &map_info_array_[row];
        //map_info->FileVersion = 1.3;
        ;{
            CODETYPE map_code = GetDataDWORD("MapCode", row); //맵코드
            FASSERT(map_code != 0 && map_code <= USHRT_MAX);
            map_info->MapCode = static_cast<MAPCODE>(map_code);
            FASSERT(NULL == FindMapInfo(map_info->MapCode));
        };
        map_info->MapKind = GetDataWORD("Mapkind", row); //맵카인드
        map_info->byMKind = GetDataBYTE("MKind", row); //M카인드(맵속성 구분-미션인지 헌팅인지 뭔지..)
        map_info->byMType = GetDataBYTE("bMtype", row); //M타입(메인미션인지 서븐미션인지..)
        map_info->byMinUserNum = GetDataBYTE("b_min_user", row);
        map_info->byMaxUserNum = GetDataBYTE("bMaxuser", row);
        map_info->minLV = GetDataWORD("minLV", row);
        map_info->maxLV = GetDataWORD("maxLV", row);
        //
        map_info->byENTCount = GetDataBYTE("ENTCount", row);
        map_info->byFCount = GetDataBYTE("Fcount", row);
        //
        map_info->byClass = GetDataBYTE("Class1", row);    
        //
        // __NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__ = {
        map_info->completeQCode = GetDataWORD("CompleteQCode", row);
        map_info->completeMCode = GetDataWORD("CompleteMCode", row);
        map_info->freePassLV = GetDataWORD("FreePassLV", row);
        // }
        strncpy(map_info->pszDebugName, GetDataString("MName", row), _countof(map_info->pszDebugName));
        map_info->pszDebugName[_countof(map_info->pszDebugName) - 1] = '\0';
        map_info->dwNCode = GetDataDWORD("NCode", row); //N코드(네임코드)
        map_info->dwFNCode = GetDataDWORD("FNCode", row);//FN코드(필드네임코드)
        map_info->dwDCode = GetDataDWORD("DCode", row); //D코드(로비툴팁관련)
        map_info->dwANCode = GetDataDWORD("ANCode", row);//AN코드(메인맵 스트링 코드)// 맵 그룹에 대표 이름 (김미숙씨의 요청에 의해서 해당 필드를 넣었습니다. - 클라에서만 사용.)

        map_info->EntranceItemCode = GetDataWORD("Guildent", row);
        map_info->EntranceItemCount = GetDataBYTE("Guilditem", row);
        map_info->wTimeLimit = GetDataWORD("TimeLim", row);

    #ifdef _NA_20100307_BETAKEY_SYSTEM
        map_info->nBetaKey = GetDataWORD("BetaKeyCode", row);        
    #endif
        //------------------------------------------------------------------------------------------
        // CHANGES: f110504.2L, check valid rules
        FASSERT(map_info->CheckValidMapKind() != false);
        //------------------------------------------------------------------------------------------

        const BYTE DEFAULT_WZID = '0'; //48;
        char area_id_string[sizeof(WzID) + 1];
        ;{
            strncpy(area_id_string, GetDataString("StrartID", row), _countof(area_id_string));
            area_id_string[_countof(area_id_string) - 1] = '\0';
            map_info->wStartAreaID = StrToWzID(area_id_string);
            if (map_info->wStartAreaID == DEFAULT_WZID) {
                map_info->wStartAreaID = INVALID_WZID;
            };
        };
        ;{
            strncpy(area_id_string, GetDataString("StartID2", row), _countof(area_id_string));
            area_id_string[_countof(area_id_string) - 1] = '\0';
            map_info->wStartAreaID2 = StrToWzID(area_id_string);
            if (map_info->wStartAreaID2 == DEFAULT_WZID)  
            {
                map_info->wStartAreaID2 = INVALID_WZID;
                if (map_info->byMKind == eZONETYPE_CHUNTING)
                {
                    SUNLOG(eCRITICAL_LOG,
                           _T("[MapInfoParser::Load] StartID2 doesn't exist! MapCode[%d]"),
                           map_info->MapCode);
                }
            };
        };
        //
        // NOTE: enterable MapKind list of the map_info
        char column_buffer[MAX_FIELD_NAME_SIZE];
        // 1. 미션맵
        for (int i = 0; i < sMAPINFO::MAX_MISSION_MAP_NUM_V1_3; ++i)
        {
            _snprintf(column_buffer, _countof(column_buffer), "bMmap%d", i + 1);
            MAPCODE map_code = GetDataWORD(column_buffer, row);
            if (map_code >= MAX_MISSION_NUM) {
                SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
                       _T("Msg=invalid mission code(%d)|MapCode=%d,Column=%s"),
                       map_code, map_info->MapCode, column_buffer);
                continue;
            };
            map_info->byMissionMapVec[i] = map_code;
        }
        // 2. PVP맵
        for (int i = 0; i < sMAPINFO::MAX_PVP_MAP_NUM_V1_3; ++i)
        {
            _snprintf(column_buffer, _countof(column_buffer), "wPmap%d", i + 1);
            MAPCODE map_code = GetDataWORD(column_buffer, row);
            map_info->wPVPMapVec[i] = map_code;
        }
        // 3. 경쟁헌팅맵
        for (int i = 0; i < sMAPINFO::MAX_CHUNTING_MAP_NUM_V1_3; ++i)
        {
            _snprintf(column_buffer, _countof(column_buffer), "wCmap%d\0", i + 1);
            MAPCODE map_code = GetDataWORD(column_buffer, row);
            map_info->wCHuntingMapVec[i] = map_code;
        }

        map_info->wContinentCode = GetDataWORD("ContinentCode", row);
    #ifdef _DH_EP2_FULL_MAP
        map_info->ContienetNumber = GetDataInt("Continent", row);
    #endif//_DH_EP2_FULL_MAP

        //------------------------------------------------------------------------------------------
        // WARNING: last ordered process. add to list, build MapGroup
        FASSERT(ApplyRuleset(map_info));
        AddMapInfo(map_info, map_info->MapCode);

        // MapGroup
        MapGroupBase* map_group = const_cast<MapGroupBase*>(GetMapGroup(map_info->MapKind));
        if (map_group == NULL) {
            map_group = new MapGroupBase;
            AddMapGroup(map_group, map_info->MapKind);
        }
        else {
            FASSERT(NULL == map_group->GetMapInfoIndex(map_info->byMType));
        };
        map_group->AddMapInfoIndex(map_info, map_info->byMType);
        map_group->AddMapInfoCode(map_info, map_info->MapCode);
    }; //end 'foreach record in records'

    //----------------------------------------------------------------------------------------------
    // NOTE: the worldsubinfo.txt script is pre-loaded.
    // alignment approach changes to reversed custom logic sequence based on the script.
    typedef util::internal::MapSubInfo MapSubInfo;
    const MapSubInfo* subinfo_it = working_container_->subinfo_array_;
    const MapSubInfo* const subinfo_end = subinfo_it +
                            working_container_->number_of_subinfos_;
    bool failed_world_and_subinfo_matching_root = false;
    while (subinfo_it != subinfo_end)
    {
        sMAPINFO_BASE* map_info = const_cast<sMAPINFO_BASE*>(FindMapInfo(subinfo_it->MapCode));
        if (map_info == NULL)
        {
            SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
                   _T("world.txt & worldsubinfo.txt data relation is failed. ")
                   _T("MapCode=%d can't find in world.txt|"),
                   subinfo_it->MapCode);

            ++subinfo_it;
            failed_world_and_subinfo_matching_root = true;
            continue;
        };
        const MAPCODE map_code = map_info->MapCode;
        bool failed_world_and_subinfo_matching = false;
        //
        const int number_of_max_fields = map_info->byFCount;

        if (number_of_max_fields == 0)
        {
            SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
                _T("FCount is zero. (world.txt, MapCode=%d)"),
                subinfo_it->MapCode);
        }
        int field_index = 0;
        for (; field_index < number_of_max_fields; ++field_index)
        {
            if (map_code != subinfo_it->MapCode)
            {
                SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
                       _T("|unexpected loading sequence map in worldsubinfo.txt, ")
                       _T("expected map code is %d and numbers is %d, but appeared %d at %dth|"),
                       map_code, number_of_max_fields, subinfo_it->MapCode, field_index + 1);
                // no increment subinfo_it
                failed_world_and_subinfo_matching = true;
                break;
            };

            map_info->FCode[field_index] = subinfo_it->FCode; 
            strncpy(map_info->FGCode[field_index], subinfo_it->FGCode,
                    _countof(map_info->FGCode[field_index]));
            map_info->FGCode[field_index][_countof(map_info->FGCode[field_index]) - 1] = '\0';
            map_info->FWMOCode[field_index] = subinfo_it->FWMOCode; 
            map_info->FWUTCode[field_index] = subinfo_it->FWUTCode; 
            map_info->MMCode[field_index]   = subinfo_it->MMCode;
            //
            ++subinfo_it;
        }; //end 'for'
        //
        if (field_index != number_of_max_fields)
        {
            SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
                   _T("|incompleted world field code list, MapCode=%d|"),
                   map_code);
            failed_world_and_subinfo_matching = true;
        };

        if (failed_world_and_subinfo_matching) {
            FASSERT(!"|Critical error!, check world.txt and worldsubinfo.txt");
        };
    };
    if (failed_world_and_subinfo_matching_root) {
        FASSERT(!"|Critical error!, check world.txt and worldsubinfo.txt");
    };
    // NOTE: f110520.5L
    BuildConstStaticZoneTable();
    ;{
        working_container_->Release();
        delete this->working_container_; // f110513.4L, fixed invalid releasing
        this->working_container_ = NULL;
    };

    return true;
}

bool MapInfoParser::ApplyRuleset(sMAPINFO_BASE* const map_info)
{
    BOOST_STATIC_ASSERT(INDUN_KIND_MAX == 7);   // 추가 타입 존재시 하단 내용 추가할 것

    const eZONETYPE zone_type = eZONETYPE(map_info->byMKind);
    eZONE_RULESET& zone_ruleset = map_info->Ruleset;

    switch (zone_type)
    {
    case eZONETYPE_INSTANCE:
        {
            const eINSTANCE_DUNGEON_KIND dungeon_type = eINSTANCE_DUNGEON_KIND(map_info->byMType);
            switch (dungeon_type)
            {
            case INDUN_KIND_TOP_OF_TRIAL:
#ifdef _NA_008012_20150130_RANKING_SYSTEM
                zone_ruleset.rules = eZONE_RULESET::eRules(
                    zone_ruleset.ZONE_RULE_INDUN_SHOW_THE_FLOOR |
                    zone_ruleset.ZONE_RULE_INDUN_RANKING_POINT);
#else
                zone_ruleset.rules = eZONE_RULESET::eRules(
                    zone_ruleset.ZONE_RULE_INDUN_SHOW_THE_FLOOR);
#endif // _NA_008012_20150130_RANKING_SYSTEM
                break;
            case INDUN_KIND_100LEVEL_QUEST:
                zone_ruleset.rules = eZONE_RULESET::eRules(
                    zone_ruleset.ZONE_RULE_INDUN_QUEST);
                break;
            case INDUN_KIND_SS_QUEST:
#ifdef _NA_008012_20150130_RANKING_SYSTEM
                zone_ruleset.rules = eZONE_RULESET::eRules(
                    zone_ruleset.ZONE_RULE_INDUN_RANKING_POINT |
                    zone_ruleset.ZONE_RULE_INDUN_STATIC_DUNGEON);
#else
                zone_ruleset.rules = eZONE_RULESET::eRules(
                    zone_ruleset.ZONE_RULE_INDUN_STATIC_DUNGEON);
#endif // _NA_008012_20150130_RANKING_SYSTEM
                break;
            case INDUN_KIND_EXT_03: // ex : 2008 크리스마스 눈사람 이벤트
#ifdef _RU_007007_20131223_POSSIBLE_ENTER_CHRISTMAS_MAP
                zone_ruleset.rules = eZONE_RULESET::eRules(                    
                    zone_ruleset.ZONE_RULE_INDUN_SINGLE_PLAYING_MODE);
#else
                zone_ruleset.rules = eZONE_RULESET::eRules(
                    zone_ruleset.ZONE_RULE_INDUN_CANT_EQUIP_WEAPON_AND_BLOCK |
                    zone_ruleset.ZONE_RULE_INDUN_SINGLE_PLAYING_MODE);
#endif // _RU_007007_20131223_POSSIBLE_ENTER_CHRISTMAS_MAP
                break;
            case INDUN_KIND_EXT_04: // ex : 몬트샤인 공성...
#ifdef _NA_008012_20150130_RANKING_SYSTEM
                zone_ruleset.rules = eZONE_RULESET::eRules(
                    zone_ruleset.ZONE_RULE_INDUN_RANKING_TIME_ATTACK |
                    zone_ruleset.ZONE_RULE_INDUN_RANKING_TIMER);
#else
                zone_ruleset.rules = eZONE_RULESET::eRules(
                    zone_ruleset.ZONE_RULE_INDUN_QUEST);
#endif // _NA_008012_20150130_RANKING_SYSTEM
                break;
            case INDUN_KIND_MAYA_SCROLL:
                zone_ruleset.rules = eZONE_RULESET::eRules(
                    zone_ruleset.ZONE_RULE_INDUN_SHADOW_NOT_PLAYING_MODE);
                break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            case INDUN_KIND_CHAOS_ZONE:
#ifdef _NA_008012_20150130_RANKING_SYSTEM
                zone_ruleset.rules = eZONE_RULESET::eRules(
                    zone_ruleset.ZONE_RULE_INDUN_RANKING_POINT);
#else
                zone_ruleset.rules = eZONE_RULESET::eRules(
                    zone_ruleset.ZONE_RULE_INDUN_QUEST);
#endif // _NA_008012_20150130_RANKING_SYSTEM
                break;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

            default:
                FASSERT(!"Invalid IndunSubType");
                return false;
            };
        }
        break;
    default:
        zone_ruleset.rules = zone_ruleset.ZONE_RULE_EMPTY;
        return true;
    }

    return true;
}

//==================================================================================================
// NOTE: f110520.5L, this method makes the static field unique index to support agent server and
// world server key matching like villages, fields and domination fields.
void MapInfoParser::BuildConstStaticZoneTable()
{
    // the '1' key index reserved here to support the character selectable zone.
    const KEYTYPE kConstStaticZoneStartKey = 2;
    KEYTYPE reserved_index = kConstStaticZoneStartKey;
    sMAPINFO_BASE* map_info_it = map_info_array_,
                 * const map_info_end = map_info_array_ + map_info_table_.size();
    for (; map_info_it != map_info_end; ++map_info_it)
    {
        const eZONETYPE zone_type = map_info_it->GetZoneType4SharedCode();
        bool is_char_select = (zone_type == eZONETYPE_CHARSELECT);
        if (is_char_select) {
            map_info_it->reserved_key_index_ = 1;
            continue;
        };

        bool is_static_zone = (zone_type == eZONETYPE_VILLAGE) ||
                              (zone_type == eZONETYPE_FIELD) ||
                              (zone_type == eZONETYPE_DOMINATION_FIELD) ||
                              (zone_type == eZONETYPE_SPA_FIELD);
        if (is_static_zone == false) {
            continue;
        };
        assert(reserved_index < UCHAR_MAX);
        map_info_it->reserved_key_index_ = static_cast<uint8_t>(reserved_index);
        for (int kind = map_info_it->byFCount; kind != 0; --kind) {
            ++reserved_index;
        };
    }; //end 'foreach map in maps'
    //
    this->instance_map_start_index_ = reserved_index;
    //
}

//==================================================================================================
//
WORD MapInfoParser::GetCurrentMapPortalIndex(
    const char* from_area_string, CODETYPE to_field_id) const
{
    char from_area_temp[sizeof(WzID) + 1];
    strncpy(from_area_temp, from_area_string, _countof(from_area_temp));
    from_area_temp[_countof(from_area_temp) - 1] = '\0';
    //
    const WzID from_area_id = StrToWzID(from_area_temp);
    sMAP_ENTERENCE_PORTAL* it = portal_info_array_,
                         * end = portal_info_array_ + map_portal_table_.size();
    for (; it != end; ++it)
    {
        sMAP_ENTERENCE_PORTAL* const portal_info = it;

        if (portal_info->wWorldFrom == to_field_id)
        {
            if (portal_info->m_dwFromAreaID == from_area_id) {
                return portal_info->wIndex;
            }
        }
    }

    return 0;
}

//==================================================================================================
//
WORD MapInfoParser::GetCurrentMapPortalIndexVer2(CODETYPE from_field_id, CODETYPE to_field_id) const
{
    sMAP_ENTERENCE_PORTAL* it = portal_info_array_,
                         * end = portal_info_array_ + map_portal_table_.size();
    for (; it != end; ++it)
    {
        sMAP_ENTERENCE_PORTAL* const portal_info = it;

        if ((portal_info->wFieldFrom == from_field_id) &&
            (portal_info->wFieldTo == to_field_id))
        {
            return portal_info->wIndex;
        }
    }

    return 0;
}

//==================================================================================================
//
WORD MapInfoParser::GetWayPointIndex(CODETYPE from_field_id, CODETYPE to_field_id) const
{
    const WzID to_portal_id = CharToWzID('W', 'P', '0', '1');// StrToWzID("WP01");

    sMAP_ENTERENCE_PORTAL* it = portal_info_array_,
                         * end = portal_info_array_ + map_portal_table_.size();
    for (; it != end; ++it)
    {
        sMAP_ENTERENCE_PORTAL* const portal_info = it;

        if (portal_info->m_dwToAreaID == to_portal_id)
        {
            if ((portal_info->wFieldFrom == from_field_id) &&
                (portal_info->wFieldTo == to_field_id))
            {
                return portal_info->wIndex;
            }
        }
    }

    return 0;
}

//==================================================================================================
// NOTE: client GM go command accessibility
int MapInfoParser::FindMapPortalInfo(OUT MAPCODE* map_codes, const size_t list_size,
                                     int control_position, BYTE continent) const
{
    // NOTE: f110504.2L, f110425.2L, logic rearrangement
    const sMAPINFO* it = map_info_array_,
                  * end = map_info_array_ + map_info_table_.size();
    //
    MAPCODE* map_codes_it = map_codes;
    const MAPCODE* map_codes_end = map_codes + list_size;
    //
    for (; map_codes_it != map_codes_end && it != end; ++it)
    {
        const sMAPINFO* map_info = it;
        assert(map_info != NULL);
        // field server fields
        const bool check_acceptable_field =
            (map_info->byMKind == eZONETYPE_VILLAGE) ||
            (map_info->byMKind == eZONETYPE_FIELD) ||
            (map_info->byMKind == eZONETYPE_TUTORIAL_FIELD) ||
            (map_info->byMKind == eZONETYPE_DOMINATION_FIELD) ||
            (map_info->byMKind == eZONETYPE_AIRSHIP_FIELD) ||
            (map_info->byMKind == eZONETYPE_SPA_FIELD);
        if (check_acceptable_field == false) {
            continue;
        };

    #ifdef _DH_EP2_FULL_MAP
        if (map_info->wContinentCode == control_position && map_info->ContienetNumber == continent)
    #else
        if (map_info->wContinentCode == control_position && continent == 1)
    #endif//_DH_EP2_FULL_MAP
        {
            *map_codes_it++ = map_info->MapCode;
        };
        //
    }; //end 'foreach sMAPINFO in maplist'
    //
    int number_of_found_maps = static_cast<int>(map_codes_it - map_codes);
    return number_of_found_maps;
}

// NOTE: f110509.2L, to support tutorial map load balance,
// implemented by __NA002642_TUTORIAL_LOAD_BALANCE_SUPPORT
FIELDCODE MapInfoParser::GetRandomFieldCodeInZone(MAPCODE map_code) const
{
    sMAPINFO* map_info = this->FindMapInfo(map_code);
    if (map_info == NULL) {
        assert(!"first, you must check whether map_code is valid");
        return 0;
    };

    int selected = 0;
    ;{
        int numbers = map_info->byFCount;
        if (numbers > 1) {
            selected = random(0, (numbers - 1));
        };
    };
    FIELDCODE selected_field = map_info->FCode[selected];
    return selected_field;
}