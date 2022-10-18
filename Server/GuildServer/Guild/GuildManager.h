#pragma once


//=============================================================================================================================
/// GuildManager class
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2005. 12. 26
	@remark
		- 길드,길드원을 관리하는 클래스
	@note
		- 길드는 실시간 처리를 한다.
		- Agent와 Game서버와의 처리를 하는 인터페이스 창구이다.
		- GuildManager는 Guild를 Guild는 GuildMember를 관리한다.
		- 먼저 생성후 DBProxy로 부터 결과가 오면 길드 정보를 처리한다. ( 선생성 후처리 )
		[길드원정보의 처리 방법]
		- 길드원이 로그오프시 길드에서 Remove하지 않는다!
		- DB로 부터 SelectMember해서 받은 길드원 정보를 다 가지고 있는다.
	@history 
		- 
*/
//=============================================================================================================================

#include "GuildCommon.h"

class Guild;
class GuildMember;

struct BASE_GUILD_INFO;
struct BASE_GUILD_MEMBER_INFO;

#include <GuildRankInfoParser.h>
#include "GuildSmallContentEntry.h"

class GuildManager
{
public:
	GuildManager(void);
	~GuildManager(void);

	// #길드공지변경#
	RC::eGUILD_RESULT	ChangeGuildNotice( DWORD dwKey, GUILDGUID GuildGuid, TCHAR *ptszNotice );

	// #랭크업#
	VOID				RankUpGuild( GUILDGUID GuildGuid );

    RC::eGUILD_RESULT	CanRankUp( GUILDGUID GuildGuid, CHARGUID OperCharGuid, GuildRankInfo& RankInfo );
    VOID				SetGuildTransaction( GUILDGUID GuildGuid, eGUILD_TRANSACTION tran );
	BOOL				IsGuildTransaction( GUILDGUID GuildGuid, eGUILD_TRANSACTION trans = GUILD_TRANS_ING  );

	VOID				OnChangeNoticeGuild( BOOL bSuccess, GUILDGUID GuildGuid, TCHAR * OUT ptszChangedNotice );

	// #길드원 카오상태 변경#
	RC::eGUILD_RESULT	ChangeChaoState( GUILDGUID GuildGuid, CHARGUID CharGuid, BYTE byChaoState );
	inline Guild*		FindGuild( GUILDGUID GuildGuid )		{ return m_GuildHash.GetData(GuildGuid); }
	
	Guild*				FindGuildName(std::string);
	VOID 				RemoveGuildName(std::string);
	VOID 				AddGuildName(std::string, GUILDGUID);
	GUILDGUID			GetGuildGuidByGuildName(std::string guild_name);
	CHARGUID			GetGuildMasterByGuildName(std::string guild_name);
	
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	RC::eGUILD_RESULT	ChuntingPVPResult(GUILDGUID WinGuildGuid, GUILDGUID LoseGuildGuid, GUILDGUID DisGuildGuid);
#endif

	VOID				DisplayGuildInfo();

	void UpdateForGuildDestroy()					{ m_GuildPenaltyMgr.update(); }
	GuildSmallContentsEntry::GuildPenalty::GuildPenaltyMgr* GetGuildPenaltyMgr()	{ return &m_GuildPenaltyMgr; }

	RC::eGUILD_RESULT	CanChangeDuty( GUILDGUID GuildGuid, CHARGUID OperCharGuid, CHARGUID TargetGuid, eGUILD_DUTY ChangeDuty );
	BOOL				ChangeMemberDuty( GUILDGUID GuildGuid, CHARGUID TargetCharGuid, eGUILD_DUTY eDuty);
	GuildMember*		FindGuildMemberByName( GUILDGUID GuildGuid, TCHAR* pszMemberName );
	GuildMember*		FindGuildMember( GUILDGUID GuildGuid, CHARGUID CharGuid );
	BOOL				CheckRight( GUILDGUID GuildGuid, CHARGUID OperGuid, eGUILD_RIGHTS Right );
	VOID				GetCharName( GUILDGUID GuildGuid, CHARGUID CharGuid, OUT TCHAR* pszCharName, IN BYTE byBufSize );
	RC::eGUILD_RESULT	IsDelegateChar( GUILDGUID GuildGuid, CHARGUID TargetGuid );
	RC::eGUILD_RESULT	IsDelegateChar( GUILDGUID GuildGuid, TCHAR* pszTargetCharName );
	BOOL				CheckDelegatedRight( const eGUILD_DUTY& Duty );
	VOID				GetDelegateRight( GUILDGUID GuildGuid, TCHAR* pszTargetCharName, OUT GUILD_RIGHT_USE_INFO* pDelegateRight );
	BOOL				IsDelegateRight( const GUILD_RIGHT_USE_INFO* pRightInfo );
	VOID				ChangeDelegateRight( GUILDGUID GuildGuid, CHARGUID TargetGuid, GUILD_RIGHT_USE_INFO* pDelegateRight );
	BOOL				ChangeMaster( GUILDGUID GuildGuid, CHARGUID OldMasterCharGuid , CHARGUID NewMasterCharGuid, INT64 nMasterChangeTime );
	RC::eGUILD_RESULT	CanChangeMaster( GUILDGUID GuildGuid, CHARGUID OldMasterCharGuid , TCHAR* pszNewMasterName );
	BYTE				GetMaxMemberByDuty( BYTE byCorpsNum, eGUILD_DUTY duty );
	RC::eGUILD_RESULT	CanRightDelegate( GUILDGUID GuildGuid, CHARGUID OperCharGuid, TCHAR* pszTargetCharName, const GUILD_RIGHT_USE_INFO* pRightInfo );
	CHARGUID FindRightMember(GUILDGUID GuildGuid, eGUILD_DUTY eDuty);

	// Guild
	RC::eGUILD_RESULT	CanCreateGuild( CHARGUID MasterCharGuid, LEVELTYPE MasterLV  );
	RC::eGUILD_RESULT	CreateGuild( GUILDGUID GuildGuid, const TCHAR* tszGuildName, BYTE byCorpsNum, CHARGUID MasterCharGuid, const TCHAR* tszMasterCharName, eGUILD_DUTY MasterDuty, 
		BYTE byMasterClass, LEVELTYPE MasterLevel, BYTE byMasterChaoState, INT64 nGuildPanaltyTime, INT64 nDestroyRequestTime, INT64 nMasterChangeTime );
	VOID			  	DestroyGuild( GUILDGUID GuildGuid );
	RC::eGUILD_RESULT  	CanDestroyGuild( GUILDGUID GuildGuid, CHARGUID OperCharGuid );
	GuildMember*		FindMaster( GUILDGUID GuildGuid );
	// Corps
	RC::eGUILD_RESULT	CreateCorps( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID TargetCharGuid );
	RC::eGUILD_RESULT  	CanCreateCorps( GUILDGUID GuildGuid, CHARGUID OperCharGuid, const TCHAR* pszTargetCharName, OUT BYTE& byCorpsNum );
	RC::eGUILD_RESULT	CanBeCommander(  GUILDGUID GuildGuid, CHARGUID CommanderCharGuid );
	BYTE				GetEmptyCommanderInfo( GUILDGUID GuildGuid, BYTE* pEmptyCommanderInfo );
	RC::eGUILD_RESULT  	CanDestoryCorps( GUILDGUID GuildGuid, CHARGUID OperCharGuid, BYTE byCorpsNum );
	RC::eGUILD_RESULT  	DestoryCorps( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID operCharGuid );
	// Commander
	RC::eGUILD_RESULT  	CanChangeCommander( GUILDGUID GuildGuid, CHARGUID OperCharGuid, const TCHAR* tszMasterCharName, BYTE byCorpsNum );
	RC::eGUILD_RESULT	ChangeCommander( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID CommanderGuid);
	RC::eGUILD_RESULT  	CanChangeCorps( GUILDGUID GuildGuid, CHARGUID OperCharGuid, const TCHAR* tszMasterCharName, BYTE byCorpsNum );
	RC::eGUILD_RESULT	ChangeCorps( GUILDGUID GuildGuid, CHARGUID TargetCharGuid, BYTE byCorpsNum );
	// RearCamp
	RC::eGUILD_RESULT  	CanCreateRearCamp( GUILDGUID GuildGuid, CHARGUID OperCharGuid, const TCHAR* pszTargetCharName, OUT BYTE& byCorpsNum );
	RC::eGUILD_RESULT   CreateRearCamp( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID TargetCharGuid );
	RC::eGUILD_RESULT   CanDestroyRearCamp( GUILDGUID GuildGuid, CHARGUID OperCharGuid, BYTE byCorpsNum );
	RC::eGUILD_RESULT  	DestroyRearCamp( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID operCharGuid );

	// #길드가입#
	RC::eGUILD_RESULT	CanInviteGuild( const GUILDGUID GuildGuid, const CHARGUID OperCharGuid, const CHARGUID TargetCharGuid, const LEVELTYPE TargetLevel );
	RC::eGUILD_RESULT	CanJoinGuild( const GUILDGUID& GuildGuid, const CHARGUID& OperCharGuid, const CHARGUID& TargetCharGuid, const LEVELTYPE& TargetLevel, OUT eGUILD_DUTY& Duty, OUT BYTE& byCorpsNum );
	RC::eGUILD_RESULT	JoinGuild( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID CharGuid, const TCHAR* tszCharName, eGUILD_DUTY Duty, BYTE byClass, LEVELTYPE Level, BYTE byChaoState );
	// #길드탈퇴#
	RC::eGUILD_RESULT	CanLeaveGuild( GUILDGUID GuildGuid, CHARGUID CharGuid );
	RC::eGUILD_RESULT	LeaveGuild( GUILDGUID GuildGuid, CHARGUID CharGuid );
	// #길드강퇴#
	RC::eGUILD_RESULT	CanKickMember( GUILDGUID GuildGuid, CHARGUID OperCharGuid, TCHAR* ptszCharName );
	RC::eGUILD_RESULT	KickMember( GUILDGUID GuildGuid, CHARGUID OperCharGuid, CHARGUID TargetCharGuid );
	// Serialize 
	BOOL				SerializeBaseGuildInfo( GUILDGUID GuildGuid, INOUT BASE_GUILD_INFO& info, eSERIALIZE se );
	BOOL				SerializeGameGuildInfo( GUILDGUID GuildGuid, INOUT GAME_GUILD_INFO& info, eSERIALIZE se );
	BOOL				SerializeGuildInfo( GUILDGUID GuildGuid, INOUT GUILD_INFO& info, eSERIALIZE se );
	BOOL				SerializeCorpsInfo( GUILDGUID GuildGuid, INOUT GUILD_CORPS_TOTAL_INFO& info, eSERIALIZE se );
	BOOL				SerializeBaseMemberTotalInfo( GUILDGUID GuildGuid, INOUT BASE_GUILD_MEMBER_TOTAL_INFO& info, eSERIALIZE se );
	BOOL				SerializeMemberTotalInfo( GUILDGUID GuildGuid, INOUT GUILD_MEMBER_TOTAL_INFO& info, eSERIALIZE se );
	BOOL				SerializeServerMemberTotalInfo( GUILDGUID GuildGuid, INOUT SERVER_GUILD_MEMBER_TOTAL_INFO& info, eSERIALIZE se );
	BOOL				GetGuildBaseMemberInfo( GUILDGUID GuildGuid, CHARGUID CharGuid, OUT BASE_GUILD_MEMBER_INFO& info );
	BOOL				GetGuildMemberInfo( GUILDGUID GuildGuid, CHARGUID CharGuid, OUT GUILD_MEMBER_INFO& info );
	BOOL				GetWorldGuildInfo( GUILDGUID GuildGuid, OUT WORLD_GUILD_INFO& info );
	// Login/out
	RC::eGUILD_RESULT	CanLogin( GUILDGUID GuildGuid, CHARGUID CharGuid );
	RC::eGUILD_RESULT	Login( GUILDGUID GuildGuid, CHARGUID CharGuid );
	RC::eGUILD_RESULT	Logout( GUILDGUID GuildGuid, CHARGUID CharGuid );
	RC::eGUILD_RESULT	CanMoveServer( GUILDGUID GuildGuid, CHARGUID CharGuid );
	// UP/GP 변환
	RC::eGUILD_RESULT	CanConvertUP2GP( GUILDGUID GuildGuid, CHARGUID OperGuid, GPTYPE ConverGP );
	BOOL				SetPoint( GUILDGUID GuildGuid, GPTYPE gp, UPTYPE up );
	RC::eGUILD_RESULT	CanDonateUP( GUILDGUID GuildGuid, CHARGUID CharGuid );
	RC::eGUILD_RESULT	DonateUP( GUILDGUID GuildGuid, UPTYPE TotalUP, CHARGUID CharGuid, UPTYPE DonateUP );
	// 레벨업			
	RC::eGUILD_RESULT	LevelUpMember( GUILDGUID GuildGuid, CHARGUID CharGuid, LEVELTYPE Level );
	// total 정보 요청
	RC::eGUILD_RESULT	CanRequestTotalInfo( GUILDGUID GuildGuid, CHARGUID CharGuid );
	VOID				SetGuildNotice( GUILDGUID GuildGuid, const TCHAR* pszNotice );
	VOID					ChangeGuildName( GUILDGUID guid, const TCHAR* pszGuildName );
	RC::eGUILD_RESULT		CanChangeGuildName( GUILDGUID guid, CHARGUID OperCharGuid = 0 );

protected:
	BOOL IsConnectedDBProxy();
	inline VOID AddGuild( Guild * pGuild, GUILDGUID GuildGuid ) 
	{ 
		m_GuildHash.Add(pGuild, GuildGuid); 
	}

	inline VOID RemoveGuild( GUILDGUID GuildGuid )		
	{ 
		m_GuildHash.Remove(GuildGuid); 
	}

private:
	enum { _MAX_GUILD_POOL_NUM = 1000, _MAX_GUILD_MEMBER_POOL_NUM = 20000, };
	//GUILDMEMBER_HASH	m_MemberHash;
	GUILD_HASH			m_GuildHash;
	GuildSmallContentsEntry::GuildPenalty::GuildPenaltyMgr	m_GuildPenaltyMgr;
	GUILD_NAME_HASH m_GuildNameMap;
};

extern GuildManager g_GuildManager;