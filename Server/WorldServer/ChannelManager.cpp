#include "StdAfx.h"
#include ".\channelmanager.h"
#include "Channel.h"
#include "UserManager.h"
#include "User.h"
#include <assert.h>

// ���� ���� ����
#include <PacketStruct_MW.h>
#include "ServerSession.h"
#include "ServerSessionManager.h"

// ������ ���� ���� ����
#include "WorldServerInfoParser.h"

// ���� ����
#include <SolarFileReader.h>

// ����/�ʵ� ���� ����
#include <MapInfoParser.h>
#include <FieldInfo.h>

#ifdef _JP_0_20101123_PMS
#include "WorldServer.h"
#endif

ChannelManager::ChannelManager(void)
{
	m_pChannelPool = NULL;
}

ChannelManager::~ChannelManager(void)
{
	assert( m_pChannelPool == NULL );
}

VOID ChannelManager::Init()
{
	m_pChannelPool = new util::CMemoryPoolFactory<Channel>;
	m_pChannelPool->Initialize( 5, 1 );

	m_StatisticsTimer.SetTimer( WorldServerInfoParser::Instance()->GetStatisticsInterval() );

	#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

		if( !LoadFieldInfo() )
			MessageOut( eCRITICAL_LOG, "FieldInfo Load Failed");

	#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

//	MessageOut( eFULL_LOG, "Set Timer %u", m_StatisticsTimer.GetIntervalTime() );
}

VOID ChannelManager::Release()
{	
	CHANNEL_MAP_ITER	it;
	Channel				*pChannel;
	CHANNEL_MAP& mapClone = m_mapChannels;
	for( it = mapClone.begin(); it != mapClone.end(); ++it )
	{
		pChannel = it->second;
		RemoveChannel( pChannel->GetChannelID() );
	}
	mapClone.clear();

	FIELDINFO_MAP_ITER	iter;

	for( iter = m_mapFieldInfo.begin(); iter != m_mapFieldInfo.end(); iter++ )
	{
		FieldInfo*& pFieldInfo = (*iter).second;
		if(pFieldInfo)
		{
			pFieldInfo->Destroy();
			SAFE_DELETE(pFieldInfo);
			//delete pFieldInfo;	<- �̰� �� �ּ�ó�� �ߴ��� Ȯ���� ����
		}
	}
	m_mapFieldInfo.clear();

	m_pChannelPool->Release();
	SAFE_DELETE(m_pChannelPool);
}

VOID ChannelManager::Update()
{
	// ���� �ð����� ���� ���� ����
	DWORD			dwTotalCount=0, dwVillageCount=0, dwBattleZoneCount=0;

	// 10�ʸ��� üũ...������ ���� �ϳ�...-0-
	if( m_StatisticsTimer.IsExpired() )
	{
		CHANNEL_MAP_ITER	it;
		Channel *pChannel;
		MSG_MW_USERCOUNT_SYN	sendMsg;
		BYTE					byChannel;
		DWORD					dwTotalTemp = 0;	// ��ü ����

		sendMsg.byChannelCount = 0;
		for( it = m_mapChannels.begin(); it != m_mapChannels.end(); ++it )
		{
			pChannel = it->second;
			byChannel = sendMsg.byChannelCount;
			sendMsg.ChannelInfo[byChannel].dwChannelID = pChannel->GetChannelID();

			pChannel->RefreshUserNum();			// �ϴ� �Ź� ���� ���� ���� - Inc/Dec �� ��� ���� ��Ȳ�� ���� ��ó�� ����� �� �ȴ�

			// �ӽ÷� �ε����̰ų� �ʵ� �����鿡 ���Ե��� �ʴ� ���� �ִ� ����� ��Ʋ�� ������ �ջ��Ѵ�
			// �ջ� ���: ������ ��Ʋ�� ���� = ��Ʋ�� ���� + (���� ����)
			//                               = ��Ʋ�� ���� + (��ü ���� - ���� ���� - ��Ʋ�� ����)
			//								 = ��ü ���� - ���� ����

			sendMsg.ChannelInfo[byChannel].dwVillageCount = pChannel->GetZoneUserNum( eZONETYPE_CHARSELECT ) 
															+ pChannel->GetZoneUserNum( eZONETYPE_VILLAGE )
                                                            + pChannel->GetZoneUserNum( eZONETYPE_DOMINATION_FIELD )
															+ pChannel->GetZoneUserNum( eZONETYPE_FIELD )
                                                            + pChannel->GetZoneUserNum( eZONETYPE_SPA_FIELD);

			if( pChannel->GetNumerOfNormalUsers() < sendMsg.ChannelInfo[byChannel].dwVillageCount )
			{
				MessageCount( eCRITICAL_LOG, "Channel[%u] User Statistics Error! Not Correct Field UserNum!![%u/%u]", 
																						pChannel->GetNumerOfNormalUsers(),
																						sendMsg.ChannelInfo[byChannel].dwVillageCount );
				sendMsg.ChannelInfo[byChannel].dwBattleZoneCount = 0;
			}
			else
			{
				sendMsg.ChannelInfo[byChannel].dwBattleZoneCount = pChannel->GetNumerOfNormalUsers()
																- sendMsg.ChannelInfo[byChannel].dwVillageCount;
			}

			sendMsg.ChannelInfo[byChannel].dwVillagePCRoomCount = pChannel->GetZonePCRoomUserNum( eZONETYPE_CHARSELECT )
																+ pChannel->GetZonePCRoomUserNum( eZONETYPE_VILLAGE )
                                                                + pChannel->GetZonePCRoomUserNum( eZONETYPE_DOMINATION_FIELD )
																+ pChannel->GetZonePCRoomUserNum( eZONETYPE_FIELD )
                                                                + pChannel->GetZonePCRoomUserNum( eZONETYPE_SPA_FIELD);

			if( pChannel->GetNumerOfPCRoomUsers() < sendMsg.ChannelInfo[byChannel].dwVillagePCRoomCount )
			{
				MessageCount( eCRITICAL_LOG, "Channel[%u] User Statistics Error! Not Correct Field UserNum!![%u/%u]", 
																					pChannel->GetNumerOfNormalUsers(),
																					sendMsg.ChannelInfo[byChannel].dwVillagePCRoomCount );
				sendMsg.ChannelInfo[byChannel].dwBattleZonePCRoomCount = 0;
			}
			else
			{
				sendMsg.ChannelInfo[byChannel].dwBattleZonePCRoomCount = pChannel->GetNumerOfPCRoomUsers()
																		- sendMsg.ChannelInfo[byChannel].dwVillagePCRoomCount;
			}

			MessageCount( eFULL_LOG, "Connected Users[Channel%d]:Total(%u)Field(%u/%u)Battle(%u/%u)",
							sendMsg.ChannelInfo[byChannel].dwChannelID, 
							pChannel->GetTotalUserNum(), 
							sendMsg.ChannelInfo[byChannel].dwVillageCount, sendMsg.ChannelInfo[byChannel].dwVillagePCRoomCount, 
							sendMsg.ChannelInfo[byChannel].dwBattleZoneCount, sendMsg.ChannelInfo[byChannel].dwBattleZonePCRoomCount );

			dwTotalCount += pChannel->GetTotalUserNum();
			dwVillageCount += sendMsg.ChannelInfo[byChannel].dwVillageCount + sendMsg.ChannelInfo[byChannel].dwVillagePCRoomCount;
			dwBattleZoneCount += sendMsg.ChannelInfo[byChannel].dwBattleZoneCount + sendMsg.ChannelInfo[byChannel].dwBattleZonePCRoomCount;
			sendMsg.byChannelCount++;
		}

		MessageCount( eCRITICAL_LOG, "Total Users:Total(%u)Field(%u)Battle(%u)", dwTotalCount, dwVillageCount, dwBattleZoneCount );

		// ServerInfo�� ���Ͽ��� ���� �ʾҴµ�
		if( !WorldServerInfoParser::Instance()->IsServerInfoFromFile() )
		{
			// ������ ������ ���ӵ� �� �� ������ ���� ���� ������ �Ŵ�!
			ServerSession *pMasterServer = ServerSessionManager::Instance()->GetMasterServer();
			if( !pMasterServer )
			{
				MessageOut( eCRITICAL_LOG, "Connect Statistics Send Failed - No Master Server");
				return;
			}
			pMasterServer->Send( (BYTE*)&sendMsg, sendMsg.GetSize() );
			MessageOut( eFULL_LOG, "Send Connect Statistics to Master Server");
		}
	}
}

Channel* ChannelManager::AllocChannel()
{
	Channel *pChannel = (Channel *)m_pChannelPool->Alloc();
	pChannel->Init();
	return pChannel;
}

VOID ChannelManager::FreeChannel( Channel *pChannel )
{
	pChannel->Release();
	m_pChannelPool->Free( pChannel );
}

VOID ChannelManager::AddChannel( DWORD dwChannelID )
{
	CHANNEL_MAP_ITER it = m_mapChannels.find( dwChannelID );

	if( it != m_mapChannels.end() )
		return;

	Channel *pChannel = AllocChannel();
	pChannel->SetChannelID( dwChannelID );
	m_mapChannels.insert( CHANNEL_MAP_PAIR( dwChannelID, pChannel ) );
	MessageOut( eCRITICAL_LOG,   "Add Channel(%d)", dwChannelID );

#ifdef _JP_0_20101123_PMS
    WorldServer::Instance()->SetPMSChannelCount(m_mapChannels.size());
#endif
}

void ChannelManager::RemoveChannel(DWORD channel_id)
{
    // CHANGES: f110521.2L, prevent recursively remove processing
    CHANNEL_MAP::iterator it = m_mapChannels.find(channel_id);
    if (it == m_mapChannels.end()) {
        return;
    };

    Channel* const channel = it->second;
    m_mapChannels.erase(it);
    FreeChannel(channel);
    MessageOut(eCRITICAL_LOG, "Remove Channel(%d)", channel_id);
#ifdef _JP_0_20101123_PMS
    WorldServer::Instance()->SetPMSChannelCount(m_mapChannels.size());
#endif
}

Channel* ChannelManager::GetChannel( DWORD dwChannelID )
{
	CHANNEL_MAP_ITER it = m_mapChannels.find( dwChannelID );
	if( it == m_mapChannels.end() )
	{
		return NULL;
	}
	else
	{
		return it->second;
	}
}
/*
VOID ChannelManager::SendToAllChannel( BYTE *pMsg, WORD wSize )
{
	CHANNEL_MAP_ITER	it;
	Channel				*pChannel;
	for( it = m_mapChannels.begin(); it != m_mapChannels.end(); ++it )
	{
		pChannel = it->second;
		pChannel->SendToAll( pMsg, wSize );
	}
}
*/

VOID ChannelManager::SendToVillages( BYTE *pMsg, WORD wSize )
{
	CHANNEL_MAP_ITER	it;
	Channel				*pChannel;
	for( it = m_mapChannels.begin(); it != m_mapChannels.end(); ++it )
	{
		pChannel = it->second;
		pChannel->SendToVillages( pMsg, wSize );
	}
}

VOID ChannelManager::SendToBattleZones( BYTE *pMsg, WORD wSize )
{
	CHANNEL_MAP_ITER	it;
	Channel				*pChannel;
	for( it = m_mapChannels.begin(); it != m_mapChannels.end(); ++it )
	{
		pChannel = it->second;
		pChannel->SendToBattleZones( pMsg, wSize );
	}
}

VOID ChannelManager::DisplayChannelInfo()
{
	CHANNEL_MAP_ITER	it;
	Channel				*pChannel;

	if( m_mapChannels.empty() )
	{
		MessageOut(eCRITICAL_LOG,   "No Channel." );
		return;
	}

	for( it = m_mapChannels.begin(); it != m_mapChannels.end(); ++it )
	{
		pChannel = it->second;
		pChannel->DisplayInfo();
	}
}

FieldInfo* ChannelManager::GetFieldInfo( DWORD dwFieldCode )
{
	FIELDINFO_MAP_ITER iter = m_mapFieldInfo.find( dwFieldCode );

	if( iter == m_mapFieldInfo.end() )
	{
		return NULL;
	}

	FieldInfo* pFieldInfo = (*iter).second;

	return pFieldInfo;
}

BOOL ChannelManager::LoadFieldInfo()
{
	//	(WAVERIX)(080605)(reduce error rates)
	MapInfoParser* const pParser = MapInfoParser::Instance();

	sMAPINFO_HASH& HashMap = pParser->GetMapInfoHash();
	if( HashMap.empty() )
	{
		MessageOut( eCRITICAL_LOG, "MapInfoParser Error - No Data" );
		return FALSE;
	}

	// ���� ���� ��츸 ������ �д´�
	//	(WAVERIX)(080718)(memory leak buf-fix) : [�ؿ�] solarauth���� �� ���ӽ� ������ �߱�ȴ�.
	//	- m_mapFieldInfo�� LoadFieldInfo�� ȣ��Ǵ� �������� �߰��ȴ�.
	//	- ���� m_mapFieldInfo�� ���� ����� �׳� true��ȯ�ϱ�� �Ѵ�.
	if( !m_mapFieldInfo.empty() )
	{
		MessageOut( eCRITICAL_LOG, "Already Loaded Fields" );
		return TRUE;
	}

	FOREACH_CONTAINER( sMAPINFO_HASH::value_type& rNode, HashMap, sMAPINFO_HASH )
	{
		sMAPINFO* const pMapInfo = rNode.second;
		const eZONETYPE ztZONE = pMapInfo->GetZoneType4SharedCode();

		switch( ztZONE )
		{
        case eZONETYPE_DOMINATION_FIELD:
		case eZONETYPE_VILLAGE: case eZONETYPE_FIELD:
        case eZONETYPE_SPA_FIELD:
			{
				// �� �о Village �߰�
				for( int i=0 ; i<sMAPINFO::MAX_FIELD_NUM; ++i)
				{
					const MAPCODE map_code = pMapInfo->FCode[i];
					if( !map_code )					continue;
					if( GetFieldInfo( map_code ) )	continue;

					FieldInfo* const pFieldInfo = new FieldInfo;
					pFieldInfo->Create( pParser->GetFieldInfo( map_code ) );
					pFieldInfo->Load();

					pFieldInfo->EstablishSectorInfo( VILLAGE_SECTOR_SIZE, TRUE );

					// �̰� �̷��� �ϴ� ���� �Ҹ�.... FCode �������鼭 ���ϴ� ������... ���� ���ذ� �Ȱ���.
					const FIELDCODE field_code = pFieldInfo->GetFieldCode();
					const std::pair< FIELDINFO_MAP_ITER, bool > ret =
						m_mapFieldInfo.insert( FIELDINFO_MAP_PAIR( field_code, pFieldInfo ) );

					if( ret.second )
						SUNLOG( eFULL_LOG, "FieldInfo[%u] Template Add Completed", field_code );
					else
					{
						SUNLOG( eCRITICAL_LOG, "FieldInfo[%u] Duplicated Info %u\n", field_code );
						delete pFieldInfo;
					}
				}
			}
			break;
		}
	}

	return TRUE;
}


BOOL			ChannelManager::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_SHOUTRANGE:
		return _LoadLinkedZoneFile( bReload );
	}

	return FALSE;
}

BOOL			ChannelManager::_LoadLinkedZoneFile(  BOOL bReload )
{
	m_mapLinkedZoneInfo.clear();

	SERVER_LINKEDZONE_INFO	LinkedZoneInfo;
	char szTemp[MAX_FIELD_NAME_SIZE];

	const INT nRowSize = GetRowSize();
	for( INT nRow=0; nRow<nRowSize ; ++nRow )
	{
		memset( &LinkedZoneInfo, 0, sizeof(SERVER_LINKEDZONE_INFO) );

		// ���� �Ľ�
		LinkedZoneInfo.dwMapCode = GetDataDWORD( "MapCode", nRow );
		strncpy( LinkedZoneInfo.szMapName, GetDataString( "MName", nRow ), MAX_MAPNAME_LENGTH );	// ���̸�
		LinkedZoneInfo.byMKind = GetDataBYTE( "MKind", nRow );	// MKind
		for( int i=0; i<7; i++ )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "LField%d", i+1 );
			LinkedZoneInfo.dwLinkedMapCode[i] = GetDataDWORD( szTemp, nRow );	// LField1
		}

		m_mapLinkedZoneInfo.insert( LINKEDZONE_MAP_PAIR(LinkedZoneInfo.dwMapCode, LinkedZoneInfo) );
	}

	return TRUE;
}

