#pragma once

struct IM_INVENTORY_MANAGER
{
	static const DWORD INVEN_DLG;			
	static const DWORD EVENTINVEN_DLG;		
	static const DWORD LOTTERYINVEN_DLG;	
	static const DWORD RIDERINVEN_DLG;
    static const DWORD EXTEND_INVEN_DLG;
    static const DWORD INVEN_FILTER_DLG;
#ifdef _NA_002935_20110704_ITEM_SCORE
    static const DWORD OTHER_PLAYER_EQUIP_DLG;
#endif //_NA_002935_20110704_ITEM_SCORE

#ifdef  _NA_000000_20130114_RENEWER_UI
	static const DWORD HELMET_UI;
#endif
};

enum eInventoryDlgState
{
	eInventoryDlgState_None = 0,
	eInventoryDlgState_Setting_SSN,
	eInventoryDlgState_Setting_PasswordCmp,		//	��й�ȣ�� Ȯ���ϱ� ���� ����
	eInventoryDlgState_Setting_PasswordSend,	//	��й�ȣ Ȯ���� ��Ŷ�� ������ ����
	eInventoryDlgState_Check_Unlock,			//	��� -> ����
	eInventoryDlgState_Check_Clear,				//	����ʱ�ȭ
	eInventoryDlgState_Lock,					//	���� -> ���
};

