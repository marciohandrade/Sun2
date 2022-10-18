#ifndef     __USER_INTERFACE_MANAGER_SYSTEM_H__
#define     __USER_INTERFACE_MANAGER_SYSTEM_H__
//------------------------------------------------------------------------------
/**
    @class uiSystemMan

    interface manager for a vendor

    (C) 2005 ckbang
*/
#include "uiSystemMan_def.h"
#include "uibaseman.h"
#include "uiSystemMessage.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "uiSystemHeimTradingvolume/uiSystemHeimTradingvolume.h"
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

typedef VOID (*fn_SysConfirm)( bool bYes );
typedef VOID (*fn_SysConfirmString)( bool bYes,const char* pString );
typedef VOID (*fn_SysVerify)(void);
typedef VOID (*fn_SysConfirmCloseButton)(int confirm_value);

enum ConfirmCloseButton
{
    kConfirmSelect1,
    kConfirmSelect2,
    kConfirmClose,
};

#ifdef _INTERNATIONAL_UI
const WzID c_wiSystemMsgFont = StrToWzID("st10");
#else
const WzID c_wiSystemMsgFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI

#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
class uiGuildWareHouse;
class uiTradeMan;
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
class Command;
class CCtrlListWZ;
class uiSystemMan :
    public uiBaseMan
{
public:
    uiSystemMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // functions
    //
public:
	BOOL                    Sys_Verify( LPCTSTR szMessage, BOOL bShutdown=FALSE);
	BOOL                    Sys_Verify( LPCTSTR szMessage, 
										fn_SysVerify _fn_sysVerify, 
										LPCTSTR pszTitle = NULL, 
										LPCTSTR pszSubject = NULL, 
										DWORD dwStrOKID = 100);

	DWORD                   Sys_Confirm( LPCTSTR szMessage, 
										 fn_SysConfirm _fn_sysconfirm, 
										 LPCTSTR pszTitle = NULL, 
										 LPCTSTR pszSubject = NULL, 
										 DWORD dwStrOKID = 100, 
										 DWORD dwStrCancleID = 101,
										 bool bLock = false,
										 DWORD dwRemainTime = 0 );
	DWORD                   Sys_ConfirmExtend( LPCTSTR szMessage, 
											fn_SysConfirm _fn_sysconfirm, 
											LPCTSTR pszTitle = NULL, 
											LPCTSTR pszSubject = NULL, 
											DWORD dwStrOKID = 100, 
											DWORD dwStrCancleID = 101,
											bool bLock = false,
											DWORD dwRemainTime = 0
											);
    DWORD                   Sys_ConfirmCloseButton(LPCTSTR szMessage, 
                                                   fn_SysConfirmCloseButton _fn_sysconfirm, 
                                                   LPCTSTR pszTitle = NULL, 
                                                   LPCTSTR pszSubject = NULL, 
                                                   DWORD dwStrOKID = 100, 
                                                   DWORD dwStrCancleID = 101,
                                                   bool bLock = false,
                                                   DWORD dwRemainTime = 0);
        
    // 닫기(x) 버튼이 없는 창
    DWORD                   Sys_ConfirmDontCloseButton(LPCTSTR szMessage, 
                                                   fn_SysConfirmCloseButton _fn_sysconfirm, 
                                                   LPCTSTR pszTitle = NULL, 
                                                   LPCTSTR pszSubject = NULL, 
                                                   DWORD dwStrOKID = 100, 
                                                   DWORD dwStrCancleID = 101,
                                                   bool bLock = false,
                                                   DWORD dwRemainTime = 0);

	BOOL                    Sys_ConfirmString(const TCHAR* szMessage, fn_SysConfirmString _fn_sysconfirmString);
	BOOL                    Sys_ConfirmMessage(const TCHAR* szMessage, fn_SysConfirmString _fn_sysconfirmString, TCHAR* szEditMessage = NULL, bool is_numberic = false);

	BOOL                    Sys_VerifyLock(const TCHAR* szMessage, BOOL bShutdown=FALSE, int closetype = 1, DWORD dwShutDownTime = 5 );

	BOOL                    Sys_VerifyLock(const TCHAR* szMessage, fn_SysVerify _fn_sysVerify);

    DWORD                   Sys_ConfirmLock(const TCHAR* szMessage, fn_SysConfirm _fn_sysconfirm);
	BOOL                    Sys_ConfirmMessageLock(const TCHAR* szMessage, fn_SysConfirmString _fn_sysconfirmString);
	BOOL                    Sys_ConfirmUnLock(const TCHAR* szMessage, fn_SysConfirm _fn_sysconfirm);

    DWORD                   Sys_ConfirmLock(const TCHAR* szMessage, Command* pOkCommand = NULL, Command* pCancelCommand = NULL );
    BOOL                    Sys_ConfirmUnLock(const TCHAR* szMessage, Command* pOkCommand = NULL, Command* pCancelCommand = NULL );

	BOOL                    Sys_ConfirmMessageUnLock(const TCHAR* szMessage, fn_SysConfirmString _fn_sysconfirmString, TCHAR* szEditMessage=NULL, TCHAR* szTitleMessage=NULL);
	void					Sys_ConfirmMessageSetFocus( void );

    DWORD                   Sys_SystemMessage(const TCHAR* message, 
                                              const TCHAR* title = NULL, 
                                              uiSystemMessage::MessageCallback callback_function = NULL, 
                                              uiSystemMessage::MessageType message_type = uiSystemMessage::kVerify_Type, 
                                              bool is_modal = false, 
                                              DWORD life_time = 0, 
                                              DWORD verify_string_code = 100, 
                                              DWORD confirm_string_code = 155536, 
                                              DWORD Rejection_string_code = 155537, 
                                              DWORD user_data = 0);
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	DWORD                   Sys_SystemHeimTradingvolumeNpcStore(const DWORD&     parent_dialog_key, 
		                                                        const NUMTYPE&   div_num, 
		                                                        const SLOTIDX&   from_slot_idx, 
		                                                        const POSTYPE&   from_postype, 
		                                                        const eSlotType& slot_type, 
		                                                        const SLOTCODE&  code, 
		                                                        const MONEY&     price, 
		                                                        bool             is_modal = false, 
		                                                        DWORD            life_time = 0);
	DWORD                   Sys_SystemHeimTradingvolumeGuildWareHouse(const DWORD&     parent_dialog_key, 
		                                                        uiGuildWareHouse* ui_guild_warehouse, 
		                                                        const MONEY&      money, 
																bool              is_modal = false, 
																DWORD life_time = 0);
	DWORD                   Sys_SystemHeimTradingvolumePersonalTrade(const DWORD&     parent_dialog_key, 
		                                                             uiTradeMan*  ui_trade_man, 
		                                                             const MONEY& money, 
		                                                             bool         is_modal = false, 
		                                                             DWORD        life_time = 0);
	DWORD                   Sys_SystemHeimTradingvolumeVendorBuy(const DWORD&   parent_dialog_key, 
		                                                         const POSTYPE& vendor_pos, 
		                                                         const MONEY&   money, 
		                                                         bool           is_modal = false, 
		                                                         DWORD          life_time = 0);
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

	void					Destroy_Sys_Confirm(DWORD dwDlgKey);

private:
	//--------------------------------------------------------------------------
	// procedure
	//
	template<class T>
	T* CreateSystemDialog(DWORD dwDialogKey, const char* pszFileName)
	{
		T* pSystemDialog = CreateUIDialog<T>( dwDialogKey, pszFileName, this, TRUE, TRUE, TRUE );
        if (pSystemDialog == NULL)
        {
            return NULL;
        }
		pSystemDialog->Clear();
		pSystemDialog->ShowWindow(TRUE);
		pSystemDialog->SetActivated(TRUE);
		return pSystemDialog;
	}

private:   
	//--------------------------------------------------------------------------
	// member vars
	//
};


//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_MANAGER_SYSTEM_H__
