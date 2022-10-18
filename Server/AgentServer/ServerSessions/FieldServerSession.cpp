#include "stdafx.h"

#include <AgentServer.h>
#include <ServerSessions/FieldServerSession.h>
#include <PacketHandler/PacketHandler.h>

//////////////////////////////////////////////////////////////////////////
#include <MapInfoParser.h>

#include <Zone/Village.h>
#include <Zone/HuntingField.h>
#include <Zone/DominationField.h>
#include <ServerOptionParserEx.h>
#include <Zone/ZoneManager.h>
#include "MapPartialLoadingParser.h"

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
#include "Zone/SpaField.h"
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO
#include <GameOptionParser.h>
#endif

FieldServerSession::FieldServerSession()
{
}

FieldServerSession::~FieldServerSession()
{
}

VOID FieldServerSession::Init()
{
	GameServerSession::Init();
}

// CHANGES: f110617.1L
void FieldServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
    // CHANGES: f110615.2L, added to solve high-load network bottleneck problem
    first_contact_msg->m_byCategory = AG_CONNECTION;
    first_contact_msg->m_byProtocol = AG_CONNECTION_GROUND_CMD;
    this->UseSendPendingBuffer(2 * 1024 * 1024);
    this->UseRecvDualBuffer(10 * 1024 * 1024);
}

VOID FieldServerSession::CreateVillage()
{
    AgentServer* pAgentServer = AgentServer::GetInstance();
    SERVER_KEY rKey(pAgentServer->GetKey());

	sMAPINFO_HASH & rHash = MapInfoParser::Instance()->GetMapInfoHash();
	for( sMAPINFO_HASH::const_iterator it = rHash.begin() ; it != rHash.end() ; ++it )
	{
		sMAPINFO * psMapinfo = (it->second);

		// 빠른 로딩을 위해 MapPartialLoading.ini를 참조한다.
		MAP_PARTIAL_LOADING *pLoadingInfo = MapPartialLoadingParser::Instance()->FindMapInfo( psMapinfo->MapKind );
		if( pLoadingInfo && !pLoadingInfo->m_bLoading )
		{
			continue;
		}

        ZoneInterface* pZone = NULL;
        RC::eROOM_RESULT (ZoneManager::*fnCreate)(MAPCODE, ZoneInterface*) = NULL;

        switch(psMapinfo->GetZoneType4SharedCode())
        {
        case eZONETYPE_VILLAGE:
            pZone = g_ZoneManager.FindVillage( psMapinfo->MapCode );
            fnCreate = &ZoneManager::CreateVillage;        
            break;

        case eZONETYPE_FIELD:
            pZone = g_ZoneManager.FindHuntingField( psMapinfo->MapCode );
            fnCreate = &ZoneManager::CreateHuntingField;        
            break;

        case eZONETYPE_DOMINATION_FIELD:
            {
            #ifdef _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO
                static const BYTE byChannel = GameOptionParser::Instance()->GetGameOption().m_byDominationChannel;
            #else
                static const BYTE byChannel = ServerOptionParserEx::Instance()->GetServerOption().m_byDominationChannel;
            #endif

                if( byChannel == rKey.GetChannelID() )
                {
                    pZone = g_ZoneManager.FindDominationField( psMapinfo->MapCode );
                    fnCreate = &ZoneManager::CreateDominationField;        
                }
            }
            break;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        case eZONETYPE_SPA_FIELD:
            {
                static const BYTE byChannel = ServerOptionParserEx::Instance()->GetServerOption().spa_field_channel;
                //값이 0이면 채널구분을 하지 않고 생성한다
                if( byChannel == 0 || byChannel == rKey.GetChannelID() )
                {
                    pZone = g_ZoneManager.FindSpaField( psMapinfo->MapCode );
                    fnCreate = &ZoneManager::CreateSpaField;        
                }
            }
            break;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

        default:
            break;
        }

        if( pZone )
        {
            pZone->SetLinkedServer(this);
            pZone->Create();
        }
        else if( fnCreate )
        {
            RC::eROOM_RESULT rt = (g_ZoneManager.*fnCreate)(psMapinfo->MapCode, NULL);
            ASSERT( rt == RC::RC_ROOM_SUCCESS );
        }
    }
}







