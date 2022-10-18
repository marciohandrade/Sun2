#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include <PacketStruct_AG.h>
#include <PacketStruct_CG.h>
#include <MapInfoParser.h>
#include "MatchInfoParser.h"
#include "ServerOptionParser.h"

//////////////////////////////////////////////////////////////////////////
#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/ServerSessionEx.h>
#include <UserSessions/User.h>

#include <Zone/ZoneManager.h>
#include <Zone/LobbyRoom.h>
#include <Zone/CHuntingRoom.h>
#include <Zone/Village.h>

#include <AgentParty/AgentPartyManager.h>


_IMPL_POOL(CHuntingRoom)
CHuntingRoom::CHuntingRoom(void)
{
	m_Link2ndPartyKey = 0;
}

CHuntingRoom::~CHuntingRoom(void)
{
}

BOOL CHuntingRoom::Process()
{
	// 일정시간이 지난 유저 히스토리는 제거한다.
	DWORD dwTimeDiff = 0;	DWORD dwMaxTimeDiff = 24*3600000;
	BASE_MatchInfo *pMatchInfo = MatchInfoParser::Instance()->GetMatchInfo( MATCH_ABUSE_JOINTIMES );
	if( pMatchInfo )	dwMaxTimeDiff = pMatchInfo->m_dwMatchLimitTime;

	for( PLAYER_HISTORY_ITER it = m_mapPlayerHistory.begin(); it != m_mapPlayerHistory.end(); )
	{
		PLAYER_HISTORY &PlayerInfo = (*it).second;
		dwTimeDiff = GetTickCount() - PlayerInfo.m_dwJoinTime;
		if( dwTimeDiff > dwMaxTimeDiff )
			it = m_mapPlayerHistory.erase( it );
		else
			++it;
	}

	return RoomInterface::Process();
}

WORD CHuntingRoom::GetPlayerHistoryNum( DWORD dwGUID )
{
	// m_mapPlayerHistory.count( dwGUID ); 이걸 사용하면 될건데 -_- 내가 왜 이랬을까? 이걸로 교체할려니 테스트 하기가 싫구나 ㅠㅜ

	WORD wJoinCount = 0;

	PLAYER_HISTORY_ITER FoundIt = m_mapPlayerHistory.find( dwGUID );
	if( FoundIt != m_mapPlayerHistory.end() )
	{
		for( PLAYER_HISTORY_ITER it = FoundIt; it != m_mapPlayerHistory.end(); ++it )
		{
			PLAYER_HISTORY &PlayerInfo = (*it).second;
			if( PlayerInfo.m_dwGUID == dwGUID )
				wJoinCount++;
			else
				break;
		}
	}

	return wJoinCount;
}

VOID CHuntingRoom::ReadyUser( User * pUser, eSEND_TYPE type, MAPCODE FieldCode, DWORD dwAreaID, WzVector * pwvStartPos, bool bFirstJoin )
{
	const sMAPINFO *psMAPINFO = MapInfoParser::Instance()->FindMapInfo( GetMapCode() );
	
	if( !dwAreaID )
	{
		if( !IsCompetition() )
			dwAreaID = psMAPINFO->wStartAreaID;
		else
			dwAreaID = psMAPINFO->wStartAreaID2;
	}

	ZoneInterface::ReadyUser( pUser, type, FieldCode, dwAreaID, pwvStartPos, bFirstJoin );
}

RC::eROOM_RESULT CHuntingRoom::Migrate( KEYTYPE key, RoomInterface * pPreviousRoom, ServerSession * pServerSession )
{
	if( pPreviousRoom->GetType() != eZONETYPE_CHUNTING && pPreviousRoom->GetType() != eZONETYPE_LOBBY )
		return RC::RC_ROOM_INVALIDROOMTYPE;

	RoomInterface::Migrate( key, pPreviousRoom, pServerSession );

	// 현재 파티원 만큼 더 들어올 수 있다.(Dual Party)
	SetLimitMaxUserNum( (BYTE)(pPreviousRoom->GetCurUserNum()*2) );

	if( pPreviousRoom->GetType() == eZONETYPE_CHUNTING )
	{
		CHuntingRoom * pRoom = (CHuntingRoom *)pPreviousRoom;
		SetAdditionalCHuntingInfo( pRoom->GetAdditionalCHuntingInfo() );
	}
	else
	{
		LobbyRoom * pRoom = (LobbyRoom *)pPreviousRoom;
		SetAdditionalCHuntingInfo( pRoom->GetAdditionalCHuntingInfo() );
	}

	On_create();

	// 릴레이 유저 등록
	RelayUser( pPreviousRoom );

    g_ZoneManager.ChangeBattlePlayersCount(this, GetNumberOfExpectedPlayers());

	// 최초로 팀이 등장
	BOOL bCompetitionStart = m_AdditionalInfo.m_Rule == eCOMPETITION_RULE_FREE_FOR_ALL;

	EnterTeam( pPreviousRoom, bCompetitionStart );

	return RC::RC_ROOM_SUCCESS;
}

VOID CHuntingRoom::EnterTeam( RoomInterface *pPreviousRoom, BOOL bCompetitionStart )
{
	//	(WAVERIX)(080726)(NOTE) 패킷부터 해서 현 시점 변경하면 좋아질 수 있는 루틴들이다.
	//	object key 가 검색의 효율성이 월등한 점...

	//	- 새로운 팀이 들어간다고 알린다.
	MSG_AG_CHUNTING_ENTER_TEAM_CMD msgCMD;
	msgCMD.m_RoomKey = GetKey();
	msgCMD.m_byReadyPlayerNum = 0;
	msgCMD.m_GuildGuid = 0;

	User* const pPreviousMaster = pPreviousRoom->GetMaster();
	if( pPreviousMaster )
		msgCMD.m_GuildGuid = pPreviousMaster->GetSelectedCharGuildGuid();

	DWORD userCount = 0;
	const ZONE_USER_LIST& rList = pPreviousRoom->GetUserList();
	FOREACH_CONTAINER( const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST )
	{
		User* const pUser = rNode.second;
		msgCMD.m_UserKeyList[userCount] = pUser->GetUserKey();
		++userCount;
		if( CHUNTING_MAX_MEMBER_NUM <= userCount )
			break;
	}

	ASSERT( (DWORD)rList.size() <= CHUNTING_MAX_MEMBER_NUM );
	msgCMD.m_byReadyPlayerNum = (BYTE)userCount;
	SendToLinkedServer( &msgCMD, msgCMD.GetSize() /*기껏 getsize만들어 놓고 안 사용하는 이유는? ㅋㅋ*/ );

	// 경쟁모드 돌입. 다른 유저 출입불가.
	SetCompetition( bCompetitionStart );
}

VOID CHuntingRoom::On_create()
{
	RoomInterface::On_create();

	// 경쟁모드 초기화
	SetCompetition( FALSE );

	m_bLinkedLobby = FALSE;

	m_mapPlayerHistory.clear();

	MSG_AG_ZONE_CHUNTION_CREATE_CMD msg;
	msg.m_dwKey = GetMaster()->GetUserKey();
	msg.m_MapCode = GetMapCode();
	msg.m_RoomKey = GetKey();
	memcpy( msg.m_pszRoomTitle, GetRoomTitle(), MAX_ROOMTITLE_LENGTH*sizeof(TCHAR) );
	memcpy( msg.m_pszRoomPWD, GetRoomPWD(), MAX_ROOMPASSWORD_LENGTH*sizeof(TCHAR) );
	msg.m_RoomInfo = *GetRoomInfo();
	msg.m_AdditionalInfo = *GetAdditionalCHuntingInfo();
	SendToLinkedServer( &msg, sizeof(msg) );
}

VOID CHuntingRoom::On_destroy()
{
	if( !IsReusedServerSession() )
		ServerSessionManager::Instance()->Decr_LOF_BattleServer( GetLinkedServer()->GetSessionIndex() );

	RoomInterface::On_destroy();

	m_Link2ndPartyKey = 0;
}
VOID CHuntingRoom::On_readyuser( User * pUser )
{
	RoomInterface::On_readyuser(pUser);

	pUser->SetZoneState( ePRS_BEFORE_ENTER_CHUNTING );
	pUser->SetZoneKey( GetKey() );
	pUser->SetZoneType( eZONETYPE_CHUNTING );
}

VOID CHuntingRoom::On_enteruser( User * pUser )
{
	RoomInterface::On_enteruser(pUser);
	pUser->SetZoneState( ePRS_AT_CHUNTING );

	// User History 기록
	PLAYER_HISTORY EnterUser;
	EnterUser.m_dwGUID = pUser->GetUserGUID();
	EnterUser.m_dwJoinTime = GetTickCount();
	m_mapPlayerHistory.insert( PLAYER_HISTORY_PAIR( EnterUser.m_dwGUID, EnterUser ) );
}

VOID CHuntingRoom::On_leaveuser( User * pUser )
{
	BOOL bMaster = GetMaster() == pUser;

	RoomInterface::On_leaveuser(pUser);

	// 최소 인원 제한에 맞지 않으면 방은 파기된다.
	CheckMinLimitUser();

	// 경쟁헌팅에 들어올 수 있는 인원은 현재인원의 2배이다.
	SetLimitMaxUserNum( (BYTE)(GetCurUserNum()*2) );

	// 링크된 로비의 최대인원을 바꾼후 유저들에게 알려준다.
	LobbyRoom *pLobby = g_ZoneManager.FindCHuntingLobbyByCkey( GetKey() );
	if( pLobby )
	{
		pLobby->SetLimitMaxUserNum( (BYTE)GetCurUserNum() );

		MSG_CG_ZONE_CHUNTINGLOBBY_CHANGE_MAXUSER_CMD msg;
		msg.m_byMaxLimitUser = (BYTE)GetCurUserNum();
		pLobby->SendToAll( &msg, sizeof(msg) );
	}

	// 슈퍼방 아이템을 소지한 자가 없으면 슈퍼방 기능이 해제된다.
    if(bMaster)
    {
        User* pNextMaster = RoomInterface::ChangeSpecialType(false, pUser);
        if(pNextMaster)
        {
            AgentPartyManager::Instance()->ChangeMaster(pNextMaster->GetPartyState().GetPartyKey(),
                                                        pNextMaster->GetPlayerKey());
        }
    }
}

VOID CHuntingRoom::CheckMinLimitUser()
{
	// 경쟁진행중에는 방이 파괴되지 않는다.
	if( IsCompetition() )	return;

	const sMAPINFO * pInfo = MapInfoParser::Instance()->FindMapInfo( GetMapCode() );
	if( !pInfo )	return;

	// 최소 인원 제한에 맞지 않으면 방은 파괴된다.
	if( GetCurUserNum() >= pInfo->byMinUserNum )	return;

	// 경쟁헌팅 멤버들을 모두 마을로 내보낸다.
	ExpelAll();

	// 링크된 로비의 멤버들을 모두 마을로 내보낸다.
	LobbyRoom *pLobby = g_ZoneManager.FindCHuntingLobbyByCkey( GetKey() );
	if( pLobby )
		pLobby->ExpelAll();
}

VOID CHuntingRoom::ExpelAll()
{
	const ZONE_USER_LIST& rList = GetUserList();
	ZONE_USER_LIST::const_iterator it	= rList.begin();
	ZONE_USER_LIST::const_iterator itend= rList.end();
	while( it!=itend )
	{
		User* const pUser = it->second;
		++it;

		Village* const pVillage	= g_ZoneManager.FindVillage( pUser->GetSelectedCharMapCode() );
		if( !pVillage )
			continue;

		CheckMoveZonePolicy* const pCheckMoveZonePolicy =
			MoveZonePolicy<MZTF_CHUNTING_TO_VILLAGE>::AlignInfo(
			pUser
			//{		From		|			To		}
			, eZONETYPE_CHUNTING,	eZONETYPE_VILLAGE	//	eZONETYPE
			, GetKey(),				pVillage->GetKey()	//	ZoneKey
			);
		const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone( pCheckMoveZonePolicy, RoomArgument::GetEmpty() );
		if( RC::RC_ROOM_SUCCESS == rt )
		{
			pUser->BeginTransaction( TR_LEAVE_CHUNTING_FOR_VILLAGE );
			pUser->SetTRRoomKey( pVillage->GetKey() );
		}
	}
}

VOID CHuntingRoom::SerializeLoad( CHUNTING_ROOMINFO & OUT rRoomInfo )
{
	rRoomInfo.m_Key					= GetKey();
	rRoomInfo.m_MapCode				= GetMapCode();
	memcpy( rRoomInfo.m_pszRoomTitle, GetRoomTitle(), MAX_ROOMTITLE_LENGTH*sizeof(TCHAR) );
	rRoomInfo.m_byRoomPublic		= GetZonePublic();
	rRoomInfo.m_CurUserNum			= (BYTE)GetUserNum();
	rRoomInfo.m_MaxLimitUserNum		= GetLimitMaxUserNum();
	rRoomInfo.m_RoomInfo			= *GetRoomInfo();
	rRoomInfo.m_AdditionalInfo		= *GetAdditionalCHuntingInfo();
}

RC::eROOM_RESULT CHuntingRoom::_CanJoinEach( User* pUser, eZONEPUBLIC RoomPublic, TCHAR * pszRoomPWD )
{
	// 카오상태 체크
	if(!GetServerOptChaoStateUseZone( GetType()) && pUser->IsChaoState())
	{
		return RC::RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE;
	}

	// 경쟁중에는 다른 멤버가 들어올 수 없다.
	if( IsCompetition() )
		return RC::RC_ROOM_DOING_COMPETITION;

	return RC::RC_ROOM_SUCCESS;
}

VOID CHuntingRoom::CompleteCompetition()
{
	SetCompetition( FALSE );

	// 경쟁이 종료된 후에 남은 유저수가 최소인원제한이 안되면 방이 파괴된다.
	CheckMinLimitUser();
}



















