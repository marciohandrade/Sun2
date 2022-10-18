#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include <PacketStruct_AG.h>
#include <PacketStruct_AW.h>
#include <PacketStruct_CG.h>
#include <RequireFieldLevelInfoParser.h>

//////////////////////////////////////////////////////////////////////////
#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/ServerSessionEx.h>
#include <UserSessions/User.h>
#include <Party/PartyManager.h>
#include <Party/Party.h>

#include "HuntingRoom.h"
#include "LobbyRoom.h"


_IMPL_POOL(HuntingRoom)
HuntingRoom::HuntingRoom(void)
{
}

HuntingRoom::~HuntingRoom(void)
{
}
RC::eROOM_RESULT	HuntingRoom::CanCreate( User * pUser, MAPCODE MapCode, eZONETYPE eZoneType, eZONEPUBLIC ePublic, BASE_ROOMINFO & IN BaseRoomInfo, ADDITIONAL_ROOMINFO & IN AdditionalRoomInfo )
{
	RC::eROOM_RESULT rt = RoomInterface::CanCreate( pUser, MapCode, eZoneType, ePublic, BaseRoomInfo );
	if( rt != RC::RC_ROOM_SUCCESS ) return rt;

	// 정보값 유효한가 체크후 셋팅
	if( AdditionalRoomInfo.m_Bonus >= eHUNTING_BONUS_TYPE_MAX ) 
		return RC::RC_ROOM_INVALID_BONUS_VALUE;
	if( AdditionalRoomInfo.m_Difficulty >= eHUNTING_MONSTER_DIFFICULT_MAX ) 
		return RC::RC_ROOM_INVALID_DIFFICULTY_VALUE;

	return RC::RC_ROOM_SUCCESS;
}
RC::eROOM_RESULT	HuntingRoom::Create( User * pUser, 
										KEYTYPE key,
										MAPCODE MapCode, 
										eZONEPUBLIC ePublic, 
										TCHAR * pszRoomTitle, 
										TCHAR * pszRoomPWD, 
										BASE_ROOMINFO & IN BaseRoomInfo,
										ADDITIONAL_ROOMINFO & IN AdditionalRoomInfo,
										ServerSession * pServerSession )
{
	// 제대로 된 미션, 헌팅, pVP맵 코드가 맞는지 체크

	RC::eROOM_RESULT rt = CanCreate( pUser, MapCode, eZONETYPE_HUNTING, ePublic, BaseRoomInfo, AdditionalRoomInfo );
	// 유저 상태 검사
	if( rt != RC::RC_ROOM_SUCCESS ) return rt;
	// 인자 유효성 검사


	// 정보 복사 -----------------------------------------------------
	RoomInterface::Create( pUser, 
							key,
							MapCode,
							ePublic,
							pszRoomTitle,
							pszRoomPWD,
							BaseRoomInfo,
							pServerSession );
	SetAdditionalInfo( &AdditionalRoomInfo );

	// 유저
	eROOMSTATE_TYPE type =  GetRoomState();
	SetRoomState( eROOMSTATE_GENERNAL );
	rt = CanJoin( pUser, GetZonePublic(), GetRoomPWD() );
	SetRoomState( type );
	if( rt != RC::RC_ROOM_SUCCESS ) return rt;

	On_create();

	return rt;

}

RC::eROOM_RESULT HuntingRoom::Migrate( KEYTYPE key, RoomInterface * pPreviousRoom, ServerSession * pServerSession )
{
	if( pPreviousRoom->GetType() != eZONETYPE_HUNTING && pPreviousRoom->GetType() != eZONETYPE_LOBBY )
		return RC::RC_ROOM_INVALIDROOMTYPE;

	RoomInterface::Migrate( key, pPreviousRoom, pServerSession );

	if( pPreviousRoom->GetType() == eZONETYPE_LOBBY )
	{
		LobbyRoom * pRoom = (LobbyRoom *)pPreviousRoom;
		SetAdditionalInfo( pRoom->GetAdditionalInfo() );
	}
	else if( pPreviousRoom->GetType() == eZONETYPE_HUNTING )
	{
		HuntingRoom * pRoom = (HuntingRoom *)pPreviousRoom;
		SetAdditionalInfo( pRoom->GetAdditionalInfo() );
	}

	On_create();

	// 릴레이 유저 등록
	RelayUser( pPreviousRoom );

	//pPreviousRoom->SetReusedKey(TRUE);

	return RC::RC_ROOM_SUCCESS;
}

VOID HuntingRoom::On_create()
{
	RoomInterface::On_create();

	MSG_AG_ZONE_HUNTING_CREATE_CMD msg;
	msg.m_dwKey = GetMaster()->GetUserKey();
	msg.m_MapCode = GetMapCode();
	msg.m_RoomKey = GetKey();
	memcpy( msg.m_pszRoomTitle, GetRoomTitle(), MAX_ROOMTITLE_LENGTH*sizeof(TCHAR) );
	memcpy( msg.m_pszRoomPWD, GetRoomPWD(), MAX_ROOMPASSWORD_LENGTH*sizeof(TCHAR) );
	msg.m_RoomInfo = *GetRoomInfo();									//< 생성할 방 정보
	msg.m_AdditionalInfo = *GetAdditionalInfo();
	SendToLinkedServer( &msg, sizeof(msg) );
}

VOID HuntingRoom::On_destroy()
{
	if( !IsReusedServerSession() )
		ServerSessionManager::Instance()->Decr_LOF_BattleServer( GetLinkedServer()->GetSessionIndex() );

	RoomInterface::On_destroy();
}

VOID HuntingRoom::On_readyuser( User * pUser )
{
	RoomInterface::On_readyuser(pUser);
	pUser->SetZoneState( ePRS_BEFORE_ENTER_HUNTING );
	pUser->SetZoneKey( GetKey() );
	pUser->SetZoneType( eZONETYPE_HUNTING );
}

VOID HuntingRoom::On_enteruser( User * pUser )
{
	RoomInterface::On_enteruser(pUser);
	pUser->SetZoneState( ePRS_AT_HUNTING );
}

VOID HuntingRoom::On_leaveuser( User * pUser )
{
	RoomInterface::On_leaveuser(pUser);
	
}

VOID HuntingRoom::SerializeLoad( HUNTING_ROOMINFO & OUT rRoomInfo )
{
	rRoomInfo.m_Key					= GetKey();
	rRoomInfo.m_MapCode				= GetMapCode();
	memcpy( rRoomInfo.m_pszRoomTitle, GetRoomTitle(), MAX_ROOMTITLE_LENGTH*sizeof(TCHAR) );
	rRoomInfo.m_byRoomPublic		= GetZonePublic();
	rRoomInfo.m_CurUserNum			= (BYTE)GetUserNum();
	rRoomInfo.m_MaxLimitUserNum		= GetLimitMaxUserNum();
	rRoomInfo.m_RoomInfo			= *GetRoomInfo();
	rRoomInfo.m_AdditionalInfo		= *GetAdditionalInfo();
}

BOOL HuntingRoom::RecommendLevel( LEVELTYPE LV )
{
	BASE_RequireFieldLevelInfo * pFieldLVInfo = RequireFieldLevelInfoParser::Instance()->GetRequireFieldLevelInfo( GetMapCode(), GetAdditionalInfo()->m_Difficulty );
	if( !pFieldLVInfo ) return FALSE;

	if( pFieldLVInfo->m_wMinlv <= LV && LV <= pFieldLVInfo->m_wMaxlv )
		return TRUE;

	return FALSE;
}

VOID HuntingRoom::Invite( User *pUser )
{
	MSG_CG_ZONE_HUNTING_JOIN_CMD JoinCmd;

	SerializeLoad( JoinCmd.m_RoomInfo );
	
	if( GetZonePublic() == eZONEPUBLIC_PRIVATE )
		memcpy( JoinCmd.m_pszRoomPWD, GetRoomPWD(), MAX_ROOMPASSWORD_LENGTH );

	pUser->SendPacket( &JoinCmd, JoinCmd.GetSize() );
}

RC::eROOM_RESULT HuntingRoom::_CanJoinEach( User* pUser, eZONEPUBLIC RoomPublic, TCHAR * pszRoomPWD )
{
	// 카오상태 체크
	if(!GetServerOptChaoStateUseZone( GetType()) && pUser->IsChaoState())
	{
		return RC::RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE;
	}

	return RC::RC_ROOM_SUCCESS;
}













