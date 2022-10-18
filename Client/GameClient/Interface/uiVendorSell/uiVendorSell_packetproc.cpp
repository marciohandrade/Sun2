//------------------------------------------------------------------------------
//  uiVendorSell_packetproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiVendorSell.h"
#include "Hero.h"
#include "InventoryDialog.h"
#include "heroactioninput.h"
#include "ItemManager.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uivendorman/uiVendorMan.h"
//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::NetworkProc( MSG_BASE * pMsg )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];
    switch (pMsg->m_byProtocol)
    {
    case CG_VENDOR_START_ACK:
        {
			//	'%s' 개인 상점을 엽니다.
			g_InterfaceManager.GetInterfaceString( eST_VENDOR_START_BRD, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, this->m_szVendorMsg);
            this->OffPending();
            this->m_bStarted = true;
			if (g_pHero)
			{
				if( g_pHero->GetCurState() != STATE::SIT)
				{
					g_HeroInput.OnSkill(eACTION_SIT_AND_STAND + eACTION_START_SKILL_CODE);					
				}				
				g_pHero->SetMarketTitle(m_szVendorMsg,strlen(m_szVendorMsg));
			}			
			g_HeroInput.SetAckBeginVendor(TRUE);

        }
        break;

    case CG_VENDOR_START_NAK:
        {
			//	개인 상점 시작에 대한 NAK
			MSG_CG_VENDOR_START_NAK * pRecvMsg = (MSG_CG_VENDOR_START_NAK * ) pMsg;
			//7226	개인 상점 개설에 실패하였습니다.
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(7226));
			GetManager()->__errorcode(pRecvMsg->m_byErrorCode);
            this->OffPending();
            this->m_bStarted = false;
        }
        break;

    case CG_VENDOR_END_ACK:
        {
            this->m_bStarted = false;
            this->OffPending();


			MSG_CG_VENDOR_END_ACK * pRecvMsg = ( MSG_CG_VENDOR_END_ACK *) pMsg;
			if( pRecvMsg->m_byEndKind == eVENDOR_END_STATE_PAUSE )
			{
				// 개인 상점을 멈췄습니다.
			}
			else
			{
				this->ShowInterface(FALSE);
			}

        }
        break;

    case CG_VENDOR_END_NAK:
        {
			//	개인 상점 종료에 대한 NAK
			MSG_CG_VENDOR_END_NAK * pRecvMsg = (MSG_CG_VENDOR_END_NAK *) pMsg;
			GetManager()->__errorcode(pRecvMsg->m_byErrorCode);
            this->OffPending();
            this->ShowInterface(FALSE);
        }
        break;

    case CG_VENDOR_BUY_CMD:
        {

#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
			MSG_CG_VENDOR_BUY_CMD *msg =
				(MSG_CG_VENDOR_BUY_CMD *)pMsg;

			if(g_pHero)
				g_pHero->SetMoney(msg->m_Money);

			InventoryDialog *pInven = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
				
			uiSCItemSlot& itemSlot = (uiSCItemSlot &)this->GetSlot(msg->m_VendorPos);
			SCSlot & rInvenSlot = pInven->GetSlot(itemSlot.GetFromPos());
			int numAtInventory = rInvenSlot.GetNum() - msg->m_SellNum;
			int numAtSlot = itemSlot.GetNum() - msg->m_SellNum;

			if( numAtSlot == 0)
			{
				
				pInven->DeleteSlot(itemSlot.GetFromPos(), NULL);
				this->DeleteSlot(msg->m_VendorPos, NULL);
				this->m_curAmouts = 0;
				this->m_curMoney = 0;
			}
			else
			{
				rInvenSlot.SetNum(numAtInventory);
				itemSlot.SetNum(numAtSlot);
				this->m_curAmouts = itemSlot.GetNum();
				this->m_curMoney = itemSlot.GetMoney();
			}

			this->m_curPos =msg->m_VendorPos;



#else
            MSG_CG_VENDOR_BUY_CMD *msg =
                (MSG_CG_VENDOR_BUY_CMD *)pMsg;

			if(g_pHero)
				g_pHero->SetMoney(msg->m_Money);

            SCSlot invenDeleteSlot;
            InventoryDialog *pInven = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
                
            uiSCItemSlot& itemSlot = (uiSCItemSlot &)this->GetSlot(msg->m_VendorPos);
            pInven->DeleteSlot(itemSlot.GetFromPos(), NULL);

            SCSlot vendorDeleteSlot;
            this->DeleteSlot(msg->m_VendorPos, NULL);

            this->m_curAmouts = 0;
            this->m_curMoney = 0;
			this->m_curPos =msg->m_VendorPos;
#endif
        }
        break;

    case CG_VENDOR_MODIFY_REQ_ACK:
        {
            MSG_CG_VENDOR_MODIFY_REQ_ACK *msg =
                (MSG_CG_VENDOR_MODIFY_REQ_ACK *)pMsg;

            if (this->GetSlotNum())
            {
                this->OpenItemModifyPopupAfterVendingStarted(this->m_PendingModifyPos);
                this->m_bStarted = false;
            }
        }
        break;

    case CG_VENDOR_MODIFY_REQ_NAK:
        {
            MSG_CG_VENDOR_MODIFY_REQ_NAK *msg =
                (MSG_CG_VENDOR_MODIFY_REQ_NAK *)pMsg;
			GetManager()->__errorcode(msg->m_byErrorCode);
        }
        break;

    case CG_VENDOR_DELETE_ACK:
        {
            MSG_CG_VENDOR_DELETE_ACK *msg =
                (MSG_CG_VENDOR_DELETE_ACK *)pMsg;

            this->RecvItemSub(msg->m_VendorPos);
        }
        break;

    case CG_VENDOR_DELETE_NAK:
        {
			MSG_CG_VENDOR_DELETE_NAK * pRecvMsg = ( MSG_CG_VENDOR_DELETE_NAK * ) pMsg;
			GetManager()->__errorcode(pRecvMsg->m_byErrorCode);
        }
        break;

    case CG_VENDOR_INSERT_ACK:
        {
            MSG_CG_VENDOR_INSERT_ACK *msg = (MSG_CG_VENDOR_INSERT_ACK *)pMsg;

            uiSCItemSlot& uiItemSlot = (uiSCItemSlot &)this->GetSlot(this->m_PendingModifyItemSlot.m_VendorPos);
            uiItemSlot.SetMoney(this->m_PendingModifyItemSlot.m_Money);

            this->m_curMoney    = this->m_PendingModifyItemSlot.m_Money;

            if (uiItemSlot.CanEquip())
            {
                this->m_curAmouts   = 1;
            }
            else
            {
            }
            this->m_curPos      = this->m_PendingModifyItemSlot.m_VendorPos;
        }
        break;

    case CG_VENDOR_INSERT_NAK:
        {
			MSG_CG_VENDOR_INSERT_NAK * pRecvMsg = ( MSG_CG_VENDOR_INSERT_NAK * ) pMsg;
			GetManager()->__errorcode(pRecvMsg->m_byErrorCode);
        }
        break;

    default:
        break;
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
