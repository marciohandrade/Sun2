#include "stdafx.h"
#include "GameServerQuery.h"
#include "./QueryPoolFactory.h"


__IMPL_SAFECUSTOMPOOL_PTR(Query_Warehouse_Select);

__IMPL_SAFECUSTOMPOOL_PTR(Query_ItemSlotInfo_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_ItemSlotInfo_Update);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Perk_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Perk_Insert);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Perk_Delete);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Perk_Update);
__IMPL_SAFECUSTOMPOOL_PTR(Query_SlotInfo_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_SlotInfo_Insert);
__IMPL_SAFECUSTOMPOOL_PTR(Query_SlotInfo_Update);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Quest_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Quest_Update);
__IMPL_SAFECUSTOMPOOL_PTR(Query_BattleRecords_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_BattleRecords_Update);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Char_State_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Char_State_Insert);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Char_State_Delete);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Char_State_Update);
__IMPL_SAFECUSTOMPOOL_PTR(Query_MissionInfo_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_MissionInfo_Insert);
__IMPL_SAFECUSTOMPOOL_PTR(Query_MissionInfo_Update);

__IMPL_SAFECUSTOMPOOL_PTR(SelectEventQuery);
__IMPL_SAFECUSTOMPOOL_PTR(UdpateEventQuery);
__IMPL_SAFECUSTOMPOOL_PTR(EventStatusQuery);
__IMPL_SAFECUSTOMPOOL_PTR(EventItemRegQuery);
__IMPL_SAFECUSTOMPOOL_PTR(Query_MissionRank_Insert);
__IMPL_SAFECUSTOMPOOL_PTR(Query_MissionRecord_Insert);
__IMPL_SAFECUSTOMPOOL_PTR(Query_MissionRecord_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_MissionReward_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_MissionReward_Update);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Char_Exist);
__IMPL_SAFECUSTOMPOOL_PTR(Query_WayPoint_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_WayPoint_Insert);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Char_WayPoint_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Char_WayPoint_Insert);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Char_WayPoint_Delete);

__IMPL_SAFECUSTOMPOOL_PTR(Query_Password_Update);

__IMPL_SAFECUSTOMPOOL_PTR(Query_Ranking_Select_Insert);

__IMPL_SAFECUSTOMPOOL_PTR(Query_FateItemIssue_Proc);
//
__IMPL_SAFECUSTOMPOOL_PTR(Query_Collected_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Collected_Insert);
#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_Reward_And_Delete);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Lotto_Reward_And_Delete);
#endif
//_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
__IMPL_SAFECUSTOMPOOL_PTR(Query_System_Memo_Reward_Item_Check_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_System_Memo_Reward_Item_Update_Select);

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
__IMPL_SAFECUSTOMPOOL_PTR(Query_Char_Achievement_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Char_Achievement_Update);
#endif

__IMPL_SAFECUSTOMPOOL_PTR(CompositionOrDecompositionHistoryInsertQuery)
__IMPL_SAFECUSTOMPOOL_PTR(CompositionOrDecompositionHistorySelectQuery)

__IMPL_SAFECUSTOMPOOL_PTR(ExtendCharSlotQuery)

#ifdef _NA_003027_20111013_HONOR_SYSTEM
__IMPL_SAFECUSTOMPOOL_PTR(Query_HonorTitle_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_HonorTitle_AddOrRemove);
__IMPL_SAFECUSTOMPOOL_PTR(Query_FameRepute_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_FameRepute_Set);
__IMPL_SAFECUSTOMPOOL_PTR(Query_DailyQuest_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_DailyQuest_Update);
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
__IMPL_SAFECUSTOMPOOL_PTR(Query_Character_Exist);
#endif //NA_0_20110613_GAME_SHOP_RENEWAL

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
__IMPL_SAFECUSTOMPOOL_PTR(Query_PointWallet_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_PointWallet_Update);
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildPoint_Donation);
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildPoint_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildPoint_Change);
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildLevel_Change);
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildRanking_Select);
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildIntroduction_Update);
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildIntroduction_Select);
#endif //_NA_004034_20120102_GUILD_INTRODUCTION
__IMPL_SAFECUSTOMPOOL_PTR(Query_Character_NameChange_Request);
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildPenalty_Update);

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
__IMPL_SAFECUSTOMPOOL_PTR(Query_Attendance_Update);
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
__IMPL_SAFECUSTOMPOOL_PTR(Query_Attendance_Update_Mondschien);
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildJoinRequest_Insert);
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildJoinRequest_Delete);
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildJoinRequest_Select);

__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildFacility_Update);
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildFacility_Select);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
__IMPL_SAFECUSTOMPOOL_PTR(Query_SunRanking_Update);
__IMPL_SAFECUSTOMPOOL_PTR(Query_SunRanking_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_SunRanking_Init);

__IMPL_SAFECUSTOMPOOL_PTR(Query_SunRankingGrade_Init);
__IMPL_SAFECUSTOMPOOL_PTR(Query_SunRankingGrade_Insert);
__IMPL_SAFECUSTOMPOOL_PTR(Query_SunRankingGrade_Select);

__IMPL_SAFECUSTOMPOOL_PTR(Query_StatueItemSlotInfo_Insert);
__IMPL_SAFECUSTOMPOOL_PTR(Query_StatueItemSlotInfo_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Statue_Char_Select);
__IMPL_SAFECUSTOMPOOL_PTR(Query_StatueCharInfo_Select);
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

/////////////// DB ERROR CODE CONVERT TO SERVER ERROR CODE ///////////////
const INT DBResultConvert<GuildWarehouseTabExQuery>::ErrorCode[] = 
{ 
    RC::RC_ITEM_SUCCESS,
	RC::RC_ITEM_GUILDWAREHOUSE_USENOT_SLOT_BY_GRADE,		//1
	RC::RC_ITEM_GUILDWAREHOUSE_NONE_USE_RIGHT,				//2
	RC::RC_ITEM_CANNOT_EXTEND_CASH_SLOT,							//3
	RC::RC_ITEM_FAILED,
};

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

VOID Query_SunRanking_Select::GetColumnInfo( ULONG row, ULONG col, ULONG*& pColPtr, LONG& lSize, SHORT& sColType )
{
    sQueryResult* pTag = result_records_;
    if (make_one_row == true)
    {
        std::map<CHARGUID, SunRankingPlayerInfo>::iterator find_iter = 
            infos.find(result_records_[0].char_guid);
        if (find_iter == infos.end())
        {
            SunRankingPlayerInfo temp_sun_rank_player_info;
            find_iter = infos.insert(std::make_pair(result_records_[0].char_guid, temp_sun_rank_player_info)).first;
            find_iter->second.char_guid = result_records_[0].char_guid;
            strncpy(find_iter->second.char_name, result_records_[0].char_name, MAX_CHARNAME_LENGTH);
            find_iter->second.class_code = static_cast<eCHAR_TYPE>(result_records_[0].class_code);
            find_iter->second.guild_guid = result_records_[0].guild_guid;
            find_iter->second.score[result_records_[0].ranking_type] = result_records_[0].score;
        }
        else
        {
            find_iter->second.score[result_records_[0].ranking_type] = result_records_[0].score;
        }

        make_one_row = false;
    }

    switch (col)
    {
    case 0:
        {
            pColPtr = (ULONG *)&(pTag[0].char_guid);
            lSize = sizeof(pTag[0].char_guid);
            sColType = _GET_DB_TYPE(pTag[0].char_guid);
        } break;
    case 1:
        {
            pColPtr = (ULONG *)&(pTag[0].ranking_type);
            lSize = sizeof(pTag[0].ranking_type);
            sColType = _GET_DB_TYPE(pTag[0].ranking_type);
        } break;
    case 2:
        {
            pColPtr = (ULONG *)&(pTag[0].score);
            lSize = sizeof(pTag[0].score);
            sColType = _GET_DB_TYPE(pTag[0].score);
        } break;
    case 3:
        {
            pColPtr = (ULONG *)(pTag[0].char_name);
            lSize = sizeof(pTag[0].char_name);
            sColType = _GET_DB_TYPE(pTag[0].char_name);
        } break;
    case 4:
        {
            pColPtr = (ULONG *)&(pTag[0].class_code);
            lSize = sizeof(pTag[0].class_code);
            sColType = _GET_DB_TYPE(pTag[0].class_code);
        } break;
    case 5:
        {
            pColPtr = (ULONG *)&(pTag[0].guild_guid);
            lSize = sizeof(pTag[0].guild_guid);
            sColType = _GET_DB_TYPE(pTag[0].guild_guid);

            make_one_row = true;
        } break;
    }
}

VOID Query_SunRanking_Select::GetLastColumnInfo()
{
    if (make_one_row == true)
    {
        std::map<CHARGUID, SunRankingPlayerInfo>::iterator find_iter = 
            infos.find(result_records_[0].char_guid);
        if (find_iter == infos.end())
        {
            SunRankingPlayerInfo temp_sun_rank_player_info;
            find_iter = infos.insert(std::make_pair(result_records_[0].char_guid, temp_sun_rank_player_info)).first;
            find_iter->second.char_guid = result_records_[0].char_guid;
            strncpy(find_iter->second.char_name, result_records_[0].char_name, MAX_CHARNAME_LENGTH);
            find_iter->second.class_code = static_cast<eCHAR_TYPE>(result_records_[0].class_code);
            find_iter->second.guild_guid = result_records_[0].guild_guid;
            find_iter->second.score[result_records_[0].ranking_type] = result_records_[0].score;
        }
        else
        {
            find_iter->second.score[result_records_[0].ranking_type] = result_records_[0].score;
        }

        make_one_row = false;
    }
}

VOID Query_SunRankingGrade_Select::GetColumnInfo( ULONG row, ULONG col, ULONG*& pColPtr, LONG& lSize, SHORT& sColType )
{
    sQueryResult* pTag = result_records_;
    if (make_one_row == true)
    {
        std::map<std::pair<CHARGUID, int>, SunRankingGradeInfo>::iterator find_iter = 
            infos.find(std::make_pair(result_records_[0].char_guid, result_records_[0].ranking_type));
        if (find_iter == infos.end())
        {
            SunRankingGradeInfo temp_sun_rank_grade_info;
            temp_sun_rank_grade_info.char_guid = result_records_[0].char_guid;
            temp_sun_rank_grade_info.ranking_type = result_records_[0].ranking_type;
            temp_sun_rank_grade_info.grade = result_records_[0].grade;
            temp_sun_rank_grade_info.score = result_records_[0].score;
            infos.insert(std::make_pair(std::make_pair(result_records_[0].char_guid, result_records_[0].ranking_type)
                , temp_sun_rank_grade_info));
        }
        else
        {
            find_iter->second.score = result_records_[0].score;
        }

        make_one_row = false;
    }

    switch (col)
    {
    case 0:
        {
            pColPtr = (ULONG *)&(pTag[0].char_guid);
            lSize = sizeof(pTag[0].char_guid);
            sColType = _GET_DB_TYPE(pTag[0].char_guid);
        } break;
    case 1:
        {
            pColPtr = (ULONG *)&(pTag[0].ranking_type);
            lSize = sizeof(pTag[0].ranking_type);
            sColType = _GET_DB_TYPE(pTag[0].ranking_type);
        } break;
    case 2:
        {
            pColPtr = (ULONG *)&(pTag[0].grade);
            lSize = sizeof(pTag[0].grade);
            sColType = _GET_DB_TYPE(pTag[0].grade);
        } break;
    case 3:
        {
            pColPtr = (ULONG *)&(pTag[0].score);
            lSize = sizeof(pTag[0].score);
            sColType = _GET_DB_TYPE(pTag[0].score);

            make_one_row = true;
        } break;
    }
}

VOID Query_SunRankingGrade_Select::GetLastColumnInfo()
{
    if (make_one_row == true)
    {
        std::map<std::pair<CHARGUID, int>, SunRankingGradeInfo>::iterator find_iter = 
            infos.find(std::make_pair(result_records_[0].char_guid, result_records_[0].ranking_type));
        if (find_iter == infos.end())
        {
            SunRankingGradeInfo temp_sun_rank_grade_info;
            temp_sun_rank_grade_info.char_guid = result_records_[0].char_guid;
            temp_sun_rank_grade_info.ranking_type = result_records_[0].ranking_type;
            temp_sun_rank_grade_info.grade = result_records_[0].grade;
            temp_sun_rank_grade_info.score = result_records_[0].score;
            infos.insert(std::make_pair(std::make_pair(result_records_[0].char_guid, result_records_[0].ranking_type)
                , temp_sun_rank_grade_info));
        }
        else
        {
            find_iter->second.score = result_records_[0].score;
        }

        make_one_row = false;
    }
}

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM