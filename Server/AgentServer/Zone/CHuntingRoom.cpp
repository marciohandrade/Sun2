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
	// �����ð��� ���� ���� �����丮�� �����Ѵ�.
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
	// m_mapPlayerHistory.count( dwGUID ); �̰� ����ϸ� �ɰǵ� -_- ���� �� �̷�����? �̰ɷ� ��ü�ҷ��� �׽�Ʈ �ϱⰡ �ȱ��� �Ф�

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

	// ���� ��Ƽ�� ��ŭ �� ���� �� �ִ�.(Dual Party)
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

	// ������ ���� ���
	RelayUser( pPreviousRoom );

    g_ZoneManager.ChangeBattlePlayersCount(this, GetNumberOfExpectedPlayers());

	// ���ʷ� ���� ����
	BOOL bCompetitionStart = m_AdditionalInfo.m_Rule == eCOMPETITION_RULE_FREE_FOR_ALL;

	EnterTeam( pPreviousRoom, bCompetitionStart );

	return RC::RC_ROOM_SUCCESS;
}

VOID CHuntingRoom::EnterTeam( RoomInterface *pPreviousRoom, BOOL bCompetitionStart )
{
	//	(WAVERIX)(080726)(NOTE) ��Ŷ���� �ؼ� �� ���� �����ϸ� ������ �� �ִ� ��ƾ���̴�.
	//	object key �� �˻��� ȿ������ ������ ��...

	//	- ���ο� ���� ���ٰ� �˸���.
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
	SendToLinkedServer( &msgCMD, msgCMD.GetSize() /*�ⲯ getsize����� ���� �� ����ϴ� ������? ����*/ );

	// ������ ����. �ٸ� ���� ���ԺҰ�.
	SetCompetition( bCompetitionStart );
}

VOID CHuntingRoom::On_create()
{
	RoomInterface::On_create();

	// ������ �ʱ�ȭ
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

	// User History ���
	PLAYER_HISTORY EnterUser;
	EnterUser.m_dwGUID = pUser->GetUserGUID();
	EnterUser.m_dwJoinTime = GetTickCount();
	m_mapPlayerHistory.insert( PLAYER_HISTORY_PAIR( EnterUser.m_dwGUID, EnterUser ) );
}

VOID CHuntingRoom::On_leaveuser( User * pUser )
{
	BOOL bMaster = GetMaster() == pUser;

	RoomInterface::On_leaveuser(pUser);

	// �ּ� �ο� ���ѿ� ���� ������ ���� �ı�ȴ�.
	CheckMinLimitUser();

	// �������ÿ� ���� �� �ִ� �ο��� �����ο��� 2���̴�.
	SetLimitMaxUserNum( (BYTE)(GetCurUserNum()*2) );

	// ��ũ�� �κ��� �ִ��ο��� �ٲ��� �����鿡�� �˷��ش�.
	LobbyRoom *pLobby = g_ZoneManager.FindCHuntingLobbyByCkey( GetKey() );
	if( pLobby )
	{
		pLobby->SetLimitMaxUserNum( (BYTE)GetCurUserNum() );

		MSG_CG_ZONE_CHUNTINGLOBBY_CHANGE_MAXUSER_CMD msg;
		msg.m_byMaxLimitUser = (BYTE)GetCurUserNum();
		pLobby->SendToAll( &msg, sizeof(msg) );
	}

	// ���۹� �������� ������ �ڰ� ������ ���۹� ����� �����ȴ�.
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
	// ���������߿��� ���� �ı����� �ʴ´�.
	if( IsCompetition() )	return;

	const sMAPINFO * pInfo = MapInfoParser::Instance()->FindMapInfo( GetMapCode() );
	if( !pInfo )	return;

	// �ּ� �ο� ���ѿ� ���� ������ ���� �ı��ȴ�.
	if( GetCurUserNum() >= pInfo->byMinUserNum )	return;

	// �������� ������� ��� ������ ��������.
	ExpelAll();

	// ��ũ�� �κ��� ������� ��� ������ ��������.
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
	// ī������ üũ
	if(!GetServerOptChaoStateUseZone( GetType()) && pUser->IsChaoState())
	{
		return RC::RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE;
	}

	// �����߿��� �ٸ� ����� ���� �� ����.
	if( IsCompetition() )
		return RC::RC_ROOM_DOING_COMPETITION;

	return RC::RC_ROOM_SUCCESS;
}

VOID CHuntingRoom::CompleteCompetition()
{
	SetCompetition( FALSE );

	// ������ ����� �Ŀ� ���� �������� �ּ��ο������� �ȵǸ� ���� �ı��ȴ�.
	CheckMinLimitUser();
}



















