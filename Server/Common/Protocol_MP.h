
#ifndef __PROTOCOL_MP_H__
#define __PROTOCOL_MP_H__

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
enum eMP_CATEGORY
{
	MP_CONNECTION		= 1,
	MP_MAX,
};
	
enum eMP_CONNECTION_PROTOCOL
{
	MP_CONNECTION_SERVER_INFO_CMD,

	MP_CONNECTION_DISCONNECT_CMD	= 200,
	MP_CONNECTION_PHASE_SHIFT_CMD	= 201,
	MP_CONNECTION_BLOCK_CMD			= 202,
    MP_CONNECTION_GROUND_CMD, // NOTE: f110617.1L
};







#endif // __PROTOCOL_MP_H__