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


// BILLING ���
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

	SB_BILLING_DEDUCTION_CMD		= 0x0301, // ���� (��/����Ʈ) ����
	BS_BILLING_EXPIRED_NTF			= 0x0302, // ���� (��/����Ʈ) ����
	SB_BILLING_USER_LOGIN_CMD		= 0x0303, // ���� �α��� (���� ����)
	SB_BILLING_USER_LOGOUT_CMD		= 0x0304, // ���� �α׾ƿ� (���� ����)
	BS_BILLING_EXPIRED_ALRAM_NTF	= 0x0305, // ���� (��/����Ʈ) ���� �ӹ� ����
    // _PCBANG_POLICY_CHANGE_POST_EXPIRED2
    BS_BILLING_RESTPOINT_SYN        = 0x0306, // �ش� (����+PC�� ID)�� �ܿ� ����Ʈ ��û
    BS_BILLING_RESTPOINT_ACK        = 0x0307, // �ش� (����+PC�� ID)�� �ܿ� ����Ʈ ���

//#endif
};


//#ifdef _NEW_BILLING_SYSTEM
//
//PACKET_BILLING_OPEN( SB_BILLING_CLIENT_LOGIN_CMD )
//	DWORD 				m_dwAccountGUID;
//	DWORD				m_dwIPAddress;
//	DWORD 				m_dwRoomGUID;
//	DWORD				m_dwGameCode;				// ���� �ڵ�
//	DWORD				m_dwServerType;				// �׼����� ���̺����� ���� �ڵ�	
//PACKET_BILLING_CLOSE
//
//PACKET_BILLING_OPEN( BS_BILLING_USER_STATUS_NTF )
//	DWORD 				m_dwAccountGUID;
//	long				m_dwBillingGUID;
//	DWORD				m_RealEndDate;				// ���� ��������(����)
//	DWORD				m_EndDate;					// ���� ��������(����)
//	double				m_dRestPoint;				// �ܿ�����Ʈ(����)
//	double				m_dRestTime;				// �ð� Sec(����)
//	short				m_sDeductType;				// BILLING_DEDUCTION_TYPE
//	short				m_sAccessCheck;				// ���� üũ ���
//	short				m_sResultCode;
//PACKET_BILLING_CLOSE
//
//PACKET_BILLING_OPEN( SB_BILLING_CLIENT_LOGOUT_CMD )
//	long				m_dwBillingGUID;			// �α��� ����� ���� GUID
//PACKET_BILLING_CLOSE
//
//PACKET_BILLING_OPEN( SB_BILLING_CLIENT_INQURE_SYN )
//	long				m_dwBillingGUID;			// �α��� ����� ���� GUID
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
	DWORD				m_dwResult;					// 0 : ��������
PACKET_BILLING_CLOSE

PACKET_BILLING_OPEN( SB_BILLING_USER_LOGIN_CMD )
	DWORD				m_dwGameCode;
	DWORD				m_dwUserGUID;
	DWORD				m_dwPCRoomGUID; 
	DWORD				m_dwIPAddress;
	DWORD				m_dwPlayTime;				// ���� �ð� (sec)
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
	DWORD				m_dwRestTime;				// ���� �ð� (sec)
PACKET_BILLING_CLOSE

//==================================================================================================
//_PCBANG_POLICY_CHANGE_POST_EXPIRED2
PACKET_BILLING_OPEN(BS_BILLING_RESTPOINT_SYN) // �ܿ� �ð� (�Ǵ� ����Ʈ) ��û
    DWORD   game_code_;
    DWORD   user_guid_;
    DWORD   pc_room_id_;
PACKET_BILLING_CLOSE

PACKET_BILLING_OPEN(BS_BILLING_RESTPOINT_ACK) // �ܿ� �ð� (�Ǵ� ����Ʈ) ���
    DWORD   game_code_;
    DWORD   user_guid_;
    DWORD   pc_room_id_;
    DWORD   deduction_type_; // ??
    DWORD   rest_point_;     // �ܿ� �ð�(��) �Ǵ� �ܿ� ����Ʈ : �׳� 0�� �ƴϸ����� ������.
PACKET_BILLING_CLOSE
//==================================================================================================

//#endif //!_NEW_BILLING_SYSTEM




#pragma pack(pop)








