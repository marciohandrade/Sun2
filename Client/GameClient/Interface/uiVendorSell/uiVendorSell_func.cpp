//------------------------------------------------------------------------------
//  uiVendorSell_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiVendorMan/uiVendorMan.h"
#include "uiVendorSell.h"
#include "ItemUnitRenderer.h"
#include "SkillSlot.h"
#include "SkillInfoParser.h"
#include "StyleInfoParser.h"
#include "ItemFormula.h"
#include "MouseHandler.h"
#include "ItemManager.h"
#include "uiVendorPopup/uiVendorPopup.h"
#include "dummyitemSlot.h"
#include "InventoryDialog.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "Hero.h"
#include "uiVendorPopup/uiVendorPopup.h"
#include "uiVendorTitlePopup/uiVendorTitlePopup.h"
#include "GameFunc.h"

#include "StructInPacket.h"
//------------------------------------------------------------------------------
/** ITEM UNIT RENDERER
*/
void
uiVendorSell::InitItemRender()
{
    SAFE_RELEASENDELETE(m_pItemUnitRenderer);

    if (NULL == m_pItemUnitRenderer )
    {
        m_pItemUnitRenderer = new ItemUnitRenderer;
        m_pItemUnitRenderer->Init( uiBase::GetDrawBase(), GetMaxSlotNum() );
    }
}

//------------------------------------------------------------------------------
/** ITEM UNIT RENDERER
*/
void
uiVendorSell::ResetItemRenderer()
{
    SAFE_RELEASENDELETE(m_pItemUnitRenderer);

    this->InitItemRender();
}

//------------------------------------------------------------------------------
/** ITEM UNIT RENDERER
*/
ItemUnitRenderer * 
uiVendorSell::GetItemUnitRender() const
{
    return m_pItemUnitRenderer;
}

//------------------------------------------------------------------------------
/**
    working for slot clean
*/
void
uiVendorSell::FlushUnitDraw()
{
    SCSlot FromSlot;
    for (int i = 0; i < GetMaxSlotNum(); ++i)
    {
        if( !IsEmpty(i) )
        {
            DeleteSlot(i, NULL);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::ItemSub(POSTYPE pos)
{
    SCSlot FromSlot;
    DeleteSlot(pos, NULL);

    this->m_curAmouts = 0;
    this->m_curMoney = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::RecvItemSub(POSTYPE pos)
{
    this->ItemSub(pos);
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::QueryItemSub(POSTYPE pos)
{
    MSG_CG_VENDOR_DELETE_SYN sendMsg;
    sendMsg.m_VendorPos     = pos;

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof(sendMsg) );
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::VendingStartEnd()
{
    if (this->m_bStarted)
    {
        EndVendorSyn();
    }
    else
    {
		OpenVendorSyn();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::ItemUp(SLOTIDX fromSlotIdx,
                     SLOTIDX toSlotIdx,
                     POSTYPE fromPos,
                     POSTYPE toPos,
                     MONEY m)
{
    uiSCItemSlot& itemSlot = (uiSCItemSlot &)this->GetSlot(toPos);
    itemSlot.SetMoney(m);
    itemSlot.SetFromPos(fromPos);

    this->m_curMoney    = itemSlot.GetMoney();

    if (itemSlot.CanEquip())
    {
        this->m_curAmouts   = 1;
    }
    else
    {
        this->m_curAmouts   = itemSlot.GetNum();
    }
    this->m_curPos      = toPos;

    if (MouseHandler::Instance()->IsExistItemAtHand())
    {
        MouseHandler::Instance()->RollbackItemAtHand();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::ItemUpCencel(SLOTIDX fromSlotIdx,
                           SLOTIDX toSlotIdx,
                           POSTYPE fromPos,
                           POSTYPE toPos)
{
    SCItemSlot FromSlot;
    this->DeleteSlot(toPos, NULL);

    this->m_curAmouts = 0;
    this->m_curMoney = 0;

    if (MouseHandler::Instance()->IsExistItemAtHand())
    {
        MouseHandler::Instance()->RollbackItemAtHand();
    }
}


//------------------------------------------------------------------------------
/**
    개인 상점 시작 후 수정 / 팝업 띄우기
*/
void
uiVendorSell::OpenItemModifyPopupAfterVendingStarted(POSTYPE pos)
{
	uiVendorPopup *vendorPopup = GET_CAST_DIALOG( uiVendorPopup, IM_VENDOR_MANAGER::VENDOR_POPUP );
    vendorPopup->ShowWindow(TRUE);
    vendorPopup->SetInfos(SI_VENDOR_SELL,
        SI_VENDOR_SELL,
        pos,
        pos);
    vendorPopup->SetUseModify();
    vendorPopup->SetRequiredQuery();

    SCSlot& tSlot = this->GetSlot(pos);
    vendorPopup->SetItemSlot((SCItemSlot*)&tSlot);
}

//------------------------------------------------------------------------------
/**
    개인 상점 시작 전 수정 / 팝업 띄우기
*/
void
uiVendorSell::OpenItemModifyPopup(POSTYPE pos)
{
	uiVendorPopup *vendorPopup = GET_CAST_DIALOG( uiVendorPopup, IM_VENDOR_MANAGER::VENDOR_POPUP );
	vendorPopup->ShowWindow(TRUE);
    vendorPopup->SetInfos(SI_VENDOR_SELL,
        SI_VENDOR_SELL,
        pos,
        pos);
    vendorPopup->SetUseModify();

    SCSlot& tSlot = this->GetSlot(pos);
    vendorPopup->SetItemSlot((SCItemSlot*)&tSlot);
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::ItemModify(POSTYPE pos, MONEY money, bool isNetSend)
{
    if (isNetSend)
    {
        uiSCItemSlot& uiItemSlot = (uiSCItemSlot &)this->GetSlot(pos);
        this->m_PendingModifyItemSlot.m_Money = money;
        this->m_PendingModifyItemSlot.m_OrgPos = pos;
        this->m_PendingModifyItemSlot.m_VendorPos = pos;
    }
    else
    {
        uiSCItemSlot& uiItemSlot = (uiSCItemSlot &)this->GetSlot(pos);
        uiItemSlot.SetMoney(money);

        this->m_curMoney    = uiItemSlot.GetMoney();

        if (uiItemSlot.CanEquip())
        {
            this->m_curAmouts   = 1;
        }
        else
        {
            this->m_curAmouts   = uiItemSlot.GetNum();
        }
        this->m_curPos      = pos;
    }
}

//------------------------------------------------------------------------------
/**
    개인 상점 시작 전 수정 취소
*/
void
uiVendorSell::ItemModifyCancel()
{
}

//------------------------------------------------------------------------------
/**
    상점 아이템 슬롯 수정 요청
*/
void
uiVendorSell::QueryItemModifyRequest(POSTYPE pos)
{
    this->m_PendingModifyPos = pos;

    MSG_CG_VENDOR_MODIFY_REQ_SYN sendMsg;
    sendMsg.m_VendorPos     = this->m_PendingModifyPos;

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof(sendMsg) );
}

//------------------------------------------------------------------------------
/**
*/
bool
uiVendorSell::IsVendedItem(POSTYPE FromPos)
{
    InventoryDialog *pInven = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
        
    if (!pInven)
        return false;

    // check already uploaded
    for (int i=0; i<this->GetMaxSlotNum(); ++i)
    {
        if (!this->IsEmpty(i))
        {
            SCSlot& _slot = pInven->GetSlot(FromPos);
            SCSlot& fromSlot = this->GetSlot(i);
            if( _slot.GetSerial() == fromSlot.GetSerial() )
            {
                return true;
            }
        }
    }

    return false;
}

void uiVendorSell::setFormatMoney(CCtrlStaticWZ* pControl , MONEY money)
{
    if (pControl)
    {
        DWORD money_color = GlobalFunc::GetMoneyColor(money);
        pControl->SetTextColor(money_color);
        GlobalFunc::SetMoneyText(pControl, money);
    }
}

BOOL uiVendorSell::EnableOpenVendorState()
{
	if (g_pHero->GetBeHaveState() != PLAYER_BEHAVE_IDLE_STATE ||
		this->GetUIMan()->IsShowDialog(InterfaceManager::DIALOG_TRADE) ||
		this->GetUIMan()->IsShowDialog(InterfaceManager::DIALOG_BATTLEZONE_CREATE) ||
		this->GetUIMan()->IsShowDialog(InterfaceManager::DIALOG_BATTLEZONE_LIST) ||
		this->GetUIMan()->IsShowDialog(InterfaceManager::DIALOG_BATTLE_LOBBY) ||
		this->GetUIMan()->IsShowDialog(InterfaceManager::DIALOG_SHOP) )
	{
		this->ShowInterface(FALSE);
		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
		g_InterfaceManager.GetInterfaceString( eST_CANNOT_CREATE_VENDOR, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		return FALSE;
	}

	return TRUE;
}

void uiVendorSell::CloseSubWindow()
{
	uiBase* vendorPopup = GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_POPUP );
	assert (vendorPopup);
	if (vendorPopup)
	{
		vendorPopup->ShowWindow(FALSE);
	}

	uiBase* vendorTitlePopup = GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_TITLE_POPUP );
	assert (vendorTitlePopup);
	if (vendorTitlePopup)
	{
		vendorTitlePopup->ShowWindow(FALSE);
	}
}

void uiVendorSell::InitDataWhenShowWindow()
{
	this->OffPending();
	this->OffLock();

	this->m_curMoney = 0;
	this->m_curAmouts = 0;
	this->m_curPos = 0;
	this->m_bStarted = false;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	g_InterfaceManager.GetInterfaceString( eST_VENDOR_TEXT, szMessage, INTERFACE_STRING_LENGTH );
	Sprintf (this->m_szVendorMsg, _T("%s"), szMessage);

	CCtrlStaticWZ * pControl = (CCtrlStaticWZ*)getControl(DIALOG_EDIT_SELLMSG);
	if( pControl )
	{
		GlobalFunc::SetCaption( pControl, this->m_szVendorMsg );
		pControl->SetTextColor(WzColor_RGBA( 152, 208, 142, 255 ));
	}
	this->ResetItemRenderer();
}


void uiVendorSell::OpenVendorSyn()
{
	if (!this->IsPending())
	{
		ESTABLISHER_VENDOR_TOTAL_INFO m_ItemInfo;
		m_ItemInfo.m_Count = 0;
		for (int i = 0; i < this->GetMaxSlotNum(); ++i)
		{
			uiSCItemSlot& rSlot = (uiSCItemSlot &)this->GetSlot(i);

			if (rSlot.GetCode())
			{
                VENDOR_ITEMSLOT vitSlot;
                vitSlot.m_Money = rSlot.GetMoneyPerOne();
                vitSlot.m_VendorPos = i;
                vitSlot.m_OrgPos = rSlot.GetFromPos();
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
                memcpy (&m_ItemInfo.sell_items_[m_ItemInfo.m_Count].m_Slot, &vitSlot, sizeof(struct VENDOR_ITEMSLOT));

				TCHAR tempEnchanName[512];
				TCHAR ItemName[512];

				memset(tempEnchanName,0,sizeof(TCHAR)*512);

				int inchant = rSlot.GetEnchant();
				GameFunc::GetItemName(&rSlot,rSlot.GetItemInfo(),ItemName,512);
				Sprintf(tempEnchanName,"%s+%d",ItemName,inchant);

				sprintf(m_ItemInfo.sell_items_[m_ItemInfo.m_Count].szName, inchant > 0 ? tempEnchanName : ItemName );

                m_ItemInfo.m_Count++;
#else
				memcpy (&m_ItemInfo.m_Slot[m_ItemInfo.m_Count], &vitSlot, sizeof(struct VENDOR_ITEMSLOT));
				m_ItemInfo.m_Count++;
#endif//_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
			}
		}

		MSG_CG_VENDOR_START_SYN sendMsg;
		sendMsg.m_byCategory    = CG_VENDOR;
		sendMsg.m_byProtocol    = CG_VENDOR_START_SYN;
		_tcsncpy( sendMsg.m_pszTitle, m_szVendorMsg, MAX_VENDOR_TITLE_LENGTH );
#ifdef _INTERNATIONAL_UI
		if(g_slangNameFilter.IsHaveReplace(sendMsg.m_pszTitle))
		{
			//	금지 단어 사용
			char	szMessage[MAX_MESSAGE_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, MAX_MESSAGE_LENGTH );
			g_InterfaceManager.ShowMessageBox( szMessage, 3000, NULL, 0 );
			return;
		}
#endif//_INTERNATIONAL_UI
		memcpy (&sendMsg.m_ItemInfo, &m_ItemInfo, sizeof(struct ESTABLISHER_VENDOR_TOTAL_INFO));
		GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sendMsg.GetSize() );
		this->OnPending();
	}
}

void uiVendorSell::EndVendorSyn()
{
	if (this->m_bStarted)
	{
		if (!this->IsPending())
		{
			if(this->m_bShow)
			{
				MSG_CG_VENDOR_END_SYN sendMsg;
				sendMsg.m_byCategory    = CG_VENDOR;
				sendMsg.m_byProtocol    = CG_VENDOR_END_SYN;
				sendMsg.m_byEndKind		= eVENDOR_END_STATE_TERMINATE;

				GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof(sendMsg) );
			}
			else
			{
				this->m_bStarted = false;
			}

			this->OnPending();
		}
	}
}



void uiVendorSell::PauseVendorSyn()
{
	if (this->m_bStarted)
	{
		if (!this->IsPending())
		{
			MSG_CG_VENDOR_END_SYN sendMsg;
			sendMsg.m_byCategory    = CG_VENDOR;
			sendMsg.m_byProtocol    = CG_VENDOR_END_SYN;
			sendMsg.m_byEndKind		= eVENDOR_END_STATE_PAUSE;

			GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof(sendMsg) );
			this->OnPending();
		}
	}
}
void uiVendorSell::PauseButtonShowNHide()
{
	if( !m_pButtonPause )
		return;

	if( m_bStarted)
	{
		m_pButtonPause->SetTextColor(RGBA(227,179,27,255));
		m_pButtonPause->ShowWindowWZ(WZ_SHOW);
	}
	else
	{
		m_pButtonPause->ShowWindowWZ(WZ_HIDE);
	}
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
