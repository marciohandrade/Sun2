#include "stdafx.h"
#include "Handler_MZ.h"
#include "PacketHandler.Shared.h"
#include <Sessions/OpServerSession.h>

#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&Handler_MZ::On##p }


sPACKET_HANDLER_INFO*	Handler_MZ::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
        ADD_HANDLER_INFO(DM_CONNECTION, DM_CONNECTION_GROUND_CMD, PHASE_UNLIMITED),
        //{__NA001254_20090109_RTTG_FUNC_ADD 
		ADD_HANDLER_INFO(MZ_GUILD, MZ_GUILD_NAME_CHANGE_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MZ_GUILD, MZ_GUILD_NAME_CHANGE_NAK, PHASE_UNLIMITED),
        //}
		//
		//ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_ANS, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_MZ::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<GUILD_SERVER>( GetHandlerInfo() );
	pInstance->RegisterHandler_<GUILD_SERVER>( PacketHandlerShared::GetHandlerInfo() );
	pInstance->RegisterHandler_<GUILD_SERVER>( PacketHandlerShared::GetDataHandlerInfo() );
}

//
//
//
#include <MasterServerEx.h>
#include <Function/ServerInfoManager.h>

HANDLER_MZ_IMPL(DM_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

//HANDLER_MZ_IMPL(SERVERCOMMON_SERVERSHUTDOWN_ANS)
//{
//	//// 종료 대기중인 놈이 종료된 것이므로 체크
//	//if( MasterServer::Instance()->IsExitServer() && MasterServer::Instance()->GetExitType() == eEXIT_GAMEWAIT )
//	//{
//	//	MasterServer::Instance()->RemoveExitServer( pGuildServerSession->GetServerKey() );	// 종료되었다!
//	//	MessageOut( eFULL_LOG, "Guild Exit Message Recv[%u]", pGuildServerSession->GetServerKey() );
//	//}
//}


//{__NA001254_20090109_RTTG_FUNC_ADD
HANDLER_MZ_IMPL(MZ_GUILD_NAME_CHANGE_ACK)
{
	MSG_MZ_GUILD_NAME_CHANGE_ACK* pMsgACK = (MSG_MZ_GUILD_NAME_CHANGE_ACK*)pMsg;
	MasterServerEx*  pMasterServer	= MasterServerEx::GetMainFrame();
	OpServerSession* pOpSession		= pMasterServer->ConnectSessions.SessionOpServer();
	if( !pOpSession) 
		return;

	MSG_MO_RTTG_CHANGE_GUILDNAME_ANS msg;
	msg.m_dwWopsKey = pMsgACK->m_dwKey;
	msg.m_dwResult  = 0;

	pOpSession->SendPacket( &msg,sizeof(MSG_MO_RTTG_CHANGE_GUILDNAME_ANS) );

	SUNLOG(eCRITICAL_LOG, "[MSG_MO_RTTG_CHANGE_GUILDNAME_ANS] 길드명 변경 결과[성공] WopsKey[%d]", msg.m_dwWopsKey);
}

HANDLER_MZ_IMPL(MZ_GUILD_NAME_CHANGE_NAK)
{
	const int MaxLen = MSG_MO_RTTG_CHANGE_GUILDNAME_ANS::MAX_RESULT_DESCRIPTION_LENGTH;

	MSG_MZ_GUILD_NAME_CHANGE_NAK* pMsgACK = (MSG_MZ_GUILD_NAME_CHANGE_NAK*)pMsg;
	MasterServerEx*  pMasterServer	= MasterServerEx::GetMainFrame();
	OpServerSession* pOpSession		= pMasterServer->ConnectSessions.SessionOpServer();
	if( !pOpSession) 
		return;

	MSG_MO_RTTG_CHANGE_GUILDNAME_ANS msg;
	msg.m_dwWopsKey = pMsgACK->m_dwKey;
	msg.m_dwResult  = MSG_MO_RTTG_CHANGE_GUILDNAME_ANS::CHANGE_FAIL;
	switch( pMsgACK->m_byErrorCode )
	{
	case RC::RC_GUILD_NAME_CHANGE_CANNOT_DB:	
		strncpy( msg.m_szResultDescription, "DB 오류", MaxLen );
		break;

	case RC::RC_GUILD_NAME_CHANGE_CANNOT_DB_SYSTEM:	
		strncpy( msg.m_szResultDescription, "DB 시스템 오류", MaxLen );
		break;

	case RC::RC_GUILD_NAME_CHANGE_CANNOT_DB_SAME_GUILDNAME:	
		strncpy( msg.m_szResultDescription, "동일 길드명 존재", MaxLen );
		break;

	case RC::RC_GUILD_NAME_CHANGE_CANNOT_DB_BLANK_SPACE:	
		strncpy( msg.m_szResultDescription, "길드명 공백 포함", MaxLen );
		break;

	case RC::RC_GUILD_DISCONNECTED_DBP_SERVER:
		strncpy( msg.m_szResultDescription, "길드 서버가 DBP서버와 연결되어 있지 않다.", MaxLen );
		break;

	default:
		strncpy( msg.m_szResultDescription, "DB ERROR", MaxLen );

	}
	msg.m_szResultDescription[MaxLen] = '\0';
	
	pOpSession->SendPacket( &msg,sizeof(MSG_MO_RTTG_CHANGE_GUILDNAME_ANS) );

	SUNLOG(eCRITICAL_LOG, "[MSG_MO_RTTG_CHANGE_GUILDNAME_ANS] WopsKey[%d] 길드명 변경 결과[%s][%d] ",
		msg.m_dwWopsKey, msg.m_szResultDescription, pMsgACK->m_byErrorCode );
//RC::RC_GUILD_NOT_EXIST_GUILD;
	//RC::RC_GUILD_DELCHK_DESTROY_WAIT;
	//RC_GUILD_DISCONNECTED_DBP_SERVER
}
//}__NA001254_20090109_RTTG_FUNC_ADD