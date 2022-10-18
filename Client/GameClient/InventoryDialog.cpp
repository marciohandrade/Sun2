//---------------------------------------------------------------------------
//
//
//	(C) 2005 y2jinc
//---------------------------------------------------------------------------


#include "SunClientPrecompiledHeader.h"
#include "inventorydialog.h"
#include <SCSlotContainer.h>
#include "ItemUnitRenderer.h"
#include "MouseHandler.h"
#include "ItemManager.h"
#include "QuickSlot.h"
#include "Hero.h"
#include "QuickContainer.h"
#include "InterfaceManager.h"
#include "Mouse.h"
#include <SCItemSlot.h>
#include "chatdialog.h"
#include "objectManager.h"
#include "SlotKeyGenerator.h"
#include "EventInventoryDialog.h"
#include "ItemCompositeParser.h"
#include "QuestManager_Concrete.h"
#include "CursorChangeTransact.h"
#include "globalfunc.h"
#include "uiItemTotalComposite/uiItemTotalComposite.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ItemManager.h"
#include "EnchantDialog2.h"

#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "uiItemCube.h"

#include "uiPasswordMan/uiPasswordMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "BattleScene.h"
#include "uiSocketRemove/uiSocketRemove.h"

#define MAX_CLIENT_INVENTORY_SLOT_NUM	(MAX_INVENTORY_SLOT_NUM+MAX_INVENTORY_SLOT_NUM)

#include "ItemInfoParser.h"
#include "ItemAttrCalculator.h"
#include "uiPersonalWayPointWindow/uiPersonalWayPointWindow.h"

#ifdef __NA_001358_ENCHANT_UNIFICATION
#include "uiEnchantMan/uiEnchantMan.h"
#endif //__NA_001358_ENCHANT_UNIFICATION
#include "ExtendInventoryDialog.h"

#include "uiSocketMan/uiSocketMan.h"
#include "uiSocketComposite/uiSocketComposite.h"

const DWORD c_dwFlashTick = 500;

#include "ItemCooltimeManager.h"

#include "Char_SkillDialog.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "HeroData.h"

#include "HotKeyManager.h"
#include "Input/KeyCommands.h"
#include "input/HotKeyInfoParser.h"
#include "ItemCrystalizeListParser.h"
#include "SoundEffect.h"
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#include "uiEquipmentAwakeningMan/uiEquipmentAwakeningDialog.h"
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#include "uiEquipmentEvolutionMan/uiEquipmentEvolutionDialog.h"
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#include "AwakeningInfoParser.h"
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM

//----------------------------------------------------------------------------
/**
*/
WzID InventoryDialog::m_wzId[INVEN_MAX] = 
{ 
	StrToWzID("B020"),	//INVEN_TB00 = 0,
	StrToWzID("B021"),	//INVEN_TB01,
	StrToWzID("B022"),	//INVEN_TB02,
	StrToWzID("B023"),	//INVEN_TB03,
	StrToWzID("B024"),	//INVEN_TB04,
	StrToWzID("B025"),	//INVEN_TB05,
	StrToWzID("B026"),	//INVEN_TB06,
	StrToWzID("B027"),	//INVEN_TB07,
	StrToWzID("B028"),	//INVEN_TB08,
	StrToWzID("B029"),	//INVEN_TB09,
	StrToWzID("B030"),	//INVEN_TB10,
	StrToWzID("B031"),	//INVEN_TB11,
	StrToWzID("B032"),	//INVEN_TB12,
	StrToWzID("B033"),	//INVEN_TB13,
	StrToWzID("B034"),	//INVEN_TB14,
    StrToWzID("T001"),  //INVEN_TEXT_MONEY,// INVEN_TEXT_MONEY,
	StrToWzID("B035"),	//INVEN_BT_MONEY,
    StrToWzID("C000"),	//INVEN_CHECK_1_PAGE,
	StrToWzID("C001"),	//INVEN_CHECK_2_PAGE,
	StrToWzID("C002"),	//INVEN_CHECK_3_PAGE,
	StrToWzID("C003"),	//INVEN_CHECK_4_PAGE,
    StrToWzID("C004"),	//INVEN_CHECK_5_PAGE,

	StrToWzID("C005"),	//INVEN_CHECK_CHARGE_1_PAGE,
	StrToWzID("C006"),	//INVEN_CHECK_CHARGE_2_PAGE,
	StrToWzID("C007"),	//INVEN_CHECK_CHARGE_3_PAGE,
	StrToWzID("C008"),	//INVEN_CHECK_CHARGE_4_PAGE,
	StrToWzID("C009"),	//INVEN_CHECK_CHARGE_5_PAGE,


	StrToWzID("BT00"),  //INVEN_BT_OPEN_EVENT
	StrToWzID("B037"),  //INVEN_BT_WASTE_BASKET

	StrToWzID("BT01"),  //INVEN_BT_UNLOCK,
	StrToWzID("B036"),  //INVEN_BT_LOCK,

	StrToWzID("T999"),  //INVEN_TXT_TITLE,

	StrToWzID("BT02"), //INVEN_FLASH_BT_OPEN_EVENT

	StrToWzID("BT03"), //INVEN_PERSONAL_WAYPOINT_BTN,

    StrToWzID("BT04"), //INVEN_BT_SETTING
    StrToWzID("BT05"), //INVEN_BT_EXTEND
    StrToWzID("B000"), //INVEN_BT_EXIT
};


//----------------------------------------------------------------------------
/**
*/
InventoryDialog::InventoryDialog(InterfaceManager* pUIMan)
	:	uiBase(pUIMan),
		m_TabNum ( 0 ),
		m_pItemUnitRenderer ( NULL )
    	,m_pExtendInventoryDialog(NULL)
#ifdef _NA_000000_20130114_RENEWER_UI
		,save_inventory_Extra_(0)
		,save_inventory_Extra_Next_(0)
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

{
    this->m_pInventoryMan = GET_DIALOG_MANAGER( uiInventoryMan, UIMAN_INVENTORY );
	assert (m_pInventoryMan );

	m_iTooltipStringCode = 0;

	m_UseTooltipCtrls.clear();

	m_UseTooltipCtrls[INVEN_BT_UNLOCK]				= 5570;	
	m_UseTooltipCtrls[INVEN_BT_LOCK]				= 5569;
	m_UseTooltipCtrls[INVEN_PERSONAL_WAYPOINT_BTN]	= 11063;

	m_UseTooltipCtrls[INVEN_BT_SETTING] = 5588;
	m_UseTooltipCtrls[INVEN_BT_EXTEND] = 5589;
	m_UseTooltipCtrls[INVEN_BT_WASTE_BASKET] = 5548;
	m_UseTooltipCtrls[INVEN_BT_OPEN_EVENT] = 5549;
	m_UseTooltipCtrls[INVEN_FLASH_BT_OPEN_EVENT] = 5549;
	SCItemSlotContainer::Init((MAX_INVENTORY_SLOT_PER_TAB * MAX_INVENTORY_TAB_NUM), SI_INVENTORY);

	GetManager()->SetPasswordState(eInventoryDlgState_None);

	m_CharCacheInfoMap.clear();

	m_bUpdateQuest = true;
}


//----------------------------------------------------------------------------
/**
*/
InventoryDialog::~InventoryDialog()
{

	m_UseTooltipCtrls.clear();

	Release();
}

//----------------------------------------------------------------------------
/**
*/

int	InventoryDialog::getTooltipString(POSTYPE pos)
{
	map<int,int>::iterator iter;

	iter = m_UseTooltipCtrls.find(pos);

	if (iter == m_UseTooltipCtrls.end())
		return 0;

	return iter->second;
}

//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::Init( CDrawBase * pDrawBase )
{
#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
    for (int i = 0 ; i < 2 ; ++i)
    {
        memset(m_hotkey[i],0,sizeof(TCHAR)*INTERFACE_STRING_LENGTH);
    }
#endif

	// 상대적인 위치처리 
	uiBase::Init(pDrawBase);

	m_TabNum = 0;

	m_iClickedIndexTab = 0;
	m_iPrevClickedIndexTab = 0;
	m_bDrawSlotInfo = FALSE;
	m_eCurInvenContainer = INVENTORY;

	m_pItemToolTip = NULL;

#ifdef _JBH_MOD_MONEY_OUTPUT
	m_dwPendingTick = 0;
	m_bCkickMoney = FALSE;
#endif	


	m_bControlsInitialized = false;

	GetControlsFromDialog();

	m_pCtrlEventBtn = (CCtrlButtonWZ*)getControl(INVEN_FLASH_BT_OPEN_EVENT);
	assert(m_pCtrlEventBtn);
	if(m_pCtrlEventBtn)
	{
		m_pCtrlEventBtn->ShowWindowWZ(WZ_HIDE);
	}

	m_dwFlashTickForEventInven = c_dwFlashTick;
	m_bEventInventory  = FALSE;
	m_bFlashEventInven = FALSE;
#ifdef _NA_000000_20130114_RENEWER_UI
	for (int index = 0 ; index < 5 ; ++index)
	{
		CControlWZ* TapAbleInvenExtra = getControl(INVEN_CHECK_CHARGE_1_PAGE + index);
		if (TapAbleInvenExtra)
		{
			TapAbleInvenExtra->EnableWindowWZ(false);
			TapAbleInvenExtra->SetTextWZA(" ");
		}
	}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
}


//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::InitRenderer()
{
	// 임시
	if (m_pItemUnitRenderer)
	{
		SAFE_RELEASENDELETE( m_pItemUnitRenderer );
	}

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( m_pDraw, GetMaxSlotNum() );
	m_pItemUnitRenderer->ShowCooldown(TRUE);
}


//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::Release()
{
	SAFE_RELEASENDELETE( m_pItemUnitRenderer );

	m_CharCacheInfoMap.clear();

}


//----------------------------------------------------------------------------
/** 이미지 리소스관련해서만 필요.
*/
VOID 
InventoryDialog::FlushUnitDraw()
{
	SCItemSlot FromSlot;
	
	m_bUpdateQuest = false;
	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		if( !IsEmpty(i) )
			DeleteSlot(i, NULL);
	}
	m_bUpdateQuest = true;
	
	Release();
	m_iClickedIndexTab = m_iPrevClickedIndexTab = 0;
}


//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::OnUpdateSolarDialog()
{
	//무언가 정보를 갱신 해야 할때 작업 

	// 돈 정보를 갱신한다. 

	m_pItemUnitRenderer->Update();

    if (m_MoneyText != NULL)
    {
        updateMoney(m_MoneyText);
    }
#ifdef _NA_000000_20130114_RENEWER_UI
	//인벤토리 페이지 정보를 갱신한다.
	UpdateExtraInventoryPage();
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

#ifdef	_JBH_MOD_MONEY_OUTPUT 
#if defined(__KR) || defined(_KOREA)
		updateToolTip();
#	endif //#if defined(__KR) || defined(_KOREA)
#endif //_JBH_MOD_MONEY_OUTPUT
}

//----------------------------------------------------------------------------
/**
*/

uiInventoryMan* InventoryDialog::GetManager()
{
	return m_pInventoryMan;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

VOID  InventoryDialog::Process(DWORD dwTick)
{
	if(m_bEventInventory)
	{
		if((DWORD)m_dwFlashTickForEventInven > dwTick && IsVisible())
		{
			m_dwFlashTickForEventInven -= dwTick;
			if( m_dwFlashTickForEventInven <= 100 )
			{
				m_bFlashEventInven ^= 1;
				m_dwFlashTickForEventInven = c_dwFlashTick;
			}
		}
	}
	else
	{
		m_bFlashEventInven = FALSE;
	}
}
//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::OnRenderSolarDialog()
{
    if (m_pItemUnitRenderer)
    {
	    m_pItemUnitRenderer->Render();

        if (MouseHandler::Instance()->GetMouseState() == MouseHandler::CRYSTALIZATION)
        {
            ImpossibleCrystalizationItemIconDeActiveRender();
        }
    }
    updateToolTip();

	if((TRUE == m_bFlashEventInven) && (WZ_HIDE == m_pCtrlEventBtn->GetState()))
	{
		m_pCtrlEventBtn->ShowWindowWZ(WZ_SHOW);
	}
	else if((FALSE == m_bFlashEventInven) && (WZ_SHOW == m_pCtrlEventBtn->GetState()))
	{
		m_pCtrlEventBtn->ShowWindowWZ(WZ_HIDE);
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::getControlPos( POSTYPE AtPos, RECT & rectOut )
{
	CControlWZ * pCtrl = getControl( AtPos );
	rectOut = pCtrl->GetSizeRect();
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
InventoryDialog::GetControlsFromDialog()
{
	int i;
	for (i = 0; i < INVEN_TAB_MAX; ++i)
	{
		m_pCtrlCheckTab[i] = (CCtrlButtonCheckWZ *) getControl(INVEN_CHECK_1_PAGE+i);

		if ( !m_pCtrlCheckTab[i] )
		{
			return FALSE;
		}
	}


	for(i = 0; i < eINVENTORY_LOCK_STATE_MAX; ++i)
	{
		m_pCtrlBtnLock[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(INVEN_BT_UNLOCK + i));
		assert(m_pCtrlBtnLock[i]);
	}

	m_TxtCtrlTitle = static_cast<CCtrlStaticWZ *>(getControl(INVEN_TXT_TITLE));
	assert(m_TxtCtrlTitle);

    m_MoneyText = static_cast<CCtrlStaticWZ *>(getControl(INVEN_TEXT_MONEY));
	assert(m_MoneyText);

	m_bControlsInitialized = true;


	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::refreshInventoryRenderUnit()
{
	ItemUnitRenderer::RenderUnit * pRender = NULL;
	RECT				rcSlot;
	CControlWZ *		pCtrl = NULL;

	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		SCSlot & rSlot = SCSlotContainer::GetSlot(i);

		if (this->IsEmpty(i))
		{
			continue;
		}

        if ( this->m_eCurInvenContainer == INVENTORY && 
				i >= m_TabNum && i < m_TabNum+INVEN_MAX_SLOT )
		{
			// 보여줘야할 목록
			if(!m_pItemUnitRenderer)
				continue;

			pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );

            pCtrl = getControl(GetRelativeSlotPos(i));
				rcSlot = pCtrl->GetSizeRect();

            if (!pRender)
			{
				pCtrl = getControl( GetRelativeSlotPos(i) );
				rcSlot = pCtrl->GetSizeRect();

				pRender = m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rcSlot, &rSlot ,pCtrl);
			}
            else
            {
                pCtrl = getControl(GetRelativeSlotPos(i));
				rcSlot = pCtrl->GetSizeRect();
                m_pItemUnitRenderer->ChangePositionItemUnit(rSlot.GetSerial(), &rcSlot, pCtrl);
            }

            if (pRender)
            {
                if(pRender != MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit)
                {
				    pRender->SetShow(TRUE);
                }
                else
                {
                    if (MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit &&
                        MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit->m_isDiv)
                    {
                        pRender->SetShow(TRUE);
                    }
                }
            }
		}
		else
		{
			// 보여지지 말아야 할 목록
			if(!m_pItemUnitRenderer)
				continue;

			pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );

			if (!pRender )
			{
				pCtrl = getControl( GetRelativeSlotPos(i) );
				rcSlot = pCtrl->GetSizeRect();

				pRender = m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rcSlot, &rSlot ,pCtrl);
			}

			if (pRender)
				pRender->SetShow(FALSE);				
		}
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::ReFreshRenderSlotALL()
{
	refreshInventoryRenderUnit();
}


//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::ReFreshRenderSlot()
{
	if ( !this->IsVisibleWindow() )
	{
        if (m_pExtendInventoryDialog != NULL)
        {
            m_pExtendInventoryDialog->RefreshRenderSlot();
        }

		return;
	}
	ReFreshRenderSlotALL();
	m_iPrevClickedIndexTab = this->m_iClickedIndexTab;

}


//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::updateMoney(CCtrlStaticWZ *pControl)
{
	if (g_pHero)
	{
		NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };

		// 돈을 셋팅한다.
        Sprintf(m_szResult, _T("%I64u"), g_pHero->GetMoney());

		::GetNumberFormat(
			LOCALE_SYSTEM_DEFAULT, 
			NULL, 
			m_szResult, 
			&nFmt, 
			m_szNumber, 
			sizeof(m_szNumber));

#ifdef _JBH_MOD_MONEY_OUTPUT
		DWORD dwColor = GlobalFunc::GetMoneyColor( g_pHero->GetMoney() );
		pControl->SetTextColor( dwColor );
#else
		pControl->SetTextColor(RGBA(255,255,255,255));
#endif
		GlobalFunc::SetCaption( pControl, m_szNumber );

	}
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
InventoryDialog::UpdateData(BOOL bTake)
{

	if(!m_bControlsInitialized)
	{
		return FALSE;
	}

	if (bTake)
	{

	}
	else
	{
		ClickCheckButton(m_iClickedIndexTab);
	}

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::OnShowWindow(BOOL val)
{
	//SolarDialog::OnShowWindow(val);

	if (val)
	{
		UpdateData(FALSE);
		ClickCheckButton(m_iClickedIndexTab );
		ReFreshRenderSlotALL();

		InitTitleByLockState(m_TxtCtrlTitle, m_pCtrlBtnLock[eINVENTORY_LOCK_STATE_LOCK],
        m_pCtrlBtnLock[eINVENTORY_LOCK_STATE_UNLOCK]);

		MSG_CG_EVENT_QUERY_EXIST_SYN SendMsg;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));

		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}
	else
	{
        UpdateData(TRUE);
        HideWindowProcess();
        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}
}

void InventoryDialog::HideWindowProcess()
{
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::SOCKET_FILL);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::ENCHANT);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::EXTRASTONE_USE);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::THROW_WASTE_BASKET);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::SOCKET_REMOVE);

#ifdef _ADD_CHARGE_ITEM_RANDOM_OPTION
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::ADD_RANDOM_OPTION );
#endif
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::RANK_DOWN);
		g_KeyQueueManager.DeleteMsg(eDoType_Rank_Down_Cancle);

        MouseHandler::Instance()->RemoveMouseState(MouseHandler::ETHERIA_EXTRACT);
        MouseHandler::Instance()->RemoveMouseState(MouseHandler::ETHERIA_EXTRACT_CASTING);
        MouseHandler::Instance()->RemoveMouseState(MouseHandler::ETHERIA_COMBINATION);
        
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::ETHER_DEVICE_FILL);
        MouseHandler::Instance()->RemoveMouseState(MouseHandler::RIDER_WAX);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::REPAIR_LIMITED);
		g_KeyQueueManager.DeleteMsg(eDoType_Repair_Limited_Cancle);
        MouseHandler::Instance()->RemoveMouseState(MouseHandler::CRYSTALIZATION);
        g_KeyQueueManager.DeleteMsg(eDoType_Crystalization);
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::AWAKENING);
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
        MouseHandler::Instance()->RollbackItemAtHand();
}
//----------------------------------------------------------------------------
/**
*/
RC::eSLOT_INSERT_RESULT		InventoryDialog::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
    //slot_safe_code:안전한 슬롯체크 구문(삽입)
    if (ValidPos(AtPos) == false)
    {
        return RC::RC_INSERT_SLOT_FAILED;
    }

	if ( GENERALPARAM->GetSpecialMode() )
	{
		CheckValidInsertWithNotice(AtPos);
	}

	if(!g_pHero)
		return RC_INSERT_SLOT_FAILED;
    g_pHero->GetQuickContainer()->UpdateEquip(rSlot.GetSlotIdx(), rSlot.GetPos(), rSlot.GetCode(), 
        SI_INVENTORY, AtPos, rSlot.GetCode());
	g_pHero->GetQuickContainer()->UpdateQuickSlot( rSlot.GetSlotIdx(), rSlot.GetPos(), rSlot.GetCode(), 
							SI_INVENTORY, AtPos, rSlot.GetCode());

	

	if( BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo( rSlot.GetCode() ) )
	{
        RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );
        //slot_safe_code:실제삽입 성공시에만 데이터처리
        if (eInsertSlotResult != RC::RC_INSERT_SLOT_SUCCESS)
        {
            return eInsertSlotResult;
        }

        if( pItemInfo->IsExistInvenOptions() )
        {
            g_pHero->AddItemEffect( g_pHero, pItemInfo, eITEM_EFFECT_TYPE_INVENTORY );

			MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_SYN SendMsg;
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
		}
	}

//////////////////////////////////////////////////////////////////////////

	// 여기서 퀵정보를 갱신.
	ItemManager::Instance()->UpdateQuickInfo();

	if(true == m_bUpdateQuest)
	{
		if ( g_pQMGR_Real )
		{
			g_pQMGR_Real->Event( QUEST_EVENT_ITEM, NULL );
			g_InterfaceManager.UpdateQuestInfo();
		}
	}

	uiItemTotalcomposite * pTotalComposite = GET_CAST_DIALOG( uiItemTotalcomposite, IM_ITEM_COMPOSITE_MANAGER::ITEM_TOTAL_COMPOSITE );

	if( pTotalComposite->IsVisible() )
	{
		pTotalComposite->updateInfo();
	}	


	// 인벤 체크 시점 검토 필요
	int nCount = GetSlotNum();
	int nMax = (g_pHero->GetCharInfo()->m_ExtraInventoryTabCount + INVEN_TAB_CHARGE_MAX) * INVEN_MAX_SLOT;

	if ( nCount == nMax )
	{
		WzColor color = WzColor_RGB(197, 47, 47);
		TCHAR szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(5663, szMessage, INTERFACE_STRING_LENGTH-1);
		g_InterfaceManager.ShowCenterMessageBox(szMessage, 5000, &color, FALSE );
	}

    if (g_pHero != NULL)
    {
        g_pHero->UpdateSkillAttr();
    }

    uiCharacterStatusMan::RefreshSimulationPlayerAttributes();

    return RC_INSERT_SLOT_SUCCESS;
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
InventoryDialog::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	assert( FALSE == IsEmpty(AtPos) );

    //slot_safe_code:안전한 슬롯체크 구문(삭제)
    if ((ValidPos(AtPos) == false) ||
        IsEmpty(AtPos))
    {
        return false;
    }

	// render unit 를 먼저 지운다.
	SCSlot & rSlot = GetSlot(AtPos);
	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->RemoveItemUnit( rSlot.GetSerial() );
	}
	else
	{
		assert(m_pItemUnitRenderer);
	}

	g_SlotKeyGenerator.Restore((DWORD)rSlot.GetSerial());

    SCItemSlot& item_slot = (SCItemSlot&)GetSlot(AtPos);
    const BASE_ITEMINFO* const pItemInfo = item_slot.GetItemInfo();
	if(pItemInfo)
	{
        if( pItemInfo->IsExistInvenOptions() )
		{
			// 이펙트 삭제
			g_pHero->RemoveItemEffectByItemCode(pItemInfo->m_Code);

			//////////////////////////////////////////////////////////////////////////
			//다른플레이어들에게 이펙트 지워졌다고 알려줌
			MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_SYN SendMsg;
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));

			//////////////////////////////////////////////////////////////////////////
			//인벤에 EffectOption 아이템이 남아있다면 다시 Add Effect
			POSTYPE start = 0;
			POSTYPE total = GetMaxSlotNum();

			int iCurPos = 0;

			for (POSTYPE i = start; i < total; ++i)
			{
				if ( IsEmpty(i) )
					continue;

				SCSlot& rSlotTemp = GetSlot(i);
				if(rSlot.GetPos() == rSlotTemp.GetPos())
					continue;

                SCItemSlot& temp_slot = (SCItemSlot&)rSlotTemp;
				if( const BASE_ITEMINFO* const pItemInfoTemp = temp_slot.GetItemInfo() )
                {
                    if( pItemInfoTemp->IsExistInvenOptions() )
                    {
                        g_pHero->AddItemEffect( g_pHero, pItemInfoTemp, eITEM_EFFECT_TYPE_INVENTORY );
					}
				}		
			}

			//옵션값 계산
            //SCItemSlot& item_slot = (SCItemSlot&)GetSlot(AtPos);
            if (!IsEmpty(AtPos))
            { 
                //BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
                //
                //if (item_info->IsExistInvenOptions())
                //{
                //    //ApplyEffect(item_slot, -1, 0);
                //}

                SCItemSlot TmpDeleteItemSlot;
                if (!SCSlotContainer::DeleteSlot(AtPos, &TmpDeleteItemSlot)) 
                {
                    return FALSE;
                }

                if (pSlotOut) 
                {
                    ((SCItemSlot*)pSlotOut)->Copy(TmpDeleteItemSlot);
                }
            }
		}
		else
		{
			SCSlotContainer::DeleteSlot(AtPos, pSlotOut );
		}
	}

	ReFreshRenderSlot();

	// TODO : 여기서 퀵정보를 갱신.
	ItemManager::Instance()->UpdateQuickInfo();

#ifdef _YMS_AUTOBOT_GAME
    ItemManager::Instance()->UpdateQuickInfo(SI_AUTOMATIC_INFO);
#endif _YMS_AUTOBOT_GAME
    //!


	if(true == m_bUpdateQuest)
	{
		if ( g_pQMGR_Real )
		{
			g_pQMGR_Real->Event( QUEST_EVENT_ITEM, NULL );
			g_InterfaceManager.UpdateQuestInfo();
		}
	}


	uiItemTotalcomposite * pTotalComposite = GET_CAST_DIALOG( uiItemTotalcomposite, IM_ITEM_COMPOSITE_MANAGER::ITEM_TOTAL_COMPOSITE );
	
	if( pTotalComposite->IsVisible() )
		pTotalComposite->updateInfo();

    if (g_pHero != NULL)
    {
        g_pHero->UpdateSkillAttr();
    }

    uiCharacterStatusMan::RefreshSimulationPlayerAttributes();

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
VOID InventoryDialog::UpdateSlot( POSTYPE AtPos, SCSlot & IN rSlot )
{
	SCSlotContainer::UpdateSlot(AtPos, rSlot);

	// 여기서 퀵정보를 갱신
	ItemManager::Instance()->UpdateQuickInfo(SI_QUICK);

    ItemManager::Instance()->UpdateQuickInfo(SI_AUTOMATIC_INFO);
}

//-------------------------------------------------------------------------------------------
/**
*/
VOID			
InventoryDialog::OnInsert( SCSlot & IN rSlotIn )
{
	// 클라에서 따로 시리얼을 발급한다.
	rSlotIn.SetSerial( g_SlotKeyGenerator.GetKey() );

	ReFreshRenderSlot();

	ItemManager::Instance()->AddUpdateItemSlot(&rSlotIn);
}

//-------------------------------------------------------------------------------------------
/**
*/
VOID			
InventoryDialog::OnDelete( SCSlot & IN rSlotIn )
{
	// render unit 를 먼저 지운다.
	ItemManager::Instance()->RemoveUpdateItemSlot(&rSlotIn);

}
//
////-------------------------------------------------------------------------------------------
///**
//*/
//void            
//InventoryDialog::UpdatePosition()
//{
//    ////SolarDialog::UpdatePosition();
//}


//----------------------------------------------------------------------------
/**
*/
ItemUnitRenderer * 
InventoryDialog::GetItemUnitRender() const
{
	return m_pItemUnitRenderer;
}


//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::MessageProc( SI_MESSAGE * pMessage )
{
	static BOOL bDclicked = FALSE;
    switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONDCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if (this->m_eCurInvenContainer != INVENTORY)
			{
				return;
			}
			
			CControlWZ * pControl = getControl(pos);
			if (!pControl)
			{
				return;
			}

			bDclicked = TRUE;
            
            POSTYPE RealSlotPos = GetRealSlotPos(pos);
            if(RealSlotPos < this->GetMaxSlotNum() && pos < INVEN_MAX_SLOT)
            {
    			this->DoubleClickSlotBtn(RealSlotPos);
            }
            else
            {
                ClickCheckButton(m_iClickedIndexTab);
            }
		}
		break;

		//----------------------
		// 더클클릭시 Lbuttondown -> Double Click -> Lbuttondown 이 연속적으로 들어와서
		// 플레그를 둬서 Lbuttondown을 한번 씹는다.
	case RT_MSG_LBUTTONCLICK:
		{
			if (bDclicked)
			{
				bDclicked = FALSE;
				ClickCheckButton(m_iClickedIndexTab);
				return;
			}

			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			
			CControlWZ * pControl = getControl(pos);
			if (!pControl)
			{
				return;
			}

			if( pos < INVEN_MAX_SLOT ) 
			{
                CControlWZ* pControl = getControl(pos);
	            if (pControl != NULL)
                {
	                POSTYPE RealPos = GetRealSlotPos(pos);
				    ClickSlotBtn(RealPos, pControl->GetSizeRect());
                }
			}
			else
			{
                ClickETCBtn(pos);
     		}			
		}
		break;

	case RT_MSG_RBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			CControlWZ * pControl = getControl(pos);
			if (!pControl)
			{
				return;
			}
            if (pos >= INVEN_MAX_SLOT) 
            {
                return;
            }
            RightClickProcess(GetRealSlotPos(pos));
		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}
//----------------------------------------------------------------------------
/**
*/
VOID InventoryDialog::RightClickProcess(POSTYPE real_pos)
{
    EnchantDialog* enchant_dialog_ptr = static_cast<EnchantDialog*>(ItemManager::Instance()->GetItemContainerDialog(SI_ENCHANT));
    if ((enchant_dialog_ptr != NULL) && (enchant_dialog_ptr->IsVisible() == TRUE))
    {
        enchant_dialog_ptr->OnEnchantItem_InventoryRightClick(SI_INVENTORY, real_pos);
        return;
    }

    // 결정화 상태일 때는 우클릭을 막자
    if (MouseHandler::Instance()->GetMouseState() == MouseHandler::CRYSTALIZATION)
    {
        return;
    }

    uiItemCube* itemcube_dialog_ptr = static_cast<uiItemCube*>(ItemManager::Instance()->GetItemContainerDialog(SI_CUBE));
    if ((itemcube_dialog_ptr != NULL) && (itemcube_dialog_ptr->IsVisible() == TRUE))
    {
        itemcube_dialog_ptr->OnItemCube_InventoryRightClick(SI_INVENTORY, real_pos);
        return;
    }

    uiSocketComposite* socketcomposite_dialog_ptr = 
        static_cast<uiSocketComposite*>(ItemManager::Instance()->GetItemContainerDialog(SI_SOCKET_COMPOSITE));
    if ((socketcomposite_dialog_ptr != NULL) && (socketcomposite_dialog_ptr->IsVisible() == TRUE))
    {
        socketcomposite_dialog_ptr->OnSocketComposite_InventoryRightClick(SI_INVENTORY, real_pos);
        return;
    }

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	uiEquipmentAwakeningDialog* equipment_wakeup_dialog = static_cast<uiEquipmentAwakeningDialog*>(ItemManager::Instance()->GetItemContainerDialog(SI_EQUIPMENT_AWAKENING));
	if ((equipment_wakeup_dialog != NULL) && (equipment_wakeup_dialog->IsVisible() == TRUE))
	{
		equipment_wakeup_dialog->OnEquipmentAwakeningItem_InventoryRightClick(SI_INVENTORY, real_pos);
		MouseHandler::Instance()->RollbackItemAtHand();
		return;
	}
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	uiEquipmentEvolutionDialog* equipment_evolution_dialog = static_cast<uiEquipmentEvolutionDialog*>(ItemManager::Instance()->GetItemContainerDialog(SI_EQUIPMENT_EVOLUTION));
	if ((equipment_evolution_dialog != NULL) && (equipment_evolution_dialog->IsVisible() == TRUE))
	{
		equipment_evolution_dialog->OnEquipmentEvolutionItem_InventoryRightClick(SI_INVENTORY, real_pos);
		MouseHandler::Instance()->RollbackItemAtHand();
		return;
	}
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM

    // default
    MouseHandler::Instance()->ItemUseTransaction(SI_INVENTORY, real_pos);  
}
//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::ClickCheckButton(int iIndex )
{
	iIndex = max(0, iIndex);
	if (iIndex >= INVEN_TAB_MAX)
	{
		return;
	}

	for (int i = 0; i < INVEN_TAB_MAX; ++i)
	{
		m_pCtrlCheckTab[i]->SetCheckState(FALSE);
	}

	m_pCtrlCheckTab[iIndex]->SetCheckState(TRUE);

	this->m_iClickedIndexTab = iIndex;
	 m_TabNum = m_iClickedIndexTab * INVEN_MAX_SLOT; 
}

VOID
InventoryDialog::ClickSlotBtn(POSTYPE RealPos, RECT rcSlot)
{
	switch (MouseHandler::Instance()->GetMouseState())
	{
	case MouseHandler::ENCHANT:
		{
#ifdef __NA_001358_ENCHANT_UNIFICATION				
			// 인스턴스 인챈트 작업
			// 클릭한 아이템이 인챈트가 가능한 아이템인지 체크 - 무기와 방어구
			// 체크 후 인챈트 패킷 전송, 팬딩
			uiEnchantMan* pUIMan = GET_DIALOG_MANAGER(uiEnchantMan, UIMAN_ENCHANT);
			if( pUIMan )
			{
				pUIMan->SetTargetItemPos(RealPos);
				if ( pUIMan->CheckEnableEnchant() )
				{
					pUIMan->AskInstantEnchant();
					MouseHandler::Instance()->RemoveMouseState( MouseHandler::ENCHANT );
				}				
			}
#endif // __NA_001358_ENCHANT_UNIFICATION
		}
		break;

	case MouseHandler::SOCKET_FILL:
		{
			// 소켓 채우기 진행 
			MouseHandler::Instance()->ItemCompositeSocketFill(
				SI_INVENTORY, 
				RealPos
				);

			MouseHandler::Instance()->RemoveMouseState(MouseHandler::SOCKET_FILL);

			// 소켓옵션 삽입을 할 아이템을 선택하시오 : 5303
			const int c_iAddSocketOption = 5303;
			g_InterfaceManager.GetInterfaceString( c_iAddSocketOption, m_szResult, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, m_szResult);
		}
		break;

    case MouseHandler::SOCKET_IDENTIFY:
        {
            // 소켓 미확인 갯수 확인
            bool result = MouseHandler::Instance()->ItemSocketIndentifyTransction(RealPos);

            MouseHandler::Instance()->RemoveMouseState(MouseHandler::SOCKET_IDENTIFY);

            if (result == false)
            {
                // 해당아이템은 소켓 수량이 이미 확인 되었거나, 소켓아이템이 아닙니다. : 6486
                const int c_iAddSocketOption = 6486;
                g_InterfaceManager.GetInterfaceString( c_iAddSocketOption, m_szResult, INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, m_szResult);
            }
        }
        break;
    case MouseHandler::CRYSTALIZATION:
        {
            // 인벤토리가 부족하다 알려주자.
            InventoryDialog* inventory_dialog_ptr = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
            if (inventory_dialog_ptr)
            {
                int count = inventory_dialog_ptr->GetSlotNum();
                int max = (g_pHero->GetCharInfo()->m_ExtraInventoryTabCount + 
                    inventory_dialog_ptr->GetMaxInvenChargeNum()) * 
                    inventory_dialog_ptr->GetMaxInvenSlotNum();

                if ( count == max )
                {
                    // 경고음 출력
                    g_SoundEffect.PlayUI(UI_SOUND_ITEM_PUT_CRYSTALIZATION);

                    TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };
                    // 5851 : 인벤토리에 공간이 없어 결정화를 진행할 수 없습니다.
                    g_InterfaceManager.GetInterfaceString(5851, result_message);
                    GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, result_message );

                    break;
                }
            }

            // 아이템 결정화
            bool result = MouseHandler::Instance()->ItemCrystalizationTransction(RealPos);

            if (result == false)
            {
                // 경고음 출력
                g_SoundEffect.PlayUI(UI_SOUND_ITEM_PUT_CRYSTALIZATION);
                // 5707 : 해당 아이템은 결정화 할 수 없는 아이템입니다
                g_InterfaceManager.GetInterfaceString(5707, m_szResult);
                GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, m_szResult );
            }
        }
        break;
    case MouseHandler::SOCKET_RECREATE:
        {
            // 헤르메시즘의 성서 사용
            bool result = MouseHandler::Instance()->ItemSocketRecreateTransction(RealPos);

            MouseHandler::Instance()->RemoveMouseState(MouseHandler::SOCKET_RECREATE);

            if (result == false)
            {
                SCItemSlot& use_slot = static_cast<SCItemSlot&>(GetSlot(RealPos));
                const BASE_ITEMINFO* selected_item_info_ptr = use_slot.GetItemInfo();

                if (selected_item_info_ptr != NULL)
                {
                    TCHAR string_name[INTERFACE_STRING_LENGTH] = {0,};
                    TCHAR string_buffer[INTERFACE_STRING_LENGTH] = {0,};
                    g_InterfaceManager.GetInterfaceString(selected_item_info_ptr->m_NCode, string_name, INTERFACE_STRING_LENGTH);
                    // 48280 %s 아이템은 소켓이 생성될 수 없는 아이템 입니다.
                    const int c_iAddSocketOption = 48280;
                    g_InterfaceManager.GetInterfaceString(c_iAddSocketOption, string_buffer, INTERFACE_STRING_LENGTH);
                    Sprintf(m_szResult, string_buffer, string_name);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, m_szResult);
                }
            }
        }
        break;
	case MouseHandler::EXTRASTONE_USE: 
		{
			// 엑스트라 스톤 적용
			BOOL bRet = MouseHandler::Instance()->ItemCompositeExtraStoneUse(
				SI_INVENTORY, 
				RealPos				
				);

			MouseHandler::Instance()->RemoveMouseState(MouseHandler::EXTRASTONE_USE);

			if ( bRet )
			{				
				// 엑스트라스톤을 적용할 아이템을 선택하시오 : 5304
				const int c_iSelectItemApplyExtraStone = 5304;
				g_InterfaceManager.GetInterfaceString(
					c_iSelectItemApplyExtraStone, 
					m_szResult, 
					INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, m_szResult);
			}
		}
		break;

	case MouseHandler::SOCKET_REMOVE:
		{			
			uiSocketRemove * pSocketRemove = static_cast<uiSocketRemove *> ( g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SOCKET_REMOVE) );
			if( pSocketRemove )
			{
				BOOL bRet =	pSocketRemove->IsAvaliableRemoveSocket(SI_INVENTORY , RealPos );
				if( bRet )
				{
					// 소켓 옵션을 리스트에 추가 하였다.
					if( pSocketRemove->InsertSocketOption(SI_INVENTORY , RealPos ) )
					{
						if( ! pSocketRemove->IsVisibleWindow() )
                        {
							pSocketRemove->ShowWindow(TRUE);
                            MouseHandler::Instance()->RemoveMouseState(MouseHandler::SOCKET_REMOVE);
                        }
					}
				}
			}
		}
		break;

#ifdef _ADD_CHARGE_ITEM_RANDOM_OPTION
	case MouseHandler::ADD_RANDOM_OPTION:
		{			
			// 소켓 옵션 제거.
			BOOL bRet = MouseHandler::Instance()->ItemCompositeAddRandomOption(
				SI_INVENTORY, 
				RealPos );

			if (bRet)
			{
				MouseHandler::Instance()->RemoveMouseState(MouseHandler::ADD_RANDOM_OPTION);
				g_KeyQueueManager.DeleteMsg(eDoType_dialog_Random_Option_cancel);
			}
		}
		break;
#endif

	case MouseHandler::REPAIR_LIMITED:
		{
			BOOL bRet = MouseHandler::Instance()->ItemRepairLimitedTransction(SI_INVENTORY, RealPos);
			if(bRet)
			{
				MouseHandler::Instance()->RemoveMouseState(MouseHandler::REPAIR_LIMITED);
				GetManager()->Send_CG_ITEM_HAMMER_OF_ALCHEMIST_SYN(RealPos);
			}
			else
				MouseHandler::Instance()->SetTryUseChageItem(FALSE);
		}
		break;


	case MouseHandler::ETHER_DEVICE_FILL:
		{
			MouseHandler::Instance()->ItemEtherDeviceFillTransction(SI_INVENTORY, RealPos);
		}
		break;


    case MouseHandler::RIDER_WAX:
        {
            MouseHandler::Instance()->ItemRiderWaxChangeTransction(SI_INVENTORY, RealPos);
        }
        break;

#ifdef _NA_0_20111012_ITEM_DATE_EXTEND
    case MouseHandler::EXTEND_ITEMDATE:
        {
            MouseHandler::Instance()->ExtendItemDateTransction(RealPos);
        }
        break;
#endif//_NA_0_20111012_ITEM_DATE_EXTEND

#ifdef _NA_003740_20111122_RANDOM_ITEM
    case MouseHandler::RANDOMIZE_ITEM:
        {
            MouseHandler::Instance()->RandomizeTransction(RealPos);
        }
        break;
#endif//_NA_003740_20111122_RANDOM_ITEM

        

    case MouseHandler::ETHERIA_EXTRACT:
        {
            //에테리아 추출이벤트(캐스팅후에 패킷보내기)
            MouseHandler::Instance()->ItemEtheriaExtractTransction(RealPos);
        }
        break;
        
    case MouseHandler::ETHERIA_COMBINATION:
        {

            if (MouseHandler::Instance()->ItemEtheriaCombinationTransction(SI_INVENTORY, RealPos))
            {
                CooltimeStruct data;
                ZeroMemory(&data, sizeof(data));
                data.type = kEtheriaCombine;
                data.max_time = 2000;
                data.EtheriaCombine.from_pos = MouseHandler::Instance()->GetPendingUseChargeItemPos();
                data.EtheriaCombine.to_pos = RealPos;
                data.slot_index = SI_INVENTORY;
                data.item_pos = RealPos;
                data.dealy_time = 2000;
                data.serial_key = GetSlot(RealPos).GetSerial();
                ItemCooltimeManager::Instance()->InsertItem(&data);
            }
        }
        break;

	case MouseHandler::GUILD_WITHDRAW:
	case MouseHandler::WITHDRAW:
		{
			//할거 없다.
		}
		break;

	case MouseHandler::ITEM_STAT_CHANGE:
		{
			SCItemSlot & rItemSlot = (SCItemSlot &)GetSlot(RealPos);
			BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( rItemSlot.GetCode() );
			if(NULL == pInfo)
				break;

            
            bool is_shell_item =  (rItemSlot.IsContainValidItem() &&
                                   rItemSlot.IsShellItem());

			
            if (rItemSlot.IsEtheriaItem() ||
                (is_shell_item == false) ||
                pInfo->IsWeapon() || 
                pInfo->IsArmor())
			{
				ItemManager::Instance()->SetChangeItemPosInStatChangePkt(RealPos);
				ItemManager::Instance()->SendStatChangeItem();
			}
			else 
			{
				TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
				//5146	기능을 사용할 수 없는 아이템입니다.
				g_InterfaceManager.GetInterfaceString(5146,szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			}
		}
		break;

	default:
		{
			//RECT rcSlot = pControl->GetSizeRect();
			MouseHandler::Instance()->ItemTransaction(
				SI_INVENTORY, 
				RealPos, 
				&rcSlot);
		}
		break;
	}
}

VOID
InventoryDialog::UpdatePage()
{
    BYTE  c_byPAGEIDS[] =
    {
        INVEN_CHECK_1_PAGE,
        INVEN_CHECK_2_PAGE,
        INVEN_CHECK_3_PAGE,
        INVEN_CHECK_4_PAGE,
        INVEN_CHECK_5_PAGE,
        INVEN_BT_OPEN_EVENT,
    };

    m_eCurInvenContainer = INVENTORY;
    m_iClickedIndexTab = m_iClickedIndexTab;
    ClickCheckButton(m_iClickedIndexTab);
    m_TabNum = m_iClickedIndexTab * INVEN_MAX_SLOT; 
    
    ReFreshRenderSlot();
}

//-------------------------------------------------------------------------------------------
/**
*/

void InventoryDialog::InitTitleByLockState(CCtrlStaticWZ* pTxtCtrlTitle,
    CCtrlButtonWZ* pCtrlBtnLock, CCtrlButtonWZ* pCtrlBtnUnlock)
{
    PLAYERINFO_FOR_PACKET::eINVEN_LOCK_MODE InventoryLockState = xGetHeroData()->GetInventoryLockState();

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	if(pTxtCtrlTitle)
	{
		if( InventoryLockState == PLAYERINFO_FOR_PACKET::DISABLEDnEXIST)
		{
			// 인벤토리 (잠김)
			g_InterfaceManager.GetInterfaceString( 1187, szMessage, INTERFACE_STRING_LENGTH );
		}
		else
		{
			// 인벤토리 (열림)
			g_InterfaceManager.GetInterfaceString( 1188, szMessage, INTERFACE_STRING_LENGTH );
		}

		GlobalFunc::SetCaption(pTxtCtrlTitle, szMessage);
	}

	InitDlgShowNHide(pCtrlBtnLock, pCtrlBtnUnlock);
}

//-------------------------------------------------------------------------------------------
/**
*/
void InventoryDialog::InventoryUnitRender()
{
    m_pItemUnitRenderer->Render();

    if (MouseHandler::Instance()->GetMouseState() == MouseHandler::CRYSTALIZATION)
    {
        ImpossibleCrystalizationItemIconDeActiveRender();
    }
}

//-------------------------------------------------------------------------------------------
/**
*/
void InventoryDialog::RefreshItemMark()
{
    if (m_pItemUnitRenderer != NULL)
    {
        m_pItemUnitRenderer->RefreshItemMark();
    }

}

void InventoryDialog::SetInventoryMouseOverType(const eMOUSE_OVER_TYPE mouse_over_type)
{
    m_MouseOverObject.eMouseOverType = mouse_over_type;
}

//-------------------------------------------------------------------------------------------
/**
*/
void InventoryDialog::InitDlgShowNHide(CCtrlButtonWZ* pCtrlBtnLock, CCtrlButtonWZ* pCtrlBtnUnlock)
{
#ifdef _GS_GSP_REMOVE_LOCKBTN_
	//락버튼 없애기
	pCtrlBtnLock->ShowWindowWZ(WZ_HIDE);
	pCtrlBtnUnlock->ShowWindowWZ(WZ_HIDE);
#else

    PLAYERINFO_FOR_PACKET::eINVEN_LOCK_MODE InventoryLockState = xGetHeroData()->GetInventoryLockState();
	if( InventoryLockState == PLAYERINFO_FOR_PACKET::DISABLEDnEXIST )
	{
		pCtrlBtnLock->ShowWindowWZ(WZ_SHOW);
		pCtrlBtnUnlock->ShowWindowWZ(WZ_HIDE);
	}
	else
	{
		pCtrlBtnUnlock->ShowWindowWZ(WZ_SHOW);
		pCtrlBtnLock->ShowWindowWZ(WZ_HIDE);
	}
#endif//_GS_GSP_REMOVE_LOCKBTN_
}

//-------------------------------------------------------------------------------------------
/**
*/
void InventoryDialog::SetInventoryLockState( PLAYERINFO_FOR_PACKET::eINVEN_LOCK_MODE iInventoryLockState )
{
    GetManager()->SetInventoryLockState( iInventoryLockState );

	InitTitleByLockState(m_TxtCtrlTitle, m_pCtrlBtnLock[eINVENTORY_LOCK_STATE_LOCK],
        m_pCtrlBtnLock[eINVENTORY_LOCK_STATE_UNLOCK]);
    if (m_pExtendInventoryDialog != NULL)
    {
        m_pExtendInventoryDialog->InitTitleByLockState();
    }
}


//-------------------------------------------------------------------------------------------
/**
*/
VOID					
InventoryDialog::ClickETCBtn(POSTYPE pos)
{
    if (MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit)
    {
        if (MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit->m_isDiv)
        {
            if (pos < INVEN_CHECK_1_PAGE || pos > INVEN_CHECK_CHARGE_5_PAGE)
            {
                UpdatePage();
                return;
            }
        }
    }

	switch(pos)
	{
	case INVEN_CHECK_1_PAGE:
	case INVEN_CHECK_2_PAGE:
	case INVEN_CHECK_3_PAGE:
	case INVEN_CHECK_4_PAGE:
	case INVEN_CHECK_5_PAGE:
		{
			m_eCurInvenContainer = INVENTORY;

			POSTYPE TempPos = pos - INVEN_CHECK_1_PAGE;

			m_iClickedIndexTab = TempPos;
			ClickCheckButton(m_iClickedIndexTab );

			if (m_iPrevClickedIndexTab == m_iClickedIndexTab)
			{
				return;
			}

			m_TabNum = m_iClickedIndexTab * INVEN_MAX_SLOT; 

			ReFreshRenderSlot();
		}
		break;	


	case INVEN_CHECK_CHARGE_1_PAGE:		
	case INVEN_CHECK_CHARGE_2_PAGE:
	case INVEN_CHECK_CHARGE_3_PAGE:
	case INVEN_CHECK_CHARGE_4_PAGE:
	case INVEN_CHECK_CHARGE_5_PAGE:
		{	
			POSTYPE TempPos = pos - INVEN_CHECK_1_PAGE;
			ClickCheckChargeBtn(TempPos);
		}
		break;

	case INVEN_BT_OPEN_EVENT:
	case INVEN_FLASH_BT_OPEN_EVENT:
		{
			EventInventoryDialog * pEventInventory = GET_CAST_DIALOG( EventInventoryDialog, IM_INVENTORY_MANAGER::EVENTINVEN_DLG );
			if ( pEventInventory )
			{
				if ( !pEventInventory->GetPacketStatus() )
				{
					// 패킷처리가 다 끝난 상황이다.
					pEventInventory->SendRequestFetchEventItem();
				}
			}
		}
		break;

	case INVEN_BT_WASTE_BASKET:
		{
            EnableWasteBasket();
		}
		break;

	case INVEN_BT_UNLOCK:
		{
            int iStrIndex = 0;
            PLAYERINFO_FOR_PACKET::eINVEN_LOCK_MODE InventoryLockState = xGetHeroData()->GetInventoryLockState();

			if( InventoryLockState == PLAYERINFO_FOR_PACKET::ENABLEnEMPTY)
			{	//	5585	인벤토리와 보조창고 보호 기능을 사용하시겠습니까? 비밀번호를 설정할 경우, 게임 종료 시 자동 잠금 상태로 변경됩니다.
				iStrIndex = 5585;
			}
			else
			{	//	5582	인벤토리와 보조 창고를 잠금 상태로 변경하시겠습니까?
				iStrIndex = 5582;
			}

            if (GetManager() != NULL)
            {
			    GetManager()->PasswordLockingSyn(iStrIndex);
            }
		}
		break;
	case INVEN_BT_LOCK:
		{
            if (GetManager() != NULL)
            {
			    //	5583	인벤토리와 보조 창고를 열림 상태로 변경하시겠습니까?
			    GetManager()->PasswordLockingSyn(5583);
            }
		}
		break;

#ifdef _JBH_MOD_MONEY_OUTPUT
	case INVEN_TEXT_MONEY:
		{
			m_bCkickMoney = TRUE;
		}
		break;
#endif

	case INVEN_PERSONAL_WAYPOINT_BTN:
		{
            if (GetManager() != NULL)
            {
                GetManager()->ToggleWayPointInvenDlg();
            }
		}
        break;

    case INVEN_BT_SETTING:
        {   // 인벤토리 설정창을 연다
            if (GetManager() != NULL)
            {
                GetManager()->ToggleFilterInvenDlg();
            }
        }
        break;
    case INVEN_BT_EXTEND:
        {   // 인벤토리 확장창을 연다, 자신은 닫는다.
            if (GetManager() != NULL)
            {
                GetManager()->ToggleExtendInvenDlg();
            }
        }
        break;
    case INVEN_BT_EXIT:
        {
            ShowDialog(FALSE);
        }
        break;
	}
}
//----------------------------------------------------------------------------
/**
*/
void InventoryDialog::SetPasswordDlg(int iStringTableNum, int iPasswordDlgState)
{
	int iMinCount = 0;
	int iMaxCount = MAX_SSN_COUNT;

	if(iPasswordDlgState == eInventoryDlgState_Setting_SSN || iPasswordDlgState == eInventoryDlgState_Check_Clear)
	{ 
		iMinCount = 0; 
		iMaxCount = MAX_SSN_COUNT;
	}
	else
	{ 
		iMinCount = 4; 
		iMaxCount = MAX_WAREHOUSE_INVENTORY_PWD_LENGTH;
	}

	GetManager()->SetPasswordState(iPasswordDlgState);

    BOOL showPassChgBtn = TRUE;

	showPassChgBtn = ( GetManager()->GetInventoryLockState() & MSG_CG_WAREHOUSE_START_ACK::DISABLED );

	uiPasswordMan* pPasswordMan =
		static_cast<uiPasswordMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PASSWORD));
	assert (pPasswordMan);

	if(pPasswordMan)
	{
		pPasswordMan->PasswordDlg(iStringTableNum, GlobalFunc::_fn_uiInvenPassword, iMinCount, iMaxCount, showPassChgBtn, ePassWordCalledState_Inven);
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
VOID					
InventoryDialog::DoubleClickSlotBtn(POSTYPE realpos)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	if (IsEmpty(realpos))
	{
		return;
	}

    if (MouseHandler::Instance()->GetMouseState() == MouseHandler::CRYSTALIZATION)
    {
        // 결정화 상태일때는 더블클릭을 막는다.

        return;
    }

	//POSTYPE realpos = GetRealSlotPos(pos);
	SCItemSlot & rItemSlot = (SCItemSlot &)GetSlot(realpos);

    // 더블클릭의로 에테리아 조합하기(반드시 슬롯아이템정보를 참조해야함!)
    if (rItemSlot.IsEtheriaItem())
    {
        MouseHandler::Instance()->SetMouseState(MouseHandler::ETHERIA_COMBINATION);
        MouseHandler::Instance()->SetPendingUseChargeItemPos(realpos);
        MouseHandler::Instance()->RollbackItemAtHand();
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_Etheria_Combine;				
        msg.DoSomething=GlobalFunc::DoEtheriaCombination;
        g_KeyQueueManager.PushBack(msg);
        //6100:에테리아를 조합할 아이템을 클릭해주세요
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6100));
    }
	switch (rItemSlot.GetItemInfo()->m_wType)
	{
	case eITEMTYPE_ENCHANT:
		{
#ifdef __NA_001358_ENCHANT_UNIFICATION				
			// 인스턴스 인챈트 작업
			// 사용하는 보석 저장
			uiEnchantMan* pUIMan = GET_DIALOG_MANAGER(uiEnchantMan, UIMAN_ENCHANT);
			if( pUIMan )
			{
				MouseHandler::Instance()->RollbackItemAtHand();
				MouseHandler::Instance()->SetMouseState( MouseHandler::ENCHANT );
				pUIMan->SetInstantEnchantPos(realpos);
				
				// 인챈트할 아이템을 선택하십시오.
				g_InterfaceManager.GetInterfaceString( 721, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

				keyMsg msg;
				ZeroMemory(&msg, sizeof(msg));
				msg.DoSomething = GlobalFunc::DoInstantEnchantCancle;
				msg.dwType = eDoType_dialog_InstantEnchant_Cancle;
				g_KeyQueueManager.PushBack(msg);
			}
#endif // __NA_001358_ENCHANT_UNIFICATION
		}
		break;

    case eITEMTYPE_CUBE:
        {
            uiItemCube* dialog = GET_CAST_DIALOG(uiItemCube, IM_ITEM_COMPOSITE_MANAGER::ITEM_CUBE);
            if (dialog)
            {
                if (dialog->IsActivated())
                {
                    if (realpos != dialog->cube_position())
                    {
                        dialog->ShowInterface(FALSE);
                    }
                    else
                    {
                        return;
                    }
                }
                dialog->ShowInterface(TRUE);
                ItemManager::Instance()->SetItemLock(SI_INVENTORY, realpos, true);
                dialog->SetCubeItemPosition(realpos);
                MouseHandler::Instance()->RollbackItemAtHand();
            }
        }
        break;

    case eITEMTYPE_SOCKET_ZARD:
		{
			// 소켓 재료.
			MouseHandler::Instance()->SetMouseState( MouseHandler::SOCKET_FILL );
			MouseHandler::Instance()->SetCrystalPos(realpos);
			//	소켓 옵션을 채울 아이템을 클릭하세요.
			g_InterfaceManager.GetInterfaceString( eST_SELECT_ITEM_FOR_SOCKET_OPTION, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

			MouseHandler::Instance()->RollbackItemAtHand();

		}
		break;

    case eITEMTYPE_SOCKET_IDENTIFY: //소켓 확인 아이템
        {   // 소켓 확인 재료.
            MouseHandler::Instance()->SetSocketIdentifyState();
            MouseHandler::Instance()->SetPendingUseChargeItemPos(realpos);
            //	6483:소켓 등급을 확인하고 싶은 아이템을 클릭하세요
            g_InterfaceManager.GetInterfaceString(6483, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
            MouseHandler::Instance()->RollbackItemAtHand();
        }
        break;

    case eITEMTYPE_STAT_CHANGE: //헤르메시즘의 성서(랜덤형/고정형)
        {   // 헤르메시즘 아이템
            MouseHandler::Instance()->SetRecreateSocketState();
            MouseHandler::Instance()->SetPendingUseChargeItemPos(realpos);
            //	6483:소켓 등급을 확인하고 싶은 아이템을 클릭하세요
            /*g_InterfaceManager.GetInterfaceString(6483, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );*/
            MouseHandler::Instance()->RollbackItemAtHand();
        }
        break;

    
	case eITEMTYPE_CRYSTAL:
		{
			sITEMCOMPOSITE * pInfo = ItemCompositeParser::Instance()->GetCompositeInfo(eITEMCOPOSITE_EXTRASTONE_USE);
			if ( pInfo && 
					(SLOTCODE)rItemSlot.GetCode() == pInfo->m_sCompositeMaterials[0].m_dwCompositeMaterialCode )
			{
				// 엑스트라 스톤 사용하기 위한 재료
				MouseHandler::Instance()->SetMouseState( MouseHandler::EXTRASTONE_USE );
				MouseHandler::Instance()->SetCrystalPos(realpos);
				// 엑스트라 스톤을 사용할 아이템을 클릭하세요.
				g_InterfaceManager.GetInterfaceString( eST_SELECT_EXTRASTON_ITEM_CLICK, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage );

				MouseHandler::Instance()->RollbackItemAtHand();
			}
		}
		break;

	case eITEMTYPE_SOCKETEXTRACT:
		{
			MouseHandler::Instance()->SetMouseState( MouseHandler::SOCKET_REMOVE );
			MouseHandler::Instance()->SetPendingSocketRemovePos(realpos);
			MouseHandler::Instance()->SetPendingSocketItemPos(realpos);
		
			//5864	소켓 옵션을 제거할 아이템을 선택해주세요
			g_InterfaceManager.GetInterfaceString( 5864 , szMessage , INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage );

			MouseHandler::Instance()->RollbackItemAtHand();
		}
		break;

	case eITEMTYPE_HAMMER_OF_ALCHEMIST:
		{
			MouseHandler::Instance()->SetMouseState( MouseHandler::REPAIR_LIMITED);
			MouseHandler::Instance()->SetPendingUseChargeItemPos(realpos);
			MouseHandler::Instance()->RollbackItemAtHand();

			keyMsg msg;
			ZeroMemory(&msg, sizeof(msg));
			msg.DoSomething = GlobalFunc::DoRepairLimitedCancle;
			msg.dwType = eDoType_Repair_Limited_Cancle;
			g_KeyQueueManager.PushBack(msg);
		}
		break;

	case eITEMTYPE_ETHER_DEVICE:
		{
			MouseHandler::Instance()->SetMouseState(MouseHandler::ETHER_DEVICE_FILL);
			MouseHandler::Instance()->SetPendingUseChargeItemPos(realpos);
			MouseHandler::Instance()->RollbackItemAtHand();

			//6128	에테르 방출장치를 장착할 아이템을 선택하세요.
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6128) );
		}
		break;

	default:
		break;
	}
}


//-------------------------------------------------------------------------------------------
/** 캐시아이템 슬롯 클릭시 처리
*/
VOID InventoryDialog::ClickCheckChargeBtn(POSTYPE pos)
{
	m_eCurInvenContainer = INVENTORY;	

	if ( !g_pHero )
	{
		ClickCheckButton(m_iPrevClickedIndexTab );
		return;		
	}

	if ( pos >= g_pHero->GetCharInfo()->m_ExtraInventoryTabCount + MAX_INVENTORY_TAB_NUM)
	{
		// 상용화 아이템을 구입하세요.
		// 현재는 사용할수 없는 텝입니다.

		ClickCheckButton(m_iPrevClickedIndexTab );
		//7018 해당 인벤토리를 사용할 수 없습니다. 원정용 가방을 사용하셔야 합니다.
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		g_InterfaceManager.GetInterfaceString(7018, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

		return;
	}

	m_iClickedIndexTab = pos;
	ClickCheckButton(m_iClickedIndexTab );
	if (m_iPrevClickedIndexTab == m_iClickedIndexTab)
	{
		return;
	}
	m_TabNum = m_iClickedIndexTab * INVEN_MAX_SLOT; 
	ReFreshRenderSlot();	

	//// 특정 패킷을 보내서 정보를 받아온 상태라면 바로 갱신.
	//// 그러기 위해서는 특정 정보에 대해서 값을 받아왔는지 알아오는 길을 알아야한다.
}

VOID InventoryDialog::ReceivedChargeInfo(POSTYPE pos)
{
	CHARGE_CACHE_INFO_ITR itr =	m_CharCacheInfoMap.find( pos );
	if ( itr == m_CharCacheInfoMap.end() )
	{
		m_CharCacheInfoMap.insert(CHARGE_CACHE_INFO_PAIR(pos, eCHARGE_CACHE_TRY_REQUEST_INFO));
	}
	else
	{
		itr->second = eCHARGE_CACHE_TRY_REQUEST_INFO;
		eCHARGE_CACHE_TYPE eTestValue = itr->second;
	}
	
	m_iClickedIndexTab = pos;
	ClickCheckButton(m_iClickedIndexTab );
	if (m_iPrevClickedIndexTab == m_iClickedIndexTab)
	{
		return;
	}

	m_TabNum = m_iClickedIndexTab * INVEN_MAX_SLOT; 
	ReFreshRenderSlot();

}


//-------------------------------------------------------------------------------------------
/** 잔상문제 관련 발생시 정보를 알려주기 위한 함수.
*/
VOID					
InventoryDialog::CheckValidInsertWithNotice(POSTYPE pos)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	if ( !IsEmpty(pos) )
	{
		//5150	%d번째 슬롯에 중복 추가되었습니다. 현재 상황에 대해 스크린샷을 첨부하여 운영팀에게 자세한 내용을 보내주시기 바랍니다.
		g_InterfaceManager.GetInterfaceString( 5150 , szMessage ,INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_TOP_SHOW, szMessage, pos);
	}
}   

//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::NetworkProc( MSG_BASE * pMsg )
{
	if ( pMsg->m_byCategory == CG_ITEM )
	{

		switch ( pMsg->m_byProtocol )
		{
		case CG_ITEM_INVENTORY_PWD_LOCKING_ACK:
			{
				MSG_CG_ITEM_INVENTORY_PWD_LOCKING_ACK* pRecvMsg = (MSG_CG_ITEM_INVENTORY_PWD_LOCKING_ACK *)pMsg;

				TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0, };

				switch( pRecvMsg->m_byRetCode )
				{
				case MSG_CG_ITEM_INVENTORY_PWD_LOCKING_ACK::RET_SUCCESS_LOCKED:
					{
						SetInventoryLockState(PLAYERINFO_FOR_PACKET::DISABLEDnEXIST);
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							//	인벤토리와 보조 창고가 잠금 상태로 변경되었습니다.
							g_InterfaceManager.GetInterfaceString( 5574, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, NULL);
						}
						GetManager()->ClearPassword();
					}
					break;

				case MSG_CG_ITEM_INVENTORY_PWD_LOCKING_ACK::RET_NEED_PWD_FOR_LOCKING:
					{
						GetManager()->Send_CG_ITEM_INVENTORY_PWD_SETTING_SYN(MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_FIRST);
					}
					break;

				case MSG_CG_ITEM_INVENTORY_PWD_LOCKING_ACK::RET_NEED_PWD_FOR_UNLOCKING:
					{
						GetManager()->Send_CG_ITEM_INVENTORY_PWD_CHECK_SYN(MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::SYNC_FIRST);
					}
					break;
				}
			}
			break;

		case CG_ITEM_INVENTORY_PWD_LOCKING_NAK:
			{
				MSG_CG_ITEM_INVENTORY_PWD_LOCKING_NAK* pRecvMsg = (MSG_CG_ITEM_INVENTORY_PWD_LOCKING_NAK *)pMsg;

				GetManager()->ClearPassword();
				ItemManager::Instance()->NoticeReasonItemErrorCode(pRecvMsg->m_byErrCode);
			}
			break;

		case CG_ITEM_INVENTORY_PWD_CHECK_ACK:
			{
				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
				MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK* pRevMsg = (MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK *)pMsg;

				GetManager()->ClearPassword();

				switch(pRevMsg->m_byRetCode)
				{
				case MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_SUCCESS:
					{
						SetInventoryLockState(PLAYERINFO_FOR_PACKET::ENABLEnEXIST);
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							//	5576	인벤토리와 보조 창고가 열림 상태로 변경되었습니다.
							g_InterfaceManager.GetInterfaceString( 5576, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, NULL);
						}
					}
					break;

				case MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_INVALID_PWD:
					{
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							GetManager()->SetPasswordState(eInventoryDlgState_Check_Unlock);
							//	비밀번호가 틀렸습니다.
							g_InterfaceManager.GetInterfaceString( 5555, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, GlobalFunc::_fn_uiInvenPassword_invalidPassword);
						}
					}
					break;

				case MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_NEED_PWD:
					{
						//	비밀번호를 입력해주세요
						SetPasswordDlg(5551, eInventoryDlgState_Check_Unlock);
					}
					break;

				case MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_NEED_SSN:
					{
						//	주민등록번호 뒷자리 입력
						SetPasswordDlg(5579, eInventoryDlgState_Check_Clear);
					}
					break;

				case MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_INVALID_SSN:
					{
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							GetManager()->SetPasswordState(eInventoryDlgState_Check_Clear);
							//	주민등록번호가 일치하지 않습니다.
							g_InterfaceManager.GetInterfaceString( 5845, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, GlobalFunc::_fn_uiInvenPassword_invalidSSN);
						}
					}
					break;

				default:
					break;
				}
		
			}
			break;

		case CG_ITEM_INVENTORY_PWD_SETTING_ACK:
			{
				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
				MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK * pRecvMsg = (MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK *)pMsg;

				switch(pRecvMsg->m_byRetCode)
				{
				case MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_FAIL:
				case MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_CANCELED:
					GetManager()->ClearPassword();
					break;

				case MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_SUCCESS:
					{
						GetManager()->ClearPassword();
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							//	비밀번호가 설정되었습니다.
							g_InterfaceManager.GetInterfaceString( 5556, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, NULL);
						}
						SetInventoryLockState(PLAYERINFO_FOR_PACKET::ENABLEnEXIST);
					}
					break;

				case MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_NEED_SSN:
					{
						//	주민등록번호 뒷자리 입력
						SetPasswordDlg(5579, eInventoryDlgState_Setting_SSN);
					}
					break;

				case MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_INVALID_SSN:
					{
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							GetManager()->SetPasswordState(eInventoryDlgState_Setting_SSN);
							//	주민등록번호가 일치하지 않습니다.
							g_InterfaceManager.GetInterfaceString( 5845, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, GlobalFunc::_fn_uiInvenPassword_invalidSSN);
						}
					}
					break;

				case MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_NEED_PWD:
					{
						//	비밀번호를 입력해 주세요
						SetPasswordDlg(5551, eInventoryDlgState_Setting_PasswordCmp);
					}
					break;

				case MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_INVALID_PWD:
					{
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							GetManager()->SetPasswordState(eInventoryDlgState_Lock);
							//	비밀번호가 틀렸습니다.
							g_InterfaceManager.GetInterfaceString( 5555, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, GlobalFunc::_fn_uiInvenPassword_invalidPassword);
						}
					}
					break;

				case MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_SUCCESS_CLEAR:
					{
						SetInventoryLockState(PLAYERINFO_FOR_PACKET::ENABLEnEMPTY);
						uiSystemMan* pSystemMan =
							static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
						if(pSystemMan)
						{
							//	잠금 기능을 [사용 안 함]으로 변경하였습니다.
							g_InterfaceManager.GetInterfaceString( 5577, szMessage, INTERFACE_STRING_LENGTH );
							pSystemMan->Sys_Verify(szMessage, NULL);
						}
					}
					break;
				}
			}
			break;

		case CG_ITEM_INVENTORY_PWD_SETTING_NAK:
			{
				GetManager()->ClearPassword();
			}
			break;
		}
	
	}

}


//----------------------------------------------------------------------------
/**
*/
VOID			
InventoryDialog::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			POSTYPE RelativePos = GetRealSlotPos(pos);

			// 0~ INVEN_MAX_SLOT 까지 아이템 정보를 보여준다.
			if (RelativePos < this->GetMaxSlotNum() && 
				pos >= 0 && pos < INVEN_MAX_SLOT)
			{
                MouseOverProcess(RelativePos);
			}
			else
			{
				m_iTooltipStringCode = getTooltipString(pos);

				m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
			}			

		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
*/
void InventoryDialog::MouseOverProcess(POSTYPE RelativePos)
{
    if (!IsEmpty(RelativePos))
    {
        SCSlot & rSlot = GetSlot( RelativePos );
        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
        m_MouseOverObject.pSlot = &rSlot;
    }
    else
    {
        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
    }
}

//----------------------------------------------------------------------------
/**
*/
POSTYPE 
InventoryDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < INVEN_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return INVEN_MAX;
}


//----------------------------------------------------------------------------
/**
*/
CControlWZ * 
InventoryDialog::getControl( POSTYPE AtPos )
{
	if ( AtPos >= INVEN_MAX )
	{
		return NULL;
	}
	return GetControlWZ( m_wzId[ AtPos % INVEN_MAX ] );
}


//----------------------------------------------------------------------------
/**
*/
POSTYPE	
InventoryDialog::GetRealSlotPos( POSTYPE pos )
{
	return pos + m_TabNum;
}


//----------------------------------------------------------------------------
/**
*/
POSTYPE	
InventoryDialog::GetRelativeSlotPos( POSTYPE pos )
{
	return (pos % INVEN_MAX_SLOT);
}


//----------------------------------------------------------------------------
/**
*/
VOID 
InventoryDialog::UpdateQuickPos()
{
	return;
}

//----------------------------------------------------------------------------
/**
*/
void InventoryDialog::updateToolTip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	bool bMouseOver = false;
	for (POSTYPE a = 0; a < INVEN_MAX; a++)
	{
		CControlWZ * pControl = getControl(a);

		if(pControl)
		{
			RECT rc = pControl->GetSizeRect();

			if (rc.left <= iMouseX && rc.right >= iMouseX)
			{
				if (rc.top <= iMouseY && rc.bottom >= iMouseY)
				{
					bMouseOver = true;
					break;
				}
			}
		}
	}
    ProcessToolTip(bMouseOver, GetDialogKey());
}

void InventoryDialog::ProcessToolTip(bool bMouseOver, WzID dialog_key)
{
	if (!bMouseOver)
	{
		m_iTooltipStringCode = 0;
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
		return;
	}
	else
	{
		if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
		{
			if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
			{
				uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
				if(pUIMan)
				{
                    if (MouseHandler::Instance()->GetMouseState() == MouseHandler::CRYSTALIZATION)
                    {
                        if (m_MouseOverObject.pSlot->GetSlotType() == ST_ITEM)
                        {
                            pUIMan->RegisterTooltipCrystalization(static_cast<SCItemSlot*>(m_MouseOverObject.pSlot));
                        }
                    }
                    else
                    {
                        pUIMan->RegisterSlotInfoTooltip(m_MouseOverObject.pSlot);
                    }

					pUIMan->SetTooltipDialogID(dialog_key);
				}
			

				if( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
				{
					uiToolTipMan* pUIMan2 = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager( UIMAN_TOOLTIP2 );
					if( pUIMan2 )
					{
						pUIMan2->RegisterHeroItemInfoTooltipFromSlot( m_MouseOverObject.pSlot );
					}
				}


				MouseHandler::MOUSE_STATE eState = MouseHandler::Instance()->GetMouseState();

				if(MouseHandler::GUILD_WITHDRAW != eState &&
				   MouseHandler::GUILD_DEPOSIT != eState &&
				   MouseHandler::WITHDRAW != eState &&
				   MouseHandler::DEPOSIT != eState )
				{
					CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
				}
			}
		}
		else if (m_iTooltipStringCode == 11063)
		{
			/*개인 웨이포인트(%s)\n\n
			룬스톤을 활성화하면 남은 사용기간 동안 현재 위치를 저장하거나, 
            저장해 놓은 위치로 순간이동할 수 있습니다.*/

			// 개인 웨이포인트 툴팁
            uiToolTipMan* pTooltipMan = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
            if(!pTooltipMan)
            {
                return;
            }

            TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceStringFormat( szString,
                INTERFACE_STRING_LENGTH,
                m_iTooltipStringCode,
                m_hotkey[1]);

            std::vector<util::_tstring> vstrMsg;
            vstrMsg.clear();

            GlobalFunc::ProcessWordWrapByFontSize(&vstrMsg,
                szString,
                StrToWzID("mn12"), 
                _tcslen(szString), 
                200);

            for (size_t i=0; i<vstrMsg.size(); ++i)
            {
                TCHAR *pStr = const_cast<TCHAR *>(vstrMsg[i].c_str());

                pTooltipMan->RegisterTooltip( pStr,
                    -1,
                    23,
                    StrToWzID("mn12"),
                    WzColor_RGBA( 255, 255, 255, 255 ),
                    TOOLTIP_BG_WZCOLOR,
                    0);
                pTooltipMan->SetTooltipDialogID(dialog_key);
            }
#else
            uiPersonalWayPointMan* pPersonalWayPointMan = GET_DIALOG_MANAGER(uiPersonalWayPointMan, UIMAN_PERSONAL_WAYPOINT);
            if (pTooltipMan && pPersonalWayPointMan)
            {
                // 11061 - 이용 가능	,	11062 - 이용 불가능
                int nEnableCode = (pPersonalWayPointMan->GetActiveWaypointItem() != NULL) ? 11061 : 11062;
                pTooltipMan->RegisterPersonalWaypoint(nEnableCode);
                pTooltipMan->SetTooltipDialogID(dialog_key);
            }

#endif
		}
#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
        else if (m_iTooltipStringCode == 5589)
        {
			/*인벤토리 창 확대/축소(%s)\n\n
			내가 소유한 아이템을 한눈에 볼 수 있도록 창의 확대/축소가 가능하며, 
			소유한 아이템들을 자신이 원하는 위치에  쉽게 이동 시킬 수 있습니다.*/
            uiToolTipMan* pTooltipMan = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

            if(!pTooltipMan)
            {
                return;
            }

            TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceStringFormat( szString,
                INTERFACE_STRING_LENGTH,
                m_iTooltipStringCode,
                m_hotkey[0]);

            std::vector<util::_tstring> vstrMsg;
            vstrMsg.clear();

            GlobalFunc::ProcessWordWrapByFontSize(&vstrMsg,
                szString,
                StrToWzID("mn12"), 
                _tcslen(szString), 
                200);

            for (size_t i=0; i<vstrMsg.size(); ++i)
            {
                TCHAR *pStr = const_cast<TCHAR *>(vstrMsg[i].c_str());
               
                pTooltipMan->RegisterTooltip( pStr,
                    -1,
                    23,
                    StrToWzID("mn12"),
                    WzColor_RGBA( 255, 255, 255, 255 ),
                    TOOLTIP_BG_WZCOLOR,
                    0);
                pTooltipMan->SetTooltipDialogID(dialog_key);
            }
        }
#endif
		else if (m_iTooltipStringCode > 0)
		{
			uiToolTipMan* pUIMan = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
			if(pUIMan)
            {
				pUIMan->RegisterTooltipForWareNInventoryLock(m_iTooltipStringCode);
				pUIMan->SetTooltipDialogID(dialog_key);
            }
		}
	}
}
//----------------------------------------------------------------------------
/**
*/
void InventoryDialog::SetbEventInventory(BOOL bVal)
{
    m_bEventInventory = bVal;
    if (m_pExtendInventoryDialog != NULL)
    {
        m_pExtendInventoryDialog->SetEventInvenState((bVal != FALSE));
    }
}

void InventoryDialog::EnableWasteBasket()
{
	if ( MouseHandler::THROW_WASTE_BASKET != MouseHandler::Instance()->GetMouseState() )
	{
		MouseHandler::Instance()->SetMouseState(MouseHandler::THROW_WASTE_BASKET);

		// 큐에 휴지통정보를 넣는다.
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_delete_cancel;				
		msg.DoSomething=GlobalFunc::DoDeleteItemCancel;
		g_KeyQueueManager.PushBack(msg);
	}
}
#ifdef _NA_000000_20130114_RENEWER_UI
void InventoryDialog::UpdateExtraInventoryPage()
{
	int useInventoryCount = g_pHero->GetCharInfo()->m_ExtraInventoryTabCount;
	
	if (useInventoryCount != save_inventory_Extra_)
	{
		int Getsize = useInventoryCount - save_inventory_Extra_;
		int ExtraCount = 0;
		TCHAR isNum[3];

		for (int index = save_inventory_Extra_Next_ ; 
			index < save_inventory_Extra_Next_+ Getsize ; ++index)
		{
			memset(isNum,0,sizeof(TCHAR)*3);

			CControlWZ* TapAbleInvenExtra = getControl(INVEN_CHECK_CHARGE_1_PAGE + index);
			if (TapAbleInvenExtra)
			{
				sprintf(isNum,"%d",(index+1)+MAX_INVENTORY_TAB_NUM);
				TapAbleInvenExtra->EnableWindowWZ(true);
				TapAbleInvenExtra->SetTextWZA(isNum);
			}
			++ExtraCount;
		}

		save_inventory_Extra_Next_ += ExtraCount;
		save_inventory_Extra_ = useInventoryCount;
	}

}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
void InventoryDialog::CheckModifyOption()
{
    HOT_KEY_FUNCTION_INDEX hotkey_func_index_[] = 
    {
        //핫키와 ID가 매칭되게 넣는다
        kModeChange_ExtendInventory_UI,
        kOpen_personal_Warp_UI,
        kKeyFunctionMax,
    };
    
    for (int i = 0 ; i < 2 ; ++i)
    {
        if(hotkey_func_index_[i] == kKeyFunctionMax)
        {
            continue;
        }

        KEY_BIND_PAIR_MAP::iterator found_itr = 
            HotKeyManager::Instance()->current_using_key_bind_info().key_bind_pair_map().find(hotkey_func_index_[i]);

        KEY_BIND_PAIR temp_key_bind_pair = found_itr->second;

        STLX_STRING hot_key_string;
        DWORD string_code = 0;

        HOT_KEY_CODE hot_ket_code(temp_key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey));
        if (hot_ket_code.key_code_ != 0)
        {
            if (hot_ket_code.ctrl_key_ == 1)
            {
                hot_key_string += g_InterfaceManager.GetInterfaceString(8218);
                hot_key_string += " + ";
            }

            if (hot_ket_code.alt_key_ == 1)
            {
                hot_key_string += g_InterfaceManager.GetInterfaceString(8219);
                hot_key_string += " + ";
            }

            if (hot_ket_code.shift_key_ == 1)
            {
                hot_key_string += g_InterfaceManager.GetInterfaceString(8217);
                hot_key_string += " + ";
            }

            string_code = HotKeyInfoParser::Instance()->GetStringCodeByKeyCode(hot_ket_code.key_code_);

            hot_key_string += g_InterfaceManager.GetInterfaceString(string_code);
        }
        else
        {
            continue;
        }

        //핫키 툴팁을 위해 사용할 툴팁넣기.
        sprintf(m_hotkey[i],"%s",hot_key_string.c_str());
    }
}

void InventoryDialog::ImpossibleCrystalizationItemIconDeActiveRender()
{    
    for (int i = 0; i < GetMaxSlotNum(); ++i)
    {
        bool is_active = true;
        ItemUnitRenderer::RenderUnit * pRender = NULL;
        SCItemSlot& rSlot = (SCItemSlot&)SCSlotContainer::GetSlot(i);

        if (this->IsEmpty(i))
        {
            continue;
        }

        if (!rSlot.IsContainValidItem())
            continue;

        const BASE_ITEMINFO* selected_item_info_ptr = rSlot.GetItemInfo();

        if (selected_item_info_ptr == NULL)
        {
            continue;
        }

        if (rSlot.CanUseMixture(MIXTURE_CRYSTALIZE) == false)
        {
            is_active = false;
        }

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
		SLOTCODE slot_code = ItemCrystalizeListParser::Instance()->GetCrystalizeCode(rSlot, rSlot.GetEnchant());
#else
        SLOTCODE slot_code = ItemCrystalizeListParser::Instance()->GetCrystalizeCode(rSlot);
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
        if (slot_code == 0)
        {
            is_active = false;
        }

        sITEM_CRYSTALIZE_LIST* item_crystalize_list = ItemCrystalizeListParser::Instance()->GetCrystalizeListInfo(slot_code);
        if (item_crystalize_list == NULL)
        {
            is_active = false;
        }

        if (is_active == false)
        {
            pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );
            if (pRender)
            {
                pRender->SetState(SLOT_RENDER_STATE_DEACTIVATED);
            }
        }
    }
}

void InventoryDialog::RevertCrystalizationItemIconDeActiveRender()
{
    for (int i = 0; i < GetMaxSlotNum(); ++i)
    {
        bool is_active = true;
        ItemUnitRenderer::RenderUnit * pRender = NULL;
        SCItemSlot& rSlot = (SCItemSlot&)SCSlotContainer::GetSlot(i);

        if (this->IsEmpty(i))
        {
            continue;
        }

        if (!rSlot.IsContainValidItem())
            continue;

        const BASE_ITEMINFO* selected_item_info_ptr = rSlot.GetItemInfo();

        if (selected_item_info_ptr == NULL)
        {
            continue;
        }
        
        pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );
        if (pRender)
        {
            pRender->SetState(SLOT_RENDER_STATE_ACTIVATED);
        }
    }

    ItemCooltimeManager::Instance()->Release();
}

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
void InventoryDialog::ImpossibleAwakeningItemIonDeActiveRender()
{
	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		bool is_active = true;
		ItemUnitRenderer::RenderUnit * pRender = NULL;
		SCItemSlot& rSlot = (SCItemSlot&)SCSlotContainer::GetSlot(i);

		if (this->IsEmpty(i))
		{
			continue;
		}

		if (!rSlot.IsContainValidItem())
			continue;

		const BASE_ITEMINFO* selected_item_info_ptr = rSlot.GetItemInfo();

		if (selected_item_info_ptr == NULL)
		{
			continue;
		}

		RC::eITEM_RESULT result_code = RC_ITEM_FAILED;
		uiEquipmentAwakeningDialog* equipment_awakening_dialog = static_cast<uiEquipmentAwakeningDialog*>(ItemManager::Instance()->GetItemContainerDialog(SI_EQUIPMENT_AWAKENING));
		if ( equipment_awakening_dialog == NULL )
		{
			return;
		}

		if ( equipment_awakening_dialog->IsVisibleWindow() == FALSE )
		{
			return;
		}

		switch ( equipment_awakening_dialog->GetAwakeningUIMode() )
		{
		case uiEquipmentAwakeningDialog::AWAKENING_MODE_EQUIPMENT:
			{
				result_code = AwakeningInfoParser::Instance()->PermitEquipmentCheck(rSlot);
			}
			break;
		case uiEquipmentAwakeningDialog::AWAKENING_MODE_MATERIAL:
			{
				SCItemSlot* equip_item = ItemManager::Instance()->GetInventoryItem( equipment_awakening_dialog->get_equip_item_pos_() );
				if ( equip_item )
				{
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
					result_code = AwakeningInfoParser::Instance()->PermitMaterialCheck( (*equip_item).GetCode(), rSlot.GetCode(), rSlot.GetNum());
#else
					result_code = AwakeningInfoParser::Instance()->PermitMaterialCheck( (*equip_item).GetCode(), rSlot.GetCode());
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD

					if ( rSlot.IsShellItem() == true || rSlot.IsEtheriaItem() == true )
					{
						result_code = RC::RC_EQUIPMENT_AWAKENING_INVALID_MATERIAL_INFO;
					}
				}
			}
			break;
		case uiEquipmentAwakeningDialog::AWAKENING_MODE_ADDITIVE:
			{
				SCItemSlot* equip_item = ItemManager::Instance()->GetInventoryItem( equipment_awakening_dialog->get_equip_item_pos_() );
				SCItemSlot* material_item = ItemManager::Instance()->GetInventoryItem( equipment_awakening_dialog->get_material_item_pos_() );
				if ( equip_item && material_item )
				{
					result_code = AwakeningInfoParser::Instance()->PermitAdditiveCheck( *equip_item, *material_item, rSlot);
				}
			}
			break;
		case uiEquipmentAwakeningDialog::AWAKENING_MODE_COMPLETE:
			{
			}
			break;
		}

		if ( result_code != RC::RC_ITEM_SUCCESS )
		{
			is_active = false;
		}

		if (is_active == false)
		{
			pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );
			if (pRender)
			{
				pRender->SetState(SLOT_RENDER_STATE_DEACTIVATED);
			}
		}
	}
}

void InventoryDialog::RevertAwakeningItemIconDeActiveRender()
{
	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		bool is_active = true;
		ItemUnitRenderer::RenderUnit * pRender = NULL;
		SCItemSlot& rSlot = (SCItemSlot&)SCSlotContainer::GetSlot(i);

		if (this->IsEmpty(i))
		{
			continue;
		}

		if (!rSlot.IsContainValidItem())
			continue;

		const BASE_ITEMINFO* selected_item_info_ptr = rSlot.GetItemInfo();

		if (selected_item_info_ptr == NULL)
		{
			continue;
		}

		pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );
		if (pRender)
		{
			pRender->SetState(SLOT_RENDER_STATE_ACTIVATED);
		}
	}
}
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM

#endif
