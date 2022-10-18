#pragma once

#include <PacketStruct.h>
#include <SunAuth/AuthProtocol.h>

#pragma pack(push,1)


struct MSG_BASE_BILLING
{
	WORD				m_wPacketSize;
	DWORD				m_dwProtocolID;
};

#define PACKET_BILLING_OPEN( ID )	struct MSG_##ID : public MSG_BASE_BILLING	{ MSG_##ID(){ m_dwProtocolID = ID; m_wPacketSize = 0; }
#define PACKET_BILLING_CLOSE	};


// BILLING 통신
enum ePROTOCOL_BILLING
{
//#ifdef _NEW_BILLING_SYSTEM
//
//	SB_BILLING_CLIENT_LOGIN_CMD		= 0x01,
//	BS_BILLING_USER_STATUS_NTF		= 0x02,
//	SB_BILLING_CLIENT_LOGOUT_CMD	= 0x03,
//	SB_BILLING_CLIENT_INQURE_SYN	= 0x04,
//
//#else

	SB_BILLING_DEDUCTION_CMD		= 0x0301, // 빌링 (분/포인트) 차감
	BS_BILLING_EXPIRED_NTF			= 0x0302, // 빌링 (분/포인트) 만료
	SB_BILLING_USER_LOGIN_CMD		= 0x0303, // 유저 로그인 (차감 시작)
	SB_BILLING_USER_LOGOUT_CMD		= 0x0304, // 유저 로그아웃 (차감 종료)
	BS_BILLING_EXPIRED_ALRAM_NTF	= 0x0305, // 빌링 (분/포인트) 만료 임박 통지
    // _PCBANG_POLICY_CHANGE_POST_EXPIRED2
    BS_BILLING_RESTPOINT_SYN        = 0x0306, // 해당 (유저+PC방 ID)의 잔여 포인트 요청
    BS_BILLING_RESTPOINT_ACK        = 0x0307, // 해당 (유저+PC방 ID)의 잔여 포인트 결과

//#endif
};


//#ifdef _NEW_BILLING_SYSTEM
//
//PACKET_BILLING_OPEN( SB_BILLING_CLIENT_LOGIN_CMD )
//	DWORD 				m_dwAccountGUID;
//	DWORD				m_dwIPAddress;
//	DWORD 				m_dwRoomGUID;
//	DWORD				m_dwGameCode;				// 게임 코드
//	DWORD				m_dwServerType;				// 테섭인지 라이브인지 구분 코드	
//PACKET_BILLING_CLOSE
//
//PACKET_BILLING_OPEN( BS_BILLING_USER_STATUS_NTF )
//	DWORD 				m_dwAccountGUID;
//	long				m_dwBillingGUID;
//	DWORD				m_RealEndDate;				// 실제 종료일자(정액)
//	DWORD				m_EndDate;					// 결재 종료일자(정액)
//	double				m_dRestPoint;				// 잔여포인트(정량)
//	double				m_dRestTime;				// 시간 Sec(정량)
//	short				m_sDeductType;				// BILLING_DEDUCTION_TYPE
//	short				m_sAccessCheck;				// 접근 체크 결과
//	short				m_sResultCode;
//PACKET_BILLING_CLOSE
//
//PACKET_BILLING_OPEN( SB_BILLING_CLIENT_LOGOUT_CMD )
//	long				m_dwBillingGUID;			// 로그인 결과로 받은 GUID
//PACKET_BILLING_CLOSE
//
//PACKET_BILLING_OPEN( SB_BILLING_CLIENT_INQURE_SYN )
//	long				m_dwBillingGUID;			// 로그인 결과로 받은 GUID
//PACKET_BILLING_CLOSE
//
//#else //!_NEW_BILLING_SYSTEM

PACKET_BILLING_OPEN( SB_BILLING_DEDUCTION_CMD )
	enum { MAX_DEDUCTION_SIZE = 3000 };
	DWORD					m_Count;
	BILLING_DEDUCTION_INFO	m_DeductionInfoList[MAX_DEDUCTION_SIZE];

    void Clear() {
        new (this) MSG_SB_BILLING_DEDUCTION_CMD;
        m_Count = 0;
    }

	int GetSize()
	{
		return sizeof(*this) - (MAX_DEDUCTION_SIZE-m_Count)*sizeof(BILLING_DEDUCTION_INFO);
	}
PACKET_BILLING_CLOSE

PACKET_BILLING_OPEN( BS_BILLING_EXPIRED_NTF )
	DWORD				m_dwGameCode;
	DWORD				m_dwUserGUID;
	DWORD				m_dwPCRoomGUID;
	DWORD				m_dwDeductionType;			// BILLING_DEDUCTION_TYPE
	DWORD				m_dwResult;					// 0 : 차감만료
PACKET_BILLING_CLOSE

PACKET_BILLING_OPEN( SB_BILLING_USER_LOGIN_CMD )
	DWORD				m_dwGameCode;
	DWORD				m_dwUserGUID;
	DWORD				m_dwPCRoomGUID; 
	DWORD				m_dwIPAddress;
	DWORD				m_dwPlayTime;				// 차감 시간 (sec)
	DWORD				m_dwResult;
PACKET_BILLING_CLOSE

PACKET_BILLING_OPEN( SB_BILLING_USER_LOGOUT_CMD )
	DWORD				m_dwGameCode; 
	DWORD				m_dwUserGUID;
	DWORD				m_dwPCRoomGUID;
	DWORD				m_dwIPAddress;
	DWORD				m_dwPlayTime;
	DWORD				m_dwResult;
PACKET_BILLING_CLOSE

PACKET_BILLING_OPEN( BS_BILLING_EXPIRED_ALRAM_NTF )
	DWORD				m_dwGameCode;
	DWORD				m_dwUserGUID;
	DWORD				m_dwPCRoomGUID;
	DWORD				m_dwDeductType;
	DWORD				m_dwRestTime;				// 남은 시간 (sec)
PACKET_BILLING_CLOSE

//==================================================================================================
//_PCBANG_POLICY_CHANGE_POST_EXPIRED2
PACKET_BILLING_OPEN(BS_BILLING_RESTPOINT_SYN) // 잔여 시간 (또는 포인트) 요청
    DWORD   game_code_;
    DWORD   user_guid_;
    DWORD   pc_room_id_;
PACKET_BILLING_CLOSE

PACKET_BILLING_OPEN(BS_BILLING_RESTPOINT_ACK) // 잔여 시간 (또는 포인트) 결과
    DWORD   game_code_;
    DWORD   user_guid_;
    DWORD   pc_room_id_;
    DWORD   deduction_type_; // ??
    DWORD   rest_point_;     // 잔여 시간(초) 또는 잔여 포인트 : 그냥 0이 아니면으로 평가하자.
PACKET_BILLING_CLOSE
//==================================================================================================

//#endif //!_NEW_BILLING_SYSTEM




#pragma pack(pop)








