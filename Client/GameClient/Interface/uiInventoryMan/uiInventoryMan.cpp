#include "SunClientPrecompiledHeader.h"
#include "uiinventoryman.h"
#include "interfacemanager.h"
#include "InventoryDialog.h"
#include "EventInventoryDialog.h"
#include "Hero.h"
#include "BattleScene.h"
#include "LotteryInventoryDialog.h"
#include "RiderInventoryDialog.h"

#include "ExtendInventoryDialog.h"
#include "InventoryFilterDialog.h"

#ifdef _NA_002935_20110704_ITEM_SCORE
#include "OtherPlayerEquipmentContainer.h"
#endif //_NA_002935_20110704_ITEM_SCORE

#include "GlobalFunc.h"
#include "MouseHandler.h"
#include "uiPersonalWayPointWindow/uiPersonalWayPointWindow.h"
#include "uiPasswordMan/uiPassWordMan.h"
#include "uiPasswordDlg/uiPasswordDlg.h"
#include "uiSystemMan/uiSystemMan.h"
#include "HeroData.h"

#include "uiHelmetShow.h"

#include "CastLeaveFieldDialog.h"

const DWORD IM_INVENTORY_MANAGER::INVEN_DLG			= StrToWzID("0411");
const DWORD IM_INVENTORY_MANAGER::EVENTINVEN_DLG	= StrToWzID("0412");
const DWORD IM_INVENTORY_MANAGER::LOTTERYINVEN_DLG	= StrToWzID("0761");
const DWORD IM_INVENTORY_MANAGER::RIDERINVEN_DLG	= StrToWzID("0413");
const DWORD IM_INVENTORY_MANAGER::EXTEND_INVEN_DLG	= StrToWzID("0416");
const DWORD IM_INVENTORY_MANAGER::INVEN_FILTER_DLG	= StrToWzID("0417");
const DWORD IM_INVENTORY_MANAGER::HELMET_UI	= StrToWzID("0418");

#ifdef _NA_002935_20110704_ITEM_SCORE
const DWORD IM_INVENTORY_MANAGER::OTHER_PLAYER_EQUIP_DLG  = g_IDGenerator.GenerateID();
#endif //_NA_002935_20110704_ITEM_SCORE

uiInventoryMan::uiInventoryMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pInvenDlg			= NULL;
    m_pEventInvenDlg	= NULL;
    m_pLotteryInvenDlg	= NULL;
    m_pRiderInvenDlg	= NULL;

    m_pExtendInvenDlg   = NULL;
    m_pInvenFilterDlg   = NULL;
    m_pPersonalWayPoint = NULL;
    ZeroMemory(m_strPassword, sizeof(m_strPassword));

#ifdef _NA_002935_20110704_ITEM_SCORE
    other_player_equipment_dialog_ = NULL;
#endif //_NA_002935_20110704_ITEM_SCORE

}

void uiInventoryMan::OnInitialize()
{
    set_open_password_confirm(false);

    _LoadUI();
}

BOOL uiInventoryMan::_LoadUI()
{
	m_pInvenDlg			= CreateUIDialog<InventoryDialog>(
							IM_INVENTORY_MANAGER::INVEN_DLG, 
							"Data\\Interface\\41_Item_Inventory(All).iwz", 
							this, TRUE);
	assert(m_pInvenDlg);

	m_pEventInvenDlg	= CreateUIDialog<EventInventoryDialog>(
		IM_INVENTORY_MANAGER::EVENTINVEN_DLG, 
		"Data\\Interface\\41_Event_Inventory(All).iwz", 
		this, TRUE);
	assert(m_pInvenDlg);

	m_pLotteryInvenDlg		= CreateUIDialog<LotteryInventoryDialog>(
		IM_INVENTORY_MANAGER::LOTTERYINVEN_DLG, 
		"Data\\Interface\\76_1_LotterySystem.iwz", 
		this );
	assert(m_pLotteryInvenDlg);

	m_pRiderInvenDlg		= CreateUIDialog<RiderInventoryDialog>(
		IM_INVENTORY_MANAGER::RIDERINVEN_DLG, 
		"Data\\Interface\\41_5_Riding_Inventory.iwz", 
		this ,TRUE);
	assert(m_pRiderInvenDlg);

	m_pExtendInvenDlg		= CreateUIDialog<ExtendInventoryDialog>(
		IM_INVENTORY_MANAGER::EXTEND_INVEN_DLG, 
		"Data\\Interface\\41_6_Item_Inventory.iwz", 
		this ,TRUE);
    assert(m_pExtendInvenDlg);

    m_pInvenFilterDlg		= CreateUIDialog<InventoryFilterDialog>(
		IM_INVENTORY_MANAGER::INVEN_FILTER_DLG, 
		"Data\\Interface\\41_7_Item_Range.iwz", 
		this ,TRUE);
    assert(m_pInvenFilterDlg);

    if ((m_pInvenDlg != NULL) && (m_pExtendInvenDlg != NULL))
    {
        m_pInvenDlg->SetExtendInven(m_pExtendInvenDlg);
    }

    is_normal_inventory_type_ = true;

#ifdef _NA_002935_20110704_ITEM_SCORE
    other_player_equipment_dialog_ = CreateUIDialog<OtherPlayerEquipmentContainer>(
        IM_INVENTORY_MANAGER::OTHER_PLAYER_EQUIP_DLG, 
        "Data\\Interface\\41_1_Item_Slot_Inventory(Common).iwz",
        this ,TRUE, TRUE);
    assert(other_player_equipment_dialog_);
#endif //_NA_002935_20110704_ITEM_SCORE

#ifdef  _NA_000000_20130114_RENEWER_UI
	m_puiHelmet = CreateUIDialog<uiHelmetShow>(
		IM_INVENTORY_MANAGER::HELMET_UI, 
		"Data\\Interface\\41_8_Helmet.iwz", 
		this, TRUE);

    if(m_puiHelmet != NULL)
    {
	    m_puiHelmet->SetDialogKey( IM_INVENTORY_MANAGER::HELMET_UI );
    }
#endif
	return TRUE;
}
//------------------------------------------------------------------------------
/**
*/

void uiInventoryMan::ClearPassword( void )
{
	SetPasswordState(eInventoryDlgState_None);
	ZeroMemory(m_strPassword, sizeof(m_strPassword));
}

//------------------------------------------------------------------------------
/**
*/
void uiInventoryMan::SetPasswordStr(TCHAR* strPassword)
{
	Snprintf(m_strPassword, MAX_WAREHOUSE_INVENTORY_PWD_LENGTH, _T("%s"), strPassword);
}


//------------------------------------------------------------------------------
/**
*/
void uiInventoryMan::SetRiderInvenDlg(BOOL bOpen)
{
	if(g_pHero && m_pRiderInvenDlg)
	{
		//열기/닫기
		if(bOpen)
		{
			if(g_pHero->IsRiding() && !m_pRiderInvenDlg->IsVisible())
			{
				m_pRiderInvenDlg->ShowInterface(TRUE);
			}
		}
		else
		{
			m_pRiderInvenDlg->ShowInterface(FALSE);	
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
#ifdef _NA_002935_20110704_ITEM_SCORE
void uiInventoryMan::SetOtherPlayerEquipInventoryDialog(Player* player_pointer)
{
    if (player_pointer && (other_player_equipment_dialog_ != NULL))
    {
        other_player_equipment_dialog_->EquipmentContainer::SetOwnerKey(OTHER_PLAYER_EQUIPMENT_CHARACTER_KEY);
        other_player_equipment_dialog_->SetEquipmentContainer(player_pointer);
        other_player_equipment_dialog_->ShowInterface(TRUE);
    }
}
#endif //_NA_002935_20110704_ITEM_SCORE
//------------------------------------------------------------------------------
/**
*/
void uiInventoryMan::SetInventoryLockState( PLAYERINFO_FOR_PACKET::eINVEN_LOCK_MODE iInventoryLockState )
{
    xGetHeroData()->SetInventoryLockState( iInventoryLockState );

	// lammy / 맵이동시 g_pHero로부터 새로운 정보를 얻어온다. 상태 변경을 같이 해주어야 한다. 맘에 안드러...;;
	if( g_pHero != NULL )
	{
		g_pHero->GetCharInfo()->m_iInventoryLock = iInventoryLockState;
	}
}

//------------------------------------------------------------------------------
/**
*/
PLAYERINFO_FOR_PACKET::eINVEN_LOCK_MODE uiInventoryMan::GetInventoryLockState( void )
{
    return xGetHeroData()->GetInventoryLockState();
}

//------------------------------------------------------------------------------
/**
*/
void uiInventoryMan::Send_CG_ITEM_INVENTORY_PWD_LOCKING_SYN( void )
{
	MSG_CG_ITEM_INVENTORY_PWD_LOCKING_SYN		SendPacket;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
    set_open_password_confirm(false);
}

//------------------------------------------------------------------------------
/**
*/
void uiInventoryMan::Send_CG_ITEM_INVENTORY_PWD_CHECK_SYN( BYTE ReqCode, TCHAR* szPassword )
{
	MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN			SendPacket;

	SendPacket.m_ReqCode = ReqCode;
	if(ReqCode == MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::SYNC_WITH_PWD)
		SendPacket.SetPassword( szPassword );
	else if(ReqCode == MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::SYNC_WITH_SSN)
		SendPacket.SetSSN( szPassword );
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, SendPacket.GetSize());
}

//------------------------------------------------------------------------------
/**
*/
void uiInventoryMan::Send_CG_ITEM_INVENTORY_PWD_SETTING_SYN( BYTE ReqCode, TCHAR* szPassword )
{
	MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN		SendPacket;

	SendPacket.m_ReqCode = ReqCode;

	if(ReqCode == MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_WITH_PWD)
		SendPacket.SetPassword( szPassword );
	else if(ReqCode == MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_WITH_SSN)
		SendPacket.SetSSN( szPassword );

	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, SendPacket.GetSize());
}

void uiInventoryMan::Send_CG_ITEM_HAMMER_OF_ALCHEMIST_SYN( POSTYPE itemPos )
{
	MSG_CG_ITEM_HAMMER_OF_ALCHEMIST_SYN		SendPacket;
	SendPacket.m_ChargeItemPos = MouseHandler::Instance()->GetPendingUseChargeItemPos();
	SendPacket.m_TargetItemPos = itemPos;

	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
}

void uiInventoryMan::ToggleExtendInvenDlg()
{
    if (m_pInvenDlg == NULL || m_pExtendInvenDlg == NULL)
    {
        return;
    }

    if (m_pInvenDlg->IsVisible())
    {
        set_normal_inventory_type(false);
        m_pInvenDlg->ShowWindow(FALSE);
        m_pExtendInvenDlg->ShowWindow(TRUE);
    }
    else
    {
        set_normal_inventory_type(true);
        m_pInvenDlg->ShowWindow(TRUE);
        m_pExtendInvenDlg->ShowWindow(FALSE);
    }
}

void uiInventoryMan::ToggleFilterInvenDlg()
{
    if (m_pInvenFilterDlg == NULL)
    {
        return;
    }

    if (m_pInvenFilterDlg->IsVisible())
    {
        m_pInvenFilterDlg->ShowWindow(FALSE);
    }
    else
    {
        m_pInvenFilterDlg->ShowWindow(TRUE);
    }
}

void uiInventoryMan::ToggleWayPointInvenDlg()
{
    if (m_pPersonalWayPoint == NULL)
    {
        m_pPersonalWayPoint = 
            GET_CAST_DIALOG(uiPersonalWayPointWindow, IM_PERSONAL_WAYPOINT_MANAGER::WAYPOINT_DIALOG);
        assert(m_pPersonalWayPoint);
        return;
    }

	{
		CastLeaveFieldDialog* cast_dialog = 
			(CastLeaveFieldDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_DELAY_CASTING);

		if ( cast_dialog->GetDoType() == eDoType_PersonalWarp )
		{
			m_pPersonalWayPoint->ShowInterface(FALSE);
			return;
		}

        if (m_pPersonalWayPoint->IsVisible())
        {
            m_pPersonalWayPoint->ShowInterface(FALSE);
        }
        else
        {
            m_pPersonalWayPoint->ShowInterface(TRUE);
        }
    }
}


void uiInventoryMan::PasswordLockingSyn(DWORD dwStrIndex)
{
#ifdef	_INVENTORY_LOCK_BLOCK	// 임시 막음
	return;
#endif

    uiPasswordMan* pPasswordMan = (uiPasswordMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD);
	if ((pPasswordMan == NULL) && (pPasswordMan->m_pUIPasswordDlg == NULL))
    {
		return;
    }
    else if ((pPasswordMan->m_pUIPasswordDlg->IsVisible() == FALSE) && 
        (open_password_confirm_ == false))
    {
	    uiSystemMan* pSystemMan =
		    static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
	    if (pSystemMan != NULL)
	    {
		    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
		    g_InterfaceManager.GetInterfaceString(dwStrIndex, szMessage, INTERFACE_STRING_LENGTH);
		    pSystemMan->Sys_Confirm(szMessage, GlobalFunc::_fn_uiInvenPasswordLockingSyn);
            set_open_password_confirm(true);
	    }
    }
}

void uiInventoryMan::LockInventoryUI()
{
    if (m_pInvenDlg != NULL)
    {
        m_pInvenDlg->Lock();
    }
    if (m_pExtendInvenDlg != NULL)
    {
        m_pExtendInvenDlg->Lock();
    }
}

void uiInventoryMan::UnlockInventoryUI()
{
    if (m_pInvenDlg != NULL)
    {
        m_pInvenDlg->Unlock();
    }
    if (m_pExtendInvenDlg != NULL)
    {
        m_pExtendInvenDlg->Unlock();
    }
}

void uiInventoryMan::LockInventorySlot(POSTYPE slot_pos)
{
    m_pInvenDlg->SetLock(slot_pos, TRUE);
}

void uiInventoryMan::UnlockInventorySlot(POSTYPE slot_pos)
{
    m_pInvenDlg->SetLock(slot_pos, FALSE);
}

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
BOOL uiInventoryMan::GetOtherPlayerEquipmentContainerVisible()
{
	if ( other_player_equipment_dialog_ )
	{
		return other_player_equipment_dialog_->IsVisible();
	}

	return FALSE;
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
