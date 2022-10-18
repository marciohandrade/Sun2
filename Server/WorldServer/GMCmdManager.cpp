#include "stdAfx.h"
#include ".\gmcmdmanager.h"
#include "User.h"
#include "UserManager.h"
//#include <GMList.h>
#include <PacketStruct_CW.h>
#include <ResultCode.h>

// ���� ����
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
		// �� ���� ũ��ŷ�̳� �������� Ÿ�� ������ ���ɼ��� �ִ�
		MessageOut(eCRITICAL_LOG, "GM �ƴ� ������ ���� ��û!!!(%s)", pUser->GetCharName().c_str());
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
    // (hukim) ó�� ���� ��ü�� ���� ������ ó����ʹ� �޸� Ȯ�强�� ������� �ʾƼ�
    // �̷� ����� ����ߴ�. ���� ȯ���� �ƴ� ���¿����� GM ��ɾ ���µǾ�� �Ѵٸ�
    // ���� ������ ���� ����� ����.
    const TCHAR* const GMSTR_PARTY_FINDING_REGISTER = _T("party_finding_register"); 
    const TCHAR* const GMSTR_PARTY_FINDING_GET_LIST = _T("party_finding_get_list"); 
    const TCHAR* const GMSTR_PARTY_FINDING_JOIN_REQUEST = _T("party_finding_join_request"); 
    const TCHAR* const GMSTR_PARTY_FINDING_CANCEL = _T("party_finding_cancel"); 
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

    bool is_success = false;
    if (gm_command.size() > 0)
    {
        const STLX_TSTRING& gm_command_head = gm_command[0];

        if (gm_command_head.compare(m_GMString.GetWorldNotice()) == 0) // ���� ����
        {
            is_success = ProcessWorldNotice(gm_player, gm_command);
        }
        else if (gm_command_head.compare(m_GMString.GetChannelNotice()) == 0) // ä�� ����
        {
            is_success = ProcessChannelNotice(gm_player, gm_command);
        }
        else if (gm_command_head.compare(m_GMString.GetZoneNotice()) == 0) // ���� ����
        {
            is_success = ProcessZoneNotice(gm_player, gm_command);
        }
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
        else if (gm_command_head.compare(GMSTR_PARTY_FINDING_REGISTER) == 0) // ��Ƽ ���� ���
        {
            is_success = ProcessPartyFindingRegister(gm_player, gm_command);
        }
        else if (gm_command_head.compare(GMSTR_PARTY_FINDING_GET_LIST) == 0) // ��Ƽ ���� ���
        {
            is_success = ProcessPartyFindingGetList(gm_player, gm_command);
        }
        else if (gm_command_head.compare(GMSTR_PARTY_FINDING_JOIN_REQUEST) == 0) // ��Ƽ ���� ����
        {
            is_success = ProcessPartyFindingJoinRequest(gm_player, gm_command);
        }
        else if (gm_command_head.compare(GMSTR_PARTY_FINDING_CANCEL) == 0) // ��Ƽ ���� ���
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
		NakMsg.m_byErrorCode =  RC::RC_GM_ISNOT_GM;		// GM�� �ƴϴ�.
		pUser->SendPacket( (BYTE*)&NakMsg, sizeof(NakMsg) );
		return FALSE;
	}

	return TRUE;
}


// ���� ����
bool GMCmdManager::ProcessWorldNotice( User* pUser, GM_CMD_STRING& vecCmd )
{
    // TODO: f101210.2L, this logic is terrible...

	// vecCmd ������ŭ �д´�
	TCHAR	tszNoticeMsg[100]={0,};
	WORD	wCount=0;

	for(DWORD i = 1; i<vecCmd.size(); i++)
	{
		// ���� �޽��� ����
		if(wCount + vecCmd[i].size() > MAX_NOTICE_LEN )
		{
			// ���� ���� �̻��� �߸���
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

	// ���� �ִ� ��� �����鿡�� ������
	UserManager::Instance()->SendToAll( (BYTE*)&noticeMsg, noticeMsg.GetSize() );

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, noticeMsg.szMsg, noticeMsg.wLen );
	MessageOut(eCRITICAL_LOG,  "-----------------------------<< ��ü���� >>-----------------------------" );
	MessageOut(eCRITICAL_LOG,  "%s(%u)", noticeMsg.szMsg, noticeMsg.wLen );
	MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------" );

	return true;
}

// ä�� ����
bool GMCmdManager::ProcessChannelNotice( User* pUser, GM_CMD_STRING& vecCmd )
{
    // TODO: f101210.2L, this logic is terrible...

	// vecCmd ������ŭ �д´�
	TCHAR	tszNoticeMsg[100]={0,};
	WORD	wCount=0;

	for(DWORD i = 1; i<vecCmd.size(); i++)
	{
		// ���� �޽��� ����
		if(wCount + vecCmd[i].size() > MAX_NOTICE_LEN )
		{
			// ���� ���� �̻��� �߸���
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

	// �ش� ������ ������ ä�ο��� �����ش�
	Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
	if(!pChannel)
	{
		// ������� ä�� ID�� Ʋ�ȴ�. ġ������ ����
		MessageOut( eCRITICAL_LOG, "����� %s�� ä�� %u�� �������� ����!! ���� ����!!", pUser->GetCharName().c_str(), pUser->GetChannelID() );
		return false;
	}
	// ����, ��Ʋ�� ��� ����
	pChannel->SendToAll( (BYTE*)&noticeMsg, noticeMsg.GetSize() );

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, noticeMsg.szMsg, noticeMsg.wLen );
	MessageOut(eCRITICAL_LOG,  "----------------------------<< %dä�� ���� >>----------------------------", pUser->GetChannelID() );
	MessageOut(eCRITICAL_LOG,  "%s(%u)", noticeMsg.szMsg, noticeMsg.wLen );
	MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------" );

	return true;
}

// ���� ����
bool GMCmdManager::ProcessZoneNotice( User* pUser, GM_CMD_STRING& vecCmd )
{
    // TODO: f101210.2L, this logic is terrible...

	// vecCmd ������ŭ �д´�
	TCHAR	tszNoticeMsg[100]={0,};
	WORD	wCount=0;

	for(DWORD i = 1; i<vecCmd.size(); i++)
	{
		// ���� �޽��� ����
		if(wCount + vecCmd[i].size() > MAX_NOTICE_LEN )
		{
			// ���� ���� �̻��� �߸���
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
		// ������� ä�� ID�� Ʋ�ȴ�. ġ������ ����
		MessageOut( eCRITICAL_LOG, "����� %s�� ä�� %u�� �������� ����!! �������� ����!!", pUser->GetCharName().c_str(), pUser->GetChannelID() );
		return false;
	}
	BaseZone *pZone = NULL;

	pZone = pChannel->FindZone( pUser->GetZoneKey() );

	if( !pZone )
	{
		// ������� Zone ID�� Ʋ�ȴ�. ġ������ ����
		MessageOut( eCRITICAL_LOG, "����� %s�� ���� %u�� �������� ����!! ���� ����!!", pUser->GetCharName().c_str(), 
																		pUser->GetZoneKey() );
		return false;
	}
	pZone->Send( (BYTE*)&noticeMsg, noticeMsg.GetSize() );

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, noticeMsg.szMsg, vecCmd.begin()->size() );
	MessageOut(eCRITICAL_LOG,  "---------------------------<< %uä�� %uZone����[ZT:%u] >>-------------------------", 
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

	// ���� �ִ� ��� �����鿡�� ������
	UserManager::Instance()->SendToAll( (BYTE*)&msgBRD, msgBRD.GetSize() );

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, msgBRD.szMsg, msgBRD.wLen );
	MessageOut(eCRITICAL_LOG,  "-----------------------------<< ��ü���� >>-----------------------------" );
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

	// �ش� ������ ������ ä�ο��� �����ش�
	Channel *pChannel = ChannelManager::Instance()->GetChannel(pUser->GetChannelID());
	if(!pChannel)
	{
		// ������� ä�� ID�� Ʋ�ȴ�. ġ������ ����
		MessageOut( eCRITICAL_LOG, "����� %s�� ä�� %u�� �������� ����!! ���� ����!!", pUser->GetCharName().c_str(), pUser->GetChannelID() );
		return;
	}
	// ����, ��Ʋ�� ��� ����
	pChannel->SendToAll( (BYTE*)&msgBRD, msgBRD.GetSize() );

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, msgBRD.szMsg, msgBRD.wLen );
	MessageOut(eCRITICAL_LOG,  "----------------------------<< %dä�� ���� >>----------------------------", pUser->GetChannelID() );
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
		// ������� ä�� ID�� Ʋ�ȴ�. ġ������ ����
		MessageOut(eCRITICAL_LOG, "����� %s�� ä�� %u�� �������� ����!! �������� ����!!", pUser->GetCharName().c_str(), pUser->GetChannelID());
		return;
	}

	BaseZone *pZone = NULL;

	pZone = pChannel->FindZone( pUser->GetZoneKey() );
	if( !pZone )
	{
		// ������� Zone ID�� Ʋ�ȴ�. ġ������ ����
		MessageOut( eCRITICAL_LOG, "����� %s�� ���� %u�� �������� ����!! ���� ����!!", pUser->GetCharName().c_str(), 
			pUser->GetZoneKey() );
		return;
	}
	pZone->Send( (BYTE*)&msgBRD, msgBRD.GetSize() );

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, msgBRD.szMsg, msgBRD.wLen );
	MessageOut(eCRITICAL_LOG,  "---------------------------<< %uä�� %uZone����[ZT:%u] >>-------------------------", 
		pUser->GetChannelID(), 
		pUser->GetZoneKey(), pUser->GetStatus() );
	MessageOut(eCRITICAL_LOG,  "%s(%u)", msgBRD.szMsg, msgBRD.wLen );
	MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------" );
}

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

// ���� ���� ���� �Լ�
namespace util {
namespace internal {

// �Է°��� �˻��Ѵ�.
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

    // �Է°��� �˻��Ѵ�.
    if (scoped_check_input(gm_player, gm_command, 5) == false)
    {
        return false;
    }

    // GM ��ɾ ��û �޽����� ��ȯ�Ѵ�.
    MSG_CW_PARTY_FINDING_REGISTER_SYN register_syn;
    scoped_string_to_number(gm_command[1], register_syn.update_wanted_party_finding_key_);
    {
        const size_t dest_count = _countof(register_syn.party_finding_title_);
        ::_tcsncpy(register_syn.party_finding_title_, gm_command[2].c_str(), dest_count);
        register_syn.party_finding_title_[dest_count-1] = _T('\0');
    }
    scoped_string_to_number(gm_command[3], register_syn.num_of_max_joinable_people_);
    scoped_string_to_number(gm_command[4], register_syn.party_goal_);

    // ó���⸦ ȣ���Ѵ�.
    Handler_CW::OnCW_PARTY_FINDING_REGISTER_SYN(gm_player, &register_syn, register_syn.GetSize());
    
    return true;
}

bool GMCmdManager::ProcessPartyFindingGetList(User* const gm_player, 
                                              const GM_CMD_STRING& gm_command) const
{
    using namespace util::internal;

    // �Է°��� �˻��Ѵ�.
    if (scoped_check_input(gm_player, gm_command, 1) == false)
    {
        return false;
    }

    // GM ��ɾ ��û �޽����� ��ȯ�Ѵ�.
    MSG_CW_PARTY_FINDING_GET_LIST_SYN get_list_syn;
    
    // ó���⸦ ȣ���Ѵ�.
    Handler_CW::OnCW_PARTY_FINDING_GET_LIST_SYN(gm_player, &get_list_syn, get_list_syn.GetSize());
    
    return true;
}

bool GMCmdManager::ProcessPartyFindingJoinRequest(User* const gm_player, 
                                                  const GM_CMD_STRING& gm_command) const
{
    using namespace util::internal;

    // �Է°��� �˻��Ѵ�.
    if (scoped_check_input(gm_player, gm_command, 2) == false)
    {
        return false;
    }

    // GM ��ɾ ��û �޽����� ��ȯ�Ѵ�.
    MSG_CW_PARTY_FINDING_JOIN_REQUEST_SYN join_request_syn;
    scoped_string_to_number(gm_command[1], join_request_syn.join_wanted_party_finding_key_);

    // ó���⸦ ȣ���Ѵ�.
    Handler_CW::OnCW_PARTY_FINDING_JOIN_REQUEST_SYN(gm_player, &join_request_syn, join_request_syn.GetSize());

    return true;
}

bool GMCmdManager::ProcessPartyFindingCancel(User* const gm_player, 
                                             const GM_CMD_STRING& gm_command) const
{
    using namespace util::internal;

    // �Է°��� �˻��Ѵ�.
    if (scoped_check_input(gm_player, gm_command, 2) == false)
    {
        return false;
    }

    // GM ��ɾ ��û �޽����� ��ȯ�Ѵ�.
    MSG_CW_PARTY_FINDING_CANCEL_SYN cancel_syn;
    scoped_string_to_number(gm_command[1], cancel_syn.cancel_wanted_party_finding_key_);

    // ó���⸦ ȣ���Ѵ�.
    Handler_CW::OnCW_PARTY_FINDING_CANCEL_SYN(gm_player, &cancel_syn, cancel_syn.GetSize());

    return true;
}

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
