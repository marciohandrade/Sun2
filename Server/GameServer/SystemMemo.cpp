#include "stdafx.h"
#include "SystemMemo.h"
#include "PacketStruct_DG.h"
#include "ServerOptionParserEx.h"

#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "AchievementManager.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM

// implemented by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION 

SystemMemo::SystemMemo()
{

}

SystemMemo::~SystemMemo()
{

}
// added by _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE
//메모 내용에 존재 하는 특정 정수형 데이터를 얻는다. -1은 에러
int 
SystemMemo::GetMemoDataToINT(const TCHAR* memo_contents, WORD index)
{
    std::string string_memo;
    string_memo.assign(memo_contents);

    std::string::size_type first_point = 0;
    std::string::size_type second_point = 0;

    first_point = second_point;
    second_point = string_memo.find('|', first_point);

    for(int i=1; i<index; ++i)
    {
        first_point = second_point+1;
        second_point = string_memo.find('|', first_point);
    }

    if (second_point != std::string::npos) 
    {
        std::string temp_string = string_memo.substr(first_point, second_point);
        return atoi(temp_string.c_str());
    }
    else
    {
        assert(!"Not found data in memo contents");
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found data in memo contents");
        return -1;
    }
}

RC::eITEM_RESULT 
SystemMemo::RewardSysMemoItem(Player* player, const MSG_BASE* recv_msg)
{
    const MSG_DG_EVENT_SYSTEMMEMO_ITEM_CHECK_ACK* msg = 
        static_cast<const MSG_DG_EVENT_SYSTEMMEMO_ITEM_CHECK_ACK*>(recv_msg);

    if (msg->reward_item_code_ == 0)
    {
        //-- 보상 아이템이 하임일 경우
        if (msg->reward_item_num_ == 0)
        {
            assert(!"Item info incorrect");
            SUNLOG(eCRITICAL_LOG,__FUNCTION__" : Item info incorrect : code : %d, num :%I64d", 
                   msg->reward_item_code_, msg->reward_item_num_);
            return RC::RC_ITEM_FAILED;
        }
        //-- 하임 보상
        if (RewardHeim(player, msg->reward_item_num_, msg->memo_type_))
        {
            GAMELOG->LogMemoReward(msg->memo_type_, 
                                   player, 
                                   msg->reward_item_code_, 
                                   msg->reward_item_num_,
                                   msg->memo_contents_);
#ifdef _NA_003027_20111013_HONOR_SYSTEM
            SystemMemo system_memo;
            int lotto_rank = system_memo.GetMemoDataToINT(msg->memo_contents_, 4); // 4번째가 등수
            if(lotto_rank > 0)
            {
                player->GetACManager()->ProcessLotto(lotto_rank);
            }
#endif //_NA_003027_20111013_HONOR_SYSTEM
        }
        
    }
    else
    {
        //-- 보상 아이템이 하임이 아닐 경우
        int check_cost = static_cast<int>(msg->reward_item_num_);
        int number_of_insertable_item;
        RC::eITEM_RESULT result_value = RC::RC_ITEM_SUCCESS; 


        //-- 보상 가능 체크
        result_value = CheckRewardItem(player, check_cost, msg->reward_item_code_, 
                                       number_of_insertable_item);

        if (result_value != RC::RC_ITEM_SUCCESS)
        {
            RollBackSystemMemoRewardItemInfo(msg->memo_guid_,
                                             msg->char_guid_, 
                                             msg->reward_item_code_, 
                                             check_cost);
            return result_value;
        }

        //-- 아이템 보상
        result_value = RewardItem(player, number_of_insertable_item, msg->reward_item_code_);

        if (result_value == RC::RC_ITEM_SUCCESS)
        {
            GAMELOG->LogMemoReward(msg->memo_type_, 
                                   player, 
                                   msg->reward_item_code_, 
                                   msg->reward_item_num_,
                                   msg->memo_contents_);
        }
        else
        {
            RollBackSystemMemoRewardItemInfo(msg->memo_guid_, 
                                             msg->char_guid_, 
                                             msg->reward_item_code_, 
                                             check_cost);

            return result_value;
        }
    }
    return RC::RC_ITEM_SUCCESS;
}
//롤백해야할 시스템 메모 아이템 정보를 Update요청하며
//메모 읽은 상태를 읽지 않음으로 Update한다.
void 
SystemMemo::RollBackSystemMemoRewardItemInfo(MEMOGUID memo_guid, CHARGUID char_guid,
                                              SLOTCODE reward_item_code, ULONGLONG reward_item_num)
{
    bool read_stat = false;
    UpdateSysMemoRewardItemInfo(memo_guid, char_guid, read_stat, reward_item_code, reward_item_num);
}

void 
SystemMemo::UpdateSysMemoRewardItemInfo(MEMOGUID memo_guid, CHARGUID char_guid,  bool read_stat,
                                        SLOTCODE reward_item_code, ULONGLONG reward_item_num)
{
    MSG_DG_EVENT_SYSTEMMEMO_ITEM_REWARD_UPDATE_CMD cmd_msg;
    {
        cmd_msg.memo_guid_ = memo_guid;
        cmd_msg.char_guid_ = char_guid;
        cmd_msg.read_stat_ = read_stat;
        cmd_msg.reward_item_code_ = reward_item_code;
        cmd_msg.reward_item_num_ = reward_item_num;
    }
    g_pGameServer->SendToServer(GAME_DBPROXY, &cmd_msg, sizeof(cmd_msg));

    SUNLOG(eFULL_LOG,"["__FUNCTION__"]memo:%d, char:%d, read:%d, item_code:%d, item_num%I64d",
           memo_guid, char_guid, read_stat, reward_item_code, reward_item_num);
}

//-- 지역 점령전 관련 시스템 메모, 클라에서 보여줄 정보에 따라서
//-- 메모내용에 문자열 파싱을 해서 DB저장을 요청하게 된다.
void
SystemMemo::SendSysMemoForDomination(const SystemMemoInfoOfDomination* memo_info,
                                      DWORD memo_info_list_num)
{
    assert(memo_info_list_num <= MAX_SYSTEM_MEMO_LIST);
    if (memo_info_list_num > MAX_SYSTEM_MEMO_LIST)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] system memo list count is over[count:%d]", 
            memo_info_list_num);
        return;
    }

    MSG_AG_EVENT_SYSTEMMEMO_SEND_CMD cmd_msg;
    {
        for (DWORD i=0; i<memo_info_list_num; ++i)
        {
            memcpy(&(cmd_msg.system_memo_info_[i]), &(memo_info[i]), sizeof(cmd_msg.system_memo_info_[i]));

            _sntprintf(cmd_msg.system_memo_info_[i].system_memo_contents_,
                       _countof(cmd_msg.system_memo_info_[i].system_memo_contents_),
                       "%d|%d|%I64d|%s|",
                       memo_info[i].system_memo_type_,
                       memo_info[i].map_code_,
                       memo_info[i].amount_cost_,
                       memo_info[i].guild_name_);

            //gamelulu : bebuging log
            SUNLOG(eFULL_LOG, "[Memo:%s]To:%s", cmd_msg.system_memo_info_[i].system_memo_contents_,
                memo_info[i].recv_user_name_);
        }
        cmd_msg.system_memo_list_num_ = memo_info_list_num;
    }
    g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, cmd_msg.GetSize());
}

//-- 기본적 시스템 메모
void 
SystemMemo::SendSysMemoForDefault(const BasicSystemMemoInfo* memo_info, DWORD memo_info_list_num)
{
    assert(memo_info_list_num <= MAX_SYSTEM_MEMO_LIST);
    if (memo_info_list_num > MAX_SYSTEM_MEMO_LIST)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] system memo list count is over[count;%d]",
            memo_info_list_num);
        return;
    }

    MSG_AG_EVENT_SYSTEMMEMO_SEND_CMD cmd_msg;
    {
        for (DWORD i=0; i<memo_info_list_num; ++i)
        {
            memcpy(&(cmd_msg.system_memo_info_[i]), &(memo_info[i]), 
                sizeof(cmd_msg.system_memo_info_[i]));

            _sntprintf(cmd_msg.system_memo_info_[i].system_memo_contents_,
                _countof(cmd_msg.system_memo_info_[i].system_memo_contents_),
                "%d|%d|",
                memo_info[i].system_memo_type_,
                memo_info[i].amount_cost_);

            //gamelulu : bebuging log
            SUNLOG(eFULL_LOG, "[Memo:%s]To:%s", memo_info[i].system_memo_contents_,
                memo_info[i].recv_user_name_);
        }
        cmd_msg.system_memo_list_num_ = memo_info_list_num;
    }
    g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, cmd_msg.GetSize());
}

//시스템 메모는 한번에 1~n개까지 동시에 보내는것으로 설정(MAX설정 값 == n)
//컨텐츠 별 쪽지로 출력하기 원하는 정보들이 다르기 때문에 컨텐츠별 취급이 필요(기본 정보는 같으며)
void
SystemMemo::SendSystemMemo(const BasicSystemMemoInfo* system_memo_info, 
                            DWORD memo_info_list_num, eSYSTEM_MEMO_TYPE system_memo_type)
{
    switch(system_memo_type)
    {
    case SYSMEMO_DOMINATION_RETURN_MONEY_NEW:
    case SYSMEMO_DOMINATION_REWARD_MONEY_NEW:
        {
#ifdef _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO 
            static const BYTE channel_id = 
                GameOptionParser::Instance()->GetGameOption().m_byDominationChannel;
#else
            static const BYTE channel_id = 
                ServerOptionParserEx::Instance()->GetServerOption().m_byDominationChannel;
#endif
            //-- 점령전 체널에서만 쪽지를 보낼 수 있게 한다.
            if (channel_id != g_pGameServer->GetKey().GetChannelID())
            {
                SUNLOG(eFULL_LOG, "["__FUNCTION__"]This is not doomination channel[channel:%d]",
                       g_pGameServer->GetKey().GetChannelID());
                return;
            }

            const SystemMemoInfoOfDomination* memo_info = 
                static_cast<const SystemMemoInfoOfDomination*>(system_memo_info);

            SendSysMemoForDomination(memo_info, memo_info_list_num);
        }
        break;
        //TODO : 시스템 메모 기본 정보외 추가 정보가 필요한 컨텐츠일 경우 메모 타입을 위와 같이 추가 
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    case SYSMEMO_WORLD_RANKING_GRADE:
        {
            const SystemMemoInfoOfWorldRanking* memo_info = 
                static_cast<const SystemMemoInfoOfWorldRanking*>(system_memo_info);
            SendSystemMemoOfWorldRanking(memo_info, memo_info_list_num);
        }
        break;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    default:
        {
            SendSysMemoForDefault(system_memo_info, memo_info_list_num);
        }
        break;
    }
}

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
void SystemMemo::SendSystemMemoOfWorldRanking( const SystemMemoInfoOfWorldRanking* memo_info, DWORD memo_info_list_num )
{
    assert(memo_info_list_num <= MAX_SYSTEM_MEMO_LIST);
    if (memo_info_list_num > MAX_SYSTEM_MEMO_LIST)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] system memo list count is over[count:%d]", 
            memo_info_list_num);
        return;
    }

    MSG_AG_EVENT_SYSTEMMEMO_SEND_CMD cmd_msg;
    {
        for (DWORD i=0; i<memo_info_list_num; ++i)
        {
            memcpy(&(cmd_msg.system_memo_info_[i]), &(memo_info[i]), sizeof(cmd_msg.system_memo_info_[i]));

            _sntprintf(cmd_msg.system_memo_info_[i].system_memo_contents_,
                _countof(cmd_msg.system_memo_info_[i].system_memo_contents_),
                "%d|%d|%d|",
                memo_info[i].system_memo_type_,
                memo_info[i].ranking_id,
                memo_info[i].reward_ranking);

            //gamelulu : bebuging log
            SUNLOG(eFULL_LOG, "[Memo:%s]To:%s", cmd_msg.system_memo_info_[i].system_memo_contents_,
                memo_info[i].recv_user_name_);
        }
        cmd_msg.system_memo_list_num_ = memo_info_list_num;
    }
    g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, cmd_msg.GetSize());
}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR