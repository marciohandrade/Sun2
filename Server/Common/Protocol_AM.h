
#ifndef __PROTOCOL_AM_H__
#define __PROTOCOL_AM_H__

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
enum eAM_CATEGORY
{
	AM_CONNECTION	= 1,
	AM_OPERATION	= 2,
	AM_ARMORKIT		= 3,
	AM_MAX,
};

enum eAM_CONNECTION_PROTOCOL
{
	AM_CONNECTION_SERVER_INFO_CMD,
	AM_CONNECTION_CCU_REPORT_NTF,
	AM_CONNECTION_PHASE_SHIFT_CMD	= 201,
	AM_CONNECTION_ENTERSERVER_CMD,
    AM_CONNECTION_GROUND_CMD, // NOTE: f110617.1L
};

enum eAM_OPERATION_PROTOCOL
{
	AM_OPERATION_CHAR_DISCONNECT_SYN,
	AM_OPERATION_CHAR_DISCONNECT_ACK,
	AM_OPERATION_CHAR_DISCONNECT_NAK,
	AM_OPERATION_LOGIC_VIOLATION_DISCONNECT_CMD	,
};

enum eAM_ARMORKIT_PROTOCOL
{
	AM_ARMORKIT_FINDHACK_SYN,
};



#endif // __PROTOCOL_AM_H__