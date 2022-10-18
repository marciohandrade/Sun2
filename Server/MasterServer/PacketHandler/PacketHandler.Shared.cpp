#include "stdafx.h"

#include "PacketHandler.Shared.h"
#include <PacketStruct_ServerCommon.h>

#include "Handler_DM.h"
#include "Handler_MX.h"
#include "Handler_GM.h"
#include "Handler_AM.h"
#include "Handler_MW.h"
#include "Handler_MO.h"
#include "Handler_MZ.h"
#include "Handler_SM.h"

#include <Sessions/ServerSessionEx.h>
#include <Sessions/OpServerSession.h>
#include <SolarDateTime.h>
#include <MasterServerEx.h>
#include "Function/AbuseUserManager.h"


PacketHandlerShared::PacketHandlerShared()
{
}

PacketHandlerShared::~PacketHandlerShared()
{
}

#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
{ MAKEWORD(c, p), (ePHASE_SHIFT)(accptablePHASE), (PH_fnHandler)&PacketHandlerShared::On##p }

sPACKET_HANDLER_INFO*	PacketHandlerShared::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_ANS, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_HEARTBEAT, PHASE_UNLIMITED),
		//{ MAKEWORD(SERVERCOMMON, SERVERCOMMON_HEARTBEAT), PHASE_UNLIMITED, (PH_fnHandler)&PacketHandlerShared::OnHEARTBEAT },

		ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_SECURE_LOG_NTF, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
}

sPACKET_HANDLER_INFO*	PacketHandlerShared::GetDataHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN, PHASE_EXCHANGE|PHASE_SERVICE),
		ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_ACK, PHASE_EXCHANGE|PHASE_SERVICE),
		ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_NAK, PHASE_EXCHANGE|PHASE_SERVICE),
		ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_NTF, PHASE_EXCHANGE|PHASE_SERVICE),
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
}


//
//
//

HANDLER_COMMON_IMPL(SERVERCOMMON_SERVERSHUTDOWN_ANS)
{
	//MasterServerInfo& rINFO = MasterServerEx::GetMainFrame()->GetMasterServerInfo();
	//if( rINFO.IsTransactionExitProcess() )
	//{
	//	WORK_KEY wkKey = rINFO.GetWorkKey4ExitProcess();
	//	sSIG_EXITED_SESSION msgSIGNAL;
	//	msgSIGNAL.Session() = pServerSessionEx;
	//	WorkLoadManager::OnMsg( &msgSIGNAL, wkKey );
	//}
	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "SHUTDOWN ANS\n" ); );
}

HANDLER_COMMON_IMPL(SERVERCOMMON_HEARTBEAT)
{	// 0:1
	//SERVERCOMMON_HEARTBEAT
//	printf( "heartbeat\n" );
}

//
//
//
#include <WorkLoad/Handler_Shared.WorkLoad.cxx>
//
//
//

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

HANDLER_COMMON_IMPL(SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN)
{
	MSG_SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN* pMsgSYN = (MSG_SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN*)pMsg;

	eSCRIPT_CODE scriptCode = (eSCRIPT_CODE)pMsgSYN->m_wDataType;
	if( ScriptDataManager::IsLoadedData( scriptCode ) )
	{
		MSG_SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK msgACK;
		pServerSessionEx->SendPacket( &msgACK, sizeof(msgACK) );

		//__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "RECEIVED {DATA_TRANSFER_PLZ_GIVEME_DATA_SYN} SEND {DATA_TRANSFER_PLZ_GIVEME_DATA_ACK}\n" ); );

		PROCESS_DATA_TRANSFER_SENDDATA_CMD* pWorkCMD = new PROCESS_DATA_TRANSFER_SENDDATA_CMD();

		Services4Session& rSERVICE = pServerSessionEx->Services();

		if( WorkLoadManager::c_NotExistKey ==
			rSERVICE.GetWorkLoadContainer().FindWorkKey( scriptCode ) )
		{
			pWorkCMD->SetServerSessionEx( pServerSessionEx );

			eSCRIPT_DATA_CMD cmd = SCRIPT_DATA_CMD::GET | SCRIPT_DATA_CMD::BUFFERINFO::BUFFER | SCRIPT_DATA_CMD::BUFFERINFO::BUFFER_SIZE;
			BYTE* pSTREAM = NULL;
			DWORD dwSize = 0;
			ScriptDataManager::DataStreamCommand( cmd, scriptCode, pSTREAM, dwSize );
			pWorkCMD->SetInformation( pSTREAM, dwSize, 0, (WORD)scriptCode );
			WORK_KEY workKey = WorkLoadManager::RegisterTask( pWorkCMD );//, TRUE, scriptCode );
			if( WorkLoadManager::c_NotExistKey != workKey )
			{
				rSERVICE.GetWorkLoadContainer().RegisterWork( workKey, (DWORD)(__int64)pWorkCMD );
				return;
			} 
		}

		SAFE_DELETE( pWorkCMD );
	}

	// <THINKING>
	//   로딩된 데이터 없다고 무조건 NAK처리 할 게 아니다.
	// 추가된 사항에 의하면, MASTER는 캐쉬된 데이터를 일정 기간동안만 보관하고 지우게 될 것이다.
	// 데이터 전송중인 경우는 잠시 대기할 필요가 있다.
	//   데이터를 지우고 나서 새로 요청이 들어온다면, 여기에 대한 큐를 설정하고, 새로 SolarAuth
	// 를 통한 신규 데이터를 전송받아야 할 것이다.

	// 요청 큐 생성 - ScriptData 해당 데이터 리로드할 수 있도록 변경
	MSG_SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK msgNAK;
	pServerSessionEx->SendPacket( &msgNAK, sizeof(msgNAK) );
	return;
}

HANDLER_COMMON_IMPL(SERVERCOMMON_DATA_TRANSFER_SENDDATA_ACK)
{
	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "RECEIVED {DATA_TRANSFER_SENDDATA_ACK}\n" ); );
}

VOID PacketHandlerShared::OnSERVERCOMMON_DATA_TRANSFER_SENDDATA_NAK(
																ServerSessionEx* pServerSessionEx,
																MSG_BASE* pMsg,
																WORD wSize
																)
{
	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "RECEIVED {DATA_TRANSFER_SENDDATA_NAK}\n" ); );
}

#pragma pack(push, 1)
union sPHASE_CMD_PAIR
{
	struct
	{
		BYTE	byCATEGORY;
		BYTE	byPROTOCOL;
	};
	WORD		uPAIR;
};
#pragma pack(pop)

static BYTE s_pPHASE_CMDs[MAX_SERVER][2] =
{
	0,					0,								//00	UNKNOWN_SERVER
	AM_CONNECTION,		AM_CONNECTION_PHASE_SHIFT_CMD,	//01	AGENT_SERVER
	GM_CONNECTION,		GM_CONNECTION_PHASE_SHIFT_CMD,	//02	FIELD_SERVER
	GM_CONNECTION,		GM_CONNECTION_PHASE_SHIFT_CMD,	//03	BATTLE_SERVER
	DM_CONNECTION,		DM_CONNECTION_PHASE_SHIFT_CMD,	//04	GAME_DBPROXY
	MW_CONNECTION,		MW_CONNECTION_PHASE_SHIFT_CMD,	//05	WORLD_SERVER
	0,					0,								//06	MASTER_SERVER
	MZ_CONNECTION,		MZ_CONNECTION_PHASE_SHIFT_CMD,	//07	GUILD_SERVER
	MX_CONNECTION,		MX_CONNECTION_PHASE_SHIFT_CMD,	//08	ACCOUNT_DBPROXY
	0,					0,								//09	TOURNAMENT_SERVER
	0,					0,								//10	AUTH_AGENT_SERVER
	MP_CONNECTION,		MP_CONNECTION_PHASE_SHIFT_CMD,	//11	PORTAL_SERVER
	0,					0,								//12	SOLARAUTH_SERVER
	0,					0,								//13	SHOP_SERVER
	0,					0,								//14	RESERVED
	0,					0,								//15	RESERVED
	0,					0,								//16	RESERVED
	0,					0,								//17	RESERVED
	0,					0,								//18	RESERVED
	0,					0,								//19	RESERVED
	0,					0,								//20	RESERVED
	0,					0,								//21	RESERVED
	0,					0,								//22	RESERVED
	0,					0,								//23	RESERVED
	0,					0,								//24	RESERVED
	0,					0,								//25	OP_SERVER
	0,					0,								//26	TEMP_SERVER
	GM_CONNECTION,		GM_CONNECTION_PHASE_SHIFT_CMD,	//27	GAME_SERVER
};

HANDLER_COMMON_IMPL(SERVERCOMMON_DATA_TRANSFER_SENDDATA_NTF)
{
	eSERVER_TYPE serverType = pServerSessionEx->GetServerType();
	sPHASE_CMD_PAIR* pPHASE_CMDs = (sPHASE_CMD_PAIR*)s_pPHASE_CMDs;

	if( (DWORD)serverType < (DWORD)MAX_SERVER )
	{
		sPHASE_CMD_PAIR& rCMDs = pPHASE_CMDs[(DWORD)serverType];
		if( 0 != rCMDs.uPAIR )
		{
			MSG_AM_CONNECTION_PHASE_SHIFT_CMD msgCMD;
			msgCMD.m_byCategory = rCMDs.byCATEGORY;
			msgCMD.m_byProtocol = rCMDs.byPROTOCOL;
			msgCMD.m_dwPHASE_SHIFT = PHASE_SERVICE;
			pServerSessionEx->Send( (BYTE*)&msgCMD, sizeof(msgCMD) );
			// 아예 CommonPacket으로 꺼내는 것이 좋을 듯 하다.
			return;
		}
	}

	ASSERT( !"Invalid Packet!" );
	return;
}


VOID	PacketHandlerShared::SEND_SERVERCOMMON_PHASESHIFT_CMD( ServerSessionEx* pServerSession, ePHASE_SHIFT shiftCMD )
{
	eSERVER_TYPE serverType = pServerSession->GetServerType();
	sPHASE_CMD_PAIR* pPHASE_CMDs = (sPHASE_CMD_PAIR*)s_pPHASE_CMDs;

	if( (DWORD)serverType < (DWORD)MAX_SERVER )
	{
		sPHASE_CMD_PAIR& rCMDs = pPHASE_CMDs[(DWORD)serverType];
		if( 0 != rCMDs.uPAIR )
		{
			MSG_AM_CONNECTION_PHASE_SHIFT_CMD msgCMD;
			msgCMD.m_byCategory = rCMDs.byCATEGORY;
			msgCMD.m_byProtocol = rCMDs.byPROTOCOL;
			msgCMD.m_dwPHASE_SHIFT = (DWORD)shiftCMD;
			pServerSession->Send( (BYTE*)&msgCMD, sizeof(msgCMD) );
			// 아예 CommonPacket으로 꺼내는 것이 좋을 듯 하다.
			return;
		}
	}

	ASSERT( !"Invalid ServerT" );
}

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__


static BOOL		ParseSecureLogMessage( const DWORD dwMsgLen, sSIG_SECURE_LOG_NOTIFY* pINFO )
{
	MSG_SERVERCOMMON_SECURE_LOG_NTF& rNTF = *(pINFO->m_pRecvNTF);
	TCHAR* pTCHR = (TCHAR*)rNTF.m_pStream;
	DWORD dwInc = 0;

	while( (dwInc++ < dwMsgLen) && (*pTCHR != _T('_')) )
		++pTCHR;

	if( *pTCHR != _T('_') )
	{
		MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_DEFAULT,
			_T("Invalid Secure Log Format {\"%s\"}"), rNTF.m_pStream );
		return FALSE;
	}

#define INC_PTR_IDX			do { ++pTCHR; ++dwInc; } while(0)
#define TCHAR_TO_DIGIT	((DWORD)((*pTCHR)-_T('0'))/sizeof(TCHAR))
#define CALC_TWO_BYTES_TO_DECIMAL	do { dwCalc = TCHAR_TO_DIGIT*10; INC_PTR_IDX; dwCalc += TCHAR_TO_DIGIT; INC_PTR_IDX; } while(0)

	INC_PTR_IDX;

	// 단지 파싱하는데 이용할 뿐 사용하진 않는다. 시간은 Master의 현재시간을 이용한다.
	BYTE	rHOUR, rMINUTE, rSECOND;
	TCHAR	pCategory[32];
	DWORD dwCalc;

	CALC_TWO_BYTES_TO_DECIMAL;
	rHOUR = (BYTE)dwCalc;

	INC_PTR_IDX;

	CALC_TWO_BYTES_TO_DECIMAL;
	rMINUTE = (BYTE)dwCalc;

	INC_PTR_IDX;

	CALC_TWO_BYTES_TO_DECIMAL;
	rSECOND = (BYTE)dwCalc;

	INC_PTR_IDX;	// skip ']'
	INC_PTR_IDX;	// skip '\t'

	do
	{
		TCHAR* pCateOff = pCategory;
		while( (dwInc++ < dwMsgLen) && (*pTCHR != _T('\t')) )
		{
			*pCateOff = *pTCHR;
			++pCateOff;
			++pTCHR;
		}

		*pCateOff = _T('\0');
	} while(0);

	INC_PTR_IDX;

	pINFO->m_pMessage = pTCHR;

	return TRUE;
}

HANDLER_COMMON_IMPL(SERVERCOMMON_SECURE_LOG_NTF)
{
	MSG_SERVERCOMMON_SECURE_LOG_NTF* pRecvNTF = (MSG_SERVERCOMMON_SECURE_LOG_NTF*)pMsg;
	DWORD dwLength = (DWORD)((pRecvNTF->m_wLength)/sizeof(TCHAR));

	sSIG_SECURE_LOG_NOTIFY sINFO;
	sINFO.m_pRecvNTF = (MSG_SERVERCOMMON_SECURE_LOG_NTF*)pRecvNTF;
	sINFO.m_ServerKey = pServerSessionEx->GetServerKey();

	if( !ParseSecureLogMessage( dwLength, &sINFO ) )
		return;

	AbuseUserManager::Instance()->OnMsg( &sINFO );
}



























