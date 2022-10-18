#include "stdafx.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������
#include "ZoneManager.h"

#include <PacketStruct_AG.h>
#include <Zone/PortalManager.h>
#include <UserSessions/UserManager.h>

//_NA_20110630_BATTLE_ZONE_RENEWAL
#include "..\AgentParty\PartyRoomList.h"
#include "..\Party\PartyRoom.h"

ZoneManager g_ZoneManager;

//��������������������������������������������������������������������������������������������������������������������������������������������������
//    <Imlementation for Control Fuctions>
//

#include "ZoneManager.Addin.hxx"

#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
#include "BattlezoneMissionTreeParser.h"
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
//��������������������������������������������������������������������������������������������������������������������������������������������������
//    <Implementation>
//
ZoneManager::ZoneManager(void)
{
    // Zone�� Ű���� WORD�� �Ѿ�� GameZone�� m_Key ���� DWORD�� �ٲ�����ϰ�, 
    //m_ZoneKeyGenerater.Create(1, 10000);
    m_pObjectKeyStream = new ObjectKeyStream;
    m_UpdateTimer.SetTimer(1*1000);
    m_ZoneUserCountTotal = 0;
    ZeroMemory(&m_ZoneCounts, sizeof(m_ZoneCounts));
    ZeroMemory(&m_ZoneUserCounts, sizeof(m_ZoneUserCounts));


    #undef REG_ZONE_NODE
    #define REG_ZONE_NODE(_typeinfo)    \
        m_PoolRouter.Register<_typeinfo::type>(_typeinfo::value);

    REG_ZONE_LIST();

    #undef REG_ZONE_NODE
    #define REG_ZONE_NODE(_typeinfo)    \
        m_PoolRouter.Initialize(_typeinfo::value, _typeinfo::size_init_pool, _typeinfo::size_addin_pool, #_typeinfo, AssertionLogger4Pool_Server);

    REG_ZONE_LIST();

    #undef REG_ZONE_NODE
    #define REG_ZONE_NODE(_typeinfo)    \
        m_PoolRouterForIndun.Register<_typeinfo::type>(_typeinfo::value);

    REG_INDUN_LIST();

    #undef REG_ZONE_NODE
    #define REG_ZONE_NODE(_typeinfo)    \
        m_PoolRouterForIndun.Initialize(_typeinfo::value, _typeinfo::size_init_pool, _typeinfo::size_addin_pool, #_typeinfo, AssertionLogger4Pool_Server);

    REG_INDUN_LIST();

    //_InitAlignKeyMatchingInfo();
}




ZoneManager::~ZoneManager(void)
{
    SAFE_DELETE(m_pObjectKeyStream);
    m_PoolRouter.Release();
    m_PoolRouterForIndun.Release();
    if (m_StaticZones.empty() == false) {
        m_StaticZones.clear();
    };
}



ZoneInterface*
ZoneManager::_FindRoomBySeperated(const eZONETYPE zoneType, const KEYTYPE key) const
{
    const DWORD ROOM_TYPE_MASK = (1 << eZONETYPE_LOBBY      )
                               | (1 << eZONETYPE_HUNTING    )
                               | (1 << eZONETYPE_MISSION    )
                               | (1 << eZONETYPE_PVP        )
                               | (1 << eZONETYPE_CHUNTING   );

    const DWORD inputedType = 1 << zoneType;
    if((ROOM_TYPE_MASK & inputedType) == 0)
        return NULL;

    const ObjectKeyStream::sNode* pNode = m_pObjectKeyStream->GetObjectInfo(key);
    if((pNode && pNode->pObject) == 0)
        return NULL;

    const eZONETYPE storedType = (eZONETYPE)pNode->AddinInfo.ZoneType;
    return (zoneType == storedType)
           ?    pNode->pObject
           :    NULL;
}

LobbyRoom*
ZoneManager::_FindLobbyBySeperated(const eZONETYPE zoneTypeOfLobby, const KEYTYPE key) const
{
    const DWORD LOOBYforROOM_TYPE_MASK = (1<<eZONETYPE_HUNTING  )
                                       | (1<<eZONETYPE_MISSION  )
                                       | (1<<eZONETYPE_PVP      )
                                       | (1<<eZONETYPE_CHUNTING );

    const DWORD inputedType = 1 << zoneTypeOfLobby;
    if((LOOBYforROOM_TYPE_MASK & inputedType) == 0)
        return NULL;

    const ObjectKeyStream::sNode* pNode = m_pObjectKeyStream->GetObjectInfo(key);
    if((pNode && pNode->pObject) == 0)
        return NULL;

    const eZONETYPE storedType = eZONETYPE(pNode->AddinInfo.ZoneType);
    if(eZONETYPE_LOBBY == storedType)
    {
        LobbyRoom* const pLobbyRoom = LobbyRoom_Info::downcast(pNode->pObject);
        const eZONETYPE lobbyForXRoom = pLobbyRoom->GetLobbyType();
        if (zoneTypeOfLobby == lobbyForXRoom) {
            return pLobbyRoom;
        };
    }

    return NULL;
}

VOID
ZoneManager::_FreeZone(ZoneInterface* IN pZone)
{
    m_PoolRouter.Free(pZone->GetType(), pZone);
}


VOID
ZoneManager::_FreeIndun(ZoneInterface* IN pZone)
{
    InstanceDungeon* const pInstanceDungeon = InstanceDungeon_Info::downcast(pZone);
    m_PoolRouterForIndun.Free(pInstanceDungeon->GetDungeonType(), pZone);
}

//ZoneInterface*
//ZoneManager::FindZone(const KEYTYPE key)
//{
//    ZONE_TABLE& rTable = m_ZoneTableList[eZONETYPE_SHI_TOTAL];
//    ZONE_TABLE_IT it = rTable.find(key);
//    return it != rTable.end() ? it->second : NULL;
//}

ZoneInterface*
ZoneManager::FindStaticZone(const MAPCODE mapCode) const
{
    const DWORD STATIC_ZONE_MASK = 
        (1 << eZONETYPE_VILLAGE) | (1 << eZONETYPE_FIELD) | (1 << eZONETYPE_DOMINATION_FIELD) | ( 1 << eZONETYPE_SPA_FIELD);

    ZONE_MCODE_TABLE::const_iterator itFind = m_StaticZones.find(mapCode);
    if(itFind != m_StaticZones.end())
    {
        const KEYTYPE zoneKey = itFind->second;
        ZoneInterface* const pZoneInterface = FindZone(zoneKey);
        if(pZoneInterface)
        {
            const DWORD zoneTypeBit = 1<<pZoneInterface->GetType();
            if(STATIC_ZONE_MASK & zoneTypeBit)
                return pZoneInterface;
        }
    }
    return NULL;
}

Village*
ZoneManager::FindVillage(const MAPCODE mapCode) const
{
    const DWORD STATIC_ZONE_MASK = (1<<eZONETYPE_VILLAGE);
    ZoneInterface* const pZoneInterface = FindStaticZone(mapCode);
    if(pZoneInterface)
    {
        const DWORD zoneTypeBit = 1 << pZoneInterface->GetType();
        if(STATIC_ZONE_MASK & zoneTypeBit)
            return Village_Info::downcast(pZoneInterface);
    }
    return NULL;
}

HuntingField*
ZoneManager::FindHuntingField(const MAPCODE mapCode) const
{
    const DWORD STATIC_ZONE_MASK = (1<<eZONETYPE_FIELD);
    ZoneInterface* const pZoneInterface = FindStaticZone(mapCode);
    if(pZoneInterface)
    {
        const DWORD zoneTypeBit = 1 << pZoneInterface->GetType();
        if(STATIC_ZONE_MASK & zoneTypeBit)
            return HuntingField_Info::downcast(pZoneInterface);
    }
    return NULL;
}

DominationField*
ZoneManager::FindDominationField(const MAPCODE mapCode) const
{
    const DWORD STATIC_ZONE_MASK = (1<<eZONETYPE_DOMINATION_FIELD);
    ZoneInterface* const pZoneInterface = FindStaticZone(mapCode);
    if(pZoneInterface)
    {
        const DWORD zoneTypeBit = 1 << pZoneInterface->GetType();
        if(STATIC_ZONE_MASK & zoneTypeBit)
            return DominationField_Info::downcast(pZoneInterface);
    }
    return NULL;
}

InstanceDungeon*
ZoneManager::FindStaticDungeon(const MAPCODE mapCode) const
{
    const DWORD STATIC_DUNGEON_MASK = (1 << eZONETYPE_INSTANCE);
    const DWORD INDUN_KIND_MASK     = (1 << INDUN_KIND_SS_QUEST);

    ZONE_MCODE_TABLE::const_iterator itFind = m_StaticDungeons.find(mapCode);
    if(itFind != m_StaticDungeons.end())
    {
        const KEYTYPE zoneKey = itFind->second;
        ZoneInterface* const pZoneInterface = FindZone(zoneKey);
        if(pZoneInterface)
        {
            const DWORD zoneTypeBit = 1 << pZoneInterface->GetType();
            if(STATIC_DUNGEON_MASK & zoneTypeBit)
            {
                InstanceDungeon* const pInstanceDungeon = InstanceDungeon_Info::downcast(pZoneInterface);
                const DWORD indunTypeBit = 1 << pInstanceDungeon->GetDungeonType();
                if(INDUN_KIND_MASK & indunTypeBit)
                    return pInstanceDungeon;
                else
                    FlowControl::FCAssert(!"Invalid Static Dungeon : Check ZoneManager Control Flow");
            }
        }
    }
    return NULL;
}

//Village*
//ZoneManager::FindVillage(const MAPCODE mapCode)
//{
//    ZONE_TABLE& rTable = m_ZoneTableList[eZONETYPE_VILLAGE];
//    ZONE_TABLE_IT it = rTable.find(mapCode);
//    return it != rTable.end() ? Village_Info::downcast(it->second) : NULL;
//}

CharSelect*
ZoneManager::FindCharSelect(const KEYTYPE key) const
{
    const KEYTYPE charselect_key = nsKeyInfo::ZONE_KEY_START+1;
    WAVERIX_ZONE_ASSERT(charselect_key == key);

    ZoneInterface* const pZoneInterface = FindZone(key);
    if(pZoneInterface)
    {
        const eZONETYPE zoneType = pZoneInterface->GetType();
        if(eZONETYPE_CHARSELECT == zoneType)
            return CharSelect_Info::downcast(pZoneInterface);
    }
    return 0;
}

LobbyRoom*
ZoneManager::FindCHuntingLobbyByCkey(const KEYTYPE key) const
{
    STLX_MAP< KEYTYPE, KEYTYPE >::const_iterator itFind = m_CHuntingLobbyByCKey.find(key);
    if(itFind != m_CHuntingLobbyByCKey.end())
    {
        const KEYTYPE lobbyKey = itFind->second;
        const ObjectKeyStream::sNode* pNode = m_pObjectKeyStream->GetObjectInfo(lobbyKey);
        if(!(pNode && pNode->pObject))
            return NULL;

        LobbyRoom_Info::pointer_r pLobby = LobbyRoom_Info::downcast(pNode->pObject);
        return pLobby;
    }
    return NULL;
}


VOID
ZoneManager::CreateCharSelect()
{
    CharSelect_Info::pointer_r pCharSelect = CharSelect_Info::downcast(_AllocZone(CharSelect_Info::value));
    const KEYTYPE charselect_key = nsKeyInfo::ZONE_KEY_START+1;
    const KEYTYPE key = AllocKey();
    WAVERIX_ZONE_ASSERT(charselect_key == key);
    pCharSelect->Create(key);
    AddRoom(pCharSelect);
}



#ifdef __NA000000_081029_STATIC_ZONE_MATCHING_TABLE__
// post script loaded, make key-mapcode table for sync worldserver
void ZoneManager::CreateConstStaticZoneTable()
{
    // NOTE: f110520.5L, f081029.x, make static field unique index
    // like villages, fields and domination fields.
    // the '1' key index reserved here to support the character selectable zone.
    const KEYTYPE kConstStaticZoneStartKey = 2;
    if (FlowControl::FCAssert(AllocKey() == kConstStaticZoneStartKey) == false) {
        __debugbreak();
        return;
    };
    const MapInfoParser* const map_parser = MapInfoParser::Instance();
    const KEYTYPE instance_map_start_index = map_parser->GetInstanceMapStartIndex();
    // pop reserved indices
    for (KEYTYPE reserved_index = this->AllocKey();
         reserved_index < instance_map_start_index;
         reserved_index = this->AllocKey())
    {
        continue;
    }
}
#endif //__NA000000_081029_STATIC_ZONE_MATCHING_TABLE__


RC::eROOM_RESULT
ZoneManager::CreateVillage(MAPCODE mapCode, ZoneInterface* OUT pVillageOut)
{
    ServerSession* pServerSession = (ServerSession*)ServerSessionManager::Instance()->GetFieldServer();
    if(!pServerSession)
    {
        SUNLOG(eFULL_LOG, "[RoomControler::CreateVillageRoom] �ʵ弭���� �ƴ� ������ �����Ϸ��Ѵ�.1\n");
        return RC::RC_ROOM_FAILED;
    }

    Village_Info::pointer_r pVillage = Village_Info::downcast(_AllocZone(Village_Info::value));

#ifdef __NA000000_081029_STATIC_ZONE_MATCHING_TABLE__
    // NOTE: f110520.5L, static fields use the reserved unique key.
    const MapInfoParser* const map_parser = MapInfoParser::Instance();
    const sMAPINFO* map_info = map_parser->FindMapInfo(mapCode);
    if (FlowControl::FCAssert(map_info != NULL) == false) {
        return RC::RC_ROOM_FAILED;
    };
    const KEYTYPE alloced_key = map_info->reserved_key_index_;
#else
    const KEYTYPE alloced_key = AllocKey();
#endif

    pVillage->Create(alloced_key, mapCode, pServerSession);
    AddRoom(pVillage);
    if(pVillageOut)
        pVillageOut = pVillage;

    return RC::RC_ROOM_SUCCESS;
}


RC::eROOM_RESULT
ZoneManager::CreateHuntingField(MAPCODE mapCode, ZoneInterface* OUT pHuntingField)
{
    ServerSession* pServerSession = (ServerSession*)ServerSessionManager::Instance()->GetFieldServer();
    if(!pServerSession)
    {
        SUNLOG(eFULL_LOG, "[RoomControler::CreateHuntingField] �ʵ弭���� �ƴ� ������ �����Ϸ��Ѵ�.1\n");
        return RC::RC_ROOM_FAILED;
    }

    HuntingField_Info::pointer_r pField = HuntingField_Info::downcast(_AllocZone(HuntingField_Info::value));
#ifdef __NA000000_081029_STATIC_ZONE_MATCHING_TABLE__
    // NOTE: f110520.5L, static fields use the reserved unique key.
    const MapInfoParser* const map_parser = MapInfoParser::Instance();
    const sMAPINFO* map_info = map_parser->FindMapInfo(mapCode);
    if (FlowControl::FCAssert(map_info != NULL) == false) {
        return RC::RC_ROOM_FAILED;
    };
    const KEYTYPE alloced_key = map_info->reserved_key_index_;
#else
    const KEYTYPE alloced_key = AllocKey();
#endif
    pField->Create(alloced_key, mapCode, pServerSession);
    AddRoom(pField);

    if(pHuntingField)
        pHuntingField = pField;

    return RC::RC_ROOM_SUCCESS;

}


RC::eROOM_RESULT
ZoneManager::CreateDominationField(MAPCODE mapCode, ZoneInterface* OUT pDominationField)
{
    ServerSession* pServerSession = (ServerSession*)ServerSessionManager::Instance()->GetFieldServer();
    if(!pServerSession)
    {
        SUNLOG(eFULL_LOG, "[RoomControler::CreateDominationField] �ʵ弭���� �ƴ� ������ �����Ϸ��Ѵ�.1\n");
        return RC::RC_ROOM_FAILED;
    }

    DominationField_Info::pointer_r pField = DominationField_Info::downcast(_AllocZone(DominationField_Info::value));
#ifdef __NA000000_081029_STATIC_ZONE_MATCHING_TABLE__
    // NOTE: f110520.5L, static fields use the reserved unique key.
    const MapInfoParser* const map_parser = MapInfoParser::Instance();
    const sMAPINFO* map_info = map_parser->FindMapInfo(mapCode);
    if (FlowControl::FCAssert(map_info != NULL) == false) {
        return RC::RC_ROOM_FAILED;
    };
    const KEYTYPE alloced_key = map_info->reserved_key_index_;
#else
    const KEYTYPE alloced_key = AllocKey();
#endif
    pField->Create(alloced_key, mapCode, pServerSession);
    AddRoom(pField);

    if(pDominationField)
        pDominationField = pField;

    return RC::RC_ROOM_SUCCESS;
}

ServerSessionEx*
ZoneManager::_CheckRoomCountLimit(FlowControl& flow)    // �� ���� ���Ѱ� ���� LinkServer Check
{
    FLOWCONTROL_START()
    {
        // start LOGIC_BLOCK
        ServerSessionManager* const pServerSessionManager = ServerSessionManager::Instance();
        ServerSessionEx* pServerSession = pServerSessionManager->GetFieldServer();
        IfFailThenBreak(pServerSession, RC::RC_ROOM_FAILED);

        const DWORD battleSessions = pServerSessionManager->GetBattleServerSessionNum();
        IfFailThenBreak(battleSessions, RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT);

        const SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();
        const WORD limit_max_room = stServerOptionInfo.m_wMAX_ROOM_COUNT;

        IfFailThenBreak(GetBattleServerZoneCount() < limit_max_room * battleSessions,
                        RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT);

        return pServerSession;
    }
    FLOWCONTROL_END;
    
    return NULL;
};


void
ZoneManager::CheckRuleForRoom(User* creator, const BASE_ROOMINFO& base, BASE_ROOMINFO* OUT modified)
{
    // Create~~Room�� �����ϱ� ���� �ѹ� ��ġ���� ����.
    *modified = base;

    // (WAVERIX) (090829) (NOTE) ������ ���� PC�� ���� ���� ��å�� �����ۿ� ���ٰ� �Ѵ�.
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    if(modified->m_SpecialType == eROOM_SPECIAL_PCBANG_SUPER_ROOM) {
        if(creator->CanApplyPCBangBenefit(TRUE) == 0)
            modified->m_SpecialType = eROOM_SPECIAL_NORMAL;
    }
#endif
}


RC::eROOM_RESULT
ZoneManager::CreateLobbyRoom(KEYTYPE staticZoneKey, User* pMasterUser,
                             MAPCODE mapCode, eZONETYPE eLobbyType, eZONEPUBLIC ePublic,
                             TCHAR* pszRoomTitle, TCHAR* pszRoomPWD, BASE_ROOMINFO& IN roomInfo,
                             BYTE byMaxLimitUserNum, KEYTYPE linkHuntingKey,
                             LobbyRoom*& OUT pLobbyRoom)
{
    FlowControl flow;
    FLOWCONTROL_START()
    {
        // start LOGIC_BLOCK
        ServerSessionEx* pServerSession = _CheckRoomCountLimit(flow);
        if(NULL == pServerSession)
            break;

        // (WAVERIX) (090522) (CHANGES) Ʃ�丮�� ���� �˻� ��� ����
        ZoneInterface* const pRequestedZone = FindZone(staticZoneKey);
        BOOLEAN isAcceptable = pRequestedZone &&
                               (pRequestedZone->GetType() == eZONETYPE_VILLAGE ||
                               pRequestedZone->GetMapCode() == HardCode::MAPCODE_for_TUTORIAL_FIELD);
        IfFailThenBreak(isAcceptable != 0, RC::RC_ROOM_NOTEXISTVILLAGE);

        //////////////////////////////////////////////////////////////////////////
        //    <Main Logic>
        const KEYTYPE lobbyKey = AllocKey();
        pLobbyRoom = LobbyRoom_Info::downcast(_AllocZone(LobbyRoom_Info::value));

        RC::eROOM_RESULT rt = pLobbyRoom->Create(pMasterUser, lobbyKey, mapCode, eLobbyType, ePublic, pszRoomTitle, pszRoomPWD, roomInfo, (ServerSession*)pServerSession, linkHuntingKey);
        IfFailThenBreak(rt == RC::RC_ROOM_SUCCESS, rt);
        //    �ο��� ����
        // �ִ��ο� ������ �����Ѵ�.
    #ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
        if(roomInfo.IsSpecialType(eROOM_SPECIAL_GUILD_PVP))
            pLobbyRoom->SetLimitMaxUserNum(5);
        else
    #endif
        if(byMaxLimitUserNum)
        {   // (WAVERIX) (090522) (CHANGES)
            const sMAPINFO* pInfo = pLobbyRoom->GetMapInfo();
            //const sMAPINFO* pInfo = pMapInfoParser->FindMapInfo(mapCode);
            byMaxLimitUserNum = min(pInfo->byMaxUserNum, byMaxLimitUserNum);
            pLobbyRoom->SetLimitMaxUserNum(byMaxLimitUserNum);
        }

        AddRoom(pLobbyRoom);

        // ���� ��Ƽ���¿� ���� �濡 ������ ��Ƽ�� �����ϰų� �����Ѵ�.
        AgentPartyManager::Instance()->CreateZone(pMasterUser, pRequestedZone, pLobbyRoom);

        RoomArgument args(ePublic, pszRoomPWD);
        rt = MoveZone(pRequestedZone, pLobbyRoom, pMasterUser, args);
        IfFailThenBreak(rt == RC::RC_ROOM_SUCCESS, rt);

        //    ��... pLobbyRoom ��~ �߳�?
        //    ���� ������ Ȯ�� �ʿ�.

        return rt;
    }
    FLOWCONTROL_END;

    const DWORD errCode = flow.GetErrorCode();
    DWORD OUT errAddin = 0;
    switch(errCode)
    {
    case RC::RC_ROOM_FAILED: break;
    case RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT: errAddin = GetBattleServerZoneCount(); break;
    case RC::RC_ROOM_NOTEXISTVILLAGE: errAddin = pMasterUser->GetZoneType(); break;
    default:
        break;
    }

    SUNLOG(eCRITICAL_LOG, _T("[%s][%u] - info [%u]"), __FUNCTION__, errCode, errAddin);
    
    return (RC::eROOM_RESULT)errCode;
}

RC::eROOM_RESULT
ZoneManager::CreateMissionRoom(KEYTYPE lobbyKey, User* pMasterUser, MissionRoom*& OUT pMissionRoom)
{
    ZoneInterface* pRoom = FindZone(lobbyKey);
    if(!pRoom || pRoom->GetType() != eZONETYPE_LOBBY) 
        return RC::RC_ROOM_NOTEXISTLOBBY;
    LobbyRoom* pLobbyRoom = (LobbyRoom *)pRoom;
    if(pLobbyRoom->GetMaster() != pMasterUser)
        return RC::RC_ROOM_ISNOTMATERPLAYER;
    if(pLobbyRoom->GetLobbyType() != eZONETYPE_MISSION)
        return RC::RC_ROOM_INVALID_LASTROOMTYPE;
    if(!pLobbyRoom->IsAllReady())
        return RC::RC_ROOM_IS_NOT_ALL_READY;
    if(!pLobbyRoom->IsPrepareUserNum())
        return RC::RC_ROOM_INSUFFICIENT_USER_NUM;

    DWORD dwServerSessionIndex = ServerSessionManager::Instance()->Incr_LOF_BattleServer();
    if(dwServerSessionIndex == 0) 
        return RC::RC_ROOM_FAILED;

    ServerSession* pServerSession = (ServerSession*)ServerSessionManager::Instance()->FindServer(dwServerSessionIndex);
    if(!pServerSession || pServerSession->GetServerType() != BATTLE_SERVER)
    {
        SUNLOG(eFULL_LOG,  "��Ʋ������ �ƴ� ������ �����Ϸ��Ѵ�.2\n");
        return RC::RC_ROOM_FAILED;
    }

    KEYTYPE MissionKey = AllocKey();
    pMissionRoom = MissionRoom_Info::downcast(_AllocZone(MissionRoom_Info::value));
    RC::eROOM_RESULT rt = pMissionRoom->Migrate(MissionKey, pLobbyRoom, pServerSession);
    if(rt != RC::RC_ROOM_SUCCESS)
    {
        FreeKey(MissionKey);
        _FreeZone(pMissionRoom);
        return rt;
    }


    AddRoom(pMissionRoom);

    // ���� ��Ƽ���¿� ���� �濡 ������ ��Ƽ�� �����ϰų� �����Ѵ�.
    AgentPartyManager::Instance()->CreateZone(pMasterUser, pLobbyRoom, pMissionRoom);

    RoomArgument args(pLobbyRoom->GetZonePublic(), pLobbyRoom->GetRoomPWD());
    rt = MoveZone(pLobbyRoom, pMissionRoom, pMasterUser, args);
    ASSERT(rt == RC::RC_ROOM_SUCCESS);

    return RC::RC_ROOM_SUCCESS;
}


RC::eROOM_RESULT
ZoneManager::CreatePVPRoom(KEYTYPE lobby_key, User* master_user, PVPRoom*& OUT pvp_room)
{
    ZoneInterface* room = FindZone(lobby_key);
    if (room == NULL || room->GetType() != eZONETYPE_LOBBY)
    {
        return RC::RC_ROOM_NOTEXISTLOBBY;
    }

    LobbyRoom* lobby_room = static_cast<LobbyRoom*>(room);
    if (lobby_room->GetMaster() != master_user)
    {
        return RC::RC_ROOM_ISNOTMATERPLAYER;
    }
    if (lobby_room->GetLobbyType() != eZONETYPE_PVP)
    {
        return RC::RC_ROOM_INVALID_LASTROOMTYPE;
    }
    if (lobby_room->IsAllReady() == FALSE)
    {
        return RC::RC_ROOM_IS_NOT_ALL_READY;
    }
    if (lobby_room->IsPrepareUserNum() == FALSE)
    {
        return RC::RC_ROOM_INSUFFICIENT_USER_NUM;
    }

    DWORD server_session_index = ServerSessionManager::Instance()->Incr_LOF_BattleServer();
    if (server_session_index == 0)
    {
        return RC::RC_ROOM_FAILED;
    }

    ServerSession* server_session = 
        (ServerSession*)ServerSessionManager::Instance()->FindServer(server_session_index);
    if (server_session == NULL || server_session->GetServerType() != BATTLE_SERVER)
    {
        SUNLOG(eFULL_LOG, __FUNCTION__" : Invalid server session to connect");
        return RC::RC_ROOM_FAILED;
    }

    KEYTYPE pvp_room_key = AllocKey();
    pvp_room = PVPRoom_Info::downcast(_AllocZone(PVPRoom_Info::value));
    RC::eROOM_RESULT result_code = pvp_room->Migrate(pvp_room_key, lobby_room, server_session);
    if (result_code != RC::RC_ROOM_SUCCESS)
    {
        FreeKey(pvp_room_key);
        _FreeZone(pvp_room);
        return result_code;
    }

    AddRoom(pvp_room);

    // ���� ��Ƽ���¿� ���� �濡 ������ ��Ƽ�� �����ϰų� �����Ѵ�.
    AgentPartyManager::Instance()->CreateZone(master_user, lobby_room, pvp_room);

    RoomArgument args(lobby_room->GetZonePublic(), lobby_room->GetRoomPWD());
    result_code = MoveZone(lobby_room, pvp_room, master_user, args);
    ASSERT(result_code == RC::RC_ROOM_SUCCESS);

    return RC::RC_ROOM_SUCCESS;
}



RC::eROOM_RESULT
ZoneManager::CreateRelayMissionLobbyRoom(MissionRoom* IN pMissionRoom , User* pMasterUser ,
                                         LobbyRoom*& OUT pLobbyRoom)
{
    ServerSessionEx* pServerSession = ServerSessionManager::Instance()->GetFieldServer();
    if(!pServerSession)
    {
        SUNLOG(eCRITICAL_LOG, "[CreateRelayMissionLobbyRoom] �ʵ弭���� �������� �ʽ��ϴ�.\n");
        return RC::RC_ROOM_FAILED;
    }

    if(!AgentPartyManager::Instance()->IsMaster(pMasterUser))
        return RC::RC_ROOM_ISNOTMATERPLAYER;

    KEYTYPE lobbyKey = AllocKey();
    pLobbyRoom = LobbyRoom_Info::downcast(_AllocZone(LobbyRoom_Info::value));
    RC::eROOM_RESULT rt = pLobbyRoom->Create(pMasterUser, lobbyKey, pMissionRoom->GetMapCode(), eZONETYPE_MISSION, 
        pMissionRoom->GetZonePublic(), pMissionRoom->GetRoomTitle(), pMissionRoom->GetRoomPWD(), 
        *pMissionRoom->GetRoomInfo(), (ServerSession*)pServerSession, 0);
    if(rt != RC::RC_ROOM_SUCCESS)
    {
        FreeKey(lobbyKey);
        _FreeZone(pLobbyRoom);
        SUNLOG(eFULL_LOG, "[CreateRelayMissionLobbyRoom] �κ���� ����! ErrorCode[%d] lobbyKey[%d] mapCode[%d]", rt, lobbyKey, pMissionRoom->GetMapCode());
        return rt;
    }

    // �ִ��ο� ������ �����Ѵ�.
    pLobbyRoom->SetLimitMaxUserNum(pMissionRoom->GetLimitMaxUserNum());

    // ��Ƽ������ ���� �� �ֵ��� ���ð��� �����Ѵ�.
    pLobbyRoom->SetWaitTimer(MISSION_RELAY_NOTICE_TIME);

    AddRoom(pLobbyRoom);

    // ���� ��Ƽ���¿� ���� �濡 ������ ��Ƽ�� �����ϰų� �����Ѵ�.
    AgentPartyManager::Instance()->CreateZone(pMasterUser, pMissionRoom, pLobbyRoom);

    RoomArgument args(pMissionRoom->GetZonePublic(), pMissionRoom->GetRoomPWD());
    rt = MoveZone(pMissionRoom, pLobbyRoom, pMasterUser, args);
    if(rt != RC::RC_ROOM_SUCCESS)
    {
        SUNLOG(eCRITICAL_LOG, "[CreateRelayMissionLobbyRoom] MoveZone Fail! Error Code[%d]\n", rt);
        ASSERT(rt == RC::RC_ROOM_SUCCESS);
    }

    return rt;
}


RC::eROOM_RESULT
ZoneManager::CreateInstanceDungeon(KEYTYPE zoneKey, User* pMasterUser, MAPCODE mapCode,
                                   InstanceDungeon*& pInstanceDungeon, eINSTANCE_DUNGEON_KIND eKind,
                                   BOOL UseChargeItem)
{
    ServerSessionManager* const pServerSessionManager = ServerSessionManager::Instance();
    pInstanceDungeon = NULL;
    KEYTYPE allocedKey = 0;

    FlowControl flow;
    FLOWCONTROL_START()
    {
        // start LOGIC_BLOCK
        ServerSessionEx* const pFieldSession = _CheckRoomCountLimit(flow);
        if(NULL == pFieldSession)
            break;

        ZoneInterface* const pZoneInterface = FindZone(zoneKey);
        if (pMasterUser) {
            IfFailThenBreak(pZoneInterface, RC::RC_ROOM_FAILED);
        }

        const DWORD alignedSessionIdx = pServerSessionManager->Incr_LOF_BattleServer();
        IfFailThenBreak(alignedSessionIdx, RC::RC_ROOM_FAILED);
        ServerSessionEx* const pBattleSession = pServerSessionManager->FindServer(alignedSessionIdx);
        IfFailThenBreak(pBattleSession && pBattleSession->GetServerType() == BATTLE_SERVER,
                        RC::RC_ROOM_FAILED);

        allocedKey = AllocKey();
        pInstanceDungeon = InstanceDungeon_Info::downcast(_AllocIndun(eKind));
        IfFailThenBreak(pInstanceDungeon->RegisterRuleset(mapCode), RC::RC_ROOM_FAILED);

        const RC::eROOM_RESULT rt = pInstanceDungeon->Create(pMasterUser, mapCode, allocedKey, pBattleSession, UseChargeItem);
        IfFailThenBreak(RC::RC_ROOM_SUCCESS == rt, rt);

        AddRoom(pInstanceDungeon);

        AgentPartyManager::Instance()->CreateZone(pMasterUser, pZoneInterface, pInstanceDungeon);

        return RC::RC_ROOM_SUCCESS;
    }
    FLOWCONTROL_END;

    const DWORD errCode = flow.GetErrorCode();
    DWORD OUT errAddin = 0;
    switch(errCode)
    {
    case RC::RC_ROOM_FAILED:    break;
    case RC::RC_ROOM_ISNOT_MASTER_OF_PARTY:
    case RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT:    errAddin = GetBattleServerZoneCount();    break;
    case RC::RC_ROOM_NOTEXISTVILLAGE:    errAddin = pMasterUser->GetZoneType(); break;
    }
    // release resource
    if(allocedKey)       FreeKey(allocedKey);
    if(pInstanceDungeon) _FreeIndun(pInstanceDungeon);

    SUNLOG(eCRITICAL_LOG, _T("[%s][%u] - info [%u]"), __FUNCTION__, errCode, errAddin);

    return (RC::eROOM_RESULT)errCode;
}

RC::eROOM_RESULT
ZoneManager::CreateStaticDungeon(const MAPCODE mapCode,
                                 InstanceDungeon*& OUT rpStaticDungeon,
                                 const eINSTANCE_DUNGEON_KIND kindOfDungeon,
                                 BOOL bUseChargeItem) // current, not used
{
    ServerSessionManager* const pServerSessionManager = ServerSessionManager::Instance();
    rpStaticDungeon = NULL;
    KEYTYPE allocedKey = 0;

    //    (THINKING) (NOTEs)
    //    -    Agent->Battle�� 1..n�� �����̴�. Load Balancing�� ��ĳ�� ���ΰ�?
    //    -    �ϴ� ���� �������� �����ϱ�� �Ѵ�.

    FlowControl flow;
    FLOWCONTROL_START()
    {
        // start LOGIC_BLOCK
        ServerSessionEx* const pFieldSession = _CheckRoomCountLimit(flow);
        if(NULL == pFieldSession)
            break;

        const DWORD alignedSessionIdx = pServerSessionManager->Incr_LOF_BattleServer();
        IfFailThenBreak(alignedSessionIdx, RC::RC_ROOM_FAILED);

        ServerSessionEx* const pBattleSession = pServerSessionManager->FindServer(alignedSessionIdx);
        IfFailThenBreak(pBattleSession && pBattleSession->GetServerType() == BATTLE_SERVER,
                        RC::RC_ROOM_FAILED);

        //    static dungeon�� �ϳ��� �����Ѵ�.
        IfFailThenBreak(!FindStaticDungeon(mapCode), RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT);


        allocedKey = AllocKey();
        rpStaticDungeon = InstanceDungeon_Info::downcast(_AllocIndun(kindOfDungeon));
        // (NOTE) (WAVERIX) remove code which is presented by [48549],
        // because of the result of a _AllocXXX must be succeed.
        // I suppose a fail status for reason of a memory problem.
        // the below skipped code section has a leak resource problem 
        // which is based on return directly.
        /*if(rpStaticDungeon == NULL)
        {
            SUNLOG(eCRITICAL_LOG, _T("_AllocIndun fail [%s]"), __FUNCTION__);
            return RC::RC_ROOM_FAILED;
        }*/
		IfFailThenBreak(rpStaticDungeon,RC::RC_ROOM_FAILED);

        IfFailThenBreak(rpStaticDungeon->RegisterRuleset(mapCode), RC::RC_ROOM_FAILED);

        const RC::eROOM_RESULT rt = rpStaticDungeon->Create(NULL, mapCode, allocedKey, pBattleSession, bUseChargeItem);
        IfFailThenBreak(RC::RC_ROOM_SUCCESS == rt, rt);

        AddRoom(rpStaticDungeon);

        return RC::RC_ROOM_SUCCESS;
    }
    FLOWCONTROL_END;

    const DWORD errCode = flow.GetErrorCode();
    DWORD OUT errAddin = 0;
    switch(errCode)
    {
    case RC::RC_ROOM_FAILED:    break;
    case RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT: errAddin = GetBattleServerZoneCount(); break;
    }
    // release resource
    if(allocedKey)        FreeKey(allocedKey);
    if(rpStaticDungeon)    _FreeIndun(rpStaticDungeon);

    SUNLOG(eCRITICAL_LOG, _T("[%s][%u] - info [%u]"), __FUNCTION__, errCode, errAddin);

    return (RC::eROOM_RESULT)errCode;
}

#if defined(__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__) && \
    !defined(__NA_0_20081029_PARTY_REFACTORING)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

RC::eROOM_RESULT
ZoneManager::CreatePortal(const QuestPortalInfo& rQuestPortalInfo, User* pUser, BOOLEAN isTempPortal)
{
    // ��Ż ���� ó���� ��� PortalManager���� �����Ѵ�.
    // �� ���� ����, �� ���� ���� ���� ���� ZoneManager���� �ذ��ؾ� �ϴ� �͵��� �����Ѵ�.
    FlowControl flow;
    ServerSessionEx* pFieldSession = _CheckRoomCountLimit(flow);
    ServerSessionEx* pBattleSession = 0;
    { // session constraints
        if(pFieldSession == 0)
            return (RC::eROOM_RESULT)flow.GetErrorCode();
        ServerSessionManager* pSessionManager = ServerSessionManager::Instance();
        DWORD alignedSessionIdx = pSessionManager->Incr_LOF_BattleServer();
        if(alignedSessionIdx == 0)
            return RC::RC_ROOM_FAILED;
        pBattleSession = pSessionManager->FindServer(alignedSessionIdx);
        if(pBattleSession == 0)
            return RC::RC_ROOM_FAILED;
    };

    if(PortalManager::IsValidRequesterForCreatePortal(pUser) == FALSE)
        return RC::RC_ROOM_INVALIDROOMTYPE;

    const MAPCODE enterableMapCode = rQuestPortalInfo.enterableMapCode;
    const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(enterableMapCode);
    if(pMapInfo == 0)
        return RC::RC_ROOM_INVALID_MAPCODE;

    //const DWORD playerKey = pUser->GetPlayerKey();
    //User* pMasterUser = pUser;
    //AgentPartyManager* pPartyManager = AgentPartyManager::Instance();
    //{   // (WAVERIX) (090211) (NOTE) partyKey�� ���� INVALID_VALUE�� ���Ǹ� ����Ѵٸ�,
    //    // ���� �Ʒ��� ���� ó���� ���� �ʾƵ� �ȴ�.
    //    WORD partyKey = pUser->GetPartyState().GetPartyKey();
    //    DWORD masterKey = pPartyManager->GetMasterKey(partyKey);
    //    if(masterKey && (playerKey != masterKey))
    //        pMasterUser = UserManager::Instance()->GetUserByObjKey(masterKey);
    //};

    RoomArgumentForRoomSetup args;
    BASE_ROOMINFO& rRoomInfo = args.m_RoomInfo;
    // (WAVERIX) (090829) (WARNING) (CURRENT) LEVELTYPE : WORD
    rRoomInfo.m_MinLV = BYTE(pMapInfo->minLV);    // ��εȴ�. ���� ����ȯ �ع����� ���߿� ã�Ⱑ...
    rRoomInfo.m_MaxLV = BYTE(pMapInfo->maxLV);
    rRoomInfo.m_ClassLimit = UCHAR_MAX;
    //{__NA1335_090407_PORTAL_MISSION_ROOM_CONSTRAINTS__
    BOOLEAN isSuperRoomPc = !!pUser->CanApplyPCBangBenefit(true);
    BOOLEAN isSuperRoomItem = !!pUser->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER);
  #ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    if(isSuperRoomPc)
        rRoomInfo.m_SpecialType |= eROOM_SPECIAL_PCBANG_SUPER_ROOM;
    if(isSuperRoomItem)
        rRoomInfo.m_SpecialType |= eROOM_SPECIAL_ITEM1_SUPER_ROOM;
  #else
    rRoomInfo.m_SpecialType = isSuperRoomPc   ? eROOM_SPECIAL_PCBANG_SUPER_ROOM
                            : isSuperRoomItem ? eROOM_SPECIAL_ITEM1_SUPER_ROOM
                            :                   0;
  #endif
    //}
    //    �ο��� ����
    // �ִ��ο� ������ �����Ѵ�.
    BYTE maxLimitUserNum = 10;
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    if(rRoomInfo.IsSpecialType(eROOM_SPECIAL_GUILD_PVP))
    {
        maxLimitUserNum = 5;
    }
    else
#endif
    {
        maxLimitUserNum = min(pMapInfo->byMaxUserNum, maxLimitUserNum);
    }

    args.m_pMapInfo = pMapInfo;
    args.m_MaxLimitUserNum = maxLimitUserNum;

    PORTALKEY portalKey = PortalManager::CreatePortal(rQuestPortalInfo, isTempPortal, args);
    if(portalKey == INVALID_PORTAL_KEY)
        return RC::RC_ROOM_FAILED;

    return RC::RC_ROOM_SUCCESS;
}

// called by PortalManager::Update (timeout event)
RC::eROOM_RESULT
ZoneManager::ClosePortal(const QuestPortalInfo& rQuestPortalInfo, KEYTYPE roomKey)
{
    __TOUCHED(rQuestPortalInfo);
    if(roomKey == 0)
        return RC::RC_ROOM_SUCCESS;

    // portal�� ���� ������ ���� PortalManager�� �������� ��������� �ƴϴ�.
    ZoneInterface* pZone = FindZone(roomKey);
    if(pZone == 0)
        return RC::RC_ROOM_SUCCESS;
    if(!FlowControl::FCAssert(pZone->IsRoomType()))
        return RC::RC_ROOM_INVALID_ROOM_TYPE;

    //RoomInterface* pRoom = static_cast<RoomInterface*>(pZone);
    // (WAVERIX) (090415) (LOGIC-CHANGE) ��Ż ���Ž� �濡 �ɸ� ������ �ʱ�ȭ �ϸ� �ȵȴ�.
    // ������ ������ݾ�? ���� PORTAL������ ���� ������...
    //{
    //    PolicymentForRoom& rPolicymentForRoom = pRoom->RoomPolicyment;
    //    rPolicymentForRoom.SetPortalKey(INVALID_PORTAL_KEY);
    //    pRoom->ChangePolicy(rPolicymentForRoom.FIELD & ~rPolicymentForRoom.QUEST_PORTAL_CONTROL);
    //}
    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT
ZoneManager::CreateRoomByPortal(PORTALKEY portalKey, User* pFirstEnterUser,
                                const RoomArgumentForRoomSetup& rRoomArgument)
{
    if(!FlowControl::FCAssert(rRoomArgument.m_pMapInfo))
        return RC::RC_ROOM_INVALID_ROOM_TYPE;

    FlowControl flow;
    ServerSessionEx* pFieldSession = _CheckRoomCountLimit(flow);
    ServerSessionEx* pBattleSession = 0;
    { // session constraints
        if(pFieldSession == 0)
            return (RC::eROOM_RESULT)flow.GetErrorCode();
        ServerSessionManager* pSessionManager = ServerSessionManager::Instance();
        DWORD alignedSessionIdx = pSessionManager->Incr_LOF_BattleServer();
        if(alignedSessionIdx == 0)
            return RC::RC_ROOM_FAILED;
        pBattleSession = pSessionManager->FindServer(alignedSessionIdx);
        if(pBattleSession == 0) // ��, ���������� �����ؾ� �Ѵ�.
            return RC::RC_ROOM_FAILED;
    };

    const eZONETYPE toZoneType = eZONETYPE(rRoomArgument.m_pMapInfo->byMKind);

    if(PortalManager::IsValidCreatingZone(toZoneType) == FALSE)
        return RC::RC_ROOM_INVALID_ROOM_TYPE; //...��Ż ������ üũ�� ��...

    const KEYTYPE allocKey = AllocKey();
    RC::eROOM_RESULT result = RC::RC_ROOM_SUCCESS;
    RoomInterface* allocRoom = 0;

    if(toZoneType == MissionRoom_Info::value)
    {
        if(!FlowControl::FCAssert(pFirstEnterUser->IsBeginTransaction() == FALSE))
            result = RC::RC_ROOM_INVALID_ROOM_TYPE;

        if(result == RC::RC_ROOM_SUCCESS)
        {
            MissionRoom* pRoom = MissionRoom_Info::downcast(_AllocZone(MissionRoom_Info::value));
            allocRoom = pRoom;
            PolicymentForRoom& rRoomPolicyment = pRoom->RoomPolicyment;
            pRoom->ChangePolicy(rRoomPolicyment.FIELD | rRoomPolicyment.QUEST_PORTAL_CONTROL);
            rRoomPolicyment.SetPortalKey(portalKey);

            pFirstEnterUser->SetBehaveState(PLAYER_BEHAVE_IDLE_STATE);
            pFirstEnterUser->BeginTransaction(TR_LEAVE_FIELD_FOR_ROOM_BY_PORTAL);
            result = pRoom->MigratePortalUserFirstEnter(allocKey, rRoomArgument,
                                                        pFirstEnterUser, pBattleSession);
        }

        if(result != RC::RC_ROOM_SUCCESS)
        {
            FreeKey(allocKey);
            _FreeZone(allocRoom);
            pFirstEnterUser->EndTransaction();
            return result;
        }

        AddRoom(allocRoom);
        KEYTYPE fromZoneKey = pFirstEnterUser->GetZoneKey();
        ZoneInterface* pFromZone = FindZone(fromZoneKey);

        AgentPartyManager* pPartyManager = AgentPartyManager::Instance();
        pPartyManager->EnterZoneByPortal(portalKey, pFirstEnterUser, pFromZone, allocRoom);

        allocRoom->SetRoomState(eROOMSTATE_GENERNAL);

        CheckMoveZonePolicy* const pCheckMoveZonePolicy =
            MoveZonePolicy<MZTF_STATIC_TO_ROOM>::AlignInfo(
                pFirstEnterUser,
                //{     From        |               To          }
                pFromZone->GetType(),   allocRoom->GetType(),   //  eZONETYPE
                fromZoneKey,            allocRoom->GetKey());   //  zoneKey

        result = MoveZone(pCheckMoveZonePolicy, rRoomArgument);
        if(result != RC::RC_ROOM_SUCCESS)
        {
            pFirstEnterUser->EndTransaction();
            // (WAVERIX) (090323) (BUG-FIX) ���� ���� ���н� ������ ��Ƽ���� Ż��
            // ���ҽ� ����... �ǰ��ϳ�...
            pPartyManager->LeaveParty(pFirstEnterUser, eREASON_LEAVE_PARTY_INTEND);
            DestroyZone(allocRoom);
            return result;
        }

        pFirstEnterUser->SetTRRoomKey(allocKey);
        return RC::RC_ROOM_SUCCESS;
    }
    else if(toZoneType == InstanceDungeon_Info::value)
    {
        // TODO(WAVERIX) ������ �����ϴ�.
        ASSERT(!"Not Implementation! Instance Dungeon Portal");
        //InstanceDungeon* pRoom = InstanceDungeon_Info::downcast(pRefRoom);
        //allocRoom = pRoom;
        //result = pRoom->MigratePortalUser(allocKey, pRefRoom, pBattleSession);
    }

    return RC::RC_ROOM_FAILED;
}

RC::eROOM_RESULT
ZoneManager::EnterPortal(PORTALKEY portalKey, User* pUser,
                         const RoomArgumentForRoomSetup& rRoomArgument, KEYTYPE enterableRoomKey)
{
    const eZONETYPE enterZoneType = eZONETYPE(rRoomArgument.m_pMapInfo->byMKind);

    //if(PortalManager::IsValidCreatingZone(enterZoneType) == FALSE)
    //    return RC::RC_ROOM_INVALID_ROOM_TYPE; //...��Ż ������ üũ�� ��...

    RC::eROOM_RESULT result = RC::RC_ROOM_SUCCESS;
    RoomInterface* pRoom = 0;
    {
        ZoneInterface* pZone = FindZone(enterableRoomKey);
        if(pZone->IsRoomType() == FALSE)
            return RC::RC_ROOM_INVALID_ROOM_TYPE;
        pRoom = static_cast<RoomInterface*>(pZone);
    };

    if(enterZoneType == MissionRoom_Info::value)
    {
        if(!FlowControl::FCAssert(pRoom &&
                                  pUser->IsBeginTransaction() == FALSE))
            result = RC::RC_ROOM_INVALID_ROOM_TYPE;

        if(result != RC::RC_ROOM_SUCCESS)
            return result;

        pUser->SetBehaveState(PLAYER_BEHAVE_IDLE_STATE);
        pUser->BeginTransaction(TR_LEAVE_FIELD_FOR_ROOM_BY_PORTAL);
        //result = pRoom->MigratePortalUser(allocKey, pRefRoom, pUser, pBattleSession);
        //PortalManager::OnRoomEvent(portalKey, true, allocKey);

        KEYTYPE fromZoneKey = pUser->GetZoneKey();
        ZoneInterface* pFromZone = FindZone(fromZoneKey);
        KEYTYPE toZoneKey = pRoom->GetKey();

        { // about party
            AgentPartyManager* pPartyManager = AgentPartyManager::Instance();
            pPartyManager->EnterZoneByPortal(portalKey, pUser, pFromZone, pRoom);
            //AgentPartyManager::Instance()->Join~(pMasterUser, /*pZoneInterface*/, allocRoom);
        }

        CheckMoveZonePolicy* const pCheckMoveZonePolicy =
            MoveZonePolicy<MZTF_STATIC_TO_ROOM>::AlignInfo(
            pUser,
            //{     From        |           To          }
            pFromZone->GetType(),   pRoom->GetType(),   //  eZONETYPE
            fromZoneKey,            toZoneKey);         //  zoneKey

        result = MoveZone(pCheckMoveZonePolicy, rRoomArgument);
        if(result != RC::RC_ROOM_SUCCESS)
        {
            pUser->EndTransaction();
            return result;
        }

        pUser->SetTRRoomKey(toZoneKey);
        return RC::RC_ROOM_SUCCESS;
    }
    else if(enterZoneType == InstanceDungeon_Info::value)
    {
        // TODO(WAVERIX) ������ �����ϴ�.
        ASSERT(!"Not Implementation! Instance Dungeon Portal");
        //InstanceDungeon* pRoom = InstanceDungeon_Info::downcast(pRefRoom);
        //allocRoom = pRoom;
        //result = pRoom->MigratePortalUser(allocKey, pRefRoom, pBattleSession);

        //if(result != RC::RC_ROOM_SUCCESS)
        //{
        //    FreeKey(allocKey);
        //    _FreeZone(pRoom);
        //    return result;
        //}
    }
    return RC::RC_ROOM_FAILED;
}

BOOL
ZoneManager::AutoMatching(KEYTYPE lobbyKey, User* pMasterUser, CHuntingRoom*& OUT pCHuntingRoom)
{
    ZoneInterface* pRoom = FindZone(lobbyKey);
    if(!pRoom || pRoom->GetType() != eZONETYPE_LOBBY) 
        return FALSE;
    LobbyRoom* pLobbyRoom = (LobbyRoom *)pRoom;
    if(pLobbyRoom->GetMaster() != pMasterUser)
        return FALSE;
    if(pLobbyRoom->GetLobbyType() != eZONETYPE_CHUNTING)
        return FALSE;
    if(!pLobbyRoom->IsAllReady())
        return FALSE;
    if(pLobbyRoom->GetZonePublic() == eZONEPUBLIC_PRIVATE)
        return FALSE;

    // ������ üũ�� �ٽ� �ѹ� ������ ����!
    if(!pLobbyRoom->IsPrepareUserNum())
        return FALSE;

    SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();

    // �������� 1���� ���� ��Ī��󿡼� ���ܵȴ�.
    if(pLobbyRoom->GetCurUserNum() < stServerOptionInfo.m_byMATCHING_MIN_USER_NUM)
        return FALSE;

    // ���� �κ���� ��շ���, ��ս·�, �ɼ�����
    LEVELTYPE LobbyAvgLevel = pLobbyRoom->GetAvgLevel();
    BYTE LobbyAvgWinRatio = 0; DWORD LobbyTotCount = 0;
    pLobbyRoom->GetCHuntingScore(LobbyAvgWinRatio, LobbyTotCount);
    int MatchingCondition = stServerOptionInfo.m_byMATCHING_WIN_RATIO;

    // ����� ���������ΰ�
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    BOOL bLobbyGuildPVP = pLobbyRoom->GetRoomInfo()->IsSpecialType(eROOM_SPECIAL_GUILD_PVP);
#endif

    {
        const INT _matchingCondValue = (INT)((INT)stServerOptionInfo.m_byMATCHING_WIN_RATIO - LobbyTotCount / 200);
        MatchingCondition = max(5, _matchingCondValue);
    };

    const ADDITIONAL_CHUNTINGINFO_EX* pLobbyInfo = pLobbyRoom->GetAdditionalCHuntingInfo();

    // ��Ī���ǿ� �´� ���� �����ϴ��� �˻�
    const MAPCODE lobbyMapCode = pLobbyRoom->GetMapCode();
    const DWORD lobbyUsers = pLobbyRoom->GetCurUserNum();
    const BYTE cMatchingLevelLimit = stServerOptionInfo.m_byMATCHING_LEVEL_LIMIT;

    FOREACH_CONTAINER(const ZONE_LIST::value_type& rNode, m_CHuntingRooms, ZONE_LIST)
    {
        const KEYTYPE key = rNode;
        const ObjectKeyStream::sNode* pNode = m_pObjectKeyStream->GetObjectInfo(key);
        if(!(pNode && pNode->pObject))
        {
            WAVERIX_ZONE_ASSERT(!"m_CHuntingRooms empty slot");
            continue;
        }
        CHuntingRoom* const pCRoom = CHuntingRoom_Info::downcast(pNode->pObject);
        // �̹� ���������� üũ
        if(pCRoom->IsCompetition())                         continue;
        // ��й��� ����
        if(pCRoom->GetZonePublic() == eZONEPUBLIC_PRIVATE)  continue;
        // �������÷� �ο��� �κ��ο��� ���ƾ� �� �� �ִ�.
        if(pCRoom->GetCurUserNum() != lobbyUsers)           continue;
        // �ʵ�, ��, ��� üũ
        if(lobbyMapCode != pCRoom->GetMapCode())            continue;

        const ADDITIONAL_CHUNTINGINFO* pCHuntingInfo = pCRoom->GetAdditionalCHuntingInfo();
        if(pLobbyInfo->m_Rule != pCHuntingInfo->m_Rule)     continue;
        if(pLobbyInfo->m_Mode != pCHuntingInfo->m_Mode)     continue;

        // ��շ��� üũ
        const INT avgLevel = (INT)pCRoom->GetAvgLevel() - (INT)LobbyAvgLevel;
        const BYTE LevelDiff = static_cast<BYTE>(std::abs(avgLevel));
        if(LevelDiff > cMatchingLevelLimit)                 continue;

        BYTE CRoomWinRatio = 0;
        DWORD CRoomTotCount = 0;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
        // ���� ��� PVP �波���� ��Ī�� �ȴ�.
        BOOL bCRoomGuildPVP = pCRoom->GetRoomInfo()->IsSpecialType(eROOM_SPECIAL_GUILD_PVP);
        if(bLobbyGuildPVP != bCRoomGuildPVP)                continue;
        if(bLobbyGuildPVP)
        {    // ���� ��峢���� ��Ī�� �ȵǰ� �Ѵ�.
            User* const pCHuntingMasterUser = pCRoom->GetMaster();
            if(pCHuntingMasterUser
                && (pMasterUser->GetSelectedCharGuildGuid()
                == pCHuntingMasterUser->GetSelectedCharGuildGuid())
            )                                               continue;
        }
        else
        {    // ��ս·� üũ
            pCRoom->GetCHuntingScore(CRoomWinRatio, CRoomTotCount);
            const BYTE WinRatioDiff = abs(CRoomWinRatio - LobbyAvgWinRatio);
            if(WinRatioDiff > MatchingCondition)            continue;
        }
#else
        pCRoom->GetCHuntingScore(CRoomWinRatio, CRoomTotCount);
        const BYTE WinRatioDiff = static_cast<BYTE>(std::abs(CRoomWinRatio - LobbyAvgWinRatio));
        if(WinRatioDiff > MatchingCondition)                continue;
#endif

        // ���� �濡 Ư���ð��� Ư��ȸ�� �̻��� ������ ���´�.
        if(pLobbyRoom->CheckAbusing(pCRoom))                continue;

        pCHuntingRoom = pCRoom;

        return TRUE;
    }

    return FALSE;
}


RC::eROOM_RESULT
ZoneManager::CreateCHuntingRoom(KEYTYPE lobbyKey, User* pMasterUser, CHuntingRoom*& OUT pCHuntingRoom)
{
    ZoneInterface* pRoom = FindZone(lobbyKey);
    if(!pRoom || pRoom->GetType() != eZONETYPE_LOBBY) 
        return RC::RC_ROOM_NOTEXISTLOBBY;
    LobbyRoom* pLobbyRoom = (LobbyRoom *)pRoom;
    if(pLobbyRoom->GetMaster() != pMasterUser)
        return RC::RC_ROOM_ISNOTMATERPLAYER;
    if(pLobbyRoom->GetLobbyType() != eZONETYPE_CHUNTING)
        return RC::RC_ROOM_INVALID_LASTROOMTYPE;
    if(!pLobbyRoom->IsAllReady())
        return RC::RC_ROOM_IS_NOT_ALL_READY;
    if(!pLobbyRoom->IsPrepareUserNum())
        return RC::RC_ROOM_INSUFFICIENT_USER_NUM;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    RC::eROOM_RESULT rcResult = pLobbyRoom->CanMigrateToChunting();
    if(rcResult != RC::RC_ROOM_SUCCESS)
        return rcResult;
#endif

    DWORD dwServerSessionIndex = ServerSessionManager::Instance()->Incr_LOF_BattleServer();
    if(dwServerSessionIndex == 0) 
        return RC::RC_ROOM_FAILED;

    ServerSession* pServerSession = (ServerSession*)ServerSessionManager::Instance()->FindServer(dwServerSessionIndex);
    if(!pServerSession || pServerSession->GetServerType() != BATTLE_SERVER)
    {
        SUNLOG(eFULL_LOG,  "��Ʋ������ �ƴ� ������ �����Ϸ��Ѵ�.2\n");
        return RC::RC_ROOM_FAILED;
    }

    KEYTYPE CHuntingKey = AllocKey();
    pCHuntingRoom = CHuntingRoom_Info::downcast(_AllocZone(CHuntingRoom_Info::value));
    RC::eROOM_RESULT rt = pCHuntingRoom->Migrate(CHuntingKey, pLobbyRoom, pServerSession);
    if(rt != RC::RC_ROOM_SUCCESS)
    {
        FreeKey(CHuntingKey);
        _FreeZone(pCHuntingRoom);
        return rt;
    }

    AddRoom(pCHuntingRoom);

    // ���� ��Ƽ���¿� ���� �濡 ������ ��Ƽ�� �����ϰų� �����Ѵ�.
    AgentPartyManager::Instance()->CreateZone(pMasterUser, pLobbyRoom, pCHuntingRoom);

    RoomArgument args(pLobbyRoom->GetZonePublic(), pLobbyRoom->GetRoomPWD());
    rt = MoveZone(pLobbyRoom, pCHuntingRoom, pMasterUser, args);
    ASSERT(rt == RC::RC_ROOM_SUCCESS);

    return RC::RC_ROOM_SUCCESS;
}



RC::eROOM_RESULT
ZoneManager::MigrateCHuntingRoom(User* pMasterUser, KEYTYPE lobbyKey, CHuntingRoom* IN pCHuntingRoom)
{
    ZoneInterface* pRoom = FindZone(lobbyKey);
    if(!pRoom || pRoom->GetType() != eZONETYPE_LOBBY) 
        return RC::RC_ROOM_NOTEXISTLOBBY;
    LobbyRoom* pLobbyRoom = (LobbyRoom *)pRoom;
    if(pLobbyRoom->GetMaster() != pMasterUser)
        return RC::RC_ROOM_ISNOTMATERPLAYER;
    if(pLobbyRoom->GetLobbyType() != eZONETYPE_CHUNTING)
        return RC::RC_ROOM_INVALID_LASTROOMTYPE;
    if(!pLobbyRoom->IsAllReady())
        return RC::RC_ROOM_IS_NOT_ALL_READY;
    if(!pLobbyRoom->IsPrepareUserNum())
        return RC::RC_ROOM_INSUFFICIENT_USER_NUM;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    RC::eROOM_RESULT rcResult = pLobbyRoom->CanMigrateToChunting();
    if(rcResult != RC::RC_ROOM_SUCCESS)
        return rcResult;
#endif

    if(!pCHuntingRoom)
        return RC::RC_ROOM_INVALID_CHUNTING_LINK_KEY;

    // �����߿��� �� �� ����.
    if(pCHuntingRoom->IsCompetition())
        return RC::RC_ROOM_DOING_COMPETITION;

    // �������÷� �ο��� �κ��ο��� ���ƾ� �� �� �ִ�.
    if(pCHuntingRoom->GetCurUserNum() != pLobbyRoom->GetCurUserNum())
        return RC::RC_ROOM_MEMBERNUM_OF_PARTY_IS_MANY;

    // �� ��° ��Ƽ�� ���ٰ� ����
    pCHuntingRoom->EnterTeam(pLobbyRoom, TRUE);

    // ��Ī�Ǵ� ��� SpecialType�� Ʋ���� Ŭ�� �˷��ش�.
    if(pLobbyRoom->GetRoomInfo()->m_SpecialType != pCHuntingRoom->GetRoomInfo()->m_SpecialType)
    {
        // Ŭ�� �˸���.
        MSG_CG_ZONE_CHANGE_SPECIALTYPE_CMD CGMsg;
        CGMsg.m_SpecialType = pCHuntingRoom->GetRoomInfo()->m_SpecialType;
        pLobbyRoom->SendToAll(&CGMsg, sizeof(CGMsg));
    }

    AgentPartyManager::Instance()->MigrateCHuntingRoom(pMasterUser, pLobbyRoom, pCHuntingRoom);

    // �κ��� ��������� �κ� ���� SYN, �� ���� SYN
    RoomMigrater<LobbyRoom, CHuntingRoom, TR_LEAVE_LOBBY_FOR_CHUNTING> op(pLobbyRoom, pCHuntingRoom);
    pLobbyRoom->Foreach(op);

    return RC::RC_ROOM_SUCCESS;
}

//_NA_000038_20100316_VIEW_TO_JOINABLE_ROOM_IN_LOBBY
RC::eROOM_RESULT ZoneManager::IsMoveableZone(CheckMoveZonePolicy* const check_move_zone_policy, 
                                             const RoomArgument& room_argument) const
{
    const RC::eROOM_RESULT enable_check_result = check_move_zone_policy->EnableCheck();
    if (enable_check_result != RC::RC_ROOM_SUCCESS)
    {
        return enable_check_result;
    }

    User* const user = check_move_zone_policy->Args->pUser;
    ZoneInterface* const to_zone = check_move_zone_policy->Args->ToZonePtr;

    const RC::eROOM_RESULT can_join_result = to_zone->CanJoin(user, room_argument.RoomExportLevel, room_argument.Password);
    if (can_join_result != RC::RC_ROOM_SUCCESS)
    {
        return can_join_result;
    }

    return RC::RC_ROOM_SUCCESS;
}

//==================================================================================================
//  <MoveZone>
//
//  <COMMENT>
//  - ���� MoveZone�� ����ʷ� �� ��, ������ ���������� �˻��ؼ� �̵���Ű�� ���´�.
//   ��, �� �̵� ���� ��å�� �������̶�� �Ҹ��̸�, �̴� �ϰ����� ũ�� ��ȭ��Ű�� �������� �Ǵ�.
//   (�߰��� �ߺ� �ڵ� �� �Ǽ� ���� ������ ������ ����.)
//  - �߾� �������� ������ ��ȯ�Ѵٸ� ���? ��... �ټ� ������ �־ ��å���� ������ �����ϴµ���
//   �ſ� ���� �� �ϴ�.
RC::eROOM_RESULT ZoneManager::MoveZone(CheckMoveZonePolicy* const check_move_zone_policy, 
                                       const RoomArgument& room_argument)
{
    // (CHANGES)(20100318)(hukim)(REQ_0038) �̵� ���� ���� �˻縦 �Ͽ�ȭ�ϱ� ���� ������ �޼����
    // �и��ߴ�.

    const RC::eROOM_RESULT is_moveable_zone_result = IsMoveableZone(check_move_zone_policy, room_argument);
    if (is_moveable_zone_result != RC::RC_ROOM_SUCCESS)
    {
        return is_moveable_zone_result;    
    }

    User* const user = check_move_zone_policy->Args->pUser;
    ZoneInterface* const from_zone = check_move_zone_policy->Args->FromZonePtr;
    ZoneInterface* const to_zone = check_move_zone_policy->Args->ToZonePtr;

    return MoveZone(from_zone, to_zone, user, room_argument);
}

RC::eROOM_RESULT
ZoneManager::MoveZone(ZoneInterface* const  pFromZone,
                      ZoneInterface* const  pToZone,
                      User* const           pUser,
                      const RoomArgument&   rRoomArg)
{
    if(pFromZone->GetLinkedServer() == pToZone->GetLinkedServer())
    {
        pFromZone->LeaveUser(pUser, eSEND_CMD, pToZone->GetType(), pToZone->GetLinkedServerIndex());
        pToZone->ReadyUser(pUser, eSEND_CMD, rRoomArg.ToFieldCode, rRoomArg.ToAreaID, rRoomArg.pStartPos);
    }
    else
    {
        pFromZone->LeaveUser(pUser, eSEND_SYN, pToZone->GetType(), pToZone->GetLinkedServerIndex());
    }

    return RC::RC_ROOM_SUCCESS;
}


RC::eROOM_RESULT
ZoneManager::KickUserFromLobby(User* pMaster, DWORD TargetPlayerKey, User*& OUT pTargetUser)
{
    // �κ��� �����ΰ�?    
    if(pMaster->GetZoneState() != ePRS_AT_LOBBY)
        return RC::RC_ROOM_INVALIDROOMTYPE;
    LobbyRoom* pLobbyRoom = FindLobby(pMaster->GetZoneKey());
    if(!pLobbyRoom) 
        return RC::RC_ROOM_NOTEXISTLOBBY;
    // �����̾���Ѵ�.
    if(pMaster != pLobbyRoom->GetMaster())
        return RC::RC_ROOM_NOT_CMD_FOR_MEMBER;

    pTargetUser = pLobbyRoom->FindUser(TargetPlayerKey);
    if(!pTargetUser)
        return RC::RC_ROOM_NOT_EXIST_MEMBER;

    // �κ񿡼� ������ ���� ��Ƽ�� �ҼӵǾ� ������ ������� ��Ų��.

    MAPCODE mcCode = pTargetUser->GetSelectedCharMapCode();
    ZoneInterface* pZone = g_ZoneManager.FindStaticZone(mcCode);
    if(!pZone) return RC::RC_ROOM_NOTEXISTVILLAGE;

    const sMAPINFO* pMAPINFO = MapInfoParser::Instance()->FindMapInfo(mcCode);
    if(!((eZONETYPE_VILLAGE == pMAPINFO->byMKind) || (eZONETYPE_TUTORIAL_FIELD == pMAPINFO->byMKind)))
        return RC::RC_ROOM_NOTEXISTVILLAGE;

    pLobbyRoom->LeaveUser(pTargetUser, eSEND_CMD, eZONETYPE_VILLAGE, pZone->GetLinkedServerIndex());
    pZone->ReadyUser(pTargetUser, eSEND_CMD);

    return RC::RC_ROOM_SUCCESS;
}


RC::eROOM_RESULT
ZoneManager::UserReadyInLobby(User* pUser, BOOL bReady)
{
    // �κ��� �����ΰ�?    
    if(pUser->GetZoneState() != ePRS_AT_LOBBY)
        return RC::RC_ROOM_INVALIDROOMTYPE;
    LobbyRoom* pLobbyRoom = FindLobby(pUser->GetZoneKey());
    if(!pLobbyRoom) 
        return RC::RC_ROOM_NOTEXISTLOBBY;

    // ������ �ƴϾ�� �Ѵ�.
    if(pUser == pLobbyRoom->GetMaster())
        return RC::RC_ROOM_NOT_CMD_FOR_MASTER;

    if(bReady == TRUE)
    {
        // ���� �ƴϾ�� �Ѵ�.
        if(pUser->IsReady())
        {
            return RC::RC_ROOM_ALREADY_READY_STATE;
        }
        pUser->SetReady(TRUE);
    }
    else
    {
        // �����̾�� �Ѵ�
        if(!pUser->IsReady())
        {
            return RC::RC_ROOM_ALREADY_NOT_READY_STATE;
        }
        pUser->SetReady(FALSE);
    }

    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT
ZoneManager::ChangeTeamInLobby(User* pUser, BYTE team)
{
    // �κ��� �����ΰ�?    
    if(pUser->GetZoneState() != ePRS_AT_LOBBY)
        return RC::RC_ROOM_INVALIDROOMTYPE;
    LobbyRoom* pLobbyRoom = FindPVPLobby(pUser->GetZoneKey());
    if(!pLobbyRoom) 
        return RC::RC_ROOM_NOTEXISTLOBBY;

    if(!pLobbyRoom->IsPVPTeamMode())
        return RC::RC_ROOM_INVALID_TEAM;
    if(1 != team && 2 != team)
        return RC::RC_ROOM_INVALID_TEAM;
    if(pUser->GetTeam() == team)
        return RC::RC_ROOM_INVALID_TEAM;

    pUser->SetTeam(team);

    return RC::RC_ROOM_SUCCESS;
}


RC::eROOM_RESULT
ZoneManager::SetNonblockSlotNumInLobby(User* pMaster, BYTE maxUser)
{
    // �κ��� �����ΰ�?    
    if(pMaster->GetZoneState() != ePRS_AT_LOBBY)
        return RC::RC_ROOM_INVALIDROOMTYPE;
    LobbyRoom* pLobbyRoom = FindLobby(pMaster->GetZoneKey());
    if(!pLobbyRoom) 
        return RC::RC_ROOM_NOTEXISTLOBBY;

    // �����̾���Ѵ�.
    if(pMaster!= pLobbyRoom->GetMaster())
        return RC::RC_ROOM_NOT_CMD_FOR_MEMBER;

    // �ι�° �������� �κ��� ��� �ɼ��� �ٲ��� ���Ѵ�.
    if(pLobbyRoom->GetAdditionalCHuntingInfo()->m_LinkHuntingKey)
        return RC::RC_ROOM_CANNOT_CHANGE_OPTION_ROOMTYPE;

    const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(pLobbyRoom->GetMapCode());
    if(!pInfo) return RC::RC_ROOM_INVALID_MAPCODE;
    if(pInfo->byMinUserNum > maxUser || pInfo->byMaxUserNum < maxUser)
        return RC::RC_ROOM_INVALID_LIMITUSERNUM;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    if(pLobbyRoom->GetRoomInfo()->IsSpecialType(eROOM_SPECIAL_GUILD_PVP))
        return RC::RC_ROOM_INVALID_LIMITUSERNUM;
#endif

    pLobbyRoom->SetLimitMaxUserNum(maxUser);
    if(maxUser > 10)
        SUNLOG(eCRITICAL_LOG, "���ο�10���ʰ�2(%u)\n", maxUser);

    return RC::RC_ROOM_SUCCESS;
}


RC::eROOM_RESULT
ZoneManager::ChangeMapInLobby(User* pMaster, MAPCODE mapCode)
{
    // �κ��� �����ΰ�?    
    if(pMaster->GetZoneState() != ePRS_AT_LOBBY)
        return RC::RC_ROOM_INVALIDROOMTYPE;
    LobbyRoom* pLobbyRoom = FindLobby(pMaster->GetZoneKey());
    if(!pLobbyRoom) 
        return RC::RC_ROOM_NOTEXISTLOBBY;
    // �����̾���Ѵ�.
    if(pMaster != pLobbyRoom->GetMaster())
        return RC::RC_ROOM_NOT_CMD_FOR_MEMBER;

    return pLobbyRoom->ChangeMap(mapCode);
}



RC::eROOM_RESULT
ZoneManager::ConfigHuntingRoomInLobby(User* pMaster, ADDITIONAL_ROOMINFO& IN additionalInfo)
{
    // �κ��� �����ΰ�?    
    if(pMaster->GetZoneState() != ePRS_AT_LOBBY)
        return RC::RC_ROOM_INVALIDROOMTYPE;
    LobbyRoom* pLobbyRoom = FindHuntingLobby(pMaster->GetZoneKey());
    if(!pLobbyRoom) 
        return RC::RC_ROOM_NOTEXISTLOBBY;
    // �����̾���Ѵ�.
    if(pMaster != pLobbyRoom->GetMaster())
        return RC::RC_ROOM_NOT_CMD_FOR_MEMBER;

    // ������ ��ȿ�Ѱ� üũ�� ����
    if(additionalInfo.m_Bonus >= eHUNTING_BONUS_TYPE_MAX) 
        return RC::RC_ROOM_INVALID_BONUS_VALUE;
    if(additionalInfo.m_Difficulty >= eHUNTING_MONSTER_DIFFICULT_MAX) 
        return RC::RC_ROOM_INVALID_DIFFICULTY_VALUE;

    pLobbyRoom->SetAdditionalRoomInfo(&additionalInfo);

    return RC::RC_ROOM_SUCCESS;
}


RC::eROOM_RESULT
ZoneManager::ConfigPVPRoomInLobby(User* pMaster, ADDITIONAL_PVPINFO& IN AdditionalPVPInfo)
{
    // �κ��� �����ΰ�?    
    if(pMaster->GetZoneState() != ePRS_AT_LOBBY)
        return RC::RC_ROOM_INVALIDROOMTYPE;
    LobbyRoom* pLobbyRoom = FindPVPLobby(pMaster->GetZoneKey());
    if(!pLobbyRoom) 
        return RC::RC_ROOM_NOTEXISTLOBBY;

    // �����̾���Ѵ�.
    if(pMaster != pLobbyRoom->GetMaster())
        return RC::RC_ROOM_NOT_CMD_FOR_MEMBER;

    // ������ ��ȿ�Ѱ� üũ�� ����
    if(AdditionalPVPInfo.m_Rule >= ePVP_RULE_MAX) 
        return RC::RC_ROOM_INVALID_RULE_VALUE;
    if(AdditionalPVPInfo.m_Mode >= ePVP_MODE_MAX) 
        return RC::RC_ROOM_INVALID_MODE_VALUE;

    if(pLobbyRoom->GetAdditionalPVPInfo()->m_Mode != AdditionalPVPInfo.m_Mode)//&&
//        AdditionalPVPInfo.m_Mode == ePVP_TEAM_MODE)
    {
        pLobbyRoom->SetAdditionalPVPInfo(&AdditionalPVPInfo);
        // �����̸�
        // ��� ������ team�� ������
        // �������� ���� brd
        pLobbyRoom->AssignTeam();
    }
    else
    {
        pLobbyRoom->SetAdditionalPVPInfo(&AdditionalPVPInfo);
    }

    return RC::RC_ROOM_SUCCESS;
}



RC::eROOM_RESULT
ZoneManager::ConfigCHuntingRoomInLobby(User* pMaster, ADDITIONAL_CHUNTINGINFO& IN additionalInfo)
{
    // �κ��� �����ΰ�?    
    if(pMaster->GetZoneState() != ePRS_AT_LOBBY)
        return RC::RC_ROOM_INVALIDROOMTYPE;
    LobbyRoom* pLobbyRoom = FindCHuntingLobby(pMaster->GetZoneKey());
    if(!pLobbyRoom) 
        return RC::RC_ROOM_NOTEXISTLOBBY;
    // �����̾���Ѵ�.
    if(pMaster != pLobbyRoom->GetMaster())
        return RC::RC_ROOM_NOT_CMD_FOR_MEMBER;

    // �ι�° �������� �κ��� ��� �ɼ��� �ٲ��� ���Ѵ�.
    if(pLobbyRoom->GetAdditionalCHuntingInfo()->m_LinkHuntingKey)
        return RC::RC_ROOM_CANNOT_CHANGE_OPTION_ROOMTYPE;

    // ������ ��ȿ�Ѱ� üũ�� ����
    if(additionalInfo.m_Bonus >= eHUNTING_BONUS_TYPE_MAX) 
        return RC::RC_ROOM_INVALID_BONUS_VALUE;
    if(additionalInfo.m_Difficulty >= eHUNTING_MONSTER_DIFFICULT_MAX) 
        return RC::RC_ROOM_INVALID_DIFFICULTY_VALUE;
    if(additionalInfo.m_Rule >= eCOMPETITION_RULE_MAX) 
        return RC::RC_ROOM_INVALID_RULE_VALUE;
    if(additionalInfo.m_Mode >= eCOMPETITION_MODE_MAX) 
        return RC::RC_ROOM_INVALID_MODE_VALUE;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    if(pLobbyRoom->GetRoomInfo()->IsSpecialType(eROOM_SPECIAL_GUILD_PVP))
    {
        if(additionalInfo.m_Rule != eCOMPETITION_RULE_DUAL_PARTY)
            return RC::RC_ROOM_INVALID_RULE_VALUE;
    }
#endif

    pLobbyRoom->SetAdditionalCHuntingInfo(&additionalInfo);

    return RC::RC_ROOM_SUCCESS;
}


struct SerializeLobbyRoomList
{
    template< ZoneManager::eLOBBY_HASH_IDX _IDX, typename _ROOMINFO >
    static VOID Execute(ZoneManager* const zone_manager, 
                        User* const user,
                        const BYTE max_room_count, 
                        BYTE& OUT room_count,
                        _ROOMINFO* OUT room_info_array)
    {
        BOOST_STATIC_ASSERT(static_cast<DWORD>(_IDX) < static_cast<DWORD>(ZoneManager::LHI_SIZE_MAX));
        
        room_count = 0;
        
        typedef typename ZoneManager::CACHE_LOBBY_LIST LOBBY_LIST;
        const LOBBY_LIST& lobby_list = zone_manager->m_LobbyRoomPerType[_IDX];
        
        FOREACH_CONTAINER(typename LOBBY_LIST::value_type& lobby_room_key, lobby_list, LOBBY_LIST)
        {
            const typename ZoneManager::ObjectKeyStream::sNode* lobby_room_node = 
                zone_manager->m_pObjectKeyStream->GetObjectInfo(lobby_room_key);
            if (!(lobby_room_node && lobby_room_node->pObject))
            {
                continue;
            }
            
            // (CHANGES)(20100317)(hukim)(REQ_0038) ����� �� ��ϸ� ��� ���� �濡 ���� ������
            // �ٸ� �÷ο��̹Ƿ� �� Ÿ�Կ� ���� �н����� �˻�� �����Ų��. 
            // �н����带 ������ �˻�� ���� �濡 ������ �� �˻�ȴ�.
            
            const LobbyRoom_Info::pointer_r lobby_room = LobbyRoom_Info::downcast(lobby_room_node->pObject);
            if (lobby_room->CanShowAtRoomList(user, lobby_room->GetZonePublic(), lobby_room->GetRoomPWD()) == TRUE)
            {
                lobby_room->SerializeLoad(room_info_array[room_count++]);
                if (max_room_count == room_count)
                {
                    return;
                }
            }
        }
    }
};


VOID
ZoneManager::_LobbyRoomListRefresh()
{
    // (f100617.3L) change to a more cheap routine
    m_LobbyRoomPerType[LHI_HUNTING  ].resize(0);
    m_LobbyRoomPerType[LHI_MISSION  ].resize(0);
    m_LobbyRoomPerType[LHI_PVP      ].resize(0);
    m_LobbyRoomPerType[LHI_CHUNTING ].resize(0);

    if(m_LobbyRooms.empty())
        return;

    FOREACH_CONTAINER(const ZONE_LIST::value_type& rNode, m_LobbyRooms, ZONE_LIST)
    {
        const KEYTYPE key = rNode;
        const ObjectKeyStream::sNode* pNode = m_pObjectKeyStream->GetObjectInfo(key);
        if(!(pNode && pNode->pObject))
            continue;

        LobbyRoom_Info::pointer_r pLobbyRoom = LobbyRoom_Info::downcast(pNode->pObject);
        switch(pLobbyRoom->GetLobbyType())
        {
        case eZONETYPE_HUNTING: m_LobbyRoomPerType[LHI_HUNTING  ].push_back(key); break;
        case eZONETYPE_MISSION: m_LobbyRoomPerType[LHI_MISSION  ].push_back(key); break;
        case eZONETYPE_PVP:     m_LobbyRoomPerType[LHI_PVP      ].push_back(key); break;
        case eZONETYPE_CHUNTING:m_LobbyRoomPerType[LHI_CHUNTING ].push_back(key); break;
        }
    }
}

VOID
ZoneManager::GetMissionLobbyRoomList(User* pUser, BYTE, BYTE MaxCount, BYTE& OUT Count, MISSION_ROOMINFO* OUT pRoomInfoArray)
{
    SerializeLobbyRoomList::Execute<ZoneManager::LHI_MISSION>(this, pUser, MaxCount, Count,
                                                              pRoomInfoArray);
}

VOID
ZoneManager::GetHuntingLobbyRoomList(User* pUser, BYTE, BYTE MaxCount, BYTE& OUT Count, HUNTING_ROOMINFO* OUT pRoomInfoArray)
{
    SerializeLobbyRoomList::Execute<ZoneManager::LHI_HUNTING>(this, pUser, MaxCount, Count,
                                                              pRoomInfoArray);
}

VOID    ZoneManager::GetPVPLobbyRoomList(User* pUser, BYTE, BYTE MaxCount, BYTE& OUT Count, PVP_ROOMINFO* OUT pRoomInfoArray)
{
    SerializeLobbyRoomList::Execute<ZoneManager::LHI_PVP>(this, pUser, MaxCount, Count,
                                                          pRoomInfoArray);
}

VOID    ZoneManager::GetCHuntingLobbyRoomList(User* pUser, BYTE, BYTE MaxCount, BYTE& OUT Count, CHUNTING_ROOMINFO* OUT pRoomInfoArray)
{
    SerializeLobbyRoomList::Execute<ZoneManager::LHI_CHUNTING>(this, pUser, MaxCount, Count,
                                                               pRoomInfoArray);
}

//    <��� ��ʰ� �̻��ϴ�.>
//
VOID    ZoneManager::GetCHuntingRoomList(User* pUser, BYTE, BYTE MaxCount, BYTE& OUT count, CHUNTING_ROOMINFO* OUT pRoomInfoArray)
{
    count = 0;
    FOREACH_CONTAINER(const ZONE_LIST::value_type& rNode, m_CHuntingRooms, ZONE_LIST)
    {
        const KEYTYPE key = rNode;
        const ObjectKeyStream::sNode* pNode = m_pObjectKeyStream->GetObjectInfo(key);
        if(!(pNode && pNode->pObject))
        {
            WAVERIX_ZONE_ASSERT(!"m_CHuntingRooms empty slot");
            continue;
        }

        CHuntingRoom_Info::pointer_r pCHuntingRoom = CHuntingRoom_Info::downcast(pNode->pObject);
        if(RC::RC_ROOM_SUCCESS != pCHuntingRoom->CanJoin(pUser, eZONEPUBLIC_PUBLIC, NULL))
            continue;

        // �̹� ��ũ �κ���� ����� ������ üũ
        if(pCHuntingRoom->GetLinkedLobby())
            continue;

        pCHuntingRoom->SerializeLoad(pRoomInfoArray[count++]);
        if(count == MaxCount)
            return;
    }
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
//���� ������� ��� PVP�� �� ����Ʈ�� �����ش�.
VOID
ZoneManager::GetGuildPVPRoomList(User* pUser, BYTE ReqCount, BYTE MaxCount, BYTE& OUT count, CHUNTING_ROOMINFO* OUT pRoomInfoArray)
{
    count = 0;
    FOREACH_CONTAINER(const ZONE_LIST::value_type& rNode, m_CHuntingRooms, ZONE_LIST)
    {
        const KEYTYPE key = rNode;
        const ObjectKeyStream::sNode* pNode = m_pObjectKeyStream->GetObjectInfo(key);
        if(!(pNode && pNode->pObject))
        {
            WAVERIX_ZONE_ASSERT(!"m_CHuntingRooms empty slot");
            continue;
        }

        CHuntingRoom_Info::pointer_r pCHuntingRoom = CHuntingRoom_Info::downcast(pNode->pObject);
        const RC::eROOM_RESULT rt = pCHuntingRoom->CanJoin(pUser, eZONEPUBLIC_PUBLIC, NULL);
        if(RC::RC_ROOM_SUCCESS != rt)
        {
            //���� �Ͽ��, ����Ʈ�� �������� ���� ���� �ٸ� ��忩�� ���� ���� �ϰ�, ����� ������ ���߶� �������� �Ѵ�.
            switch(rt)
            {
            case RC::RC_ROOM_CANNOT_JOIN_ROOM_FOR_DIFF_GUILD:
            case RC::RC_ROOM_SHORT_ENTRANCE_CARD_OF_GUILD_PVP:
                break;
            default:
                continue;
            }
        }

        // �̹� ��ũ �κ���� ����� ������ üũ
        if(pCHuntingRoom->GetLinkedLobby())
            continue;

        //��� PVP���� �ƴϸ� ����Ʈ�� �������� �ʴ´�.
        if(!pCHuntingRoom->IsSpecialType(eROOM_SPECIAL_GUILD_PVP))
            continue;

        pCHuntingRoom->SerializeLoad(pRoomInfoArray[count++]);
        if(count == MaxCount)
            return;
    }
}
#endif //__NA_000000_20070528_TAIWAN_GUILD_PVP


VOID
ZoneManager::GetHuntingRoomList(User* pUser, BYTE, BYTE MaxCount, BYTE& OUT Count,
                                HUNTING_ROOMINFO* OUT pRoomInfoArray)
{
    SerializeLobbyRoomList::Execute<ZoneManager::LHI_HUNTING>(this, pUser, MaxCount, Count, pRoomInfoArray);
}

RoomInterface*
ZoneManager::FindRecommendRoom(User* pUser, eZONETYPE RoomType)
{
    CACHE_LOBBY_LIST* pList = 0;
    switch(RoomType)
    {
    case eZONETYPE_PVP:     pList = &m_LobbyRoomPerType[ LHI_PVP      ]; break;
    case eZONETYPE_MISSION: pList = &m_LobbyRoomPerType[ LHI_MISSION  ]; break;
    case eZONETYPE_HUNTING: pList = &m_LobbyRoomPerType[ LHI_HUNTING  ]; break;
    case eZONETYPE_CHUNTING:pList = &m_LobbyRoomPerType[ LHI_CHUNTING ]; break;
    default:
        return NULL;
    };

    RoomInterface* pGeneralRoom = NULL;
    CACHE_LOBBY_LIST& rList = *pList;
    FOREACH_CONTAINER(const CACHE_LOBBY_LIST::value_type& rNode, rList, CACHE_LOBBY_LIST)
    {
        const KEYTYPE key = rNode;
        const ObjectKeyStream::sNode* pNode = m_pObjectKeyStream->GetObjectInfo(key);
        if(!(pNode && pNode->pObject))
            continue;

        LobbyRoom_Info::pointer_r pLobbyRoom = LobbyRoom_Info::downcast(pNode->pObject);
        const RC::eROOM_RESULT rt = pLobbyRoom->IsRecommendRoom(pUser);
        if(RC::RC_ROOM_SUCCESS_FOR_RECOMMEND_JOIN == rt)
            return pLobbyRoom;
        else if(RC::RC_ROOM_SUCCESS_FOR_GENERAL_JOIN == rt)
            pGeneralRoom = pLobbyRoom;
    }
    return pGeneralRoom;
}

//����������������������������������������������������������������������������������
//
//
VOID
ZoneManager::Process()
{
    ZONE_LIST::iterator it=m_TotalZones.begin(), itend=m_TotalZones.end();
    while(it != itend)
    {
        const KEYTYPE key = *it;
        const ObjectKeyStream::sNode* pNode = m_pObjectKeyStream->GetObjectInfo(key);
        if(!(pNode && pNode->pObject))
        {
            WAVERIX_ZONE_ASSERT(!"huh!");
            ++it;
            continue;
        }

        ZoneInterface* const pZoneInterface = pNode->pObject;
        if(FALSE == pZoneInterface->Process())
        {
            it = m_TotalZones.erase(it);
            DestroyZone(pZoneInterface);
            //    iterator ++ �� ���� �������� �켱������ �޶��� �� �ִ�. ���� �̸� ������ ������.
            //    ���������� m_TotalZones�� ���� �ش� Ű�� �����Ϸ��� �ص� �̹� ���ŵǾ����Ƿ� iterator�� ��ġ�� �����ϴ�.
            continue;
        }

        ++it;
    }

    if(m_UpdateTimer.IsExpired())
    {
        _LobbyRoomListRefresh();
    }
}


VOID
ZoneManager::DestroyZone(ZoneInterface* pZone)
{
    const KEYTYPE regedKey = (!pZone->IsReusedKey()) ? pZone->GetKey() : 0;
    const eZONETYPE ztTYPE = pZone->GetType();

    RemoveRoom(pZone);
    pZone->Destroy();

    switch(ztTYPE)
    {
#undef REG_ZONE_NODE
#define REG_ZONE_NODE(_typeinfo)    case _typeinfo::value:

    REG_ZONE_LIST();
        _FreeZone(pZone);
        break;

    case InstanceDungeon_Info::value: _FreeIndun(pZone); break;
    };

    if(regedKey)
        FreeKey(regedKey);
}

VOID
ZoneManager::AddRoom(ZoneInterface* const pRoom)
{
    const eZONETYPE ztTYPE = pRoom->GetType();
    const KEYTYPE zoneKey = pRoom->GetKey();
    if(!(zoneKey && ((DWORD)ztTYPE < eZONETYPE_MAX)))
    {
        WAVERIX_ZONE_ASSERT(!(zoneKey && ((DWORD)ztTYPE < eZONETYPE_MAX)));
        return;
    }

    if(FALSE == m_pObjectKeyStream->AddObjectInfo(zoneKey, pRoom, ZoneKeyStreamAddin(pRoom)))
    {
        WAVERIX_ZONE_ASSERT(0);
        return;
    }

    switch(ztTYPE)
    {
    case eZONETYPE_LOBBY:
        {    //    Lobby�� ���� ���� Ÿ������ ó���ؾ� �Ѵ�.
            m_LobbyRooms.insert(zoneKey);

            LobbyRoom* const pLobbyRoom = LobbyRoom_Info::downcast(pRoom);
            if(pLobbyRoom->GetLobbyType() == eZONETYPE_CHUNTING)
            {
                const KEYTYPE linkKey = pLobbyRoom->GetAdditionalCHuntingInfo()->m_LinkHuntingKey;
                m_CHuntingLobbyByCKey.insert(std::make_pair(linkKey, zoneKey));
            }
        }
        break;
    case eZONETYPE_CHUNTING:
        {
            m_CHuntingRooms.insert(zoneKey);
        }
        break;
    case eZONETYPE_VILLAGE:
    case eZONETYPE_DOMINATION_FIELD:
    case eZONETYPE_FIELD:
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    case eZONETYPE_SPA_FIELD:
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
        {
            const MAPCODE mapCode = pRoom->GetMapCode();
            m_StaticZones.insert(std::make_pair(mapCode, zoneKey));
        }
        break;
    case eZONETYPE_INSTANCE:
        {
            InstanceDungeon* const pInstanceDungeon = InstanceDungeon_Info::downcast(pRoom);
            switch(pInstanceDungeon->GetDungeonType())
            {
            //case INDUN_KIND_TOP_OF_TRIAL:
            //case INDUN_KIND_100LEVEL_QUEST:
            //    break;
            case INDUN_KIND_SS_QUEST:
                {
                    const MAPCODE mapCode = pRoom->GetMapCode();
                    m_StaticDungeons.insert(std::make_pair(mapCode, zoneKey));
                }
                break;
            }
        }
        break;
    };

    PortalManager::OnRoomEvent(pRoom, true);
    //
    ++m_ZoneCounts[ztTYPE];
    AddZone(zoneKey);

    return;
}


VOID
ZoneManager::RemoveRoom(ZoneInterface* const pRoom)
{
    const eZONETYPE ztTYPE = pRoom->GetType();
    const KEYTYPE zoneKey = pRoom->GetKey();
    if(!(zoneKey && ((DWORD)ztTYPE < eZONETYPE_MAX)))
    {
        WAVERIX_ZONE_ASSERT(!(zoneKey && ((DWORD)ztTYPE < eZONETYPE_MAX)));
        return;
    }

    if(FALSE == m_pObjectKeyStream->RemoveObjectInfo(zoneKey))
    {
        WAVERIX_ZONE_ASSERT(0);
        return;
    }

    switch(ztTYPE)
    {
    case eZONETYPE_LOBBY:
        {    //    Lobby�� ���� ���� Ÿ������ ó���ؾ� �Ѵ�.
            m_LobbyRooms.erase(zoneKey);

            LobbyRoom* const pLobbyRoom = LobbyRoom_Info::downcast(pRoom);
            if(pLobbyRoom->GetLobbyType() == eZONETYPE_CHUNTING)
            {
                const KEYTYPE linkKey = pLobbyRoom->GetAdditionalCHuntingInfo()->m_LinkHuntingKey;
                m_CHuntingLobbyByCKey.erase(linkKey);
            }
        }
        break;
    case eZONETYPE_CHUNTING:
        {
            m_CHuntingRooms.erase(zoneKey);
        }
        break;
    case eZONETYPE_VILLAGE:
    case eZONETYPE_DOMINATION_FIELD:
    case eZONETYPE_FIELD:
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    case eZONETYPE_SPA_FIELD:
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
        {
            const MAPCODE mapCode = pRoom->GetMapCode();
            m_StaticZones.erase(mapCode);
        }
        break;
    case eZONETYPE_INSTANCE:
        {
            InstanceDungeon* const pInstanceDungeon = InstanceDungeon_Info::downcast(pRoom);
            switch(pInstanceDungeon->GetDungeonType())
            {
                //case INDUN_KIND_TOP_OF_TRIAL:
                //case INDUN_KIND_100LEVEL_QUEST:
                //    break;
            case INDUN_KIND_SS_QUEST:
                {
                    const MAPCODE mapCode = pRoom->GetMapCode();
                    m_StaticDungeons.erase(mapCode);
                }
                break;
            }
        }
        break;
    };

    PortalManager::OnRoomEvent(pRoom, false);
    RemoveZone(zoneKey);
    --m_ZoneCounts[ztTYPE];

    return;
}

void
ZoneManager::ChangeBattlePlayersCount(RoomInterface* pRoom, DWORD number_of_players)
{
    if(pRoom == 0 || pRoom->IsRoomType() == 0)
        return;

    DWORD zone_type_flag = pRoom->GetType();
          zone_type_flag = 1 << zone_type_flag;
    if((PolicymentForRoom::MonsterAbilityChangableRoomsBit & zone_type_flag) == 0)
        return;

    MSG_AG_ZONE_CHANGE_MONSTER_ABILITY_CMD msg;
    msg.roomKey = pRoom->GetKey();
    msg.number_of_players = (WORD)number_of_players;
    pRoom->SendToLinkedServer(&msg, sizeof(msg));
}

DWORD
ZoneManager::GetNumberOfHuntingRooms()
{
    return /*GetCHuntingLobbyRoomCount() +*/ GetCHuntingRoomCount();
}

DWORD
ZoneManager::GetNumberOfMissionRooms()
{
    return /*GetMissionLobbyRoomCount()+*/ GetMissionRoomCount();
}

VOID
ZoneManager::Display(BOOL bLog)
{
    TCHAR* const format = _T("%-18s\t: (%3u) : User's# (%4u)%s");
    const TCHAR* delimcode = bLog ? "" : "\n";
    const CSunLog::eLOGOPTION option = bLog ? CSunLog::cOUTPUT_FILE : CSunLog::cOUTPUT_CONSOLE;

#undef REG_ZONE_NODE
#define REG_ZONE_NODE(_typeinfo)    \
    MessageOutEx(g_pSunLog, CSunLog::cCRITICAL_LOG, option, CSunLog::cCATE_DEFAULT,    \
        format, #_typeinfo, m_ZoneCounts[_typeinfo::value], m_ZoneUserCounts[_typeinfo::value], delimcode);

    MessageOutEx(g_pSunLog, CSunLog::cCRITICAL_LOG, option, CSunLog::cCATE_DEFAULT,
                 "====================================================");
    REG_ZONE_LIST();

    //    REG_INDUN_LIST(); <-�δ��� �������� �Ǿ� ���� �ʴ�.
    MessageOutEx(g_pSunLog, CSunLog::cCRITICAL_LOG, option, CSunLog::cCATE_DEFAULT,    \
                 format, "InstanceDungeon_Info",
                 m_ZoneCounts[InstanceDungeon_Info::value],
                 m_ZoneUserCounts[InstanceDungeon_Info::value], delimcode);

    MessageOutEx(g_pSunLog, CSunLog::cCRITICAL_LOG, option, CSunLog::cCATE_DEFAULT,
                 "====================================================");
}


VOID
ZoneManager::DisplayerZonePoolInfo()
{
    typedef util::CMemoryPoolFactory<LobbyRoom>    POOL_for_LOBBY;
    util::IVMemoryPool* pPool = m_PoolRouter.GetPool(eZONETYPE_LOBBY);
    POOL_for_LOBBY* pPF = DOWNCASTER_SELECT<POOL_for_LOBBY*>(pPool);

    printf("LOBBY %u %u %u %u\n",
           pPF->GetPoolBasicSize(), pPF->GetPoolExtendSize(),
           pPF->GetNumberOfBands(), pPF->GetAvailableNumberOfTypes());
}

VOID
ZoneManager::CreateMissionByLobby(User* request_user,
                                  const MSG_AG_ZONE_CAN_CREATE_ROOM_ACK* recv_msg)
{
    MSG_AG_ZONE_CAN_CREATE_ROOM_ACK::CheckArgMembers check_member = recv_msg->m_CheckArgMembers;

    MSG_CG_ZONE_MISSION_CREATE_NAK nak_msg;

    ZoneInterface* zone_interface = NULL;
    LobbyRoom* lobby_room = NULL;

    if(check_member.roomKey == request_user->GetZoneKey())
    {
        zone_interface = g_ZoneManager.FindZone(check_member.roomKey);
    }

    if(zone_interface && zone_interface->GetType() == eZONETYPE_LOBBY)
    {
        lobby_room = static_cast<LobbyRoom*>(zone_interface);
    }

    if(FALSE == (lobby_room && lobby_room->GetLobbyType() == eZONETYPE_MISSION))
    {
        nak_msg.m_dwErrorCode = RC::RC_ROOM_INVALID_ROOM_TYPE;
        request_user->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    if(lobby_room->IsAllReady() == FALSE)
    {
        nak_msg.m_dwErrorCode = RC::RC_ROOM_IS_NOT_ALL_READY;
        request_user->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    RoomInterface* room_interface = static_cast<RoomInterface*>(zone_interface);
    if (room_interface->ExceptionProcessByLobbyMember(recv_msg, request_user) == false)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Fail mission start");
        return;
    }

    request_user->BeginTransaction(TR_LEAVE_LOBBY_FOR_MISSION);

    MissionRoom* pRoom = NULL;
    RC::eROOM_RESULT rt = g_ZoneManager.CreateMissionRoom(check_member.roomKey, request_user, pRoom);
    if (RC::RC_ROOM_SUCCESS == rt)
    {
        request_user->SetTRRoomKey(pRoom->GetKey());
        RoomMigrater<LobbyRoom, MissionRoom, TR_LEAVE_LOBBY_FOR_MISSION> op(lobby_room, pRoom);
        lobby_room->Foreach(op);
    }
    else
    {
        request_user->EndTransaction();
        nak_msg.m_dwErrorCode = rt;
        request_user->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

//_NA_20110630_BATTLE_ZONE_RENEWAL
VOID
ZoneManager::ProcessCreatePVPRoom(User* request_user)
{
    AgentPartyManager* party_manager = AgentPartyManager::Instance();
    WORD party_key = request_user->GetPartyState().GetPartyKey();

    const PartyRoom* party_room = party_manager->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party room : %d", party_key);
        return;
    }

    BaseParty* party = const_cast<BaseParty*>(party_manager->FindParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return;
    }

    PVPRoom* pvp_room = NULL;

    ADDITIONAL_PVPINFO pvp_info;
    pvp_info.m_Rule = static_cast<BYTE>(ePVP_RULE_INFINITY);
    pvp_info.m_Mode = static_cast<BYTE>(party_room->GetPvPMode());

    MAPCODE map_code = party_room->GetMapCode();
    request_user->BeginTransaction(TR_LEAVE_FIELD_FOR_PARTY_PVP);
    RC::eROOM_RESULT result_code = g_ZoneManager.CreatePVPRoomByParty(request_user,
                                                                      pvp_room,
                                                                      map_code,
                                                                      party_room->GetPartyRoomType(),
                                                                      &pvp_info);
    if (result_code == RC::RC_ROOM_SUCCESS)
    {
        party_manager->ResetPartyRoomReadyState(request_user, party_key);
        party_manager->SetIsMissionStart(party_key, true);
        request_user->SetTRRoomKey(pvp_room->GetKey());
        RoomMigraterByParty<PVPRoom, TR_LEAVE_FIELD_FOR_PARTY_PVP> op(pvp_room);
        party->For_eachMembers(op);
    }
    else
    {
        MSG_CG_PARTY_ROOM_START_ACK nak_msg;
        request_user->EndTransaction();
        nak_msg.result_code_ = RC::RC_PARTY_FAIL_CREATE_ROOM;
        party_manager->SendPacketAllMember(party_key, &nak_msg, sizeof(nak_msg));
        party_manager->ResetPartyRoomReadyState(request_user, party_key);
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : CreatePVPRoom process result fail : %d", 
               result_code);
    }
}

VOID
ZoneManager::ProcessCreateInstanceDungeonRoom(User* request_user)
{
    AgentPartyManager* party_manager = AgentPartyManager::Instance();
    WORD party_key = request_user->GetPartyState().GetPartyKey();

    const PartyRoom* party_room = party_manager->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party room : %d", party_key);
        return;
    }

    BaseParty* party = const_cast<BaseParty*>(party_manager->FindParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return;
    }

    InstanceDungeon* Instance_dungeon = NULL;

    BOOL need_check_open_time = 
        (party_room->GetPartyRoomType() == PartyOfTopOfTrialByNormal) ? true : false;

    MAPCODE map_code = party_room->GetMapCode();
    request_user->BeginTransaction(TR_LEAVE_FIELD_FOR_PARTY_INSTANCEDUNGEON);
    RC::eROOM_RESULT result_code = 
        g_ZoneManager.CreateInstanceDungeonRoomByParty(request_user,
                                                       (InstanceDungeon *&)Instance_dungeon,
                                                       map_code,
                                                       party_room->GetPartyRoomType(),
                                                       INDUN_KIND_TOP_OF_TRIAL,
                                                       need_check_open_time);
    if (result_code == RC::RC_ROOM_SUCCESS)
    {
        party_manager->ResetPartyRoomReadyState(request_user, party_key);
        party_manager->SetIsMissionStart(party_key, true);
        request_user->SetTRRoomKey(Instance_dungeon->GetKey());
        g_ZoneManager.ChangeBattlePlayersCount(Instance_dungeon, party->GetMemberNum());
        RoomMigraterByParty<InstanceDungeon, TR_LEAVE_FIELD_FOR_PARTY_INSTANCEDUNGEON> op(Instance_dungeon);
        party->For_eachMembers(op);
    }
    else
    {
        MSG_CG_PARTY_ROOM_START_ACK nak_msg;
        request_user->EndTransaction();
        nak_msg.result_code_ = RC::RC_PARTY_FAIL_CREATE_ROOM;
        party_manager->SendPacketAllMember(party_key, &nak_msg, sizeof(nak_msg));
        party_manager->ResetPartyRoomReadyState(request_user, party_key);
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : CreateInstanceDungeonRoom process result fail : %d", 
               result_code);
    }
}

VOID
ZoneManager::ProcessCreateMissionRoom(User* request_user)
{
    AgentPartyManager* party_manager = AgentPartyManager::Instance();
    WORD party_key = request_user->GetPartyState().GetPartyKey();

    const PartyRoom* party_room = party_manager->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party room : %d", party_key);
        return;
    }

    BaseParty* party = const_cast<BaseParty*>(party_manager->FindParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return;
    }

    MissionRoom* party_mission_room = NULL;

    MAPCODE map_code = party_room->GetMapCode();
    request_user->BeginTransaction(TR_LEAVE_FIELD_FOR_PARTY_MISSION);
    RC::eROOM_RESULT result_code = 
        g_ZoneManager.CreateMissionRoomByParty(request_user, 
                                               party_mission_room,
                                               map_code,
                                               party_room->GetPartyRoomType());
    if (RC::RC_ROOM_SUCCESS == result_code)
    {
        party_manager->ResetPartyRoomReadyState(request_user, party_key);
        party_manager->SetIsMissionStart(party_key, true);
        request_user->SetTRRoomKey(party_mission_room->GetKey());
        g_ZoneManager.ChangeBattlePlayersCount(party_mission_room, party->GetMemberNum());
        RoomMigraterByParty<MissionRoom, TR_LEAVE_FIELD_FOR_PARTY_MISSION> op(party_mission_room);
        party->For_eachMembers(op);
    }
    else
    {
        MSG_CG_PARTY_ROOM_START_ACK nak_msg;
        request_user->EndTransaction();
        nak_msg.result_code_ = RC::RC_PARTY_FAIL_CREATE_ROOM;
        party_manager->SendPacketAllMember(party_key, &nak_msg, sizeof(nak_msg));
        party_manager->ResetPartyRoomReadyState(request_user, party_key);
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : CreateMissionRoom process result fail : %d", 
               result_code);
    }
}

RC::eROOM_RESULT
ZoneManager::CreatePVPRoomByParty(User* master_user, 
                                  PVPRoom*& OUT pvp_room, 
                                  MAPCODE map_code,
                                  PartyZoneType party_room_type,
                                  const ADDITIONAL_PVPINFO* additional_pvp_info)
{
    ServerSessionManager* const server_session_manager = ServerSessionManager::Instance();

    FlowControl flow;
    ServerSessionEx* const pFieldSession = _CheckRoomCountLimit(flow);
    if (pFieldSession == NULL)
    {
        return RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT;
    }

    const DWORD server_session_index = server_session_manager->Incr_LOF_BattleServer();
    if (server_session_index == 0)
    {
        return RC::RC_ROOM_FAILED;
    }

    ServerSession* server_session = 
        static_cast<ServerSession*>(ServerSessionManager::Instance()->FindServer(server_session_index));
    if (!server_session || server_session->GetServerType() != BATTLE_SERVER)
    {
        return RC::RC_ROOM_FAILED;
    }

    KEYTYPE pvp_room_key = AllocKey();
    pvp_room = PVPRoom_Info::downcast(_AllocZone(PVPRoom_Info::value));
    RC::eROOM_RESULT result_code = pvp_room->Create(master_user, 
                                                    map_code, 
                                                    pvp_room_key, 
                                                    server_session,
                                                    additional_pvp_info);
    if(result_code != RC::RC_ROOM_SUCCESS)
    {
        FreeKey(pvp_room_key);
        _FreeZone(pvp_room);
        return result_code;
    }

    AddRoom(pvp_room);
    ZoneInterface* current_zone = FindZone(master_user->GetZoneKey());
    if (current_zone == NULL)
    {
        return RC::RC_ROOM_FAILED;
    }

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const PartyRoom* party_room = 
        party_manager->FindPartyRoom(master_user->GetPartyState().GetPartyKey());
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Not found party room : key : %d", 
               master_user->GetPartyState().GetPartyKey());
        return RC::RC_ROOM_NOT_EXIST_ROOM;
    }

    MSG_AG_PARTY_PARTYROOM_ITEM_DELETE_CMD cmd_msg;
    cmd_msg.require_item_ = party_room->GetPartyRoomAdditionalInfo()->require_item_;
    cmd_msg.member_key_ = master_user->GetPlayerKey();
#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    cmd_msg.party_zone_type_ = BattlezoneMissionTreeParser::Instance()->GetType(map_code);
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    master_user->SendToLinkedServer(&cmd_msg, sizeof(cmd_msg));

    RoomArgument args(eZONEPUBLIC_PUBLIC);
    result_code = MoveZone(current_zone, pvp_room, master_user, args);
    if (result_code != RC::RC_ROOM_SUCCESS)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Party master fail move to mission room");
        return result_code;
    }

    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT
ZoneManager::CreateInstanceDungeonRoomByParty(User* master_user, 
                                              InstanceDungeon*& OUT Instance_dungeon, 
                                              MAPCODE map_code,
                                              PartyZoneType party_room_type,
                                              eINSTANCE_DUNGEON_KIND dungeon_kind, 
                                              BOOL need_check_open_time)
{
    ServerSessionManager* const server_session_manager = ServerSessionManager::Instance();

    FlowControl flow;
    ServerSessionEx* const pFieldSession = _CheckRoomCountLimit(flow);
    if (pFieldSession == NULL)
    {
        return RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT;
    }

    const DWORD server_session_index = server_session_manager->Incr_LOF_BattleServer();
    if (server_session_index == 0)
    {
        return RC::RC_ROOM_FAILED;
    }

    ServerSession* server_session = 
        static_cast<ServerSession*>(ServerSessionManager::Instance()->FindServer(server_session_index));
    if (!server_session || server_session->GetServerType() != BATTLE_SERVER)
    {
        return RC::RC_ROOM_FAILED;
    }

    KEYTYPE alloc_key = AllocKey();
    Instance_dungeon = InstanceDungeon_Info::downcast(_AllocIndun(dungeon_kind));
    if (Instance_dungeon->RegisterRuleset(map_code) == false)
    {
        return RC::RC_ROOM_FAILED;
    }
    RC::eROOM_RESULT result_code = Instance_dungeon->Create(master_user, 
                                                            map_code, 
                                                            alloc_key, 
                                                            server_session, 
                                                            need_check_open_time);
    if (result_code != RC::RC_ROOM_SUCCESS)
    {
        FreeKey(alloc_key);
        _FreeIndun(Instance_dungeon);
        return result_code;
    }

    AddRoom(Instance_dungeon);
    ZoneInterface* const current_zone = FindZone(master_user->GetZoneKey());
    if (current_zone == NULL)
    {
        return RC::RC_ROOM_FAILED;
    }

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const PartyRoom* party_room = 
        party_manager->FindPartyRoom(master_user->GetPartyState().GetPartyKey());
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Not found party room : key : %d", 
               master_user->GetPartyState().GetPartyKey());
        return RC::RC_ROOM_NOT_EXIST_ROOM;
    }

    MSG_AG_PARTY_PARTYROOM_ITEM_DELETE_CMD cmd_msg;
    cmd_msg.require_item_ = party_room->GetPartyRoomAdditionalInfo()->require_item_;
    cmd_msg.member_key_ = master_user->GetPlayerKey();
#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    cmd_msg.party_zone_type_ = BattlezoneMissionTreeParser::Instance()->GetType(map_code);
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    master_user->SendToLinkedServer(&cmd_msg, sizeof(cmd_msg));

    RoomArgument args(eZONEPUBLIC_PUBLIC);
    result_code = MoveZone(current_zone, Instance_dungeon, master_user, args);
    if (result_code != RC::RC_ROOM_SUCCESS)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Party master fail move to Instance room");
        return result_code;
    }
    
    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT
ZoneManager::CreateMissionRoomByParty(User* master_user, 
                                      MissionRoom*& OUT party_mission_room, 
                                      MAPCODE map_code,
                                      PartyZoneType party_room_type)
{
    ServerSessionManager* const server_session_manager = ServerSessionManager::Instance();

    FlowControl flow;
    ServerSessionEx* const pFieldSession = _CheckRoomCountLimit(flow);
    if (pFieldSession == NULL)
    {
        return RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_LIMIT;
    }

    const DWORD server_session_index = server_session_manager->Incr_LOF_BattleServer();
    if (server_session_index == 0)
    {
        return RC::RC_ROOM_FAILED;
    }

    ServerSession* server_session = 
        static_cast<ServerSession*>(ServerSessionManager::Instance()->FindServer(server_session_index));
    if (!server_session || server_session->GetServerType() != BATTLE_SERVER)
    {
        return RC::RC_ROOM_FAILED;
    }

    KEYTYPE mission_key = AllocKey();
    party_mission_room = MissionRoom_Info::downcast(_AllocZone(MissionRoom_Info::value));
    RC::eROOM_RESULT result_code = party_mission_room->Create(master_user, map_code, 
                                                              mission_key, server_session);
    if(result_code != RC::RC_ROOM_SUCCESS)
    {
        FreeKey(mission_key);
        _FreeZone(party_mission_room);
        return result_code;
    }

    AddRoom(party_mission_room);
    ZoneInterface* current_zone = FindZone(master_user->GetZoneKey());
    if (current_zone == NULL)
    {
        return RC::RC_ROOM_FAILED;
    }

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const PartyRoom* party_room = 
        party_manager->FindPartyRoom(master_user->GetPartyState().GetPartyKey());
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Not found party room : key : %d", 
               master_user->GetPartyState().GetPartyKey());
        return RC::RC_ROOM_NOT_EXIST_ROOM;
    }

    MSG_AG_PARTY_PARTYROOM_ITEM_DELETE_CMD cmd_msg;
    cmd_msg.require_item_ = party_room->GetPartyRoomAdditionalInfo()->require_item_;
    cmd_msg.member_key_ = master_user->GetPlayerKey();
#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    cmd_msg.party_zone_type_ = BattlezoneMissionTreeParser::Instance()->GetType(map_code);
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    master_user->SendToLinkedServer(&cmd_msg, sizeof(cmd_msg));

    RoomArgument args(eZONEPUBLIC_PUBLIC);
    result_code = MoveZone(current_zone, party_mission_room, master_user, args);
    if (result_code != RC::RC_ROOM_SUCCESS)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Party master fail move to mission room");
        return result_code;
    }

    return RC::RC_ROOM_SUCCESS;
}


#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
SpaField* ZoneManager::FindSpaField( const MAPCODE map_code ) const
{
    const DWORD STATIC_ZONE_MASK = (1<<eZONETYPE_SPA_FIELD);

    ZoneInterface* const pZoneInterface = FindStaticZone(map_code);
    if(pZoneInterface)
    {
        const DWORD zoneTypeBit = 1 << pZoneInterface->GetType();
        if(STATIC_ZONE_MASK & zoneTypeBit)
            return SpaField_Info::downcast(pZoneInterface);
    }
    return NULL;
}

RC::eROOM_RESULT ZoneManager::CreateSpaField( MAPCODE mapCode, ZoneInterface* OUT spa_field )
{
    ServerSession* pServerSession = (ServerSession*)ServerSessionManager::Instance()->GetFieldServer();
    if(!pServerSession)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"] �ʵ弭���� �ƴ� ������ �����Ϸ��Ѵ�.!\n");
        return RC::RC_ROOM_FAILED;
    }

    SpaField_Info::pointer_r pField = SpaField_Info::downcast(_AllocZone(SpaField_Info::value));

    // NOTE: f110520.5L, static fields use the reserved unique key.
    const MapInfoParser* const map_parser = MapInfoParser::Instance();
    const sMAPINFO* map_info = map_parser->FindMapInfo(mapCode);
    if (FlowControl::FCAssert(map_info != NULL) == false) {
        return RC::RC_ROOM_FAILED;
    };
    const KEYTYPE alloced_key = map_info->reserved_key_index_;

    pField->Create(alloced_key, mapCode, pServerSession);
    AddRoom(pField);

    if(spa_field)
        spa_field = pField;

    return RC::RC_ROOM_SUCCESS;
}

#endif //_NA_008069_20150210_ADD_SPA_SYSTEM