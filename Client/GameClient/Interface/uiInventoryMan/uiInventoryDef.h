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
	eInventoryDlgState_Setting_PasswordCmp,		//	비밀번호를 확인하기 위한 상태
	eInventoryDlgState_Setting_PasswordSend,	//	비밀번호 확인후 패킷을 보내는 상태
	eInventoryDlgState_Check_Unlock,			//	잠금 -> 열림
	eInventoryDlgState_Check_Clear,				//	비번초기화
	eInventoryDlgState_Lock,					//	열림 -> 잠금
};

