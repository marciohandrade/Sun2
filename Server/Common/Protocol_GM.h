#ifndef __PROTOCOL_GM_H__
#define __PROTOCOL_GM_H__

//-------------------------------------------------------------------------------------------------------
// *  Prefix
//		C : Client
//		G : Game Server
//		M : Master Server
//		D : DBP Server
//
// * Postfix
//		SYN - Server�� ��û
//		ACK - ��û�� ���� ���
//		NAK - ��û�� ���� �ź�
//		CMD - Server�� ���
//		BRD - Server�� ����ȭ ���� ������ �˸�
//		DBR - DBProxy������ �����!
//
// * �������� Naming Rule
//		Prefix_ī�װ���_�������ݸ�_Postfix
//		ex) CG_CONNECTION_REGISTER_SYN
//
//-------------------------------------------------------------------------------------------------------


// GameServer <-> MasterServer Protocol


enum eGM_CATEGORY
{
	GM_CONNECTION		= 1,
	GM_OPERATION,
	GM_MAX,
};


// CONNECTION = 1,		// ���� ����

enum ePROTOCOL_GM_CONNECTION
{
	GM_CONNECTION_SERVER_INFO_CMD		= 0x01,

	GM_CONNECTION_DISCONNECT_CMD	= 200,
	GM_CONNECTION_PHASE_SHIFT_CMD	= 201,
	GM_CONNECTION_BLOCK_CMD			= 202,
    GM_CONNECTION_GROUND_CMD, // NOTE: f110617.1L
};

// OTHER, 	// ��Ÿ

enum ePROTOCOL_GM_OPERATION
{
	GM_RELOAD_DATA_CMD,
	GM_USERCOUNT_SYN,
	GM_USERMONEY_NTF,

	GM_OPERATION_USERMONEY_REQ,	// ��ȭ�� ��û
	GM_OPERATION_USERMONEY_ANS,	// ��ȭ�� ����

//	GM_OPERATION_SERVERSHUTDOWN_REQ,
//	GM_OPERATION_SERVERSHUTDOWN_ANS,

	GM_OPERATION_FUNCRESTRICT_CMD,	// Ư�� ��� �����ϵ��� ���

	GM_OPERATION_STATISTICS_REQ,	// ���� ��� ����
	GM_OPERATION_STATISTICS_ANS,

	//__NA000000_070523_REPORT_TO_SOLARAUTH
	GM_OPERATION_REPORT_CACHEITEM_PURCHASE_NTF,
};


#endif // __PROTOCOL_GM_H__


