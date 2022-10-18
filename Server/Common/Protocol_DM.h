#ifndef __PROTOCOL_DM_H__
#define __PROTOCOL_DM_H__


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


/// ī�װ�
enum eDM_CATEGORY
{
	DM_CONNECTION		= 130,  // (WAVERIX) (WARNING) ������ ī�װ� ���� ��ȣ ������ ��.
	DM_OPERATION,
    MZ_CONNECTION, // < SUN_CODE_DELETE_RESERVED
    MZ_GUILD, // CHANGES: f110523.5L
	DM_MAX,
};


/// ��������
enum eDM_CONNECTION_PROTOCOL
{
	DM_CONNECTION_SERVER_INFO_CMD,

	DM_CONNECTION_DISCONNECT_CMD	= 200,
	DM_CONNECTION_PHASE_SHIFT_CMD	= 201,
	DM_CONNECTION_BLOCK_CMD			= 202,
    DM_CONNECTION_GROUND_CMD, // NOTE: f110617.1L
};

enum eDM_OPERATION_PROTOCOL
{
//	DM_OPERATION_SERVERSHUTDOWN_REQ,
//	DM_OPERATION_SERVERSHUTDOWN_ANS,
};

#if SUN_CODE_DELETE_RESERVED
enum eMZ_CONNECTION_PROTOCOL
{
	MZ_SERVER_INFO_CMD,

	MZ_CONNECTION_DISCONNECT_CMD	= 200,
	MZ_CONNECTION_PHASE_SHIFT_CMD	= 201,
	MZ_CONNECTION_BLOCK_CMD			= 202,
};
#endif

//__NA001254_20090109_RTTG_FUNC_ADD
enum eMZ_GUILD_PROTOCOL
{
	MZ_GUILD_NAME_CHANGE_SYN,
	MZ_GUILD_NAME_CHANGE_ACK,
	MZ_GUILD_NAME_CHANGE_NAK,
};

#endif // __PROTOCOL_DM_H__


