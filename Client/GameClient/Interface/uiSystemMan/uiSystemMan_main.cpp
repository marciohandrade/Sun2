//------------------------------------------------------------------------------
//  uiSystemMan_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiSystemMan.h"
#include "uiSystemConfirm/uiSystemConfirm.h"
#include "uiSystemIdNumberConfirm/uiSystemIdNumberConfirm.h"
#include "uiSystemInputMessageConfirm/uiSystemInputMessageConfirm.h"
#include "uiSystemVerify/uiSystemVerify.h"
#include "hero.h"
#include "uiChatMan/uiChatMan_def.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "uiSystemHeimTradingvolume/HeimTradingvolumeSubject/HeimTradingvolumeNPCStore.h"
#include "uiSystemHeimTradingvolume/HeimTradingvolumeSubject/HeimTradingvolumeGuildWareHouse.h"
#include "uiSystemHeimTradingvolume/HeimTradingvolumeSubject/HeimTradingvolumePersonalTrade.h"
#include "uiSystemHeimTradingvolume/HeimTradingvolumeSubject/HeimTradingvolumeVendorBuy.h"
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

//------------------------------------------------------------------------------
/**
*/
uiSystemMan::uiSystemMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
    시스템 컨펌창을 Lock하지 않으면, 포커스를 잃을때 콜백으로 false가 가면서
    컨펌창이 닫힌다.
*/
DWORD
uiSystemMan::Sys_Confirm( LPCTSTR szMessage, fn_SysConfirm _fn_sysconfirm, LPCTSTR pszTitle, LPCTSTR pszSubject, DWORD dwStrOKID, DWORD dwStrCancleID, bool bLock, DWORD dwRemainTime )
{
	DWORD dwDialogKey = g_IDGenerator.GenerateID();
	uiSystemConfirm* pUISystemConfirm = CreateSystemDialog<uiSystemConfirm>(dwDialogKey, c_strDlgFname_SystemConfirm);
    if (pUISystemConfirm == NULL)
    {
        assert(pUISystemConfirm && "uiSystemMan::Sys_Confirm");
        g_IDGenerator.DestroyID(dwDialogKey);
    }
	
	pUISystemConfirm->m_dwDialogKey = dwDialogKey;
	if(pszTitle)
		pUISystemConfirm->SetTitle(pszTitle);

	if(pszSubject)
		pUISystemConfirm->SetSubject(pszSubject);

	pUISystemConfirm->SetBtnStringID(dwStrOKID, dwStrCancleID);

    pUISystemConfirm->m_fnSysConfirm = _fn_sysconfirm;

	if( bLock )	
	{
		g_InterfaceManager.LockDlgs(dwDialogKey);
		pUISystemConfirm->SetLockingEffect();
	}

	if(szMessage)
		pUISystemConfirm->SetConfirmMessage(szMessage);


    g_InterfaceManager.AttachFirstDialog(dwDialogKey);

	if(dwRemainTime)
	{
		pUISystemConfirm->SetTimer(dwRemainTime);
	}

    pUISystemConfirm->Unlock();

	return dwDialogKey;
}

DWORD
uiSystemMan::Sys_ConfirmExtend( LPCTSTR szMessage, fn_SysConfirm _fn_sysconfirm, LPCTSTR pszTitle, LPCTSTR pszSubject, DWORD dwStrOKID, DWORD dwStrCancleID, bool bLock, DWORD dwRemainTime )
{
	DWORD dwDialogKey = g_IDGenerator.GenerateID();
	uiSystemConfirm* pUISystemConfirm = CreateSystemDialog<uiSystemConfirm>(dwDialogKey, c_strDlgFname_SystemVerifyExtend);
    if (pUISystemConfirm == NULL)
    {
        assert(pUISystemConfirm && "uiSystemMan::Sys_ConfirmExtend");
        g_IDGenerator.DestroyID(dwDialogKey);
    }

	pUISystemConfirm->m_dwDialogKey = dwDialogKey;
	if(pszTitle)
		pUISystemConfirm->SetTitle(pszTitle);

	if(pszSubject)
		pUISystemConfirm->SetSubject(pszSubject);

	pUISystemConfirm->SetBtnStringID(dwStrOKID, dwStrCancleID);

	pUISystemConfirm->m_fnSysConfirm = _fn_sysconfirm;

	if( bLock )	
	{
		g_InterfaceManager.LockDlgs(dwDialogKey);
		pUISystemConfirm->SetLockingEffect();
	}

	if(szMessage)
		pUISystemConfirm->SetConfirmMessage(szMessage);


	g_InterfaceManager.AttachFirstDialog(dwDialogKey);

	if(dwRemainTime)
	{
		pUISystemConfirm->SetTimer(dwRemainTime);
	}

	pUISystemConfirm->Unlock();

	return dwDialogKey;
}

DWORD uiSystemMan::Sys_ConfirmCloseButton(LPCTSTR szMessage, 
                                          fn_SysConfirmCloseButton _fn_sysconfirmCloseButton, 
                                          LPCTSTR pszTitle, LPCTSTR pszSubject, 
                                          DWORD dwStrOKID, 
                                          DWORD dwStrCancleID, 
                                          bool bLock, 
                                          DWORD dwRemainTime)
{
    DWORD dwDialogKey = g_IDGenerator.GenerateID();
    uiSystemConfirm* pUISystemConfirm = CreateSystemDialog<uiSystemConfirm>(dwDialogKey, c_strDlgFname_SystemVerifyCloseButton);
    if (pUISystemConfirm == NULL)
    {
        assert(pUISystemConfirm && "uiSystemMan::Sys_ConfirmCloseButton");
        g_IDGenerator.DestroyID(dwDialogKey);
    }

    pUISystemConfirm->m_dwDialogKey = dwDialogKey;
    if(pszTitle)
        pUISystemConfirm->SetTitle(pszTitle);

    if(pszSubject)
        pUISystemConfirm->SetSubject(pszSubject);

    pUISystemConfirm->SetBtnStringID(dwStrOKID, dwStrCancleID);

    pUISystemConfirm->m_fnSysConfirm = NULL;
    pUISystemConfirm->m_fnSysConfirmCloseButton = _fn_sysconfirmCloseButton;

    if( bLock )	
    {
        g_InterfaceManager.LockDlgs(dwDialogKey);
        pUISystemConfirm->SetLockingEffect();
    }

    if(szMessage)
        pUISystemConfirm->SetConfirmMessage(szMessage);

    g_InterfaceManager.AttachFirstDialog(dwDialogKey);

    if(dwRemainTime)
    {
        pUISystemConfirm->SetTimer(dwRemainTime);
    }

    pUISystemConfirm->Unlock();

    return dwDialogKey;
}
//------------------------------------------------------------------------------
//**
DWORD uiSystemMan::Sys_ConfirmDontCloseButton(LPCTSTR szMessage, 
                                                   fn_SysConfirmCloseButton _fn_sysconfirm, 
                                                   LPCTSTR pszTitle, 
                                                   LPCTSTR pszSubject, 
                                                   DWORD dwStrOKID, 
                                                   DWORD dwStrCancleID,
                                                   bool bLock,
                                                   DWORD dwRemainTime)
{
    DWORD dialog_key = Sys_ConfirmCloseButton(szMessage, _fn_sysconfirm, pszTitle, pszSubject, dwStrOKID, dwStrCancleID, bLock, dwRemainTime);

    uiSystemConfirm* dialog = static_cast<uiSystemConfirm*>(g_InterfaceManager.GetDialog(dialog_key));
    if (dialog)
    {
        dialog->SetShowCancleButton(false);
        static BYTE gap = 30;
        dialog->SetTitleTextGap(gap);
        g_KeyQueueManager.DeleteMsg(eDoType_System_Dialog); // esc큐 삭제(큐 삽입은 create 할 때 이미 들어감)
    }

    return dialog_key;

}
//------------------------------------------------------------------------------
/**
    시스템 컨펌창을 Lock하지 않으면, 포커스를 잃을때 콜백으로 false가 가면서
    컨펌창이 닫힌다.
*/
BOOL
uiSystemMan::Sys_ConfirmString(const TCHAR* szMessage, fn_SysConfirmString _fn_sysconfirmString)
{	
	DWORD dwDialogKey = g_IDGenerator.GenerateID();
	uiSystemIdNumberConfirm* pUISystemIdNumberConfirm = CreateSystemDialog<uiSystemIdNumberConfirm>(dwDialogKey, c_strDlgFname_SystemConfirmString);
    if (pUISystemIdNumberConfirm == NULL)
    {
        assert(pUISystemIdNumberConfirm && "uiSystemMan::Sys_ConfirmString");
        g_IDGenerator.DestroyID(dwDialogKey);
    }
#ifdef _NA_000000_20130114_RENEWER_UI
	DWORD WzIDtoString = StrToWzID("L001");
	CCtrlListWZ* Control = pUISystemIdNumberConfirm->GetListControlWZ(WzIDtoString);

	TCHAR isNewSize[INTERFACE_STRING_LENGTH];

	if(Control)
	{
		memset(isNewSize,0,sizeof(TCHAR)*INTERFACE_STRING_LENGTH);

		float Consize = Control->GetSizeWidth();
		int Widthsize = GlobalFunc::GetTextWidth(const_cast<TCHAR*>(szMessage),StrToWzID("mn12"));

		//컨트롤과 너비중 가장 심플한것만/
		int pixelsize = ((int)Consize-Widthsize)/2;

		int iTextLength = (int)_tcslen(szMessage);
		
		//빈칸 하나의 사이즈 확인
		Widthsize = GlobalFunc::GetTextWidth(" ",StrToWzID("mn12"));

		int index = 0;
		for (; index < pixelsize/Widthsize ; ++index)
		{
			//빈칸삽입
			isNewSize[index] = ' ';
		}

		//해당 빈칸의 사이즈 확인
		

		memcpy(&isNewSize[index],szMessage,sizeof(TCHAR)*iTextLength);
	}

	pUISystemIdNumberConfirm->m_fnSysConfirmString = _fn_sysconfirmString;
    pUISystemIdNumberConfirm->SetConfirmMessage(isNewSize,true);
#else
	pUISystemIdNumberConfirm->m_fnSysConfirmString = _fn_sysconfirmString;
    pUISystemIdNumberConfirm->SetConfirmMessage(szMessage);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

    g_InterfaceManager.LockDlgs(dwDialogKey);

    pUISystemIdNumberConfirm->Unlock();

    return TRUE;
}

//------------------------------------------------------------------------------
/**
시스템 컨펌창을 Lock하지 않으면, 포커스를 잃을때 콜백으로 false가 가면서
컨펌창이 닫힌다.
*/
BOOL
uiSystemMan::Sys_ConfirmMessage(const TCHAR* szMessage, 
                                fn_SysConfirmString _fn_sysconfirmString, 
                                TCHAR* szEditMessage, 
                                bool is_numberic)
{
	DWORD dwDialogKey = g_IDGenerator.GenerateID();
	uiSystemInputMessageConfirm* pUISystemInputMessageConfirm = CreateSystemDialog<uiSystemInputMessageConfirm>(dwDialogKey, c_strDlgFname_SystemInuptMsgConfirmString);
	
	pUISystemInputMessageConfirm->m_fnSysConfirmString = _fn_sysconfirmString;
	pUISystemInputMessageConfirm->SetConfirmMessage(szMessage);

	if(szEditMessage)
	{
		pUISystemInputMessageConfirm->SetEditMessage(szEditMessage);
	}	

	g_InterfaceManager.LockDlgs(dwDialogKey);

	pUISystemInputMessageConfirm->Unlock();

    pUISystemInputMessageConfirm->SetNumberic(is_numberic);

	return TRUE;
}

//------------------------------------------------------------------------------
/** fn_SysConfirm 대신 Command를 사용
*/
DWORD
uiSystemMan::Sys_ConfirmLock(const TCHAR* szMessage, Command* pOkCommand /*= NULL*/, Command* pCancelCommand /*= NULL*/ )
{
    DWORD dwDialogKey = g_IDGenerator.GenerateID();
    uiSystemConfirm* pUISystemConfirm = CreateSystemDialog<uiSystemConfirm>(dwDialogKey, c_strDlgFname_SystemConfirm);
    if (pUISystemConfirm == NULL)
    {
        assert(pUISystemConfirm && "uiSystemMan::Sys_ConfirmLock");
        g_IDGenerator.DestroyID(dwDialogKey);
    }

    pUISystemConfirm->SetOkCommand( pOkCommand );
    pUISystemConfirm->SetCancelCommand( pCancelCommand );
    //pUISystemConfirm->m_fnSysConfirm = _fn_sysconfirm;
    pUISystemConfirm->SetConfirmMessage(szMessage);
    pUISystemConfirm->SetLockingEffect();


    g_InterfaceManager.AttachFirstDialog(dwDialogKey);

    g_InterfaceManager.LockDlgs(dwDialogKey);

    pUISystemConfirm->Unlock();

    return dwDialogKey;
}

//------------------------------------------------------------------------------
/**
*/
DWORD
uiSystemMan::Sys_ConfirmLock(const TCHAR* szMessage, fn_SysConfirm _fn_sysconfirm)
{
	DWORD dwDialogKey = g_IDGenerator.GenerateID();
	uiSystemConfirm* pUISystemConfirm = CreateSystemDialog<uiSystemConfirm>(dwDialogKey, c_strDlgFname_SystemConfirm);
    if (pUISystemConfirm == NULL)
    {
        assert(pUISystemConfirm && "uiSystemMan::Sys_ConfirmLock");
        g_IDGenerator.DestroyID(dwDialogKey);
    }

    pUISystemConfirm->m_fnSysConfirm = _fn_sysconfirm;
    pUISystemConfirm->SetConfirmMessage(szMessage);
    pUISystemConfirm->SetLockingEffect();

    g_InterfaceManager.AttachFirstDialog(dwDialogKey);

    g_InterfaceManager.LockDlgs(dwDialogKey);

    pUISystemConfirm->Unlock();

    return dwDialogKey;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
uiSystemMan::Sys_ConfirmMessageLock(const TCHAR* szMessage, fn_SysConfirmString _fn_sysconfirm)
{
	DWORD dwDialogKey = g_IDGenerator.GenerateID();
	uiSystemInputMessageConfirm* pUISystemInputMessageConfirm = CreateSystemDialog<uiSystemInputMessageConfirm>(dwDialogKey, c_strDlgFname_SystemInuptMsgConfirmString);
    if (pUISystemInputMessageConfirm == NULL)
    {
        assert(pUISystemInputMessageConfirm && "uiSystemMan::Sys_ConfirmMessageLock");
        g_IDGenerator.DestroyID(dwDialogKey);
    }

	pUISystemInputMessageConfirm->m_fnSysConfirmString = _fn_sysconfirm;
	pUISystemInputMessageConfirm->SetConfirmMessage(szMessage);
	pUISystemInputMessageConfirm->SetLockingEffect();

	g_InterfaceManager.AttachFirstDialog(dwDialogKey);
	g_InterfaceManager.LockDlgs(dwDialogKey);
	pUISystemInputMessageConfirm->Unlock();	

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
uiSystemMan::Sys_ConfirmUnLock(const TCHAR* szMessage, Command* pOkCommand /*= NULL*/, Command* pCancelCommand /*= NULL*/ )
{
	DWORD dwDialogKey = g_IDGenerator.GenerateID();
	uiSystemConfirm* pUISystemConfirm = CreateSystemDialog<uiSystemConfirm>(dwDialogKey, c_strDlgFname_SystemConfirm);
    if (pUISystemConfirm == NULL)
    {
        assert(pUISystemConfirm && "uiSystemMan::Sys_ConfirmUnLock");
        g_IDGenerator.DestroyID(dwDialogKey);
    }

    pUISystemConfirm->SetOkCommand( pOkCommand );
    pUISystemConfirm->SetCancelCommand( pCancelCommand );
	//pUISystemConfirm->m_fnSysConfirm = _fn_sysconfirm;
	pUISystemConfirm->SetConfirmMessage(szMessage);
	pUISystemConfirm->SetLockingEffect();

    g_InterfaceManager.AttachFirstDialog(dwDialogKey);

	return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL
uiSystemMan::Sys_ConfirmUnLock(const TCHAR* szMessage, fn_SysConfirm _fn_sysconfirm)
{
	DWORD dwDialogKey = g_IDGenerator.GenerateID();
	uiSystemConfirm* pUISystemConfirm = CreateSystemDialog<uiSystemConfirm>(dwDialogKey, c_strDlgFname_SystemConfirm);
    if (pUISystemConfirm == NULL)
    {
        assert(pUISystemConfirm && "uiSystemMan::Sys_ConfirmUnLock");
        g_IDGenerator.DestroyID(dwDialogKey);
    }

	pUISystemConfirm->m_fnSysConfirm = _fn_sysconfirm;
	pUISystemConfirm->SetConfirmMessage(szMessage);
	pUISystemConfirm->SetLockingEffect();

    g_InterfaceManager.AttachFirstDialog(dwDialogKey);

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
uiSystemMan::Sys_ConfirmMessageUnLock(const TCHAR* szMessage, fn_SysConfirmString _fn_sysconfirm, TCHAR* szEditMessage, TCHAR* szTitleMessage)
{
	DWORD dwDialogKey = g_IDGenerator.GenerateID();
	uiSystemInputMessageConfirm* pUISystemInputMessageConfirm = CreateSystemDialog<uiSystemInputMessageConfirm>(dwDialogKey, c_strDlgFname_SystemInuptMsgConfirmString);
    if (pUISystemInputMessageConfirm == NULL)
    {
        assert(pUISystemInputMessageConfirm && "uiSystemMan::Sys_ConfirmMessageUnLock");
        g_IDGenerator.DestroyID(dwDialogKey);
    }

	pUISystemInputMessageConfirm->m_fnSysConfirmString = _fn_sysconfirm;
	pUISystemInputMessageConfirm->SetConfirmMessage(szMessage);
	/*	Lock창이 아닌 경우인데 락 이펙트를 추가하여, 다른 창이 열리거나 포커스를 가질 때 알파 적용으로 안보이는 문제 발생해서 제거 함.
	*///m_pUISystemInputMessageConfirm->SetLockingEffect(); 

	if(szEditMessage)
	{
		pUISystemInputMessageConfirm->SetEditMessage(szEditMessage);
	}

	if(szTitleMessage)
	{
		pUISystemInputMessageConfirm->SetTitleMessage(szTitleMessage);
	}

    g_InterfaceManager.AttachFirstDialog(dwDialogKey);

	return TRUE;
}

// ConfirmMessage창의 포커스 설정
void
uiSystemMan::Sys_ConfirmMessageSetFocus( void )
{	
}


//------------------------------------------------------------------------------
/**
    시스템 확인창을 띄울때 Lock 하지 않으면,
    포커스를 잃을때 확인창이 닫힌다.
*/
BOOL
uiSystemMan::Sys_Verify(const TCHAR* szMessage, BOOL bShutdown/* =FALSE */)
{
	DWORD dwDialogKey = g_IDGenerator.GenerateID();
	uiSystemVerify* pUISystemVerify = CreateSystemDialog<uiSystemVerify>(dwDialogKey, c_strDlgFname_SystemVerify);
    if (pUISystemVerify == NULL)
    {
        assert(pUISystemVerify && "uiSystemMan::Sys_Verify");
        g_IDGenerator.DestroyID(dwDialogKey);
    }

    pUISystemVerify->SetVerifyMessage(szMessage);
	pUISystemVerify->m_fnSysVerify = NULL;

    if (bShutdown)
        pUISystemVerify->SetShutdownAfterVerify();

    g_InterfaceManager.AttachFirstDialog(dwDialogKey);

    pUISystemVerify->Unlock();

    return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL                                
uiSystemMan::Sys_Verify(const TCHAR* szMessage, fn_SysVerify _fn_sysVerify, LPCTSTR pszTitle, LPCTSTR pszSubject, DWORD dwStrOKID)
{
	DWORD dwDialogKey = g_IDGenerator.GenerateID();
	uiSystemVerify* pUISystemVerify = CreateSystemDialog<uiSystemVerify>(dwDialogKey, c_strDlgFname_SystemVerify);
    if (pUISystemVerify == NULL)
    {
        assert(pUISystemVerify && "uiSystemMan::Sys_Verify");
        g_IDGenerator.DestroyID(dwDialogKey);
    }

	if(pszTitle)
		pUISystemVerify->SetTitle(pszTitle);

	if(pszSubject)
		pUISystemVerify->SetSubject(pszSubject);

	pUISystemVerify->SetBtnStringID(dwStrOKID);
    pUISystemVerify->SetVerifyMessage(szMessage);
	pUISystemVerify->m_fnSysVerify = _fn_sysVerify;

    g_InterfaceManager.AttachFirstDialog(dwDialogKey);
    
    pUISystemVerify->Unlock();

	return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL    uiSystemMan::Sys_VerifyLock(const TCHAR* szMessage, BOOL bShutdown, int closetype, DWORD dwShutDownTime )
{
	DWORD dwDialogKey = g_IDGenerator.GenerateID();
	uiSystemVerify* pUISystemVerify = CreateSystemDialog<uiSystemVerify>(dwDialogKey, c_strDlgFname_SystemVerify);
    if (pUISystemVerify == NULL)
    {
        assert(pUISystemVerify && "uiSystemMan::Sys_VerifyLock");
        g_IDGenerator.DestroyID(dwDialogKey);
    }

    pUISystemVerify->SetVerifyMessage(szMessage);
    pUISystemVerify->SetLockingEffect();
    pUISystemVerify->m_fnSysVerify = NULL;

    if (bShutdown)
	{
		pUISystemVerify->SetShutdownAfterVerify();
		pUISystemVerify->SetCloseType( closetype );
		pUISystemVerify->ReSetShutDownTime( dwShutDownTime );
	}

    g_InterfaceManager.AttachFirstDialog(dwDialogKey);

    g_InterfaceManager.LockDlgs(dwDialogKey);

    pUISystemVerify->Unlock();

    return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
BOOL            
uiSystemMan::Sys_VerifyLock(const TCHAR* szMessage, fn_SysVerify _fn_sysVerify)
{
	DWORD dwDialogKey = g_IDGenerator.GenerateID();
	uiSystemVerify* pUISystemVerify = CreateSystemDialog<uiSystemVerify>(dwDialogKey, c_strDlgFname_SystemVerify);
    if (pUISystemVerify == NULL)
    {
        assert(pUISystemVerify && "uiSystemMan::Sys_VerifyLock");
        g_IDGenerator.DestroyID(dwDialogKey);
    }

    pUISystemVerify->SetVerifyMessage(szMessage);
    pUISystemVerify->SetLockingEffect();
	pUISystemVerify->m_fnSysVerify = _fn_sysVerify;

    g_InterfaceManager.AttachFirstDialog(dwDialogKey);

    g_InterfaceManager.LockDlgs(dwDialogKey);

    pUISystemVerify->Unlock();

    return TRUE;
}

void	uiSystemMan::Destroy_Sys_Confirm(DWORD dwDlgKey)
{
	if(0 == dwDlgKey)
		return;

	uiSystemConfirm* pDlg = (uiSystemConfirm*)g_InterfaceManager.GetDialog(dwDlgKey);
	if(NULL == pDlg)
		return;

	if(pDlg->GetDestroyWindow())
		return;

	if(pDlg->IsVisibleWindow())
	{
		pDlg->SetbShow(FALSE);
		pDlg->SetDestroyWindow(TRUE);
	}
}

DWORD uiSystemMan::Sys_SystemMessage(const TCHAR* message, 
                                    const TCHAR* title/* = NULL*/, 
                                    uiSystemMessage::MessageCallback callback_function/* = NULL*/, 
                                    uiSystemMessage::MessageType message_type/* = uiSystemMessage::kVerify_Type*/, 
                                    bool is_modal/* = false*/, 
                                    DWORD life_time/* = 0*/, 
                                    DWORD verify_string_code/* = 100*/, 
                                    DWORD confirm_string_code/* = 155536*/, 
                                    DWORD Rejection_string_code/* = 155537*/, 
                                    DWORD user_data/* = 0*/)
{
    DWORD dialog_key = g_IDGenerator.GenerateID();
    uiSystemMessage* system_message = CreateSystemDialog<uiSystemMessage>(dialog_key, c_strDlgFname_SystemMessage);
    if (system_message == NULL)
    {
        assert(system_message && "uiSystemMan::Sys_SystemMessage");
        g_IDGenerator.DestroyID(dialog_key);
    }

    system_message->SetMessage(message);
    system_message->SetTitle(title);
    system_message->set_callback_function(callback_function);
    system_message->SetMessageType(message_type);
    if (is_modal == true)
    {
        system_message->SetModal(true);
    }
    system_message->SetTimer(life_time);
    system_message->SetButtonText(uiSystemMessage::kVerify_Button, verify_string_code);
    system_message->SetButtonText(uiSystemMessage::kConfirm_Button, confirm_string_code);
    system_message->SetButtonText(uiSystemMessage::kRejection_Button, Rejection_string_code);
    if (user_data != 0)
    {
        system_message->SetUserData(user_data);
    }    

    return dialog_key;
}

#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
DWORD uiSystemMan::Sys_SystemHeimTradingvolumeNpcStore( const DWORD&   parent_dialog_key, 
	                                                    const NUMTYPE& div_num, 
													    const SLOTIDX& from_slot_idx, 
														const POSTYPE& from_postype, 
														const eSlotType& slot_type, 
														const SLOTCODE& code, 
														const MONEY& price, 
														bool is_modal /*= false*/, 
														DWORD life_time /*= 0*/ )
{
	DWORD dialog_key = g_IDGenerator.GenerateID();
	uiSystemHeimTradingvolume*  system_heim_tradingvolume = CreateSystemDialog<uiSystemHeimTradingvolume>(dialog_key, c_strDlgFname_SystemHeimTradingvolumeNotification);
	if (system_heim_tradingvolume == NULL)
	{
		assert(system_heim_tradingvolume && "uiSystemMan::Sys_SystemHeimTradingVoumeNotification");
		g_IDGenerator.DestroyID(dialog_key);
	}

	system_heim_tradingvolume->set_parent_dialog_key_(parent_dialog_key);

	if (is_modal == true)
	{
		system_heim_tradingvolume->SetModal(true);
	}
	system_heim_tradingvolume->SetTimer(life_time);

	HeimTradingvolumeNPCStore* heim_tradingvolume_npc_store = new HeimTradingvolumeNPCStore(div_num, 
		                                                                                    from_slot_idx, 
																							from_postype, 
																							slot_type, 
																							code, 
																							price);

	system_heim_tradingvolume->SetData(heim_tradingvolume_npc_store);

	return dialog_key;
}

DWORD uiSystemMan::Sys_SystemHeimTradingvolumeGuildWareHouse( const DWORD& parent_dialog_key, uiGuildWareHouse* ui_guild_warehouse, const MONEY& money, bool is_modal /*= false*/, DWORD life_time /*= 0*/ )
{
	DWORD dialog_key = g_IDGenerator.GenerateID();
	uiSystemHeimTradingvolume*  system_heim_tradingvolume = CreateSystemDialog<uiSystemHeimTradingvolume>(dialog_key, c_strDlgFname_SystemHeimTradingvolumeNotification);
	if (system_heim_tradingvolume == NULL)
	{
		assert(system_heim_tradingvolume && "uiSystemMan::Sys_SystemHeimTradingVoumeNotification");
		g_IDGenerator.DestroyID(dialog_key);
	}

	system_heim_tradingvolume->set_parent_dialog_key_(parent_dialog_key);

	if (is_modal == true)
	{
		system_heim_tradingvolume->SetModal(true);
	}
	system_heim_tradingvolume->SetTimer(life_time);

	HeimTradingvolumeGuildWareHouse* heim_tradingvolume_guild_warehouse = new HeimTradingvolumeGuildWareHouse(ui_guild_warehouse, 
		                                                                                                money);

	system_heim_tradingvolume->SetData(heim_tradingvolume_guild_warehouse);

	return dialog_key;
}

DWORD uiSystemMan::Sys_SystemHeimTradingvolumePersonalTrade( const DWORD& parent_dialog_key, uiTradeMan* ui_trade_man, const MONEY& money, bool is_modal /*= false*/, DWORD life_time /*= 0*/ )
{
	DWORD dialog_key = g_IDGenerator.GenerateID();
	uiSystemHeimTradingvolume*  system_heim_tradingvolume = CreateSystemDialog<uiSystemHeimTradingvolume>(dialog_key, c_strDlgFname_SystemHeimTradingvolumeNotification);
	if (system_heim_tradingvolume == NULL)
	{
		assert(system_heim_tradingvolume && "uiSystemMan::Sys_SystemHeimTradingVoumeNotification");
		g_IDGenerator.DestroyID(dialog_key);
	}

	system_heim_tradingvolume->set_parent_dialog_key_(parent_dialog_key);

	if (is_modal == true)
	{
		system_heim_tradingvolume->SetModal(true);
	}
	system_heim_tradingvolume->SetTimer(life_time);

	HeimTradingvolumePersonalTrade* heim_tradingvolume_personal_trade = new HeimTradingvolumePersonalTrade(ui_trade_man, 
		money);

	system_heim_tradingvolume->SetData(heim_tradingvolume_personal_trade);

	return dialog_key;
}

DWORD uiSystemMan::Sys_SystemHeimTradingvolumeVendorBuy( const DWORD& parent_dialog_key, const POSTYPE& vendor_pos, const MONEY& money, bool is_modal /*= false*/, DWORD life_time /*= 0*/ )
{
	DWORD dialog_key = g_IDGenerator.GenerateID();
	uiSystemHeimTradingvolume*  system_heim_tradingvolume = CreateSystemDialog<uiSystemHeimTradingvolume>(dialog_key, c_strDlgFname_SystemHeimTradingvolumeNotification);
	if (system_heim_tradingvolume == NULL)
	{
		assert(system_heim_tradingvolume && "uiSystemMan::Sys_SystemHeimTradingVoumeNotification");
		g_IDGenerator.DestroyID(dialog_key);
	}

	system_heim_tradingvolume->set_parent_dialog_key_(parent_dialog_key);

	if (is_modal == true)
	{
		system_heim_tradingvolume->SetModal(true);
	}
	system_heim_tradingvolume->SetTimer(life_time);

	HeimTradingvolumeVendorBuy* heim_tradingvolume_vendor_buy = new HeimTradingvolumeVendorBuy(vendor_pos, money);

	system_heim_tradingvolume->SetData(heim_tradingvolume_vendor_buy);

	return dialog_key;
}

#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------