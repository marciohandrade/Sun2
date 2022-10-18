#include "stdafx.h"
#include "Handler_MP.h"

#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
#include "PacketHandler.Shared.h"


#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&Handler_MP::On##p }


sPACKET_HANDLER_INFO*	Handler_MP::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		//
		//ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_ANS, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_MP::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<PORTAL_SERVER>( GetHandlerInfo() );
	pInstance->RegisterHandler_<PORTAL_SERVER>( PacketHandlerShared::GetHandlerInfo() );
	pInstance->RegisterHandler_<PORTAL_SERVER>( PacketHandlerShared::GetDataHandlerInfo() );
}

//
//
//
#include <MasterServerEx.h>
#include <Function/ServerInfoManager.h>

//HANDLER_MP_IMPL(SERVERCOMMON_SERVERSHUTDOWN_ANS)
//{
//	//// 종료 대기중인 놈이 종료된 것이므로 체크
//	//if( MasterServer::Instance()->IsExitServer() && MasterServer::Instance()->GetExitType() == eEXIT_GAMEWAIT )
//	//{
//	//	MasterServer::Instance()->RemoveExitServer( pServerSession->GetServerKey() );	// 종료되었다!
//	//	MessageOut( eFULL_LOG, "Portal Exit Message Recv[%u]", pServerSession->GetServerKey() );
//	//}
//}

#endif //if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)