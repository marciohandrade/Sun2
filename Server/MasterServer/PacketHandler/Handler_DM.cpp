#include "stdafx.h"
#include "Handler_DM.h"
#include "PacketHandler.Shared.h"


#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&Handler_DM::On##p }


sPACKET_HANDLER_INFO*	Handler_DM::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		//
        ADD_HANDLER_INFO(DM_CONNECTION, DM_CONNECTION_GROUND_CMD, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_HEARTBEAT, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_ANS, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_DM::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<GAME_DBPROXY>( GetHandlerInfo() );
	pInstance->RegisterHandler_<GAME_DBPROXY>( PacketHandlerShared::GetHandlerInfo() );
	pInstance->RegisterHandler_<GAME_DBPROXY>( PacketHandlerShared::GetDataHandlerInfo() );
}

//
//
//
#include <MasterServerEx.h>
#include <Function/ServerInfoManager.h>

HANDLER_DM_IMPL(DM_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

//HANDLER_DM_IMPL( SERVERCOMMON_HEARTBEAT )

//HANDLER_DM_IMPL( SERVERCOMMON_SERVERSHUTDOWN_ANS )
//{
//	//// 종료 대기중인 놈이 종료된 것이므로 체크
//	//if( MasterServer::Instance()->IsExitServer() && MasterServer::Instance()->GetExitType() == eEXIT_DBPROXYWAIT )
//	//{
//	//	MasterServer::Instance()->RemoveExitDBServer( pGameDBProxy->GetServerKey() );	// 종료되었다!
//	//	MessageOut( eCRITICAL_LOG, "GameDBProxy Exit Message Recv[%u]", pGameDBProxy->GetServerKey() );
//	//}
//}
