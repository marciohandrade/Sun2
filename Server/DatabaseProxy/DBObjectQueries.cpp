#include "stdafx.h"

#include <TimeConverter.h>
#include <SolarDateTime.h>

#include <SerializeStream.h>
#include <VersionControl.h>
#include <SCItemSlot.h>

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
#else
#include <AreaConquest.h>
#endif

#include <PacketStruct_AD.h>
#include <PacketStruct_DG.h>
#include <PacketStruct_DW.h>

#include "./DBUser.h"
#include "./DBCharacter.h"
#include "ProxyNodes/DBQueryProxyManager.h"
#include "Services/GuildManagerInDBP.h"

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
#include "Services/AccumulatePointMatchDate.h"
#endif
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "CommonCharacterFormula.h"
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
//==================================================================================================
// @history
//  |10.04.16|waverix|extract queries from DBUser and DBCharacter

//==================================================================================================
//==================================================================================================
///  DBUser part
//==================================================================================================

bool DBUser::UpdateChar(eDB_UPDATE_TYPE db_update_type, int channel_id,
                        DBCharacter* activated_char)
{
    // NOTE: f101216.1L, the channel mismatch filter like that a below code is not use anymore.
    if (channel_id_ != channel_id)
    {   // for temporary report. reserve deletion
        SUNLOG(eCRITICAL_LOG,
               _T("["__FUNCTION__"]|Msg=mismatch channel id|UserGuid=%d|")
               _T("|UpdateType=%d|ExpectedChannel=%d|RequestChannel=%d|"),
               user_key_, db_update_type, channel_id_, channel_id);
    }

    // CHANGES: f110412.2L, changes logic to prevent disabled update
    // when the this entry point has connectless character
    DBCharacter* updating_char = FindCharacter(conn_char_guid_);
    if (activated_char != NULL) {
        assert(activated_char->GetParentDBUser() == this);
        updating_char = activated_char;
    };

    if (updating_char == NULL) {
        return false;
    }
    // (CHANGES) (f100419.1L) remove checker related to CanDBUpdate()
    const CHARGUID char_guid = updating_char->GetCharGuid();__TOUCHED(char_guid);
    updating_char->UpdateCharInfo();
    updating_char->UpdateInventory();
    updating_char->UpdateSubData();
    UpdateWarehouse();
    //__KR_001355_20090608_GROUPINGSYSTEM_DATA
    //상위 멤버의 상태가 오프라인일 경우에만 Update시켜준다.
    if (updating_char->GetAccumEXP() > 0) {
        UpdateGroupAccumEXP();
    }

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
    g_DBProxyServer.ExecuteQuery_MatchChar_Update(user_guid_,
                                                  updating_char->GetCharName(),
                                                  updating_char->GetLV(),
                                                  updating_char->GetClassCode());
    g_DBProxyServer.ExecuteQuery_MatchFatigue_Update(user_guid_, user_key_, GetPlayTime());
#endif
    return true;
}

bool DBUser::UpdateWarehouse()
{
    const bool kRemainChanged = false;
    const bool kFlushed = true;
    //
    using nsQuery::DBQueryProxyManager;
    DBQueryProxyManager* const db_query_proxy_manager = DBQueryProxyManager::Instance();

    DBQueryProxyManager::eRequestResult result = db_query_proxy_manager->Request_User_Update(\
        this, nsQuery::DBQueryProxyUserNode::eTrans_Update_Warehouse);
    if (result != DBQueryProxyManager::eReqResult_Success) {
        return kRemainChanged;
    }
    return kFlushed;
}

// implemented by __KR_001355_20090608_GROUPINGSYSTEM_DATA
void DBUser::UpdateGroupAccumEXP()
{
    const DBCharacter* const conn_char = FindCharacter(conn_char_guid_);
    if (conn_char == NULL) 
    {
        return;
    }
    if (CanDBUpdate() == false) 
    {
        return;
    }

    const int accum_exp = (int)conn_char->GetAccumEXP();
    TCHAR query_str[MAX_QUERY_LENGTH_128+1];
    _sntprintf(
        query_str, 
        MAX_QUERY_LENGTH_128,
        "{?=call S_Group_AccumExp_Insert(%d, %d, %d, 2)}", 
        conn_char->GetHighMemberGuid(), 
        conn_char->GetCharGuid(), 
        accum_exp
    );
    query_str[MAX_QUERY_LENGTH_128] = '\0';

    Query_Group_AccumExp_Insert* const query = Query_Group_AccumExp_Insert::ALLOC();
    query->SetUserKey(user_key_);
    query->SetQuery(query_str);
    query->SetVoidObject(server_session_);

    this->DBQuery(DW_GROUP, DW_GROUP_ACCUMEXP_DBR, query);
}


bool DBUser::OnSetWarehouseBaseInfo(const Query_Warehouse_Select& query_result)
{
    const Query_Warehouse_Select::sQueryResult* warehouse = query_result.result_records_;
    warehouse_version_ = ITEM_STREAM_SIZE_LAST_VERSION;
    warehouse_money_ = warehouse->money_;
    strncpy(warehouse_password_, warehouse->password_, _countof(warehouse_password_));
    return true;
}


void DBUser::MakeQuery_Warehouse_Update(Query_Warehouse_Update* query)
{
    // NOTE: removed the below check because an update call can occur no link state
    //if (!FlowControl::FCAssert(connected_character_)) {
    //    return;
    //}
    if (GetWarehouseVersion() == 0) {
        warehouse_version_ = ITEM_STREAM_SIZE_LAST_VERSION;
    }
    if (GetWarehouseMoney() > LLONG_MAX) {
        SetWarehouseMoney(LLONG_MAX);
    }
    SetNeedWarehouseToSave(false);
    //
    STRING_SAFER_QUERY64 query_buffer;
    // f101011.1L
    query_buffer.MakeString("{?=call S_Warehouse_Update (?,?)}");

    query->SetQuery(query_buffer);
    query->SetUserKey(user_key_);
    // NOTE: removed
    //query->SetCharName(connected_character_->GetCharName());
    // f101011.1L
    Query_Warehouse_Update::sQueryParam* const parameter = &query->parameters_[0]
    ;{
        parameter->user_guid_ = user_key_;
        parameter->money_ = GetWarehouseMoney();
    };
}

//==================================================================================================
//==================================================================================================
/// DBCharacter part
//==================================================================================================

#if SUN_CODE_BACKUP
void
DBCharacter::UpdateByGuildWarehouse(GuildWarehouseInfo* pGuildWarehouse)
{
    db_last_updated_tick_ = GetTickCount();

    db_user_->UpdateChar(eDB_UPDATE_GUILDWAREHOUSE_SYN, db_user_->GetChannelID(), NULL);
}
#endif
void
DBCharacter::Updatea()
{
    if (db_last_updated_tick_ && 
        db_last_updated_tick_ + _DBP_UPDATE_CYCLE_DELAY < GetTickCount())
    {
        db_last_updated_tick_ = GetTickCount();

        db_user_->UpdateChar(eDB_UPDATE_INNER, db_user_->GetChannelID(), NULL);
    }
}

void
DBCharacter::SerializeCharInfo(BASE_PLAYERINFO& player_info, eSERIALIZE serialize_type)
{
    BOOST_STATIC_ASSERT(sizeof(*player_info_) == sizeof(player_info));
    if (serialize_type == SERIALIZE_STORE)
    {
        CopyMemory(player_info_, &player_info, sizeof(*player_info_));
        AlarmInvalidState("SerializeCharInfo STORE");

        if (player_info_->m_iHP == 0) {
            player_info_->m_iHP = 1;
        }
    }
    else if (serialize_type == SERIALIZE_LOAD)
    {
        CopyMemory(&player_info, player_info_, sizeof(player_info));
    }
}

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
void 
DBCharacter::SerializeAchievementInfo(Character_Achievements& achievement_info, eSERIALIZE eType)
{
	if (eType == SERIALIZE_STORE)
	{		
		for(int i = 0 ; i < achievement_info.achievement_count_ ; i++)
		{
			const Character_Achievement& source_achievement = achievement_info.achievement_info_[i];
			
#ifdef _NA_001990_ACSYSTEM_ADD_INCREASE_INDEX
			DBCharacter_Achievement* dest_achievement = achievement_info_.find(source_achievement.index_number_);
            if (!dest_achievement)
            {
                if(achievement_info_.achievement_max_index_ >= MAX_ACHIEVEMENT_COUNT)
                {
                    //
                    return;
                }

                dest_achievement = &achievement_info_.achievement_info_[achievement_info_.achievement_max_index_++];
            }
#else
			if(MAX_ACHIEVEMENT_COUNT < source_achievement.index_number_ || source_achievement.index_number_ < 1)
			{
				ASSERT(!"업적 인덱스가 최소 최대값 범위에서 벗어 납니다.");
				continue;
			}
			if(source_achievement.index_number_ > achievement_info_.achievement_max_index_)
			{
				achievement_info_.achievement_max_index_ = source_achievement.index_number_;
			}

			DBCharacter_Achievement* const dest_achievement = \
				&achievement_info_.achievement_info_[source_achievement.index_number_ - 1];
#endif //_NA_001990_ACSYSTEM_ADD_INCREASE_INDEX

            if(dest_achievement != source_achievement)
            {
                dest_achievement->set_modification();
                dest_achievement->index_number_ = source_achievement.index_number_;
                dest_achievement->is_reward_ = source_achievement.is_reward_;

                BOOST_STATIC_ASSERT(\
                    sizeof(dest_achievement->object_value_[0]) == sizeof(source_achievement.object_value_[0]) &&
                    sizeof(dest_achievement->object_value_) == sizeof(source_achievement.object_value_));
                //
                CopyMemory(dest_achievement->object_value_, source_achievement.object_value_, 
                    sizeof(dest_achievement->object_value_));
            }
		}		
	}
	else if (eType == SERIALIZE_LOAD)
	{
		achievement_info.clear();

		for(int i = 0 ; i < achievement_info_.achievement_max_index_; i++)
		{
			DBCharacter_Achievement& source_achievement = achievement_info_.achievement_info_[i];
			if(source_achievement.is_empty())
			{
				continue;
			}

			Character_Achievement* const dest_achievement = \
				&achievement_info.achievement_info_[achievement_info.achievement_count_++];

			dest_achievement->index_number_ = source_achievement.index_number_;
			dest_achievement->is_reward_	= source_achievement.is_reward_;

			BOOST_STATIC_ASSERT(\
				sizeof(dest_achievement->object_value_[0]) == sizeof(source_achievement.object_value_[0]) &&
				sizeof(dest_achievement->object_value_) == sizeof(source_achievement.object_value_));

			CopyMemory(dest_achievement->object_value_, source_achievement.object_value_, 
				sizeof(dest_achievement->object_value_));

		}
	}
}
#endif

#ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
void
DBCharacter::SerializeRepurchasItemInfo(REPURCHASITEM_TOTAL_INFO& repurchase_items,
                                        eSERIALIZE serialize_type)
{
    BOOST_STATIC_ASSERT(sizeof(repurchase_items_) == sizeof(repurchase_items));

    if (serialize_type == SERIALIZE_STORE)
    {
        CopyMemory(&repurchase_items_, &repurchase_items, sizeof(repurchase_items_));
    }
    else if (serialize_type == SERIALIZE_LOAD)
    {
        CopyMemory(&repurchase_items, &repurchase_items_, sizeof(repurchase_items));
    }
}
#endif //_NA_20100106_REPURCHAS_BUGFIX_2496

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
void
DBCharacter::SerializeFriendListInfo(CHAR_INVITATEDFRIEND_INFOs& Invitatedfriend_count,
                                        eSERIALIZE serialize_type)
{
    BOOST_STATIC_ASSERT(sizeof(Invitatedfriend_count_) == sizeof(Invitatedfriend_count));

    if (serialize_type == SERIALIZE_STORE)
    {
        CopyMemory(&Invitatedfriend_count_, &Invitatedfriend_count, sizeof(Invitatedfriend_count_));
    }
    else if (serialize_type == SERIALIZE_LOAD)
    {
        CopyMemory(&Invitatedfriend_count, &Invitatedfriend_count_, sizeof(Invitatedfriend_count));
    }
}

bool
DBCharacter::OnSetInfo(const sQUERY_CHARACTER* pQuerySrcInfo)
{
    //pQuerySrcInfo->SetSafeBound();
    BASE_PLAYERINFO* const pDestInfo = GetInfo();
    // r101007.3L
    pDestInfo->m_UserGuid   = pQuerySrcInfo->m_UserGuid;
    pDestInfo->m_CharGuid   = pQuerySrcInfo->m_CharGuid;
    pDestInfo->m_bySlot     = static_cast<uint8_t>(pQuerySrcInfo->m_bySlot);

    BOOST_STATIC_ASSERT((sizeof(pDestInfo->m_tszCharName) ==
                         sizeof(pQuerySrcInfo->result_info_char_name_)));
    _tcsncpy(pDestInfo->m_tszCharName, pQuerySrcInfo->result_info_char_name_,
             _countof(pDestInfo->m_tszCharName));
    pDestInfo->m_tszCharName[_countof(pDestInfo->m_tszCharName) - 1] = _T('\0');
    pDestInfo->m_byClassCode = static_cast<uint8_t>(pQuerySrcInfo->m_byClass);
    pDestInfo->m_LV         = pQuerySrcInfo->m_LV;
    pDestInfo->m_iRegion    = pQuerySrcInfo->m_iRegion;
    pDestInfo->m_sLocationX = pQuerySrcInfo->m_sX;
    pDestInfo->m_sLocationY = pQuerySrcInfo->m_sY;
    pDestInfo->m_sLocationZ = pQuerySrcInfo->m_sZ;
    pDestInfo->m_iPreRegion = pQuerySrcInfo->m_iPreRegion;
    pDestInfo->m_i64Exp     = pQuerySrcInfo->m_i64Exp;
    pDestInfo->m_iMaxHP     = pQuerySrcInfo->m_iMaxHP;
    pDestInfo->m_iHP        = pQuerySrcInfo->m_iHP;
    pDestInfo->m_iMaxMP     = pQuerySrcInfo->m_iMaxMP;
    pDestInfo->m_iMP        = pQuerySrcInfo->m_iMP;
    pDestInfo->m_Money      = pQuerySrcInfo->m_Money;
    pDestInfo->m_iRemainStat = pQuerySrcInfo->m_iRemainStat;
    pDestInfo->m_iRemainSkill = pQuerySrcInfo->m_iRemainSkill;
    ASSERT(pDestInfo->m_iSelectStyleCode <= USHRT_MAX);
    pDestInfo->m_iSelectStyleCode = pQuerySrcInfo->m_iSelectStyleCode;  
    pDestInfo->m_byPKState = static_cast<uint8_t>(pQuerySrcInfo->m_byPKState);
    pDestInfo->m_byCharState = static_cast<uint8_t>(pQuerySrcInfo->m_byCharState);
    pDestInfo->m_StateTime = pQuerySrcInfo->m_StateTime;

    _tcsncpy(pDestInfo->m_tszTitleID, pQuerySrcInfo->result_info_title_id_,
             _countof(pDestInfo->m_tszTitleID));
    pDestInfo->m_tszTitleID[_countof(pDestInfo->m_tszTitleID) - 1] = '\0';
    //pDestInfo->m_tszTitleID[MAX_TITLEID_LENGTH] = '\0';
    //pDestInfo->m_tszTitleID[_tcslen(pQuerySrcInfo->m_tszTitleID)] = '\0';
    pDestInfo->m_TitleTime = pQuerySrcInfo->m_TitleTime;

    // (CHANGES) (f100324.1L) (WAVERIX) moved to OnSetInfoInventory
    //pDestInfo->m_iInventoryLock = pQuerySrcInfo->m_bInventoryLock;
    pDestInfo->m_sStrength  = pQuerySrcInfo->m_sStrength;
    pDestInfo->m_sDexterity = pQuerySrcInfo->m_sDexterity;
    pDestInfo->m_sVitality  = pQuerySrcInfo->m_sVitality;
    pDestInfo->m_sInteligence = pQuerySrcInfo->m_sInteligence;
    pDestInfo->m_sSpirit    = pQuerySrcInfo->m_sSpirit;
    pDestInfo->m_sSkillStat1 = pQuerySrcInfo->m_sSkillStat1;
    pDestInfo->m_sSkillStat2 = pQuerySrcInfo->m_sSkillStat2;

    // 키, 얼굴, 머리
    pDestInfo->m_byHeight   = static_cast<uint8_t>(pQuerySrcInfo->m_byHeight);
    pDestInfo->m_byFace     = static_cast<uint8_t>(pQuerySrcInfo->m_byFace);
    pDestInfo->m_byHair     = static_cast<uint8_t>(pQuerySrcInfo->m_byHair);
    pDestInfo->m_ChaoState  = static_cast<uint8_t>(pQuerySrcInfo->m_byChaoState);
    pDestInfo->m_ChaoEndTime = pQuerySrcInfo->m_i64ChaoTime;

    pDestInfo->m_PlayLimitedTime = pQuerySrcInfo->m_iPlayLimitedTime;
    // 
    pDestInfo->m_UserPoint = pQuerySrcInfo->m_UserPoint;
    pDestInfo->m_byInvisibleOptFlag = static_cast<uint8_t>(pQuerySrcInfo->m_byInvisibleOpt);
    // 길드
    // (WAVERIX) (090506) 전반적 코드 정리 겸, 버퍼 관련 (안정성 + 유지보수) 수정
    pDestInfo->m_GuildGuid = pQuerySrcInfo->m_GuildGuid;
    //
    BOOST_STATIC_ASSERT((_countof(pDestInfo->m_tszGuildNickName) == MAX_CHARNAME_LENGTH + 1) &&
                        (_countof(pQuerySrcInfo->result_info_guild_nick_name_) == 14 + 1));
    // copy 14(+1) to 17
    strncpy(pDestInfo->m_tszGuildNickName, pQuerySrcInfo->result_info_guild_nick_name_,
            _countof(pDestInfo->m_tszGuildNickName));
    pDestInfo->m_tszGuildNickName[_countof(pDestInfo->m_tszGuildNickName) - 1] = '\0';
    //
    BOOST_STATIC_ASSERT((sizeof(pDestInfo->m_tszGuildName) ==
                         sizeof(pQuerySrcInfo->result_info_guild_name_)) &&
                        (_countof(pDestInfo->m_tszGuildName) ==
                         MAX_GUILDNAME_LENGTH + 1));
    strncpy(pDestInfo->m_tszGuildName, pQuerySrcInfo->result_info_guild_name_,
            _countof(pDestInfo->m_tszGuildName));
    pDestInfo->m_tszGuildName[_countof(pDestInfo->m_tszGuildName) - 1] = '\0';

    pDestInfo->m_eGuildDuty   = static_cast<eGUILD_DUTY>(pQuerySrcInfo->m_eGuildDuty);
    pDestInfo->m_bWhisperFlag = pQuerySrcInfo->m_bWhisperFlag;
    pDestInfo->m_bTradeFlag   = pQuerySrcInfo->m_bTradeFlag;

    // f100929.2L, removed because of a duplicated information // 경쟁 헌팅
    //pDestInfo->m_iCHuntingDisCount = pQuerySrcInfo->m_iCHuntingDisCount;
    //pDestInfo->m_iCHuntingVCount   = pQuerySrcInfo->m_iCHuntingVCount;
    //pDestInfo->m_iCHuntingLCount   = pQuerySrcInfo->m_iCHuntingLCount;
    //pDestInfo->m_iCHuntingTotCount = pQuerySrcInfo->m_iCHuntingTotCount;

    event_use_time_ = pQuerySrcInfo->m_EventUseTime;
    //__NA000000_090506_EVENT_TYPE_MODIFICATION__
    event_use_time_normal_ = pQuerySrcInfo->m_EventUseTimeNormal;
    // 캐릭터 생성시간
    pDestInfo->m_CreateTime = util::ConvertTCharToTime_t(pQuerySrcInfo->result_info_create_time_);

    // 삭제여부
    delete_checked_ = static_cast<uint8_t>(pQuerySrcInfo->m_byDelChk);
    // (f100418.7L) copy 17 from 21 ...
    _tcsncpy(delete_date_, pQuerySrcInfo->result_info_del_date_, _countof(delete_date_));
    delete_date_[_countof(delete_date_) - 1] = '\0';

    // (CHANGES) (f100324.1L) (WAVERIX) moved to OnSetInfoInventory
    //pDestInfo->m_ExtraInventoryTabCount = pQuerySrcInfo->m_byExtraInventoryTabCount;
    pDestInfo->m_ExtraEquipCashSlotCount = \
        static_cast<uint8_t>(pQuerySrcInfo->m_ExtraEquipCashSlotCount);
    pDestInfo->m_byFreeInitCount = static_cast<uint8_t>(pQuerySrcInfo->m_byFreeInitCount);

    {   // 체인지업 정보
        BYTE change_of_class = pQuerySrcInfo->m_ChangeOfClassStep ?
                                    static_cast<uint8_t>(pQuerySrcInfo->m_ChangeOfClassStep) :
                                    static_cast<uint8_t>(pQuerySrcInfo->m_byClass);
        // CHANGES: f110209.3L, changes changeup code control
        // to solve not updated state after create character.
        if (change_of_class == 0 || pQuerySrcInfo->m_byClass == change_of_class) {
            change_of_class = BYTE(pQuerySrcInfo->m_byClass + eCHAREX_100LEVEL_CHANGEUP_CONSTANT);
        }
        pDestInfo->m_ChangeOfClassStep.Set(change_of_class);
    }

    pDestInfo->m_bSummonedPet = pQuerySrcInfo->m_bSummonedPet;
    pDestInfo->m_bMAXIntimacy = pQuerySrcInfo->m_bMAXIntimacy;
    pDestInfo->m_PetItemPos   = static_cast<uint8_t>(pQuerySrcInfo->m_PetItemPos);
    pDestInfo->m_bMAXOptionRemainTime = pQuerySrcInfo->m_bMAXOptionRemainTime;
    pDestInfo->m_bActiveEtherDevice = pQuerySrcInfo->m_bActiveEther;
    pDestInfo->m_byEtherBulletPos   = static_cast<uint8_t>(pQuerySrcInfo->m_byEtherBulletPos);
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    pDestInfo->m_wAccumulateYear  = pQuerySrcInfo->m_wAccumulateYear;
    pDestInfo->m_dwAccumulateTime = pQuerySrcInfo->m_dwAccumulateTime;
    pDestInfo->m_wAccumulatePoint = pQuerySrcInfo->m_wAccumulatePoint;

    // 기존의 년도가 초기값 '0'을 가진 경우 적립시간 및 적립 포인트 '0'으로 초기화.
    // 기존의 년도가 현재 년도와 다른 경우 적립시간만 '0'으로 초기화 한다.
    AccumulatePointMatchDate::Instance()->ApplyCurrentYear(pDestInfo);
#endif
#ifdef _NA_002050_20110216_ADD_GENDER
    pDestInfo->gender = pQuerySrcInfo->gender; // 성별
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
	pDestInfo->honor_title_ = pQuerySrcInfo->honor_title;
#endif //_NA_003027_20111013_HONOR_SYSTEM
    //_NA_004035_20120227_COSTUME_ITEM
    pDestInfo->is_equipped_costume_ = pQuerySrcInfo->is_equipped_costume ? true : false; // 코스튬 활성화 여부
    pDestInfo->costume_item_pos_ = pQuerySrcInfo->costume_item_pos; // 코스튬 인벤토리 위치
    pDestInfo->costume_item_code_ = pQuerySrcInfo->costume_item_code; // 코스튬 인벤토리 위치

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    pDestInfo->is_equipped_decorate_costume_ = pQuerySrcInfo->is_equipped_deco_costume ? true : false; // 데코 코스튬 활성화 여부
    pDestInfo->decorate_costume_item_pos_ = pQuerySrcInfo->deco_costume_item_pos; // 데코 코스튬 인벤토리 위치
    pDestInfo->decorate_costume_item_code_= pQuerySrcInfo->deco_costume_item_code; // 데코 코스튬 인벤토리 위치
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    pDestInfo->cur_FP_ = 0.0f;
    pDestInfo->max_FP_ = CalcFP((eCHAR_TYPE)pDestInfo->m_byClassCode);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    AlarmInvalidState("OnSetInfo Before SetStream");

    // 버전 관리 ---------------------------------------------------------------------------------------------------------------------------------------------
#if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
    int new_ac_version = 0;
    if (AreaConquest::ACStreamConverter(MAX_AC_SECTION_NUM,
                                        pQuerySrcInfo->m_iACVersion,
                                        const_cast<BYTE*>(pQuerySrcInfo->m_pACStream),
                                        new_ac_version, ac_stream_))
    {
        pDestInfo->m_iACVersion = new_ac_version;
    }
    else
    {
        ASSERT(FALSE);
    }
#endif
    pDestInfo->m_iMaxSD = pQuerySrcInfo->m_iMaxSD;
    pDestInfo->m_iSD    = pQuerySrcInfo->m_iSD;

    AlarmInvalidState("OnSetInfo End");

    return TRUE;
}

// (CHANGES) (f100324.1L) (WAVERIX) add interface for inventory stream alignment function \
// detached from 'OnSetInfo'.
// (WARNING) don't call directly current version (100324)
bool DBCharacter::OnSetInfoInventory(const sQUERY_CHARACTER* queried_char_info)
{
    BASE_PLAYERINFO* const dest_info = GetInfo();
    dest_info->m_iInventoryLock = queried_char_info->m_bInventoryLock;
    dest_info->m_ExtraInventoryTabCount = \
        static_cast<uint8_t>(queried_char_info->m_byExtraInventoryTabCount);

    // 나머지는 NULL로 초기화 될 것이다.
    strncpy(dest_info->m_InventoryPwd, queried_char_info->result_info_password_,
            MAX_WAREHOUSE_INVENTORY_PWD_LENGTH);

    //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
    ;{ //set inventory configuration
        nsSlot::InventoryConfig* inven_config = &dest_info->m_InventoryConfig;
        inven_config->configures[0] = queried_char_info->config01;
        inven_config->configures[1] = queried_char_info->config02;
        inven_config->configures[2] = queried_char_info->config03;
        inven_config->configures[3] = queried_char_info->config04;
        inven_config->configures[4] = queried_char_info->config05;
        inven_config->configures[5] = queried_char_info->config06;
        inven_config->configures[6] = queried_char_info->config07;
        inven_config->configures[7] = queried_char_info->config08;
        inven_config->configures[8] = queried_char_info->config09;
        inven_config->configures[9] = queried_char_info->config10;
    };
    //
    return true;
}

bool
DBCharacter::UpdateCharInfo()
{
    using nsQuery::DBQueryProxyManager;
    DBQueryProxyManager* const db_query_proxy_manager = DBQueryProxyManager::Instance();

    DBQueryProxyManager::eRequestResult result = db_query_proxy_manager->Request_Char_Update(\
        db_user_, this, nsQuery::DBQueryProxyCharNode::eTrans_Update_CharInfo);
    if (result != DBQueryProxyManager::eReqResult_Success) {
        return false;
    }
    return true;
}

bool
DBCharacter::UpdateInventory() //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
{
    using nsQuery::DBQueryProxyManager;
    DBQueryProxyManager* const db_query_proxy_manager = DBQueryProxyManager::Instance();

    DBQueryProxyManager::eRequestResult result = db_query_proxy_manager->Request_Char_Update(\
        db_user_, this, nsQuery::DBQueryProxyCharNode::eTrans_Update_InvenInfo);
    if (result != DBQueryProxyManager::eReqResult_Success) {
        return false;
    }
    return true;
}


bool DBCharacter::UpdateSubData() // skill & perk, ...
{
    bool changed_events = false;
    ;{
        bool cleared = db_query_node_.Update(db_query_node_.eUpdate_All);
        if (cleared == false) {
            changed_events = true;
        }
    };
    if (changed_events) {
        return false;
    }
    return true;
}


// f100928.2L, this routine support a nested data synchronization on all loaded data
bool DBCharacter::SynchronizeDataBeforeSendCharInfo(SERVER_CHARACTER_PART* const player_part)
{
    //__NA001084_PET_IMAGE_DRAW_IN_CHARACTER_SELECT_SCREEN
    // (f100527.4L) ...problem point...
    // (CHANGES) (f100603.4L) change a pet item synchronization routine 
    // to solve a database mismatch problem case.
    // CHANGES: f100817.3L, reduce a serializing process overhead
    // because of a logic of f100603.4L is processed
    // based on full inventory serializing to check a pet integrity check
    // CHANGES: f100928.2L, moved from a Serialize routine
    // CHANGES: f100904.7L, changed a pet item status validatation test logic
    // based on a tokenized item slot info table.
    player_part->m_bSummoned = player_info_->m_bSummonedPet;
    db_query_node_.LoadPetItemStream(&player_part->m_PetSlotStream);
    // TODO: should be synchronize the ether bullet information
    //
    return true;
}

void
DBCharacter::Serialize(SERVER_CHARACTER_PART& player_part, eSERIALIZE serialize_type)
{
    if (serialize_type == SERIALIZE_LOAD)
    {
        //
        const BASE_PLAYERINFO* player_info = GetInfo();
        player_part.m_CharGuid = player_info->m_CharGuid;
        player_part.m_bySlot   = player_info->m_bySlot;
        BOOST_STATIC_ASSERT(sizeof(player_part.m_tszCharName) == sizeof(player_info->m_tszCharName));
        _tcsncpy(player_part.m_tszCharName, player_info->m_tszCharName,
                 _countof(player_part.m_tszCharName));
        player_part.m_tszCharName[_countof(player_part.m_tszCharName) - 1] = _T('\0');
        //_tcsncpy(rPart.m_tszCharName, pInfo->m_tszCharName, MAX_CHARNAME_LENGTH);
        //rPart.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
        player_part.m_byHeight = player_info->m_byHeight;
        player_part.m_byFace   = player_info->m_byFace;
        player_part.m_byHair   = player_info->m_byHair;
        player_part.m_byClass  = player_info->m_byClassCode;
        player_part.m_LV       = player_info->m_LV;
        player_part.m_iRegion  = player_info->m_iRegion;
        player_part.m_sX       = player_info->m_sLocationX;
        player_part.m_sY       = player_info->m_sLocationY;
        player_part.m_sZ       = player_info->m_sLocationZ;
        player_part.m_ChangeOfClassStep = player_info->m_ChangeOfClassStep;
        player_part.m_CharState  = player_info->m_byCharState;
        player_part.m_GuildGuid  = player_info->m_GuildGuid;
        player_part.m_eGuildDuty = player_info->m_eGuildDuty;

        _tcsncpy(player_part.m_tszGuildNicName, player_info->m_tszGuildNickName, MAX_CHARNAME_LENGTH);
        player_part.m_tszGuildNicName[MAX_CHARNAME_LENGTH] = '\0';
        player_part.m_DelChk = delete_checked_;
        _tcsncpy(player_part.m_tszDelDate, delete_date_, MAX_SMALLDATETIME_SIZE);
        player_part.m_tszDelDate[MAX_SMALLDATETIME_SIZE] = '\0';
#ifdef _NA_002050_20110216_ADD_GENDER
        player_part.gender = player_info->gender;
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
		player_part.honor_title = player_info->honor_title_;
#endif //_NA_003027_20111013_HONOR_SYSTEM
        //_NA_004035_20120227_COSTUME_ITEM
        player_part.is_equipped_costume = player_info->is_equipped_costume_;
        player_part.costume_item_pos = player_info->costume_item_pos_;
        player_part.costume_item_code = player_info->costume_item_code_;

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
        player_part.is_equipped_deco_costume = player_info->is_equipped_decorate_costume_;
        player_part.deco_costume_item_pos = player_info->decorate_costume_item_pos_;
        player_part.deco_costume_item_code = player_info->decorate_costume_item_code_;
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

        ;{  // (f100527.4L)
            _EQUIP_TOTAL_INFO* equip_block = &player_part.m_EquipItemInfo;
            SerializeItemStreamLoad(SI_EQUIPMENT,
                                    equip_block->m_Slot, &equip_block->m_Count);
        };

        // f100929.4L, remove an unused section
        //STATE_GM_INFO rStateGMInfo;
        //SerializeStateInfo(player_part.m_TotalStateInfo, rStateGMInfo, SERIALIZE_LOAD);
        //
        // CHANGES: f100928.2L, moved logic to SynchronizeDataBeforeSendCharInfo
        // about a synchrozizing data like a pet item validity
        this->SynchronizeDataBeforeSendCharInfo(&player_part);
    }
    else if (serialize_type == SERIALIZE_STORE)
    {
        ASSERT(!"UnExpected Code");
    }
}

// CHANGES, f100817.2L, move to a code-file
//                      split a composed SerializeItemStream method to it of load and it of store
void DBCharacter::SerializeItemStreamLoad(SLOTIDX slot_index,
                                          ITEMSLOTEX_INSERVER* INOUT slot_array,
                                          POSTYPE* result_number_of_slots)
{
    db_query_node_.LoadItemSlotInfo(slot_index, slot_array, result_number_of_slots);
    return;
}

// CHANGES, f100817.2L, move to a code-file
//                      split a composed SerializeItemStream method to it of load and it of store
void DBCharacter::SerializeItemStreamStore(SLOTIDX slot_index,
                                           const ITEMSLOTEX_INSERVER* slot_array,
                                           POSTYPE number_of_slots)
{
    db_query_node_.StoreItemSlotInfo(slot_index, slot_array, number_of_slots);
    return;
}


void
DBCharacter::SerializeStateInfo(STATE_DETAIL_TOTAL_INFO& INOUT state_total_info,
                                STATE_GM_INFO& INOUT state_gm_info, eSERIALIZE serialize_type)
{
    if (serialize_type == SERIALIZE_LOAD)
    {
        db_query_node_.LoadCharStates(&state_total_info);
        state_gm_info = state_gm_info_;
    }
    else if (serialize_type == SERIALIZE_STORE)
    {
        db_query_node_.StoreCharStates(state_total_info);
        state_gm_info_ = state_gm_info;
    }
}


#if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
void
DBCharacter::SerializeACStream(BYTE* stream, WORD size, eSERIALIZE serialize_type)
{
    ASSERT(size == MAX_ACSTREAM_SIZE);

    if (serialize_type == SERIALIZE_STORE) {
        memcpy(ac_stream_, stream, MAX_ACSTREAM_SIZE);
    }
    else if (serialize_type == SERIALIZE_LOAD) {
        memcpy(stream, ac_stream_, size);
    }
}
#endif

void
DBCharacter::MakeQuery_Char_Update(Query_Char_Update* query)
{
    if (AlarmInvalidState("MakeQuery_Char_Update")) {
        return;
    }

    time_t cur_time = 0;
    util::TimeSync::time(&cur_time);

    BOOLEAN isPcRoom = !!db_user_->IsPCRoom();

    int iPlayTime = 0, iEventTime = 0;
    //{__NA000000_090506_EVENT_TYPE_MODIFICATION__
    if (isPcRoom == 0)
    {   // 일반 유저 이벤트 타임 처리
        iPlayTime = (int)abs(difftime(cur_time, event_last_updated_time_) / 60);
        event_last_updated_time_ = cur_time;
        event_use_time_normal_ += iPlayTime;
    }
    //}
    if (isPcRoom)
    {
        iEventTime = (int)abs(difftime(cur_time, event_last_updated_time_) / 60);
        event_last_updated_time_ = cur_time;
        event_use_time_ += iEventTime;
    }
    int itotal_play_time = (int)abs(difftime(cur_time, login_time_) / 60);
    //DISPMSG("총 플레이한 시간: %d분\n", itotal_play_time);
    //==============================================================================================
    //
    Query_Char_Update::sQueryParam* const parameter = &query->parameter_[0];
    //
    ;{  // setup update query data.
        parameter->result_ = 1; // to check success result validation
        //
        parameter->m_UserGuid = player_info_->m_UserGuid; //@UserGuid int
        parameter->m_CharGuid = player_info_->m_CharGuid;        //@CharGuid int
        // added by _NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM = {
        parameter->m_byHeight = player_info_->m_byHeight; //@Height tinyint,
        parameter->m_byFace = player_info_->m_byFace; //@Face tinyint,
        parameter->m_byHair = player_info_->m_byHair; //@Hair tinyint,
        // }
        parameter->m_LV = player_info_->m_LV;  //@LV int ,
        parameter->m_sStrength = player_info_->m_sStrength; //@Strength smallint  ,
        parameter->m_sDexterity = player_info_->m_sDexterity; //@Dexterity smallint  ,
        parameter->m_sVitality = player_info_->m_sVitality; //@Vitality smallint  ,
        parameter->m_sInteligence = player_info_->m_sInteligence; //@Inteligence smallint  ,
        parameter->m_sSpirit = player_info_->m_sSpirit; //@Spirit smallint  ,
        parameter->m_sSkillStat1 = player_info_->m_sSkillStat1; //@SkillStat1 smallint  ,
        parameter->m_sSkillStat2 = player_info_->m_sSkillStat2; //@SkillStat2 smallint  ,
        parameter->m_UserPoint = player_info_->m_UserPoint;
        parameter->m_i64Exp = player_info_->m_i64Exp;    //@Exp bigint ,
        parameter->m_iMaxHP = player_info_->m_iMaxHP; //@MaxHP real  ,
        parameter->m_iHP = player_info_->m_iHP; //@HP real  ,
        parameter->m_iMaxMP = player_info_->m_iMaxMP; //@MaxMP real  ,
        parameter->m_iMP = player_info_->m_iMP; //@MP real  ,
        parameter->m_Money = player_info_->m_Money; //@Money int  ,
        parameter->m_iRemainStat = player_info_->m_iRemainStat; //@RemainStat int  ,
        parameter->m_iRemainSkill = player_info_->m_iRemainSkill; //@RemainSkill int  ,
        parameter->m_iSelectStyleCode = player_info_->m_iSelectStyleCode; //@RightSelect tinyint  ,
        parameter->m_byPKState = player_info_->m_byPKState;//@PKState tinyint  ,
        parameter->m_byCharState = player_info_->m_byCharState;//@CharState tinyint  ,
        parameter->m_StateTime = player_info_->m_StateTime;//@StateTime bigint,
        parameter->m_iRegion = player_info_->m_iRegion; //@Region int  ,
        parameter->m_sX = player_info_->m_sLocationX;//@LocationX smallint  ,
        parameter->m_sY = player_info_->m_sLocationY; //@LocationY smallint  ,
        parameter->m_sZ = player_info_->m_sLocationZ; //@LocationZ smallint  ,
        parameter->m_iPreRegion = player_info_->m_iPreRegion; //@PreRegion int  ,
        parameter->param_info_title_id_; // @TitleID varchar (10), empty value, no-control
        parameter->m_TitleTime = player_info_->m_TitleTime; //@TitleTime bigint  ,
        parameter->m_byInvisibleOpt = player_info_->m_byInvisibleOptFlag;//@ VisualOpt tinyint  ,
        // CHANGES: f100924.2L, change to dummy value
        // { _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
        // @InventoryLock bit,
        // @InventoryItem varbinary (5250)
        // @TempInventoryItem varbinary (340),
        // @EquipItem varbinary (840)
        // }
        // @Skill varbinary (200)
        // @Quick varbinary (192)
        // @Style varbinary (8)
        // }
#if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        parameter->m_pACStream; // @ACStream varbinary (28) ,
#endif
        parameter->m_byChaoState = player_info_->m_ChaoState;  //tinyint
        parameter->m_i64ChaoTime = player_info_->m_ChaoEndTime; //int64
        //
        parameter->m_iPlayLimitedTime = player_info_->m_PlayLimitedTime; //int
        //
        parameter->m_EventUseTime = iEventTime; // accumulated value of playing time in pc room
        parameter->m_EventUseTimeNormal = iPlayTime; //__NA000000_090506_EVENT_TYPE_MODIFICATION__
        //
        parameter->m_ExtraEquipCashSlotCount = player_info_->m_ExtraEquipCashSlotCount;
        parameter->m_bTradeFlag = (player_info_->m_bTradeFlag != 0);
        parameter->m_bWhisperFlag = (player_info_->m_bWhisperFlag != 0);
        parameter->m_byFreeInitCount = player_info_->m_byFreeInitCount; 
        parameter->m_ChangeOfClassStep = player_info_->m_ChangeOfClassStep;

        parameter->m_bSummonedPet = (player_info_->m_bSummonedPet != 0);
        parameter->m_bMAXIntimacy = (player_info_->m_bMAXIntimacy != 0);
        parameter->m_PetItemPos = player_info_->m_PetItemPos;
        parameter->m_bMAXOptionRemainTime = player_info_->m_bMAXOptionRemainTime;    

        parameter->m_bActiveEther = (player_info_->m_bActiveEtherDevice != 0);
        parameter->m_byEtherBulletPos = player_info_->m_byEtherBulletPos;
#if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        parameter->m_iACVersion = player_info_->m_iACVersion;
#endif
        //{__NA_001290_20090525_SHIELD_SYSTEM
        parameter->m_iMaxSD = player_info_->m_iMaxSD;
        parameter->m_iSD = player_info_->m_iSD;
        //}
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
        parameter->m_wAccumulateYear = player_info_->m_wAccumulateYear;
        parameter->m_dwAccumulateTime = player_info_->m_dwAccumulateTime;
        parameter->m_wAccumulatePoint = player_info_->m_wAccumulatePoint;
#endif
#ifdef _NA_002050_20110216_ADD_GENDER
        parameter->gender = player_info_->gender;
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
		parameter->honor_title = player_info_->honor_title_;
#endif //_NA_003027_20111013_HONOR_SYSTEM
        //_NA_004035_20120227_COSTUME_ITEM
        parameter->is_equipped_costume = player_info_->is_equipped_costume_ ? 1 : 0;
        parameter->costume_item_pos = player_info_->costume_item_pos_;
        parameter->costume_item_code = player_info_->costume_item_code_;

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
        parameter->is_equipped_deco_costume = player_info_->is_equipped_decorate_costume_ ? 1 : 0;
        parameter->deco_costume_item_pos = player_info_->decorate_costume_item_pos_;
        parameter->deco_costume_item_code = player_info_->decorate_costume_item_code_;
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

    }; //end 'setup update query data.'
    ;{  // NOTE: lower bound holder for more easy MAJOR version control
        BOOST_STATIC_ASSERT(NATION_BUILD_VERSION >= 1002);
    };
    //
    STRING_SAFER_QUERY512 query_buffer;

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    //_NA_004035_20120227_COSTUME_ITEM
    const TCHAR* query_arguments_format =
        //  [0][1][2][3][4][5][6][7][8][9]
        _T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ")
        _T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ")
        _T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ")
        _T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ")
        _T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ")
        _T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?");
    BOOST_STATIC_ASSERT(query->UPDATE_CHARACTER_PARAM_COL_NUM == 63 + 1);
#else
    //_NA_004035_20120227_COSTUME_ITEM
    const TCHAR* query_arguments_format =
        //  [0][1][2][3][4][5][6][7][8][9]
        _T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ")
        _T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ")
        _T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ")
        _T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ")
        _T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ")
        _T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?");
    BOOST_STATIC_ASSERT(query->UPDATE_CHARACTER_PARAM_COL_NUM == 60 + 1);
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

    query_buffer.MakeString(_T("{?=call S_Char_Update(%s)}"), query_arguments_format);
    query->SetQuery(query_buffer);
    //
#if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
    SerializeACStream(parameter->m_pACStream, MAX_ACSTREAM_SIZE, SERIALIZE_LOAD);
#endif
}

//_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
void DBCharacter::MakeQuery_Char_Update_Inventory(Query_Char_Update_Inventory* query)
{
    Query_Char_Update_Inventory::sQueryParam* const param = &query->parameters_[0];
    param->result_ = 1; // dummy value to support result application check

    const BASE_PLAYERINFO* player_info = this->player_info_;
    const nsSlot::InventoryConfig& inven_config = player_info->m_InventoryConfig;
    ;{  // f101006.2L, initialize parameters
        param->char_guid_ = player_info->m_CharGuid;
        param->inventory_lock_ = (player_info->m_iInventoryLock != 0);
        param->extra_inven_tab_count_ = player_info->m_ExtraInventoryTabCount;
        BOOST_STATIC_ASSERT(_countof(param->password_) ==
                            _countof(player_info_->m_InventoryPwd));
        strncpy(param->password_, player_info_->m_InventoryPwd, _countof(param->password_));
        assert(param->password_null_terminated_[0] == '\0');
        param->config_version = inven_config.kConfigVersion;
        param->config01 = inven_config.configures[0];
        param->config02 = inven_config.configures[1];
        param->config03 = inven_config.configures[2];
        param->config04 = inven_config.configures[3];
        param->config05 = inven_config.configures[4];
        param->config06 = inven_config.configures[5];
        param->config07 = inven_config.configures[6];
        param->config08 = inven_config.configures[7];
        param->config09 = inven_config.configures[8];
        param->config10 = inven_config.configures[9];
    };

    // changes, f100818.1L, change a SerializeItemStream to SerializeBinaryStream
    //
    STRING_SAFER_QUERY256 query_string;
    // (CHANGES) (f100518.2L) updates an update logic
    // which is related to the 3rd changed db query specification
    query_string.MakeString(_T("{?=call S_Char_Update_Inventory (?,?,?,?,?,")
                            _T("?,?,?,?,?,?,?,?,?,?)}"));
    query->SetQuery(query_string);
    query->SetUserKey(player_info->m_UserGuid);
}

