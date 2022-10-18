#pragma once

//==================================================================================================
/// InstanceDungeon 클래스
    /**
    @author    Kim Chang Hyun < shogen@webzen.com >
    @since    2007. 01. 25
    @remark
    - 인던에 대한 존 정보 관리
    @note
    - 
    @history 
    - 2008/03/07    : 다중 타입들 관리를 위한 통합 정책 적용
    @usage
    - 
    */
//==================================================================================================

#include "RoomInterface.h"


//==================================================================================================
//  <InstanceDungeon>

class InstanceDungeon : public RoomInterface
{
public:
    InstanceDungeon(const eINSTANCE_DUNGEON_KIND kindOfDungeon) : m_KindOfDungeon(kindOfDungeon) {}
    virtual ~InstanceDungeon(void);
    
    inline virtual eZONETYPE GetType() { return eZONETYPE_INSTANCE; }
    inline eINSTANCE_DUNGEON_KIND   GetDungeonType() const { return m_KindOfDungeon; }
    BOOLEAN RegisterRuleset(const MAPCODE map_code);
    inline eINSTANCE_DUNGEON_KIND   GetDungeonTypeVariation() const { return m_KindOfDungeonVariation; }

    virtual VOID SendJoinNAKPacket(User* pUser) = 0;
    virtual VOID SendJoinACKPacket(User* pUser) = 0;    

    virtual RC::eROOM_RESULT Create(User* pUser, MAPCODE mapCode, KEYTYPE instanceDungeonKey,
                                    ServerSession* pServerSession, BOOL useChargeItem = FALSE);
protected:
    virtual VOID On_create();
    virtual VOID On_destroy();
    virtual RC::eROOM_RESULT CanCreate(User* pUser, MAPCODE mapCode,
                                       eZONETYPE eZoneType, eZONEPUBLIC ePublic,
                                       BASE_ROOMINFO& IN baseRoomInfo, BOOL checkOpenTime = TRUE);
    virtual RC::eROOM_RESULT SetBaseRoom(User* pUser, BASE_ROOMINFO& baseRoomInfo, MAPCODE mapCode);
    virtual VOID On_readyuser(User* pUser);
    virtual VOID On_enteruser(User* pUser);
    virtual VOID On_leaveuser(User* pUser);

    RC::eROOM_RESULT CheckPartyConstraint(User* const pUser, const sMAPINFO* pInfo, Party*& OUT rpParty);
    virtual RC::eROOM_RESULT _CanJoinEach(User* pUser, eZONEPUBLIC RoomPublic, TCHAR* pszRoomPWD);

    const eINSTANCE_DUNGEON_KIND    m_KindOfDungeon;
    eINSTANCE_DUNGEON_KIND          m_KindOfDungeonVariation;
public:
    const eZONE_RULESET             Ruleset;
    __DISABLE_COPY(InstanceDungeon);
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
public:
    inline void Link2ndParty(const WORD party_key) { link_2nd_party_key_ = party_key; }
    inline const WORD GetLink2ndParty() const { return link_2nd_party_key_; }
private:
    WORD link_2nd_party_key_;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
};


//==================================================================================================




//==================================================================================================
//  <Milestone100LevelQuest>
class Milestone100LevelQuest : public InstanceDungeon
{
public:
    Milestone100LevelQuest();
    virtual ~Milestone100LevelQuest();
    virtual eINSTANCE_DUNGEON_KIND GetDungeonType() { return INDUN_KIND_100LEVEL_QUEST; }

    virtual RC::eROOM_RESULT CanCreate(User* pUser, MAPCODE mapCode,
                                       eZONETYPE eZoneType, eZONEPUBLIC ePublic,
                                       BASE_ROOMINFO& IN baseRoomInfo, BOOL checkOpenTime = TRUE);

    virtual VOID SendJoinNAKPacket(User* pUser);
    virtual VOID SendJoinACKPacket(User* pUser);
    __DISABLE_COPY(Milestone100LevelQuest);
};
