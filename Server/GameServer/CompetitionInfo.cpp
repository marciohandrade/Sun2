#include "StdAfx.h"
#include "Competitioninfo.h"
#include "PacketStruct_AG.h"


CompetitionInfo::CompetitionInfo( Player *pPlayer ) : m_pPlayer(pPlayer)
{
}

CompetitionInfo::~CompetitionInfo()
{
}

VOID CompetitionInfo::Init()
{
	m_TeamKey = 0;
	PointInit();
}

VOID CompetitionInfo::PointInit()
{
	m_KillPoint = 0;
	m_DieNum = m_Series = m_MaxSeries = m_byRanking = 0;
}

VOID CompetitionInfo::Update()
{
}

VOID CompetitionInfo::IncPlayTimes()
{
	m_pPlayer->GetCharInfo()->m_iCHuntingTotCount++;

	// Agent에 플레이횟수를 알려준다.
	MSG_AG_CHUNTING_START_COMPETITION_CMD CmdMsg;
	CmdMsg.m_TotCount = m_pPlayer->GetCharInfo()->m_iCHuntingTotCount;
	m_pPlayer->SendPacket( &CmdMsg, sizeof(CmdMsg) );
}

VOID CompetitionInfo::IncWinTimes()
{
	m_pPlayer->GetCharInfo()->m_iCHuntingVCount++;

	// Agent에 승리횟수를 알려준다.
	MSG_AG_CHUNTING_WIN_CMD CmdMsg;
	CmdMsg.m_WinCount = m_pPlayer->GetCharInfo()->m_iCHuntingVCount;
	CmdMsg.m_TotCount = m_pPlayer->GetCharInfo()->m_iCHuntingTotCount;
	m_pPlayer->SendPacket( &CmdMsg, sizeof(CmdMsg) );
}

VOID CompetitionInfo::ClearScore()
{
	m_pPlayer->GetCharInfo()->m_iCHuntingDisCount = 0;
	m_pPlayer->GetCharInfo()->m_iCHuntingVCount = 0;
	m_pPlayer->GetCharInfo()->m_iCHuntingLCount = 0;
	m_pPlayer->GetCharInfo()->m_iCHuntingTotCount = 0;

	// Agent에 승리횟수를 알려준다.
	MSG_AG_CHUNTING_WIN_CMD CmdMsg;
	CmdMsg.m_WinCount = m_pPlayer->GetCharInfo()->m_iCHuntingVCount;
	CmdMsg.m_TotCount = m_pPlayer->GetCharInfo()->m_iCHuntingTotCount;
	m_pPlayer->SendPacket( &CmdMsg, sizeof(CmdMsg) );
}






















