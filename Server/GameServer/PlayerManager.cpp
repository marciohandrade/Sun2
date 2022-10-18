#include "stdafx.h"
#include "./PlayerManager.h"

#include <Macro.h>
#include <PacketStruct_DG.h>
#include <ServerSession.h>
#include <ServerOptionParserEx.h>

#include "Player.h"
#include "ObjectFactory.h"
#include "StatusManager.h"
#include "GameZone.h"

#include "WareHouseSlotContainer.h"
#include "GuildWareHouseSlotContainer.h"
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
#include "AccumulatePoint.h"
#endif

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "./GameShop/GameShop.Manager.h"
#endif
//==================================================================================================
//==================================================================================================
//==================================================================================================
#if SUN_CODE_DELETE_RESERVED
// __PLAYER_OBJECT_VERIFY_CODE__
static struct sPlayerVerifyCodes
{
    DWORD dwCodeOfPlayer;
    DWORD dwCodeOfSCPlayer;
    DWORD dwCodeOfIGridObject;
} s_PlayerVerifyCodes;

static union s_UNIONPLAYERCODE
{
    Player* player;
    Character*   pCharacter;
    //SCPlayer*  pSCPlayer;
    IGridObject* pIGridObject;
    //
    DWORD* pdwVerifyCode;
} s_PCODE;

static Player* s_pActivePlayerInUpdate = NULL;

static BOOL GenerateObjectOfPlayer()
{
    Player* player = new Player();
    s_PCODE.player = player;
    s_PlayerVerifyCodes.dwCodeOfPlayer = *s_PCODE.pdwVerifyCode;

    //s_PCODE.pSCPlayer = dynamic_cast<SCPlayer*>(player);
    //s_PlayerVerifyCodes.dwCodeOfSCPlayer = *s_PCODE.pdwVerifyCode;

    s_PCODE.pIGridObject = dynamic_cast<IGridObject*>(player);
    s_PlayerVerifyCodes.dwCodeOfIGridObject = *s_PCODE.pdwVerifyCode;

    if (!s_PlayerVerifyCodes.dwCodeOfPlayer)
        return false;

    delete player;

    return true;
}

Player* Player::VerifyNRestoreTHIS(Player* player)
{
    DWORD dwResult = 0;

    // 플레이어가 유효한 메모리 상태인지 체크1
    if (IsBadWritePtr(player, sizeof(*player)))
    {
        SUNLOG(eCRITICAL_LOG, _T("Invalid MemPtr %08X\n"), (DWORD)(__int64)player);
        return NULL;
    }

    if (eVERIFY_MAGICCODE_INIT != player->m_StartCheckCode)
        dwResult |= 0x01;
    if (eVERIFY_MAGICCODE_INIT != player->m_StartInitCode)
        dwResult |= 0x02;
    if (eVERIFY_MAGICCODE_INIT != player->m_EndCheckCode)
        dwResult |= 0x04;
    if (eVERIFY_MAGICCODE_INIT != player->m_EndInitCode)
        dwResult |= 0x08;

    s_PCODE.player = player;
    if (s_PlayerVerifyCodes.dwCodeOfPlayer != *s_PCODE.pdwVerifyCode)
    {
        dwResult |= 0x10;
        *s_PCODE.pdwVerifyCode = s_PlayerVerifyCodes.dwCodeOfPlayer;
    }

    s_PCODE.pIGridObject = dynamic_cast<IGridObject*>(player);
    if (s_PlayerVerifyCodes.dwCodeOfIGridObject != *s_PCODE.pdwVerifyCode)
    {
        dwResult |= 0x20;
        *s_PCODE.pdwVerifyCode = s_PlayerVerifyCodes.dwCodeOfIGridObject;
    }

    //s_PCODE.pSCPlayer = dynamic_cast<SCPlayer*>(player);
    //if (s_PlayerVerifyCodes.dwCodeOfSCPlayer != *s_PCODE.pdwVerifyCode)
    //{
    //  dwResult |= 0x40;
    //  *s_PCODE.pdwVerifyCode = s_PlayerVerifyCodes.dwCodeOfSCPlayer;
    //}

    if (dwResult)
    {
        MessageOutEx(g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_DEFAULT,
            _T("Corrupted Object Restoration REASON [%08X]"), dwResult);
        return NULL;
    }

    return player;
}
// __PLAYER_OBJECT_VERIFY_CODE__
#endif //SUN_CODE_DELETE_RESERVED
//==================================================================================================
//==================================================================================================
//==================================================================================================

//==================================================================================================
//

PlayerManager::PlayerManager()
    : sector_delay_interval_(0)
    , object_key_stream_(new ObjectKeyStream())
{
    static BOOL bRet = GenerateObjectOfPlayer();
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    m_attendance_day = 0;
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
}

PlayerManager::~PlayerManager()
{
    SAFE_DELETE(object_key_stream_);
}

//==================================================================================================
//
//


void PlayerManager::Update(const util::ServerTimeInfoPerFrame* time_info)
{
    // sector delay interval (0.8 second)
    const DWORD kUpdateDelayTime = 800;
    ;{
        sector_delay_interval_ += time_info->delta_tick_;
        m_dwDeltaTick = time_info->delta_tick_;
    };
    ;{
        m_SectorUpdateOnUpdate = false;
        if (sector_delay_interval_ > kUpdateDelayTime)
        {
            m_SectorUpdateOnUpdate = true;
            sector_delay_interval_ -= kUpdateDelayTime;
        }
    };
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    AccumulatePoint::Calculate_Current_Year(time_info->delta_tick_);
#endif

    for (PLAYER_HASHMAP::iterator it = user_guid_table_.begin(); it != user_guid_table_.end(); ++it)
    {
        Player* const player = it->second;
        if (player == NULL || player->IsDeleted())
        {
            continue;
        }
        GameZone* entered_zone = NULL;
        MAPCODE entered_field_code = 0;
        const eZONE_STATE room_state = player->GetGameZone(entered_zone, entered_field_code);
        if (room_state & ePRS_AT_ZONE) 
        {
            player->Update(time_info, m_SectorUpdateOnUpdate);
        };
    }

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    AttendanceUpdate(time_info);
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
}

//─────────────────────────────────────────────────────────────────────
//
//

void PlayerManager::Disconnect(Player* player)
{
    //_NA_008506_20150923_LOG_LOGOUT_IN_GAMESERVER
    GAMELOG->LogLogout(player);

    if (player->IsDeleted())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|Player is already deleted.|UserGuid = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            player->GetUserGuid()
        );
    }
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    GameShop::Manager::Instance()->CloseShop(player);
#endif
    ReleasePlayer(player);
    RemovePlayer(player);
}


void PlayerManager::ReleasePlayer(Player* player)
{
    if (player->IsDeleted())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|Player is already deleted.|UserGuid = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            player->GetUserGuid()
        );
    }
    GameZone * pEnterZone = NULL;
    MAPCODE EnterFieldCode = 0;

    eZONE_STATE eRoomState = player->GetGameZone(pEnterZone, EnterFieldCode);
    if (eRoomState & ePRS_AT_ZONE)
    {
        pEnterZone->Leave(player);
        if ((eRoomState == ePRS_AT_FIELD || eRoomState == ePRS_AT_DOMINATION_FIELD)
            && player->GetHP() == 0 
            && player->GetPreVillageMapCode() != 0)     //현재 필드에서 죽은 상태에서 종료한다면..
            player->SetStartMapCode(player->GetPreVillageMapCode());    //이전마을 위치로 저장한다.
    }

    MSG_DG_CHARINFO_UPDATE_TRANSACTION_CONTROL_SYN msg_transaction;
    msg_transaction.m_byTransControl = msg_transaction.DBUPDATE_LOCKING;
    msg_transaction.char_guid_ = player->GetCharGuid(); // CHANGES: f110412.2L
    player->SendToGameDBPServer(&msg_transaction, sizeof(msg_transaction));

    player->SerializeInfoToDBProxy(UPDATE_FACTOR_LOGOUT);

    // 로그아웃시 인벤토리 정보 로그 남기기.
    GAMELOG->LogSnapShot(player);

    MSG_DG_CHARINFO_DISCONNECT_CMD CharInfoMsg;
    CharInfoMsg.m_ChannelID = g_pGameServer->GetKey().GetChannelID();
    player->SendToGameDBPServer(&CharInfoMsg, sizeof(CharInfoMsg));
}

//==================================================================================================
//
void PlayerManager::AllDisconnect()
{
    //  Agent와 연결이 끊어지는 경우에만 사용된다. 
    ObjectFactory* pObjectFactory = ObjectFactory::Instance();
    FOREACH_CONTAINER(const PLAYER_HASHMAP::value_type& rNode, user_guid_table_, PLAYER_HASHMAP)
    {
        Player* const player = rNode.second;
        ReleasePlayer(player);
        pObjectFactory->FreeObject(player);
    }

    object_key_stream_->Clear();
    user_guid_table_.clear();
    char_guid_table_.clear();
    char_name_table_.clear();
}

//─────────────────────────────────────────────────────────────────────
// <통계 관련>
//

// 사용자 평균 보유 금액 구한다
MONEY PlayerManager::GetAveUserTotalMoney() const
{
    MONEY ui64AveUserMoney = 0;
    ULONGLONG ui64UserNum = (ULONGLONG)user_guid_table_.size();

    // 0으로 나누기 방지
    if (0 == ui64UserNum)
    {
        return 0;
    }

    ui64AveUserMoney = GetUserTotalMoney();
    ui64AveUserMoney = MONEY(ui64AveUserMoney / ui64UserNum);

    return ui64AveUserMoney;
}

MONEY PlayerManager::GetUserTotalMoney() const
{
    MONEY total_money = 0;
    for (PLAYER_HASHMAP::const_iterator it = user_guid_table_.begin(); 
        it != user_guid_table_.end(); ++it)
    {
        const Player* const player = it->second;
        if (player == NULL || player->IsDeleted())
        {
            continue;
        }
        total_money += player->GetMoney();
    }
    return total_money;
}

// 유저 평균 통화량 계산
MONEY PlayerManager::GetAveWarehouseTotalMoney() const
{
    MONEY ui64AveUserMoney = 0;
    ULONGLONG ui64UserNum = (ULONGLONG)user_guid_table_.size();

    // 0으로 나누기 방지
    if (0 == ui64UserNum)
    {
        return 0;
    }

    ui64AveUserMoney = GetWarehouseTotalMoney();
    ui64AveUserMoney = MONEY(ui64AveUserMoney / ui64UserNum);

    return ui64AveUserMoney;
}

MONEY PlayerManager::GetWarehouseTotalMoney() const
{
    MONEY total_money = 0;
    for (PLAYER_HASHMAP::const_iterator it = user_guid_table_.begin(); 
        it != user_guid_table_.end(); ++it)
    {
        const Player* const player = it->second;
        if (player == NULL || player->IsDeleted())
        {
            continue;
        }
        if (player->GetWarehouseContainer()->IsStarted())
        {
            total_money += player->GetWarehouseContainer()->GetCurMoney();
        }
    }
    return total_money;
}

//==================================================================================================
//
//
bool PlayerManager::AddPlayer(Player* player)
{
    // 플레이어의 생성 주기 시작
    player->Init();

    const DWORD ERR_INVALID_KEY          = 10;
    const DWORD ERR_SYNCHRONIZE_FAULT    = 20;
    const DWORD ERR_ALREADY_EXIST_PLAYER = 30;

    const DWORD user_guid = player->GetUserKey();
    const DWORD object_key = player->GetObjectKey();

    FlowControl flow;
    FLOWCONTROL_START()
    {
        // start LOGIC_BLOCK
        IfFailThenBreak(user_guid && object_key, ERR_INVALID_KEY);

        const ObjectKeyStream::sNode* object_node = object_key_stream_->GetObjectInfo(object_key);
        IfFailThenBreak(object_node && object_node->pObject == 0, ERR_SYNCHRONIZE_FAULT);

        IfFailThenBreak(user_guid_table_.find(user_guid) == user_guid_table_.end(),
                        ERR_ALREADY_EXIST_PLAYER);

        user_guid_table_.insert(PLAYER_HASHMAP::value_type(user_guid, player));
        object_key_stream_->AddObjectInfo(object_key, player, PlayerObjectKeyStreamAddin());

        return true;
    }
    FLOWCONTROL_END;

    switch (flow.GetErrorCode())
    {
    case ERR_INVALID_KEY:
    case ERR_SYNCHRONIZE_FAULT:
    case ERR_ALREADY_EXIST_PLAYER:
        SUNLOG(eCRITICAL_LOG, _T("[%s][Fail][UserKey:%u] %s"),
               __FUNCTION__, user_guid, flow.GetErrorString());
        break;
    default:
        SUNLOG(eCRITICAL_LOG, _T("[%s][Fail] Unexpected Result = %s"),
               __FUNCTION__, flow.GetErrorString());
        break;
    }

    return false;
}


//==================================================================================================

Player* PlayerManager::FindPlayerByName(const char* char_name) const
{
    //__NA003373_20090402_NOT_FIND_ENGLISHNAME_BUG
    char lower_char_name[MAX_CHARNAME_LENGTH + 1];
    strncpy(lower_char_name, char_name, _countof(lower_char_name));
    lower_char_name[_countof(lower_char_name) - 1] = '\0';
    _strlwr(lower_char_name);

    ulong hash_value = util::StreamHashKey::GetStringHashKey(lower_char_name);
    PLAYER_NAME_HASH::const_iterator found_it = char_name_table_.find(hash_value);
    if (found_it == char_name_table_.end()) {
        return NULL;
    };
    const DWORD object_key = found_it->second;
    const ObjectKeyStream::sNode* object_node = object_key_stream_->GetObjectInfo(object_key);
    if (object_node == NULL) {
        return NULL;
    };
    Player* const player = object_node->pObject;
    if (player != NULL && player->IsDeleted())
    {
        return NULL;
    }
    return player;
    //
}

bool PlayerManager::AddPlayerName(Player* player)
{
    player->GetLoadingCheckTimer().SetTimer(
        ServerOptionParserEx::Instance()->GetServerOption().m_wABUSE_MIN_CLIENT_LOADING_TIME);

    const DWORD user_guid = player->GetUserKey();
    const char* char_name = player->GetCharName();
    //----------------------------------------------------------------------------------------------
    // CharGuid도 존재
    const DWORD object_key = player->GetObjectKey();
    const DWORD char_guid = player->GetCharGuid();
    if (object_key_stream_->UpdateObjectAddinInfo(object_key,
            PlayerObjectKeyStreamAddin(char_guid)) == false)
    {
        SUNLOG(eFULL_LOG, "[%s]PlayerCharGuid[%s][%u][charguid:%u]",
               __FUNCTION__, char_name, user_guid, char_guid);
        return false;
    }
    char_guid_table_[char_guid] = object_key;
    //----------------------------------------------------------------------------------------------
    char lower_char_name[MAX_CHARNAME_LENGTH + 1];
    strncpy(lower_char_name, char_name, _countof(lower_char_name));
    lower_char_name[_countof(lower_char_name) - 1] = '\0';
    _strlwr(lower_char_name);

    const ulong hash_value = util::StreamHashKey::GetStringHashKey(lower_char_name);
    player->char_name_hash_ = 0;
    PLAYER_NAME_HASH::iterator found_it = char_name_table_.find(hash_value);
    if (found_it != char_name_table_.end())
    {
        SUNLOG(eCRITICAL_LOG, "[%s][CharGuid=%u]PlayerName exist [%s]",
               __FUNCTION__, user_guid, char_name);
        return false;
    };
    player->char_name_hash_ = hash_value;
    char_name_table_[hash_value] = object_key;
    return true;
}

//==================================================================================================

bool PlayerManager::RemovePlayer(Player* player)
{
    const DWORD ERR_INVALID_USERKEY = 10;
    const DWORD ERR_CANT_FIND_USER  = 20;

    // 플레이어의 생성 주기 삭제
    const DWORD user_guid = player->GetUserKey();
    const DWORD object_key = player->GetObjectKey();
    const DWORD char_guid = player->GetCharGuid();

    FlowControl flow;
    FLOWCONTROL_START()
    {
        // start LOGIC_BLOCK
        IfFailThenBreak(user_guid && object_key, ERR_INVALID_USERKEY);

        PLAYER_HASHMAP::iterator found_it = user_guid_table_.find(user_guid);
        IfFailThenBreak(found_it != user_guid_table_.end(), ERR_CANT_FIND_USER);

        user_guid_table_.erase(found_it);
        object_key_stream_->RemoveObjectInfo(object_key);
        if (char_guid) {
            char_guid_table_.erase(char_guid);
        };
        const DWORD char_name_hash = player->GetCharNameHash();
        if (char_name_hash) {
            char_name_table_.erase(char_name_hash);
        };
        ObjectFactory::Instance()->FreeObject(player);

        return true;
    }
    FLOWCONTROL_END;
    //----------------------------------------------------------------------------------------------
    switch (flow.GetErrorCode())
    {
    case ERR_INVALID_USERKEY: case ERR_CANT_FIND_USER:
        SUNLOG(eCRITICAL_LOG, _T("[%s][Fail][UserKey:%u] '%s'"),
               __FUNCTION__, user_guid, flow.GetErrorString());
        break;
    default:
        SUNLOG(eCRITICAL_LOG, _T("[%s][Fail] Unexpected Result = '%s'"),
               __FUNCTION__, flow.GetErrorString());
        break;
    }

    return false;
}

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
VOID PlayerManager::SendPacketToAllPlayer(MSG_BASE_FORWARD* msg, size_t size)
{
    if (GetNumberOfPlayers() == 0) {
        return ;
    }

    PLAYER_HASHMAP::iterator itr = user_guid_table_.begin();

    while (itr != user_guid_table_.end() )
    {
        Player* player = itr->second;
        if (player == NULL) {
            return;
        }

        player->SendPacket(msg, size);
        ++itr;
    }
}
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
void PlayerManager::AttendanceUpdate(const util::ServerTimeInfoPerFrame* time_info)
{
    if (m_attendance_day != time_info->system_time.wDay)
    {
        m_attendance_day = time_info->system_time.wDay;
        for (PLAYER_HASHMAP::iterator it = user_guid_table_.begin(); it != user_guid_table_.end(); ++it)
        {
            Player* const player = it->second;
            if (player == NULL || player->IsDeleted())
            {
                continue;
            }
            if (player->GetExtraInfoManager().GetAttendanceManager().Attend(
                time_info->system_time.wYear,
                time_info->system_time.wMonth,
                time_info->system_time.wDay - 1,
                player) == true)
            {
                MSG_CG_CHARINFO_ATTENDANCE_INFO_PACKET msg;
                memcpy(&msg.attendance_infos, 
                    &player->GetExtraInfoManager().GetAttendanceManager().GetAttendanceInfos(), 
                    sizeof(msg.attendance_infos));
                player->SendPacket(&msg, sizeof(msg));
            }
        }
    }
}
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM