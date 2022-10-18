#pragma once
//=============================================================================================================================
/// Guild class
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2005. 12. 26
	@remark
		- 길드를 나타내는 클래스
	@note
		- 길드는 실시간 처리를 한다.
		- XXX 와 OnXXX 계열의 함수는 서로 1:1 대응된다. 
		- OnXXX 계열의 함수는 DBR로 결과값이 온 후 호출되는 콜벡스타일의 함수이다.
		- 길드를 받아 오게 되면, 모든 서버에게 broadcast한다.
		- 수정 변경 사항도 모든 서버에게 broadcast한다.
	@history 
		- 
*/
//=============================================================================================================================

#include "GuildCommon.h"

class GuildRankInfo;

#include "GuildCorps.h"
#include "GuildRelation.h"
#include "GuildSmallContentEntry.h"

#include <string>
#include <map>

typedef STLX_MAP< BYTE, GuildCorps* >     GUILD_CORPS_MAP;
typedef GUILD_CORPS_MAP::iterator	            GUILD_CORPS_MAP_ITER;

struct MSG_BASE;
struct BASE_GUILD_INFO;
struct BASE_GUILD_MEMBER_INFO;

class GuildMember;

class Guild
{
	__DECL_GUILDPOOL(Guild)
	friend class GuildManager;

public:
	Guild(void);
	virtual ~Guild(void);

public:
	VOID    InitializeGuild();
	VOID    LogOutMember( CHARGUID CharGuid );
	VOID    SetGuildInfo( BASE_GUILD_INFO & IN rInfo );
	VOID    ReleaseGuild();
	
	BOOL   CanDestroy();
	BOOL   SendDBProxy( MSG_BASE * pMsg, WORD wSize );
	
	GuildMember*      LogInMember( CHARGUID CharGuid );
	GuildMember*      FindMember( CHARGUID CharGuid );
	GuildMember*      FindMemberByCharName( const TCHAR *ptszCharName );

	// 조건 UP(길드용), 하임(마스터용)
	VOID				RankUp();
	RC::eGUILD_RESULT	CanRankUp(  OUT GuildRankInfo& RankInfo );
	BOOL				IsFullMember();

	
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	CHUNTING_PVP_INFO* GetChuntingPVPInfo() { return &m_ChuntingPVPInfo; }
#endif

public:
	BOOL						SetMemberDuty( CHARGUID CharGuid, eGUILD_DUTY eDuty );
	BOOL						IsExistMemberDuty( eGUILD_DUTY eDuty, BYTE byCorpsNum );

public:
			VOID				CreateGuild( GUILDGUID GuidGuid, const TCHAR* tszGuildName, BYTE byCorpsNum, CHARGUID MasterCharGuid, const TCHAR* tszMasterCharName, eGUILD_DUTY MasterDuty,
								BYTE byMasterClass, LEVELTYPE MasterLevel, BYTE byChaoState, INT64 nGuildPanaltyTime, INT64 nDestroyRequestTime, INT64 nMasterChangeTime );
			VOID				Destroy();
			WORD				GetMemberCount();
			GuildCorps*			CreateCorps( BYTE byCorpsNum );
			VOID				DestroyCorps( BYTE byCorpsNum );
			GuildCorps*			CreateCorpsBySequence();
			BYTE				GetEmptyCorpsNumBySequence();
			GuildCorps* 		FindCorps( BYTE corps_number ) const;
			BYTE				GetCorpsCount() const { return static_cast<BYTE>(m_mapGuildCorps.size()); }			//현재 군단개수
			GuildMember*		FindMaster() const;
			const TCHAR*		GetMasterCharName();
			GuildMember*		FindMemberByDuty(eGUILD_DUTY eDuty);
			BYTE				GetEmptyCommanderInfo( BYTE* pEmptyCommderInfo );
			RC::eGUILD_RESULT	JoinGuild( BYTE byCorpsNum, CHARGUID CharGuid, const TCHAR* tszCharName, eGUILD_DUTY Duty, BYTE byClass, LEVELTYPE Level, BYTE byChaoState );
			RC::eGUILD_RESULT	LeaveGuild( CHARGUID CharGuid );
			RC::eGUILD_RESULT	ChangeCorps( CHARGUID CharGuid, BYTE byNewCorpsNum );
			BOOL				IsFullCorpsMemberByDuty( BYTE byCorpsNum, eGUILD_DUTY Duty );
			BOOL				CreateRearCamp( BYTE byCorpsNum );
			BOOL				DestroyRearCamp( BYTE byCorpsNum );
			RC::eGUILD_RESULT	DestroyMember( CHARGUID CharGuid );
			BOOL				GetBaseMemberInfo( CHARGUID CharGuid, OUT BASE_GUILD_MEMBER_INFO& info );
			BOOL				GetMemberInfo( CHARGUID CharGuid, OUT GUILD_MEMBER_INFO& info );
			VOID				ChangeGuildName( const TCHAR* pszGuildName );
			int					GetRelationGuid(STLX_VECTOR<GUILDGUID>& vGuildGuid, eGUILD_RELATION RelationType = eGUILD_RELATION_NONE );
	inline	VOID 				SetGuildName( const TCHAR* pszGuildName );	
	inline	VOID 				SetGuildGP( GPTYPE gp ) { m_GuildInfo.m_GuildPoint = gp; }
	inline	VOID 				SetGuildUP( UPTYPE up ) { m_GuildInfo.m_UserPoint = up; }
	inline	VOID 				SetGuildGrade( eGUILD_GRADE grade ) { m_GuildInfo.m_GuildGrade = grade;	}
	inline	VOID 				SetGuildGuid( GUILDGUID	Guid ) { m_GuildInfo.m_GuildGuid = Guid;	}
	inline	GPTYPE 				GetGuildGP() const { return m_GuildInfo.m_GuildPoint; }
	inline	UPTYPE 				GetGuildUP() const { return m_GuildInfo.m_UserPoint; }
	inline	eGUILD_GRADE 		GetGuildGrade()	const { return static_cast<eGUILD_GRADE>(m_GuildInfo.m_GuildGrade); }
	inline	GUILDGUID			GetGuildGuid() const { return m_GuildInfo.m_GuildGuid; }
	const	TCHAR* 				GetGuildName() const { return m_GuildInfo.m_tszGuildName; }
			VOID				GetWorldGuildInfo( WORLD_GUILD_INFO&  rInfo );
	inline	VOID				SetMasterChangeTime( INT64 nChangeTime ){ m_nMasterChangeTime = nChangeTime; }
	inline	INT64				GetMasterChangeTime() const { return m_nMasterChangeTime; }
#ifdef _NA_20100604_BETTER_DOMINATION_MANAGER
    inline  VOID                SetDominationJointCount( WORD count ){ m_GuildInfo.m_DominationJoinCount = count; }
    inline  VOID                SetDominationDefenseCount( WORD count ){ m_GuildInfo.m_DominationDefenseCount = count; }
    inline  VOID                SetDominationHuntingCount( WORD count ){ m_GuildInfo.m_DominationHuntingCount = count; }
    inline  WORD                GetDominationJoinCount(){ return m_GuildInfo.m_DominationJoinCount; }
    inline  WORD                GetDominationDefenseCount(){ return m_GuildInfo.m_DominationDefenseCount; }
    inline  WORD                GetDominationHuntingCount(){ return m_GuildInfo.m_DominationHuntingCount; }
#endif

	//Serialize
			VOID				SerializeBaseGuildInfo( INOUT BASE_GUILD_INFO&  rInfo, eSERIALIZE se );
			VOID				SerializeGameGuildInfo( INOUT GAME_GUILD_INFO&  rInfo, eSERIALIZE se );
			VOID				SerializeGuildInfo( INOUT GUILD_INFO&  rInfo, eSERIALIZE se );
			VOID				SerializeCorpsInfo( INOUT GUILD_CORPS_TOTAL_INFO&  rInfo, eSERIALIZE se );
			VOID				SerializeBaseMemberTotalInfo( INOUT BASE_GUILD_MEMBER_TOTAL_INFO&  rInfo, eSERIALIZE se );
			VOID				SerializeMemberTotalInfo( INOUT GUILD_MEMBER_TOTAL_INFO&  rInfo, eSERIALIZE se );
			VOID				SerializeServerMemberTotalInfo( INOUT SERVER_GUILD_MEMBER_TOTAL_INFO&  rInfo, eSERIALIZE se );

	const TCHAR*				GetGuildNotice() const { return m_szGuildNotice; }
	VOID                        SetGuildNotice( const TCHAR* pszGuildNotice );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetPenaltyTime(GuildSmallContentsEntry::GuildPenalty::PenaltyTime PenaltyTime) { m_GuildInfo.m_GuildPenaltyTime = PenaltyTime;}
	void SetDestroyTime(GuildSmallContentsEntry::GuildPenalty::PenaltyTime DestroyTime) { m_GuildInfo.m_GuildDestroyTime = DestroyTime;}
	void SetDeleteGuildState(eGUILD_DELCHK eDelChk) { m_GuildInfo.m_GuildDestroyWaitFlag = eDelChk; }
	
	void DeleteGuildMark()
	{ 
		m_GuildInfo.m_PatternIdx= 0; 
		m_GuildInfo.m_BackGroundIdx = 0; 
	}

	void RegisterGuildMark(GUILDMARKIDX i, GUILDMARKIDX p)
	{
		m_GuildInfo.m_PatternIdx = p; 
		m_GuildInfo.m_BackGroundIdx = i; 
	}

	void AcquireGuildMark(GUILDMARKIDX& b, GUILDMARKIDX& p)
	{ 
		p = m_GuildInfo.m_PatternIdx; 
		b = m_GuildInfo.m_BackGroundIdx; 
	}

	GuildSmallContentsEntry::GuildPenalty::PenaltyTime GetPenaltyTime() const { return m_GuildInfo.m_GuildPenaltyTime; }
	GuildSmallContentsEntry::GuildPenalty::PenaltyTime GetDestroyTime() const { return m_GuildInfo.m_GuildDestroyTime; }
	GuildSmallContentsEntry::GuildPenalty::PenaltyTime GetDeleteGuildState() const	{ return m_GuildInfo.m_GuildDestroyWaitFlag; }

	GuildRelationSystem::GuildRelationEntry* GetRelationEntry() { return &m_GuildRelationEntry;  }
	eGUILD_DELCHK CheckGuildDeleteState() const
	{ 
		return m_GuildInfo.m_GuildDestroyWaitFlag;
		//return eGUILD_DELCHK_NORMAL;
	}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	CHUNTING_PVP_INFO	m_ChuntingPVPInfo;
#endif

	GUILD_INFO			m_GuildInfo;
	INT64				m_nMasterChangeTime;			// 이 값 사용하지 말것... 그냥 보존용이다.
	GUILD_CORPS_MAP		m_mapGuildCorps;
	GuildRelationSystem::GuildRelationEntry	m_GuildRelationEntry;
	TCHAR				m_szGuildNotice[MAX_GUILDNOTICE_LENGTH+1];
};
