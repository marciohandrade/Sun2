#include "StdAfx.h"
#include ".\portalrankingmanager.h"
#include <PacketStruct_DP.h>
#include "PortalServer.h"
#include <ArchiveOfStream.h>
PortalRankingManager g_RankingMgr;

PortalRankingManager::PortalRankingManager(void)
:	m_bInitialized(FALSE)
{
}

PortalRankingManager::~PortalRankingManager(void)
{
}

void PortalRankingManager::Initialize()
{
	if( !m_bInitialized )
	{
		RankingManager::Initialize();

		SAFE_NUMERIC_TYPECAST( int, GetPeriod(), BYTE );
		MSG_DP_RANKING_INFO_REQ_SYN msg;
		msg.m_SettledStartDate		= GetOldSettledDate();
		msg.m_Period				= (BYTE)GetPeriod();
		g_PortalServer.SendDBProxy( &msg, sizeof(msg) );

		m_bInitialized = TRUE;
	}
}

void PortalRankingManager::OnChangedStatus()
{
	MSG_GP_RANKING_CHAGED_STATUS_NTF msg;
	msg.m_ChangedStatus = GetStatus();
	g_PortalServer.SendToSessionType( GAME_SERVER, &msg, sizeof(msg) );

    SUNLOG(eFULL_LOG, "RankingManager Status Change: %d", (int)GetStatus());
}

void PortalRankingManager::OnSettled()
{
	//MSG_GP_RANKING_SETTLED_INFO_CMD m_msg;
	//m_msg.m_Size = MSG_GP_RANKING_SETTLED_INFO_CMD::MAX_BUFFER_SIZE;
	//g_RankingMgr.Serialize( m_msg.m_pBUFFER, m_msg.m_Size,  RANKING_SERIALIZE_LOAD );
	//g_PortalServer.SendToSessionType( GAME_SERVER, &m_msg, m_msg.GetSize() );

	static const int MAX_PAGE = 5;
	for( int type = 0 ; type < eRANKING_MAX ; ++type )
	{
		for( int MissionNo = 0 ; MissionNo < MAX_MAPCODE_NUM ; ++MissionNo )
		{
			MissionRanking * pRanking = FindMissionRanking( (eRANKING_TYPE)type, MissionNo );
			if( !pRanking ) continue;

			for( int page = 0 ; page < MAX_PAGE ; ++page )
			{
				m_msg.m_Type		= type;
				m_msg.m_MissionNo	= MissionNo;
				m_msg.m_Page		= page;
				m_msg.m_Size		= m_msg.MAX_BUFFER_SIZE;
				ArchiveOfStream lArchive(m_msg.m_pBUFFER, m_msg.m_Size);
				pRanking->Serialize( m_msg.m_Page, lArchive, RANKING_SERIALIZE_LOAD );
				m_msg.m_Size = lArchive.GetSize();
				g_PortalServer.SendToSessionType( GAME_SERVER, &m_msg, m_msg.GetSize() );
			}
		}
	}

	MSG_GP_RANKING_SETTLED_INFO_END_NTF ntf;
	g_PortalServer.SendToSessionType( GAME_SERVER, &ntf, sizeof(ntf) );
}