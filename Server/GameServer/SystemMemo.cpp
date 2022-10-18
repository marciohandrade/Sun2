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
//�޸� ���뿡 ���� �ϴ� Ư�� ������ �����͸� ��´�. -1�� ����
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
        //-- ���� �������� ������ ���
        if (msg->reward_item_num_ == 0)
        {
            assert(!"Item info incorrect");
            SUNLOG(eCRITICAL_LOG,__FUNCTION__" : Item info incorrect : code : %d, num :%I64d", 
                   msg->reward_item_code_, msg->reward_item_num_);
            return RC::RC_ITEM_FAILED;
        }
        //-- ���� ����
        if (RewardHeim(player, msg->reward_item_num_, msg->memo_type_))
        {
            GAMELOG->LogMemoReward(msg->memo_type_, 
                                   player, 
                                   msg->reward_item_code_, 
                                   msg->reward_item_num_,
                                   msg->memo_contents_);
#ifdef _NA_003027_20111013_HONOR_SYSTEM
            SystemMemo system_memo;
            int lotto_rank = system_memo.GetMemoDataToINT(msg->memo_contents_, 4); // 4��°�� ���
            if(lotto_rank > 0)
            {
                player->GetACManager()->ProcessLotto(lotto_rank);
            }
#endif //_NA_003027_20111013_HONOR_SYSTEM
        }
        
    }
    else
    {
        //-- ���� �������� ������ �ƴ� ���
        int check_cost = static_cast<int>(msg->reward_item_num_);
        int number_of_insertable_item;
        RC::eITEM_RESULT result_value = RC::RC_ITEM_SUCCESS; 


        //-- ���� ���� üũ
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

        //-- ������ ����
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
//�ѹ��ؾ��� �ý��� �޸� ������ ������ Update��û�ϸ�
//�޸� ���� ���¸� ���� �������� Update�Ѵ�.
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

//-- ���� ������ ���� �ý��� �޸�, Ŭ�󿡼� ������ ������ ����
//-- �޸𳻿뿡 ���ڿ� �Ľ��� �ؼ� DB������ ��û�ϰ� �ȴ�.
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

//-- �⺻�� �ý��� �޸�
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

//�ý��� �޸�� �ѹ��� 1~n������ ���ÿ� �����°����� ����(MAX���� �� == n)
//������ �� ������ ����ϱ� ���ϴ� �������� �ٸ��� ������ �������� ����� �ʿ�(�⺻ ������ ������)
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
            //-- ������ ü�ο����� ������ ���� �� �ְ� �Ѵ�.
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
        //TODO : �ý��� �޸� �⺻ ������ �߰� ������ �ʿ��� �������� ��� �޸� Ÿ���� ���� ���� �߰� 
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