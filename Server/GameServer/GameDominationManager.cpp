#include "StdAfx.h"
#include <PacketStruct_GZ.h>
#include <PacketStruct_CG.h>
#include "GameDominationManager.h"
#include "DominationInfoParser.h"
#include "GameDominationField.h"
#include "GameZoneManager.h"
#include "SkillAttrCalculator.h"
#include "Player.h"
#include "StatusManager.h"
#include "GameGuildManager.h"
#include "GameGuild.h"
#include "GameGuildMember.h"
#include <time.h>
#include <SolarDateTime.h>
#include "GameZoneManager.h"
#include "ItemManager.h"
#include "ResultCode.h"
#include "DominationArea.h"
#include "DominationAuction.h"
#include "ServerOptionParserEx.h"

#ifdef _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY
#include "SlotManager.h"
#endif

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
#include "MapInfoParser.h"
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
#ifdef _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
#include "GameInstanceDungeon.h"
#endif // _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD

GameDominationManager::GameDominationManager()
:is_initialize_(false)
//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
,is_init_member_(false)
{
    domination_update_timer_.SetTimer(1000*5);
}

GameDominationManager::~GameDominationManager()
{
    _DeleteArea();
}

void 
GameDominationManager::_DeleteArea()
{
    AreaInfoMapIterator begin_iterator = area_info_map_.begin(),
                        end_iterator = area_info_map_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        DominationArea* domination_area = begin_iterator->second;
        delete domination_area;
    }

    area_info_map_.clear();
}

void 
GameDominationManager::SetAuctionData(MAPCODE map_code, const DominationAuctionInfo* auction_info, 
                                      int data_count)
{    
    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }

    DominationAuction* auction = domination_area->GetAuction();
    if (auction == NULL)
    {
        return;
    }
    auction->SetInfo(auction_info, data_count);
}

bool 
GameDominationManager::UpdateAuctionData(Player* player, MAPCODE map_code, 
                                         const DominationAuctionInfo& db_auction_info)
{
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Player not found");
        return false;
    }

    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return false;
    }

    DominationAuction* auction = domination_area->GetAuction();
    if (auction == NULL)
    {
        return false;
    }
    eDOMINATION_TENDER_TYPE type = auction->EntryAuction(db_auction_info);

    if (type == NEW_TENDER_COST)
    {
        MONEY money;
    #ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        money = DominationInfoParser::Instance()->GetAreaLimitMoney(map_code);
    #else
        money = db_auction_info.m_ActionInfo.m_TenderCost;
    #endif
        player->MinusMoney(money);
        GAMELOG->LogMoney(MONEY_DOMINATION_ACTIONMONEY_APPLY, player, money);
    }
    else if (type == DUP_TENDER_COST)
    {
    #ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        GUILDGUID guild_guid = db_auction_info.m_ActionInfo.m_GuilGuid;

        MONEY existing_money = GetTenderCost(map_code, guild_guid),
              apply_money = db_auction_info.m_ActionInfo.m_TenderCost;

        if (existing_money >= apply_money)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__":Action money incorrect : %d , %d",
                   map_code, db_auction_info.m_ActionInfo.m_GuilGuid);
            return false;
        }

        //경매 차액 = 새로 추가할 경매 금액 - 기존 경매 금액
        player->MinusMoney(apply_money - existing_money);
        GAMELOG->LogMoney(MONEY_DOMINATION_ACTIONMONEY_APPLY, player, money);
    #endif
    }
    return true;
}

MONEY
GameDominationManager::GetTenderCost(MAPCODE map_code, GUILDGUID guild_guid)const
{
    const DominationArea* domination_area = FindAreaInfo(map_code);
    if (domination_area == NULL)
    {
        // 맵코드는 여기에 진입전에 반드시 사전검사 되어야 함수의 논리적 의미가 정확해 진다.
        // 여기에 진입했다면 진입 경로를 검토해 보아야 한다.
        SUNLOG(eCRITICAL_LOG, __FUNCTION__"Not found domination area info : %d", map_code);
        return 0;
    }

    const DominationAuction* auction = domination_area->GetAuction();
    if (auction == NULL)
    {
        return 0;
    }
    return auction->GetTenderCost(guild_guid);
}

MONEY
GameDominationManager::GetBestTenderCost(MAPCODE map_code)const
{
    const DominationArea* domination_area = FindAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return 0;
    }

    const DominationAuction* auction = domination_area->GetAuction();
    if (auction == NULL)
    {
        return 0;
    }
    return auction->GetBestTenderCost();
}

const DominationArea*
GameDominationManager::FindAreaInfo(MAPCODE map_code)const
{
    ConstAreaInfoMapIterator find_iterator = area_info_map_.find(map_code);
    if (find_iterator == area_info_map_.end())
    {
        SUNLOG(eCRITICAL_LOG,__FUNCTION__"Not found domination area info : %d", map_code);

        return NULL;
    }

    return find_iterator->second;
}

DominationArea*
GameDominationManager::GetAreaInfo(MAPCODE map_code)
{
    AreaInfoMapIterator find_iterator = area_info_map_.find(map_code);
    if (find_iterator == area_info_map_.end())
    {
        assert(!"Not found domination area info");
        SUNLOG(eCRITICAL_LOG,__FUNCTION__"Not found domination area info : %d", map_code);
        return NULL;
    }

    return find_iterator->second;
}

void 
GameDominationManager::LoadProcess()
{
    if (_IsLoadedScript() == false)
    {
        SUNLOG(eCRITICAL_LOG,__FUNCTION__" : Script data is not yet loaded");
        return;
    }

    if (is_initialize_ == false)
    {
        _Initialize();
    }

    (_CheckLoadedStatus()) ? domination_update_timer_.DisableCheckTime() : _RequestAreaInfoLoad();
}

void 
GameDominationManager::_Initialize()
{
    is_initialize_ = true;
    _CreateArea();
}

bool 
GameDominationManager::_CheckLoadedStatus()
{
    ConstAreaInfoMapIterator begin_iterator = area_info_map_.begin(),
                             end_iterator = area_info_map_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const DominationArea& domination_area = *(begin_iterator->second);
        if (domination_area.IsLoaded() == false)
        {
            return false;
        }
    }
    
    SUNLOG(eCRITICAL_LOG, __FUNCTION__":Complete domination data load");
    return true;
}

void 
GameDominationManager::_InsertDomiationInfo()
{
    ConstAreaInfoMapIterator begin_iterator = area_info_map_.begin(),
                             end_iterator = area_info_map_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const DominationArea& domination_area = *(begin_iterator->second);
        const DOMINATION_INFO* domination_info = &(domination_area.GetInfo());
        MAPCODE map_code = domination_area.GetMapCode();

        GameDominationField* domination_field = 
            static_cast<GameDominationField*>(g_GameZoneManager.GetFieldVillageZone(map_code));
        if (domination_field == NULL)
        {
            SUNLOG(eCRITICAL_LOG,__FUNCTION__"Not found domination field : %d", map_code);
            continue;
        }
        domination_field->InsertDominationInfo(domination_info);
    }
}

void
GameDominationManager::_RequestAreaInfoLoad()const
{
    ConstAreaInfoMapIterator begin_iterator = area_info_map_.begin(),
                             end_iterator = area_info_map_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const DominationArea& domination_area = *(begin_iterator->second);
        if (domination_area.IsLoaded() == false)
        {
            domination_area.DataLoad();
        }
    }
}

void GameDominationManager::_CreateArea()
{
    const ScriptDataMap& script_infos = 
        DominationInfoParser::Instance()->GetDominationInfoMap();

    for (ScriptDataMap::const_iterator it = script_infos.begin(); it != script_infos.end(); ++it)
    {
        const MAPCODE map_code = it->first;
        DominationArea* area = new DominationArea(map_code);
        area_info_map_[map_code] = area;
    }
}

void 
GameDominationManager::_SetDominationInfo(const DOMINATION_INFO& domination_info)
{
    const MAPCODE map_code = domination_info.m_FieldCode;

    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    domination_area->SetInfo(domination_info, _GetCurrentDominationEvent(map_code));
#else
    domination_area->SetInfo(domination_info);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
}

void 
GameDominationManager::_UpdateDominationInfo(const DOMINATION_INFO& domination_info)
{
    const MAPCODE map_code = domination_info.m_FieldCode;

    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }

    domination_area->UpdateInfo(domination_info);
}

bool
GameDominationManager::_IsAvailableAuctionArea(MAPCODE map_code)const
{
    ConstAreaInfoMapIterator find_iterator = area_info_map_.find(map_code);

    if (find_iterator == area_info_map_.end())
    {
        assert(!"Is not auction area");
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Is not auction area : %d", map_code);
        return false;
    }

    return true;
}

RC::eITEM_RESULT 
GameDominationManager::RewardItem(Player* player, int amount_cost, SLOTCODE item_code)
{
    if (player == NULL)
    {
        assert(!"Player Not Found");
        return RC::RC_ITEM_FAILED;
    }

    MSG_CG_ITEM_OBTAIN_ITEM_CMD send_msg;

    RC::eITEM_RESULT result_code = 
        player->GetItemManager()->Obtain(item_code, amount_cost, &send_msg.m_tagInventory);

    if (result_code != RC::RC_ITEM_SUCCESS)
    {
        return result_code;
    }

    player->SendPacket(&send_msg, send_msg.GetSize());

    return result_code;
}

//_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
bool
GameDominationManager::RemovePointItem(Player* player, SLOTCODE item_code)
{
    MSG_CG_ITEM_LOSE_ACK ack_msg;
    {
        ItemManager* item_manager = player->GetItemManager();
        const POSTYPE item_num = 1;

        RC::eITEM_RESULT result_value = 
            item_manager->DeleteAllKindOfItem(1, &item_code, &item_num, &(ack_msg.m_ItemInfo));

        if (result_value != RC::RC_ITEM_SUCCESS)
        {
            return false;
        }
    }
    player->SendPacket( &ack_msg, ack_msg.GetSize() );

    return true;
}

bool 
GameDominationManager::IncreaseDeffenseWallPoint(Player* player, 
                                                 SLOTCODE item_code, 
                                                 MAPCODE map_code)
{
    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return false;
    }

    if (player == NULL)
    {
        assert(!"Player Not Found");
        SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]Player Not Found");
        return false;
    }

    //아이템 유효성 검사
    const ItemInfoParser* const item_info_parser = ItemInfoParser::Instance();
    const BASE_ITEMINFO* const point_item =  item_info_parser->GetItemInfo(item_code);

    if (point_item->m_wType != eITEMTYPE_DOMINATION_DEFFENSE_POINT &&
        point_item->m_wType != eITEMTYPE_DOMINATION_DEFFENSE_CASH_POINT)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not same point item type[code:%d]", item_code);
        return false;
    }

    //아이템 삭제
    if (RemovePointItem(player, item_code) != true)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Fail delete add point item[code:%d]", item_code);
        return false;
    }

    //포인트 증가및 정보 업데이트

    DOMINATION_INFO& domination_info = domination_area->GetInfo();

    ++(domination_info.m_TotalPoint);

    MSG_GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN send_msg;
    send_msg.m_dwKey = player->GetCharGuid();
    send_msg.domination_info_ = domination_info;
    send_msg.item_code_ = item_code;

    if (g_pGameServer->SendToServer(GUILD_SERVER, &send_msg, sizeof(send_msg)) == false)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not connected to guild server");
        return false;
    }

    return true;
}

RC::eDOMINATION_RESULT
    GameDominationManager::CanIncreaseDeffenseWallPoint(Player* player, MAPCODE map_code)
{
    if (player == NULL)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    //트렌젝션 체크
    const DominationArea* domination_area = FindAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    if (domination_area->CanIncreaseDeffenseWall() == false)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    //길드 유효성
    GameGuildMember* guild_member = _GetGuildMemberInfo(player);
    if (guild_member == NULL)
    {
        return RC::DOMINATION_RESULT_NOT_GUILDMEMBER;
    }
    else if (guild_member->GetDuty() != eGUILD_DUTY_MASTER)
    {
        return RC::DOMINATION_RESULT_NOT_GUILDMASTER;
    }

    const DOMINATION_INFO* domination_info = GetDominationInfo(map_code);
    if (domination_info == NULL)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }
    else if (player->GetGuildGuid() != domination_info->m_inGuildInfo.m_GuildGuid)
    {
        return RC::DOMINATION_RESULT_NOT_GUILDMEMBER;
    }

    //방벽 포인트 유효성
    int total_point = _GetDefenseWallTotalPoint(map_code);
    const BYTE max_point = MAX_DOMINATION_DEFENSEWALL_NUM * DOMINATION_DEFFENSWALL_POINT;
    if (total_point < 0 || total_point >= max_point)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    return RC::DOMINATION_RESULT_SUCCESS;
}

void
GameDominationManager::_InitDefenseWall(MAPCODE map_code)
{
    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }

    DOMINATION_INFO& info = domination_area->GetInfo();
    info.m_DeffenseWallA = 0;
    info.m_DeffenseWallB = 0;
    info.m_DeffenseWallC = 0;
    info.m_TotalPoint = 5;
}

RC::eDOMINATION_RESULT
GameDominationManager::CanSaveGateState(Player* player, MAPCODE map_code, const BYTE* deffens_wall)
{
    if (player == NULL)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    GameGuildMember* guild_member = _GetGuildMemberInfo(player);
    if (guild_member == NULL)
    {
        //해당 케릭터가 길드에 속하여 있어야 한다.
        return RC::DOMINATION_RESULT_NOT_GUILDMEMBER;
    }
    else if (guild_member->GetDuty() != eGUILD_DUTY_MASTER)
    {
        //해당 케릭터가 길드 마스터가 아니다.
        return RC::DOMINATION_RESULT_NOT_GUILDMASTER;
    }

    bool check_defense_wall = CheckDefenseWall(map_code, deffens_wall);
    if (check_defense_wall == false)
    {
        //저장하려는 방벽 데이터가 사용할 수 있는 총 포인트보다 높다.
        return RC::DOMINATION_RESULT_DEFFENSEWALL_OVERPOINT;
    }

    const DOMINATION_INFO* domination_info = GetDominationInfo(map_code);
    if (domination_info == NULL)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    const DOMINATION_BASIC_GUILD_INFO& guild_info = domination_info->m_inGuildInfo;
    if (_strnicmp(guild_info.m_tszGuildMaster, player->GetCharName(), 
        _countof(guild_info.m_tszGuildMaster) != 0))
    {
        //해당 지역 마스터가 아니다.
        return RC::DOMINATION_RESULT_NOT_GUILDMASTER;
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

    return RC::DOMINATION_RESULT_SUCCESS;
}


void 
GameDominationManager::ResponseAuctionApply(Player* player, MAPCODE map_code)const
{
    MSG_CG_DOMINATION_APPLY_ACK ack_msg;

    eDOMINATION_EVENT_STATE current_event = _GetCurrentDominationEvent(map_code);
    if (_IsApplyAuction(player, current_event) == true)
    {
        ack_msg.m_ResultCode = RC::DOMINATION_RESULT_SUCCESS;
        ack_msg.m_CheckBatting = _CanAuctionBatting(player, map_code);
        ack_msg.m_DataNum = _GetAuctionList(map_code, ack_msg.m_ActionList, _countof(ack_msg.m_ActionList));
    }
    else
    {
        ack_msg.m_ResultCode = RC::DOMINATION_RESULT_FAIL;
        ack_msg.m_DataNum = 0;
    }    
    player->SendPacket(&ack_msg, ack_msg.GetSize());
}

void 
GameDominationManager::SendDominationNotice(Player* player, MAPCODE map_code)
{
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not found player");
        return;
    }

    const DOMINATION_INFO* domination_info = GetDominationInfo(map_code);
    if (domination_info == NULL)
    {
        return;
    }

    //클라이언트 측에서 guild_guid 가 0일 경우에 따로 처리 해야 하는 로직이 존재
    MSG_CG_DOMINATION_NOTICE_ACK ack_msg;
    ack_msg.m_ResultCode = RC::DOMINATION_RESULT_SUCCESS;
    ack_msg.m_GuildInfo.m_GuildGuid = 0;
    DOMINATION_GUILD_INFO& guild_info = ack_msg.m_GuildInfo;
    eDOMINATION_EVENT_STATE current_event = _GetCurrentDominationEvent(map_code);
    if (current_event > DOMINATION_EVENT_AUCTION_START && current_event < DOMINATION_EVENT_END)
    {   
        const DOMINATION_BASIC_GUILD_INFO& dis_guild_info = domination_info->m_disGuildInfo;
        guild_info.m_GuildGuid = dis_guild_info.m_GuildGuid;;   
        guild_info.m_PatternIndex = dis_guild_info.m_PatternIndex;
        guild_info.m_BackGroundIndex = dis_guild_info.m_BackGroundIndex;
        _tcsncpy(guild_info.m_tszGuildName, dis_guild_info.m_tszGuildName, 
                 _countof(guild_info.m_tszGuildName));
        guild_info.m_tszGuildName[_countof(guild_info.m_tszGuildName) - 1] = '\0';
        _tcsncpy(guild_info.m_tszGuildMaster, dis_guild_info.m_tszGuildMaster, 
                 _countof(guild_info.m_tszGuildMaster));
        guild_info.m_tszGuildMaster[_countof(guild_info.m_tszGuildMaster) - 1] = '\0';
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        guild_info.progress_state_ = domination_info->progress_state_;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
    }

    CheckNotice(player, map_code, ack_msg);
    player->SendPacket(&ack_msg, sizeof(ack_msg));

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    SendMemberList(player, map_code);
}

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
RC::eDOMINATION_RESULT
GameDominationManager::CanEntryAuctionCost(Player* player, MAPCODE map_code, int entry_money, 
                                           POSTYPE pos) const
{
    if (player == NULL)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    if (player->GetBehave() != PLAYER_BEHAVE_IDLE_STATE)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

#ifdef _NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST
    GameGuild* guild = g_GameGuildManager.FindGuild(player->GetGuildGuid());
    if (guild == NULL)
    {
        SUNLOG(
            eFULL_LOG, 
            __FUNCTION__" : Not found guild info for current player : guid = %d",
            player->GetCharGuid()
            );
        return RC::DOMINATION_RESULT_FAIL;
    }

    const sDOMINATION_INFO* const parser_info = DominationInfoParser::Instance()->GetDominationInfo(map_code);
    if (parser_info == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Not found script data : map code = %d",
            map_code);
        return RC::DOMINATION_RESULT_FAIL;;
    }

    BYTE check_guild_level = parser_info->m_BattingGuildLevelLimit;

    if (check_guild_level > guild->GetGuildGrade())
    {
        return RC::DOMINATION_RESULT_AUCTION_LOW_LEVEL;
    }
#endif

    //현지역 경매 데이터 갯수 검사
    const DominationArea* domination_area = FindAreaInfo(map_code);
    const DominationAuction* auction = domination_area->GetAuction();
    if (auction->CountAuctionList() > MAX_DOMINATION_AUCTION)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    //경매 지역 맵 코드 검사
    if (_IsAvailableAuctionArea(map_code) == false)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    //입찰 아이템 검사
    bool check_token = CheckDominationTokken(player, entry_money, pos, map_code);
    if (check_token == false)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    //길드 맴버 정보
    GameGuildMember *guild_member = _GetGuildMemberInfo(player);
    if (guild_member == NULL)
    {
        return  RC::DOMINATION_RESULT_NOT_GUILDMEMBER;
    }
    else if (guild_member->GetDuty() != eGUILD_DUTY_MASTER)
    {
        return RC::DOMINATION_RESULT_NOT_GUILDMASTER;
    }

    //현재 이벤트
    eDOMINATION_EVENT_STATE current_event = _GetCurrentDominationEvent(map_code);
    if (_IsApplyAuction(player, current_event) == FALSE )
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    //기획 변경 : 한주에 한지역 입찰 -> 한주에 여러지역 입찰
    //입찰 내역 검사
    //MAPCODE applied_map_code = _DidApplyGuild(player->GetGuildGuid());
    //if (applied_map_code != 0 && applied_map_code != map_code)
    //{
    //  return RC::DOMINATION_RESULT_CAN_BETTING_ONLYONE_AREA;
    //}

    //지역 기본 입찰금
    MONEY base_cost = DominationInfoParser::Instance()->GetAreaLimitMoney(map_code);

    if (player->GetMoney() < base_cost )
    {
        //기존 입찰금이 있는 경우에는 추가 비용이 들지 않는다.
        MONEY tender_cost = GetTenderCost(map_code, player->GetGuildGuid());
        if(tender_cost == 0)
        {
            return RC::DOMINATION_RESULT_NOT_ENOUGHMONEY;
        }
    }

    return RC::DOMINATION_RESULT_SUCCESS;
}
#else//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY
RC::eDOMINATION_RESULT
GameDominationManager::CanEntryAuctionCost(Player* player, MAPCODE map_code, MONEY entry_money) const
{
    if (player == NULL)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    if (player->GetBehave() != PLAYER_BEHAVE_IDLE_STATE)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    //현지역 경매 데이터 갯수 검사
    const DominationArea* domination_area = FindAreaInfo(map_code);
    const DominationAuction* auction = domination_area->GetAuction();
    if (auction->CountAuctionList() > MAX_DOMINATION_AUCTION)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    //경매 지역 맵 코드 검사
    if (_IsAvailableAuctionArea(map_code) == false)
    {
        return RC::DOMINATION_RESULT_FAIL;
    }

    GameGuildMember* guild_member = _GetGuildMemberInfo(player);
    if (guild_member == NULL)
    {
        //해당 케릭터가 길드에 속하여 있어야 한다.
        return  RC::DOMINATION_RESULT_NOT_GUILDMEMBER;
    }
    else if (guild_member->GetDuty() != eGUILD_DUTY_MASTER)
    {
        //해당 케릭터가 길드 마스터가 아니다.
        return RC::DOMINATION_RESULT_NOT_GUILDMASTER;
    }

    const MONEY limit_money = DominationInfoParser::Instance()->GetAreaLimitMoney(map_code);
    if(limit_money > entry_money)
    {
        //배팅한 하임이 기본 경매 금액보다 작다.
        return RC::DOMINATION_RESULT_LIMIT_ENOUGHMONEY;
    }

    const MONEY best_money = GetTenderCost(map_code);
    if (best_money >= entry_money)
    {
        //배팅한 하임이 지역별 최고 금액보다 적다.
        return RC::DOMINATION_RESULT_BEST_ENOUGHMONEY;
    }

    const eDOMINATION_EVENT_STATE current_event = _GetCurrentDominationEvent(map_code);
    if (_IsApplyAuction(player, current_event) == FALSE)
    {
        //기타 다른 경매에 참여할수 없는 이유로 실패 했다.
        return RC::DOMINATION_RESULT_FAIL;
    }

    MONEY tender_cost = GetTenderCost(map_code, player->GetGuildGuid());
    if ((player->GetMoney() + tender_cost) < entry_money)
    {
        //배팅한 하임이 소지 금액보다 많다.
        return RC::DOMINATION_RESULT_NOT_ENOUGHMONEY;
    }

    return RC::DOMINATION_RESULT_SUCCESS;
}
#endif//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY

bool 
GameDominationManager::CheckDominationTokken(Player* player, int amount_token, 
                                             POSTYPE pos, MAPCODE map_code) const
{
    const SLOTCODE item_code = DominationInfoParser::Instance()->GetBattingItemCodeByMapCode(map_code);
#ifdef _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY
    ItemManager* const item_manager = player->GetItemManager();
    const int amount_of_inven = item_manager->ExistItem(SI_INVENTORY, item_code, amount_token);
    if (amount_of_inven < amount_token)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|Amount of inventory is shortage.|number of request = %d, amount of inventory = %d|"), 
            __FUNCTION__, 
            amount_token, 
            amount_of_inven
        );
        return false;
    }
#else//_NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY
    SCSlotContainer * inventory_container = 
        player->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);
    if (!inventory_container->ValidPos(pos) || 
        inventory_container->IsEmpty(pos)   ||
        inventory_container->IsLocked(pos))
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not available inventory container");
        return false;
    }

    SCItemSlot& slot_info = (SCItemSlot&)inventory_container->GetSlot(pos);
    SLOTCODE request_cost_type = slot_info.GetItemInfo()->m_Code;
    if (request_cost_type != item_code)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not available token type");
        return false;
    }

    BYTE user_token_count = slot_info.GetNum();
    if (user_token_count != amount_token)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not available token count");
        return false;
    }
#endif//_NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY

    return true;
}

WORD 
GameDominationManager::GetAreaCount(const GUILDGUID guild_guid)
{
    AreaInfoMapIterator begin_iterator = area_info_map_.begin(),
        end_iterator = area_info_map_.end();

    WORD number_of_domination_map = 0;

    for(;begin_iterator != end_iterator; ++begin_iterator)
    {
        const DominationArea& domination_area = *(begin_iterator->second);
        const DOMINATION_INFO& info = domination_area.GetInfo();

        if (info.m_inGuildInfo.m_GuildGuid == guild_guid)
        {
            ++number_of_domination_map;
        }
    }

    return number_of_domination_map;
}

//_NA_20100604_BETTER_DOMINATION_MANAGER 
void 
GameDominationManager::UpdateGuildInfoByDomination(const GUILDGUID guild_guid, 
                                                   const eDOMINATION_ADD_GUILD_INFO_TYPE type)
{
    if (guild_guid == 0)
    {
        return;
    }

    MSG_GZ_DOMINATION_UPDATE_GUILDINFO_SYN send_msg;

    send_msg.guild_guid_ = guild_guid;

    //-- 정보의 증가치는 무조건 1이다.
    send_msg.domination_join_count_ = !!(type & (1<<0));
    send_msg.domination_defense_count_ = !!(type & (1<<1));
    send_msg.domination_hunting_count_ = !!(type & (1<<2));

    g_pGameServer->SendToServer(GUILD_SERVER, &send_msg, sizeof(send_msg));
}

bool
GameDominationManager::_IsLoadedScript()const
{
    bool is_loaded = DominationInfoParser::Instance()->IsLoadComplete();

    return is_loaded;
}

bool 
GameDominationManager::_CanAuctionBatting(Player* player, MAPCODE map_code)const
{
    if (player == NULL)
    {
        return false;
    }

    eDOMINATION_EVENT_STATE current_event = _GetCurrentDominationEvent(map_code);

    //경매 가능여부
    bool check_auction_event = _IsApplyAuction(player, current_event);
    if (check_auction_event == false)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Is not auction event time[event:%d]", current_event);
        return false;        
    }

    //길드원이여야 함
    GameGuildMember* game_guild_member = _GetGuildMemberInfo(player);
    if (game_guild_member == NULL)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Is not guild member[%s]", player->GetCharName());
        return false;
    }

    //마스터만 가능
    if (game_guild_member->GetDuty() != eGUILD_DUTY_MASTER)
    {
        SUNLOG( eFULL_LOG, "["__FUNCTION__"]Is not guild master[%s]",player->GetCharName());
        return false;
    }

    //점령길드 마스터 불가능
    const DOMINATION_INFO* current_info = GetDominationInfo(map_code);
    if (current_info == NULL)
    {
        SUNLOG( eFULL_LOG, "["__FUNCTION__"]Not found to domination Info[%d]",map_code );
        return false;
    }

    const DOMINATION_BASIC_GUILD_INFO& guild_info = current_info->m_inGuildInfo;
    if (guild_info.m_GuildGuid == player->GetGuildGuid())
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Same to guild by current area of owner guild[guild:%s]",
               guild_info.m_tszGuildName);
        return false;
    }

    return true;
}



void 
GameDominationManager::_RemoveDisGuildInfo(MAPCODE map_code, bool is_swap)
{
    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }

    DOMINATION_INFO& current_info = domination_area->GetInfo();

    if (is_swap)
    {
        std::swap(current_info.m_inGuildInfo, current_info.m_disGuildInfo);
    }    
    ZeroMemory(&current_info.m_disGuildInfo, sizeof(current_info.m_disGuildInfo));

    _InitDefenseWall(map_code);
}

void 
GameDominationManager::UpdateDominationInfo(const DOMINATION_INFO& domination_info, 
                                            eDOMINATION_EVENT_TYPE event_type)
{
    const MAPCODE map_code = domination_info.m_FieldCode;
    const GUILDGUID in_guild = domination_info.m_inGuildInfo.m_GuildGuid;
    const GUILDGUID dis_guild = domination_info.m_disGuildInfo.m_GuildGuid;

    switch (event_type)
    {
        case DOMINATION_INIT:
        {
#ifdef _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
            const DOMINATION_INFO* prev_info = GetDominationInfo(map_code);
            GUILDGUID prev_guild_guid = 0;
            if (prev_info != NULL)
            {
                // 초기화시 인장 효과를 재적용 한다.
                prev_guild_guid = prev_info->m_inGuildInfo.m_GuildGuid;
                ApplyAllMemberRewardOption(prev_guild_guid, false);
                _SetDominationInfo(domination_info);
                _RemoveGuildAreaInfoFromMapCode(map_code);
                _InsertGuildAreaInfo(map_code, in_guild);
                ApplyAllMemberRewardOption(prev_guild_guid, true);
            }
            else
            {
                _SetDominationInfo(domination_info);
                _RemoveGuildAreaInfoFromMapCode(map_code);
                _InsertGuildAreaInfo(map_code, in_guild);
            }
#else  // _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
            _SetDominationInfo(domination_info);
            _InsertGuildAreaInfo(map_code, in_guild);
#endif // _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
        }
        break;
    case DOMINATION_DEFFENSEWALL_SET:
    case DOMINATION_INFO_SET:
        {
            _UpdateDominationInfo(domination_info);
            _InsertGuildAreaInfo(map_code, in_guild);
        }
        break;

    case DOMINATION_SUCCESS_DEFENCE:
        {
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
            _UpdateDominationInfo(domination_info);
            eDOMINATION_TYPE domi_type = 
                DominationInfoParser::Instance()->GetDominationTypeFromFieldCode(map_code);
            //몬트샤인일 경우 기존 점령지 정보를 삭제하고 추가한다.
            if (domi_type == DOMINATION_TYPE_MONDSCHIEN)
            {
                _RemoveGuildAreaInfoFromMapCode(map_code);
                _InsertGuildAreaInfo(map_code, in_guild);               // 졈령지 정보 추가
            }
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
            _RemoveDisGuildInfo(map_code);                           // 공격측 정보 삭제
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
            _ResultNotifyAllUser(map_code, in_guild, 0); // 해당 지역에 수성성공을 알림
#else
            _ResultNotify(map_code, in_guild, dis_guild);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

#ifdef _NA_000000_20130429_DOMINATION_PENALTY
            // 공성길드 패널티 설정
            SERVER_COMMON_OPTION& server_option_info = \
                ServerOptionParserEx::Instance()->GetServerOption();
            WORD penalty_days = server_option_info.domination_penalty.penalty_days_of_offence_fail;

            ProcessDominationPenalty(dis_guild, map_code, penalty_days);
#endif //_NA_000000_20130429_DOMINATION_PENALTY
        }
        break;

    case DOMINATION_SUCCESS_OFFENCE:
        {
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
            _UpdateDominationInfo(domination_info);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
            _RemoveDisGuildInfo(map_code,true);                      // 공격, 수비 변경후 공격 측 삭제
            _InsertGuildAreaInfo(map_code, dis_guild);               // 졈령지 정보 추가
            _RemoveGuildAreaInfo(map_code, in_guild);                // 졈령지 정보 삭제
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
            _ResultNotifyAllUser(map_code, dis_guild, in_guild);    // 서버 전체에 알림
#else
            _ResultNotify(map_code, in_guild, dis_guild);           // 승.패 알림
#endif // _NA_006826_20130722_DOMINATION_RENEWAL
            

#ifdef _NA_000000_20130429_DOMINATION_PENALTY
            // 수성길드 패널티 설정
            SERVER_COMMON_OPTION& server_option_info = \
                ServerOptionParserEx::Instance()->GetServerOption();
            WORD penalty_days = server_option_info.domination_penalty.penalty_days_of_defense_fail;

            ProcessDominationPenalty(in_guild, map_code, penalty_days);
#endif //_NA_000000_20130429_DOMINATION_PENALTY
        }
        break;

    case DOMINATION_DISGUILD_SET:
        {
            _UpdateDominationInfo(domination_info);
            _InsertGuildAreaInfo(map_code, in_guild);
            if(g_pGameServer->GetServerType() == FIELD_SERVER)
            {
                _DeleteGuildInfoInAuction(domination_info);         //1. 입찰 성공 길드 정보 삭제
            #ifndef _NA_20120106_DO_NOT_REWARD_DOMINATION_TOKEN
                SendRewardMemo(&domination_info, map_code);
            #endif
                SendInitActionInfo(map_code);                       //4. 경매 정보 초기화 요청(G->Z->DB)
            }
        }
        break;       
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    case DOMINATION_TURN_OVER:
        {
            _UpdateDominationInfo(domination_info);
            _RemoveDisGuildInfo(map_code,true);                      // 공격, 수비 변경후 공격 측 삭제
            _InsertGuildAreaInfo(map_code, dis_guild);               // 졈령지 정보 추가
            _RemoveGuildAreaInfo(map_code, in_guild);                // 졈령지 정보 삭제
        }
        break;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    }

    GameDominationField* zone = 
        static_cast<GameDominationField *>(g_GameZoneManager.GetFieldVillageZone(map_code));
    if (zone == NULL)
    {
        SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]Not found zone[Map:%d]", map_code);
        return;
    }

    zone->SetGuildGuid(in_guild, dis_guild, event_type);
    zone->SetRewardEventInfo(domination_info.m_rewardEvent);
}

const DOMINATION_INFO* 
GameDominationManager::GetDominationInfo(MAPCODE map_code)const
{
    const DominationArea* domination_area = FindAreaInfo(map_code);
    if (domination_area == NULL)
    {
        SUNLOG(eCRITICAL_LOG,__FUNCTION__"Not found domination info in area : %d", map_code);
        return NULL;
    }
    return &(domination_area->GetInfo());
}

bool 
GameDominationManager::RemoveAuctionItem(Player* player, POSTYPE token_pos,
                                         int amount_cost, SLOTCODE item_code) const
{
    if (player == NULL)
    {
        assert(!"Player Not Found");
        SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]Player Not Found");
        return false;
    }

#ifdef _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY
    enum { kPosTypeMax = UCHAR_MAX };
    SLOTCODE item_codes[kPosTypeMax];
    ::ZeroMemory(item_codes, sizeof(item_codes));
    POSTYPE item_nums[kPosTypeMax];
    ::ZeroMemory(item_nums, sizeof(item_nums));

    BOOST_STATIC_ASSERT(sizeof(POSTYPE) == sizeof(BYTE));
    BOOST_STATIC_ASSERT(_countof(item_codes) == _countof(item_nums));

    int num_of_kinds = amount_cost / kPosTypeMax;
    if (num_of_kinds > _countof(item_codes) || num_of_kinds > _countof(item_nums))
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|number of kinds is overflow.|number of kinds = %d, max kinds = %d|"), 
            __FUNCTION__, 
            num_of_kinds, 
            _countof(item_codes)
        );
        return false;
    }
    int remainder = amount_cost % kPosTypeMax;
    if (remainder != 0)
    {
        if (num_of_kinds == kPosTypeMax)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|number of kinds is overflow.|number of kinds = %d, max kinds = %d|"), 
                __FUNCTION__, 
                num_of_kinds, 
                _countof(item_codes)
            );
            return false;
        }
        ++num_of_kinds;
    }
    for (int i = 0; i < num_of_kinds; ++i)
    {
        item_codes[i] = item_code;
        if (remainder != 0 && (i == (num_of_kinds - 1)))
        {
            item_nums[i] = (POSTYPE)remainder;
        }
        else
        {
            item_nums[i] = kPosTypeMax;
        }
    }
    MSG_CG_ITEM_LOSE_ACK ack_msg;
    {
        ItemManager* item_manager = player->GetItemManager();
        RC::eITEM_RESULT result_value = 
            item_manager->DeleteAllKindOfItem(num_of_kinds, item_codes, item_nums, &(ack_msg.m_ItemInfo));
        if (result_value != RC::RC_ITEM_SUCCESS)
        {
            SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]Not delete domination item[guid:%d]", 
                   player->GetCharGuid());
            return false;
        }
    }
    player->SendPacket( &ack_msg, ack_msg.GetSize() );

#else//_NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY
    RC::eITEM_RESULT result = player->GetItemManager()->Delete(token_pos);
    if (result != RC::RC_ITEM_SUCCESS)
    {
        const char* event_string = 
        (result == RC::RC_ITEM_INVALIDSTATE)        ? "RC_ITEM_INVALIDSTATE" :
        (result == RC::RC_ITEM_INVALIDPOS)          ? "RC_ITEM_INVALIDPOS" :
        (result == RC::RC_ITEM_CANNOT_DESTROY_ITEM) ? "RC_ITEM_CANNOT_DESTROY_ITEM" : "UNKNOWN";

        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Result Code[%s] Token Position[%d]",
               event_string, token_pos);

        return false;
    }
#endif//_NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY
    return true;
}

bool 
GameDominationManager::DominationTimerIsExpired()
{
    return !!domination_update_timer_.IsExpired();
}

void 
GameDominationManager::GetInGuildInfo(MAPCODE map_code, DOMINATION_GUILD_INFO& guild_info)
{
    const DOMINATION_INFO* current_info = GetDominationInfo(map_code);

    //해당 맵코드가 존재 하지 않는 경우( 없는 지역이거나 DB에 값이 존재 하지 않을 경우 ) 
    //GuildGuid 를 0으로 리턴 존재 할 경우 읽어들인 정보로 길드 정보를 셋팅한다.
    if (current_info == NULL)
    {
        guild_info.m_GuildGuid = 0;
        return;
    }
    else
    {
        guild_info.m_BackGroundIndex = current_info->m_inGuildInfo.m_BackGroundIndex;
        guild_info.m_GuildGuid = current_info->m_inGuildInfo.m_GuildGuid;
        guild_info.m_MapCode = current_info->m_FieldCode;
        guild_info.m_PatternIndex = current_info->m_inGuildInfo.m_PatternIndex;

        _tcsncpy(guild_info.m_tszGuildMaster, current_info->m_inGuildInfo.m_tszGuildMaster, 
                 _countof(guild_info.m_tszGuildMaster));
        guild_info.m_tszGuildMaster[_countof(guild_info.m_tszGuildMaster) - 1] = '\0';

        _tcsncpy(guild_info.m_tszGuildName, current_info->m_inGuildInfo.m_tszGuildName, 
                 _countof(guild_info.m_tszGuildName));
        guild_info.m_tszGuildName[_countof(guild_info.m_tszGuildName) - 1] = '\0';
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        guild_info.progress_state_ = current_info->progress_state_;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
    }
}

void 
GameDominationManager::SendUpdateDominationInfo(const DOMINATION_INFO& domination_info, 
                                                eDOMINATION_EVENT_TYPE event_type)const
{
    MSG_GZ_DOMINATION_UPDATE_INFO_SYN send_msg;
    {
        send_msg.m_DominationInfo = domination_info;
        send_msg.m_eSetType = event_type;
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    #ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION 
        switch(event_type)
        {
        case DOMINATION_INFO_SET:   //길드 정보를 변경할시에는 점령전 진행 상태가 변화되지 않는다.
            break;
        case DOMINATION_TURN_OVER:  //턴오버 시에는 공성이 다시 시작하므로 아래와 같이 설정한다.
            send_msg.m_DominationInfo.progress_state_ = DOMINATION_PROGRESS_STATE_START;
            break;
        default:
            send_msg.m_DominationInfo.progress_state_ = DOMINATION_PROGRESS_STATE_NONE;
            break;
        }
    #else
        send_msg.m_DominationInfo.progress_state_ = DOMINATION_PROGRESS_STATE_NONE;
    #endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
    }
    g_pGameServer->SendToServer(GUILD_SERVER, &send_msg, sizeof(send_msg));
}

void 
GameDominationManager::SetDominationGuildInfo(MAPCODE map_code, eDOMINATION_EVENT_TYPE event_type, 
                                              DOMINATION_REWARD_EVENT_INFO reward_event)
{
    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }
  
    DOMINATION_INFO& domination_info = domination_area->GetInfo();
    domination_info.m_rewardEvent = reward_event;
    SendUpdateDominationInfo(domination_info, event_type);
}

void 
GameDominationManager::SetDominationGuildInfo(MAPCODE map_code, eDOMINATION_EVENT_TYPE event_type)
{
    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }

    DOMINATION_INFO& domination_info = domination_area->GetInfo();
    SendUpdateDominationInfo(domination_info, event_type);
}

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
void GameDominationManager::SetDominationGuildInfo(MAPCODE map_code, eDOMINATION_EVENT_TYPE event_type, 
                                                   DOMINATION_REWARD_EVENT_INFO reward_event, Player* player)
{
    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }

    DOMINATION_INFO& domination_info = domination_area->GetInfo();
    domination_info.m_rewardEvent = reward_event;
    if (player != NULL) 
    {
        GameGuild* dis_guild = g_GameGuildManager.FindGuild(player->GetGuildGuid());
        if (dis_guild)
        {
            DOMINATION_BASIC_GUILD_INFO& m_disGuildInfo = domination_info.m_disGuildInfo;
            m_disGuildInfo.m_GuildGuid = dis_guild->GetGuildGuid();
            dis_guild->GetGuildMark(m_disGuildInfo.m_BackGroundIndex, m_disGuildInfo.m_PatternIndex);
            _tcsncpy(m_disGuildInfo.m_tszGuildName, dis_guild->GetGuildName(), MAX_GUILDNAME_LENGTH);
            GameGuildMember* guild_master = dis_guild->FindMaster();
            if (guild_master) 
            {
                m_disGuildInfo.m_GuildMasterGuid = guild_master->GetCharGuid();
                _tcsncpy(m_disGuildInfo.m_tszGuildMaster, guild_master->GetCharName(), MAX_CHARNAME_LENGTH);
            }
            else
            {
                // 점령자 이름을 표시해줌
                m_disGuildInfo.m_GuildMasterGuid = player->GetCharGuid();
                _tcsncpy(m_disGuildInfo.m_tszGuildMaster, player->GetCharName(), MAX_CHARNAME_LENGTH);
            }
        }
    }
    SendUpdateDominationInfo(domination_info, event_type);
}
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

void 
GameDominationManager::SetDefenseWall(MAPCODE map_code, const BYTE* deffens_wall)
{
    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }
    DOMINATION_INFO& domination_info = domination_area->GetInfo();
    domination_info.m_DeffenseWallA = deffens_wall[0];
    domination_info.m_DeffenseWallB = deffens_wall[1];
    domination_info.m_DeffenseWallC = deffens_wall[2];
    
    SendUpdateDominationInfo(domination_info, DOMINATION_INFO_SET);
}

void 
GameDominationManager::ChangeGuildMark(GUILDGUID guild_guid, 
                                       GUILDMARKIDX pattern_index, GUILDMARKIDX background_index )
{
    AreaInfoMapIterator begin_iterator = area_info_map_.begin(),
                        end_iterator = area_info_map_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        DominationArea& domination_area = *(begin_iterator->second);
        if (domination_area.ExistGuild(guild_guid) == true)
        {
            domination_area.ChangeGuildMark(guild_guid, pattern_index, background_index);
            const DOMINATION_INFO& domination_info = domination_area.GetInfo();
            SendUpdateDominationInfo(domination_info, DOMINATION_INFO_SET);
        }
    }
    return;
}

void 
GameDominationManager::ChangeGuildName(GUILDGUID guild_guid, TCHAR* guild_name)
{
    AreaInfoMapIterator begin_iterator = area_info_map_.begin(),
                        end_iterator = area_info_map_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        DominationArea& domination_area = *(begin_iterator->second);
        if (domination_area.ExistGuild(guild_guid) == true)
        {
            domination_area.ChangeGuildName(guild_guid, guild_name);
            const DOMINATION_INFO& domination_info = domination_area.GetInfo();
            SendUpdateDominationInfo(domination_info, DOMINATION_INFO_SET);
        }
    }
    return;
}

void 
GameDominationManager::ChangeGuildMaster(GUILDGUID guild_guid, TCHAR* master_name)
{
    AreaInfoMapIterator begin_iterator = area_info_map_.begin(),
        end_iterator = area_info_map_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        DominationArea& domination_area = *(begin_iterator->second);
        if (domination_area.ExistGuild(guild_guid) == true)
        {
            domination_area.ChangeGuildMaster(guild_guid, master_name);
            const DOMINATION_INFO& domination_info = domination_area.GetInfo();
            SendUpdateDominationInfo(domination_info, DOMINATION_INFO_SET);
        }
    }
    return;
}

void 
GameDominationManager::_InsertGuildAreaInfo(const MAPCODE map_code, GUILDGUID win_guild_guid)
{
    AreaInGuildMapPair range = area_in_guild_map.equal_range(win_guild_guid);

    AreaInGuildMapIterator area_map_iterator = range.first;

    for(;range.second != area_map_iterator; ++area_map_iterator)
    {
        if (map_code == area_map_iterator->second)
        {
            return;
        }
    }

#ifdef _NA_008334_20150608_SONNENSCHEIN
    GameGuild* guild = g_GameGuildManager.FindGuild(win_guild_guid);
    if (guild == NULL) 
    {
        area_in_guild_map.insert(std::make_pair(win_guild_guid, map_code));
    }
    else
    {
        guild->ApplyGuildRewardOptionForAllMembers(FALSE);
        area_in_guild_map.insert(std::make_pair(win_guild_guid, map_code));

        //몬트샤인지역이고, 시작 상태에서는 버프 부여x, 점령정보갱신 x
        GameDominationField* domi_field = 
            static_cast<GameDominationField*>(g_GameZoneManager.GetFieldVillageZone(map_code));
        if (domi_field == NULL) 
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__":Fail get domination field : %d", map_code);
            return;
        }

        if (map_code == DominationInfoParser::Instance()->GetMondScheinMapCode() &&
            domi_field->GetEventState() == DOMINATION_EVENT_START)
        {
            return;
        }
        else
        {
            guild->ApplyGuildRewardOptionForAllMembers(TRUE);
            SendAreaInfoToGuild(win_guild_guid);
        }
    }
#else
    area_in_guild_map.insert(std::make_pair(win_guild_guid, map_code));
#endif //_NA_008334_20150608_SONNENSCHEIN
}

void 
GameDominationManager::_RemoveGuildAreaInfo( const MAPCODE map_code, GUILDGUID loser_guild_guid )
{
    AreaInGuildMapPair range = area_in_guild_map.equal_range(loser_guild_guid);

    AreaInGuildMapIterator area_map_iterator = range.first;

    for (; range.second != area_map_iterator; ++area_map_iterator)
    {
        const MAPCODE current_map_code = area_map_iterator->second;

        if (map_code == current_map_code)
        {
#ifdef _NA_008334_20150608_SONNENSCHEIN
            GameGuild* guild = g_GameGuildManager.FindGuild(loser_guild_guid);
            if (guild == NULL) 
            {
                area_in_guild_map.erase(area_map_iterator);
            }
            else
            {
                guild->ApplyGuildRewardOptionForAllMembers(FALSE);
                area_in_guild_map.erase(area_map_iterator);
                
                //몬트샤인지역이고, 시작 상태에서는 버프 부여x, 점령정보갱신 x
                GameDominationField* domi_field = 
                    static_cast<GameDominationField*>(g_GameZoneManager.GetFieldVillageZone(map_code));
                if (domi_field == NULL) 
                {
                    SUNLOG(eCRITICAL_LOG, __FUNCTION__":Fail get domination field : %d", map_code);
                    return;
                }

                if (map_code == DominationInfoParser::Instance()->GetMondScheinMapCode() &&
                    domi_field->GetEventState() == DOMINATION_EVENT_START)
                {
                    return;
                }
                else
                {
                    guild->ApplyGuildRewardOptionForAllMembers(TRUE);
                    SendAreaInfoToGuild(loser_guild_guid);
                }
            }
#else
            area_in_guild_map.erase(area_map_iterator);
#endif //_NA_008334_20150608_SONNENSCHEIN
            return;
        }
    }
}

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
bool GameDominationManager::_IsAreaInGuild( const GUILDGUID guild_guid ) const
{
    ConstAreaInGuildMapIterator iterater_area_in_guild_map = area_in_guild_map.find(guild_guid);

    if (iterater_area_in_guild_map != area_in_guild_map.end())
    {
        return true;
    }
    return false;
}
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

BYTE
GameDominationManager::GetMiniMapInfo(DOMINATION_GUILD_INFO* guild_info, BYTE max_count)const
{
    DominationInfoParser* info = DominationInfoParser::Instance();

    ConstAreaInfoMapIterator begin_iterator = area_info_map_.begin(),
                             end_iterator = area_info_map_.end();
    BYTE index = 0;

    for (; index < max_count && begin_iterator != end_iterator; ++begin_iterator, ++index)
    {
        const DominationArea& domination_area = *(begin_iterator->second);
        const DOMINATION_INFO& domination_info = domination_area.GetInfo();
        const DOMINATION_BASIC_GUILD_INFO& current_guild = domination_info.m_inGuildInfo;
        DOMINATION_GUILD_INFO* const dest_it = &guild_info[index];
        
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
        //모든 점령지는 해당 필드와 연결되어 있는데 몬트샤인은 독립적으로 존재하고
        //미니맵에 표시되는 영역이 연결된 필드와 다르기 때문에 아래와 같이 예외처리한다.
        const eDOMINATION_TYPE domi_type = \
            info->GetDominationTypeFromFieldCode(domination_info.m_FieldCode);
        switch(domi_type)
        {
        case DOMINATION_TYPE_MONDSCHIEN:
            dest_it->m_MapCode = domination_info.m_FieldCode;
            break;
        case DOMINATION_TYPE_NORMAL:
            dest_it->m_MapCode = info->GetDominationLinkMapCode(domination_info.m_FieldCode);
            break;
        }
#else
        dest_it->m_MapCode = info->GetDominationLinkMapCode(domination_info.m_FieldCode);
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

        dest_it->m_BackGroundIndex = current_guild.m_BackGroundIndex;
        dest_it->m_GuildGuid = current_guild.m_GuildGuid;
        dest_it->m_PatternIndex = current_guild.m_PatternIndex;

        _tcsncpy(dest_it->m_tszGuildMaster, current_guild.m_tszGuildMaster,
                 _countof(dest_it->m_tszGuildMaster));
        dest_it->m_tszGuildMaster[_countof(dest_it->m_tszGuildMaster) - 1] = '\0';

        _tcsncpy(dest_it->m_tszGuildName, current_guild.m_tszGuildName,
                 _countof(dest_it->m_tszGuildName));
        dest_it->m_tszGuildName[_countof(dest_it->m_tszGuildName) - 1] = '\0';

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        dest_it->progress_state_ = domination_info.progress_state_;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
    }

    return index;
}

BYTE 
GameDominationManager::_GetAuctionList(MAPCODE map_code, 
                                      DOMINATION_ACTION_INFO* OUT auction_list, 
                                      BYTE max_count)const
{
    const DominationArea* domination_area = FindAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return 0;
    }

    const DominationAuction* auction = domination_area->GetAuction();
    if (auction == NULL)
    {
        return 0;
    }
    return auction->GetAuctionList(auction_list, max_count);
}

BYTE 
GameDominationManager::GetDefenseWall(MAPCODE map_code, BYTE* deffens_wall)
{
    const DominationArea* domination_area = FindAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return 0;
    }
    const DOMINATION_INFO& domination_info = domination_area->GetInfo();

    deffens_wall[0] = domination_info.m_DeffenseWallA;
    deffens_wall[1] = domination_info.m_DeffenseWallB;
    deffens_wall[2] = domination_info.m_DeffenseWallC;

    return domination_info.m_TotalPoint;
}

//_NA_0_20100208_DOMINATION_AREA_INFO
void 
GameDominationManager::SerializeAreaInfo(const GUILDGUID guild_guid, 
                                         DOMINATION_STATE_TOTAL_INFO& OUT state_total_info)
{
    BYTE& count = state_total_info.m_Count;
    count = 0;

    AreaInGuildMapPair range = area_in_guild_map.equal_range(guild_guid);
    AreaInGuildMapIterator area_map_iterator = range.first;

    for(;range.second != area_map_iterator; ++area_map_iterator)
    {
        DOMINATION_STATE_TOTAL_INFO::SLOT_TYPE& rSlot = state_total_info.m_Slot[count++];
        rSlot.m_wDominationCode = area_map_iterator->second;
    }
}

void 
GameDominationManager::SendAreaInfoToPlayer(Player* player, bool reward_flag)
{
    if (player == NULL)
    {
        return;
    }

    const GUILDGUID guild_guid = player->GetGuildGuid();
    if (guild_guid == INVALID_GUILDGUID)
    {
        return;
    }

    AreaInGuildMapPair range = area_in_guild_map.equal_range(guild_guid);
    AreaInGuildMapIterator area_map_iterator = range.first;

    MSG_CG_DOMINATION_AREA_INFO_CMD cmd_msg;

    //_NA_003457_20111013_EXCEPT_DOMINATION_REWARD
    cmd_msg.m_dwPlayerKey = player->GetObjectKey();
    cmd_msg.m_stateTotalInfo.Clear();
    if(reward_flag)
        SerializeAreaInfo(guild_guid, cmd_msg.m_stateTotalInfo);

    player->SendPacket(&cmd_msg,cmd_msg.GetSize());
}

void 
GameDominationManager::SendAreaInfoToGuild(GUILDGUID guild_guid)
{
    GameGuild* game_guild = g_GameGuildManager.FindGuild(guild_guid);

    if (game_guild == NULL)
    {
        return;
    }

    AreaInGuildMapPair range = 
        area_in_guild_map.equal_range(game_guild->GetGuildGuid());
    AreaInGuildMapIterator area_map_iterator = range.first;

    MSG_CG_DOMINATION_AREA_INFO_CMD cmd_msg;

    //_NA_0_20100208_DOMINATION_AREA_INFO
    SerializeAreaInfo(guild_guid, cmd_msg.m_stateTotalInfo);

    game_guild->SendPacketMemberAround(cmd_msg, INVALID_GUILDGUID);
}

void 
GameDominationManager::_ResultNotify(MAPCODE map_code, GUILDGUID win_guild_guid, 
                                     GUILDGUID lose_guild_guid)
{
    //패킷의 중복전달을 막기 위해서 필드서버에서만 패킷을 보내도록 한다.
    if (g_pGameServer->GetGuid() != FIELD_SERVER)
        return;

    GameGuild* win_guild  = g_GameGuildManager.FindGuild(win_guild_guid);
    GameGuild* lose_guild = g_GameGuildManager.FindGuild(lose_guild_guid);

    MSG_CG_DOMINATION_RESULT_BRD brd_msg;

    brd_msg.m_winGuid  = win_guild_guid;
    brd_msg.m_loseGuid = lose_guild_guid;
    brd_msg.m_mapCode  = map_code;

    if (win_guild != NULL)
    {
        win_guild->SendPacketMember(brd_msg);
    }

    if (lose_guild != NULL)
    {
        lose_guild->SendPacketFieldMember(brd_msg, map_code);
    }
}

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
void GameDominationManager::_ResultNotifyAllUser(MAPCODE map_code, GUILDGUID win_guild_guid, GUILDGUID lose_guild_guid)
{
    //패킷의 중복전달을 막기 위해서 필드서버에서만 패킷을 보내도록 한다.
    if (g_pGameServer->GetGuid() != FIELD_SERVER)
        return;

    MSG_AG_ZONE_DOMINATION_NOTIFIER_BRD brd_msg;
    ZeroMemory(brd_msg.win_guild_name, sizeof(brd_msg.win_guild_name));
    brd_msg.map_code = map_code;
    brd_msg.win_guild_guid  = win_guild_guid;
    brd_msg.lose_guild_guid = lose_guild_guid; // lose_guild 가 0이면 수성 성공

    GameGuild* const win_guild = g_GameGuildManager.FindGuild(win_guild_guid);
    if (win_guild) {
        _tcsncpy(brd_msg.win_guild_name, win_guild->GetGuildName(), MAX_GUILDNAME_LENGTH);
    }

    g_pGameServer->SendToServer(AGENT_SERVER, &brd_msg, brd_msg.GetSize());
}
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

namespace impl {

void ApplyRewardOption(
    Player* const player, const DominationInfoParser::TakenMaps& taken_maps, const bool is_add)
{
    if (player == NULL)
    {
        return;
    }
    if (taken_maps.size() < 1)
    {
        return;
    }

    const DominationInfoParser* const parser = DominationInfoParser::Instance();
    const DominationInfoParser::SelectedRewardOptions& sel_options = 
        parser->GetRewardOption(taken_maps);
    if (sel_options.size() < 1)
    {
        return;
    }

    SkillAttrCalculator attr_calculator(player->GetAttr());

    for (DominationInfoParser::SelectedRewardOptions::const_iterator it = sel_options.begin(); 
        it != sel_options.end(); ++it)
    {
        const DominationInfoParser::SelectedRewardOption& option = it->second;
        const AttrOptionInfo* const option_info = option.option_info;
        if (option_info == NULL)
        {
            continue;
        }
        if (option_info->m_wOptionKind == 0)
        {
            continue;
        }
        if (option_info->m_iOptionValue == 0)
        {
            continue;
        }

        const eATTR_TYPE attr_type = g_ITEM_OPTION[option_info->m_wOptionKind];
        if (is_add)
        {
            attr_calculator.AddAttr(
                attr_type, option_info->m_byOptionType, option_info->m_iOptionValue, false);
        }
        else
        {
            attr_calculator.DelAttr(
                attr_type, option_info->m_byOptionType, option_info->m_iOptionValue, false);
        }
        
        SUNLOG(
            eDEV_LOG, 
            _T("|%s|%d|점령전 보상 옵션이 적용 되었습니다.|IsAdd = %d, AttrType = %d, AttrValueType = %d, AttrValue = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            (is_add ? 1 : 0), 
            attr_type,
            option_info->m_byOptionType, 
            option_info->m_iOptionValue
        );
    }
}

} // impl

void GameDominationManager::ApplyAllRewardOption(Player* const player, const bool is_add)
{
    if (player == NULL)
    {
        return;
    }

    const GUILDGUID guild_guid = player->GetGuildGuid();
    if (guild_guid == INVALID_GUILDGUID)
    {
        return;
    }

#ifdef _NA_008334_20150608_SONNENSCHEIN
    DominationInfoParser::TakenMaps taken_maps;
    GetGuildTakenMaps(guild_guid, &taken_maps);
#else
    std::pair<AreaInGuildMap::const_iterator, AreaInGuildMap::const_iterator> taken_map_range = 
        area_in_guild_map.equal_range(guild_guid);

    //_NA_004899_20120530_CHANGE_DOMINATION_REWARD_OPTION
    DominationInfoParser::TakenMaps taken_maps;
    {
        for (AreaInGuildMap::const_iterator it = taken_map_range.first; 
            it != taken_map_range.second; ++it)
        {
            const MAPCODE taken_map_code = it->second;
            taken_maps.push_back(taken_map_code);
        }
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
    
    impl::ApplyRewardOption(player, taken_maps, is_add);
}

void GameDominationManager::ApplyRewardField(
    Player* const player, const MAPCODE map_code, const bool is_add) const
{
    if (player == NULL)
    {
        return;
    }

    const MAPCODE domination_map_code = 
        DominationInfoParser::Instance()->GetDominationFieldCode(map_code);
    if (domination_map_code == 0)
    {
        //gamelulu_dev_note : 해당 함수가 예외처리로 인한 결과를 하나의 로직으로 판단 해야 하는지 
        //아니면 해당 함수의 할일을 전부 하지 못했음으로 실패로 처리해야 하는지에 대한 결론이 필요.
        return;
    }

    const DOMINATION_INFO* const domination_info = GetDominationInfo(domination_map_code);
    if (domination_info == NULL)
    {
        return;
    }

    if (domination_info->m_inGuildInfo.m_GuildGuid == player->GetGuildGuid())
    {
        _AcceptRewardField(player, map_code, is_add);
    }
}

void GameDominationManager::_AcceptRewardField(
    Player* const player, const MAPCODE map_code, const bool is_add) const
{
    const GUILDGUID guild_guid = player->GetGuildGuid();
    if (guild_guid == INVALID_GUILDGUID)
    {
        return;
    }

    StatusManager* const status_manager = player->GetStatusManager();
    status_manager->ChangeInterState(eCHAR_STATE_NPC_NON_PREEMPTIVE_ATTACK, is_add);

    int heim_rate = 0;
    {
        const sDOMINATION_INFO* const script_info =
            DominationInfoParser::Instance()->GetDominationInfo(map_code);
        if (script_info != NULL)
        {
            heim_rate = script_info->m_RewardDropHeimRate;
        }
    }
    if (heim_rate == 0)
    {
        return;
    }
    
    SkillAttrCalculator attr_calculator(player->GetAttr());

#ifdef _NA000000_VALUE_TYPE_SIMPLIFICATION
    const int attr_value_type = VALUE_TYPE_VALUE;
#else
    const int attr_value_type = VALUE_TYPE_RATIO_VALUE;
#endif
    if (is_add)
    {
        attr_calculator.AddAttr(eATTR_BONUS_MONEY_RATIO, attr_value_type, heim_rate, false);
    }
    else
    {
        attr_calculator.DelAttr(eATTR_BONUS_MONEY_RATIO, attr_value_type, heim_rate, false);
    }

    SUNLOG(
        eDEV_LOG, 
        _T("|%s|%d|하임 보상 옵션이 적용 되었습니다.|IsAdd = %d, AttrType = %d, AttrValueType = %d, AttrValue = %d|"), 
        __FUNCTION__, 
        __LINE__, 
        (is_add ? 1 : 0), 
        eATTR_BONUS_MONEY_RATIO,
        attr_value_type, 
        heim_rate
    );
}

GameGuildMember* 
GameDominationManager::_GetGuildMemberInfo(Player* player)const
{
    if (player == NULL)
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Not found player[%d]", player->GetCharGuid());
        return NULL;
    }

    GameGuild* guild = g_GameGuildManager.FindGuild(player->GetGuildGuid());
    if (guild == NULL)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not found guild info for current player[%d]",
               player->GetCharGuid());
        return NULL;
    }

    GameGuildMember* guild_member = guild->FindMember(player->GetCharGuid());
    if (guild_member == NULL)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not found guild member info for current player[%d]",
               player->GetCharGuid());
        return NULL;
    }

    return guild_member; 
}

void 
GameDominationManager::GetCurrentDateTime(TCHAR* OUT result_time, size_t time_length) const
{
    TCHAR time_string[MAX_TIMEDATA_SIZE];

    SYSTEMTIME system_time;
    util::TimeSync::GetLocalTime(&system_time);

    _sntprintf(time_string, _countof(time_string),
               "%02d-%02d-%02d %02d:%02d:%02d", 
               system_time.wYear, 
               system_time.wMonth, 
               system_time.wDay, 
               system_time.wHour, 
               system_time.wMinute, 
               system_time.wSecond);
    time_string[_countof(time_string) - 1] = '\0';

    _tcsncpy(result_time, time_string, time_length);
    result_time[time_length - 1] = '\0';
}

bool 
GameDominationManager::CheckDefenseWall(MAPCODE map_code, const BYTE* deffens_wall)
{
    int total_point = _GetDefenseWallTotalPoint(map_code);
    if (total_point < 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Fail get to deffens wall point[map:%d]", map_code);
        return false;
    }

    BYTE temp_deffens_wall = 0;

    for (int i=0; i<MAX_DOMINATION_DEFENSEWALL_NUM; ++i)
    {
        temp_deffens_wall += deffens_wall[i];
    }

    return (temp_deffens_wall <= total_point) ?  true : false;
}

int 
GameDominationManager::_GetDefenseWallTotalPoint(MAPCODE map_code)
{
    const DominationArea* domination_area = FindAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return 0;
    }

    const DOMINATION_INFO& domination_info = domination_area->GetInfo();

    return domination_info.m_TotalPoint;
}

eDOMINATION_EVENT_STATE 
GameDominationManager::_GetCurrentDominationEvent(MAPCODE map_code)const
{   
    MAPCODE current_map_code = 
        DominationInfoParser::Instance()->GetDominationFieldCode(map_code);
    if (current_map_code == 0)
    {
        return DOMINATION_EVENT_MAX;
    }

    GameDominationField* domination_field = 
        static_cast<GameDominationField*>(g_GameZoneManager.GetFieldVillageZone(current_map_code));

    if (domination_field == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Fail get domination field : %d", map_code);
        return DOMINATION_EVENT_MAX;
    }

    return domination_field->GetEventState();
}

void 
GameDominationManager::CheckNotice(Player* player, MAPCODE map_code, 
                                   MSG_CG_DOMINATION_NOTICE_ACK& msg)
{
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Player info is null");
        return;
    }

    eDOMINATION_EVENT_STATE current_event = _GetCurrentDominationEvent(map_code);

    msg.m_AuctionFlag = _IsApplyAuction(player, current_event);
    msg.m_EnterFlag = _IsEnterArea(player, current_event);
    msg.m_ManageFlag = _IsGateManage(player, current_event, map_code);
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    msg.m_JoinFlag = _IsJoinMember(player, map_code);
}

bool
GameDominationManager::IsDominationArea(MAPCODE map_code)const
{
    ConstAreaInfoMapIterator find_iterator = area_info_map_.find(map_code);

    if (find_iterator == area_info_map_.end())
    {
        return false;
    }

    return true;
}

//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
bool 
GameDominationManager::CanEnterDominationField(Player* player, MAPCODE map_code)const
{
    eDOMINATION_EVENT_STATE current_event = _GetCurrentDominationEvent(map_code);

    return _IsEnterArea(player, current_event);
}

bool 
GameDominationManager::IsDeleteMember(MAPCODE map_code)const
{
    eDOMINATION_EVENT_STATE current_event = _GetCurrentDominationEvent(map_code);

    bool is_delete = (current_event == DOMINATION_EVENT_START || 
                      current_event == DOMINATION_EVENT_WAIT) ? false : true;

    return is_delete;
}
bool
GameDominationManager::IsJoinMember(CHARGUID char_guid, MAPCODE map_code)const
{
    const DominationArea* domination_area = FindAreaInfo(map_code);
    const DominationMember* member = domination_area->GetMember();

    if (member->FindMember(char_guid) == NULL)
    {
        return false;
    }

    return true;
}

void 
GameDominationManager::InitMemberList()
{
    if (is_init_member_ == true)
    {
        return;
    }

    AreaInfoMapIterator begin_iterator = area_info_map_.begin(),
                        end_iterator = area_info_map_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        DominationArea* domination_area = begin_iterator->second;
        domination_area->GetMember()->ClearMember();
    }

    MSG_GZ_DOMINATION_MEMBERLIST_INIT_CMD send_msg;
    g_pGameServer->SendToServer(GUILD_SERVER, &send_msg, sizeof(send_msg));
    
    is_init_member_ = true;
}

void 
GameDominationManager::DeleteMemberList(MAPCODE map_code)
{
    AreaInfoMapIterator find_iterator = area_info_map_.find(map_code);

    if (find_iterator == area_info_map_.end())
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found domination info : %d", map_code);
        return;
    }
    
    DominationArea* domination_area = find_iterator->second;
    domination_area->GetMember()->ClearMember();
}

bool
GameDominationManager::MakeMemberInfo(Player* player, MAPCODE map_code, 
                                      DominationMemberInfo& OUT info)const
{
    if (player == NULL)
    {
        return false;
    }

    const DominationArea* const domination_area = FindAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return false;
    }

    GUILDGUID guild_guid = player->GetGuildGuid();
    if (domination_area->IsAttackGuild(guild_guid) == true)
    {
        info.guild_type_ = true;
    }
    else if (domination_area->IsDeffenseGuild(guild_guid) == true)
    {
        info.guild_type_ = false;
    }
    else
    {
        return false;
    }

    info.member_guid_ = player->GetCharGuid();
    info.member_level_ = player->GetLevel();
    info.member_class_ = player->GetCharType();
    _tcsncpy(info.member_name_, player->GetCharName(), _countof(info.member_name_));
    info.member_name_[_countof(info.member_name_) - 1] = '\0';

    return true;
}

RC::eDOMINATION_RESULT
GameDominationManager::CanJoinMember(Player* player, MAPCODE map_code)const
{
    if (player == NULL)
    {
        return RC::DOMINATION_RESULT_NOT_PLAYER;
    }

    if (player->GetBehave() != PLAYER_BEHAVE_IDLE_STATE)
    {
        return RC::DOMINATION_RESULT_NOT_IDLE_STATE;
    }

    GameGuildMember *guild_member = _GetGuildMemberInfo(player);
    if (guild_member == NULL)
    {
        return  RC::DOMINATION_RESULT_NOT_GUILDMEMBER;
    }

    GUILDGUID guild_guid = player->GetGuildGuid();
    const DominationArea* const domination_area = FindAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return RC::DOMINATION_RESULT_NOT_FOUND_AREA;
    }

    RC::eDOMINATION_RESULT result_code = domination_area->CanJoinMember(guild_guid, player->GetCharGuid());

    if(result_code != RC::DOMINATION_RESULT_SUCCESS)  
    {
        return result_code;
    }

    CHARGUID member_guid = player->GetCharGuid();
    MAPCODE joined_map_code = _DidJoinArea(member_guid);
    if (joined_map_code != 0)
    {
        return RC::DOMINATION_RESULT_CAN_JOIN_ONLYONE_AREA;
    }

    eDOMINATION_EVENT_STATE current_event = _GetCurrentDominationEvent(map_code);
    if (current_event != DOMINATION_EVENT_SELECT_ATTACKGUILD_END)
    {
        return RC::DOMINATION_RESULT_NOT_FOUND_EVENT;
    }

    return RC::DOMINATION_RESULT_SUCCESS;
}

void 
GameDominationManager::SendMemberList(Player* player, MAPCODE map_code)const
{
    //리스트를 보여줄 필요가 없으면 패킷을 보내지 않아도 된다.
    bool show_flag = _IsShowMemberList(map_code);
    if (show_flag == false)
    {
        return;
    }

    const DominationArea* domination_area = FindAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }

    const DominationMember* member = domination_area->GetMember();
    if (member == NULL)
    {
        return;
    }

    MSG_CG_DOMINATION_MEMBERLIST_NTF ntf_msg;

    ntf_msg.is_delete_member_ = IsDeleteMember(map_code);
    ntf_msg.map_code_ = map_code;
    ntf_msg.data_count_ = member->GetMemberList(ntf_msg.member_list_, _countof(ntf_msg.member_list_));

    player->SendPacket(&ntf_msg, ntf_msg.GetSize());
}

bool
GameDominationManager::_IsShowMemberList(MAPCODE map_code)const
{
    eDOMINATION_EVENT_STATE cur_event = _GetCurrentDominationEvent(map_code);

    //공격길드 선정 - 점령전 종료 시까지 맴버 리스트를 보여줘야 한다.
    if (cur_event > DOMINATION_EVENT_SELECT_ATTACKGUILD && cur_event < DOMINATION_EVENT_END)
    {
        return true;
    }

    return false;
}

bool
GameDominationManager::_IsJoinMember(Player* player, MAPCODE map_code)const
{
    if (CanJoinMember(player, map_code) != RC::DOMINATION_RESULT_SUCCESS)
    {
        return false;
    }

    return true;
}

MAPCODE
GameDominationManager::_DidJoinArea(CHARGUID char_guid)const
{
    ConstAreaInfoMapIterator begin_iterator = area_info_map_.begin(),
                             end_iterator = area_info_map_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const DominationArea* domination_area = begin_iterator->second;
        const DominationMember* member = domination_area->GetMember();

        if (member->FindMember(char_guid) != NULL)
        {
            return domination_area->GetMapCode();
        }
    }

    return 0;
}

bool 
GameDominationManager::_IsApplyAuction(Player* player, eDOMINATION_EVENT_STATE current_event)const
{
    if (player == NULL)
    {
        SUNLOG(eFULL_LOG,"["__FUNCTION__"]player value is null");
        return false;
    }

    //GUILD 맴버 검사
    GUILDGUID guild_guid = player->GetGuildGuid();
    if (guild_guid == INVALID_GUILDGUID)
    {
        return false;
    }

    //경매시작 || 점령전 끝 || 경매 시작 2 일경우 활성화
    if (current_event == DOMINATION_EVENT_AUCTION_START || 
        current_event == DOMINATION_EVENT_END ||
        current_event == DOMINATION_EVENT_AUCTION_START2)
    {
        return true;
    }

    return false;
}

MAPCODE 
GameDominationManager::_DidApplyGuild(GUILDGUID guild_guid)const
{
    ConstAreaInfoMapIterator begin_iterator = area_info_map_.begin(),
                             end_iterator = area_info_map_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const DominationArea* domination_area = begin_iterator->second;
        const DominationAuction* auction = domination_area->GetAuction();
        if (auction->IsApplyToGuild(guild_guid))
        {
            return auction->GetMapCode();
        }
    }
    return 0;
}

bool 
GameDominationManager::_IsEnterArea(Player* player, eDOMINATION_EVENT_STATE current_event)const
{
    if (player == NULL)
    {
        SUNLOG(eFULL_LOG,"["__FUNCTION__"]Player value is null");
        return false;
    }

    GameGuildMember* guild_member = _GetGuildMemberInfo(player);
    if (guild_member == NULL)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Player is not guild member");
        return false;
    }

    GameField* field = player->GetField();
    if (field == NULL) 
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not found field");
        return false;
    }

    MAPCODE map_code = 
        DominationInfoParser::Instance()->GetDominationFieldCode(field->GetFieldCode());
    if (map_code == 0)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not found mapcode");
        return false;
    }

    const DOMINATION_INFO* domination_info = GetDominationInfo(map_code);
    if (domination_info == NULL)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not found domination info");    
        return false;
    }

    GUILDGUID guild_guid = player->GetGuildGuid();

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    const eDOMINATION_TYPE domination_type = 
        DominationInfoParser::Instance()->GetDominationTypeFromFieldCode(field->GetFieldCode());

    switch(domination_type)
    {
    case DOMINATION_TYPE_MONDSCHIEN:
        if (_IsAreaInGuild(guild_guid) == false) //점령지가 없는 길드이다.
        {
            const DominationArea* domination_area = FindAreaInfo(map_code);
            if (domination_area == NULL)
            {
                SUNLOG(eFULL_LOG,"["__FUNCTION__"]Not found DominationArea(map code) = %d", map_code);
                return false;
            }

            if (domination_area->IsPrevInGuild(guild_guid) == false) //점령지가 없고 이전 몬트샤인의 주인이 아니다.
            {
                return false;
            }
        }
        break;
    case DOMINATION_TYPE_NORMAL:
    default:
        break;
    }
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    if (current_event == DOMINATION_EVENT_WAIT || current_event == DOMINATION_EVENT_START)
    {
        //점령전 맵에 입장해야할 시간일 경우, 공수 상관 없이 리스트에 존재해야 입장 가능
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        return true;
#else
        if (IsJoinMember(player->GetCharGuid(), map_code) == true)
        {
            return true;
        }
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Player is not Join member");    
        return false;
#endif // _NA_006826_20130722_DOMINATION_RENEWAL
    }
    else if (current_event == DOMINATION_EVENT_SYSTEM_RESET)
    {
        //시스템 리셋 시간에는 모두 입장 불가
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Reset time");    
        return false;
    }
    else
    {
        //평상시에는 점령 길드만 입장
        if (guild_guid == domination_info->m_inGuildInfo.m_GuildGuid) 
        {
            return true;
        }
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not Matched Guild");    
    }

    SUNLOG(eFULL_LOG, "["__FUNCTION__"]Unknow");    
    return false;
}

bool 
GameDominationManager::_IsGateManage(Player* player, eDOMINATION_EVENT_STATE current_event,MAPCODE map_code)
{
    if (player == NULL)
    {
        SUNLOG(eFULL_LOG,"["__FUNCTION__"]Player value is null");
        return false;
    }
    
    const DOMINATION_INFO* domination_info = GetDominationInfo(map_code);
    if (domination_info == NULL)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not found domination info");
        return false;
    }
    
    GameGuildMember* game_guild_member = _GetGuildMemberInfo(player);
    if (game_guild_member == NULL)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Player is not guild member");
        return false;
    }

    GUILDGUID guild_guid = player->GetGuildGuid();
    if (guild_guid == domination_info->m_inGuildInfo.m_GuildGuid)
    {
        if (game_guild_member->GetDuty() == eGUILD_DUTY_MASTER)
        {
            //--시스템리셋 || 점령전대기 || 점령전시작 이 아니면 관리 가능
            if (current_event != DOMINATION_EVENT_SYSTEM_RESET &&
                current_event != DOMINATION_EVENT_WAIT &&
                current_event != DOMINATION_EVENT_START)
            {
                return true;
            }
        }
    }

    return false;
}

//공격 길드 결정 : 경매 정보내 길드 정보 (copy to) 공격 길드 정보
void 
GameDominationManager::SetDominationInfoDisGuild(MAPCODE map_code)
{
    MAPCODE check_map_code = DominationInfoParser::Instance()->GetDominationFieldCode(map_code);
    if (check_map_code == 0)
    {
        return;
    }

    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }

    DOMINATION_INFO& domination_info = domination_area->GetInfo();
    ZeroMemory(&(domination_info.m_disGuildInfo), sizeof(domination_info.m_disGuildInfo));

    DominationAuction* auction = domination_area->GetAuction();
    if (auction == NULL)
    {
        return;
    }
    const DominationAuctionInfo* auction_info = auction->SelectAttackGuild();
    if (auction_info == NULL)
    {
        domination_info.m_disGuildInfo.m_GuildGuid = 0;
        return;
    }

    DOMINATION_BASIC_GUILD_INFO& dis_guild_info = domination_info.m_disGuildInfo;
    {
        dis_guild_info.m_GuildGuid        = auction_info->m_ActionInfo.m_GuilGuid;
        dis_guild_info.m_PatternIndex     = auction_info->m_PatternIndex;
        dis_guild_info.m_BackGroundIndex  = auction_info->m_BackGroundIndex;
        dis_guild_info.m_GuildMasterGuid  = auction_info->m_MasterGuid;

        _tcsncpy(dis_guild_info.m_tszGuildName, auction_info->m_ActionInfo.m_tszGuildName,
            _countof(dis_guild_info.m_tszGuildName));
        dis_guild_info.m_tszGuildName[_countof(dis_guild_info.m_tszGuildName) - 1]='\0';

        _tcsncpy(dis_guild_info.m_tszGuildMaster, auction_info->m_tszGuildMaster,
            _countof(dis_guild_info.m_tszGuildMaster));
        dis_guild_info.m_tszGuildMaster[_countof(dis_guild_info.m_tszGuildMaster) - 1]='\0';
    }

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    domination_info.m_BestCost = 0;
#else
    sDOMINATION_INFO* parser_info = DominationInfoParser::Instance()->GetDominationInfo(map_code);
    if (parser_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not found data in script : %d", map_code);
        return;
    }

    const float percentage = 100.0f;
    domination_info.m_BestCost = static_cast<MONEY>((auction_info->m_ActionInfo.m_TenderCost) * 
                                                    (parser_info->m_BattingCharge / percentage));
#endif

    SendUpdateDominationInfo(domination_info, DOMINATION_DISGUILD_SET);
    GAMELOG->LogDomination(ACT_DOMINATION_SET_ATTACK_GUILD, domination_info.m_FieldCode);

    //_NA_20100604_BETTER_DOMINATION_MANAGER
    UpdateGuildInfoByDomination(domination_info.m_inGuildInfo.m_GuildGuid, DOMINATION_JOIN_COUNT);

    GameGuild* guild = g_GameGuildManager.FindGuild(auction_info->m_ActionInfo.m_GuilGuid);
    if (guild == NULL)
    {
        SUNLOG(eFULL_LOG, __FUNCTION__":Not found guild info");
        return;
    }

    MSG_CG_DOMINATION_SELECTED_BRD brd_msg;
    brd_msg.m_DominationFieldCode = 
        DominationInfoParser::Instance()->GetDominationLinkMapCode(domination_info.m_FieldCode);
    guild->SendPacketAll(&brd_msg, sizeof(brd_msg));

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    is_init_member_ = false;
}

void 
GameDominationManager::SendInitActionInfo(MAPCODE map_code)
{
    MSG_GZ_DOMINATION_INIT_ACTIONINFO_SYN send_msg;
    send_msg.m_MapCode = map_code;
    g_pGameServer->SendToServer(GUILD_SERVER, &send_msg, sizeof(send_msg));
}

void 
GameDominationManager::_ReturnDominationActionHeim(const DOMINATION_INFO& domination_info)
{
    MSG_GZ_DOMINATION_RETURNHIME_SYN send_msg;

    send_msg.m_DominationInfo = domination_info;
    g_pGameServer->SendToServer(GUILD_SERVER, &send_msg, sizeof(send_msg));
}

void 
GameDominationManager::_DeleteGuildInfoInAuction(const DOMINATION_INFO& domination_info)
{
    MAPCODE map_code = domination_info.m_FieldCode;
    GUILDGUID guild_guid = domination_info.m_disGuildInfo.m_GuildGuid;

    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }

    DominationAuction* auction = domination_area->GetAuction();
    if (auction == NULL)
    {
        return;
    }
    auction->DeleteAuctionInfo(guild_guid);
}

// implemented by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
bool 
GameDominationManager::SendRewardMemo(const DOMINATION_INFO* domination_info, MAPCODE map_code)
{
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    SLOTCODE reward_item_code = 
        DominationInfoParser::Instance()->GetBattingItemCodeByMapCode(domination_info->m_FieldCode);
#else
    SLOTCODE reward_item_code = 0;//하임의 아이템 코드는 0 갯수는 0이상
#endif

    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return false;
    }

    DominationAuction* auction = domination_area->GetAuction();
    if (auction == NULL)
    {
        return false;
    }
    auction->SendRewardMemo(reward_item_code, SYSMEMO_DOMINATION_RETURN_MONEY_NEW);

    return true;
}

void 
GameDominationManager::ClearAuctionInfo(MAPCODE map_code)
{
    DominationArea* domination_area = GetAreaInfo(map_code);
    if (domination_area == NULL)
    {
        return;
    }

    DominationAuction* auction = domination_area->GetAuction();
    if (auction == NULL)
    {
        return;
    }
    auction->ClearAuctionInfo();
}

void GameDominationManager::GuildMasterNameChange( \
    GUILDGUID guild_guid, CHARGUID char_guid, const TCHAR* character_name)
{
    for (AreaInfoMapIterator it = area_info_map_.begin(); it != area_info_map_.end(); ++it)
    {
        DominationArea& domination_area = *(it->second);
        DOMINATION_INFO& domination_info = domination_area.GetInfo();

        if (domination_info.m_inGuildInfo.m_GuildGuid == guild_guid &&
            domination_info.m_inGuildInfo.m_GuildMasterGuid == char_guid) 
        {
            memset(domination_info.m_inGuildInfo.m_tszGuildMaster, 0, \
                sizeof(domination_info.m_inGuildInfo.m_tszGuildMaster));
            _tcsncpy(domination_info.m_inGuildInfo.m_tszGuildMaster, \
                character_name, MAX_CHARNAME_LENGTH);
        }

        if (domination_info.m_disGuildInfo.m_GuildGuid == guild_guid &&
            domination_info.m_disGuildInfo.m_GuildMasterGuid == char_guid) 
        {
            memset(domination_info.m_disGuildInfo.m_tszGuildMaster, 0, \
                sizeof(domination_info.m_disGuildInfo.m_tszGuildMaster));
            _tcsncpy(domination_info.m_disGuildInfo.m_tszGuildMaster, \
                character_name, MAX_CHARNAME_LENGTH);
        }
    }
}

#ifdef _NA_000000_20130429_DOMINATION_PENALTY
void GameDominationManager::ProcessDominationPenalty( \
    GUILDGUID guild_guid, MAPCODE map_code, int penalty_days)
{
    GameGuild* guild = g_GameGuildManager.FindGuild(guild_guid);
    if (guild) 
    {
        int member_count_in_domination_field = \
            guild->GetMemberCountInDominataionFiled(map_code);

        SERVER_COMMON_OPTION& server_option_info = \
            ServerOptionParserEx::Instance()->GetServerOption();

        if (member_count_in_domination_field < \
            server_option_info.domination_penalty.minimum_participation_members) 
        {
            CTime current_time = util::TimeSync::_time64(NULL);
            CTime penalty_time = current_time + CTimeSpan(penalty_days, 0, 0, 0); 

            MSG_GZ_DOMINATION_PENALTY_CMD msg;
            msg.guild_guid = guild_guid;
            msg.map_code = map_code;
            msg.member_count = member_count_in_domination_field;
            msg.penalty_time = penalty_time;
            g_pGameServer->SendToServer(GUILD_SERVER, &msg, sizeof(msg));

            MSG_CG_DOMINATION_BATTING_HEIM_ACK warning_msg;
            warning_msg.m_ResultCode = RC::DOMINATION_RESULT_PENALTY_TIME;
            warning_msg.m_RemainCost = 0;
            warning_msg.penalty_time = penalty_time.GetTime();

            guild->SendPacketAll(&warning_msg, sizeof(warning_msg));
        }
    }
}
#endif //_NA_000000_20130429_DOMINATION_PENALTY

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
bool GameDominationManager::CheckDominationIsProgressing()
{
    for (AreaInfoMapIterator it = area_info_map_.begin(); it != area_info_map_.end(); ++it)
    {
        DominationArea& domination_area = *(it->second);
        eDOMINATION_EVENT_STATE current_event = \
            _GetCurrentDominationEvent(domination_area.GetMapCode());

        if (current_event == DOMINATION_EVENT_START || current_event == DOMINATION_EVENT_WAIT) {
            return true;
        }
    }

    return false;
}

void GameDominationManager::ChangeAreaProgressState( MAPCODE map_code, const eDOMINATION_PROGRESS_STATE area_state )
{
    AreaInfoMapIterator find_iterator = area_info_map_.find(map_code);

    if (find_iterator == area_info_map_.end())
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found domination info : %d", map_code);
        return;
    }

    DominationArea* domination_area = find_iterator->second;
    domination_area->ChangeAreaProgressState(area_state);
}

void GameDominationManager::ChangeAllAreaProgressState( const eDOMINATION_PROGRESS_STATE area_state )
{
    for (AreaInfoMapIterator it = area_info_map_.begin(); it != area_info_map_.end(); ++it)
    {
        DominationArea& domination_area = *(it->second);
        
        domination_area.ChangeAreaProgressState(area_state);
    }
}

void GameDominationManager::OnDeadDominationMapNPC( const MAPCODE map_code, const MONSTERCODE monster_code )
{
    AreaInfoMapIterator find_iterator = area_info_map_.find(map_code);

    if (find_iterator == area_info_map_.end())
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found domination info : %d", map_code);
        return;
    }

    DominationArea* domination_area = find_iterator->second;

    // 점령전 진행중이 아닐때, 오브젝트파괴로 상태가 바뀌지 않도록 한다. (GM테스트의 경우 가능한 상황)
    if ( domination_area->GetAreaProgressState() == DOMINATION_PROGRESS_STATE_NONE ) {
        return;
    }

    eDOMINATION_PROGRESS_STATE change_state = DOMINATION_PROGRESS_STATE_NONE;
    switch (monster_code)
    {
        // 외벽(60003), 개구멍(60008) 파괴당함
    case 60003:
    case 60008:
        change_state = DOMINATION_PROGRESS_STATE_BREAK_GATE;
        break;
        // 피의 고리(60006) 파괴당함
    case 60006:
        change_state = DOMINATION_PROGRESS_STATE_BREAK_TEMPLEOFBLOOD;
        break;
        // 피의 결정체(60007) 파괴당함
    case 60007:
        change_state = DOMINATION_PROGRESS_STATE_BREAK_CRYSTALIZEDBLOOD;
        break;
    default:
        return;
    }
    // 현재 진행상태는 더 낮은 진행상태단계로 내려가지는 않는다
    if (domination_area->GetAreaProgressState() < change_state) {
        domination_area->ChangeAreaProgressState(change_state);
    }
}
#endif //_NA_006826_20130722_DOMINATION_RENEWAL


#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION

void GameDominationManager::SetDominationContinentLordInfo(const sDOMINATION_CONTINENT_LORD_INFO lord_info)
{
    ContinentLordInfos::iterator iter = continent_lord_infos_.find(lord_info.continent_number);
    if (iter != continent_lord_infos_.end())
    {
        continent_lord_infos_.erase(iter);
    }
    continent_lord_infos_.insert(std::make_pair<int, sDOMINATION_CONTINENT_LORD_INFO>
                                 (lord_info.continent_number, lord_info));
}

const sDOMINATION_CONTINENT_LORD_INFO* GameDominationManager::GetDominationContinentLordInfo(int continent) const
{
    ContinentLordInfos::const_iterator iter = continent_lord_infos_.find(continent);
    if (iter != continent_lord_infos_.end())
    {
        return &iter->second;
    }
    return NULL;
}

void GameDominationManager::_RemoveGuildAreaInfoFromMapCode( const MAPCODE map_code )
{
    AreaInGuildMapIterator area_map_iter = area_in_guild_map.begin();
    AreaInGuildMapIterator area_map_iter_end = area_in_guild_map.end();

    for (; area_map_iter != area_map_iter_end; ++area_map_iter)
    {
        const GUILDGUID current_map_guild = area_map_iter->first;
        const MAPCODE current_map_code = area_map_iter->second;
        if (map_code == current_map_code)
        {
            area_in_guild_map.erase(area_map_iter);
            return;
        }
    }
}

#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION


#ifdef _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD

void GameDominationManager::ApplyAllMemberRewardOption( const GUILDGUID guild_guid, const bool is_add )
{
    const GameGuild* const guild = g_GameGuildManager.FindGuild(guild_guid);
    if (guild == NULL)
    {
        return;
    }

    const GUILDMEMBER_HASH& guild_members = guild->GetMemberHash();
    for (GUILDMEMBER_HASH::const_iterator it = guild_members.begin(); it != guild_members.end(); ++it)
    {
        const GameGuildMember* const guild_member = it->second;
        if (guild_member == NULL)
        {
            continue;
        }

        Player* const player = PlayerManager::Instance()->FindPlayerByCharKey(guild_member->GetCharGuid());
        if (player == NULL)
        {
            continue;
        }
        GameField* field = player->GetField();
        if (field == NULL)
        {
            continue;
        }

        GameZone* zone = field->GetGameZone();
        if (zone == NULL)
        {
            continue;
        }

        eZONETYPE zone_type = zone->GetZoneType();

        // 다음 존, 필드 타입들은 공성인장 효과를 적용 받지 않는 장소이므로 
        if (zone->GetZoneType() == eZONETYPE_INSTANCE)
        {
            GameInstanceDungeon* const pIndun = static_cast<GameInstanceDungeon*>(zone);
            eINSTANCE_DUNGEON_KIND indun_type = pIndun->GetTypeOfIndun();
            if(indun_type == INDUN_KIND_SS_QUEST || indun_type == INDUN_KIND_CHAOS_ZONE)
            {
                continue;
            }
        }

        if (zone_type == eZONETYPE_DOMINATION_FIELD)
        {
            continue;
        }

        // 효과 실제 적용
        ApplyAllRewardOption(player, is_add);

        // 인장 정보 갱신
        SendAreaInfoToPlayer(player, is_add);

        // 인장 이펙트 갱신
        player->FirstGuildMemberEnter();
    }
}

#endif // _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD

#ifdef _NA_008334_20150608_SONNENSCHEIN
VOID GameDominationManager::GetGuildTakenMaps(GUILDGUID guild_guid, OUT DominationInfoParser::TakenMaps* taken_maps)
{
    //_NA_004899_20120530_CHANGE_DOMINATION_REWARD_OPTION
    std::pair<AreaInGuildMap::const_iterator, AreaInGuildMap::const_iterator> taken_map_range = 
        area_in_guild_map.equal_range(guild_guid);

    for (AreaInGuildMap::const_iterator it = taken_map_range.first; 
        it != taken_map_range.second; ++it)
    {
        const MAPCODE taken_map_code = it->second;
        taken_maps->push_back(taken_map_code);
    }
}

#endif //_NA_008334_20150608_SONNENSCHEIN