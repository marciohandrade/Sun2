#include "InstanceDungeon.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
class DungeonChaosZone : public InstanceDungeon
{
public:
    DungeonChaosZone() : InstanceDungeon(INDUN_KIND_CHAOS_ZONE)
#ifdef _NA_008334_20150608_SONNENSCHEIN
        , chaoszone_type_(kChaosZoneTypeNone)
#endif //_NA_008334_20150608_SONNENSCHEIN
    {
    }
    virtual ~DungeonChaosZone()
    {
    }
    
    virtual RC::eROOM_RESULT CanCreate(User* user, MAPCODE map_code, eZONETYPE zone_type,
                                              eZONEPUBLIC zone_public, BASE_ROOMINFO& IN base_room_info,
                                              BOOL check_open_time = TRUE);
    virtual RC::eROOM_RESULT Create(User* user, MAPCODE map_code, KEYTYPE instance_dungeon_key,
        ServerSession* server_session, BOOL use_charge_item = FALSE);

    virtual void SendJoinNAKPacket(User* user);
    virtual void SendJoinACKPacket(User* user);

#ifdef _NA_008334_20150608_SONNENSCHEIN
    virtual VOID On_leaveuser(User* user);
    virtual VOID On_destroy();

    VOID SetChaosZoneType(const ChaosZoneType& zone_type) { chaoszone_type_ = zone_type;}
    const ChaosZoneType GetChaosZoneType() { return chaoszone_type_; }

#endif //_NA_008334_20150608_SONNENSCHEIN

private:
    virtual RC::eROOM_RESULT SetBaseRoom(User* user, BASE_ROOMINFO& base_room_info, MAPCODE map_code);

#ifdef _NA_008334_20150608_SONNENSCHEIN
    ChaosZoneType chaoszone_type_;
#endif //_NA_008334_20150608_SONNENSCHEIN
};
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND