#pragma once

#include "GMCmdParser.h"
#include "GMCmdString.h"

class User;
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
class Handler_CW;
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

class GMCmdManager
{
public:
	GMCmdManager(void);
	~GMCmdManager(void);

	bool					ParseCommand( User* pUser, const char* szCmd );
	bool					ProcessCommand( User* pUser, GM_CMD_STRING& vecCmd );
	BOOL					IsGMUser( User *pUser );

	bool					ProcessNoticeType(User *pUser, eNOTICE_TYPE eType, TCHAR *ptszNoticeMsg);

private:
	bool ProcessWorldNotice( User* pUser, GM_CMD_STRING& vecCmd );
	bool ProcessChannelNotice( User* pUser, GM_CMD_STRING& vecCmd );
	bool ProcessZoneNotice( User* pUser, GM_CMD_STRING& vecCmd );

	VOID ProcessWorldNotice(User *pUser, TCHAR *ptszNoticeMsg);
	VOID ProcessChannelNotice(User *pUser, TCHAR *ptszNoticeMsg);
	VOID ProcessZoneNotice(User *pUser, TCHAR *ptszNoticeMsg);

public:
	GMCmdParser		m_Parser;
	GMCmdString		m_GMString;
	GM_CMD_STRING	m_vecCmd;

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
private:
    bool ProcessPartyFindingRegister(User* const gm_player, 
                                     const GM_CMD_STRING& gm_command) const; // 파티 모집 등록
    bool ProcessPartyFindingGetList(User* const gm_player, 
                                    const GM_CMD_STRING& gm_command) const; // 파티 모집 목록 얻기
    bool ProcessPartyFindingJoinRequest(User* const gm_player, 
                                        const GM_CMD_STRING& gm_command) const; // 파티 모집 참가 신청
    bool ProcessPartyFindingCancel(User* const gm_player, 
                                   const GM_CMD_STRING& gm_command) const; // 파티 모집 취소
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
};
