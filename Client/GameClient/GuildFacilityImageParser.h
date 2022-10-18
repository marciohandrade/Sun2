#pragma once
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

struct BASE_GuildFacilityImageInfo
{
    DWORD index_;
    int facility_order_;
    int facility_name_code_;
    int facility_explanation_code_;
    int x_;
    int y_;
    int width_;
    int height_;
};

typedef util::SolarHashTable<BASE_GuildFacilityImageInfo*> HASH_TABLE_GUILD_FACILITY_IMAGE_INFO;
typedef HASH_TABLE_GUILD_FACILITY_IMAGE_INFO::iterator HASH_TABLE_GUILD_FACILITY_IMAGE_ITR;
class GuildFacilityImageParser : public Singleton<GuildFacilityImageParser>, public IParser
{
public:
    GuildFacilityImageParser(void);
    ~GuildFacilityImageParser(void);

    VOID                                    Init(DWORD pool_size);
    VOID                                    Init(DWORD pool_size, char* pack_file_name);
    VOID                                    Release();
    virtual BOOL                            LoadScript(eSCRIPT_CODE script_code, BOOL bReload);

    BASE_GuildFacilityImageInfo*            GetGuildFacilityImageInfo(DWORD _key_order);
    HASH_TABLE_GUILD_FACILITY_IMAGE_INFO*   GetHashTable() { return data_table_; }

private:
    VOID                                     Unload();
    HASH_TABLE_GUILD_FACILITY_IMAGE_INFO*    data_table_;
};
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
