//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	기본 포맷
//	typedef uREG_PV (*IGResHandler)( uREG_NODE* );
//	<ARGUMENTS>		uREG_NODE	: { ARG0, ARG1, ARG2, ARG3 }
//	<ACTION>		(~~)
//	<RETURN>		uREG_NODE	: { ... }

// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		(NONE)
// <RETURN>		uREG_NODE	: { NULL, NULL, NULL, NULL }
DECL_GRES_HASH(EMPTY_CODE								, 001, 001)
// <ARGUMENTS>	uREG_NODE	: { BYTE*, NULL, NULL, NULL }
// <ACTION>		ARG0.PTRB[0] = HIGH, ARG0.PTRB[1] = MIDDLE, ARG0.PTRB[2] = LOW
// <RETURN>		uREG_NODE	: uRET { TRUE }
DECL_GRES_HASH(GET_PROTOCOL_VERSION_A					, 002, 002)
// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.EXA[0] = HIGH, uRET.EXA[1] = MIDDLE, uRET.EXA[2] = LOW, uRET.EXA[3] = RGCG250
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(GET_PROTOCOL_VERSION_B					, 002, 003)
// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.EX = CryptManager::GetMolaProtocolCRC()
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(GET_MOLA_PROTOCOL_CRC					, 003, 001)
// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.EX = NATION_CODE+CATEGORY_ODD_NUMBER
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(GET_NATION_CODE_A						, 003, 003)
// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.EX = NATION_CODE+PROTOCOL_ODD_NUMBER
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(GET_NATION_CODE_B						, 004, 004)
// <ARGUMENTS>	uREG_NODE	: { 별도 정의 }
// <ACTION>		(별도 정의)
// <RETURN>		uREG_NODE	: { 별도 정의 }
DECL_GRES_HASH(CALCULATION_T1							, 005, 005)
// <ARGUMENTS>	uREG_NODE	: { 별도 정의 }
// <ACTION>		(별도 정의)
// <RETURN>		uREG_NODE	: { 별도 정의 }
DECL_GRES_HASH(CALCULATION_T2							, 006, 006)
// <ARGUMENTS>	uREG_NODE	: { 별도 정의 }
// <ACTION>		(별도 정의)
// <RETURN>		uREG_NODE	: { 별도 정의 }
DECL_GRES_HASH(CALCULATION_T3							, 007, 007)
// <ARGUMENTS>	uREG_NODE	: { 별도 정의 }
// <ACTION>		(별도 정의)
// <RETURN>		uREG_NODE	: { 별도 정의 }
DECL_GRES_HASH(CALCULATION_T4							, 008, 008)
// <ARGUMENTS>	uREG_NODE	: { 별도 정의 }
// <ACTION>		(별도 정의)
// <RETURN>		uREG_NODE	: { 별도 정의 }
DECL_GRES_HASH(CALCULATION_T5							, 009, 009)
// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.EX = GetTickCount()
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(GET_TICK_COUNT							, 011, 011)
// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.PTR = ([GAMESERVER][SEND_INFO].CProtocolCryptography *)
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(NETWORK_GET_GAME_SEND_CLIENT_CRYPT_INFO	, 012, 012)
// <ARGUMENTS>	uREG_NODE	: { NULL, NULL, NULL, NULL }
// <ACTION>		uRET.PTR = ([GAMESERVER][RECV_INFO].CProtocolCryptography *)
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(NETWORK_GET_GAME_RECV_CLIENT_CRYPT_INFO	, 012, 013)
// <ARGUMENTS>	uREG_NODE	: { PVOID, WORD, NULL, NULL }
// <ACTION>		uRET.EX = GlobalFunc::SendPacket( CI_GAMESERVERIDX, pARGs->EXA[0].PTR, pARGs->EXA[1].EX );
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(NETWORK_SENDPACKET_TO_GAMESERVER			, 014, 014)
// <ARGUMENTS>	uREG_NODE	: { CHAR buffer[17], WORD, NULL, NULL }
// <ACTION>		strncpy( pARG->PTRC, (CHARNAME*), 16 ); pARG->PTRC[16] = '\0';
// <RETURN>		uREG_NODE	: uRET
DECL_GRES_HASH(GET_USER_ID								, 015, 015)
//
//DECL_GRES_HASH(GET_C2S_GAME_SENDCRYPT_INFO_DATA			, 100, 100)
//DECL_GRES_HASH(GET_C2S_GAME_RECVCRYPT_INFO_DATA			, 101, 101)
//DECL_GRES_HASH(GET_CHAR_MOVSPD_IN_DATA					, 102, 102)
//DECL_GRES_HASH(GET_CHAR_MOVSPD_IN_GAME					, 103, 103)
//DECL_GRES_HASH(GET_S2C_AGENT_SENDCRYPT_INFO_DATA		, 104, 104)
//DECL_GRES_HASH(GET_S2C_AGENT_RECVCRYPT_INFO_DATA		, 105, 105)