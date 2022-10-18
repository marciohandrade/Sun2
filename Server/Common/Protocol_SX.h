#ifndef __PROTOCOL_SX_H__
#define __PROTOCOL_SX_H__


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
enum eSX_CATEGORY
{
	SX_CONNECTION		= 120,
	SX_LOG				= 121,
};


/// ��������
enum eSX_CONNECTION_PROTOCOL
{
	SX_CONNECTION_REQ_SERVER_INFO_SYN,
	SX_CONNECTION_REQ_SERVER_INFO_ACK,
	SX_CONNECTION_REQ_SERVER_INFO_DBR,
	SX_CONNECTION_REQ_SERVER_INFO_CMD,

	//SX_FUNCRESTRICT_RELOAD_REQ,
	//SX_FUNCRESTRICT_RELOAD_ANS,
	//SX_FUNCRESTRICT_RELOAD_DBR,
	//SX_FUNCRESTRICT_RELOAD_NAK,

	SX_CONNECTION_DISCONNECT_CMD	= 200,
	SX_CONNECTION_PHASE_SHIFT_CMD	= 201,
	SX_CONNECTION_BLOCK_CMD			= 202,
};

enum eSX_LOG_PROTOCOL
{
	SX_LOG_SECURE_LOG_NTF			= 124,
};


#endif // __PROTOCOL_SX_H__


