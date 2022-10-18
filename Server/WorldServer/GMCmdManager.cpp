#include "stdAfx.h"
#include ".\gmcmdmanager.h"
#include "User.h"
#include "UserManager.h"
//#include <GMList.h>
#include <PacketStruct_CW.h>
#include <ResultCode.h>

// 공지 위해
#include "BaseZone.h"
#include "Channel.h"
#include "ChannelManager.h"

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
#include "Handler_CW.h"
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

GMCmdManager::GMCmdManager(void)
{
	m_vecCmd.reserve( 10 );
	m_GMString.Init();
}

GMCmdManager::~GMCmdManager(void)
{

}


bool GMCmdManager::ParseCommand( User* pUser, const char* szCmd )
{
	m_vecCmd.clear();

	if( !IsGMUser( pUser ) )
	{
		// 이 경우는 크래킹이나 프로토콜 타입 버그일 가능성이 있다
		MessageOut(eCRITICAL_LOG, "GM 아닌 유저가 공지 요청!!!(%s)", pUser->GetCharName().c_str());
		return false;
	}

	if( !m_Parser.Parsing( szCmd, m_vecCmd ) )
		return false;

	if( !ProcessCommand( pUser, m_vecCmd ) )
		return false;

	return true;
}


bool GMCmdManager::ProcessCommand(User* gm_player, GM_CMD_STRING& gm_command)
{
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    // (hukim) 처음 구조 자체가 게임 서버쪽 처리기와는 달리 확장성이 고려되지 않아서
    // 이런 방식을 사용했다. 개발 환경이 아닌 상태에서도 GM 명령어가 오픈되어야 한다면
    // 구조 개선을 먼저 고려해 보자.
    const TCHAR* const GMSTR_PARTY_FINDING_REGISTER = _T("party_finding_register"); 
    const TCHAR* const GMSTR_PARTY_FINDING_GET_LIST = _T("party_finding_get_list"); 
    const TCHAR* const GMSTR_PARTY_FINDING_JOIN_REQUEST = _T("party_finding_join_request"); 
    const TCHAR* const GMSTR_PARTY_FINDING_CANCEL = _T("party_finding_cancel"); 
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

    bool is_success = false;
    if (gm_command.size() > 0)
    {
        const STLX_TSTRING& gm_command_head = gm_command[0];

        if (gm_command_head.compare(m_GMString.GetWorldNotice()) == 0) // 월드 공지
        {
            is_success = ProcessWorldNotice(gm_player, gm_command);
        }
        else if (gm_command_head.compare(m_GMString.GetChannelNotice()) == 0) // 채널 공지
        {
            is_success = ProcessChannelNotice(gm_player, gm_command);
        }
        else if (gm_command_head.compare(m_GMString.GetZoneNotice()) == 0) // 지역 공지
        {
            is_success = ProcessZoneNotice(gm_player, gm_command);
        }
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
        else if (gm_command_head.compare(GMSTR_PARTY_FINDING_REGISTER) == 0) // 파티 모집 등록
        {
            is_success = ProcessPartyFindingRegister(gm_player, gm_command);
        }
        else if (gm_command_head.compare(GMSTR_PARTY_FINDING_GET_LIST) == 0) // 파티 모집 목록
        {
            is_success = ProcessPartyFindingGetList(gm_player, gm_command);
        }
        else if (gm_command_head.compare(GMSTR_PARTY_FINDING_JOIN_REQUEST) == 0) // 파티 모집 참가
        {
            is_success = ProcessPartyFindingJoinRequest(gm_player, gm_command);
        }
        else if (gm_command_head.compare(GMSTR_PARTY_FINDING_CANCEL) == 0) // 파티 모집 취소
        {
            is_success = ProcessPartyFindingCancel(gm_player, gm_command);
        }
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
    }

    return is_success;
}


BOOL GMCmdManager::IsGMUser( User *pUser )
{
	if( pUser->GetGMGrade() <= eGM_GRADE_BASE || pUser->GetGMGrade() >= eGM_GRADE_MAX )
	{
		MSG_CW_GM_STRING_CMD_NAK NakMsg;
		NakMsg.m_byErrorCode =  RC::RC_GM_ISNOT_GM;		// GM이 아니다.
		pUser->SendPacket( (BYTE*)&NakMsg, sizeof(NakMsg) );
		return FALSE;
	}

	return TRUE;
}


// 월드 공지
bool GMCmdManager::ProcessWorldNotice( User* pUser, GM_CMD_STRING& vecCmd )
{
    // TODO: f101210.2L, this logic is terrible...

	// vecCmd 개수만큼 읽는다
	TCHAR	tszNoticeMsg[100]={0,};
	WORD	wCount=0;

	for(DWORD i = 1; i<vecCmd.size(); i++)
	{
		// 공지 메시지 조립
		if(wCount + vecCmd[i].size() > MAX_NOTICE_LEN )
		{
			// 공지 길이 이상은 잘린다
			break;
		}
		_tcscat( tszNoticeMsg + wCount, vecCmd[i].c_str() );
		wCount += static_cast<WORD>( vecCmd[i].size() );
		tszNoticeMsg[wCount++] = ' ';
	}
	tszNoticeMsg[wCount]='\0';

	MSG_CW_NOTICE_BRD noticeMsg;
	noticeMsg.m_byCategory	= CW_CHAT;
	noticeMsg.m_byProtocol	= CW_NOTICE_BRD;
	noticeMsg.wLen			= wCount;
	memset(noticeMsg.szMsg, 0, sizeof(noticeMsg.szMsg));
	memcpy( noticeMsg.szMsg, tszNoticeMsg, noticeMsg.wLen ); 

	// 갖고 있는 모든 유저들에게 날린다
	UserManager::Instance()->SendToAll( (BYTE*)&noticeMsg, noticeMsg.GetSize() );

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, noticeMsg.szMsg, noticeMsg.wLen );
	MessageOut(eCRITICAL_LOG,  "-----------------------------<< 전체공지 >>-----------------------------" );
	MessageOut(eCRITICAL_LOG,  "%s(%u)", noticeMsg.szMsg, noticeMsg.wLen );
	MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------" );

	return true;
}

// 채널 공지
bool GMCmdManager::ProcessChannelNotice( User* pUser, GM_CMD_STRING& vecCmd )
{
    // TODO: f101210.2L, this logic is terrible...

	// vecCmd 개수만큼 읽는다
	TCHAR	tszNoticeMsg[100]={0,};
	WORD	wCount=0;

	for(DWORD i = 1; i<vecCmd.size(); i++)
	{
		// 공지 메시지 조립
		if(wCount + vecCmd[i].size() > MAX_NOTICE_LEN )
		{
			// 공지 길이 이상은 잘린다
			break;
		}
		_tcscat( tszNoticeMsg + wCount, vecCmd[i].c_str() );
		wCount += static_cast<WORD>( vecCmd[i].size() );
		tszNoticeMsg[wCount++] = ' ';
	}
	tszNoticeMsg[wCount]='\0';

	MSG_CW_NOTICE_BRD noticeMsg;
	noticeMsg.m_byCategory	= CW_CHAT;
	noticeMsg.m_byProtocol	= CW_NOTICE_BRD;
	noticeMsg.wLen			= wCount;
	memset(noticeMsg.szMsg, 0, sizeof(noticeMsg.szMsg));
	memcpy( noticeMsg.szMsg, tszNoticeMsg, noticeMsg.wLen ); 

	// 해당 유저가 접속한 채널에만 날려준다
	Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
	if(!pChannel)
	{
		// 사용자의 채널 ID가 틀렸다. 치명적인 버그
		MessageOut( eCRITICAL_LOG, "사용자 %s의 채널 %u가 존재하지 않음!! 공지 실패!!", pUser->GetCharName().c_str(), pUser->GetChannelID() );
		return false;
	}
	// 마을, 배틀존 모두 공지
	pChannel->SendToAll( (BYTE*)&noticeMsg, noticeMsg.GetSize() );

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, noticeMsg.szMsg, noticeMsg.wLen );
	MessageOut(eCRITICAL_LOG,  "----------------------------<< %d채널 공지 >>----------------------------", pUser->GetChannelID() );
	MessageOut(eCRITICAL_LOG,  "%s(%u)", noticeMsg.szMsg, noticeMsg.wLen );
	MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------" );

	return true;
}

// 지역 공지
bool GMCmdManager::ProcessZoneNotice( User* pUser, GM_CMD_STRING& vecCmd )
{
    // TODO: f101210.2L, this logic is terrible...

	// vecCmd 개수만큼 읽는다
	TCHAR	tszNoticeMsg[100]={0,};
	WORD	wCount=0;

	for(DWORD i = 1; i<vecCmd.size(); i++)
	{
		// 공지 메시지 조립
		if(wCount + vecCmd[i].size() > MAX_NOTICE_LEN )
		{
			// 공지 길이 이상은 잘린다
			break;
		}
		_tcscat( tszNoticeMsg + wCount, vecCmd[i].c_str() );
		wCount += static_cast<WORD>( vecCmd[i].size() );
		tszNoticeMsg[wCount++] = ' ';
	}
	tszNoticeMsg[wCount]='\0';

	MSG_CW_NOTICE_BRD noticeMsg;
	noticeMsg.m_byCategory	= CW_CHAT;
	noticeMsg.m_byProtocol	= CW_NOTICE_BRD;
	noticeMsg.wLen			= wCount;
	memset(noticeMsg.szMsg, 0, sizeof(noticeMsg.szMsg));
	memcpy( noticeMsg.szMsg, tszNoticeMsg, noticeMsg.wLen ); 

	Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
	if(!pChannel)
	{
		// 사용자의 채널 ID가 틀렸다. 치명적인 버그
		MessageOut( eCRITICAL_LOG, "사용자 %s의 채널 %u가 존재하지 않음!! 지역공지 실패!!", pUser->GetCharName().c_str(), pUser->GetChannelID() );
		return false;
	}
	BaseZone *pZone = NULL;

	pZone = pChannel->FindZone( pUser->GetZoneKey() );

	if( !pZone )
	{
		// 사용자의 Zone ID가 틀렸다. 치명적인 버그
		MessageOut( eCRITICAL_LOG, "사용자 %s의 마을 %u가 존재하지 않음!! 공지 실패!!", pUser->GetCharName().c_str(), 
																		pUser->GetZoneKey() );
		return false;
	}
	pZone->Send( (BYTE*)&noticeMsg, noticeMsg.GetSize() );

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, noticeMsg.szMsg, vecCmd.begin()->size() );
	MessageOut(eCRITICAL_LOG,  "---------------------------<< %u채널 %uZone공지[ZT:%u] >>-------------------------", 
									pUser->GetChannelID(), 
									pUser->GetZoneKey(), pUser->GetStatus() );
	MessageOut(eCRITICAL_LOG,  "%s(%u)", noticeMsg.szMsg, noticeMsg.wLen );
	MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------" );

	return true;
}

bool GMCmdManager::ProcessNoticeType(User *pUser, eNOTICE_TYPE eType, TCHAR *ptszNoticeMsg)
{
	switch(eType)
	{
	case eNOTICE_WORLD:
		{
			ProcessWorldNotice(pUser, ptszNoticeMsg);
			return true;
		}
		break;
	case eNOTICE_CHANNEL:
		{
			ProcessChannelNotice(pUser, ptszNoticeMsg);
			return true;
		}
		break;
	case eNOTICE_ZONE:
		{
			ProcessZoneNotice(pUser, ptszNoticeMsg);
			return true;
		}
		break;
	}

	return false;
}

VOID GMCmdManager::ProcessWorldNotice(User *pUser, TCHAR *ptszNoticeMsg)
{
	MSG_CW_NOTICE_BRD		msgBRD;

	msgBRD.m_byCategory	= CW_CHAT;
	msgBRD.m_byProtocol	= CW_NOTICE_BRD;
	msgBRD.m_byRed		= 236;
	msgBRD.m_byGreen	= 96;
	msgBRD.m_byBlue		= 12;
    // CHANGES: f101210.2L, check boundary
    if (size_t str_len = strlen(ptszNoticeMsg) + 1)
    {
        // NOTE: always true, because wLen need consider with null-terminated position
        strncpy(msgBRD.szMsg, ptszNoticeMsg, _countof(msgBRD.szMsg));
        msgBRD.szMsg[_countof(msgBRD.szMsg) - 1] = '\0';
        str_len = min(_countof(msgBRD.szMsg), str_len);
        msgBRD.wLen = static_cast<uint16_t>(str_len);
    }

	// 갖고 있는 모든 유저들에게 날린다
	UserManager::Instance()->SendToAll( (BYTE*)&msgBRD, msgBRD.GetSize() );

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, msgBRD.szMsg, msgBRD.wLen );
	MessageOut(eCRITICAL_LOG,  "-----------------------------<< 전체공지 >>-----------------------------" );
	MessageOut(eCRITICAL_LOG,  "%s(%u)", msgBRD.szMsg, msgBRD.wLen );
	MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------" );
}

VOID GMCmdManager::ProcessChannelNotice(User *pUser, TCHAR *ptszNoticeMsg)
{
	MSG_CW_NOTICE_BRD		msgBRD;

	msgBRD.m_byCategory	= CW_CHAT;
	msgBRD.m_byProtocol	= CW_NOTICE_BRD;
	msgBRD.m_byRed		= 236;
	msgBRD.m_byGreen	= 96;
	msgBRD.m_byBlue		= 12;
    // CHANGES: f101210.2L, check boundary
    if (size_t str_len = strlen(ptszNoticeMsg) + 1)
    {
        // NOTE: always true, because wLen need consider with null-terminated position
        strncpy(msgBRD.szMsg, ptszNoticeMsg, _countof(msgBRD.szMsg));
        msgBRD.szMsg[_countof(msgBRD.szMsg) - 1] = '\0';
        str_len = min(_countof(msgBRD.szMsg), str_len);
        msgBRD.wLen = static_cast<uint16_t>(str_len);
    }

	// 해당 유저가 접속한 채널에만 날려준다
	Channel *pChannel = ChannelManager::Instance()->GetChannel(pUser->GetChannelID());
	if(!pChannel)
	{
		// 사용자의 채널 ID가 틀렸다. 치명적인 버그
		MessageOut( eCRITICAL_LOG, "사용자 %s의 채널 %u가 존재하지 않음!! 공지 실패!!", pUser->GetCharName().c_str(), pUser->GetChannelID() );
		return;
	}
	// 마을, 배틀존 모두 공지
	pChannel->SendToAll( (BYTE*)&msgBRD, msgBRD.GetSize() );

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, msgBRD.szMsg, msgBRD.wLen );
	MessageOut(eCRITICAL_LOG,  "----------------------------<< %d채널 공지 >>----------------------------", pUser->GetChannelID() );
	MessageOut(eCRITICAL_LOG,  "%s(%u)", msgBRD.szMsg, msgBRD.wLen );
	MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------" );
}

VOID GMCmdManager::ProcessZoneNotice(User *pUser, TCHAR *ptszNoticeMsg)
{	
	MSG_CW_NOTICE_BRD		msgBRD;

	msgBRD.m_byCategory	= CW_CHAT;
	msgBRD.m_byProtocol	= CW_NOTICE_BRD;
	msgBRD.m_byRed		= 236;
	msgBRD.m_byGreen	= 96;
	msgBRD.m_byBlue		= 12;
    // CHANGES: f101210.2L, check boundary
    if (size_t str_len = strlen(ptszNoticeMsg) + 1)
    {
        // NOTE: always true, because wLen need consider with null-terminated position
        strncpy(msgBRD.szMsg, ptszNoticeMsg, _countof(msgBRD.szMsg));
        msgBRD.szMsg[_countof(msgBRD.szMsg) - 1] = '\0';
        str_len = min(_countof(msgBRD.szMsg), str_len);
        msgBRD.wLen = static_cast<uint16_t>(str_len);
    }

	Channel *pChannel = ChannelManager::Instance()->GetChannel(pUser->GetChannelID());
	if(!pChannel)
	{
		// 사용자의 채널 ID가 틀렸다. 치명적인 버그
		MessageOut(eCRITICAL_LOG, "사용자 %s의 채널 %u가 존재하지 않음!! 지역공지 실패!!", pUser->GetCharName().c_str(), pUser->GetChannelID());
		return;
	}

	BaseZone *pZone = NULL;

	pZone = pChannel->FindZone( pUser->GetZoneKey() );
	if( !pZone )
	{
		// 사용자의 Zone ID가 틀렸다. 치명적인 버그
		MessageOut( eCRITICAL_LOG, "사용자 %s의 마을 %u가 존재하지 않음!! 공지 실패!!", pUser->GetCharName().c_str(), 
			pUser->GetZoneKey() );
		return;
	}
	pZone->Send( (BYTE*)&msgBRD, msgBRD.GetSize() );

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, msgBRD.szMsg, msgBRD.wLen );
	MessageOut(eCRITICAL_LOG,  "---------------------------<< %u채널 %uZone공지[ZT:%u] >>-------------------------", 
		pUser->GetChannelID(), 
		pUser->GetZoneKey(), pUser->GetStatus() );
	MessageOut(eCRITICAL_LOG,  "%s(%u)", msgBRD.szMsg, msgBRD.wLen );
	MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------" );
}

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

// 구현 전용 지역 함수
namespace util {
namespace internal {

// 입력값을 검사한다.
inline bool scoped_check_input(User* const gm_player, 
                               const GM_CMD_STRING& gm_command, 
                               const size_t gm_command_size)
{
    if (gm_player == NULL)
    {
        return false;
    }

    if (gm_command.size() != gm_command_size)
    {
        MSG_CG_GM_STRING_CMD_NAK cmd_nak;
        cmd_nak.m_byErrorCode = static_cast<BYTE>(RC::RC_GM_INVALID_ARGUMENT);
        gm_player->SendPacket(reinterpret_cast<BYTE*>(&cmd_nak), sizeof(cmd_nak));
        return false;
    }

    return true;
}

template <typename NUM_TYPE>
inline void scoped_string_to_number(const STLX_TSTRING& number_string, NUM_TYPE& OUT number)
{
    number = static_cast<NUM_TYPE>(_ttoi(number_string.c_str()));
}

} // namespace internal
} // namespace util

bool GMCmdManager::ProcessPartyFindingRegister(User* const gm_player, 
                                               const GM_CMD_STRING& gm_command) const
{
    using namespace util::internal;

    // 입력값을 검사한다.
    if (scoped_check_input(gm_player, gm_command, 5) == false)
    {
        return false;
    }

    // GM 명령어를 요청 메시지로 변환한다.
    MSG_CW_PARTY_FINDING_REGISTER_SYN register_syn;
    scoped_string_to_number(gm_command[1], register_syn.update_wanted_party_finding_key_);
    {
        const size_t dest_count = _countof(register_syn.party_finding_title_);
        ::_tcsncpy(register_syn.party_finding_title_, gm_command[2].c_str(), dest_count);
        register_syn.party_finding_title_[dest_count-1] = _T('\0');
    }
    scoped_string_to_number(gm_command[3], register_syn.num_of_max_joinable_people_);
    scoped_string_to_number(gm_command[4], register_syn.party_goal_);

    // 처리기를 호출한다.
    Handler_CW::OnCW_PARTY_FINDING_REGISTER_SYN(gm_player, &register_syn, register_syn.GetSize());
    
    return true;
}

bool GMCmdManager::ProcessPartyFindingGetList(User* const gm_player, 
                                              const GM_CMD_STRING& gm_command) const
{
    using namespace util::internal;

    // 입력값을 검사한다.
    if (scoped_check_input(gm_player, gm_command, 1) == false)
    {
        return false;
    }

    // GM 명령어를 요청 메시지로 변환한다.
    MSG_CW_PARTY_FINDING_GET_LIST_SYN get_list_syn;
    
    // 처리기를 호출한다.
    Handler_CW::OnCW_PARTY_FINDING_GET_LIST_SYN(gm_player, &get_list_syn, get_list_syn.GetSize());
    
    return true;
}

bool GMCmdManager::ProcessPartyFindingJoinRequest(User* const gm_player, 
                                                  const GM_CMD_STRING& gm_command) const
{
    using namespace util::internal;

    // 입력값을 검사한다.
    if (scoped_check_input(gm_player, gm_command, 2) == false)
    {
        return false;
    }

    // GM 명령어를 요청 메시지로 변환한다.
    MSG_CW_PARTY_FINDING_JOIN_REQUEST_SYN join_request_syn;
    scoped_string_to_number(gm_command[1], join_request_syn.join_wanted_party_finding_key_);

    // 처리기를 호출한다.
    Handler_CW::OnCW_PARTY_FINDING_JOIN_REQUEST_SYN(gm_player, &join_request_syn, join_request_syn.GetSize());

    return true;
}

bool GMCmdManager::ProcessPartyFindingCancel(User* const gm_player, 
                                             const GM_CMD_STRING& gm_command) const
{
    using namespace util::internal;

    // 입력값을 검사한다.
    if (scoped_check_input(gm_player, gm_command, 2) == false)
    {
        return false;
    }

    // GM 명령어를 요청 메시지로 변환한다.
    MSG_CW_PARTY_FINDING_CANCEL_SYN cancel_syn;
    scoped_string_to_number(gm_command[1], cancel_syn.cancel_wanted_party_finding_key_);

    // 처리기를 호출한다.
    Handler_CW::OnCW_PARTY_FINDING_CANCEL_SYN(gm_player, &cancel_syn, cancel_syn.GetSize());

    return true;
}

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
