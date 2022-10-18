#include "StdAfx.h"
#include ".\BaseZone.h"
#include "User.h"
#include <assert.h>

#include "FieldInfo.h"
#include "ZoneFactory.h"

////////////////////////////////////////////////////////////////////////////
/// BaseZone
////////////////////////////////////////////////////////////////////////////
BaseZone::BaseZone(void) : 
	m_MapCode(0)
   ,m_bBlockZone(FALSE)
{
#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
    room_key_ = 0;
#endif  // _NA_004694_20120412_1202_PARTY_ROOM_KEY
}

BaseZone::~BaseZone(void)
{
	m_LinkedZone.DeleteAll();
	m_bBlockZone = FALSE;
}

VOID BaseZone::Init()
{
	m_bBlockZone = FALSE;
}

VOID BaseZone::Release()
{
	m_mapUsers.clear();
	m_LinkedZone.DeleteAll();
	m_bBlockZone = FALSE;
}

VOID BaseZone::AddLinkedZone( BaseZone *pZone )
{
	m_LinkedZone.AddTail(pZone);
}

////////////////////////////////////////////////////////////////////////////
/// ChatZone
////////////////////////////////////////////////////////////////////////////
ChatZone::ChatZone()
{
}

ChatZone::~ChatZone()
{
}

BOOL ChatZone::AddUser( DWORD dwGuid, User *pUser, DWORD dwRoomKey )
{
	USER_MAP_ITER it = m_mapUsers.find( dwGuid );
	if( it != m_mapUsers.end() )
	{
		MessageOut( eCRITICAL_LOG, "ChatZone::AddUser[Guid:%u] Error - Already In ChatZone[ZT:%u][ZK:%u]", pUser->GetGUID(), m_zoneType, m_dwKey );
		m_mapUsers.erase( it );	// 기존 데이터가 있으면 지워준다
	}
	m_mapUsers.insert( USER_MAP_PAIR( dwGuid, pUser ) );

	switch( GetZoneType() )
	{
		case eZONETYPE_LOBBY:
		case eZONETYPE_CHARSELECT:
		case eZONETYPE_MISSION:
		case eZONETYPE_QUEST:
		case eZONETYPE_PVP:
		case eZONETYPE_EVENT:
		case eZONETYPE_GUILD:
		case eZONETYPE_SIEGE:
		case eZONETYPE_HUNTING:
		case eZONETYPE_CHUNTING:
		case eZONETYPE_INSTANCE:
		{
			break;
		}
		case eZONETYPE_MAX:
		{
			// 파티 등에 쓰이는 ChatZone은 여기 차지한다
			break;
		}
		default:
		{
#ifdef _DEBUG
			assert( 0 && "ChatZone::AddUser Error - Invalid Zone Type!!" );
#else
			MessageOut( eCRITICAL_LOG, "ChatZone AddUser Error - Invalid Zone Type[GUID:%u][C:%u][R:%u]", pUser->GetGUID(), pUser->GetChannelID(), pUser->GetZoneKey() );
#endif
			break;
		}
	}
	pUser->OnEnterZone( dynamic_cast<BaseZone*>(this) );
	pUser->SetNormalChatZone( this );
	MessageOut( eFULL_LOG, "ChatZone::AddUser[Type:%u][Key:%u][Guid:%u]", m_zoneType, m_dwKey, pUser->GetGUID() );

	return TRUE;
}

BOOL ChatZone::RemoveUser( DWORD dwGuid )
{
	USER_MAP_ITER it = m_mapUsers.find( dwGuid );
	//assert( it != m_mapUsers.end() );
	if( it != m_mapUsers.end() )
	{
		User *pUser = it->second;
		m_mapUsers.erase( it );
		if( !pUser )
		{
			MessageOut( eCRITICAL_LOG, "[Guid:%u]User Not In ChatZone[ZT:%u][ZK:%u]", dwGuid, m_zoneType, m_dwKey );
			return FALSE;
		}
		MessageOut( eFULL_LOG, "ChatZone::RemoveUser [%s][Guid:%u][ZT:%u][ZK:%u]", pUser->GetCharName().c_str(), pUser->GetGUID(), m_zoneType, m_dwKey );
		pUser->OnLeaveZone( this );
		return TRUE;
	}
	return FALSE;
}

BOOL ChatZone::Send( BYTE *pMsg, WORD wSize )
{
	USER_MAP_ITER	it;
	User			*pUser;
	for( it = m_mapUsers.begin(); it != m_mapUsers.end(); ++it )
	{
		pUser = it->second;
		if( !pUser || eZONETYPE_MAX == pUser->GetStatus() )
		{
            // 알수 없는 이유로 동기화가 어긋날경우 로그가 많이 출력되서 임시로 주석 처리
            // 크리티컬한 이슈가 아니라서 동기화 어긋나는 원인은 나중에 수정 예정
            //MessageOut( eCRITICAL_LOG, "ChatZone::Send : User Data Error - In ChatZone [%u]", m_dwKey );
			continue;
		}

		pUser->SendPacket( pMsg, wSize );
	}

	return TRUE;
}

VOID ChatZone::SendLinkedZone( BYTE *pMsg, WORD wSize )
{
	BaseZone *pZone = NULL;
	void* pNode = NULL;

	if(m_LinkedZone.GetCount() > 0)
	{
		pNode = m_LinkedZone.GetFirstPos();
		while( (pZone = m_LinkedZone.GetNextPos(pNode)) != NULL )
		{
			pZone->SendToMe(pMsg, wSize);
		}
	}
}

/*
// 헌팅존에서만 사용 - 해당 RoomKey로 외치기 연결된 맵 연결
VOID ChatZone::AddShoutLinkedZone( Channel *pChannel, DWORD dwRoomKey )	// 외치기용 링크 존 설정
{
	pChannel->AddLinkedZone( this
}
*/

////////////////////////////////////////////////////////////////////////////
/// ViewPortZone
////////////////////////////////////////////////////////////////////////////
ViewPortZone::ViewPortZone()
{
	m_pParentVillage = NULL;
}

ViewPortZone::~ViewPortZone()
{
	m_pParentVillage = NULL;
}

BOOL ViewPortZone::AddUser( DWORD dwGuid, User *pUser, DWORD dwRoomKey )
{
	USER_MAP_ITER it = m_mapUsers.find( dwGuid );
	if( it != m_mapUsers.end() )
	{
		MessageOut( eCRITICAL_LOG, "ViewPortZone::AddUser[Guid:%u] Error - Already In ViewPortZone[ZT:%u][ZK:%u]", pUser->GetGUID(), m_zoneType, m_dwKey );
		m_mapUsers.erase( it );	// 기존 데이터가 있으면 지워준다
	}
	m_mapUsers.insert( USER_MAP_PAIR( dwGuid, pUser ) );

	pUser->OnEnterSector( this );
	MessageOut( eFULL_LOG, "ViewPortZone::AddUser[Type:%u][Key:%u][Guid:%u]", m_zoneType, m_dwKey, pUser->GetGUID() );

//	pUser->SetFieldCode( m_dwKey );
	pUser->SetNormalChatZone( this );

	return TRUE;
}

BOOL ViewPortZone::RemoveUser( DWORD dwGuid )
{
	USER_MAP_ITER it = m_mapUsers.find( dwGuid );
	//assert( it != m_mapUsers.end() );
	if( it != m_mapUsers.end() )
	{
		User *pUser = it->second;
		m_mapUsers.erase( it );
		if( !pUser )
		{
			MessageOut( eCRITICAL_LOG, "[Guid:%u]User Not In ViewPortZone[ZT:%u][ZK:%u]", dwGuid, m_zoneType, m_dwKey );
			return FALSE;
		}
		MessageOut( eFULL_LOG, "ViewPortZone::RemoveUser [%s][Guid:%u][ZT:%u][ZK:%u]", pUser->GetCharName().c_str(), pUser->GetGUID(), m_zoneType, m_dwKey );
		pUser->OnLeaveSector( this );
		return TRUE;
	}
	return FALSE;
}

BOOL ViewPortZone::Send( BYTE *pMsg, WORD wSize )
{
	SendToMe( pMsg, wSize );

	// 주변 유저에게도 보낸다
	BaseZone *pZone = NULL;
	void* pNode = NULL;

	if(m_LinkedZone.GetCount() > 0)
	{
		pNode = m_LinkedZone.GetFirstPos();
		while( (pZone = m_LinkedZone.GetNextPos(pNode)) != NULL )
		{
			pZone->SendToMe(pMsg, wSize);
		}
	}

	return TRUE;
}

VOID ViewPortZone::SendToMe( BYTE *pMsg, WORD wSize )
{
	USER_MAP_ITER	it;
	User			*pUser;
	for( it = m_mapUsers.begin(); it != m_mapUsers.end(); ++it )
	{
		pUser = it->second;
		if( !pUser )
		{
			MessageOut( eCRITICAL_LOG, "ViewPortZone::SendToMe : User Data Error - In ViewPortZone [%u]", m_dwKey );
			continue;
		}
		pUser->SendPacket( pMsg, wSize );
	}
}

VOID ViewPortZone::SendToAll( BYTE *pMsg, WORD wSize )
{
	// 자신의 상위 클래스에서 보낸다
	m_pParentVillage->SendToAll( pMsg, wSize );
}

VillageZone::VillageZone()
{
	m_MapCode = 0;
	m_pFieldInfo = NULL;
}

VillageZone::~VillageZone()
{
    m_mapZone.clear();
    m_mapBattle.clear();
	m_MapCode = 0;
	m_pFieldInfo = NULL;
}

VOID VillageZone::Init( MAPCODE MapCode, FieldInfo *pFieldInfo )
{
	m_MapCode = MapCode;
	
	// 필드 데이터를 갖고 초기화
	m_pFieldInfo = pFieldInfo;

	// 이 필드에 들어갈 뷰포트 전부 생성
	
	//DISPMSG("MapCode(%u) dwSectorIndex(%u)\n", m_MapCode, pFieldInfo->GetTotalSectorNum());
	//	(WAVERIX)(080605) reduce overhead
	ViewPortZoneFactory* const pViewPortZoneFactory = ViewPortZoneFactory::Instance();
	const DWORD ub_sectors = m_pFieldInfo->GetTotalSectorNum();

	for( DWORD dwSectorIndex = 0 ; dwSectorIndex < ub_sectors ; ++dwSectorIndex )
	{
		ViewPortZone* const pViewPort = pViewPortZoneFactory->AllocZone();
		pViewPort->SetZoneType( eZONETYPE_VILLAGE );
		pViewPort->SetKey(dwSectorIndex);
		pViewPort->SetParentVillage( this );
		pViewPort->SetMapCode( m_MapCode );
		m_mapZone.insert(VIEWPORT_MAP_PAIR(dwSectorIndex, pViewPort));
	}

	for(DWORD dwSectorIndex = 0 ; dwSectorIndex < ub_sectors ; ++dwSectorIndex )
	{
		// 이웃 존 설정
		SectorInfo* const pSector = pFieldInfo->GetSectorInfo(dwSectorIndex);
		util::CLinkedlist<SectorInfo *>& SectorList = pSector->GetNeighborList();

		if( SectorList.GetCount() > 0 )
		{
			ViewPortZone* const pViewPort = m_mapZone.find( dwSectorIndex )->second;
			POS pNode = SectorList.GetFirstPos();
			SectorInfo* pNextSector = NULL;

			while( ( pNextSector = SectorList.GetNextPos( pNode ) ) != NULL )
			{
				VIEWPORT_MAP_ITER iter = m_mapZone.find(pNextSector->GetSectorIndex());
				if(iter == m_mapZone.end())
				{
					//심각한 에러
					MessageOut( eCRITICAL_LOG, "NeighborZone 세팅 에러!!!" );
					return;
				}
				ViewPortZone* const pNeighbor = (*iter).second;
				pViewPort->AddLinkedZone( pNeighbor );
			}
		}
	}
    CONSOLELOG(eCRITICAL_LOG, "    ChatField[Code:%u][SectorNum:%u] Init Complete\n",
               pFieldInfo->GetFieldCode(), pFieldInfo->GetTotalSectorNum());
}


VOID VillageZone::Release()
{
	// 생성한 뷰포트 모두 삭제

#if WAVERIX_CODE_RENEWAL
    if (m_mapZone.empty()) {
        return;
    };
	ViewPortZoneFactory* const pViewPortZoneFactory = ViewPortZoneFactory::Instance();
	FOREACH_CONTAINER( VIEWPORT_MAP::value_type& rNode, m_mapZone, VIEWPORT_MAP )
	{
		ViewPortZone* const pViewPort = rNode.second;
		pViewPort->Release();
		pViewPortZoneFactory->FreeZone( pViewPort );
	}
    m_mapZone.clear();
#else //WAVERIX_CODE_BACKUP
	VIEWPORT_MAP_ITER	iter;
	ViewPortZone		*pViewPort=NULL;

	while( !m_mapZone.empty() )
	{
		iter = m_mapZone.begin();
		pViewPort = iter->second;
		pViewPort->Release();
		ViewPortZoneFactory::Instance()->FreeZone(pViewPort);
		m_mapZone.erase(iter);
	}
#endif

//	if(m_pFieldInfo)
//	{
//		m_pFieldInfo->Destroy();
//	}
}

BOOL VillageZone::AddUser( DWORD dwGuid, User *pUser, DWORD dwRoomKey )
{
	// SectorCode가 있느냐 없느냐를 계산해서, ENTER_ZONE인지 CHATSTATE인지 구분
	if( pUser->GetSectorIndex() > -1 )
	{
		// Sector에 직접 넣는다
		VIEWPORT_MAP_ITER	iter = m_mapZone.find( pUser->GetSectorIndex() );

		if( iter == m_mapZone.end() )
		{
			MessageOut( eCRITICAL_LOG, "VillageZone::AddUser[Guid:%u] Error - No Sector In VillageZone[ZT:%u][ZK:%u][UZK:%u][ST:%u]", 
																													pUser->GetGUID(), 
																													m_zoneType, 
																													m_dwKey, 
																													pUser->GetZoneKey(),
																													pUser->GetSectorIndex() 
																												);
			return FALSE;
		}

		ViewPortZone* pViewPort = iter->second;
		pViewPort->AddUser( dwGuid, pUser, dwRoomKey );

		pUser->SetNormalChatZone( pViewPort );	// NormalChatZone 갱신
		MessageOut( eFULL_LOG, "VillageZone::AddUser In Sector[Type:%u][Key:%u][ST:%u][Guid:%u]", m_zoneType, m_dwKey, pUser->GetSectorIndex(), pUser->GetGUID() );
	}
	else
	{
		// 일단 이 Zone에만 넣는다
		USER_MAP_ITER it = m_mapUsers.find( dwGuid );
		if( it != m_mapUsers.end() )
		{
			MessageOut( eCRITICAL_LOG, "VillageZone::AddUser[Guid:%u] Error - Already In VillageZone[ZT:%u][ZK:%u]", pUser->GetGUID(), m_zoneType, m_dwKey );
			m_mapUsers.erase( it );	// 기존 데이터가 있으면 지워준다
		}
		m_mapUsers.insert( USER_MAP_PAIR( dwGuid, pUser ) );

		pUser->OnEnterZone( dynamic_cast<BaseZone*>(this) );
		pUser->SetNormalChatZone( this );
		MessageOut( eFULL_LOG, "VillageZone::AddUser[Type:%u][Key:%u][Guid:%u]", m_zoneType, m_dwKey, pUser->GetGUID() );
	}

	return TRUE;
}

BOOL VillageZone::RemoveUser( DWORD dwGuid )
{
	// 빼라 빼~ VillageZone의 userMap과 ViewPortZone의 userMap에서 다 빼준다
	USER_MAP_ITER it = m_mapUsers.find( dwGuid );
	//assert( it != m_mapUsers.end() );
	if( it != m_mapUsers.end() )
	{
		User *pUser = it->second;
		m_mapUsers.erase( it );
		if( !pUser )
		{
			MessageOut( eCRITICAL_LOG, "[Guid:%u]User Not In ChatZone[ZT:%u][ZK:%u]", dwGuid, m_zoneType, m_dwKey );
			return FALSE;
		}
		if( pUser->GetSectorIndex() != -1 )
		{
			VIEWPORT_MAP_ITER iterView = m_mapZone.find( pUser->GetSectorIndex() );

			if( iterView != m_mapZone.end() )
			{
				ViewPortZone *pViewPort = iterView->second;

				pViewPort->RemoveUser( dwGuid );
			}
		}
		MessageOut( eFULL_LOG, "VillageZone::RemoveUser [%s][Guid:%u][ZT:%u][ZK:%u]", pUser->GetCharName().c_str(), pUser->GetGUID(), m_zoneType, m_dwKey );
		pUser->OnLeaveZone( this );
		return TRUE;
	}
	return FALSE;
}

VOID VillageZone::SendToMe( BYTE *pMsg, WORD wSize )
{
	ChatZone::Send( pMsg, wSize );
/*
	USER_MAP_ITER	it;
	User			*pUser;
	for( it = m_mapUsers.begin(); it != m_mapUsers.end(); ++it )
	{
		pUser = it->second;
		if( !pUser )
		{
			MessageOut( eCRITICAL_LOG, "VillageZone::SendToMe : User Data Error - In ChatZone [%u]", m_dwKey );
			continue;
		}
		pUser->SendPacket( pMsg, wSize );
	}
*/
}

DWORD VillageZone::GetFieldCode()
{
	if( !m_pFieldInfo )
	{
		return 0;
	}

	return m_pFieldInfo->GetFieldCode();
}

BOOL VillageZone::Send( BYTE *pMsg, WORD wSize )
{
	// 여기에 들어왔다는 얘기는 VIEWPORT_CHARSTATE 동기화에 실패했다는 것이므로, 정보 전송을 요청한다
//	ChatZone::Send( pMsg, wSize );
	return FALSE;
}

VOID VillageZone::AddBattleLinkedZone( ChatZone *pZone )
{
	m_mapBattle.insert( BATTLE_MAP_PAIR( pZone->GetKey(), pZone ) );
}

VOID VillageZone::RemoveBattleLinkedZone( ChatZone *pZone )
{
#ifdef _DEBUG
	assert( pZone && "RemoveBattleLinkedZone Error - No Zone!!" );
#else
	if( !pZone )
	{
		MessageOut( eCRITICAL_LOG, "RemoveBattleLinkedZone Error - No Zone!!" );
		return;
	}
#endif

	BATTLE_MAP_ITER	iter = m_mapBattle.find( pZone->GetKey() );

	if( iter == m_mapBattle.end() )
	{
		MessageOut( eCRITICAL_LOG, "RemoveBattleLinkedZone Error - No Zone[ZK:%u]", pZone->GetKey() );
		return;
	}

	m_mapBattle.erase( iter );

//	MessageOut( eFULL_LOG, "BattleZone Link Remove.[V:%u][ZK:%u]", GetFieldCode(), pZone->GetKey() );
}

// 배틀존 연결도 같이 처리한다
VOID VillageZone::SendLinkedZone( BYTE *pMsg, WORD wSize )
{
	ChatZone::SendLinkedZone( pMsg, wSize );

	if( !m_mapBattle.empty() )
	{
		BATTLE_MAP_ITER	iter;
		for( iter = m_mapBattle.begin(); iter != m_mapBattle.end(); iter++ )
		{
			(*iter).second->SendToMe( pMsg, wSize );
		}
	}
}


///////////////////////////////////////////
// 커뮤니티용
CommChatZone::CommChatZone()
{
	memset( m_ptszChatRoomName, 0, sizeof(TCHAR) * MAX_ROOMTITLE_LENGTH+1 );
}

CommChatZone::~CommChatZone()
{
}

VOID CommChatZone::Release()
{
	// 모든 유저 파티 제거
	USER_MAP_ITER	it;
	User			*pUser;
	for( it = m_mapUsers.begin(); it != m_mapUsers.end(); ++it )
	{
		pUser = it->second;
		if( !pUser )
		{
			MessageOut( eCRITICAL_LOG, "ChatZone::Send : User Data Error - In ChatZone [%u]", m_dwKey );
			continue;
		}

		pUser->RemoveParty();
	}
	m_mapUsers.clear();
}

BOOL CommChatZone::AddUser( DWORD dwGuid, User *pUser, DWORD dwRoomKey )
{
	USER_MAP_ITER it = m_mapUsers.find( dwGuid );
	if( it != m_mapUsers.end() )
	{
		MessageOut( eCRITICAL_LOG, "CommChatZone::AddUser[Guid:%u] Error - Already In ChatZone[ZT:%u][ZK:%u]", pUser->GetGUID(), m_zoneType, m_dwKey );
		m_mapUsers.erase( it );	// 기존 데이터가 있으면 지워준다
	}
	m_mapUsers.insert( USER_MAP_PAIR( dwGuid, pUser ) );

	MessageOut( eFULL_LOG, "CommChatZone::AddUser[Type:%u][Key:%u][Guid:%u]", m_zoneType, m_dwKey, pUser->GetGUID() );

	return TRUE;
}

BOOL CommChatZone::RemoveUser( DWORD dwGuid )
{
	USER_MAP_ITER it = m_mapUsers.find( dwGuid );
	//assert( it != m_mapUsers.end() );
	if( it != m_mapUsers.end() )
	{
		User *pUser = it->second;
		m_mapUsers.erase( it );
		if( !pUser )
		{
			MessageOut( eCRITICAL_LOG, "[Guid:%u]User Not In CommChatZone[ZK:%u]", dwGuid, m_dwKey );
			return FALSE;
		}
		MessageOut( eFULL_LOG, "CommChatZone::RemoveUser [%s][Guid:%u][ZK:%u]", pUser->GetCharName().c_str(), pUser->GetGUID(), m_dwKey );
		return TRUE;
	}
	return FALSE;
}

BYTE CommChatZone::GetPartyVoiceChatInfo( BYTE *pStatus, User *pTarget )
{
	STRUCT_VOICE_STATUS* pPartyStatus = (STRUCT_VOICE_STATUS*)pStatus;
	USER_MAP_ITER	iter;
	User			*pUser = NULL;
	BYTE			byCnt=0;

	for( iter = m_mapUsers.begin(); iter != m_mapUsers.end(); ++iter )
	{
		pUser = (*iter).second;

		// 자기 정보는 보낼 필요 없다
		if( pUser == pTarget )
			continue;

		if( byCnt > 8 )
		{
			break;
		}

		_tcsncpy(pPartyStatus[byCnt].m_ptszCharName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
		pPartyStatus[byCnt].m_bVoiceOn = pUser->IsVoiceChatOn();
		byCnt++;
	}

	return byCnt;
}

// 채팅방 이름 설정
VOID CommChatZone::SetChatRoomName( TCHAR *ptszName )
{
	memset( m_ptszChatRoomName, 0, sizeof(TCHAR) * (MAX_ROOMTITLE_LENGTH+1) );
	_tcsncpy( m_ptszChatRoomName, ptszName, MAX_ROOMTITLE_LENGTH );
}

// 채팅방 접속중인 유저정보 얻음
BOOL CommChatZone::GetFirstUserName( TCHAR *ptszName )
{
	if( m_mapUsers.empty() )
		return FALSE;

	m_iterUserName = m_mapUsers.begin();
	_tcsncpy(ptszName, (*m_iterUserName).second->GetCharName().c_str(), MAX_CHARNAME_LENGTH);

	return TRUE;
}

BOOL CommChatZone::GetNextUserName( TCHAR *ptszName )
{
	if( m_iterUserName == m_mapUsers.end() )
		return FALSE;

	m_iterUserName++;
	if( m_iterUserName == m_mapUsers.end() )
		return FALSE;

	_tcsncpy(ptszName, (*m_iterUserName).second->GetCharName().c_str(), MAX_CHARNAME_LENGTH);

	return TRUE;
}
