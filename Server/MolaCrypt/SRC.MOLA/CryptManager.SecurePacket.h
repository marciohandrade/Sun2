#pragma once

// =======================================================================================

//////////////////////////////////////////////////////////////////////////
// <MODULE VARIABLEs>
//static BYTE*	
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// [0]~[63] IWorkNode [64]~ MSGINFO

const DWORD		MAX_QUERY_PACKET_SIZE	= 64;
const DWORD		MAX_ANSWER_PACKET_SIZE	= 8;

#pragma pack(push,1)

union uDWORD_TOKEN
{
	DWORD	uDWORD;
	BYTE	uBYTES[sizeof(DWORD)];
};

union uHEX_TOKEN
{
	DWORD	uDWORDS[4];
	BYTE	uBYTES[sizeof(DWORD)*4];
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	<1010 PACKET BASE PROTOTYPEs>
struct MSG_SECURE_BASE : public MSG_BASE
{
//	sSECURE_PACKET_INFO_BASE	m_BASE;
};



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	<1010 PACKET STRUCTUREs>
#define PACKET_SECURE_OPEN( c, p )	struct MSGINTER_##p : public MSG_SECURE_BASE { MSGINTER_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_SECURE_CLOSE		};


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	<5000 ADDIN PROTOCOLs>
PACKET_SECURE_OPEN(CG_SYNC, CG_SECURE_POLICY_PATTERN01_SIMPLEX_SNK_CMD)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_SYNC, CG_SECURE_POLICY_PATTERN01_SIMPLEX_SNK_ACK)
PACKET_SECURE_CLOSE

//////////////////////////////////////////////////////////////////////////
PACKET_SECURE_OPEN(CG_SYNC, CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_CMD)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_SYNC, CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_ACK)
PACKET_SECURE_CLOSE

//////////////////////////////////////////////////////////////////////////
PACKET_SECURE_OPEN(CG_ITEM, CG_SECURE_POLICY_PATTERN03_SIMPLEX_SNK_CMD)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_ITEM, CG_SECURE_POLICY_PATTERN03_SIMPLEX_SNK_ACK)
PACKET_SECURE_CLOSE

//////////////////////////////////////////////////////////////////////////
PACKET_SECURE_OPEN(CG_TRADE, CG_SECURE_POLICY_PATTERN04_SIMPLEX_SNK_CMD)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_TRADE, CG_SECURE_POLICY_PATTERN04_SIMPLEX_SNK_ACK)
PACKET_SECURE_CLOSE

//////////////////////////////////////////////////////////////////////////
PACKET_SECURE_OPEN(CG_VENDOR, CG_SECURE_POLICY_PATTERN05_QUERY_MOLA_CMD)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_VENDOR, CG_SECURE_POLICY_PATTERN05_QUERY_MOLA_ACK)
PACKET_SECURE_CLOSE

//////////////////////////////////////////////////////////////////////////
PACKET_SECURE_OPEN(CG_WAREHOUSE, CG_SECURE_POLICY_PATTERN06_CONFIGURE_MOLA_CMD)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_WAREHOUSE, CG_SECURE_POLICY_PATTERN06_CONFIGURE_MOLA_ACK)
PACKET_SECURE_CLOSE

//////////////////////////////////////////////////////////////////////////
PACKET_SECURE_OPEN(CG_PARTY, CG_SECURE_POLICY_PATTERN07_CONFIGURE_HEARTBEAT_CMD)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_PARTY, CG_SECURE_POLICY_PATTERN07_CONFIGURE_HEARTBEAT_ACK)
PACKET_SECURE_CLOSE

//////////////////////////////////////////////////////////////////////////
PACKET_SECURE_OPEN(CG_STATUS, CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_KEY)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_STATUS, CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_ACK)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_STATUS, CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_CMD)
PACKET_SECURE_CLOSE


PACKET_SECURE_OPEN(CG_CONNECTION, CG_SECURE_QUERY_BUILTIN_CHK_CMD)
	BYTE	m_dwBuiltinCMD[8/* +a */];
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_CONNECTION, CG_SECURE_QUERY_BUILTIN_CHK_ACK)
	BYTE	m_dwBuiltinACK[8/* +b */];
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_CONNECTION, CG_SECURE_QUERY_RESOURCE_CHK_T1_CMD)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_CONNECTION, CG_SECURE_QUERY_RESOURCE_CHK_T1_ACK)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_CHARINFO, CG_SECURE_QUERY_RESOURCE_CHK_T2_CMD)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_CHARINFO, CG_SECURE_QUERY_RESOURCE_CHK_T2_ACK)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_MAP, CG_SECURE_QUERY_RESOURCE_CHK_NTF1_SYN)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_BATTLE, CG_SECURE_QUERY_RESOURCE_CHK_T3_CMD)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_BATTLE, CG_SECURE_QUERY_RESOURCE_CHK_T3_ACK)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_STYLE, CG_SECURE_QUERY_RESOURCE_CHK_T4_CMD)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_STYLE, CG_SECURE_QUERY_RESOURCE_CHK_T4_ACK)
PACKET_SECURE_CLOSE

PACKET_SECURE_OPEN(CG_STYLE, CG_SECURE_QUERY_RESOURCE_CHK_NTF2_SYN)
PACKET_SECURE_CLOSE


#pragma pack(pop)



