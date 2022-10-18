#include "StdAfx.h"
#include "WorldSystemMemo.h"
#include "WorldServer.h"
#include "AgentServerSession.h"
#include "ServerSessionManager.h"
#include <Protocol_DW.h>
#include <PacketStruct_DW.h>

// implemented by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION

//���� �������� �ý��� �޸� �ۼ��ҽ� �������� �ڵ鷯
void 
WorldSystemMemo::SendSystemMemo(const BasicSystemMemoInfo* memo_info_list, DWORD list_num,
                                eSYSTEM_MEMO_TYPE system_memo_type)
{
    //default �� ���� �������� ���� �ý��� �޸�
    //World�������� �߰��Ǵ� case�� ��Ʈ�� �Ѵ�.
    switch(system_memo_type)
    {
    case SYSMEMO_LOTTO_WIN_NEW:
        {
            const SystemMemoInfoOfLotto* lotto_memo_info = 
                static_cast<const SystemMemoInfoOfLotto*>(memo_info_list);

            SendSystemMemoOfLotto(lotto_memo_info, list_num);
        }
        break;
    default://���� �������� ���� �ý��� �޸��� ��� ó��
        {
            SendSystemMemoToDBProxy(memo_info_list, list_num);
        }
        break;
    }
}

//���� �ζ� �޸� ������ �ۼ��Ͽ� DB�� ������.
void 
WorldSystemMemo::SendSystemMemoOfLotto(const SystemMemoInfoOfLotto* memo_info_list, DWORD list_num)
{
    assert(list_num <= MAX_SYSTEM_MEMO_LIST);
    if (list_num > MAX_SYSTEM_MEMO_LIST)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] system memo list count is over[count:%d]", 
            list_num);
        return;
    }

    BasicSystemMemoInfo basic_memo_info[MAX_SYSTEM_MEMO_LIST];

    for (DWORD i=0; i<list_num; ++i)
    {
        memcpy(&(basic_memo_info[i]), &(memo_info_list[i]), sizeof(basic_memo_info[i]));

        _sntprintf(basic_memo_info[i].system_memo_contents_, 
                   _countof(basic_memo_info[i].system_memo_contents_), 
                   "%d|%I64d|%d|%d|%I64d",
                   memo_info_list[i].system_memo_type_, 
                   memo_info_list[i].i64Idx_,
                   memo_info_list[i].draw_num_,
                   memo_info_list[i].status_,
                   memo_info_list[i].amount_cost_);
    }

    SendSystemMemoToDBProxy(basic_memo_info, list_num);
}

void
WorldSystemMemo::SendSystemMemoToDBProxy(const BasicSystemMemoInfo* memo_info_list,
                                          DWORD list_num)
{
    if (list_num > MAX_SYSTEM_MEMO_LIST)
    {
        assert(! "system memo list count is over");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] system memo list count is over[count:%d]", list_num);
        return;
    }

    ServerSession* game_dbproxy = ServerSessionManager::Instance()->GetGameDBProxy();
    if (game_dbproxy == NULL)
    {
        SUNLOG(eCRITICAL_LOG,  "["__FUNCTION__"]No GameDBProxy");
        return ;
    }

    for(DWORD i=0; i<list_num; ++i)
    {
        const BasicSystemMemoInfo& memo_info = memo_info_list[i];

        MSG_DW_MEMO_SEND_SYN syn_msg;
        {
            if (memo_info.recv_user_guid_ != 0)
            {
                //-- guid �� �޸� �����Ѵ�.
                syn_msg.m_recvGuid = memo_info.recv_user_guid_;

                ZeroMemory(syn_msg.m_ptszRecvName, sizeof(syn_msg.m_ptszRecvName));
            }
            else
            {
                //-- �̸����� �޸� �����Ѵ�.
                syn_msg.m_recvGuid = memo_info.recv_user_guid_;

                _tcsncpy(syn_msg.m_ptszRecvName, memo_info.recv_user_name_, 
                         _countof(syn_msg.m_ptszRecvName));
                syn_msg.m_ptszRecvName[_countof(syn_msg.m_ptszRecvName) - 1] = '\0';
            }

            //-- �ý��� �޸�
            syn_msg.m_SendGuid = 0;
            //-- �޸� Ÿ��
            syn_msg.m_typeSystemMemo = memo_info.system_memo_type_;
            //-- ���� ������ �ڵ�, 0�̸� ������ �ǹ�
            syn_msg.m_recvItemCode = memo_info.recv_item_code_;
            //-- ���� ������ ����, 0�̸� ������ �ǹ�
            syn_msg.m_amountCost = memo_info.amount_cost_;
            //-- �޸� ����
            _tcsncpy(syn_msg.m_ptszMemo, memo_info.system_memo_contents_,
                     _countof(syn_msg.m_ptszMemo));
            syn_msg.m_ptszMemo[_countof(syn_msg.m_ptszMemo) - 1] = '\0';
        }

        //-- ������ ������ 0�̸� �ý��� ������ ������ �ǹ̰� ����.

        if(syn_msg.m_amountCost > 0)
        {
            game_dbproxy->Send((BYTE*)&syn_msg, sizeof(syn_msg));
        }
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        //���巩ŷ�ý��۸޸�� �����۰����� ��� ������
        else if (syn_msg.m_typeSystemMemo == SYSMEMO_WORLD_RANKING_GRADE)
        {
            game_dbproxy->Send((BYTE*)&syn_msg, sizeof(syn_msg));
        }
#endif
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
        switch(syn_msg.m_typeSystemMemo)
        {
        case SYSMEMO_SUN_RANKING_SETTLE_RESULT:
            game_dbproxy->Send((BYTE*)&syn_msg, sizeof(syn_msg));
            break;
        default:
            break;
        }
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
    }
}