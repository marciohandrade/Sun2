#pragma once
//=============================================================================================================================
/// WorldGuildManager class
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2005. 12. 27
	@remark
		- ���Ӽ������� ��带 �����ϴ� Ŭ����
	@note
		- ��弭������ �޾ƿ� ������� Ŭ������ �����Ѵ�.
		- 1. ���Ӽ����� ������ ĳ���Ͱ� ��带 Find
		- 2. �޴����� ��� ������ ������ �״�� ����ϰ�, ������ ��弭���� ��û�Ѵ�.
	@history 
		-  2006. 01. 16 Modified by Sung Hyun Chang < whatthehell@webzen.co.kr > (���� ���������� ����)
*/
//=============================================================================================================================

#include <struct.h>
#include <SolarHashTable.h>
#include <ResultCode.h>
#include <Singleton.h>

struct MSG_WZ_GUILD_DOMINATION_RETURNHIME_CMD; // packet, information messenger
class WorldGuild;

typedef util::SolarHashTable<WorldGuild *>					GUILD_HASH;
typedef util::SolarHashTable<WorldGuild *>::iterator		GUILD_HASH_ITR;

struct PACKET_FOR_GUILD;

class User;

class WorldGuildManager : public util::Singleton<WorldGuildManager, util::SingletonRelease::kAuto>
{
	enum { MAX_GUILD_POOL_NUM = 1000, };
public:
	WorldGuildManager(void);
	virtual ~WorldGuildManager(void);

	VOID Release();

	RC::eGUILD_RESULT SelectGuildInfo( User * pUser, PACKET_FOR_GUILD & OUT rGuildInfo );
	RC::eGUILD_RESULT DestroyGuild( User * pUser );

	VOID DisplayerGuildInfo();

	inline WorldGuild * FindGuild( GUILDGUID GuildGuid )					{ return m_GuildHash.GetData(GuildGuid); }

	VOID SendGuildMessage( BYTE* pMsg, WORD wSize );

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	VOID	SetChuntingPVPInfo( GUILDGUID GuildGuid, CHUNTING_PVP_INFO & IN PVPInfo );
#endif

	BOOL				ChangeMemberDuty( GUILDGUID GuildGuid, CHARGUID TargetCharGuid, eGUILD_DUTY eGuildDuty);
	BOOL				SendGuildPacket( GUILDGUID GuildGuid, BYTE* pMsg, WORD wSize );
	//VOID				GetCharName( GUILDGUID GuildGuid, CHARGUID CharGuid, OUT TCHAR* pszCharName, IN BYTE byBufSize );

	BOOL				CreateRearCamp( GUILDGUID GuildGuid, BYTE byCorpsNum );
	BOOL				DestroyRearCamp( GUILDGUID GuildGuid, BYTE byCorpsNum );
	VOID				ChangeCommander( GUILDGUID GuildGuid,  CHARGUID TargetCharGuid, BYTE byCorpsNum );
	VOID				ChangeCorps( GUILDGUID GuildGuid,  CHARGUID TargetCharGuid, BYTE byCorpsNum );
	VOID				CreateGuild( const WORLD_GUILD_INFO& rGuildInfo, const BASE_GUILD_MEMBER_INFO& rMemberInfo );
	VOID				LoginGuild( const WORLD_GUILD_INFO& GuildInfo, const BASE_GUILD_MEMBER_INFO& memberInfo );
	RC::eGUILD_RESULT	LogoutGuild( GUILDGUID GuildGuid, CHARGUID CharGuid );
	VOID				RequestGuildRestore( GUILDGUID GuildGuid, CHARGUID CharGuid );
	VOID				GuildRestore( const WORLD_GUILD_INFO& GuildInfo, const BASE_GUILD_MEMBER_INFO& memberInfo );
	VOID				SerializeWorldGuildInfo( GUILDGUID GuildGuid, INOUT WORLD_GUILD_INFO& Info, eSERIALIZE se );
	VOID				SerializeBaseMemberInfo( GUILDGUID GuildGuid, INOUT BASE_GUILD_MEMBER_INFO& Info, eSERIALIZE se );
	VOID				GetCharName( GUILDGUID GuildGuid, CHARGUID CharGuid, OUT char* pszCharName, int nSize );
	VOID				KickMember(  GUILDGUID GuildGuid, CHARGUID CharGuid );
	VOID				JoinGuild( GUILDGUID GuildGuid, const BASE_GUILD_MEMBER_INFO& rMemberInfo );
	VOID				WithdrawGuild( GUILDGUID GuildGuid, CHARGUID CharGuid );
	VOID				DestroyGuild( GUILDGUID GuildGuid );
	RC::eGUILD_RESULT	CanChangeNotice( GUILDGUID GuildGuid, CHARGUID OperCharGuid, const TCHAR* pszNotice, OUT int& nNoticeLen );
    bool ChangeNotice(GUILDGUID guild_guid, const util::StrArrayForGuildNotice& notice);
    VOID                SendDominationHeimReturnMemo( const MSG_WZ_GUILD_DOMINATION_RETURNHIME_CMD* msg );
    BOOL                GetValueInMemo( const TCHAR* pszMemo, eSYSTEM_MEMO_TYPE& OUT type, MAPCODE& OUT mapcode );

protected:
	inline	VOID	RemoveGuild( GUILDGUID GuildGuid )						{ m_GuildHash.Remove(GuildGuid); }
	inline	VOID	AddGuild( WorldGuild * pGuild, GUILDGUID GuildGuid )	{ m_GuildHash.Add(pGuild, GuildGuid); }

//	VOID RemoveOnlineMember( GUILDGUID GuildGuid );
//	VOID AddOnlineMember( WorldGuildMember *pGuild, GUILDGUID GuildGuid );

private:
	GUILD_HASH		m_GuildHash;
};


//extern WorldGuildManager g_WorldGuildManager;