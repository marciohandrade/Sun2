//------------------------------------------------------------------------------
//  uiVendorBuy_packetproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiVendorBuy.h"
#include "uiVendorSell/uiVendorSell_def.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Hero.h"
#include "ChatDialog.h"
#include "InventoryDialog.h"
#include "TargetDialog.h"
#include "ItemManager.h"
#include "uiVendorMan/uiVendorMan.h"
#include "globalfunc.h"
#include "InterfaceManager.h"

#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
#include "InventoryDialog.h"
#endif

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
#include "../uiVendorMan/uiVendorSearch/uiVendorSearch.h"
#endif
//------------------------------------------------------------------------------
/**
*/
void
uiVendorBuy::NetworkProc( MSG_BASE * pMsg )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];
	TCHAR	szResult[INTERFACE_STRING_LENGTH];

    switch (pMsg->m_byProtocol)
    {
    case CG_VENDOR_START_BRD:
        {
            MSG_CG_VENDOR_START_BRD *msg = (MSG_CG_VENDOR_START_BRD *)pMsg;

			DWORD dwKey = msg->m_dwPlayerKey;

            Player *pPlayer = (Player *)g_ObjectManager.GetObject( dwKey );

            if (pPlayer)
            {
                assert (pPlayer->IsKindOfObject(PLAYER_OBJECT));

                pPlayer->SetBeHaveState(PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE);

				TCHAR szVendorName[MAX_VENDOR_TITLE_LENGTH+1] = {0,};
                StrnCopy (szVendorName, msg->m_pszTitle, MAX_VENDOR_TITLE_LENGTH);

				pPlayer->SetMarketTitle(msg->m_pszTitle, MAX_VENDOR_TITLE_LENGTH);
				// '%s'님이 '%s' 개인 상점을 개설하였습니다.\n
				const int	c_iVENDOR_START_BRD = 70393;
				g_InterfaceManager.GetInterfaceString( c_iVENDOR_START_BRD, szMessage, INTERFACE_STRING_LENGTH );
				Snprintf(szResult, INTERFACE_STRING_LENGTH-1,szMessage,pPlayer->GetName(), szVendorName);

				if(GlobalFunc::IsParty(dwKey))
				{
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult);
				}
                
            }

        }
        break;

    case CG_VENDOR_END_BRD:
        {
            MSG_CG_VENDOR_END_BRD *msg = (MSG_CG_VENDOR_END_BRD *)pMsg;

			DWORD dwKey = msg->m_dwPlayerKey;

            Player *pPlayer = (Player *)g_ObjectManager.GetObject( dwKey );

            if (pPlayer)
            {
                assert (pPlayer->IsKindOfObject(PLAYER_OBJECT));

                pPlayer->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);

				//	'%s'님이 개인 상점을 닫으셨습니다.\n
				const int	c_iVENDOR_END_BRD = 70394;
				g_InterfaceManager.GetInterfaceString( c_iVENDOR_END_BRD, szResult, INTERFACE_STRING_LENGTH );
                Sprintf (szMessage, szResult, pPlayer->GetName());

				if(GlobalFunc::IsParty(dwKey))
				{
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
            }

            if (msg->m_dwPlayerKey == this->m_dwSalerKey)
            {
                if (this->IsVisible())
                    this->ShowInterfaceWithSound(FALSE);
            }
        }
        break;

    case CG_VENDOR_VIEW_START_ACK:
        {
            // 관람 시작
            // 1. 창 띄우고
            // 2. 아이템 채우기
			
            this->ShowInterfaceWithSound(TRUE);

            MSG_CG_VENDOR_VIEW_START_ACK *msg =
                (MSG_CG_VENDOR_VIEW_START_ACK *)pMsg;
			int i=0;

			BitStream bitstream(msg->m_BitStreamBuffer, msg->m_Size, FALSE);
			this->SCItemSlotContainer::Serialize( bitstream, eSLOT_SERIALIZE_CLIENT_SAVE );
			if(g_pHero)
				g_pHero->SetBeHaveState(PLAYER_BEHAVE_VENDOR_OBSERVER_STATE);

			ZeroMemory(m_szVendorMsg, sizeof(m_szVendorMsg));
			StrnCopy(m_szVendorMsg, msg->m_pszTitle, MAX_VENDOR_TITLE_LENGTH);

			uiVendorMan * puiVendorMan = GET_DIALOG_MANAGER(uiVendorMan, UIMAN_VENDOR);

			if (puiVendorMan)
			{
				m_dwSalerKey = puiVendorMan->GetPendingSaler();
			}

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
			uiVendorSearch * pVendorSearch = GET_CAST_DIALOG( uiVendorSearch, IM_VENDOR_MANAGER::VENDOR_SEARCH );
			if(pVendorSearch )
			{
				if(SCSlotContainer::IsEmpty(pVendorSearch->GetVendorSelectItem().BuySlotPos_))
				{
					//pVendorSearch->RewindSearchPacket();

					///TCHAR notice_message[INTERFACE_STRING_LENGTH] = {'\0', };
					//Sprintf(notice_message,"%s아이템은 판매되어 구매하실수 없습니다",pVendorSearch->GetVendorSelectItem().Item_Name_);
					//g_InterfaceManager.GetInterfaceStringFormat(notice_message,INTERFACE_STRING_LENGTH,
					//	2395,temp_buffer);

				//	uiSystemMan* system_manager = 
				//		static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));

				//	if (system_manager)
				//	{
				//		system_manager->Sys_VerifyLock(notice_message);
				//	}

					return;
				}
			}
#endif
        }
        break;

    case CG_VENDOR_VIEW_START_NAK:
        {
			MSG_CG_VENDOR_VIEW_START_NAK * msg = 
				( MSG_CG_VENDOR_VIEW_START_NAK *) pMsg;

			GetManager()->__errorcode(msg->m_byErrorCode);
        }
        break;

    case CG_VENDOR_VIEW_END_ACK:
        {
            MSG_CG_VENDOR_VIEW_END_ACK *msg =
                (MSG_CG_VENDOR_VIEW_END_ACK *)pMsg;

            this->ShowInterfaceWithSound(FALSE);

			if(g_pHero)
				g_pHero->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);
        }
        break;

    case CG_VENDOR_VIEW_END_NAK:
        {
            MSG_CG_VENDOR_VIEW_END_NAK *msg =
                (MSG_CG_VENDOR_VIEW_END_NAK *)pMsg;

			GetManager()->__errorcode(msg->m_byErrorCode);

            this->ShowInterfaceWithSound(FALSE);
        }
        break;

    case CG_VENDOR_BUY_ACK:
        {
            MSG_CG_VENDOR_BUY_ACK *msg =
                (MSG_CG_VENDOR_BUY_ACK *)pMsg;

			if(g_pHero)
				g_pHero->SetMoney(msg->m_Money);
        
			ItemManager::Instance()->InsertInventoryTotalInfo(BUY, msg->m_InventoryTotalInfo);

            this->m_curAmouts = 0;
            this->m_curMoney = 0;
        }
        break;

    case CG_VENDOR_BUY_NAK:
        {
            MSG_CG_VENDOR_BUY_NAK *msg =
                (MSG_CG_VENDOR_BUY_NAK *)pMsg;
			GetManager()->__errorcode(msg->m_byErrorCode);
        }
        break;

    case CG_VENDOR_BUY_BRD:
        {
			MSG_CG_VENDOR_BUY_BRD *msg =
				(MSG_CG_VENDOR_BUY_BRD *)pMsg;
#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
	
			uiSCItemSlot & rItemSlot = (uiSCItemSlot &) this->GetSlot( msg->m_VendorPos );
			int numAtSlot = rItemSlot.GetNum() - msg->m_SellNum;

			if( numAtSlot == 0)
			{
				this->DeleteSlot(msg->m_VendorPos, NULL);
			}
			else
			{
				rItemSlot.SetNum( numAtSlot );
				if( m_curPos == msg->m_VendorPos)
				{
					m_curMoney = rItemSlot.GetMoney();
					m_curAmouts = rItemSlot.GetNum();
				}
			}		
#else
           
            SCSlot FromSlot;
            this->DeleteSlot(msg->m_VendorPos, NULL);
#endif
        }
        break;

    case CG_VENDOR_DELETE_BRD:
        {
            MSG_CG_VENDOR_DELETE_BRD *msg =
                (MSG_CG_VENDOR_DELETE_BRD *)pMsg;

            this->ItemSub(msg->m_VendorPos);
        }
        break;

    case CG_VENDOR_MODIFY_REQ_BRD:
        {
            MSG_CG_VENDOR_MODIFY_REQ_BRD *msg =
                (MSG_CG_VENDOR_MODIFY_REQ_BRD *)pMsg;

            // 락킹 된 상태
            // 따라서 다른 아이콘과 다르게 보이게 설정해야함
        }
        break;

    case CG_VENDOR_INSERT_BRD:
        {
            MSG_CG_VENDOR_INSERT_BRD *msg =
                (MSG_CG_VENDOR_INSERT_BRD *)pMsg;

            if (!this->IsEmpty(msg->m_VendorPos))
            {
                SCSlot FromSlot;
                this->DeleteSlot(msg->m_VendorPos, NULL);
            }

            SCItemSlot itemSlot(msg->m_ItemSlot.m_Stream.m_Stream);
            uiSCItemSlot uiItemSlot = uiSCItemSlot( itemSlot );

            uiItemSlot.SetMoney(msg->m_ItemSlot.m_Money);
            this->InsertSlot(msg->m_VendorPos, (SCSlot &)uiItemSlot);
        }
        break;
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
