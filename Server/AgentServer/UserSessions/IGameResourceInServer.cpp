//	GameResourceInterface.cpp
//	(SUN_SECURE)
#include "stdafx.h"
#include "IGameResourceInServer.h"

#undef DECL_GRES_HASH
#define DECL_GRES_HASH(name, val, val2)				\
	uREG_NODE		IWzGResTableInServer::##name( uREG_NODE* pARGs )

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	기본 포맷
//	typedef uREG_PV (*IGResHandler)( uREG_NODE* );
//	<ARGUMENTS>		uREG_NODE	: { ARG0, ARG1, ARG2, ARG3 }
//	<ACTION>		(~~)
//	<RETURN>		uREG_NODE	: { ... }

// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		(NONE)
// <RETURN>		uREG_NODE	: { NULL, NULL, NULL, NULL }
DECL_GRES_HASH(EMPTY_CODE								, 001, 001)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;
	ZeroMemory( &uRET, sizeof(uRET) );

	uRET.EX = TRUE;

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { BYTE*, NULL, NULL, NULL }
// <ACTION>		ARG0.PTRB[0] = HIGH, ARG0.PTRB[1] = MIDDLE, ARG0.PTRB[2] = LOW
// <RETURN>		uREG_NODE	: uRET { TRUE }
DECL_GRES_HASH(GET_PROTOCOL_VERSION_A					, 002, 002)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

	pARGs->PTRB[0] = C2S_HIGH_VERSION_NO;
	pARGs->PTRB[1] = C2S_MIDDLE_VERSION_NO;
	pARGs->PTRB[2] = C2S_LOW_VERSION_NO;

	uRET.EX = TRUE;

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.ARRAY[0] = HIGH, uRET.ARRAY[1] = MIDDLE, uRET.ARRAY[2] = LOW, uRET.ARRAY[3] = CATEGORY_GENERATOR_250
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(GET_PROTOCOL_VERSION_B					, 002, 003)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

	uRET.ARRAY[0].EX = C2S_HIGH_VERSION_NO;
	uRET.ARRAY[1].EX = C2S_MIDDLE_VERSION_NO;
	uRET.ARRAY[2].EX = C2S_LOW_VERSION_NO;
	uRET.ARRAY[3].EX = CATEGORY_GENERATOR_250;

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.EX = CryptManager::GetMolaProtocolCRC()
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(GET_MOLA_PROTOCOL_CRC					, 003, 001)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

	uRET.EX = CryptManager::GetMolaProtocolCRC();

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.EX = NATION_CODE+CATEGORY_ODD_NUMBER
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(GET_NATION_CODE_A						, 003, 003)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

	uRET.EX = NATION_CODE + CATEGORY_ODD_NUMBER;

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.EX = NATION_CODE+PROTOCOL_ODD_NUMBER
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(GET_NATION_CODE_B						, 004, 004)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

	uRET.EX = NATION_CODE + PROTOCOL_ODD_NUMBER;

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { 별도 정의 }
// <ACTION>		(별도 정의)
// <RETURN>		uREG_NODE	: { 별도 정의 }
DECL_GRES_HASH(CALCULATION_T1							, 005, 005)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

	ZeroMemory( &uRET, sizeof(uRET) );

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { 별도 정의 }
// <ACTION>		(별도 정의)
// <RETURN>		uREG_NODE	: { 별도 정의 }
DECL_GRES_HASH(CALCULATION_T2							, 006, 006)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

	ZeroMemory( &uRET, sizeof(uRET) );

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { 별도 정의 }
// <ACTION>		(별도 정의)
// <RETURN>		uREG_NODE	: { 별도 정의 }
DECL_GRES_HASH(CALCULATION_T3							, 007, 007)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

	ZeroMemory( &uRET, sizeof(uRET) );

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { 별도 정의 }
// <ACTION>		(별도 정의)
// <RETURN>		uREG_NODE	: { 별도 정의 }
DECL_GRES_HASH(CALCULATION_T4							, 008, 008)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

	ZeroMemory( &uRET, sizeof(uRET) );

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { 별도 정의 }
// <ACTION>		(별도 정의)
// <RETURN>		uREG_NODE	: { 별도 정의 }
DECL_GRES_HASH(CALCULATION_T5							, 009, 009)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

	ZeroMemory( &uRET, sizeof(uRET) );

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.EX = GetTickCount()
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(GET_TICK_COUNT							, 011, 011)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

	uRET.EX = GetTickCount();

	return uRET;
}

// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.PTR = ([GAMESERVER][SEND_INFO].CProtocolCryptography *)
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(NETWORK_GET_GAME_SEND_CLIENT_CRYPT_INFO	, 012, 012)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

#if defined(_SERVER)
#elif defined(WISECLIENT_CORE_EXPORTS)
	WiseUser* pUser = UserSessionMgr::GetActiveTransactionWiseUser();
	if( pUser )
	{
		UserSessionMgr& rMGR = pUser->GetSessionMgr();
		ServerSession* pSession = rMGR.GetSession( AGENT_SESSION );
		if( pSession )
		{
			ClientSecurePackNode4Session& rSESSION = pSession->SECURE_PACK();
			uRET.PTR = &rSESSION.SEND_CRYPT_PACKET_INFO();
			return uRET;
		}
	}
#else //CLIENT
	
#endif //

	uRET.PTR = NULL;

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.PTR = ([GAMESERVER][RECV_INFO].CProtocolCryptography *)
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(NETWORK_GET_GAME_RECV_CLIENT_CRYPT_INFO	, 012, 013)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

#if defined(_SERVER)
#elif defined(WISECLIENT_CORE_EXPORTS)
	WiseUser* pUser = UserSessionMgr::GetActiveTransactionWiseUser();
	if( pUser )
	{
		UserSessionMgr& rMGR = pUser->GetSessionMgr();
		ServerSession* pSession = rMGR.GetSession( AGENT_SESSION );
		if( pSession )
		{
			ClientSecurePackNode4Session& rSESSION = pSession->SECURE_PACK();
			uRET.PTR = &rSESSION.RECV_CRYPT_PACKET_INFO();
			return uRET;
		}
	}
#else //CLIENT
#endif

	uRET.PTR = NULL;

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { PVOID, WORD, NULL, NULL }
// <ACTION>		uRET.EX = GlobalFunc::SendPacket( CI_GAMESERVERIDX, pARGs->ARRAY[0].PTR, pARGs->ARRAY[1].EX );
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(NETWORK_SENDPACKET_TO_GAMESERVER			, 014, 014)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

#if defined(_SERVER)
	User* pUser = GetActiveTransactionUser();
	MSG_BASE* pMSG = (MSG_BASE*)pARGs->ARRAY[0].PTR;
	WORD rwSize = (WORD)pARGs->ARRAY[1].EX;

	if( pUser && pMSG && rwSize )
	{
		if( pUser->SendPacket( pMSG, rwSize ) )
		{
			uRET.EX = TRUE;
			return uRET;
		}
	}
#elif defined(WISECLIENT_CORE_EXPORTS)
	WiseUser* pUser = UserSessionMgr::GetActiveTransactionWiseUser();
	MSG_BASE* pMSG = (MSG_BASE*)pARGs->ARRAY[0].PTR;
	WORD rwSize = (WORD)pARGs->ARRAY[1].VAL;
	if( pUser && pMSG && rwSize )
	{
		UserSessionMgr& rMGR = pUser->GetSessionMgr();
		uRET.EX = rMGR.SendPacket( AGENT_SESSION, pMSG, rwSize);
		return uRET;
	}
#else //CLIENT
#endif

	uRET.EX = FALSE;

	return uRET;
}
// <ARGUMENTS>	uREG_NODE	: { CHAR buffer[17], WORD, NULL, NULL }
// <ACTION>		strncpy( pARGs->PTRC, (CHARNAME*), 16 ); pARGs->PTRC[16] = '\0';
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(GET_USER_ID								, 015, 015)
{
    __TOUCHED(pARGs);
	uREG_NODE uRET;

	//strncpy( pARG->PTRC, (CHARNAME*), 16 ); pARG->PTRC[16] = '\0';
#if defined(_SERVER)
	if( GetActiveTransactionUser() )
	{
		strncpy( pARGs->PTRC, GetActiveTransactionUser()->GetUserID(), 16 );
		pARGs->PTRC[16] = '\0';
		uRET.EX = TRUE;
	}
#elif defined(WISECLIENT_CORE_EXPORTS)
#else //CLIENT
#endif //

	uRET.EX = FALSE;

	return uRET;
}
//DECL_GRES_HASH(GET_C2S_GAME_SENDCRYPT_INFO_DATA			, 100, 100)
//DECL_GRES_HASH(GET_C2S_GAME_RECVCRYPT_INFO_DATA			, 101, 101)
//DECL_GRES_HASH(GET_CHAR_MOVSPD_IN_DATA					, 102, 102)
//DECL_GRES_HASH(GET_CHAR_MOVSPD_IN_GAME					, 103, 103)
//DECL_GRES_HASH(GET_S2C_AGENT_SENDCRYPT_INFO_DATA		, 104, 104)
//DECL_GRES_HASH(GET_S2C_AGENT_RECVCRYPT_INFO_DATA		, 105, 105)



#undef DECL_GRES_HASH
//};


#if defined(_SERVER)
#elif defined(WISECLIENT_CORE_EXPORTS)
#else //CLIENT
#endif //


