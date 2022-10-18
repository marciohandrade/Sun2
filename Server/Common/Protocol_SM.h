
#ifndef __PROTOCOL_SM_H__
#define __PROTOCOL_SM_H__

#include <Protocol.h>

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


// Solar Auth Server�� ���Ȼ��� ������ �����Ͱ� Passive���� �����ϵ��� �����Ѵ�.
// ��, SM���� ��û�ϴ� ��ü�� (S)olar auth server�� �Ǵ� ��찡 ����.
// _CMD �� �ǰڱ�...

// SM���� �� �����ʹ� �������� ������ �޶��� �� �ִ�. random number�� �ϱ⿡�� ������ �ִ�.
// ������ ������ ������ �����ؾ� �� ���̴�.

enum eSM_CATEGORY
{
	SM_CONNECTION		= 11,
	SM_SERVER_INFO		= 81,
	SM_CRYPTOGRAPHY		= 161,
	SM_DATA_TRANSFER	= 221,
};

enum eSM_CONNECTION_PROTOCOL
{
	SM_CONNECTION_HEARTBEAT			= 92,

	// ù��° ���� �ܰ�
	SM_CONNECTION_CONNECTION_SYN	= 102,
	SM_CONNECTION_CONNECTION_ACK	= 122,
	SM_CONNECTION_CONNECTION_NAK	= 142,

	SM_CONNECTION_DISCONNECT_CMD	= 162,

	SM_CONNECTION_PHASE_SHIFT_CMD	= 172,
	SM_CONNECTION_BLOCK_CMD			= 182,

	// ���� ���� ����� ��� �Ҳ�?
	// Ư�� ���� ���� ��� �ʿ��ѱ�?
};

enum eSM_SERVER_INFO_PROTOCOL
{
	// �ι�° ��ȣ ���� �ܰ� - ���� ���� �Ʒ��� ���� ������ �����Ѵ�.
	SM_SERVER_INFO_DESTINATION_KEY_SYN			= 53,
	SM_SERVER_INFO_DESTINATION_KEY_ACK			= 63,
	SM_SERVER_INFO_DESTINATION_KEY_NAK			= 73,

	SM_SERVER_INFO_QUERY_SYN					= 93,
	SM_SERVER_INFO_QUERY_ACK					= 103,
	SM_SERVER_INFO_QUERY_CMD					= 113,
	SM_SERVER_INFO_QUERY_NAK					= 123,

	// �̰� ����. �ð��� ���� �ɸ� ��.
	SM_SERVER_INFO_AUTH_KEY_CHANGE_CMD			= 133,
	SM_SERVER_INFO_AUTH_KEY_CHANGE_ACK			= 143,
	SM_SERVER_INFO_AUTH_KEY_CHANGE_NAK			= 153,
	SM_SERVER_INFO_AUTH_KEY_DATA_CMD			= 163,
	SM_SERVER_INFO_AUTH_KEY_DATA_ACK			= 173,

	SM_SERVER_INFO_AUTHENTICATION_SYN			= 203,
	SM_SERVER_INFO_AUTHENTICATION_ACK			= 213,
	SM_SERVER_INFO_AUTHENTICATION_NAK			= 223,

	//__NA000000_070523_REPORT_TO_SOLARAUTH
	SM_SERVER_INFO_CACHEITEM_REPORT_NTF			= 233,
	SM_SERVER_INFO_CCU_REPORT_NTF				= 234,
};

enum eSM_CRYPTOGRAPHY_PROTOCOL
{
	SM_CRYPTOGRAPHY_KEY						= 34,				// ��Ŷ ��ȣȭ Ű ��ȯ
	SM_CRYPTOGRAPHY_ACK						= 44,				
	SM_CRYPTOGRAPHY_NAK						= 54,				
	SM_CRYPTOGRAPHY_CMD						= 64,				

	SM_CRYPTOGRAPHY_PACKET_ENCODED			= 254,			// 144 + n	
	SM_CRYPTOGRAPHY_PACKET_ENCODING			= 255,				

	SM_CRYPTOGRAPHY_SNK_KEY					= 84,						
	SM_CRYPTOGRAPHY_SNK_CMD					= 94,						
	SM_CRYPTOGRAPHY_SNK_ACK					= 104,						
	SM_CRYPTOGRAPHY_SNK_NAK					= 114,						
};

enum eSM_DATA_TRANSFER_PROTOCOL
{
	SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN	= 67,	// M->S
	SM_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK	= 77,	// S->M with DATA INFO
	SM_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK	= 87,	// S->M
	// ������ ���� : �ɼ� �������� �ұ� ����...
	SM_DATA_TRANSFER_SENDDATA_CMD			= 97,	// S->M with Offset
	SM_DATA_TRANSFER_SENDDATA_ACK			= 107,	// M->S with CRC
	SM_DATA_TRANSFER_SENDDATA_NAK			= 117,	// M->S
};



#endif //__PROTOCOL_SM_H__
