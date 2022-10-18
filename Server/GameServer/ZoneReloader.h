#ifndef SUN_GAMESERVER_ZONERELOADER_H_
#define SUN_GAMESERVER_ZONERELOADER_H_


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _NA_0_20100416_RELOAD_MAPOBJECT

class ZoneReloader
{
public:
    ZoneReloader();
    ~ZoneReloader();

    void Reload();

private:
    struct Location
    {
        GameZone*   zone;
        Player*     player;
        MAPCODE     field_code;
        WzID        area_id;
        WzVector    position;
    };

    typedef STLX_HASH_MAP<PLAYERKEY, Location> Location_Hash;
    Location_Hash location_hash_;

    void LeavePlayer(GameField* field);
    void LeaveNPC(GameField* field);
    void LeaveMapObject(GameField* field);
    void LeaveCollection(GameField* field);
    void LeaveItem(GameField* field);
};


#endif //_NA_0_20100416_RELOAD_MAPOBJECT

#endif //SUN_GAMESERVER_ZONERELOADER_H_
