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

				// ���͸��� �Һ�
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
					
					//�� %d�� ���� ��, %d���� �����ϰ� %d���� �����Ͽ����ϴ�. 5728
					Snprintf( szMessage, INTERFACE_STRING_LENGTH -1, buff, 
						pRecvMsg->m_byMakingNum, pRecvMsg->m_bySuccessNum,iFaileNum );
				}
				else
				{
					// ���տ� �����߽��ϴ�. 5703
					g_InterfaceManager.GetInterfaceString(5703 , szMessage , INTERFACE_STRING_LENGTH );
				}

				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW , szMessage );

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                if (pRecvMsg->m_byPreventNum > 0)
                {
                    // 7099 : ���ü� ȿ���� �ߵ��Ǿ� ���������� �Ҹ���� �ʾҽ��ϴ�.
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

				// ���͸��� �Һ�
#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
#else
                spendTotalComposeMaterials();
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
				
				if( m_pUIItemTotalComposite )
					m_pUIItemTotalComposite->updateInfo();

				if (g_pHero)
					g_pHero->SetMoney( pRecvMsg->m_Money );

				// ���տ� �����߽��ϴ�. ���� �����ᰡ ��� ��������ϴ�. 5724
				g_InterfaceManager.GetInterfaceString(5724 , szMessage , INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW , szMessage );

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                if (pRecvMsg->m_byPreventNum > 0)
                {
                    // 7099 : ���ü� ȿ���� �ߵ��Ǿ� ���������� �Ҹ���� �ʾҽ��ϴ�.
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

				// 5722	����ȭ�� �����߽��ϴ�.
				g_InterfaceManager.GetInterfaceString(eST_FAILED_CRYSTALIZE , szMessage ,INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			}
			break;


		case CG_ITEM_CRYSTALIZE_NAK:
			{
				this->UnsetPKBool(ePKBOOL_ITEM_COMPOSITE_CRYSTALIZE);

				MSG_CG_ITEM_CRYSTALIZE_NAK * pRecvMsg = (MSG_CG_ITEM_CRYSTALIZE_NAK *)pMsg;
				
				ItemManager::Instance()->NoticeReasonItemErrorCode(pRecvMsg->m_byErrorCode);

                // ����� ���
                g_SoundEffect.PlayUI(UI_SOUND_ITEM_PUT_CRYSTALIZATION);
                
                // �ϳ��� ����ȭ�� �����ϸ� ��� ��Ÿ���� ���ڸ��� ���� ���´�.
                ItemCooltimeManager::Instance()->Release();
                // 5849 : ����ȭ�� ��� ��ҵǾ����ϴ�.
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
