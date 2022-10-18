#pragma once
//==================================================================================================
/// ZoneManager 클래스
/**
    @author    Kim Min Wook < taiyo@webzen.com >
    @since    2005. 12. 6
    @remark
        - 
    @note
        - 
    @history 
        - 
    @usage
        - 
*/
//==================================================================================================

//==================================================================================================
//  <INCLUDEs>
//
#include <ObjectKeyStream.h>
#define WAVERIX_ZONE_ASSERT        ASSERT    // for DEBUGGING
#define DEBUG_WRAP_PREFIX(name)    __TMP__##name
// 2008.10.29 / 이은파 / 마을,필드 WorldServer와 키 맞추기 위한 테이블 설정
#define __NA000000_081029_STATIC_ZONE_MATCHING_TABLE__

#include "ZoneCommon.h"

struct MSG_AG_ZONE_CAN_CREATE_ROOM_ACK;
struct MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK;

//==================================================================================================
//  <DEFINITIONs>
//
struct ZoneKeyStreamAddin
{
    WORD    ZoneType;       // eZONETYPE
    WORD    ZoneSubType;    // eINSTANCE_DUNGEON_KIND
    ZoneKeyStreamAddin()
        : ZoneType(eZONETYPE_MAX), ZoneSubType(INDUN_KIND_MAX)
    {}
    ZoneKeyStreamAddin(ZoneInterface* const pZone);
    ZoneKeyStreamAddin(const ZoneKeyStreamAddin& rSrc)
        : ZoneType(rSrc.ZoneType), ZoneSubType(rSrc.ZoneSubType)
    {}

    ZoneKeyStreamAddin& operator=(const ZoneKeyStreamAddin& rSrc)
    {
        ZoneType    = rSrc.ZoneType;
        ZoneSubType = rSrc.ZoneSubType;
        return *this;
    }
};

//==================================================================================================
//  <ZoneManager>
//
class ZoneManager
{
private:
    friend class ZoneInterface;

    typedef ObjectKeyStream<ZoneInterface,
                            nsKeyInfo::ZONE_KEY_START, nsKeyInfo::ZONE_KEY_LAST,
                            ZoneKeyStreamAddin> ObjectKeyStream;
public:
    ZoneManager(void);
    ~ZoneManager(void);

    //----------------------------------------------------------------------------------------------
    //  로직에 의해 만들어진 메서드
    BOOL    AutoMatching(KEYTYPE lobbyKey, User* pMasterUser, CHuntingRoom*& OUT pCHuntingRoom);

    //----------------------------------------------------------------------------------------------
    //  FieldServer와 접속시 한번만 수행하는 계열
    //  - CreateCharSelect, CreateVillage, CreateHuntingField
    VOID    CreateCharSelect();
#ifdef __NA000000_081029_STATIC_ZONE_MATCHING_TABLE__
    VOID    CreateConstStaticZoneTable();    // post script loaded, make key-mapcode table for sync worldserver
#endif
    RC::eROOM_RESULT CreateVillage(MAPCODE mapCode, ZoneInterface* OUT pVillage);
    RC::eROOM_RESULT CreateHuntingField(MAPCODE mapCode, ZoneInterface* OUT pHuntingField);
    RC::eROOM_RESULT CreateDominationField(MAPCODE mapCode, ZoneInterface* OUT pDominationField);
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    RC::eROOM_RESULT CreateSpaField(MAPCODE mapCode, ZoneInterface* OUT spa_field);
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    //----------------------------------------------------------------------------------------------
    // _PCBANG_POLICY_CHANGE_POST_EXPIRED
    void CheckRuleForRoom(User* creator, const BASE_ROOMINFO& base, BASE_ROOMINFO* OUT modified);
    //----------------------------------------------------------------------------------------------
    //  요청시마다 수행되는 계열
    RC::eROOM_RESULT CreateLobbyRoom(KEYTYPE staticZoneKey, User* pMasterUser,
                                     MAPCODE mapCode, eZONETYPE eLobbyType, eZONEPUBLIC ePublic,
                                     TCHAR* pszRoomTitle, TCHAR* pszRoomPWD,
                                     BASE_ROOMINFO& IN roomInfo,
                                     BYTE byMaxLimitUserNum, KEYTYPE linkHuntingKey,
                                     LobbyRoom*& OUT pLobbyRoom);
    // (WAVERIX) (090624) (CHANGES) Remove unused section 'Hunting'
    //RC::eROOM_RESULT CreateHuntingRoom(KEYTYPE lobbyKey, User* pMasterUser,
    //                                   HuntingRoom*& OUT pHuntingRoom);
    RC::eROOM_RESULT CreateMissionRoom(KEYTYPE lobbyKey, User* pMasterUser,
                                       MissionRoom*& OUT pMissionRoom);
    RC::eROOM_RESULT CreatePVPRoom(KEYTYPE lobbyKey, User* pMasterUser, PVPRoom*& OUT pPVPRoom);
    RC::eROOM_RESULT CreateCHuntingRoom(KEYTYPE lobbyKey, User* pMasterUser,
                                        CHuntingRoom*& OUT pCHuntingRoom);
    RC::eROOM_RESULT MigrateCHuntingRoom(User* pMasterUser, KEYTYPE lobbyKey,
                                         CHuntingRoom* IN pCHuntingRoom);
    // (WAVERIX) (090624) (CHANGES) Remove unused section 'Hunting'
    //RC::eROOM_RESULT CreateRelayHuntingRoom(KEYTYPE prevHuntingKey, User* pMasterUser,
    //                                        MAPCODE mapCode, eZONEPUBLIC ePublic, TCHAR* pszRoomPWD,
    //                                        BASE_ROOMINFO& IN roomInfo,
    //                                        ADDITIONAL_ROOMINFO& IN additionalRoomInfo,
    //                                        HuntingRoom*& OUT pHuntingRoom);
    RC::eROOM_RESULT CreateRelayMissionLobbyRoom(MissionRoom* IN pMissionRoom, User* pMasterUser,
                                                 LobbyRoom*& OUT pLobbyRoom);

    //    (WAVERIX) (WARNING) 특수경우 제외하고 eKind=INDUN_KIND_TOP_OF_TRIAL를 설정할 것
    //    - sub_type by script 에 의한 단일 객체(추후 InstanceDungeonEx)의 rule set만 변경하는 구조로 전환
    RC::eROOM_RESULT CreateInstanceDungeon(KEYTYPE zoneKey, User* pMasterUser, MAPCODE mapCode,
                                           InstanceDungeon*& pInstanceDungeon,
                                           eINSTANCE_DUNGEON_KIND eKind, BOOL UseChargeItem = FALSE);
    RC::eROOM_RESULT CreateStaticDungeon(const MAPCODE mapCode,
                                         InstanceDungeon*& OUT rpStaticDungeon,
                                         const eINSTANCE_DUNGEON_KIND kindOfDungeon,
                                         const BOOL bUseChargeItem = FALSE); // current, not used
    //{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
    RC::eROOM_RESULT CreatePortal(const QuestPortalInfo& rQuestPortalInfo, User* pUser,
                                  BOOLEAN isTempPortal);
    RC::eROOM_RESULT ClosePortal(const QuestPortalInfo& rQuestPortalInfo, KEYTYPE roomKey);
    // call by PortalManager::EnterPortalBy~
    RC::eROOM_RESULT CreateRoomByPortal(PORTALKEY portalKey, User* pFirstEnterUser,
                                        const RoomArgumentForRoomSetup& rRoomArgument);
    VOID             CreateMissionByLobby(User* request_user, const MSG_AG_ZONE_CAN_CREATE_ROOM_ACK* msg);
    // call by PortalManager::EnterPortalBy~
    RC::eROOM_RESULT EnterPortal(PORTALKEY portalKey, User* pUser,
                                 const RoomArgumentForRoomSetup& rRoomArgument,
                                 KEYTYPE enterableRoomKey);
    //}
    // (ADD)(20100318)(hukim)(REQ_0038) IsMoveableZone()
    // 입장 가능 여부를 검사하는 로직이 중복되어 흩어지는 것을 막기 위해 별도의 메서드로 분리

    RC::eROOM_RESULT IsMoveableZone(CheckMoveZonePolicy* const check_move_zone_policy, 
                                    const RoomArgument& room_argument) const;
    RC::eROOM_RESULT MoveZone(CheckMoveZonePolicy* const pPolicy, const RoomArgument& rRoomArg);

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    VOID             ProcessCreateMissionRoom(User* request_user);
    VOID             ProcessCreateInstanceDungeonRoom(User* request_user);
    VOID             ProcessCreatePVPRoom(User* request_user);

    RC::eROOM_RESULT CreateMissionRoomByParty(User* master_user, 
                                              MissionRoom*& OUT mission_room, 
                                              MAPCODE map_code,
                                              PartyZoneType party_room_type);

    RC::eROOM_RESULT CreateInstanceDungeonRoomByParty(User* master_user, 
                                                 InstanceDungeon*& OUT Instance_dungeon, 
                                                 MAPCODE mapCode,
                                                 PartyZoneType party_room_type,
                                                 eINSTANCE_DUNGEON_KIND dungeon_kind, 
                                                 BOOL need_check_open_time);

    RC::eROOM_RESULT CreatePVPRoomByParty(User* master_user, 
                                          PVPRoom*& OUT pvp_room, 
                                          MAPCODE map_code,
                                          PartyZoneType party_room_type,
                                          const ADDITIONAL_PVPINFO* additional_pvp_info);

protected:
    RC::eROOM_RESULT MoveZone(ZoneInterface* const pFromZone, ZoneInterface* const pToZone,
                              User* const pUser, const RoomArgument& rRoomArg);

public:
    //----------------------------------------------------------------------------------------------
    //  로비에서 설정가능 한 부분
    RC::eROOM_RESULT KickUserFromLobby(User* pMaster, DWORD targetPlayerKey, User*& OUT pTargetUser);
    RC::eROOM_RESULT UserReadyInLobby(User* pUser, BOOL bReady);
    RC::eROOM_RESULT ChangeTeamInLobby(User* pUser, BYTE team);
    RC::eROOM_RESULT ChangeMapInLobby(User* pMaster, MAPCODE mapCode);
    RC::eROOM_RESULT ConfigHuntingRoomInLobby(User* pMaster, ADDITIONAL_ROOMINFO& IN additionalInfo);
    RC::eROOM_RESULT ConfigPVPRoomInLobby(User* pMaster, ADDITIONAL_PVPINFO& IN additionalPVPInfo);
    RC::eROOM_RESULT ConfigCHuntingRoomInLobby(User* pMaster, ADDITIONAL_CHUNTINGINFO& IN additionalInfo);
    RC::eROOM_RESULT SetNonblockSlotNumInLobby(User* pMaster, BYTE MaxUser);


    VOID GetMissionLobbyRoomList(User* pUser, BYTE reqCount, BYTE maxCount, BYTE& OUT count,
                                 MISSION_ROOMINFO* OUT pRoomInfoArray);
    VOID GetHuntingLobbyRoomList(User* pUser, BYTE reqCount, BYTE maxCount, BYTE& OUT count,
                                 HUNTING_ROOMINFO* OUT pRoomInfoArray);
    VOID GetPVPLobbyRoomList(User* pUser, BYTE reqCount, BYTE maxCount, BYTE& OUT count,
                             PVP_ROOMINFO* OUT pRoomInfoArray);
    VOID GetCHuntingLobbyRoomList(User* pUser, BYTE reqCount, BYTE maxCount, BYTE& OUT count,
                                  CHUNTING_ROOMINFO* OUT pRoomInfoArray);
    VOID GetCHuntingRoomList(User* pUser, BYTE reqCount, BYTE maxCount, BYTE& OUT count,
                             CHUNTING_ROOMINFO* OUT pRoomInfoArray);
    VOID GetHuntingRoomList(User* pUser, BYTE reqCount, BYTE maxCount, BYTE& OUT count,
                            HUNTING_ROOMINFO* OUT pRoomInfoArray);
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    VOID GetGuildPVPRoomList(User* pUser, BYTE reqCount, BYTE maxCount, BYTE& OUT count,
                             CHUNTING_ROOMINFO* OUT pRoomInfoArray);
#endif

    RoomInterface* FindRecommendRoom(User* pUser, eZONETYPE roomType);
    //----------------------------------------------------------------------------------------------
    //
    VOID    Process();
    VOID    Display(BOOL bLog = FALSE);
    VOID    DisplayerZonePoolInfo();
    VOID    DestroyZone(ZoneInterface* pZone);
    //----------------------------------------------------------------------------------------------
    //
    void    ChangeBattlePlayersCount(RoomInterface* pRoom, DWORD number_of_players);
    //----------------------------------------------------------------------------------------------
    //
    DWORD   GetNumberOfHuntingRooms();
    DWORD   GetNumberOfMissionRooms();

    //----------------------------------------------------------------------------------------------
    //
    //  특정 Room을 위한 목록
    enum eLOBBY_HASH_IDX {
        LHI_HUNTING ,
        LHI_MISSION ,
        LHI_PVP     ,
        LHI_CHUNTING,
        LHI_SIZE_MAX,
    };

    inline ZoneInterface* FindZone(const KEYTYPE key) const;
    ZoneInterface*  FindStaticZone(const MAPCODE mapCode) const;
    Village*        FindVillage(const MAPCODE mapCode) const;
    HuntingField*   FindHuntingField(const MAPCODE mapCode) const;
    DominationField* FindDominationField(const MAPCODE mapCode) const;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    SpaField*       FindSpaField(const MAPCODE map_code) const;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    InstanceDungeon* FindStaticDungeon(const MAPCODE mapCode) const;

    CharSelect*     FindCharSelect(const KEYTYPE key /*for fault trace*/) const;
    //    개별 테이블에서 쿼리하는 Interface
private:
    ZoneInterface*  _FindRoomBySeperated(const eZONETYPE zoneType, const KEYTYPE key) const;
    LobbyRoom*      _FindLobbyBySeperated(const eZONETYPE zoneTypeOfLobby, const KEYTYPE key) const;
public:
    //  BattleServer Room
    inline LobbyRoom*   FindLobby(const KEYTYPE key) const;
    inline HuntingRoom* FindHunting(const KEYTYPE key) const;
    inline MissionRoom* FindMission(const KEYTYPE key) const;
    inline PVPRoom*     FindPVP(const KEYTYPE key) const;
    inline CHuntingRoom* FindCHunting(const KEYTYPE key) const;
    //  Lobby for BattleServer Room
    inline LobbyRoom* FindHuntingLobby(const KEYTYPE key) const;
    inline LobbyRoom* FindMissionLobby(const KEYTYPE key) const;
    inline LobbyRoom* FindPVPLobby(const KEYTYPE key) const;
    inline LobbyRoom* FindCHuntingLobby(const KEYTYPE key) const;
    LobbyRoom* FindCHuntingLobbyByCkey(const KEYTYPE key) const;
    //inline InstanceDungeon* FindInstanceDungeon(const MAPCODE mapCode){ return (InstanceDungeon*)_FindZoneBySeperated(eZONETYPE_INSTANCE, mapCode); }
    //  (WAVERIX) (080709) 인터페이스가 지독하게 많지만 이걸 사용하는 코드 수정하는 것보다야 뭐...
    //  - 개인적인 의견... 이런 인터페이스들이 많은 것이 오히려 하드코딩이라 생각되요. 어떻게 생각하시는지?
    inline DWORD GetTotalZoneCount() const;
    inline DWORD GetCharSelectCount() const;
    inline DWORD GetVillageCount() const;
    inline DWORD GetFieldServerZoneCount() const;
    inline DWORD GetBattleServerZoneCount() const;
    inline DWORD GetMissionRoomCount() const;
    inline DWORD GetCHuntingRoomCount() const;
    inline VOID  AddZone(const KEYTYPE zone_key);
    inline VOID  RemoveZone(const KEYTYPE zone_key);

protected:
    inline VOID IncreaseUserCount(const DWORD idx);
    inline VOID DecreaseUserCount(const DWORD idx);

    inline VOID IncVillageUserCount()          { IncreaseUserCount(eZONETYPE_VILLAGE);  }
    inline VOID IncHuntingLobbyRoomUserCount() { IncreaseUserCount(eZONETYPE_LOBBY);    }
    inline VOID IncMissionLobbyRoomUserCount() { IncreaseUserCount(eZONETYPE_LOBBY);    }
    inline VOID IncPVPLobbyRoomUserCount()     { IncreaseUserCount(eZONETYPE_LOBBY);    }
    inline VOID IncCHuntingLobbyRoomUserCount(){ IncreaseUserCount(eZONETYPE_LOBBY);    }

    inline VOID IncHuntingRoomUserCount()      { IncreaseUserCount(eZONETYPE_HUNTING);  }
    inline VOID IncMissionRoomUserCount()      { IncreaseUserCount(eZONETYPE_MISSION);  }
    inline VOID IncPVPRoomUserCount()          { IncreaseUserCount(eZONETYPE_PVP);      }
    inline VOID IncCHuntionRoomUserCount()     { IncreaseUserCount(eZONETYPE_CHUNTING); }
    inline VOID IncHuntionFieldRoomUserCount() { IncreaseUserCount(eZONETYPE_FIELD);    }
    inline VOID IncInstanceDungeonUserCount()  { IncreaseUserCount(eZONETYPE_INSTANCE); }

    inline VOID DecVillageUserCount()          { DecreaseUserCount(eZONETYPE_VILLAGE);  }
    inline VOID DecHuntingLobbyRoomUserCount() { DecreaseUserCount(eZONETYPE_LOBBY);    }
    inline VOID DecMissionLobbyRoomUserCount() { DecreaseUserCount(eZONETYPE_LOBBY);    }
    inline VOID DecPVPLobbyRoomUserCount()     { DecreaseUserCount(eZONETYPE_LOBBY);    }
    inline VOID DecCHuntingLobbyRoomUserCount(){ DecreaseUserCount(eZONETYPE_LOBBY);    }

    inline VOID DecHuntingRoomUserCount()      { DecreaseUserCount(eZONETYPE_HUNTING);  }
    inline VOID DecMissionRoomUserCount()      { DecreaseUserCount(eZONETYPE_MISSION);  }
    inline VOID DecPVPRoomUserCount()          { DecreaseUserCount(eZONETYPE_PVP);      }
    inline VOID DecCHuntingRoomUserCount()     { DecreaseUserCount(eZONETYPE_CHUNTING); }
    inline VOID DecHuntionFieldRoomUserCount() { DecreaseUserCount(eZONETYPE_FIELD);    }
    inline VOID DecInstanceDungeonUserCount()  { DecreaseUserCount(eZONETYPE_INSTANCE); }

    inline VOID IncDominationFieldUserCount()  { IncreaseUserCount(eZONETYPE_DOMINATION_FIELD); }
    inline VOID DecDominationFieldUserCount()  { DecreaseUserCount(eZONETYPE_DOMINATION_FIELD); }

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    inline VOID IncSpaFieldUserCount()          { IncreaseUserCount(eZONETYPE_SPA_FIELD); }
    inline VOID DecSpaFieldUserCount()          { DecreaseUserCount(eZONETYPE_SPA_FIELD); }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

private:
    inline KEYTYPE  AllocKey()  { return (KEYTYPE)ObjKeyGender::Instance()->GetKey(OBJKEYTYPE_ZONEKEY); }
    inline VOID     FreeKey(KEYTYPE Key) { ObjKeyGender::Instance()->RestoreKey(OBJKEYTYPE_ZONEKEY, Key); }
    VOID    AddRoom(ZoneInterface* const pRoom);
    VOID    RemoveRoom(ZoneInterface* const pRoom);
    //----------------------------------------------------------------------------------------------
    typedef STLX_SET< KEYTYPE/*Zone Key*/ >                 ZONE_LIST;
    typedef STLX_HASH_MAP< MAPCODE/*StaticZone*/, KEYTYPE > ZONE_MCODE_TABLE;    // for static-zone (Village+HuntingField) mapping table
    typedef STLX_VECTOR< KEYTYPE >                          CACHE_LOBBY_LIST;

    inline ZoneInterface* _AllocZone(const eZONETYPE zoneType) { return (ZoneInterface*)m_PoolRouter.Alloc(zoneType); }
    inline ZoneInterface* _AllocIndun(const eINSTANCE_DUNGEON_KIND zoneType) { return (ZoneInterface*)m_PoolRouterForIndun.Alloc(zoneType); }
    VOID    _FreeZone(ZoneInterface* IN pZone);
    VOID    _FreeIndun(ZoneInterface* IN pZone);
    ServerSessionEx* _CheckRoomCountLimit(FlowControl& flow);  // 방 개수 제한과 같은 LinkServer Check
    VOID    _LobbyRoomListRefresh();
    //----------------------------------------------------------------------------------------------
private:
    ObjectKeyStream*    m_pObjectKeyStream;
    util::PoolRouter<eZONETYPE_MAX>   m_PoolRouter;
    util::PoolRouter<INDUN_KIND_MAX>  m_PoolRouterForIndun;
    ITimerBase          m_UpdateTimer;
    ZONE_LIST           m_TotalZones;   // Total Zone
    ZONE_MCODE_TABLE    m_StaticZones;  // Village + HuntingField
    ZONE_MCODE_TABLE    m_StaticDungeons;                    // SSQ + ...
    ZONE_LIST   m_CHuntingRooms;    // special for competition hunting
    STLX_MAP< KEYTYPE, KEYTYPE > m_CHuntingLobbyByCKey; // irregular <- 개선안 강구할 것
    ZONE_LIST   m_LobbyRooms;       // All Lobby Links
    CACHE_LOBBY_LIST    m_LobbyRoomPerType[LHI_SIZE_MAX];  // LobbyList Serialize Refresh Update by timer (1sec)
    DWORD   m_ZoneUserCountTotal;
    DWORD   m_ZoneCounts[eZONETYPE_MAX];
    DWORD   m_ZoneUserCounts[eZONETYPE_MAX];
    friend struct SerializeLobbyRoomList;
};

//==================================================================================================
//
extern ZoneManager g_ZoneManager;
//
//==================================================================================================

//==================================================================================================
//

inline VOID
ZoneManager::AddZone(const KEYTYPE zone_key)
{
    m_TotalZones.insert(zone_key);
#ifdef _JP_0_20101123_PMS
    PMSManager* pms_manager = AgentServer::GetInstance()->GetPMSManager();
    pms_manager->SetRoomCount(GetTotalZoneCount());
#endif
}

inline VOID
ZoneManager::RemoveZone(const KEYTYPE zone_key)
{
    m_TotalZones.erase(zone_key);
#ifdef _JP_0_20101123_PMS
    PMSManager* pms_manager = AgentServer::GetInstance()->GetPMSManager();
    pms_manager->SetRoomCount(GetTotalZoneCount());
#endif
}

inline ZoneInterface*
ZoneManager::FindZone(const KEYTYPE key) const
{
    const ObjectKeyStream::sNode* pNode = m_pObjectKeyStream->GetObjectInfo(key);
    return (pNode) ? pNode->pObject : 0;
}

//  BattleServer Room
inline LobbyRoom*
ZoneManager::FindLobby(const KEYTYPE key) const {
    return (LobbyRoom*)_FindRoomBySeperated(eZONETYPE_LOBBY, key);
}

inline HuntingRoom*
ZoneManager::FindHunting(const KEYTYPE key) const {
    return (HuntingRoom*)_FindRoomBySeperated(eZONETYPE_HUNTING, key);
}

inline MissionRoom*
ZoneManager::FindMission(const KEYTYPE key) const {
    return (MissionRoom*)_FindRoomBySeperated(eZONETYPE_MISSION, key);
}

inline PVPRoom*
ZoneManager::FindPVP(const KEYTYPE key) const {
    return (PVPRoom*)_FindRoomBySeperated(eZONETYPE_PVP, key);
}

inline CHuntingRoom*
ZoneManager::FindCHunting(const KEYTYPE key) const {
    return (CHuntingRoom*)_FindRoomBySeperated(eZONETYPE_CHUNTING, key);
}

//  Lobby for BattleServer Room
inline LobbyRoom*
ZoneManager::FindHuntingLobby(const KEYTYPE key) const {
    return (LobbyRoom*)_FindLobbyBySeperated(eZONETYPE_HUNTING, key);
}

inline LobbyRoom*
ZoneManager::FindMissionLobby(const KEYTYPE key) const {
    return (LobbyRoom*)_FindLobbyBySeperated(eZONETYPE_MISSION, key);
}

inline LobbyRoom*
ZoneManager::FindPVPLobby(const KEYTYPE key) const {
    return (LobbyRoom*)_FindLobbyBySeperated(eZONETYPE_PVP, key);
}

inline LobbyRoom*
ZoneManager::FindCHuntingLobby(const KEYTYPE key) const {
    return (LobbyRoom*)_FindLobbyBySeperated(eZONETYPE_CHUNTING, key);
}

//inline    InstanceDungeon*    FindInstanceDungeon(const MAPCODE mapCode){ return (InstanceDungeon*)_FindZoneBySeperated(eZONETYPE_INSTANCE, mapCode); }
//  (WAVERIX) (080709) 인터페이스가 지독하게 많지만 이걸 사용하는 코드 수정하는 것보다야 뭐...
//  - 개인적인 의견... 이런 인터페이스들이 많은 것이 오히려 하드코딩이라 생각되요. 어떻게 생각하시는지?
inline DWORD
ZoneManager::GetTotalZoneCount() const {
    return (DWORD)m_TotalZones.size();
}

inline DWORD
ZoneManager::GetCharSelectCount() const {
    return m_ZoneCounts[ eZONETYPE_CHARSELECT ];
}

inline DWORD
ZoneManager::GetVillageCount() const {
    return m_ZoneCounts[ eZONETYPE_VILLAGE    ];
}

inline DWORD
ZoneManager::GetFieldServerZoneCount() const {
    return m_ZoneCounts[ eZONETYPE_VILLAGE    ] +
           m_ZoneCounts[ eZONETYPE_FIELD ] +
           m_ZoneCounts[ eZONETYPE_DOMINATION_FIELD ] +
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
           m_ZoneCounts[ eZONETYPE_SPA_FIELD] +
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
           m_ZoneUserCounts[ eZONETYPE_CHARSELECT ];
}

inline DWORD
ZoneManager::GetBattleServerZoneCount() const {
    return (DWORD)m_TotalZones.size() - GetFieldServerZoneCount();
}

//inline DWORD GetMissionLobbyRoomCount() const {
//return (DWORD)m_ZoneTableList[ eZONETYPE_SHI_MISSION_LOBBY ].size();
//}

inline DWORD
ZoneManager::GetMissionRoomCount() const {
    return m_ZoneCounts[ eZONETYPE_MISSION    ];
}

//inline DWORD GetCHuntingLobbyRoomCount() const {
//return (DWORD)m_ZoneTableList[ eZONETYPE_SHI_CHUNTING_LOBBY ].size();
//}

inline DWORD
ZoneManager::GetCHuntingRoomCount() const {
    return m_ZoneCounts[ eZONETYPE_CHUNTING   ];
}

inline VOID
ZoneManager::IncreaseUserCount(const DWORD idx) {
    if(eZONETYPE_MAX > idx)
        ++m_ZoneUserCounts[idx];
}

inline VOID
ZoneManager::DecreaseUserCount(const DWORD idx) {
    if(eZONETYPE_MAX > idx)
        --m_ZoneUserCounts[idx];
}

