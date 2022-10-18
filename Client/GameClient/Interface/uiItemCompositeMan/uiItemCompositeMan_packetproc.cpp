#include "SunClientPrecompiledHeader.h"
#include "uiItemCompositeMan.h"
//#include "uiItemComposite/uiItemComposite.h"
#include "ItemManager.h"
#include <SCItemSlot.h>
#include "hero.h"
#include "globalfunc.h"
#include "InterfaceManager.h"
#include "uiItemTotalComposite/uiItemTotalComposite.h"
#include "ItemCooltimeManager.h"
#include "SoundEffect.h"
//-------------------------------------------------------------------------------------------
/**
*/
void	        
uiItemCompositeMan::NetworkProc( MSG_BASE * pMsg )
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
	if ( pMsg->m_byCategory == CG_ITEM )
	{
		switch (pMsg->m_byProtocol) 
		{
		case CG_ITEM_COMPOSE_SUCCESS_ACK:
			{
				this->UnsetPKBool(ePKBOOL_ITEM_COMPOSITE_COMPOSE);

				MSG_CG_ITEM_COMPOSE_SUCCESS_ACK * pRecvMsg = (MSG_CG_ITEM_COMPOSE_SUCCESS_ACK *)pMsg;

				// 메터리얼 소비
#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
#else
				spendTotalComposeMaterials();
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION

				if (g_pHero)
					g_pHero->SetMoney( pRecvMsg->m_Money );

				ItemManager::Instance()->InsertInventoryTotalInfo( CREATE, pRecvMsg->m_ItemInfo );
				

				if( pRecvMsg->m_byMakingNum > 1)
				{
					TCHAR buff[INTERFACE_STRING_LENGTH];
					int iFaileNum = pRecvMsg->m_byMakingNum - pRecvMsg->m_bySuccessNum;
					g_InterfaceManager.GetInterfaceString( 5728, buff, INTERFACE_STRING_LENGTH );
					
					//총 %d개 조합 후, %d개를 성공하고 %d개를 실패하였습니다. 5728
					Snprintf( szMessage, INTERFACE_STRING_LENGTH -1, buff, 
						pRecvMsg->m_byMakingNum, pRecvMsg->m_bySuccessNum,iFaileNum );
				}
				else
				{
					// 조합에 성공했습니다. 5703
					g_InterfaceManager.GetInterfaceString(5703 , szMessage , INTERFACE_STRING_LENGTH );
				}

				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW , szMessage );

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                if (pRecvMsg->m_byPreventNum > 0)
                {
                    // 7099 : 길드시설 효과가 발동되어 재료아이템이 소모되지 않았습니다.
                    g_InterfaceManager.GetInterfaceString(7099, szMessage);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
			}
			break;

		case CG_ITEM_COMPOSE_FAILED_ACK:
			{
				this->UnsetPKBool(ePKBOOL_ITEM_COMPOSITE_COMPOSE);

				MSG_CG_ITEM_COMPOSE_FAILED_ACK * pRecvMsg = (MSG_CG_ITEM_COMPOSE_FAILED_ACK *)pMsg;

				// 메터리얼 소비
#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
#else
                spendTotalComposeMaterials();
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
				
				if( m_pUIItemTotalComposite )
					m_pUIItemTotalComposite->updateInfo();

				if (g_pHero)
					g_pHero->SetMoney( pRecvMsg->m_Money );

				// 조합에 실패했습니다. 재료와 수수료가 모두 사라졌습니다. 5724
				g_InterfaceManager.GetInterfaceString(5724 , szMessage , INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW , szMessage );

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                if (pRecvMsg->m_byPreventNum > 0)
                {
                    // 7099 : 길드시설 효과가 발동되어 재료아이템이 소모되지 않았습니다.
                    g_InterfaceManager.GetInterfaceString(7099, szMessage);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

			}
			break;

		case CG_ITEM_COMPOSE_NAK:
			{
				this->UnsetPKBool(ePKBOOL_ITEM_COMPOSITE_COMPOSE);
				MSG_CG_ITEM_COMPOSE_NAK * pRecvMsg = (MSG_CG_ITEM_COMPOSE_NAK *)pMsg;
				

				ItemManager::Instance()->NoticeReasonItemErrorCode(pRecvMsg->m_byErrorCode);

			}
			break;


		case CG_ITEM_CRYSTALIZE_SUCCESS_ACK:	
			{
				this->UnsetPKBool(ePKBOOL_ITEM_COMPOSITE_CRYSTALIZE);
				MSG_CG_ITEM_CRYSTALIZE_SUCCESS_ACK * pRecvMsg = (MSG_CG_ITEM_CRYSTALIZE_SUCCESS_ACK *)pMsg;

				if (g_pHero)
					g_pHero->SetMoney( pRecvMsg->m_Money );

				ItemManager::Instance()->InsertInventoryTotalInfo( CREATE, pRecvMsg->m_ItemInfo );
			}
			break;

		case CG_ITEM_CRYSTALIZE_FAILED_ACK:
			{
				this->UnsetPKBool(ePKBOOL_ITEM_COMPOSITE_CRYSTALIZE);
				MSG_CG_ITEM_CRYSTALIZE_SUCCESS_ACK * pRecvMsg = (MSG_CG_ITEM_CRYSTALIZE_SUCCESS_ACK *)pMsg;

				if (g_pHero)
					g_pHero->SetMoney( pRecvMsg->m_Money );

				// 5722	결정화에 실패했습니다.
				g_InterfaceManager.GetInterfaceString(eST_FAILED_CRYSTALIZE , szMessage ,INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			}
			break;


		case CG_ITEM_CRYSTALIZE_NAK:
			{
				this->UnsetPKBool(ePKBOOL_ITEM_COMPOSITE_CRYSTALIZE);

				MSG_CG_ITEM_CRYSTALIZE_NAK * pRecvMsg = (MSG_CG_ITEM_CRYSTALIZE_NAK *)pMsg;
				
				ItemManager::Instance()->NoticeReasonItemErrorCode(pRecvMsg->m_byErrorCode);

                // 경고음 출력
                g_SoundEffect.PlayUI(UI_SOUND_ITEM_PUT_CRYSTALIZATION);
                
                // 하나라도 결정화의 실패하면 모두 쿨타임을 제자리로 돌려 놓는다.
                ItemCooltimeManager::Instance()->Release();
                // 5849 : 결정화가 모두 취소되었습니다.
                TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0, };
                g_InterfaceManager.GetInterfaceString(5849, temp_message);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, temp_message);
			}
			break;
		default:
			break;
		}
	}
}
