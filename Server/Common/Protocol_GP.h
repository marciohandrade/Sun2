
#ifndef __PROTOCOL_GP_H__
#define __PROTOCOL_GP_H__

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

#include "Protocol_DG.h"

// CHANGES: f110509.4L, removed by _NA002635_GP_DP_PROTOCOL_UINIFICATION
// moved = { eGP_CATEGORY, eGP_RANKING_PROTOCOL }

#endif // __PROTOCOL_GP_H__
