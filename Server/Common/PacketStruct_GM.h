#ifndef _PACKETSTRUCT_GM_H_
#define _PACKETSTRUCT_GM_H_

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_GM.h>

#pragma pack(push,1)


// DB ������ ������ ������ �����ش�.
struct MSG_GM_CONNECTION_SERVER_INFO_CMD : public MSG_BASE
{
	SERVER_KEY				m_ServerKey;
	DWORD					m_dwServerGUID;
	CHAR					m_szDBProxyServerIP[MAX_IP_STRING_LEN+1];
	WORD					m_wDBProxyServerPort;
	CHAR					m_szAgentServerIP[MAX_IP_STRING_LEN+1];
	WORD					m_wAgentServerPort;
	CHAR					m_szGuildServerIP[MAX_IP_STRING_LEN+1];
	WORD					m_wGuildServerPort;
	CHAR					m_szPortalServerIP[MAX_IP_STRING_LEN+1];
	WORD					m_wPortalServerPort;
	CHAR					m_szShopServerIP[MAX_IP_STRING_LEN+1];
	WORD					m_wShopServerPort;
};

// �����͸� ���ε� �ϵ��� ����Ѵ�.
struct MSG_GM_RELOAD_DATA_CMD : public MSG_BASE
{
};

// ���� ������ ��ȭ�� ����
struct MSG_GM_USERMONEY_NTF : public MSG_BASE_FORWARD
{
	ULONGLONG		ui64TotalGetMoney;			// ���� �ð� ���� ���ܳ� �� �Ѱ�
	ULONGLONG		ui64TotalLoseMoney;			// ���� �ð� ���� ������ �� �Ѱ�
	ULONGLONG		ui64TotalUserMoney;			// ���� ���� �������� ���� �ִ� ��� �ݾ�
};

// ���� ���
struct MSG_GM_OPERATION_STATISTICS_REQ : public MSG_BASE
{
	MSG_GM_OPERATION_STATISTICS_REQ() { m_byCategory = GM_OPERATION; m_byProtocol = GM_OPERATION_STATISTICS_REQ; }
};

struct MSG_GM_OPERATION_STATISTICS_ANS : public MSG_BASE
{
	MSG_GM_OPERATION_STATISTICS_ANS() { m_byCategory = GM_OPERATION; m_byProtocol = GM_OPERATION_STATISTICS_ANS; }

	ULONGLONG	ui64UserMoney;
	ULONGLONG	ui64WarehouseAveMoney;		// â�� ��� �ݾ�

	ULONGLONG	ui64StatisticsType[41];		// ��ü ���(eSTATISTICS_TYPE ����)
};

struct MSG_GM_OPERATION_USERMONEY_REQ : public MSG_BASE
{
	MSG_GM_OPERATION_USERMONEY_REQ() { m_byCategory = GM_OPERATION; m_byProtocol = GM_OPERATION_USERMONEY_REQ; }
};

struct MSG_GM_OPERATION_USERMONEY_ANS : public MSG_BASE
{
	MSG_GM_OPERATION_USERMONEY_ANS() { m_byCategory = GM_OPERATION; m_byProtocol = GM_OPERATION_USERMONEY_ANS; }

	ULONGLONG	ui64UserMoney;
	ULONGLONG	ui64TotalPlusMoney;
	ULONGLONG	ui64TotalMinusMoney;

	ULONGLONG	ui64WarehouseAveMoney;		// â�� ��� �ݾ�
	ULONGLONG	ui64WarehousePlusMoney;		// â�� �� ��
	ULONGLONG	ui64WarehouseMinusMoney;	// â���� ���� ��

	USHORT		usJackpotCnt;				// ���� ���� Ƚ��
	ULONGLONG	JackpotMoney;				// ���� �ݾ� �հ�
};


/*
struct STRUCT_RESTRICT
{
	DWORD	dwFunc;
	BOOL	bAllow;
};
*/
// Ư�� ��� ��� ����
struct MSG_GM_OPERATION_FUNCRESTRICT_CMD : public MSG_BASE
{
	enum { MAX_FUNC_COUNT = 255, };

	MSG_GM_OPERATION_FUNCRESTRICT_CMD() { m_byCategory = GM_OPERATION; m_byProtocol = GM_OPERATION_FUNCRESTRICT_CMD; }

	BYTE				byRestrictNum;				// ������ ��� ����
	STRUCT_RESTRICT		SRESTRICT[MAX_FUNC_COUNT];

	int GetSize() { return ( sizeof(MSG_GM_OPERATION_FUNCRESTRICT_CMD) - ( sizeof(STRUCT_RESTRICT) * ( MAX_FUNC_COUNT - byRestrictNum ) ) ); }
};

/*
//////////////////////////////////
// ���� ��Ŷ

struct MSG_GM_OPERATION_SERVERSHUTDOWN_REQ : public MSG_BASE_FORWARD
{
};

struct MSG_GM_OPERATION_SERVERSHUTDOWN_ANS : public MSG_BASE_FORWARD
{
};
*/

#define PACKET_GM_OPEN( c, p )			\
struct MSG_##p : public MSG_BASE	\
{	MSG_##p() { m_byCategory = c; m_byProtocol = p; }

#define PACKET_GM_CLOSE		};

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

PACKET_GM_OPEN( GM_CONNECTION, GM_CONNECTION_DISCONNECT_CMD )
PACKET_GM_CLOSE

PACKET_GM_OPEN( GM_CONNECTION, GM_CONNECTION_PHASE_SHIFT_CMD )
DWORD	m_dwPHASE_SHIFT;
PACKET_GM_CLOSE

PACKET_GM_OPEN( GM_CONNECTION, GM_CONNECTION_BLOCK_CMD )
PACKET_GM_CLOSE

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

//__NA000000_070523_REPORT_TO_SOLARAUTH
PACKET_GM_OPEN( GM_OPERATION, GM_OPERATION_REPORT_CACHEITEM_PURCHASE_NTF )
	sCACHEITEM_REPORT_INFO	m_INFO;
PACKET_GM_CLOSE


#pragma pack(pop)

#endif // _PACKETSTRUCT_GM_H_