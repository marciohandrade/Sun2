#include "StdAfx.h"
#include ".\channel.h"
#include "ZoneFactory.h"
#include "BaseZone.h"
#include "User.h"
#include "UserManager.h"

// �� �ε� ���� �߰�
#include <mapinfoparser.h>
#include "FieldInfo.h"
//#include <MapDefine.h>

// ���� ���� ���� ó����
#include "ChannelManager.h"

// ��Ƽ ���� ���� �߰�
//#include "Party.h"
#include "PartyManager.h"

// ���� ��ũ ����
#include <LinkedList.h>

Channel::Channel(void)
{
//	m_pCharSceneZone = new ChatZone;

	for( USHORT i=eZONETYPE_LOBBY; i<eZONETYPE_MAX; i++ )
	{
		m_dwZoneUserNum[i] = 0;
		m_dwZonePCRoomUserNum[i] = 0;
		m_dwZoneNum[i] = 0;
	}
	m_ChatRoomKeyGenerator.Create( 1, 10000 );	// �ϴ� 10000�� ����...ä�ù��� ������ ���� �� �ֱ� ������ �� �̻��� ���� �ִ�

	m_pPartyManager = new PartyManager;
}

Channel::~Channel(void)
{
//	delete m_pCharSceneZone;

	delete m_pPartyManager;
}

VOID Channel::Init()
{
	m_dwNormalUserNum = 0;
	m_dwPCRoomUserNum = 0;

	LoadMap();
}

VOID Channel::Release()
{
	USER_MAP_ITER	it;
	User			*pUser;

	for( it = m_mapUsers.begin(); it != m_mapUsers.end(); ++it )
	{
		pUser = it->second;
		RemoveUser( pUser, FALSE );
		pUser->Logout();
		m_mapUsers.erase( it++ );
		if( it == m_mapUsers.end() )
			break;
	}
	m_mapUsers.clear();

	// ĳ���� ������ ������
//	m_pCharSceneZone->Release();

	ZONE_MAP_ITER	zone_it;
	ChatZone		*pZone;

	for( zone_it = m_mapZones.begin(); zone_it != m_mapZones.end(); ++zone_it )
	{
		pZone = zone_it->second;
		pZone->Release();
	}
	m_mapZones.clear();

	ClearZoneUserNum();
	ClearZoneNum();

	for( zone_it = m_mapFriendChat.begin(); zone_it != m_mapFriendChat.end(); ++zone_it )
	{
		pZone = zone_it->second;
		pZone->Release();
	}
	m_mapFriendChat.clear();

	// ������ �ʱ�ȭ
	for( USHORT i=eZONETYPE_LOBBY; i< eZONETYPE_MAX; i++ )
	{
		m_dwZoneUserNum[i] = 0;
		m_dwZonePCRoomUserNum[i] = 0;
		m_dwZoneNum[i] = 0;
	}

	m_dwNormalUserNum = 0;
	m_dwPCRoomUserNum = 0;
}

VOID Channel::Update()
{
}

BOOL Channel::LoadMap()
{
    // NOTE: f110520.5L, f081029.x, make static field unique index
    // like villages, fields and domination fields.
    // the '1' key index reserved here to support the character selectable zone.
    const KEYTYPE kConstStaticZoneStartKey = 2;
    const MapInfoParser* const map_parser = MapInfoParser::Instance();
    const sMAPINFO_HASH& map_info_table = map_parser->GetMapInfoHash();

    m_mapVillages.clear();
    ChannelManager* const channel_manager = ChannelManager::Instance();
    VillageZoneFactory* const zone_factory = VillageZoneFactory::Instance();
    FOREACH_CONTAINER(const sMAPINFO_HASH::value_type& node, map_info_table, sMAPINFO_HASH)
    {
        const sMAPINFO* const map_info = node.second;
        const eZONETYPE zone_type = map_info->GetZoneType4SharedCode();

        bool is_static_zone = zone_type == eZONETYPE_VILLAGE ||
                              zone_type == eZONETYPE_FIELD ||
                              zone_type == eZONETYPE_DOMINATION_FIELD ||
                              zone_type == eZONETYPE_SPA_FIELD; //_NA_008069_20150210_ADD_SPA_SYSTEM
        if (is_static_zone == false) {
            continue;
        };
        // if the zone has sub-fields, the ranges are incremented from start_zone_key.
        ASSERT(map_info->reserved_key_index_ != 0);
        KEYTYPE start_zone_key = map_info->reserved_key_index_;
        CONSOLELOG(eCRITICAL_LOG, _T("Adding Map='%d'"), map_info->MapCode);
        // NOTE: f110520.5L, the FCode index is sequential since 2011.05.03, f110503.6L.
        const MAPCODE* field_it = map_info->FCode,
                     * const field_end = &map_info->FCode[1];//&map_info->FCode[map_info->byFCount];
        for (; field_it != field_end; ++field_it, ++start_zone_key)
        {
            const FIELDCODE field_code = (*field_it);
            if (field_code == 0) { // assert(field_code != 0)
                break;
            };
            //
            FieldInfo* field_info = channel_manager->GetFieldInfo(field_code);
            assert(field_info != NULL);
            //
            VillageZone* const zone = zone_factory->AllocZone();
            zone->Init(map_info->MapCode, field_info);
            //
            ;;;; if (zone_type == eZONETYPE_VILLAGE) {
                this->AddVillage(start_zone_key, zone);
            }
            else if (zone_type == eZONETYPE_FIELD) {
                this->AddField(start_zone_key, zone);
            }
            else if (zone_type == eZONETYPE_DOMINATION_FIELD) {
                this->AddDominationField(start_zone_key, zone);
            }
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
            else if (zone_type == eZONETYPE_SPA_FIELD) {
                this->AddSpaField(start_zone_key, zone);
            }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
            else {
                ASSERT(!"unexpected zone type");
                return false;
            };
            //
            assert(map_info->MapCode == zone->GetMapCode());
            m_mapVillages.insert(VILLAGE_MAP::value_type(map_info->MapCode, zone));
            CONSOLELOG(eCRITICAL_LOG, _T("    Added FieldInfo='%d'"), field_info->GetFieldCode());
        }
    };
    //----------------------------------------------------------------------------------------------
    // add linked zones
    ;{
        VILLAGE_MAP::const_iterator zone_it = m_mapVillages.begin(),
                                    zone_end = m_mapVillages.end();
        for (; zone_it != zone_end; ++zone_it)
        {
            DWORD map_code = zone_it->first;
            ChatZone* link_zone = zone_it->second;
            this->AddShoutLinkedZone(map_code, link_zone);
        }
    };
    return true;
    //
#if SUN_CODE_BACKUP
	MapInfoParser *pParser = MapInfoParser::Instance();

	// ���� ���� ��츸 ������ �д´�
	sMAPINFO	*pMapInfo = NULL;
	FieldInfo	*pFieldInfo = NULL;

	sMAPINFO_HASH&		HashMap = pParser->GetMapInfoHash();
	if( HashMap.size() == 0 )
	{
		MessageOut( eCRITICAL_LOG, "MapInfoParser Error - No Data" );
		return FALSE;
	}
    sMAPINFO_HASH::const_iterator iter;

	m_mapVillages.clear();

	DWORD dwZoneKey = 2;	// 2����(1�� Charselect) ������� ���� - Agent������ �� �д� ������ �����ϹǷ� ������� �����ϸ� ������ Key�� ���´�
	for( iter = HashMap.begin(); iter != HashMap.end(); iter++ )
	{
		pMapInfo = iter->second;

		eZONETYPE ztZONE = pMapInfo->GetZoneType4SharedCode();

		if( ztZONE == eZONETYPE_VILLAGE )
		{
			// �� �о Village �߰�
//			MapInfo *pVillageInfo = new MapInfo;

//			pVillageInfo->SetMapInfo(pMapInfo);

			for(int i=0;i<sMAPINFO::MAX_FIELD_NUM;++i)
			{
				if( 0 != pMapInfo->FCode[i])
				{
//					pFieldInfo = new FieldInfo;
//					pFieldInfo->Create( MapInfoParser::Instance()->GetFieldInfo( pMapInfo->FCode[i] ) );
//					pFieldInfo->Load();
//
//					pFieldInfo->EstablishSectorInfo( VILLAGE_SECTOR_SIZE, TRUE );
					pFieldInfo = ChannelManager::Instance()->GetFieldInfo( pMapInfo->FCode[i] );

					if( !pFieldInfo )
					{
						MessageOut( eCRITICAL_LOG, "FieldInfo Setting Error[%u]", pMapInfo->FCode[i] );
						return FALSE;
					}

					VillageZone *pVillageZone = VillageZoneFactory::Instance()->AllocZone();
					pVillageZone->Init( pMapInfo->MapCode, pFieldInfo );

					AddVillage( dwZoneKey, pVillageZone );

					// �ڿ� ���� ��ũ �۾��� �ϱ� ���� �ʿ� �ִ´�
					m_mapVillages.insert( std::pair< DWORD, VillageZone *>( pVillageZone->GetMapCode(), pVillageZone ) );

					DISPMSG( "FieldInfo[%u] Add Completed\n", pFieldInfo->GetFieldCode() );
					dwZoneKey++;
				}
			}
//			delete pVillageInfo;
		}
		if( ztZONE == eZONETYPE_FIELD )
		{
			// �� �о Field �߰�
//			MapInfo *pVillageInfo = new MapInfo;

//			pVillageInfo->SetMapInfo(pMapInfo);

			for(int i=0;i<sMAPINFO::MAX_FIELD_NUM;++i)
            {
				if( 0 != pMapInfo->FCode[i])
				{
//					pFieldInfo = new FieldInfo;
//					pFieldInfo->Create( MapInfoParser::Instance()->GetFieldInfo( pMapInfo->FCode[i] ) );
//					pFieldInfo->Load();
//
//					pFieldInfo->EstablishSectorInfo( VILLAGE_SECTOR_SIZE, TRUE );
					pFieldInfo = ChannelManager::Instance()->GetFieldInfo( pMapInfo->FCode[i] );

					if( !pFieldInfo )
					{
						MessageOut( eCRITICAL_LOG, "FieldInfo Setting Error[%u]", pMapInfo->FCode[i] );
						return FALSE;
					}

					VillageZone *pVillageZone = VillageZoneFactory::Instance()->AllocZone();
					pVillageZone->Init( pMapInfo->MapCode, pFieldInfo );

					AddField( dwZoneKey, pVillageZone );

					// �ڿ� ���� ��ũ �۾��� �ϱ� ���� �ʿ� �ִ´�
					m_mapVillages.insert( std::pair< DWORD, VillageZone *>( pVillageZone->GetMapCode(), pVillageZone ) );
					dwZoneKey++;

					DISPMSG( "FieldInfo[M:%u/F:%u] Add Completed\n", pVillageZone->GetMapCode(), pFieldInfo->GetFieldCode() );
				}
			}
//			delete pVillageInfo;
		}
        if( ztZONE == eZONETYPE_DOMINATION_FIELD )
        {
            for(int i=0;i<sMAPINFO::MAX_FIELD_NUM;++i)
            {
                if( 0 != pMapInfo->FCode[i])
                {
                    pFieldInfo = ChannelManager::Instance()->GetFieldInfo( pMapInfo->FCode[i] );

                    if( !pFieldInfo )
                    {
                        MessageOut( eCRITICAL_LOG, "FieldInfo Setting Error[%u]", pMapInfo->FCode[i] );
                        return FALSE;
                    }

                    VillageZone *pVillageZone = VillageZoneFactory::Instance()->AllocZone();
                    pVillageZone->Init( pMapInfo->MapCode, pFieldInfo );

                    AddDominationField( dwZoneKey, pVillageZone );

                    // �ڿ� ���� ��ũ �۾��� �ϱ� ���� �ʿ� �ִ´�
                    m_mapVillages.insert( std::pair< DWORD, VillageZone *>( pVillageZone->GetMapCode(), pVillageZone ) );
                    dwZoneKey++;

                    DISPMSG( "FieldInfo[M:%u/F:%u] Add Completed\n", pVillageZone->GetMapCode(), pFieldInfo->GetFieldCode() );
                }
            }
        }
	}

	// ��ũ ������ ����
	VILLAGE_MAP_ITER	iter_vill;

	for( iter_vill = m_mapVillages.begin(); iter_vill != m_mapVillages.end(); iter_vill++ )
	{
		AddShoutLinkedZone( (*iter_vill).first, (*iter_vill).second );
	}

	return TRUE;
#endif //SUN_CODE_BACKUP
}

BOOL Channel::AddShoutLinkedZone( DWORD dwMapCode, ChatZone *pLinkZone )
{
	if( !pLinkZone )
	{
		return FALSE;
	}

	VillageZone			*pLinkedZone=NULL;
	VILLAGE_MAP_ITER	iter;

	SERVER_LINKEDZONE_INFO	LinkedInfo;
	if( !ChannelManager::Instance()->FindLinkedZoneInfo( dwMapCode, LinkedInfo ) )
	{
		MessageOut( eFULL_LOG, "NO Linked Zone[%u]", dwMapCode );
		return FALSE;
	}
	else
	{
		// ��ũ�� ������ ����
		for( int i=0; i<7; i++ )
		{
			if( LinkedInfo.dwLinkedMapCode[i] )
			{
				iter = m_mapVillages.find( LinkedInfo.dwLinkedMapCode[i] );
				if( iter == m_mapVillages.end() )
				{
					MessageOut( eFULL_LOG, "NO Linked Zone[%u]", dwMapCode );
					continue;
				}
				pLinkedZone = (*iter).second;

				// �ڱ� �ڽ��� ���(���� �����ؾ� �Ѵ� - LinkedZone�� ��ĥ �� SendToMe �������� �̰͵� ���� ������ ��)
//				if( pLinkZone == (ChatZone*)pLinkedZone || !pLinkedZone )
//				{
//					continue;
//				}

				pLinkZone->AddLinkedZone( pLinkedZone );
//				MessageOut( eCRITICAL_LOG, "Linked Zone[%u->%u]", pLinkZone->GetKey(), pLinkedZone->GetKey() );
			}
		}
	}

	return TRUE;
}

BOOL Channel::AddBattleShoutLinkedZone( DWORD dwMapCode, ChatZone *pLinkZone )
{
	if( !pLinkZone )
	{
		return FALSE;
	}

	VillageZone			*pLinkedZone=NULL;
	VILLAGE_MAP_ITER	iter;

	SERVER_LINKEDZONE_INFO	LinkedInfo;
	if( !ChannelManager::Instance()->FindLinkedZoneInfo( dwMapCode, LinkedInfo ) )
	{
		MessageOut( eFULL_LOG, "NO Linked Zone[%u]", dwMapCode );
		return FALSE;
	}
	else
	{
		// ��ũ�� ������ ����
		for( int i=0; i<7; i++ )
		{
			if( LinkedInfo.dwLinkedMapCode[i] )
			{
				iter = m_mapVillages.find( LinkedInfo.dwLinkedMapCode[i] );
				if( iter == m_mapVillages.end() )
				{
					MessageOut( eFULL_LOG, "NO Linked Zone[%u]", dwMapCode );
					continue;
				}
				pLinkedZone = (*iter).second;

				// �ڱ� �ڽ��� ���(���� �����ؾ� �Ѵ� - LinkedZone�� ��ĥ �� SendToMe �������� �̰͵� ���� ������ ��)
				if( pLinkZone == (ChatZone*)pLinkedZone || !pLinkedZone )
				{
					continue;
				}

				pLinkZone->AddLinkedZone( pLinkedZone );
				// �����̳� �ʵ� �ƴϹǷ� ����ʿ����� ���� ��ũ�� ��� �ȴ�
				pLinkedZone->AddBattleLinkedZone( pLinkZone );
				MessageOut( eFULL_LOG, "BattleZone Link Add[V:%u][ZK:%u]", pLinkedZone->GetFieldCode(), pLinkZone->GetKey() );
//				MessageOut( eCRITICAL_LOG, "Linked Zone[%u->%u]", pLinkZone->GetKey(), pLinkedZone->GetKey() );
			}
		}
	}

	return TRUE;
}

VOID Channel::RemoveBattleShoutLinkedZone( ChatZone *pLinkZone )
{
	// ������ ���� ������� �Ѵ�
	util::CLinkedlist<BaseZone*> &LinkedList = pLinkZone->GetLinkedZoneList();
	VillageZone *pNextZone = NULL;
	void* pNode = NULL;

	if(LinkedList.GetCount() > 0)
	{
		pNode = LinkedList.GetFirstPos();
		while( (pNextZone = reinterpret_cast< VillageZone * >(LinkedList.GetNextPos((POS&)pNode))) != NULL )
		{
			pNextZone->RemoveBattleLinkedZone( pLinkZone );
			MessageOut( eFULL_LOG, "BattleZone Link Remove[V:%u][ZK:%u]", pNextZone->GetFieldCode(), pLinkZone->GetKey() );
		}
	}
}

ChatZone* Channel::AddChatZone( eZONETYPE eZoneType, DWORD dwZoneKey, DWORD dwMapCode )
{
	ChatZone *pZone = NULL;

	if( eZoneType == eZONETYPE_VILLAGE )
	{
		// ������ ���⼭ �߰����� �ʴ´�. ä�� �߰��� �� �ڵ����� �ؾ� �ϹǷ� �̽�
		MessageOut( eCRITICAL_LOG, "AddChatZone Error - Village Add[ZK:%u]", dwZoneKey );
		return NULL;
	}

	pZone = ChatZoneFactory::Instance()->AllocZone();

	pZone->SetZoneType( eZoneType );
	pZone->SetKey( dwZoneKey );
	pZone->SetMapCode( dwMapCode );
	m_mapZones.insert( ZONE_MAP_PAIR( dwZoneKey, pZone ) );

	AddBattleShoutLinkedZone( dwMapCode, pZone );	// ��ġ�� ��ũ ����

	IncZoneNum( eZoneType );

	return pZone;
}

VOID Channel::AddVillage( DWORD dwZoneKey, ChatZone *pVillage )
{
	pVillage->SetZoneType( eZONETYPE_VILLAGE );
	pVillage->SetKey( dwZoneKey );
	m_mapZones.insert( ZONE_MAP_PAIR( dwZoneKey, pVillage ) );
	IncZoneNum( eZONETYPE_VILLAGE );
}

VOID Channel::AddField( DWORD dwZoneKey, ChatZone *pField )
{
	pField->SetZoneType( eZONETYPE_FIELD );
	pField->SetKey( dwZoneKey );
	m_mapZones.insert( ZONE_MAP_PAIR( dwZoneKey, pField ) );
	IncZoneNum( eZONETYPE_FIELD );
}

VOID Channel::AddDominationField( DWORD dwZoneKey, ChatZone *pField )
{
    pField->SetZoneType( eZONETYPE_DOMINATION_FIELD );
    pField->SetKey( dwZoneKey );
    m_mapZones.insert( ZONE_MAP_PAIR( dwZoneKey, pField ) );
    IncZoneNum( eZONETYPE_DOMINATION_FIELD );
}

ChatZone* Channel::AddFriendChat( DWORD dwZoneKey )
{
	ChatZone *pZone = ChatZoneFactory::Instance()->AllocZone();
	pZone->SetZoneType( eZONETYPE_MAX );
	pZone->SetKey( dwZoneKey );
	m_mapFriendChat.insert( ZONE_MAP_PAIR( dwZoneKey, pZone ) );

	return pZone;
}

VOID Channel::RemoveChatZone( DWORD dwZoneKey )
{
	ZONE_MAP_ITER it = m_mapZones.find( dwZoneKey );

	if( it == m_mapZones.end() )
		return;

	ChatZone *pZone = it->second;
	eZONETYPE eZoneType = pZone->GetZoneType();

	// �����̶�� �����ϸ� �ȵȴ�
	switch( eZoneType )
	{
		case eZONETYPE_VILLAGE:
		case eZONETYPE_FIELD:
        case eZONETYPE_DOMINATION_FIELD:
		case eZONETYPE_CHARSELECT:
        case eZONETYPE_SPA_FIELD:   //_NA_008069_20150210_ADD_SPA_SYSTEM
			break;
		default:
		{
			m_mapZones.erase( it );

			// ��ġ�� ���� �����ش�
			RemoveBattleShoutLinkedZone( pZone );

			ChatZoneFactory::Instance()->FreeZone( pZone );

			DecZoneNum( eZoneType );

			break;
		}
	}
}

VOID Channel::RemoveFriendChat( DWORD dwZoneKey )
{
	ZONE_MAP_ITER it = m_mapFriendChat.find( dwZoneKey );

	if( it == m_mapFriendChat.end() )
		return;

    ChatZone *pZone = it->second;
	m_mapFriendChat.erase( it );

	ChatZoneFactory::Instance()->FreeZone( pZone );
}

ChatZone* Channel::FindZone( DWORD dwZoneKey )
{
	ZONE_MAP_ITER it = m_mapZones.find( dwZoneKey );
	if( it != m_mapZones.end() )
	{
		return it->second;
	}

	return NULL;
}

VillageZone* Channel::FindVillage( DWORD dwMapCode )
{
	VILLAGE_MAP_ITER it = m_mapVillages.find( dwMapCode );
	if( it != m_mapVillages.end() )
	{
		return it->second;
	}

	return NULL;
}

ChatZone* Channel::FindFriendChat( DWORD dwZoneKey )
{
	ZONE_MAP_ITER it = m_mapFriendChat.find( dwZoneKey );
	if( it != m_mapFriendChat.end() )
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

// ä�ο� ó������ ������ �߰��Ѵ�.
// ĳ���;��� �߰��� ���� ��Ű ����
VOID Channel::AddUser( User *pUser, eZONETYPE userStatus, DWORD dwZoneKey )
{
//	pUser->SetStatus( userStatus );

	if( !UserEnterZone( pUser, userStatus, dwZoneKey ) )
	{
		return;
	}

	// ä�� ���� ����Ʈ���� �߰�
	m_mapUsers.insert( USER_MAP_PAIR( pUser->GetGUID(), pUser ) );

	// ��ü ���� ����Ʈ���� �߰�
	UserManager::Instance()->AddUser( pUser->GetGUID(), pUser );

	// PC�� ���� �����Ͽ� ��ü ������ �߰�
	if( pUser->GetPCRoomID() )
	{
		IncNumberOfPCRoomUsers();
	}
	else
	{
		IncNumberOfNormalUsers();
	}

	MessageOut( eFULL_LOG,   "Channel(%d) Added User! [Guid:%u][CharName:%s][Status:%u]",
		GetChannelID(), pUser->GetGUID(), pUser->GetCharName().c_str(), userStatus );
}

// ä���� ������ �������� �� ȣ��
VOID Channel::RemoveUser( User *pUser, BOOL bIterErase )
{
	MessageOut( eFULL_LOG,   "Channel(%d) Removed User [Guid:%u][CharName:%s][Status:%u]",
		GetChannelID(), pUser->GetGUID(), pUser->GetCharName().c_str(), pUser->GetStatus() );

	WithdrawParty( pUser );

	if( pUser->GetStatus() >= eZONETYPE_LOBBY && pUser->GetStatus() < eZONETYPE_MAX )
	{
		// ä�ο��� ���� ��ġ�� ����Ʈ���� ����
		UserLeavePrevZone( pUser );
	}

	if( bIterErase )
	{
		// ä�� ���� ����Ʈ������ ����
		USER_MAP_ITER it = m_mapUsers.find( pUser->GetGUID() );
		if( it != m_mapUsers.end() )
		{
			m_mapUsers.erase( it );
		}
	}

	// ��ü ���� ����Ʈ������ ����
	UserManager::Instance()->RemoveUser( pUser->GetGUID() );

	// PC�� ���� �����Ͽ� ��ü �������� �谨
	if( pUser->GetPCRoomID() )
	{
		DecNumberOfPCRoomUsers();
	}
	else
	{
		DecNumberOfNormalUsers();
	}
}

VOID Channel::UserLeavePrevZone( User *pUser )
{
	ChatZone *pZone = FindZone( pUser->GetZoneKey() );

	if( pZone )
	{
        if( pZone->RemoveUser( pUser->GetGUID() ) )
		{
//			DecZoneUserNum( pZone->GetZoneType(), pUser->GetPCRoomID() );
//			MessageOut( eMIDDLE_LOG, "DecZoneUserNum[Guid:%u][CharGuid:%u][ZT:%u][ZK:%u]", pUser->GetGUID(), 
//																								pUser->GetCharGuid(), 
//																								pZone->GetZoneType(), 
//																								pZone->GetKey() );
		}

		MessageOut( eFULL_LOG, "UserLeaveZone[Guid:%u][CharGuid:%u][ZT:%u][ZK:%u]", pUser->GetGUID(), 
																							pUser->GetCharGuid(), 
																							pZone->GetZoneType(), 
																							pZone->GetKey() );

		// �� ��Ʋ���̸� ����
		if( pZone->GetNumberOfUsers() == 0 )
		{
			RemoveChatZone( pZone->GetKey() );
		}
	}
}
/*
DWORD Channel::GetZoneUserNum( eZONETYPE eZoneType )
{
	ZONE_MAP_ITER	it;
	DWORD			dwCnt=0;

	for( it = m_mapZones.begin(); it != m_mapZones.end(); ++it )
	{
		if( it->second->GetZoneType() == eZoneType )
		{
			dwCnt += it->second->GetNumberOfUsers();
		}
	}

	return dwCnt;
}
*/
BOOL Channel::UserEnterZone( User *pUser, eZONETYPE eZoneType, DWORD dwZoneKey, DWORD dwMapCode )
{
#ifdef _DEBUG
	assert( eZoneType >= eZONETYPE_LOBBY && eZoneType < eZONETYPE_MAX && "UserEnterZone - invalid user status" );
#else
	if( eZoneType < eZONETYPE_LOBBY || eZoneType >= eZONETYPE_MAX )
	{
		MessageOut( eCRITICAL_LOG, "[Guid:%u] Channel::UserEnterZone Error - Invalid ZoneType[%u][ZK:%u]", pUser->GetGUID(), eZoneType, dwZoneKey );
		// �Ҿ�������� �������!!!
		pUser->Logout();
		return FALSE;
	}
#endif

	if( pUser->GetStatus() != eZONETYPE_MAX )
	{
		// ���� CRITICAL¥������ �αװ� �ʹ� ���� ���Ƽ� MIDDLE_LOG�� ����.-_-; ��ɿ� ũ�� ������ ��ġ�� ��Ȳ�� �ƴ� �� ������...
		MessageOut( eMIDDLE_LOG, "[Guid:%u][C:%u] UserEnterZone Error - Leave���� EnterZone[ZT:%u/%u][ZK:%u/%u]", pUser->GetGUID(), 
																											GetChannelID(), 
																											pUser->GetStatus(), 
																											eZoneType, 
																											pUser->GetZoneKey(), 
																											dwZoneKey );
		// ���� Zone���� ���� ���� �ٽ� �ִ´�
		UserLeavePrevZone( pUser );
	}

	if( pUser->GetZoneKey() == dwZoneKey )
	{
		MessageOut( eCRITICAL_LOG, "[Guid:%u] Channel:UserEnterZone Error - �ߺ�[ZT:%u][ZK:%u]", pUser->GetGUID(), eZoneType, dwZoneKey );
		return FALSE;
	}

	pUser->SetStatus( eZoneType );

	// �ش� Ű�� ������ ã�ƺ��� ������ ���� �߰� �� ���� �߰�
	ChatZone *pZone = FindZone( dwZoneKey );
	if( !pZone )
	{
		pZone = AddChatZone( eZoneType, dwZoneKey, dwMapCode );
		if( !pZone )
		{
			MessageOut( eCRITICAL_LOG, "[UserEnterZone] Can't AddChatZone! Zone is NULL!!" );
			return FALSE;
		}
	}

	if( pZone->AddUser( pUser->GetGUID(), pUser ) )
	{
//		IncZoneUserNum( eZoneType, pUser->GetPCRoomID() );
//		MessageOut( eMIDDLE_LOG, "IncZoneUserNum[Guid:%u][CharGuid:%u][ZT:%u][ZK:%u]", pUser->GetGUID(), pUser->GetCharGuid(), eZoneType, dwZoneKey );
	}

	MessageOut( eFULL_LOG, "UserEnterZone[Guid:%u][CharGuid:%u][ZT:%u][ZK:%u]", pUser->GetGUID(), pUser->GetCharGuid(), eZoneType, dwZoneKey );

	return TRUE;
}

VOID Channel::SendToAll( BYTE *pMsg, WORD wSize )
{
	USER_MAP_ITER	it;
	User			*pUser;
	for( it = m_mapUsers.begin(); it !=m_mapUsers.end(); ++it )
	{
		pUser = it->second;
		pUser->SendPacket( pMsg, wSize );
	}
}

VOID Channel::SendToVillages( BYTE *pMsg, WORD wSize )
{
	ZONE_MAP_ITER	it;
	ChatZone		*pZone;
	eZONETYPE		eVillageZoneType;

	for( it = m_mapZones.begin(); it != m_mapZones.end(); ++it )
	{
		pZone = it->second;
		eVillageZoneType = pZone->GetZoneType();
		if( eVillageZoneType == eZONETYPE_VILLAGE || 
            eVillageZoneType == eZONETYPE_DOMINATION_FIELD ||
            eVillageZoneType == eZONETYPE_FIELD ||
            eVillageZoneType == eZONETYPE_SPA_FIELD)    //_NA_008069_20150210_ADD_SPA_SYSTEM
		{
			pZone->SendToMe( pMsg, wSize );	// ������ �� �����ֹǷ� SendToAll���� ���� �ʿ� ����
		}
	}
}

VOID Channel::SendToBattleZones( BYTE *pMsg, WORD wSize )
{
	ZONE_MAP_ITER	it;
	ChatZone		*pZone;
	eZONETYPE		eBattleZoneType;

	for( it = m_mapZones.begin(); it != m_mapZones.end(); ++it )
	{
		pZone = it->second;
		eBattleZoneType = pZone->GetZoneType();
		if( eBattleZoneType != eZONETYPE_VILLAGE &&
            eBattleZoneType != eZONETYPE_CHARSELECT &&
            eBattleZoneType != eZONETYPE_DOMINATION_FIELD &&
            eBattleZoneType != eZONETYPE_FIELD &&
            eBattleZoneType != eZONETYPE_SPA_FIELD)     //_NA_008069_20150210_ADD_SPA_SYSTEM
		{
			pZone->SendToMe( pMsg, wSize );	// ������ �� �����ֹǷ� SendToAll���� ���� �ʿ� ����
		}
	}
}

VOID Channel::DisplayInfo()
{
	DWORD dwCharSelectUserNum = 0, dwVillageUserNum = 0, dwFieldUserNum = 0, dwBattleUserNum = 0;
	DWORD dwVillageNum = 0, dwFieldNum = 0, dwBattleNum = 0;
    DWORD dwDominationFieldUserNum = 0;
    DWORD dwDominationFieldNum = 0;

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    DWORD spafield_user_number=0;
    DWORD spafield_number=0;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

	for( USHORT i=eZONETYPE_LOBBY; i<eZONETYPE_MAX; i++ )
	{
		switch(i)
		{
			case eZONETYPE_CHARSELECT:
			{
				dwCharSelectUserNum += m_dwZoneUserNum[i];
				dwCharSelectUserNum += m_dwZonePCRoomUserNum[i];
				break;
			}
			case eZONETYPE_VILLAGE:
			{
				dwVillageUserNum += m_dwZoneUserNum[i];
				dwVillageUserNum += m_dwZonePCRoomUserNum[i];
				dwVillageNum += m_dwZoneNum[i];
				break;
			}
			case eZONETYPE_FIELD:
			{
				dwFieldUserNum += m_dwZoneUserNum[i];
				dwFieldUserNum += m_dwZonePCRoomUserNum[i];
				dwFieldNum += m_dwZoneNum[i];
				break;
			}
            case eZONETYPE_DOMINATION_FIELD:
                {
                    dwDominationFieldUserNum += m_dwZoneUserNum[i];
                    dwDominationFieldUserNum += m_dwZonePCRoomUserNum[i];
                    dwDominationFieldNum += m_dwZoneNum[i];
                    break;
                }
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
            case eZONETYPE_SPA_FIELD:
                {
                    spafield_user_number += m_dwZoneUserNum[i];
                    spafield_user_number += m_dwZonePCRoomUserNum[i];
                    spafield_number += m_dwZoneNum[i];
                    break;
                }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
			default:
			{
				dwBattleUserNum += m_dwZoneUserNum[i];
				dwBattleUserNum += m_dwZonePCRoomUserNum[i];
				dwBattleNum += m_dwZoneNum[i];
				break;
			}
		}
	}

	DISPMSG( "�� Channel No.%d: ", GetChannelID() );
	DISPMSG( "Total(%u) CharScene(%u) Village(%u/%u) Field(%u/%u) Domination(%u/%u) Battle(%u/%u)\n",
			m_mapUsers.size(), dwCharSelectUserNum, dwVillageNum, dwVillageUserNum, 
            dwFieldNum, dwFieldUserNum, dwDominationFieldNum, dwDominationFieldUserNum,
            dwBattleNum, dwBattleUserNum );
}

VOID Channel::CreateParty( WORD wPartyKey )
{
	m_pPartyManager->CreateParty( wPartyKey );
}

BOOL Channel::JoinParty( WORD wPartyKey, User *pJoinUser )
{
	return m_pPartyManager->JoinParty( wPartyKey, pJoinUser );
}

BOOL Channel::WithdrawParty( User *pWithdrawUser )
{
	return m_pPartyManager->WithdrawParty( pWithdrawUser );
}

VOID Channel::DestroyParty( WORD wPartyKey )
{
	m_pPartyManager->DestroyParty( wPartyKey );
}

VOID Channel::RestoreParty( WORD wPartyKey, User *pUser )
{
	m_pPartyManager->RestoreParty( wPartyKey, pUser );
}

#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
VOID Channel::SetRoomKey(const WORD party_key, const WORD room_key)
{
    m_pPartyManager->SetRoomKey(party_key, room_key);
}
#endif  // _NA_004694_20120412_1202_PARTY_ROOM_KEY

// �Ϲ� ����, PC�� ���� ��ü ���� ����
VOID Channel::RefreshUserNum()
{
	USER_MAP_ITER	iter;
	User			*pUser=NULL;

	m_dwNormalUserNum = 0;
	m_dwPCRoomUserNum = 0;

	for( int i = 0; i < eZONETYPE_MAX; i++ )
	{
		m_dwZonePCRoomUserNum[i] = 0;
		m_dwZoneUserNum[i] = 0;
	}

	if( m_mapUsers.empty() )
		return;

	for( iter = m_mapUsers.begin(); iter != m_mapUsers.end(); ++iter )
	{
		pUser = (*iter).second;
		if( !pUser )
			continue;

		if( pUser->GetPCRoomID() )
		{
			m_dwZonePCRoomUserNum[pUser->GetStatus()]++;
			m_dwPCRoomUserNum++;
		}
		else
		{
			m_dwZoneUserNum[pUser->GetStatus()]++;
			m_dwNormalUserNum++;
		}
	}
}

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
VOID Channel::AddSpaField( DWORD zone_key, ChatZone* field )
{
    field->SetZoneType( eZONETYPE_SPA_FIELD );
    field->SetKey( zone_key );
    m_mapZones.insert( ZONE_MAP_PAIR( zone_key, field ) );
    IncZoneNum( eZONETYPE_SPA_FIELD );
}
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
