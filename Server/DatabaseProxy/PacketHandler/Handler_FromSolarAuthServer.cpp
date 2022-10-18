#include "StdAfx.h"
#include <Macro.h>
#include "Sessions/ChildServerSession.h"
#include <PacketStruct_SX.h>
#include <PacketStruct_DM.h>
#include <PacketStruct_ServerCommon.h>
#include "Handler_FromSolarAuthServer.h"
#include "DBProxyServerEx.h"
#include "QueryObjects/SolarAuthServerQuery.h"

// 이 파일은 임시로 ACCOUNTDB와 MASTER간의 루틴을 그대로 이었다.
// 이후 T_ServerInfo가 확장된 이후에 처리 로직을 변경하기로 한다.

//==================================================================================================
// CHANGES: f110518.2L
const nsPacket::PacketHandlerInfo Handler_FromSolarAuthServer::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), false, phase, Handler_FromSolarAuthServer::On##p }
#define HANDLER_NODE_INFO_DBR(c, p, phase) { MAKEWORD(c, p), true, phase, Handler_FromSolarAuthServer::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, false, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        //
    #ifdef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
        HANDLER_NODE_INFO(SX_CONNECTION, SX_CONNECTION_REQ_SERVER_INFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(SX_CONNECTION, SX_CONNECTION_REQ_SERVER_INFO_DBR, PHASE_UNLIMITED),
    #endif
        HANDLER_NODE_INFO_EOR() // end of records
    };
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};

//==================================================================================================


Handler_FromSolarAuthServer_IMPL(SX_CONNECTION_REQ_SERVER_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	ExecuteServerInfoQuery( pServerSession, 1 );
}

VOID Handler_FromSolarAuthServer::ExecuteServerInfoQuery( ChildServerSession* pServerSession, BYTE page )
{
	STRING_SAFER_QUERY64 szQueryBuff;
	szQueryBuff.MakeString(
		"S_GlobalServerInfoList %u,%u",
		page, GlobalServerInfoQuery::SERVERINFO_ROW_NUM );

	GlobalServerInfoQuery* pQuery = GlobalServerInfoQuery::ALLOC();

	ZeroMemory( pQuery->pServerInfo, sizeof(GlobalServerInfoQuery::sGLOBALSERVERINFO)*GlobalServerInfoQuery::SERVERINFO_ROW_NUM );
	pQuery->SetIndex( MAKEDWORD( (WORD)SX_CONNECTION, (WORD)SX_CONNECTION_REQ_SERVER_INFO_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pQuery->SetQuery( szQueryBuff );
	pQuery->SetCurPage(page);
	pServerSession->DBQuery( pQuery );
}

Handler_FromSolarAuthServer_IMPL(SX_CONNECTION_REQ_SERVER_INFO_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT* pResult	= (MSG_DBPROXY_RESULT *)pMsg;

	GlobalServerInfoQuery* pQResult = (GlobalServerInfoQuery *)pResult->m_pData;

	if( pQResult->GetResultRowNum() != 0 )
	{
		ASSERT( MSG_SX_CONNECTION_REQ_SERVER_INFO_ACK::_MAX_SERVERINFO_ARRAY_NUM != GlobalServerInfoQuery::SERVERINFO_ROW_NUM );
		ASSERT( pQResult->GetResultRowNum() < MSG_SX_CONNECTION_REQ_SERVER_INFO_ACK::_MAX_SERVERINFO_ARRAY_NUM );

		MSG_SX_CONNECTION_REQ_SERVER_INFO_ACK msgACK;
		msgACK.m_byCategory = SX_CONNECTION;
		msgACK.m_byProtocol = SX_CONNECTION_REQ_SERVER_INFO_ACK;

		DWORD dwRowNum = (DWORD)pQResult->GetResultRowNum();
		
		for( DWORD dwInc=0 ; dwInc<dwRowNum ; ++dwInc )
		{
			GLOBAL_SERVER_INFO_STRUCT& rDEST_INFO = msgACK.m_ServerInfo[dwInc];
			GlobalServerInfoQuery::sGLOBALSERVERINFO& rSRC_INFO = pQResult->pServerInfo[dwInc];

			memcpy( rDEST_INFO.szMasterIP, rSRC_INFO.pszMasterIP, MAX_IPADDRESS_SIZE );
			rDEST_INFO.szMasterIP[MAX_IPADDRESS_SIZE] = '\0';

			rDEST_INFO.skServerKey.Set(
									rSRC_INFO.byWorldID, rSRC_INFO.byChannelID,
									rSRC_INFO.byServerType, rSRC_INFO.byServerID
									);

			memcpy( rDEST_INFO.szPublicIP, rSRC_INFO.pszIP, MAX_IPADDRESS_SIZE );
			rDEST_INFO.szPublicIP[MAX_IPADDRESS_SIZE] = '\0';

			memcpy( rDEST_INFO.szNatIP, rSRC_INFO.pszNatIP, MAX_IPADDRESS_SIZE );
			rDEST_INFO.szNatIP[MAX_IPADDRESS_SIZE] = '\0';

			rDEST_INFO.wPublicPort = (WORD)rSRC_INFO.iPort;

			memcpy( rDEST_INFO.szInnerIP, rSRC_INFO.pszInnerIP, MAX_IPADDRESS_SIZE );
			rDEST_INFO.szInnerIP[MAX_IPADDRESS_SIZE] = '\0';

			rDEST_INFO.wInnerPort = (WORD)rSRC_INFO.iInnerPort;
		}

		msgACK.m_dwNumberOfServerInfo = dwRowNum;
		pServerSession->Send( (BYTE *)&msgACK, msgACK.GetPacketSize() );
		MessageOut( eCRITICAL_LOG, _T("MSG_SX_CONNECTION_REQ_SERVER_INFO_ACK") );
	}

	if( pQResult->GetResultRowNum() == 0 || pQResult->GetResultRowNum() < GlobalServerInfoQuery::SERVERINFO_ROW_NUM )
	{
		/// 마지막 데이터 이다.
		MSG_SX_CONNECTION_REQ_SERVER_INFO_CMD msgCMD;
		msgCMD.m_byCategory = SX_CONNECTION;
		msgCMD.m_byProtocol = SX_CONNECTION_REQ_SERVER_INFO_CMD;
		pServerSession->Send( (BYTE *)&msgCMD, sizeof(msgCMD) );
		MessageOut( eCRITICAL_LOG, _T("MSG_SX_CONNECTION_REQ_SERVER_INFO_CMD") );
	}
	else
	{
		ExecuteServerInfoQuery( pServerSession, pQResult->GetCurPage()+1 );
	}

	GlobalServerInfoQuery::FREE( pQResult ); pQResult = NULL;
}

Handler_FromSolarAuthServer_IMPL(SERVERCOMMON_HEARTBEAT)
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

Handler_FromSolarAuthServer_IMPL( SX_CONNECTION_BLOCK_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	//printf( "RECEIVED {SX_CONNECTION_BLOCK_CMD}\n" );
}

//-------------------------------------------------------------------------------------------------

