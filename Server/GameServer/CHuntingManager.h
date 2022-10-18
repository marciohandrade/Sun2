#pragma once

#include <MemoryPoolFactory.h>
#include <ObjKeyGenerator.h>
#include "CompetitionTeam.h"

class GameCHuntingRoom;

enum eCOMPETITION_STATE
{
	eCOMPETITION_NONE,
	eCOMPETITION_ALL_ENTER_WAIT,	// �÷��̾ �� ������ ��ٸ��� ����
	eCOMPETITION_START_WAIT,		// ���� ��� ����
	eCOMPETITION_MATCH,				// ���� ����
	eCOMPETITION_END_WAIT,			// ���� ��� ����
};

typedef util::SolarHashTable<CompetitionTeam*>	COMPETITION_INFO_HASH;
typedef COMPETITION_INFO_HASH::iterator			COMPETITION_INFO_HASH_ITR;

class CHuntingManager
{
	enum { MAX_COMPETITION_INFO_NUM = 10 };
	enum { ALL_ENTER_WAIT_TIME = 90*1000 };
	enum { START_WAIT_TIME = 20*1000 };
	enum { RESURRECTION_WAIT_TIME = 5*1000 };
	enum { DISCONNECT_PENALTY_PVPPOINT = 50 };
public:
	CHuntingManager();
	~CHuntingManager();

	VOID				Release();
	VOID				Init( GameCHuntingRoom *pCHuntingZone, ADDITIONAL_CHUNTINGINFO * pInfo );
	VOID				Update();

	VOID				EnterPlayer( Player * pPlayer );
	VOID				LeavePlayer( Player * pPlayer );

	BOOL				CanUsePotion( Player *pPlayer );
	VOID				UsePotion( Player *pPlayer );
	VOID				DeadCharacter( Character *pDeadChar, Character *pKillChar );
	VOID				LevelUp( Player * pPlayer );
	VOID				IncWinCount( Player * pPlayer, int iIncVCount );

	RC::eSTATUS_RESULT	ResurrectionAtSafezone( Player *pPlayer, BOOL bModeCheck );

	CompetitionTeam *	CreateTeam();
	VOID				DestroyTeam( WORD wTeamKey );

	VOID				EnterTeam( BYTE byReadyPlayerNum, DWORD * IN pUserKeyList, GUILDGUID GuildGuid );

	VOID				SetReadyPlayerNum( BYTE byReadyPlayerNum )	{ m_byReadyPlayerNum = byReadyPlayerNum; }
	BYTE				GetMaxTeamMemberNum();
	VOID				SendPacketToAll( MSG_BASE_FORWARD * pMsg, WORD wSize, DWORD dwMyObjKey = 0 );
	BYTE				GetAllPointInfo( WORD TeamKey, CompetitionPoint * OUT pPointInfo );

	BOOL				IsApplyPenalty()	{ return IsTimeAttackMode(); }

	GameCHuntingRoom *	GetCHunting()		{ return m_pCHuntingZone; }
	BOOL				IsCompetition()		{ return m_eCompetitionState == eCOMPETITION_MATCH; }
	eCOMPETITION_STATE	GetCompetitionState()	{ return m_eCompetitionState; }

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	BOOL				IsGuildPVPMode();
	WORD				OnGetMaxPlayTimes()	{ return (WORD)(m_ExpireTimer.GetIntervalTime()/1000); }
#endif

    // _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    VOID                GetTeamWinCount(BYTE* OUT HomeTeamWinCount, 
                                        BYTE* OUT AwayTeamWinCount, 
                                        WORD HomeTeamKey);

private:
	VOID				OnStartWait();	// �������� ���� Wait
	VOID				OnStart();		// �������� ����
	VOID				OnResult();		// ��� ó��

	CompetitionTeam *	DecideTie();				// ���º��ΰ�
	CompetitionTeam *	DecideWinner();				// ���� ���� ����
	VOID 				DecideMemberNumWinner();	// ������� ���� ��
	CompetitionTeam *	DecideKillPointWinner( COMPETITION_INFO_HASH *pTeamHash );	// ų����Ʈ�� ���� ��

	BOOL				IsExpired();
	BOOL				IsReMatchCondition();
	CompetitionTeam *	FindTeam( WORD wTeamKey )	{ return m_CompetionTeamHash.GetData(wTeamKey); }
	Player *			FindPlayer( DWORD dwObjKey );
	BOOL				IsAllPlayerEntered();	// �÷��̾ ��� ���Գ�
	BOOL				IsAllLoseTeamLeft();	// �й��� ���� ��� ������

	CompetitionTeam *	FindReadyTeam( Player *pPlayer );

	BOOL				IsFreeForAllRule()	{ return m_pAdditionalInfo->m_Rule == eCOMPETITION_RULE_FREE_FOR_ALL; }
	BOOL				IsDualPartyRule()	{ return m_pAdditionalInfo->m_Rule == eCOMPETITION_RULE_DUAL_PARTY; }
	BOOL				IsSurvivalMode()	{ return m_pAdditionalInfo->m_Mode == eCOMPETITION_MODE_SURVIVAL; }
	BOOL				IsTimeAttackMode()	{ return m_pAdditionalInfo->m_Mode == eCOMPETITION_MODE_TIME_ATTACK; }
	BOOL				IsDeathMatchMode()	{ return m_pAdditionalInfo->m_Mode == eCOMPETITION_MODE_DEATH_MATCH; }
	
public:
	eUSER_RELATION_TYPE	IsFriendPVP( Player *pAttacker,Player *pTarget );	
	eUSER_RELATION_TYPE	IsFriendFreeForAll( Player *pAttacker, Player *pTarget );
	eUSER_RELATION_TYPE	IsFriendDualParty( Player *pAttacker, Player *pTarget );
	

private:
	GameCHuntingRoom*						m_pCHuntingZone;
	struct {	// CHunting Info
		ADDITIONAL_CHUNTINGINFO*			m_pAdditionalInfo;
		eCOMPETITION_STATE					m_eCompetitionState;
		BYTE								m_byReadyPlayerNum;		// �����Ϸ��� �÷��̾� ��
		WORD								m_WinnerKey;
	};
	struct {	// Time Info
		util::Timer							m_StartWaitTimer;
		util::Timer							m_ExpireTimer;
		BOOL								m_TimeExpired;			// �ð������ ������ ����Ǿ��°�?
	};

	util::CObjKeyGenerator<>				m_CompetionKeyGenerater;
	util::CMemoryPoolFactory<CompetitionTeam>		m_CompetionTeamPool;
	COMPETITION_INFO_HASH					m_CompetionTeamHash;
	COMPETITION_INFO_HASH					m_TempTeamHash;
};












