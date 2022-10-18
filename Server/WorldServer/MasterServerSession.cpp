#include "stdafx.h"
#include <PacketStruct.h>
#include <PacketStruct_MW.h>
#include <Protocol_MW.h>
#include <const.h>
#include "MasterServerSession.h"
#include "PacketHandler.h"
#include "WorldServer.h"

MasterServerSession::MasterServerSession()
{
}

MasterServerSession::~MasterServerSession()
{
}

VOID MasterServerSession::Init()
{
	ServerSession::Init();
}

VOID MasterServerSession::Release()
{
	ServerSession::Release();
}

VOID MasterServerSession::Update()
{
	ServerSession::Update();
}

VOID MasterServerSession::OnRedirect()
{
	MessageOut(eCRITICAL_LOG,  "Master server connected.(%s)", GetIP() );
}

VOID MasterServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );

	if( bSuccess )
	{
		MessageOut(eCRITICAL_LOG,  "Connected to master server." );
		SetSyncKey( MASTER_SERVER );
	}
}

VOID MasterServerSession::OnDisconnect()
{
	ServerSession::OnDisconnect();

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	ScriptDataManager::ReRegisterReloadDataList();
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	WorldServer::Instance()->SetPhaseStatus( PHASE_UNKNOWN );
	MessageOut(eCRITICAL_LOG,  "Disconnected from master server." );
}

VOID MasterServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	MSG_BASE *pBaseMsg = (MSG_BASE*)pMsg;

	PacketHandler::Instance()->ParsePacket_MW( this, (MSG_BASE*)pMsg, wSize );
}
