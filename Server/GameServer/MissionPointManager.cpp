#include "Stdafx.h"
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#include "MissionPointManager.h"
#include "Player.h"
#include "PointRankingParser.h"
#include "GameServerEx.h"
#include <PacketStruct_DG.h>
#include <PacketStruct_GP.h>
#include <SolarDateTime.h>
#include "GameRankingManager.h"

VOID MissionPointManager::OnCreate()
{	
}

VOID MissionPointManager::OnDestroy()
{
	m_HashPlayerPoint.clear();
}

VOID MissionPointManager::JoinPlayer( Player *pPlayer, MAPCODE MapCode )
{
	//추가
	st_PlayerMissionPoint st_Point;
	//st_Point.pPlayer		= pPlayer;
    st_Point.char_guid   = pPlayer->GetCharGuid();
	st_Point.UserGUID		= pPlayer->GetUserGuid();
	st_Point.RankingNum		= MapCode;
	st_Point.Point			= 0;

	MISSION_POINT_HASH::iterator it = m_HashPlayerPoint.find( pPlayer->GetUserGuid() );
	if( it != m_HashPlayerPoint.end() )
	{
		SUNLOG(eCRITICAL_LOG, "[JoinRoom:PointManager]Already exist User - shogen [%u]", pPlayer->GetUserGuid() );
		return;

	}
	m_HashPlayerPoint[st_Point.UserGUID] = st_Point;

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    // 저주받은탑 레벨제한 제거
#else
	WORD PointIndex = g_GameRankingMgr.GetPointRankingIndex( MapCode );	
	st_PointRankingTable* pTable = PointRankingParser::Instance()->GetPointTable( PointIndex );
	if( !pTable )
		return;

	if( !pPlayer || 
		pPlayer->GetLevel() < pTable->lvApplyMinLV ||
		pPlayer->GetLevel() > pTable->lvApplyMaxLV
		)
	{
		MSG_CG_MISSION_POINT_CANNOT_REGISTER_BRD brd;
		pPlayer->SendPacket( &brd, sizeof(brd));
		return;
	}
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

	// 포탈 서버가 켜져있는지 체크?
	if( !(g_pGameServer->IsConnected(RANKING_SERVER)) )
		return;

	//Portal에서 정산시간 받아오기
	MSG_GP_MISSION_RANKING_SETTLE_DATE_SYN SynMsg;

	DWORD64 RegisterTime=0;
	util::GetDateTime_YYYYMMDDHHMMSSNNN( RegisterTime );
	SynMsg.RecordInfo.m_MissionNo		= MapCode;
	SynMsg.RecordInfo.m_ClearTime		= 0;	
	SynMsg.RecordInfo.m_RegisterTime	= RegisterTime;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    SynMsg.RecordInfo.ranking_type = eRANKING_TYPE_POINT;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

	SynMsg.MemberInfo.m_UserGuid	= pPlayer->GetUserGuid();
	SynMsg.MemberInfo.m_Class		= pPlayer->GetCharInfo()->m_byClassCode;
	SynMsg.MemberInfo.m_LV			= pPlayer->GetLevel();
	_tcsncpy( SynMsg.MemberInfo.m_tszCharName, pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	SynMsg.MemberInfo.m_tszCharName[MAX_CHARNAME_LENGTH]= '\0';
	if( 0 != pPlayer->GetCharInfo()->m_GuildGuid )
	{
		_tcsncpy( SynMsg.MemberInfo.m_tszGuildName, pPlayer->GetCharInfo()->m_tszGuildName, MAX_GUILDNAME_LENGTH );
		SynMsg.MemberInfo.m_tszGuildName[MAX_GUILDNAME_LENGTH]= '\0';
	}
	else
	{
		SynMsg.MemberInfo.m_tszGuildName[0]= '\0';
	}
	//SynMsg.SettledStartDate	= g_GameRankingMgr.GetOldSettledDate();		//< 정산 시작날짜
	//SynMsg.Period			= g_GameRankingMgr.GetPeriod();				//< 주기	

	g_pGameServer->SendToServer( RANKING_SERVER, &SynMsg, sizeof(SynMsg) );	
}

VOID MissionPointManager::LeavePlayer( Player *pPlayer, MAPCODE MapCode )
{
	MSG_GP_RANKING_REGISTER_MISSION_POINT_SYN PortalSyn;

	MISSION_POINT_HASH::iterator it = m_HashPlayerPoint.find( pPlayer->GetUserGuid() );
	if( it == m_HashPlayerPoint.end() )
	{
		SUNLOG(eCRITICAL_LOG, "[LeaveRoom:PointManager]Not exist User - shogen [%u]", pPlayer->GetUserGuid() );
		return;
	}

	if( (it->second).bReady == FALSE )
	{
		m_HashPlayerPoint.erase( it );
		return;
	}

	// 포탈 서버가 켜져있는지 체크?
	if( !(g_pGameServer->IsConnected(RANKING_SERVER)) )
	{
		m_HashPlayerPoint.erase( it );
		return;
	}

    //정산중이면 등록 안함
	if( g_GameRankingMgr.GetStatus() != eRANKING_STATUS_NORMAL )
	{
		m_HashPlayerPoint.erase( it );
		return;
	}

    //얻은 포인트 합계가 0보다 작거나 같다면(사실상 0이하는 없다.)포인트 랭킹 0점을 받을시에는 등록되지 않도록 수정
    if( (it->second).Point <= 0 )
    {
        m_HashPlayerPoint.erase( it );
        return;
    }

	DWORD64 RegisterTime=0;
	util::GetDateTime_YYYYMMDDHHMMSSNNN( RegisterTime );
	PortalSyn.RecordInfo.m_MissionNo		= MapCode;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    PortalSyn.RecordInfo.ranking_type       = eRANKING_TYPE_POINT;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	PortalSyn.RecordInfo.m_ClearTime		= (it->second).Point;
	PortalSyn.RecordInfo.m_RegisterTime		= RegisterTime;

	PortalSyn.MemberInfo.m_UserGuid		= pPlayer->GetUserGuid();
	PortalSyn.MemberInfo.m_Class		= pPlayer->GetCharInfo()->m_byClassCode;
	PortalSyn.MemberInfo.m_LV			= pPlayer->GetLevel();
	_tcsncpy( PortalSyn.MemberInfo.m_tszCharName, pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	PortalSyn.MemberInfo.m_tszCharName[MAX_CHARNAME_LENGTH]= '\0';
	if( 0 != pPlayer->GetCharInfo()->m_GuildGuid )
	{
		_tcsncpy( PortalSyn.MemberInfo.m_tszGuildName, pPlayer->GetCharInfo()->m_tszGuildName, MAX_GUILDNAME_LENGTH );
		PortalSyn.MemberInfo.m_tszGuildName[MAX_GUILDNAME_LENGTH]= '\0';
	}
	else
	{
		PortalSyn.MemberInfo.m_tszGuildName[0]= '\0';
	}
	PortalSyn.DBKey						= (it->second).DBKey;
	
	g_pGameServer->SendToServer( RANKING_SERVER, &PortalSyn, sizeof(PortalSyn) );

	if( it != m_HashPlayerPoint.end() )			//쫄아서 함더 체크했음.
	{
		m_HashPlayerPoint.erase( it );
	}	
}

VOID MissionPointManager::KillMonster( eNPC_GRADE grade, GameZone * pZone, WORD PointIndex )
{	
	DWORD dwDropPer = 0;
	DWORD dwPointMin  = 0;
	DWORD dwPointMax  = 0;

	st_PointRankingTable* pTable = PointRankingParser::Instance()->GetPointTable( PointIndex );
	if( !pTable )
	{
		SUNLOG(eCRITICAL_LOG, "Point Ranking Index must not 0 - shogen");
		return;
	}
	
	switch( grade )
	{
	case eNPC_GENERAL:			// 1 : 일반
		dwDropPer	= pTable->dwNomal_Per;
		dwPointMin	= pTable->dwNomal_Min;
		dwPointMax	= pTable->dwNomal_Max;
		break;
	case eNPC_ELITE:			// 2 : 정예
		dwDropPer	= pTable->dwVeteran_Per;
		dwPointMin	= pTable->dwVeteran_Min;
		dwPointMax	= pTable->dwVeteran_Max;
		break;
	case eNPC_LEADER:			// 3 : 리더
		dwDropPer	= pTable->dwLeader_Per;
		dwPointMin	= pTable->dwLeader_Min;
		dwPointMax	= pTable->dwLeader_Max;
		break;
	case eNPC_MIDDLEBOSS:		// 4 : 중보스
		dwDropPer	= pTable->dwCaptain_Per;
		dwPointMin	= pTable->dwCaptain_Min;
		dwPointMax	= pTable->dwCaptain_Max;
		break;
	case eNPC_BOSS:				// 5 : 보스
		dwDropPer	= pTable->dwBoss_Per;
		dwPointMin	= pTable->dwBoss_Min;
		dwPointMax	= pTable->dwBoss_Max;
		break;
	default:
		return;
	}
	
	DWORD GetPoint;
	MSG_CG_MISSION_GET_MISSION_POINT_BRD PlayerBrd;	
	//포인트 드랍 유무 결정
	if( (DWORD)random(1, 10000) < dwDropPer )
	{
		MISSION_POINT_HASH::iterator it = m_HashPlayerPoint.begin();
		while( it != m_HashPlayerPoint.end() )
		{
			if( (it->second).bReady )
			{
                Player* pPlayer = PlayerManager::Instance()->FindPlayerByCharKey((it->second).char_guid);
				if (pPlayer != NULL && !(pPlayer->IsDead()))
				{
					//포인트 점수 결정
					GetPoint = (DWORD)random( dwPointMin, dwPointMax );

					(it->second).Point += GetPoint;

					//유저에게 알림
					PlayerBrd.m_dwPoint = (it->second).Point;
					if( pPlayer )
						pPlayer->SendPacket( &PlayerBrd, sizeof(PlayerBrd) );

					GAMELOG->LogRankingPoint( pPlayer, GetPoint, (it->second).Point );
				}
				
			}
			it++;
		}
	}	
}

VOID MissionPointManager::SetPlayerPoint( USERGUID UserGUID, DWORD Point, RANKINGKEY DBKey )
{
	
	MISSION_POINT_HASH::iterator it = m_HashPlayerPoint.find( UserGUID );
	if( it == m_HashPlayerPoint.end() )
	{
		SUNLOG(eCRITICAL_LOG, "[SetPlayerPoint:PointManager]Not exist User [%u]", UserGUID );
		return;
	}
	
	m_HashPlayerPoint[UserGUID].DBKey	= DBKey;
	m_HashPlayerPoint[UserGUID].Point	= Point;
	m_HashPlayerPoint[UserGUID].bReady	= TRUE;
}

//	(SpecialCmd) 타락한 사원용 최종 정산 처리 목적
//	- 타락한 사원용 처리 방법 (KillMonster는 호출되지 않는다. 최종 계산된 값을 통해 일괄 처리된다.)
VOID
MissionPointManager::SetLastSettleAccount(
								Player* const pPlayer
								, const MAPCODE mapCode
								, const WORD indexOfPoint
								, const WORD totalPoint
								)
{
	st_PointRankingTable* const pTable = PointRankingParser::Instance()->GetPointTable( indexOfPoint );
	if( !pTable )
	{
		SUNLOG(eCRITICAL_LOG, "Point Ranking Index must not 0 - shogen");
		return;
	}

	//	죽거나 살거나 포인트는 등록된다.
	const DWORD userKey = pPlayer->GetUserGuid();
	MISSION_POINT_HASH::iterator it = m_HashPlayerPoint.find( userKey );

	if( it == m_HashPlayerPoint.end() )
	{
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] Invalid Status Player UID:%u - Can't Find", userKey );
		return;
	}

	st_PlayerMissionPoint& rPointInfo = (it->second);
	const BOOLEAN bReady = !!rPointInfo.bReady;
	if( !bReady )
		return;

    // (WAVERIX) (090302) (BUG-FIX) 타락한 사원 포인트 누적 방식인데 오해로 인해 포인트 변경을 했음.
	rPointInfo.Point += totalPoint;

	return;
}
#endif // _NA_008012_20150130_RANKING_SYSTEM