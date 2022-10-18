#include "WorldServerPch.hxx"
#include "./WorldSystemMemo.h"

#include "WorldServer.h"
#include "Objects/WorldUser.h"

//==================================================================================================

// implemented by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION

bool ns_world::WorldSystemMemo::IsMemoTransaction(ns_world::WorldUser* user)
{
    const ns_world::MemoLink& memo_link = user->GetMemoInfo();
    return memo_link.transaction_;
}

bool ns_world::WorldSystemMemo::StartMemoTransaction(ns_world::WorldUser* user)
{
    ns_world::MemoLink* const editable_memo_link = \
        const_cast<ns_world::MemoLink*>(&user->GetMemoInfo());
    if (editable_memo_link->transaction_ != false) {
        return false;
    };
    editable_memo_link->transaction_ = true;
    return true;
}

bool ns_world::WorldSystemMemo::EndMemoTransaction(ns_world::WorldUser* user)
{
    ns_world::MemoLink* const editable_memo_link = \
        const_cast<ns_world::MemoLink*>(&user->GetMemoInfo());
    if (editable_memo_link->transaction_ == false) {
        return false;
    };
    editable_memo_link->transaction_ = false;
    return true;
}

bool ns_world::WorldSystemMemo::EnabledMemoListRequest(ns_world::WorldUser* user)
{
    const util::ServerTimeInfoPerFrame& time_info = \
        ns_world::WorldServer::Instance()->GetTimeInfoPerFrame();

    ns_world::MemoLink* const editable_memo_link = \
        const_cast<ns_world::MemoLink*>(&user->GetMemoInfo());
    if (editable_memo_link->blocking_memo_list_request_ > time_info.aging_counter_) {
        return false;
    }
    editable_memo_link->blocking_memo_list_request_ = time_info.aging_counter_ + 5;
    return true;
}

//월드 서버에서 시스템 메모를 작성할시 컨텐츠별 핸들러
void ns_world::WorldSystemMemo::SendSystemMemo(
    const BasicSystemMemoInfo* memo_info_list, DWORD list_num,
    eSYSTEM_MEMO_TYPE system_memo_type)
{
    //default 는 게임 서버에서 오는 시스템 메모
    //World서버에서 추가되는 case만 컨트롤 한다.
    switch(system_memo_type)
    {
    case SYSMEMO_LOTTO_WIN_NEW:
        {
            const SystemMemoInfoOfLotto* lotto_memo_info = 
                static_cast<const SystemMemoInfoOfLotto*>(memo_info_list);

            SendSystemMemoOfLotto(lotto_memo_info, list_num);
        }
        break;

    default://게임 서버에서 보낸 시스템 메모일 경우 처리
        {
            SendSystemMemoToDBProxy(memo_info_list, list_num);
        }
        break;
    }
}

//하임 로또 메모 내용을 작성하여 DB로 보낸다.
void ns_world::WorldSystemMemo::SendSystemMemoOfLotto(
    const SystemMemoInfoOfLotto* memo_info_list, DWORD list_num)
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

void ns_world::WorldSystemMemo::SendSystemMemoToDBProxy(
    const BasicSystemMemoInfo* memo_info_list, DWORD list_num)
{
    if (list_num > MAX_SYSTEM_MEMO_LIST)
    {
        assert(! "system memo list count is over");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] system memo list count is over[count:%d]", list_num);
        return;
    }

    ns_world::ServerSessionInWorld* const game_dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (game_dbproxy == NULL)
    {
        SUNLOG(eCRITICAL_LOG,  "["__FUNCTION__"]No GameDBProxy");
        return;
    }

    for (DWORD i = 0; i < list_num; ++i)
    {
        const BasicSystemMemoInfo& memo_info = memo_info_list[i];

        MSG_DW_MEMO_SEND_SYN syn_msg;
        {
            if (memo_info.recv_user_guid_ != 0)
            {
                //-- guid 로 메모를 전송한다.
                syn_msg.m_recvGuid = memo_info.recv_user_guid_;

                ZeroMemory(syn_msg.m_ptszRecvName, sizeof(syn_msg.m_ptszRecvName));
            }
            else
            {
                //-- 이름으로 메모를 전송한다.
                syn_msg.m_recvGuid = memo_info.recv_user_guid_;

                _tcsncpy(syn_msg.m_ptszRecvName, memo_info.recv_user_name_, 
                         _countof(syn_msg.m_ptszRecvName));
                syn_msg.m_ptszRecvName[_countof(syn_msg.m_ptszRecvName) - 1] = '\0';
            }

            //-- 시스템 메모
            syn_msg.m_SendGuid = 0;
            //-- 메모 타입
            syn_msg.m_typeSystemMemo = memo_info.system_memo_type_;
            //-- 보상 아이템 코드, 0이면 없음을 의미
            syn_msg.m_recvItemCode = memo_info.recv_item_code_;
            //-- 보상 아이템 갯수, 0이면 없음을 의미
            syn_msg.m_amountCost = memo_info.amount_cost_;
            //-- 메모 내용
            _tcsncpy(syn_msg.m_ptszMemo, memo_info.system_memo_contents_,
                     _countof(syn_msg.m_ptszMemo));
            syn_msg.m_ptszMemo[_countof(syn_msg.m_ptszMemo) - 1] = '\0';
        }

        //-- 아이템 갯수가 0이면 시스템 쪽지를 보내는 의미가 없다.
        if (syn_msg.m_amountCost > 0)
        {
            game_dbproxy->Send((BYTE*)&syn_msg, sizeof(syn_msg));
        }
    }
}

// NOTE: f110819.5L, added the system memo type acquisition interface
const TCHAR* ns_world::WorldSystemMemo::GetSystemMemoStringByType(
    const eSYSTEM_MEMO_TYPE memo_type)
{
    const TCHAR* memory_string = _T("System");
    //
    switch (memo_type)
    {
    // added filter to support _KR_0_20091021_FIRENDINVITATION_EXTEND //시스템 쪽지 이름 설정 = {
    case SYSMEMO_FRIENDINVITATION_INVATES_TART:
    case SYSMEMO_FRIENDINVITATION_INVATE_END:
    case SYSMEMO_FRIENDINVITATION_INVATE_PRESENT:
    case SYSMEMO_FRIENDINVITATION_BEINVATED_START:
    case SYSMEMO_FRIENDINVITATION_BEINVATED_END:
    case SYSMEMO_FRIENDINVITATION_BEINVATED_PRESENT:
        memory_string = "Event Manager";
        break;
    //}
    // added filter to support __NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    case SYSMEMO_LOTTO_WIN:
    case SYSMEMO_LOTTO_WIN_NEW:
        memory_string = "Lotto";
        break;
    // }
    //__KR_001355_20090805_CHECK_GROUPSYSTEM_MSG
    case SYSMEMO_GROUP_MGR:
        memory_string = "Group";
        break;
    case SYSMEMO_DOMINATION_RETURN_MONEY:
    case SYSMEMO_DOMINATION_REWARD_MONEY:
    case SYSMEMO_DOMINATION_RETURN_MONEY_NEW:
    case SYSMEMO_DOMINATION_REWARD_MONEY_NEW:
        memory_string = "Domination";
        break;
    default:
        SUNLOG(eCRITICAL_LOG, "Unknown system memo type : %d", memo_type);
        break;
    };

    return memory_string;
};

