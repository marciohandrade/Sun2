#pragma once

#include "Player.h"

class CompetitionInfo
{
public:
	CompetitionInfo( Player *pPlayer );
	~CompetitionInfo();

	VOID			Init();
	VOID			PointInit();
	VOID			Update();

	VOID			SetTeamKey( WORD wTeamKey )		{ m_TeamKey = wTeamKey; }
	WORD			GetTeamKey()					{ return m_TeamKey; }

	VOID			SetResurrectionTimer( DWORD dwInterval )	{ m_ResurrectionTimer.SetTimer( dwInterval ); }
	BOOL			IsExpiredResurrection()						{ return m_ResurrectionTimer.IsExpiredManual( FALSE ); }

	inline WORD		GetKillPoint()					{ return m_KillPoint; }
	inline BYTE		GetDieNum()						{ return m_DieNum; }
	inline BYTE		GetMaxSeries()					{ return m_MaxSeries; }
	inline VOID		IncKill( WORD wKillPoint )		{ m_KillPoint += wKillPoint; ++m_Series; if(m_MaxSeries<m_Series) m_MaxSeries=m_Series; }
	inline VOID		IncDie()						{ ++m_DieNum; m_Series=0; }

	VOID			SetRanking( BYTE byRanking )	{ m_byRanking = byRanking; }
	BYTE			GetRanking()					{ return m_byRanking; }

	// 전적 관리
	inline VOID		IncLoseTimes()		{ m_pPlayer->GetCharInfo()->m_iCHuntingLCount++; }
	inline VOID		IncDisTimes()		{ m_pPlayer->GetCharInfo()->m_iCHuntingDisCount++; }
	VOID			IncPlayTimes();
	VOID			IncWinTimes();

	VOID			ClearScore();

private:
	Player *		m_pPlayer;
	WORD			m_TeamKey;
	util::Timer		m_ResurrectionTimer;

	WORD			m_KillPoint;		// 킬 포인트
	BYTE			m_DieNum;			// 죽은 횟수
	BYTE			m_Series;			// 현재 연승
	BYTE			m_MaxSeries;		// 최대 연승

	BYTE			m_byRanking;		// 팀내에서의 킬 포인트 등수 
};





































