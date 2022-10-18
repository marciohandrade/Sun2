#pragma once
//=============================================================================================================================
/// WorldGuild class
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2005. 12. 27
	@remark
		- 길드 정보를 활용하는 목적의 게임 길드용 클래스
	@note
		- 
	@history 
		-  2006. 01. 16 Modified by Sung Hyun Chang < whatthehell@webzen.co.kr > (월드 서버용으로 수정)
*/
//=============================================================================================================================

#include <struct.h>
#include <SolarHashTable.h>
#include "WorldGuildCommon.h"

class User;
class WorldGuildMember;
struct PACKET_FOR_GUILD;
struct PACKET_FOR_GUILD_MEMBER;

typedef util::SolarHashTable<WorldGuildMember *>				GUILDMEMBER_HASH;

class WorldGuild
{
	__DECL_GUILDPOOL(WorldGuild)
	enum { _MAX_GUILDMEMBER_NUM = 140, };
public:
	WorldGuild(void);
	virtual ~WorldGuild(void);

	VOID Destroy();
	VOID Withdraw( CHARGUID CharGuid );

	inline	GUILDGUID	GetGuildGuid()			{ return m_Info.m_GuildGuid; }
	inline	DWORD		GetMemberNum()			{ return m_MemberHash.GetDataNum(); }
	inline	BOOL		IsFullMember()			{ return TRUE; }
	const	TCHAR*		GetGuildName(){ return m_Info.m_tszGuildName; }
			VOID		ChangeGuildName(const TCHAR* pszGuildName);
	GUILDMEMBER_HASH::iterator	GetMemberIterHead() { return m_MemberHash.begin(); }
	GUILDMEMBER_HASH::iterator	GetMemberIterEnd() { return m_MemberHash.end(); }
	inline WorldGuildMember * FindMember( CHARGUID GuildGuid )				{ return m_MemberHash.GetData(GuildGuid); }


#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	VOID	SetChuntingPVPInfo( CHUNTING_PVP_INFO & IN PVPInfo );
#endif
	VOID	OnGuildAdditionInfo(BYTE byRelCnt, BYTE byCorpsCnt, const BASE_GUILD_ADD_INFO & IN m_AddInfo);
	BOOL	ChangeMemberDuty(CHARGUID TargetCharGuid, eGUILD_DUTY eGuildDuty);

	VOID		CreateRearCamp( BYTE byCorpsNum );
	VOID		DestroyRearCamp( BYTE byCorpsNum );
	VOID		Create( const WORLD_GUILD_INFO& rInfo, const BASE_GUILD_MEMBER_INFO& rMemberInfo );
	VOID		SendAllMember( BYTE* pSendMsg, WORD wSize );
	VOID		SendCorpsMember( BYTE byCorpsNum, BYTE* pSendMsg, WORD wSize );
	VOID		SerializeWorldGuildInfo( INOUT WORLD_GUILD_INFO& Info, eSERIALIZE se );
	VOID		SerializeBaseMemberInfo( INOUT BASE_GUILD_MEMBER_INFO& Info, eSERIALIZE se );
	VOID		Join( const BASE_GUILD_MEMBER_INFO& rMemberInfo );
	VOID		CreateMember( const BASE_GUILD_MEMBER_INFO& rMemberInfo );
	VOID		DestroyMember( const CHARGUID& CharGuid );

	const	TCHAR*		GetGuildNotice();
			VOID		SetGuildNotice( const util::StrArrayForGuildNotice&  ptszNotice );

public:
	inline VOID RemoveMember( CHARGUID  CharGuid )							{ m_MemberHash.Remove(CharGuid); }
	inline VOID AddMember( WorldGuildMember * pGuild, CHARGUID CharGuid )	{ m_MemberHash.Add(pGuild, CharGuid); }


private:
	GUILDMEMBER_HASH	m_MemberHash;
	BOOL				m_bRearCamp[MAX_GUILD_CORPS_NUM];			//훈련캠프 창설여부.
	WORLD_GUILD_INFO	m_Info;
};

