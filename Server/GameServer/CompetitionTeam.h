#pragma once

#include <set>

enum eTEAM_STATE
{
	eTEAM_STATE_NONE,
	eTEAM_STATE_MATCH,		// 경쟁 상태
	eTEAM_STATE_END,		// 종료 상태
};

enum eEXPIRE_STATE
{
	eEXPIRE_STATE_NONE,
	eEXPIRE_STATE_ALL_DEAD,		// 멤버 모두 죽은 상태
	eEXPIRE_STATE_ALL_LEAVE,	// 멤버 모두 떠난 상태
};

class CHuntingManager;

// 킬포인트 sort를 위한 벡터
typedef STLX_VECTOR<Player*>			PLAYER_SORT_VECTOR;
typedef PLAYER_SORT_VECTOR::iterator	PLAYER_SORT_VECTOR_ITR;

class CompetitionTeam
{
	enum { END_WAIT_TIME = 20*1000 };
	enum { MAX_WIN_POINT = 3000 };
	enum { MAX_VICTORY_NUM = 30 };

public:
	CompetitionTeam();
	~CompetitionTeam();

	VOID				Init( CHuntingManager *pMgr, WORD wTeamKey, eCOMPETITION_RULE_TYPE eRule, eCOMPETITION_MODE_TYPE eMode );
	VOID				Update();

	VOID				Start();
	VOID				ResultTie();
	VOID				ResultWin( CompetitionTeam *pLoseTeam, WORD wProcessTime );
	VOID				ResultLose( CompetitionTeam *pWinnerTeam );

	BOOL				IsExpired()			{ return m_State == eTEAM_STATE_END; }
	eEXPIRE_STATE		GetExpireState()	{ return m_eExpireState; }
	BOOL				IsAllDied();

	VOID				AddMember( Player *pPlayer );
	VOID				RemoveMember( DWORD dwObjKey );
	Player *			FindMember( DWORD dwObjKey );
	BYTE				GetMemberNum();
	BYTE				GetAliveMemberNum();

	BYTE				GetAllPointInfo( CompetitionPoint * OUT pPointInfo );
	WORD				GetTeamKey()			{ return m_wTeamKey; }
	WORD				GetKillPoint()			{ return m_wKillPoint; }
	WORD				GetWinPoint()			{ return m_wWinPoint; }

	VOID				SetPartyKey( WORD wPartyKey )	{ m_wPartyKey = wPartyKey; }
	WORD				GetPartyKey()					{ return m_wPartyKey; }

	LEVELTYPE			GetAvgLevel();
	LEVELTYPE			GetTotLevel();

	VOID				IncNPCKillPoint( Player *pKiller, NPC *pNPC );
	VOID				IncPlayerKillPoint( Player *pKiller, Player *pDeadPlayer );
	VOID				UsePotion();
	VOID				LevelUp()				{ m_TotLevel++; }
	VOID				IncWinCount( Player * pPlayer, int iIncVCount );

	VOID				SetChampion( BOOL bChampion )	{ m_bChampion = bChampion; }
	BOOL				IsChampion()					{ return m_bChampion; }

	RC::eSTATUS_RESULT	ResurrectionAtSafezone( Player *pPlayer, BOOL bModeCheck );
	VOID				AllResurrectionAtSafezone();

	VOID				SendPacketToAll( MSG_BASE_FORWARD * pMsg, WORD wSize, DWORD dwMyObjKey = 0 );

	VOID				SetReadyPlayer( BYTE byReadyPlayerNum, DWORD * IN pUserKeyList );
	BOOL				IsAllPlayerEntered();
	BOOL				IsReadyPlayer( DWORD UserKey );

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	GUILDGUID			GetGuildGuid()	{ return m_GuildGuid; }
	VOID				SetGuildGuid( GUILDGUID GuildGuid )	{ m_GuildGuid = GuildGuid; }
#endif

    // _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    BYTE                GetWinCount() { return m_byTeamWinCount; }

private:
	VOID				OnEnd();
	VOID				OnRewards( Player * pPlayer, LEVELTYPE EnemyPartyAvgLV, WORD wProcessTime, BOOL bGetItem );
	BOOL				OnIsExpired();
	VOID				OnCalcRanking();
	WORD				OnGetMaxPlayTimes();
	RC::eSTATUS_RESULT	OnMoveToSafezone( Player *pPlayer );

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	VOID				OnExpel( Player *pPlayer );
	VOID				OnExpels();
#else
	VOID				OnExpel();
#endif

	BOOL				IsCompetition()		{ return m_State == eTEAM_STATE_MATCH; }
	BOOL				IsFreeForAllRule()	{ return m_Rule == eCOMPETITION_RULE_FREE_FOR_ALL; }
	BOOL				IsDualPartyRule()	{ return m_Rule == eCOMPETITION_RULE_DUAL_PARTY; }
	BOOL				IsSurvivalMode()	{ return m_Mode == eCOMPETITION_MODE_SURVIVAL; }
	BOOL				IsTimeAttackMode()	{ return m_Mode == eCOMPETITION_MODE_TIME_ATTACK; }
	BOOL				IsDeathMatchMode()	{ return m_Mode == eCOMPETITION_MODE_DEATH_MATCH; }

private:
	CHuntingManager *		m_pCHuntngMgr;
	WORD					m_wTeamKey;
	WORD					m_wPartyKey;
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	GUILDGUID				m_GuildGuid;
#endif
	eCOMPETITION_RULE_TYPE	m_Rule;
	eCOMPETITION_MODE_TYPE	m_Mode;
	eTEAM_STATE				m_State;
	BOOL					m_bChampion;	// 챔피언인가 도전자인가
	PLAYER_HASH				m_PlayerHash;

	util::Timer				m_EndWaitTimer;

	LEVELTYPE				m_TotLevel;				// 초기멤버 전체레벨
	BYTE					m_byInitMemberNum;		// 초기멤버 수

	WORD					m_wKillPoint;			// 킬 포인트
	WORD					m_wWinPoint;			// 승리 포인트

    // _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    BYTE                    m_byTeamWinCount;    // 연승 횟수

	eEXPIRE_STATE			m_eExpireState;			// 종료상태

	BYTE					m_byReadyPlayerNum;		// 입장하려는 플레이어 수
	STLX_SET<DWORD>			m_setReadyUserKeys;
	BOOL					m_bResultTie;			// 이전판이 무승부 판이었는가
};





