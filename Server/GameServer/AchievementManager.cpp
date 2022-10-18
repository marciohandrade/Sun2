#include "StdAfx.h"
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
#include ".\AchievementManager.h"
#include ".\RewardManager.h"
#include "ItemManager.h"
#include "EquipmentSlotContainer.h"

AchievementManager::AchievementManager(void) 
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
: is_initialize_levelup_ac_(FALSE)
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
{
	achievement_search_ = AchievementSearch::Instance();
}

AchievementManager::~AchievementManager(void)
{	
}

void AchievementManager::Init( Player * player )
{
	achievement_.Init(this);
	player_ = player;
}

void AchievementManager::Release()
{
	is_initialize_levelup_ac_ = FALSE;
}

BOOL AchievementManager::IncCurseTowerValue( WORD object_key)
{	
	Achievement_SearchInfo* search_info = NULL;

	achievement_search_->SetFirst();
	
	while(achievement_search_->GetNext(&search_info))
	{
		if(search_info->parser_info_->obejcts_[0].object_code_ == object_key)
		{
			achievement_.SetAchievementValue(search_info->parser_info_,
				search_info->index_,
				1, ACHIEVEMENT_VALUE_APPEND);
		}
	}

	return TRUE;
}

BOOL AchievementManager::IncMissionValue( WORD object_code)
{
	return IncConditionValue(OBJECT_TYPE_CODE,  0, object_code);
}

BOOL AchievementManager::IncConditionValue( BYTE type ,WORD object_key, WORD object_code )
{

	Achievement_SearchInfo* achievement_parserInfo = NULL;
	if(type == OBJECT_TYPE_KEY)
		achievement_parserInfo = achievement_search_->GetAchievement_SearchInfo(object_key);
	else if(type == OBJECT_TYPE_CODE)
		achievement_parserInfo = achievement_search_->GetAchievement_SearchInfo(object_code);
	else
		return FALSE;


	if(achievement_parserInfo == NULL)
	{
		SUNLOG(eCRITICAL_LOG, "Not Found ObjectCode (%d)", object_key);
		return FALSE;
	}

#ifdef _NA_001990_ACSYSTEM_ADD
    return SetAchievementValue(achievement_parserInfo, 1, ACHIEVEMENT_VALUE_APPEND);
#else
	return achievement_.SetAchievementValue(achievement_parserInfo->parser_info_,
											achievement_parserInfo->index_,
											1, ACHIEVEMENT_VALUE_APPEND);
#endif //_NA_001990_ACSYSTEM_ADD
}

BOOL AchievementManager::AddConditionValue( BYTE type ,WORD object_key ,WORD object_value )
{
	Achievement_SearchInfo* achievement_parserInfo = 
		achievement_search_->GetAchievement_SearchInfo(object_key);

	if(achievement_parserInfo == NULL)
	{
		SUNLOG(eCRITICAL_LOG, "Not Found ObjectCode (%d)", object_key);
		return FALSE;
	}

	return achievement_.SetAchievementValue(achievement_parserInfo->parser_info_,
											achievement_parserInfo->index_,
											object_value, ACHIEVEMENT_VALUE_APPEND);
}

BOOL AchievementManager::SetConditionValue( WORD location ,WORD index ,BYTE object_index, WORD value)
{
	return achievement_.SetAchievementValue(location, index, object_index, value,
											ACHIEVEMENT_VALUE_SET);
}

BOOL AchievementManager::ClearAC( WORD location_code )
{
	return achievement_.InitAchievementPart(location_code);
}

BOOL AchievementManager::CanCollectTreasureBox(const int location_code)
{
    return achievement_.CanCollectTreasureBox((WORD)location_code);
}

void AchievementManager::Serialize( Character_Achievements& achievement_info, ACHIEVEMENT_SERIALIZE eType )
{
	achievement_.Serialize(achievement_info, eType);
}

void AchievementManager::RewardAchievement(WORD index_number)
{
    Achievement_ParserInfo* achievement_parserinfo = \
        AchievementParser::Instance()->GetAchievement_ParserInfo(index_number);
    if (achievement_parserinfo == NULL) {
        return;
    }

    WORD location_code = achievement_parserinfo->location_code_;

    //유저의 기록을 가져와서
    AchievementPart* achievement_part = achievement_.GetAchievementPart(location_code);
    if(!achievement_part)
        return;

    //유저의 기록을 가져와서
    AchievementInfo* achievementinfo = achievement_part->GetAchievement(index_number);
    if(!achievementinfo)
        return;

    // 이미 보상을 받았음
    if (achievementinfo->IsReward() == true) {
        SUNLOG(eCRITICAL_LOG, "[%s:%d] already got rewarded - %d, %s", \
            __FUNCTION__, __LINE__, index_number, player_->GetCharName());
        return;
    }

    //////////////////////////////////////////////////////////////////////////
    //
	if(!achievement_.RewardAchievement(index_number, location_code, TRUE))
	{
		SendRewardFail(RC::RC_AC_FAILED);
        return;
	}
}

void AchievementManager::OnCanCollectTreasureBox()
{
    player_->SendAllFieldCollectionInfo();
}

void AchievementManager::OnValueChanged(WORD index, WORD location_code, BYTE object_index, 
										WORD previous_value, WORD current_value)
{
	char add_info_string[MAX_ADDINFO_SIZE+1];
	_sntprintf( add_info_string, MAX_ADDINFO_SIZE, 
		"index:%d, object_Index:%d, previous_value:%d, current_value:%d", 
		index, object_index, previous_value, current_value );

    bool write_log = true;
#ifdef _NA_000000_20131031_GAMELOG_REDUCE
    Achievement_SearchInfo* achievement_parserInfo = achievement_search_->GetAchievement_SearchInfo(index);
    if (achievement_parserInfo)
    {
        BYTE object_type = achievement_parserInfo->parser_info_->object_type_;
        if ( object_type == ACHIEVEMENT_OBJECT_KILL_ALL && current_value % 10 != 0) {
            write_log = false;
        }
    }
#endif //_NA_000000_20131031_GAMELOG_REDUCE
    
    if (write_log) {
	    GAMELOG->LogSimpleAction(ACT_ACHIEVEMENT_VALUE_CHANGED, player_, add_info_string);
    }

	MSG_CG_AREACONQUEST_VALUE_CHANGE_CMD msg;	
	msg.index_number_ = index;
	msg.location_code = location_code;
	msg.object_index_ = object_index;
	msg.object_value_ = current_value;

	player_->SendPacket( &msg, sizeof(msg) );
}

void AchievementManager::Onreward(WORD index, WORD location_code, 
                                  Achievement_ParserInfo_Reward  rewards_[MAX_ACHIEVEMENT_REWARD], 
                                  bool is_reward, RC::eAC_RESULT rc)
{
	if(is_reward)
	{
		if(rc == RC::RC_AC_SUCCESS)
		{
			//보상해 주자
			for(int i = 0 ; i < MAX_ACHIEVEMENT_REWARD ; ++i)
			{
				Reward(index, location_code, rewards_[i].reward_type_, rewards_[i].reward_code_);
			}
		}
		else
		{
			SendRewardFail(rc);
		}
	}
	else //보상 안받기로 변경한 경우 (GM 명령어 처리 빼고는 업는 경우~
	{
        SUNLOG(eCRITICAL_LOG, "[%s:%d] AC reward get reverted - GM or abuser %d,%s", 
            __FUNCTION__, __LINE__,
            player_->GetCharGuid(), player_->GetCharName());
	}
}

void AchievementManager::Reward(WORD index, WORD location_code, WORD reward_type, WORD reward_code)
{
	if(reward_type == ACHIEVEMENT_REWARD_NONE) {} //보상 없음	
	else//rewardinfo.txt 보상
	{
		MSG_CG_AREACONQUEST_REWARD_ACK msg;        
        msg.index_ = index;
        msg.location_code_ = location_code;
        msg.m_accomplishment_ratio = 0;
        msg.m_ItemInfo.Clear();
        msg.m_ItemInfo.m_InvenCount = 0;
        msg.m_ItemInfo.m_TmpInvenCount = 0;
        msg.m_i64Exp = 0;
        msg.m_Money = 0;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        msg.honor_title = 0;
#endif //_NA_003027_20111013_HONOR_SYSTEM

		if(reward_type == ACHIEVEMENT_REWARD_ACCOMPLISHMENT) //달성률 보상
		{
			achievement_.AddAccomplishmentRation(location_code, reward_code);
			msg.m_accomplishment_ratio = reward_code;			

            if(msg.m_accomplishment_ratio <= 0)
            {
                SUNLOG(eCRITICAL_LOG, "Achievement accomplishemnt reward fail");
                return ;
            }
		}
		else //reward_type = ACHIEVEMENT_REWARD_CODE
		{
            MONEY PreMoney = player_->GetMoney();
            EXPTYPE OldExp = player_->GetExp();

            GAMELOG->SetAchievementInfo(location_code, index);

            RC::eREWARD_RESULT rt = g_RewardManager.RewardItem( RewardManager::eREWARD_AC, 
                reward_code, 0, player_, msg.m_ItemInfo );
            if( rt != RC::RC_REWARD_SUCCESS )
            {
                // TODO: need result code converter to support convert reward result to ac result.
                SendRewardFail(RC::RC_AC_INSUFFICIENT_SPACE); 
                if(!achievement_.RewardAchievement(index, location_code, false))
                {
                    SUNLOG(eCRITICAL_LOG, "reward rollback fail");
                }
                return;
            }

            rt = g_RewardManager.RewardExp( reward_code, player_, msg.m_i64Exp );
            ASSERT( rt == RC::RC_REWARD_SUCCESS );
            rt = g_RewardManager.RewardHime( reward_code, player_, msg.m_Money );
            ASSERT( rt == RC::RC_REWARD_SUCCESS );
            rt = g_RewardManager.RewardSkill( reward_code, player_, msg.m_SkillCode );
            ASSERT( rt == RC::RC_REWARD_SUCCESS );

            if( (msg.m_Money-PreMoney) != 0 )
                GAMELOG->LogMoney( MONEY_REWARD_AC, player_, (msg.m_Money-PreMoney) );

            if( (msg.m_i64Exp-OldExp) != 0 )
                GAMELOG->LogExp( EXP_AC, player_, msg.m_i64Exp, OldExp );

#ifdef _NA_003027_20111013_HONOR_SYSTEM
            rt = g_RewardManager.RewardHonorTitle(reward_code, player_, msg.honor_title);
#endif //_NA_003027_20111013_HONOR_SYSTEM

            GAMELOG->SetAchievementInfo(0, 0); // AC log info clear...
		}

		player_->SendPacket( &msg, msg.GetSize() );
	}
	
}

void AchievementManager::SendRewardFail(RC::eAC_RESULT rc)
{
	MSG_CG_AREACONQUEST_REWARD_NAK msg;
	msg.m_byErrorCode = rc;
	player_->SendPacket( &msg, sizeof(msg) );
}

#ifdef _NA_001990_ACSYSTEM_ADD
BOOL AchievementManager::ProcessItemUse(SLOTCODE Code, WORD ACCode, BYTE ACReferenceID)
{
    return IncConditionValue(ACHIEVEMENT_OBJECT_ITEM, (ACCode == OBJECT_TYPE_CODE) ? Code : ACReferenceID);
}

BOOL AchievementManager::ProcessItemObtain(SLOTCODE Code, WORD ACCode, BYTE ACReferenceID, WORD count)
{
    return IncConditionValue(ACHIEVEMENT_OBJECT_ITEM_GET, (ACCode == OBJECT_TYPE_CODE) ? Code : ACReferenceID, count);
}

BOOL AchievementManager::ProcessItemEquip( SLOTCODE Code, WORD ACCode, BYTE ACReferenceID )
{
    WORD keycode = (ACCode == OBJECT_TYPE_CODE) ? Code : ACReferenceID;

    AchievementSearch::ACVectorElem* achievement_parserInfo_vector = achievement_search_->GetSearchInfoVector_find(ACHIEVEMENT_OBJECT_ITEM_EQUIP, keycode);
    if (!achievement_parserInfo_vector)
        return FALSE;

    CEquipmentSlotContainer* const equip_container = static_cast<CEquipmentSlotContainer*>(
        player_->GetItemManager()->GetItemSlotContainer(SI_EQUIPMENT));

    const ns_formula::EquipSlotsManageInfo* manage_info = equip_container->GetEquipmentManageInfo();

    WORD equipcount = (ACCode == OBJECT_TYPE_CODE) ? 
        manage_info->GetEquipCount(keycode) : manage_info->GetEquipACGroupCount(keycode);

    AchievementSearch::ACVectorElem::iterator elem = (*achievement_parserInfo_vector).begin(), elem_end = (*achievement_parserInfo_vector).end();

    for( ; elem != elem_end; ++elem)
    {
        Achievement_SearchInfo* pinfo = &(*elem);
        AchievementObjectInfo objectInfo;
        if( GetAchievement(pinfo, objectInfo) )
        {
            WORD script_value   = objectInfo.Achievement_;
            WORD user_record    = objectInfo.value_;
            WORD user_value     = min(script_value, equipcount);

            if( user_record != user_value)
                SetAchievementValue( pinfo, user_value, ACHIEVEMENT_VALUE_SET );
        }
    }

    return TRUE;
}

BOOL AchievementManager::ProcessPerkLevel( WORD id, WORD level )
{
    SetValueMax_if(ACHIEVEMENT_OBJECT_PERK_LEVEL, id, level);

    return TRUE;
}

BOOL AchievementManager::ProcessMissionClearTime( WORD missioncode, WORD cleartime )
{
    SetValueMax_if(ACHIEVEMENT_OBJECT_MISSION_CLEAR_TIME, missioncode, cleartime);

    return TRUE;
}

BOOL AchievementManager::ProcessLevelUp( WORD level )
{
#ifdef _NA_008166_20150330_MODIFY_AC_LEVELUP
    if (GetFlag_InitializeLevelUpAC() == FALSE) {
        _SetFlag_InitializeLevelUpAC();
    }

    return IncConditionValue_LevelUpAC( level );
#else
    return IncConditionValue( ACHIEVEMENT_OBJECT_LEVELUP, level );
#endif //_NA_008166_20150330_MODIFY_AC_LEVELUP
}

BOOL AchievementManager::ProcessSkillStepUp( WORD line )
{
    const ACHIEVEMENT_OBJECT_TYPE type = ACHIEVEMENT_OBJECT_SKILL_STEP;

    AchievementSearch::ACMapElem* rack = achievement_search_->GetSearchInfo_find(type);
    if (!rack)
        return FALSE;

    AchievementSearch::ACMapElem::iterator elem = (*rack).begin(), elem_end = (*rack).end();

    for( ; elem != elem_end; ++elem)
    {
        WORD keycode = elem->first;
        if(keycode <= line)
            IncConditionValue( type, keycode );
    }

    return TRUE;
}

BOOL AchievementManager::ProcessACClear( WORD index )
{
    return IncConditionValue( ACHIEVEMENT_OBJECT_AC_CLEARE, index );
}

BOOL AchievementManager::ProcessQuest(QCODE Code, WORD ACCode, BYTE ACReferenceID)
{
    return IncConditionValue(ACHIEVEMENT_OBJECT_QUEST, (ACCode == OBJECT_TYPE_CODE) ? Code : ACReferenceID);
}

void AchievementManager::SetValueZero_if(ACHIEVEMENT_OBJECT_TYPE type, WORD keycode)
{
    // 보상을 받지 않은 AC에 대하여
    // 달성 목표값에 도달하지 못했을 경우, 값을 초기화한다.

    AchievementSearch::ACVectorElem* achievement_parserInfo_vector = achievement_search_->GetSearchInfoVector_find(type, keycode);
    if (!achievement_parserInfo_vector)
        return;

    AchievementSearch::ACVectorElem::iterator elem = (*achievement_parserInfo_vector).begin(), elem_end = (*achievement_parserInfo_vector).end();

    for( ; elem != elem_end; ++elem)
    {
        Achievement_SearchInfo* pinfo = &(*elem);
        AchievementObjectInfo objectInfo;
        if( GetAchievement(pinfo, objectInfo) )
        {
            WORD script_value   = objectInfo.Achievement_;
            WORD user_record    = objectInfo.value_;

            if( user_record < script_value)
                SetAchievementValue( pinfo, 0, ACHIEVEMENT_VALUE_SET );
        }
    }
}

void AchievementManager::SetValueMax_if(ACHIEVEMENT_OBJECT_TYPE type, WORD keycode, DWORD value, WORD scale)
{
    // 보상을 받지 않은 AC에 대하여
    // value를 scale로 나눈뒤, 달성 목표값보다 크면 목표값으로 설정
    // 3 argument : type, keycode, value.
    AchievementSearch::ACVectorElem* achievement_parserInfo_vector = achievement_search_->GetSearchInfoVector_find(type, keycode);
    if (!achievement_parserInfo_vector)
        return;

    AchievementSearch::ACVectorElem::iterator elem = (*achievement_parserInfo_vector).begin(), elem_end = (*achievement_parserInfo_vector).end();

    for( ; elem != elem_end; ++elem)
    {
        Achievement_SearchInfo* pinfo = &(*elem);
        AchievementObjectInfo objectInfo;
        if( GetAchievement(pinfo, objectInfo) )
        {
            WORD script_value   = objectInfo.Achievement_;
            WORD user_record    = objectInfo.value_;

            value = value / scale;

            if( user_record != script_value && value >= script_value)
            {
                SetAchievementValue( pinfo, script_value, ACHIEVEMENT_VALUE_SET );
            }
        }
    }
}

void AchievementManager::SetValue_if(ACHIEVEMENT_OBJECT_TYPE type, /*WORD keycode,*/ WORD value)
{
    // 2 argument : type, value. (no keycode)
    AchievementSearch::ACMapElem* rack = achievement_search_->GetSearchInfo_find(type);
    if (!rack)
        return;

    AchievementSearch::ACMapElem::iterator elem = (*rack).begin(), elem_end = (*rack).end();

    for( ; elem != elem_end; ++elem) // for every keycode...
    {
        AchievementSearch::ACVectorElem::iterator \
            elem2 = (elem->second).begin(), elem2_end = (elem->second).end();

        for( ; elem2 != elem2_end; ++elem2)
        {
            Achievement_SearchInfo* pinfo = &(*elem2);
            AchievementObjectInfo objectInfo;
            if( GetAchievement(pinfo, objectInfo) )
            {
                WORD script_value   = objectInfo.Achievement_;
                WORD user_record    = objectInfo.value_;

                if( user_record != script_value && value <= script_value)
                {
                    SetAchievementValue( pinfo, value, ACHIEVEMENT_VALUE_SET );
                }

                //!~ 미완료 처리된 AC를 완료 해준다. arycoat - 2012.03.29
                if (user_record != script_value && value > script_value) 
                {
                    SetAchievementValue( pinfo, script_value, ACHIEVEMENT_VALUE_SET );
                }
                //~!
            }
        }
    }
}

void AchievementManager::ProcessBattleEnter(WORD mapcode)
{
    SetValueZero_if(ACHIEVEMENT_OBJECT_PC_KILL, mapcode);   // PC Kill 초기화
    SetValueZero_if(ACHIEVEMENT_OBJECT_NPC_KILL, mapcode);  // NPC Kill 초기화
    SetValueZero_if(ACHIEVEMENT_OBJECT_KILL, mapcode);      // 전체 Kill 초기화
}

void AchievementManager::ProcessBattleResult(WORD mapcode, DWORD heal, WORD maxDamage, DWORD damageToMonster, DWORD damageByMonster, WORD countKill, WORD countDead)
{
    SetValueMax_if(ACHIEVEMENT_OBJECT_BATTLE_HEAL, mapcode, heal, 1000);
    SetValueMax_if(ACHIEVEMENT_OBJECT_BATTLE_DAMAGE_TOP, mapcode, maxDamage);
    SetValueMax_if(ACHIEVEMENT_OBJECT_BATTLE_DAMAGE_TO_MONSTER, mapcode, damageToMonster, 1000);
    SetValueMax_if(ACHIEVEMENT_OBJECT_BATTLE_DAMAGE_BY_MONSTER, mapcode, damageByMonster, 1000);
    SetValueMax_if(ACHIEVEMENT_OBJECT_BATTLE_KILL, mapcode, countKill);
    SetValueMax_if(ACHIEVEMENT_OBJECT_BATTLE_DEAD, mapcode, countDead);
}

BOOL AchievementManager::IncConditionValue( ACHIEVEMENT_OBJECT_TYPE actype, WORD key, WORD count )
{
    AchievementSearch::ACVectorElem* achievement_parserInfo_vector = achievement_search_->GetSearchInfoVector_find( actype, key );
    if (!achievement_parserInfo_vector)
        return FALSE;

    AchievementSearch::ACVectorElem::iterator elem = (*achievement_parserInfo_vector).begin(), elem_end = (*achievement_parserInfo_vector).end();

    for( ; elem != elem_end; ++elem)
    {
        Achievement_SearchInfo* pinfo = &(*elem);
        AchievementObjectInfo objectInfo;
        if( GetAchievement(pinfo, objectInfo) )
        {
            WORD script_value   = objectInfo.Achievement_;
            WORD user_record    = objectInfo.value_;

            if( user_record != script_value)
            {
                SetAchievementValue(pinfo, count, ACHIEVEMENT_VALUE_APPEND);
            }
        }
    }

    return TRUE;
}

BOOL AchievementManager::GetAchievement(Achievement_SearchInfo* achievement_parserInfo, OUT AchievementObjectInfo& object_info)
{
    Achievement_ParserInfo* parser_info = achievement_parserInfo->parser_info_;
    WORD& location_code = parser_info->location_code_;
    WORD& index_number  = parser_info->index_;
    BYTE& object_index  = achievement_parserInfo->index_;

    AchievementPart* achievement_part = achievement_.GetAchievementPart(location_code);
    if(!achievement_part)
        return FALSE;

    //유저의 기록을 가져와서
    AchievementInfo* achievementinfo = achievement_part->GetAchievement(index_number);
    if(!achievementinfo)
        return FALSE;

    if(achievementinfo->IsReward()) // 보상 받았으므로 더이상 진행할 필요가 없다.
        return FALSE;

    ////////////////////////////////////////////////////////////////////////////
    // 선행 조건 검사
    // 1. 클래스 검사
    if(parser_info->class_type_ > 0)
    {
        if( player_->GetCharType() != parser_info->class_type_ )
            return FALSE;
    }

    // 2. 선행 AC완료 여부 검사
    // 선행 AC완료 여부 검사는 지금 적용 안함...ㅜㅜ;
    /*for(int i = 0 ; i < MAX_ACHIEVEMENT_PARENT_CODE ; ++i)
    {
        if (parser_info->parent_ac_code_[i] > 0 && 
            !achievement_.IsAccomplishmentByIndex(parser_info->parent_ac_code_[i]))
        {
            return FALSE;
        }
    }*/

    object_info = achievementinfo->GetObjectInfo(object_index);

    return TRUE;
}

BOOL AchievementManager::SetAchievementValue(Achievement_SearchInfo* achievement_parserInfo, WORD value, ACHIEVEMENT_VALUE_CHANGE_TYPE type)
{
    BOOL ret = achievement_.SetAchievementValue(achievement_parserInfo->parser_info_, achievement_parserInfo->index_, value, type);

    if( ret )
    {
        if( achievement_.IsAccomplishment(achievement_parserInfo->parser_info_) )
        {
            //보상없는 AC가 완료 되었을경우
            if (achievement_parserInfo->parser_info_->rewards_count_ == 0)
            {
                WORD index = achievement_parserInfo->parser_info_->index_;
                WORD location_code = achievement_parserInfo->parser_info_->location_code_;

                achievement_.Force_RewardAchievement( index, location_code, true);

                MSG_CG_AREACONQUEST_REWARD_ACK msg;
                msg.index_ = index;
                msg.location_code_ = location_code;
                msg.m_accomplishment_ratio = 0;
                msg.m_ItemInfo.Clear();
                msg.m_ItemInfo.m_InvenCount = 0;
                msg.m_ItemInfo.m_TmpInvenCount = 0;
                msg.m_i64Exp = player_->GetExp();
                msg.m_Money  = player_->GetMoney();
#ifdef _NA_003027_20111013_HONOR_SYSTEM
				msg.honor_title = 0;
#endif //_NA_003027_20111013_HONOR_SYSTEM
                player_->SendPacket(&msg, msg.GetSize());
            }

            ProcessACClear(achievement_parserInfo->parser_info_->index_);
        }
    }

    return ret;
}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
BOOL AchievementManager::ProcessFameRepute( BYTE type, WORD code )
{
    int grade = player_->GetHonorManager()->GetFameReputeGrade(type, code);
    if (grade > 0)
    {
        ACHIEVEMENT_OBJECT_TYPE ac_object_type = ACHIEVEMENT_OBJECT_NOT_DEFINE;
        if(type == HONOR_POINT_INFO::FAME)
            ac_object_type = ACHIEVEMENT_OBJECT_FAME;
        else if(type == HONOR_POINT_INFO::REPUTE)
            ac_object_type = ACHIEVEMENT_OBJECT_REPUTE;

        SetValueMax_if( ac_object_type, code, grade );

        ProcessFameReputeCount(type, grade);
    }

    return TRUE;
}

BOOL AchievementManager::ProcessFameReputeCount( BYTE type, WORD grade )
{
    int count = player_->GetHonorManager()->GetFameReputeGradeCount(type, grade);

    ACHIEVEMENT_OBJECT_TYPE ac_object_type = ACHIEVEMENT_OBJECT_NOT_DEFINE;
    if(type == HONOR_POINT_INFO::FAME)
        ac_object_type = ACHIEVEMENT_OBJECT_FAME_COUNT;
    else if(type == HONOR_POINT_INFO::REPUTE)
        ac_object_type = ACHIEVEMENT_OBJECT_REPUTE_COUNT;

    SetValueMax_if( ac_object_type, grade, count );

    return TRUE;
}

BOOL AchievementManager::ProcessQuestCount(WORD normalQ_count, WORD dailyQ_count)
{
    SetValue_if( ACHIEVEMENT_OBJECT_COMPLETE_NORMALQUEST, normalQ_count );
    SetValue_if( ACHIEVEMENT_OBJECT_COMPLETE_DAILYQUEST, dailyQ_count );
    
    return TRUE;
}

BOOL AchievementManager::ProcessLotto( int lotto_rank )
{
    return IncConditionValue(ACHIEVEMENT_OBJECT_LOTTO, lotto_rank);
}

BOOL AchievementManager::ProcessDailyQuest(BOOL success)
{
    const ACHIEVEMENT_OBJECT_TYPE type = ACHIEVEMENT_OBJECT_COMPLETE_DAILYQUEST_CONTINOUS;

    AchievementSearch::ACMapElem* rack = achievement_search_->GetSearchInfo_find(type);
    if (!rack)
        return FALSE;

    AchievementSearch::ACMapElem::iterator elem = (*rack).begin(), elem_end = (*rack).end();

    for( ; elem != elem_end; ++elem)
    {
        WORD keycode = elem->first;

        if(success) {
            IncConditionValue(type, keycode);
        }
        else {
            SetValueZero_if(type, keycode);
        }
    }

    return TRUE;
}
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA_002935_20110704_ITEM_SCORE
BOOL AchievementManager::ProcessItemScore(const int item_score)
{
    const AchievementSearch::ACMapElem* const score_map = 
        achievement_search_->GetSearchInfo_find(ACHIEVEMENT_OBJECT_ITEM_SCORE);
    if (score_map == NULL)
    {
        return FALSE;
    }
    AchievementSearch::ACMapElem::const_iterator it = score_map->begin();
    for (; it != score_map->end(); ++it)
    {
        const WORD condition_score = it->first;
        if (condition_score <= item_score)
        {
            IncConditionValue(ACHIEVEMENT_OBJECT_ITEM_SCORE, condition_score);
        }
    }
    return TRUE;
}
#endif // _NA_002935_20110704_ITEM_SCORE

bool AchievementManager::ProcessEnchant(const SLOTCODE item_code, const uint8_t enchant)
{
    SetValueMax_if(ACHIEVEMENT_OBJECT_ENCHANT, item_code, enchant);
    return true;
}

#ifdef _NA_008166_20150330_MODIFY_AC_LEVELUP
BOOL AchievementManager::IncConditionValue_LevelUpAC( WORD data, WORD count /*= 1*/ )
{
    //레벨업 관련 AC
    AchievementSearch::ACMapElem* level_ac_map = 
                  achievement_search_->GetSearchInfo_find( ACHIEVEMENT_OBJECT_LEVELUP );
    if (level_ac_map == NULL)
        return FALSE;

    AchievementSearch::ACMapElem::iterator map_itr = (*level_ac_map).begin();
    for (map_itr; map_itr != (*level_ac_map).end(); ++map_itr)
    {
        // data변수는 플레이어의 현재 레벨
        // 플레이어의 레벨보다 낮은 레벨AC에 대해서만 처리
        if ((map_itr->first) > data) {
            continue;
        }

        AchievementSearch::ACVectorElem::iterator vec_itr = (map_itr->second).begin();
        for (vec_itr; vec_itr != (map_itr->second).end(); ++vec_itr)
        {
            Achievement_SearchInfo* pinfo = &(*vec_itr);
            AchievementObjectInfo objectInfo;
            if( GetAchievement(pinfo, objectInfo) )
            {
                WORD script_value   = objectInfo.Achievement_;
                WORD user_record    = objectInfo.value_;

                if( user_record != script_value)
                {
                    SetAchievementValue(pinfo, count, ACHIEVEMENT_VALUE_APPEND);
                }
            }
        }
    }

    return TRUE;
}
#endif //_NA_008166_20150330_MODIFY_AC_LEVELUP

#endif //_NA_001990_ACSYSTEM_ADD

#endif //C_NA_0_20100520_ACSYSTEM_CHANGED