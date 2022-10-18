#include "stdafx.h"
#include "./BattleOpenTimeManager.h"

#include <time.h>

#include <UserSessions/UserManager.h>

#if __ITIMERBASE_VERSION_CRTL >= 0x0120
//==================================================================================================

BattleOpenTimeManager g_BattleOpenTimeMng;

BattleOpenTimeManager::BattleOpenTimeManager()
{
    loaded_ = false;
    update_timer_.SetTimer(kUpdateInterval);
    ZeroMemory(&current_mode_, sizeof(current_mode_));
}

BattleOpenTimeManager::~BattleOpenTimeManager()
{
}

void BattleOpenTimeManager::SyncParser()
{
    const CurseTowerTimeParser* parser = CurseTowerTimeParser::Instance();
    if (parser->is_loaded() == false) {
        return;
    }
    loaded_ = true;
}

bool BattleOpenTimeManager::IsOpenDungeon(uint16_t index)
{
    __UNUSED(index); // this index has only one.
    if (loaded_ == false) {
        SyncParser();
        return false;
    };
    //
    switch (current_mode_.time_mode_)
    {
    case current_mode_.eOpened:
    case current_mode_.eClosingCountdown_3Minutes:
    case current_mode_.eClosingCountdown_1Minutes:
        assert(current_mode_.index_ != 0);
        return true;
    }
    return false;
}

void BattleOpenTimeManager::Update(const util::TimeInfoPerFrame& time_info)
{
    if (update_timer_.IsExpired() == false) {
        return;
    }
    if (loaded_ == false) {
        SyncParser();
        return;
    }
    if (current_mode_.index_ != 0 && current_mode_.IsInRange(time_info.acm_time_)) {
        return;
    };
    const CurseTowerTimeParser* parser = CurseTowerTimeParser::Instance();
    if (current_mode_.time_mode_ == 0)
    {
        if (parser->IsExistStartTimeNode(time_info, &current_mode_) == false) {
            return;
        }
    }
    //
    if (current_mode_.IsInRange(time_info.acm_time_) == false)
    {
        if (parser->GetCurrentTimeNode(time_info, current_mode_.index_, &current_mode_) == false) {
            return;
        }
    }
    //
    if (current_mode_.check_event_ == false) {
        // first event
        ExecuteNotifier();
    }
}

void BattleOpenTimeManager::ExecuteNotifier()
{
    assert(current_mode_.check_event_ == false && current_mode_.index_ != 0);
    MSG_CG_ETC_BATTLE_OPEN_TIME_NOTICE_BY_CODE_BRD msg_brd;
    msg_brd.m_NoticeCode = current_mode_.string_code_;
    UserManager::Instance()->SendMessageToAllUsers(&msg_brd, sizeof(msg_brd));
    //
    current_mode_.check_event_ = true;

    //printf("--notifier|%u:%u|\n", current_mode_.time_mode_, current_mode_.start_tm_);
}

// making message: find closely first event and calculation
bool BattleOpenTimeManager::SetupMessage(const util::TimeInfoPerFrame& time_info,
                                         MSG_CG_ETC_GET_BATTLE_OPEN_TIME_ACK* const msg_ack) const
{
    typedef CurseTowerTimeParser::FirstEventList FirstEventList;
    const ns_func::BattleTimeNode::AcmTmFmt k24hours = CurseTowerTimeParser::k24hours;
    const CurseTowerTimeParser* parser = CurseTowerTimeParser::Instance();
    const FirstEventList& first_event_list = parser->GetFirstEventList();
    FirstEventList::const_iterator it = first_event_list.begin(),
                                   end = first_event_list.end();
    int cur_time = time_info.acm_time_;
    const ns_func::BattleTimeNode* time_node = *it; //first event;
    bool last_event = false;
    if (cur_time < static_cast<int>(time_node->start_tm_)) {
        cur_time += k24hours;
        last_event = true;
    }
    for ( ; it != end; ++it)
    {
        const ns_func::BattleTimeNode* time_node_in_loop = *it;
        const bool delimeter = (time_node_in_loop->time_mode_ == time_node_in_loop->eNone);
        const int open_time = time_node_in_loop->start_tm_;
        const int close_time = delimeter ? time_node_in_loop->end_tm_ : open_time + 15;
        if (cur_time >= open_time && cur_time < close_time)
        {
            time_node = time_node_in_loop;
            break;
        };
    }; //end 'for'
    //
    const int fix_time = (last_event ? k24hours : 0);
    if (last_event) {
        time_node = first_event_list.front();
    }
    else if (time_node->time_mode_ == time_node->eNone) {
        if (it == end || it == end - 1) {
            last_event = true;
            time_node = first_event_list.front();
        }
        else {
            time_node = *(++it);
        }
    }
    else {
        time_node = *it;
    };
    const int open_time = time_node->start_tm_ + 5 + fix_time;
    const int close_time = time_node->start_tm_ + 15 + fix_time;
    if (cur_time >= open_time && cur_time < close_time) {
        msg_ack->m_eJoinState = eJION_TOP_STATE_ADMISSION;
    }
    else {
        msg_ack->m_eJoinState = eJION_TOP_STATE_NONADMISSION;
    };
    msg_ack->m_JoinOverTime = 0;

    struct tm cur_time_struct;
    util::YYYYMMDDHHMMSSToTMStructure(time_info.wz_yyyymmddhhmmss_, &cur_time_struct);
    cur_time_struct.tm_year -= 1900;
    cur_time_struct.tm_mon -= 1;
    struct tm next_time_struct = cur_time_struct;
    // test value {
    cur_time_struct.tm_hour = cur_time / 60;
    cur_time_struct.tm_min = cur_time % 60;
    // }
    time_t cur_time_val = mktime(&cur_time_struct);
    //
    if (msg_ack->m_eJoinState == eJION_TOP_STATE_ADMISSION)
    {
        next_time_struct.tm_hour = close_time / 60;
        next_time_struct.tm_min = close_time % 60;
    }
    else if (msg_ack->m_eJoinState == eJION_TOP_STATE_NONADMISSION)
    {
        next_time_struct.tm_hour = open_time / 60;
        next_time_struct.tm_min = open_time % 60;
    };
    time_t open_time_val = mktime(&next_time_struct);
    double diff_val = difftime(open_time_val, cur_time_val);
    msg_ack->m_JoinOverTime = static_cast<DWORD>(diff_val) * 1000;


    return true;
}

//==================================================================================================
#else // __ITIMERBASE_VERSION_CRTL < 0x0120
//==================================================================================================

BattleOpenTimeManager g_BattleOpenTimeMng;

BattleOpenTimeManager::BattleOpenTimeManager()
{
}

BattleOpenTimeManager::~BattleOpenTimeManager()
{
}

VOID BattleOpenTimeManager::BattleJoinTimeUpdate()
{
	BATTLE_OPEN_TIME_LIST_HASH::iterator	HashIt = CurseTowerTimeParser::Instance()->GetListHash()->begin();
	BATTLE_OPEN_TIME_LIST::iterator			ListIt;
	DWORD									dwTick;

	MSG_CG_ETC_BATTLE_OPEN_TIME_NOTICE_BY_CODE_BRD NoticeBrd;

	dwTick = GetTickCount();
	
	while(HashIt != CurseTowerTimeParser::Instance()->GetListHash()->end())
	{
		if(!(HashIt->second))
		{
			++HashIt;
			continue;
		}

		ListIt = (HashIt->second)->m_List.begin();

		while(ListIt != (HashIt->second)->m_List.end() )
		{			
			if( (*ListIt)->OpenState == st_BattleOpenTimeNode::STATE_CLOSE )
			{				
				if( dwTick > (*ListIt)->dwBeforeOpen5MinTick )
				{
					(*ListIt)->OpenState = st_BattleOpenTimeNode::STATE_BEFORE_OPEN_5MIN;
					CurseTowerTimeParser::Instance()->GetRegisterList()->push_back((*ListIt));
					//World로 전체 메세지 날리기 추가 해야 함
					NoticeBrd.m_NoticeCode = (*ListIt)->dwStrCodeBeforeOpen5Min;
					UserPacketSender<MSG_CG_ETC_BATTLE_OPEN_TIME_NOTICE_BY_CODE_BRD> op(NoticeBrd);
					UserManager::Instance()->Foreach( op );
				#ifdef _JAPAN
					SUNLOG( eCRITICAL_LOG, "[BattleOpenTimeManager::BattleJoinTimeUpdate] Send Open Packet, CurTick = %d, Open5MinTick = %d",
						dwTick, (*ListIt)->dwBeforeOpen5MinTick );
				#endif
				}
			}

			++ListIt;
		}

		++HashIt;
	}
}

VOID BattleOpenTimeManager::RegisterTimeUpdate()
{
	BATTLE_OPEN_TIME_LIST::iterator it = CurseTowerTimeParser::Instance()->GetRegisterList()->begin();
	while( it != CurseTowerTimeParser::Instance()->GetRegisterList()->end())
	{
		// 시간 체크를 하고, 마감되면 TRUE를 아니면 FALSE를 리턴한다.
		if( TimeCheckRegisterNode( (*it) ) )
		{
			BATTLE_OPEN_TIME_LIST::iterator TempIt = it++;
			CurseTowerTimeParser::Instance()->GetRegisterList()->erase(TempIt);
			continue;
		}

		++it;
	}
}

BOOL BattleOpenTimeManager::TimeCheckRegisterNode(st_BattleOpenTimeNode *pNode)
{
	BOOL	bResult = FALSE;
	DWORD	dwCheckTick;
	DWORD	dwStringCode;
	INT		NextState;

	MSG_CG_ETC_BATTLE_OPEN_TIME_NOTICE_BY_CODE_BRD NoticeBrd;

	switch( pNode->OpenState )
	{
	case st_BattleOpenTimeNode::STATE_BEFORE_OPEN_5MIN:
		dwCheckTick		= pNode->dwBeforeOpen1MinTick;
		dwStringCode	= pNode->dwStrCodeBeforeOpen1Min;
		NextState		= st_BattleOpenTimeNode::STATE_BEFORE_OPEN_1MIN;
		break;
	case st_BattleOpenTimeNode::STATE_BEFORE_OPEN_1MIN:
		dwCheckTick		= pNode->dwOpenTick;
		dwStringCode	= pNode->dwStrCodeOpen;
		NextState		= st_BattleOpenTimeNode::STATE_OPEN;
		break;
	case st_BattleOpenTimeNode::STATE_OPEN:
		dwCheckTick		= pNode->dwBeforeClose3MinTick;
		dwStringCode	= pNode->dwStrCodeBeforeClose3Min;
		NextState		= st_BattleOpenTimeNode::STATE_BEFORE_CLOSE_3MIN;
		break;
	case st_BattleOpenTimeNode::STATE_BEFORE_CLOSE_3MIN:
		dwCheckTick		= pNode->dwBeforeClose1MinTick;
		dwStringCode	= pNode->dwStrCodeBeforeClose1Min;
		NextState		= st_BattleOpenTimeNode::STATE_BEFORE_CLOSE_1MIN;
		break;
	case st_BattleOpenTimeNode::STATE_BEFORE_CLOSE_1MIN:
		dwCheckTick		= pNode->dwCloseTick;
		dwStringCode	= pNode->dwStrCodeClose;
		NextState		= st_BattleOpenTimeNode::STATE_CLOSE;
		break;
	default:
		ASSERT(FALSE);
		SUNLOG( eCRITICAL_LOG, "[TimeCheckRegisterNode] Invalid OpenState[%d]", pNode->OpenState );
		pNode->OpenState = st_BattleOpenTimeNode::STATE_CLOSE;
		return TRUE;
	}

	DWORD dwCurTick = GetTickCount();

	if( dwCurTick > dwCheckTick )
	{
		//SUNLOG( eCRITICAL_LOG, "상태변경 : %u->%u ", pNode->OpenState, NextState);
	#ifdef _JAPAN
		SUNLOG( eCRITICAL_LOG, "[BattleOpenTimeManager::TimeCheckRegisterNode] Send Open Packet, CurTick = %d, CheckTick = %d, OpenState = %d, NextState = %d",
			dwCurTick, dwCheckTick, pNode->OpenState, NextState );
	#endif
		pNode->OpenState = NextState;
		if( pNode->OpenState == st_BattleOpenTimeNode::STATE_CLOSE )
		{
			//24시간 추가
			pNode->dwBeforeOpen5MinTick		+= (24 * 60 * 60 * 1000);
			pNode->dwBeforeOpen1MinTick		+= (24 * 60 * 60 * 1000);
			pNode->dwOpenTick				+= (24 * 60 * 60 * 1000);
			pNode->dwBeforeClose3MinTick	+= (24 * 60 * 60 * 1000);
			pNode->dwBeforeClose1MinTick	+= (24 * 60 * 60 * 1000);
			pNode->dwCloseTick				+= (24 * 60 * 60 * 1000);

			bResult = TRUE;
		}
		//메세지 날리기
		NoticeBrd.m_NoticeCode = dwStringCode;
		UserPacketSender<MSG_CG_ETC_BATTLE_OPEN_TIME_NOTICE_BY_CODE_BRD> op(NoticeBrd);
		UserManager::Instance()->Foreach( op );
	}

	return bResult;
}

BOOL BattleOpenTimeManager::IsOpenDungeon(WORD wIndex)
{
	BATTLE_OPEN_TIME_LIST::iterator it = CurseTowerTimeParser::Instance()->GetRegisterList()->begin();
	while( it != CurseTowerTimeParser::Instance()->GetRegisterList()->end())
	{
		if( (*it)->wIndexCode == wIndex )
		{
			if( (*it)->OpenState == st_BattleOpenTimeNode::STATE_OPEN				||
				(*it)->OpenState == st_BattleOpenTimeNode::STATE_BEFORE_CLOSE_3MIN	||
				(*it)->OpenState == st_BattleOpenTimeNode::STATE_BEFORE_CLOSE_1MIN	)
			{
				return TRUE;
			}

		}
		++it;
	}

	return FALSE;
}

//==================================================================================================
#endif //__ITIMERBASE_VERSION_CRTL
