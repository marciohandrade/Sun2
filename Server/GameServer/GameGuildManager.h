#pragma once
//=============================================================================================================================
/// GameGuildManager class
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
		- 
*/
//=============================================================================================================================

class GameGuild;
class CGuildWarehouseSlotContainer;

typedef util::SolarHashTable<GameGuild *>				GUILD_HASH;

typedef std::map<std::string, GameGuild*>	GUILD_NAME_MAP;

struct PACKET_FOR_GUILD;

class GameGuildManager
{
	enum { MAX_GUILD_POOL_NUM = 20, };
public:
	GameGuildManager(void);
	virtual ~GameGuildManager(void);

	VOID Release();

	RC::eGUILD_RESULT	InviteMember( Player * pPlayer, Player * pReceiver );
	RC::eGUILD_RESULT	AnswerMember( Player * pPlayer, Player * pReceiver, BOOL bAccept );
	RC::eGUILD_RESULT	WithdrawMember( Player * pPlayer );
	RC::eGUILD_RESULT	KickMember( Player * pMaster, TCHAR *ptszCharName );
	BOOL				ValidPlayer( Player * pPlayer );

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
			VOID		SetChuntingPVPInfo( GUILDGUID GuildGuid, CHUNTING_PVP_INFO & IN PVPInfo );
#endif
	
	VOID				DisplayerGuildInfo( BOOL bLog = FALSE );
	inline	GameGuild*	FindGuild( GUILDGUID GuildGuid )					{ return m_GuildHash.GetData(GuildGuid); }

	//��� ��å
			VOID				ChangeMemberDuty( GUILDGUID GuildGuid, CHARGUID TargetCharGuid, eGUILD_DUTY eDuty );
			VOID				ChangeMaster( GUILDGUID GuildGuid, CHARGUID OldMasterCharGuid, CHARGUID NewMasterCharGuid );
			RC::eGUILD_RESULT	CanChangeDuty( GUILDGUID GuildGuid, CHARGUID OperCharGuid, CHARGUID TargetCharGuid, eGUILD_DUTY eDuty );
			//���Ӽ��������� Can�� �ǹ̴� ���Ӽ��������� üũ �� �� �ִ� ���¸� üũ �Ը� �ǹ��Ѵ�.
			RC::eGUILD_RESULT	ValidPlayerWithNPC( Player* pPlayer );
			VOID				ChangeCommander( GUILDGUID GuildGuid,  CHARGUID TargetCharGuid, BYTE byCorpsNum );
			VOID				ChangeCorps( GUILDGUID GuildGuid,  CHARGUID TargetCharGuid, BYTE byCorpsNum );
			VOID				CreateRearCamp( GUILDGUID GuildGuid, BYTE byCorpsNum );
			VOID				DestroyRearCamp( GUILDGUID GuildGuid, BYTE byCorpsNum );
			RC::eGUILD_RESULT	CanCreateGuild( Player* pPlayer, OUT MONEY& heim );
			RC::eGUILD_RESULT	CreateGuild( Player* pPlayer, const GAME_GUILD_INFO& rInfo, const BASE_GUILD_MEMBER_INFO& rMemberInfo );
			RC::eGUILD_RESULT	CanDestroyGuild( Player* pPlayer );
			VOID				DestroyGuild( GUILDGUID GuildGuid );
			RC::eGUILD_RESULT	CanJoinGuild( Player* pOperPlayer, Player* pTargetPlayer );
			RC::eGUILD_RESULT	JoinGuild( GUILDGUID GuildGuid, const BASE_GUILD_MEMBER_INFO& rMemberInfo );
			VOID				DestroyMember( const GUILDGUID GuildGuid, const CHARGUID CharGuid );
			RC::eGUILD_RESULT	CanRankUpGuild( const GUILDGUID GuildGuid, const MONEY Money, OUT MONEY& UseHeim );
			VOID				RankUpGuild( const GUILDGUID GuildGuid, BYTE byGrade );
			BOOL			LoginChar( const GAME_GUILD_INFO& rInfo, const BASE_GUILD_MEMBER_INFO& rMemberInfo );
			VOID				Logout( GUILDGUID GuildGuid, CHARGUID CharGuid );
			VOID				LeaveGuild( GUILDGUID GuildGuid, CHARGUID CharGuid );

			VOID				LeaveServer( GUILDGUID GuildGuid, CHARGUID CharGuid );
			VOID				SendPacket( const GUILDGUID GuildGuid, MSG_BASE_FORWARD* pMsg, int nSize, CHARGUID ExceptionCharGuid = 0 );
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            VOID                ApplyGuildFacilityPassiveAllmember(GUILDGUID guild_guid, const bool is_add);
            VOID                ApplyGuildFacilityPassive(Player* player, const bool is_add);
            RC::eGUILD_RESULT   ApplyGuildFacilityActive(Player* player, FCODE facility_code);
            VOID                SendGuildFacilityTotalInfo(Player* player, GUILDGUID guild_guid);
#else
			VOID				EnterServer( const GAME_GUILD_INFO& rInfo, const BASE_GUILD_MEMBER_INFO& rMemberInfo );
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
			GUILD_HASH* GetGuildHash() { return &m_GuildHash; }
			CGuildWarehouseSlotContainer* GetGuildWarehouse(GUILDGUID GuildGuid);

	GameGuild* FindGuildName(std::string strGuildName);
	VOID RemoveGuildName(std::string strGuildName);
	VOID AddGuildName(std::string strGuildName, GameGuild *pGuild);
	GUILD_NAME_MAP	m_GuildNameMap;

	VOID				ChangeGuildName( GUILDGUID guid, const TCHAR* pszGuildName );
	RC::eGUILD_RESULT	CanChangeGuildName( GUILDGUID guid, CHARGUID OperCharGuid, const TCHAR* pszGuildName );

#ifdef _NA_008334_20150608_SONNENSCHEIN
    //��� �ҳٻ��� ����� �ʱ�ȭ��Ų��
    VOID InitializeAllSonnenScheinPhase();
    BOOL SetGuildSonnenScheinPhase(GUILDGUID guid, BYTE phase);
#endif //_NA_008334_20150608_SONNENSCHEIN

protected:

	inline VOID RemoveGuild( GUILDGUID GuildGuid )						{ m_GuildHash.Remove(GuildGuid); }
	inline VOID AddGuild( GameGuild * pGuild, GUILDGUID GuildGuid )		{ m_GuildHash.Add(pGuild, GuildGuid); }

private:
	GUILD_HASH		m_GuildHash;
};


extern GameGuildManager g_GameGuildManager;