#pragma once
#ifndef PROGRAMCOMMON_MAPINFO_PARSER_H
#define PROGRAMCOMMON_MAPINFO_PARSER_H

//==================================================================================================
/// MapInfoParser class
/**
    @author
    -  Kim Min Wook < taiyo@webzen.com >
    @since
    -  2004. 11. 30
    @remarks
    - 
    @note
    - 
    @history
    - 2005.12.23 : MapGroup class 积己窍咯 包府
    - 2011.05.03, waverix, f110503.6L, logic improvements
*/
//==================================================================================================

#include "Struct.h"
#include "ScriptCode.IParser.h"

//==================================================================================================
//
struct sMAPSUBINFO;
class MapGroupBase;
typedef const MapGroupBase MapGroup;
class MapInfoParser;

//==================================================================================================
// global accessibilities
// CHANGES: f110609.7L, changes data structure to support waypoint sorted list.
typedef STLX_MAP<CODETYPE, sMAP_ENTERENCE_PORTAL*> sMAPPORTAL_HASH_BASE;
typedef const sMAPPORTAL_HASH_BASE sMAPPORTAL_HASH;
typedef STLX_HASH_MAP<CODETYPE, sMAPINFO*> sMAPINFO_HASH_BASE;
typedef const sMAPINFO_HASH_BASE sMAPINFO_HASH;
//
//==================================================================================================
// MapGroup
class MapGroupBase
{
public:
    sMAPINFO_HASH& GetMapInfoHash() const;
    sMAPINFO* GetMapInfoIndex(WORD index) const;
    sMAPINFO* GetMapInfoCode(MAPCODE map_code) const;

private:
    MapGroupBase();
    ~MapGroupBase();

    void AddMapInfoIndex(sMAPINFO* map_info, WORD index/*0/1/2*/);
    void AddMapInfoCode(sMAPINFO* map_info, MAPCODE map_code);

    sMAPINFO_HASH_BASE map_infos_by_index_;
    sMAPINFO_HASH_BASE map_infos_by_code_;
    //
    friend class MapInfoParser;
};

// MapGroupBase implements

inline sMAPINFO_HASH& MapGroupBase::GetMapInfoHash() const {
    return map_infos_by_index_;
}

//==================================================================================================
//
//
class MapInfoParser : public IParser
{
public:
    typedef STLX_HASH_MAP<CODETYPE, sFIELDINFO *> sFIELDINFO_HASH;
    typedef STLX_HASH_MAP<CODETYPE, MapGroup *> MAPGROUP_HASH;
    typedef STLX_HASH_MAP<CODETYPE, sWMOINFO *> sWMOINFO_HASH;
    typedef STLX_HASH_MAP<CODETYPE, sWUTINFO *> sWUTINFO_HASH;
    typedef std::vector<sMAPSUBINFO*> sMAPSUBINFO_VEC;

    static MapInfoParser* Instance();
    static void DestroyInstance(); // old util::Singleton support interface

    // IParser interfaces
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);
    virtual void Release();
    //
    void Init(DWORD dwPoolSize);
    void Unload();
    //
    sFIELDINFO* GetFieldInfo(DWORD field_code) const;
    MapGroup* GetMapGroup(CODETYPE map_kind) const;
    sMAP_ENTERENCE_PORTAL* GetMapPortal(CODETYPE index) const;
    // client facility
    WORD GetCurrentMapPortalIndex(const char* from_area_id, CODETYPE to_field_id) const;
    // client facility
    WORD GetCurrentMapPortalIndexVer2(CODETYPE from_field_id, CODETYPE to_field_id) const;

    sMAPPORTAL_HASH* GetMapPortalHashTable() const;
    WORD GetWayPointIndex(CODETYPE from_field_id, CODETYPE to_field_id) const;

    sMAPINFO_HASH& GetMapInfoHash() const;
    const sFIELDINFO_HASH& GetFieldInfoHash() const;

    const sWMOINFO* FindWMOInfo(FIELDCODE wmocode) const;
    const sWUTINFO* FindWUTInfo(FIELDCODE wmocode) const;
    const sMAPINFO* FindMapInfo(MAPCODE map_code) const;

    // NOTE: client GM go command accessibility
    int FindMapPortalInfo(OUT MAPCODE* map_codes, const size_t list_size,
        int control_position, BYTE continent) const;
    // NOTE: f110509.2L, to support tutorial map load balance,
    // implemented by __NA002642_TUTORIAL_LOAD_BALANCE_SUPPORT
    FIELDCODE GetRandomFieldCodeInZone(MAPCODE map_code) const;
    KEYTYPE GetInstanceMapStartIndex() const; // NOTE: f110520.5L
private:
    struct MapInfoParserWorkingContainer;
    // list = { world, worldsubinfo, field, fieldwmo, fieldwut, mapenterenceportal }
    static const int kNumberOfTotalLoaded = 6;
    //
    MapInfoParser();
    virtual ~MapInfoParser();
    static MapInfoParser* CreateInstance();
    //
    bool _LoadMapEnterencePortal();
    bool _LoadFieldWMO();
    bool _LoadFieldWUT();
    bool _LoadWorld();
    bool _LoadField(); 
    bool _LoadWorldSubInfo();

    void AddMapGroup(MapGroup* map_group, WORD map_kind);
    void AddMapInfo(sMAPINFO* map_info, MAPCODE map_code);
    void AddMapPortal(sMAP_ENTERENCE_PORTAL* portal_info, WORD index);
    void ValidWorldCheck(sMAPINFO_BASE* map_info);
    bool ApplyRuleset(sMAPINFO_BASE* const map_info);
    // NOTE: f110520.5L, make static field unique index like villages, fields and domination fields.
    void BuildConstStaticZoneTable();
    //----------------------------------------------------------------------------------------------
    // data fields...
    int number_of_loaded_;
    KEYTYPE instance_map_start_index_; // NOTE: f110520.5L
    sMAPINFO_HASH_BASE map_info_table_;
    sFIELDINFO_HASH field_info_table_;
    MAPGROUP_HASH   map_group_table_;
    sMAPPORTAL_HASH_BASE map_portal_table_;
    sWMOINFO_HASH   wmo_info_table_;
    sWUTINFO_HASH   wut_info_table_;
    //
    sMAPINFO_BASE* map_info_array_;
    sFIELDINFO_BASE* field_info_array_;
    sWMOINFO_BASE* wmo_info_array_;
    sWUTINFO_BASE* wut_info_array_;
    sMAP_ENTERENCE_PORTAL_BASE* portal_info_array_;
    //
    MapInfoParserWorkingContainer* working_container_; // temporary storage for parsing
    //
    static MapInfoParser* instance_static_;
    //
    __DISABLE_COPY(MapInfoParser);
};

//==================================================================================================
// inlining implements
//
inline MapInfoParser* MapInfoParser::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}

inline sMAPPORTAL_HASH* MapInfoParser::GetMapPortalHashTable() const {
    return &map_portal_table_;
}

inline sMAPINFO_HASH& MapInfoParser::GetMapInfoHash() const {
    return map_info_table_;
}

inline const MapInfoParser::sFIELDINFO_HASH& MapInfoParser::GetFieldInfoHash() const {
    return field_info_table_;
}

inline KEYTYPE MapInfoParser::GetInstanceMapStartIndex() const {
    return instance_map_start_index_;
};

#endif //PROGRAMCOMMON_MAPINFO_PARSER_H