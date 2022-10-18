#ifndef _PACKETSTRUCT_GM_H_
#define _PACKETSTRUCT_GM_H_

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_GM.h>

#pragma pack(push,1)


// DB 프락시 서버의 정보를 보내준다.
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

// 데이터를 리로드 하도록 명령한다.
struct MSG_GM_RELOAD_DATA_CMD : public MSG_BASE
{
};

// 현재 서버의 통화량 정리
struct MSG_GM_USERMONEY_NTF : public MSG_BASE_FORWARD
{
	ULONGLONG		ui64TotalGetMoney;			// 일정 시간 동안 생겨난 돈 총계
	ULONGLONG		ui64TotalLoseMoney;			// 일정 시간 동안 없어진 돈 총계
	ULONGLONG		ui64TotalUserMoney;			// 현재 동접 유저들이 갖고 있는 평균 금액
};

// 게임 통계
struct MSG_GM_OPERATION_STATISTICS_REQ : public MSG_BASE
{
	MSG_GM_OPERATION_STATISTICS_REQ() { m_byCategory = GM_OPERATION; m_byProtocol = GM_OPERATION_STATISTICS_REQ; }
};

struct MSG_GM_OPERATION_STATISTICS_ANS : public MSG_BASE
{
	MSG_GM_OPERATION_STATISTICS_ANS() { m_byCategory = GM_OPERATION; m_byProtocol = GM_OPERATION_STATISTICS_ANS; }

	ULONGLONG	ui64UserMoney;
	ULONGLONG	ui64WarehouseAveMoney;		// 창고 평균 금액

	ULONGLONG	ui64StatisticsType[41];		// 전체 통계(eSTATISTICS_TYPE 참조)
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

	ULONGLONG	ui64WarehouseAveMoney;		// 창고 평균 금액
	ULONGLONG	ui64WarehousePlusMoney;		// 창고에 들어간 돈
	ULONGLONG	ui64WarehouseMinusMoney;	// 창고에서 나간 돈

	USHORT		usJackpotCnt;				// 잭팟 터진 횟수
	ULONGLONG	JackpotMoney;				// 잭팟 금액 합계
};


/*
struct STRUCT_RESTRICT
{
	DWORD	dwFunc;
	BOOL	bAllow;
};
*/
// 특정 기능 사용 금지
struct MSG_GM_OPERATION_FUNCRESTRICT_CMD : public MSG_BASE
{
	enum { MAX_FUNC_COUNT = 255, };

	MSG_GM_OPERATION_FUNCRESTRICT_CMD() { m_byCategory = GM_OPERATION; m_byProtocol = GM_OPERATION_FUNCRESTRICT_CMD; }

	BYTE				byRestrictNum;				// 금지할 기능 개수
	STRUCT_RESTRICT		SRESTRICT[MAX_FUNC_COUNT];

	int GetSize() { return ( sizeof(MSG_GM_OPERATION_FUNCRESTRICT_CMD) - ( sizeof(STRUCT_RESTRICT) * ( MAX_FUNC_COUNT - byRestrictNum ) ) ); }
};

/*
//////////////////////////////////
// 종료 패킷

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