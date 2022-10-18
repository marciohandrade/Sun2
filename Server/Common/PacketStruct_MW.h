#pragma once

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_MW.h>

#pragma pack(push,1)

#define PACKET_MW_OPEN(c, p)    struct MSG_##p : public MSG_BASE_FORWARD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_MW_CLOSE     };

#define PACKET_MW_INHERIT_OPEN(c, p, b) struct MSG_##p : public b { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_MW_INHERIT_CLOSE     };


// ������ ������ ���� ������ ���� ���
PACKET_MW_OPEN(MW_OPERATION, MW_NOTICE_CMD)
    DWORD dwServerUID;                    //  ������ ������ ������ uid.(0=��ü)
    DWORD dwTickPerLine;                  //  1���� ��� �ð�����(ms)
    DWORD dwNoticeLength;                 //  ���� ���ڿ��� ����
    // CHANGES: f110812.1L, change layout order
    BYTE  m_byRed;
    BYTE  m_byGreen;
    BYTE  m_byBlue;

    char  szNotice[MAX_NOTICE_LEN + 1];     //  ���� ���ڿ�

    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_MW_CLOSE;

PACKET_MW_OPEN(MW_CONNECTION, MW_SERVER_INFO_CMD)
    SERVER_KEY ServerKey;

    char m_szWorldServerIP[MAX_IP_STRING_LEN + 1];
    WORD m_wWorldServerPort;
    char m_szWorldServerInnerIP[MAX_IP_STRING_LEN + 1];
    WORD m_wWorldServerInnerPort;
    char m_szGameDBProxyServerIP[MAX_IP_STRING_LEN + 1];
    WORD m_wGameDBProxyServerPort;
PACKET_MW_CLOSE;

// Reference
//struct CHANNEL_USERCOUNT
//{
//  DWORD   dwChannelID;
//  DWORD   dwVillageCount;
//  DWORD   dwVillagePCRoomCount;
//  DWORD   dwBattleZoneCount;
//  DWORD   dwBattleZonePCRoomCount;
//};

PACKET_MW_OPEN(MW_OPERATION, MW_USERCOUNT_SYN)
    enum { MAX_CHANNEL_LIST = 50, };
    BYTE byChannelCount;
    CHANNEL_USERCOUNT ChannelInfo[MAX_CHANNEL_LIST];
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(ChannelInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(ChannelInfo[0]) * byChannelCount);
    };
PACKET_MW_CLOSE;

PACKET_MW_OPEN(MW_OPERATION, MW_BATTLEZONEINFO_REQ)
    uint16_t GetSize() const {
        return sizeof(*this);
    };
PACKET_MW_CLOSE;

PACKET_MW_OPEN(MW_OPERATION, MW_BATTLEZONEINFO_ANS)
    enum { MAX_CHANNEL_NUM = 256, };
    BYTE byCount; // ä�� ����
    STRUCT_BATTLEZONE_INFO ChannelInfo[MAX_CHANNEL_NUM]; // ä�δ� ��Ʋ�� ����

    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(ChannelInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(ChannelInfo[0]) * byCount);
    };
PACKET_MW_CLOSE;

PACKET_MW_OPEN(MW_OPERATION, MW_USERMONEY_SYN)
    DWORD dwTotalGetMoney;  // ���� �ð� ���� ���ܳ� �� �Ѱ�
    DWORD dwTotalLoseMoney; // ���� �ð� ���� ������ �� �Ѱ�
    DWORD dwTotalUserMoney; // ���� ���� �������� ���� �ִ� ��� �ݾ�
PACKET_MW_CLOSE;

//////////////////////////////////
// Armorkit ���� ��Ŷ
// CHANGES: f110812.1L, remove the unused section
//struct MSG_MW_ARMORKIT_FINDHACK_SYN : public MSG_BASE
//{
//	MSG_MW_ARMORKIT_FINDHACK_SYN() { m_byCategory = MW_ARMORKIT; m_byProtocol = MW_ARMORKIT_FINDHACK_SYN; }
//
//	DWORD	dwGuid;			// ��ŷ�� ������ Ŭ���̾�Ʈ guid
//	TCHAR	szIP[16];		// ��ŷ�� ������ Ŭ���̾�Ʈ IP�ּ�
//	int		HackType;		// Ž���� ��ŷ Ÿ��  (e.g. AKDETECT_SPEEDHACK)
//	int		Size;			// Buffer �� ����
//	TCHAR	Buf[1024];		// ������ ����-1024�� ������Ʈ���� ���޹��� �Ϲ��� �ִ밪(����â)
//};

/*
//////////////////////////////////
// ���� ��Ŷ

struct MSG_MW_OPERATION_SERVERSHUTDOWN_REQ : public MSG_BASE
{
	MSG_MW_OPERATION_SERVERSHUTDOWN_REQ() { m_byCategory = MW_OPERATION; m_byProtocol = MW_OPERATION_SERVERSHUTDOWN_REQ; }
};

struct MSG_MW_OPERATION_SERVERSHUTDOWN_ANS : public MSG_BASE
{
	MSG_MW_OPERATION_SERVERSHUTDOWN_ANS() { m_byCategory = MW_OPERATION; m_byProtocol = MW_OPERATION_SERVERSHUTDOWN_ANS; }
};
*/
//==================================================================================================

//__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__ interface
PACKET_MW_OPEN(MW_CONNECTION, MW_CONNECTION_DISCONNECT_CMD)
PACKET_MW_CLOSE;

//__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__ interface
PACKET_MW_OPEN(MW_CONNECTION, MW_CONNECTION_PHASE_SHIFT_CMD)
    DWORD m_dwPHASE_SHIFT;
PACKET_MW_CLOSE;

//__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__ interface
PACKET_MW_OPEN(MW_CONNECTION, MW_CONNECTION_BLOCK_CMD)
PACKET_MW_CLOSE;

//==================================================================================================

PACKET_MW_OPEN(MW_OPERATION, MW_MULTIMEMO_SEND_SYN)
    //TCHAR* GetMeMo() {return m_MMemoInfo.m_szMessage;}
    MMEMO_INFOS m_MMemoInfo;
PACKET_MW_CLOSE;

PACKET_MW_OPEN(MW_OPERATION, MW_MULTIMEMO_SEND_ACK)
    DWORD m_dwCount; // ���� ���� ��� ����
    MMEMO_ERR_RESULT m_arrResultInfoList[MMEMO_ERR_RESULT::MAX_RESULT_INFO]; // ���� ���� ��� ������ 
    // NOTE: f110812.1L, need 'GetSize' interface
PACKET_MW_CLOSE;

#pragma pack(pop)
