#ifndef __PROTOCOL_MX_H__
#define __PROTOCOL_MX_H__


//-------------------------------------------------------------------------------------------------------
// *  Prefix
//		C : Client
//		G : Game Server
//		M : Master Server
//		D : DBP Server
//		X : AccountDBP Server
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


/// ī�װ�
enum eMX_CATEGORY
{
	MX_CONNECTION		= 110,
	MX_LOG				= 111,
};


/// ��������
enum eMX_CONNECTION_PROTOCOL
{
	MX_CONNECTION_REQ_SERVER_INFO_SYN,
	MX_CONNECTION_REQ_SERVER_INFO_ACK,
	MX_CONNECTION_REQ_SERVER_INFO_DBR,
	MX_CONNECTION_REQ_SERVER_INFO_CMD,

	MX_FUNCRESTRICT_RELOAD_REQ,
	MX_FUNCRESTRICT_RELOAD_ANS,
	MX_FUNCRESTRICT_RELOAD_DBR,
	MX_FUNCRESTRICT_RELOAD_NAK,

    MX_CONNECTION_GROUND_CMD, // NOTE: f110617.1L

	MX_CONNECTION_DISCONNECT_CMD	= 200,
	MX_CONNECTION_PHASE_SHIFT_CMD	= 201,
	MX_CONNECTION_BLOCK_CMD			= 202,
};

enum eMX_LOG_PROTOCOL
{
	MX_LOG_SECURE_LOG_NTF			= 124,
};


#endif // __PROTOCOL_MX_H__


