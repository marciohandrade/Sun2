#include "SunClientPrecompiledHeader.h"
#include "VillageScene.h"
#include "NPC.h"
#include "ObjectManager.h"
#include "hero.h"
#include "NPCInfoParser.h"
#include "BattleLobby.h"
#include "LoadScene.h"
#include "ChatDialog.h"
#include "UserSlot.h"
#include "HeroActionInput.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiBattleZoneList/uiBattleZoneList.h"
#include "uiBattleZoneCreate/uiBattleZoneCreate.h"
#include "ItemManager.h"
#include "MapInfoParser.h"
#include "NPCPortalDialog.h"
#include "uiHelpMan/uiHelpMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "uiWayPointMan/uiWayPointMan.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "AskAirShipPortal.h"
#include "uiAssistMan/uiAssistMan.h"
//////////////////////////////////////////////////////////////////////////
// ���ú� ��Ŷ���õ� ������ �̰�����
//////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/**
*/
void 
VillageScene::ParsePacket( MSG_BASE * pMsg )
{
    switch( pMsg->m_byCategory )
	{	
	case CG_ZONE:
		{
			OnRecvCG_ZONE( pMsg );
		}
		break;

	default:
		{
			BattleScene::ParsePacket( pMsg );
		}
		break;
	}
}



//------------------------------------------------------------------------------
/**
*/
void
VillageScene::OnRecvCG_PARTY( MSG_BASE * pMsg )
{
}

//----------------------------------------------------------------------------
/** 
*/
void VillageScene::OnRecvCG_ZONE( MSG_BASE * pMsg )
{
	switch (pMsg->m_byProtocol)
	{
    case CG_ZONE_PORTAL_FIELD_MOVE_ACK:
        {
			SetRqstFieldMoveMapCode(0);

            MSG_CG_ZONE_PORTAL_FIELD_MOVE_ACK* pRecvMsg =
                (MSG_CG_ZONE_PORTAL_FIELD_MOVE_ACK *)pMsg;

            WORD wIndex = pRecvMsg->m_wIndex;							// �̿��Ϸ��� ��Ż idx
        
            sMAP_ENTERENCE_PORTAL* entPortal =
                MapInfoParser::Instance()->GetMapPortal( wIndex );

            if (entPortal)
            {
                if (g_pHero) 
                {
					if (!Strcmp(_T("WP01"), entPortal->sAreaToID))	// ��������Ʈ�� �̵�
					{
						uiWayPointMan* pWayPointMan = GET_DIALOG_MANAGER(uiWayPointMan, UIMAN_WAYPOINT);
						if(pWayPointMan)
						{
							if(entPortal->dwHeim)
							{
								g_pHero->SetMoney( g_pHero->GetMoney() - entPortal->dwHeim );
							}
						}
					}
					else
					{
						NPCPortalDialog* portalDlg =
							static_cast<NPCPortalDialog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_NPC_PORTAL));
						if (portalDlg)
						{
							if (portalDlg->GetPendingMoney())
							{
								g_pHero->SetMoney( g_pHero->GetMoney() - portalDlg->GetPendingMoney() );
								portalDlg->ClearPendingMoney();
							}
						}
					}

					g_pHero->GetCharInfo()->m_iRegion = entPortal->wFieldTo;

					// ������ ����
					if ( entPortal->bWaste_Item )
					{
						BOOL bRet = ItemManager::Instance()->SpendItem(
							SI_INVENTORY, 
							entPortal->wItemCode, 
							entPortal->wItemNum);

						assert(bRet);
					}		
				}

                
                g_LoadScene.SetGoingToAreaPos(StrToWzID(entPortal->sAreaToID));
#ifdef _NHN_USA_CHANNELING
				g_LoadScene.SetPortalFlag(TRUE);
#endif//_NHN_USA_CHANNELING

                g_LoadScene.SetNextSceneAfterLoaded( entPortal->wWorldTo, entPortal->wFieldTo );
                GameFramework::ChangeScene(SCENE_TYPE_LOAD);
            }
        }
        break;

    case CG_ZONE_PORTAL_FIELD_MOVE_NAK:
        {
            MSG_CG_ZONE_PORTAL_FIELD_MOVE_NAK* pRecvMsg = (MSG_CG_ZONE_PORTAL_FIELD_MOVE_NAK *)pMsg;

			NPCPortalDialog* portalDlg = static_cast<NPCPortalDialog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_NPC_PORTAL));
			if( portalDlg )
			{
				portalDlg->ClearPendingMoney();		
			}
            BYTE err = pRecvMsg->m_byErrorCode;
        
            // ������ ���ؼ� �ڼ��� �������ش�.
            // Verify �� ǥ������.

            TCHAR   szMessage[INTERFACE_STRING_LENGTH];
            AskAirShipPortal* airshipportal_dialog =
                static_cast<AskAirShipPortal*>(GET_CAST_DIALOG(AskAirShipPortal, IM_ASSIST_MANAGER::AIRSHIP_PORTAL_DIALOG));

            if (airshipportal_dialog && airshipportal_dialog->AirshipErrorMessage(err))
            {
                /* �޽��� ��� 
                11472	�̵� ������ ������ �ƴմϴ�.
                AirshipErrorMessage() �ȿ��� ó��
                */
            }
            else
            {
                if (g_InterfaceManager.GetStringZoneError(err, szMessage, INTERFACE_STRING_LENGTH ))
                {
                    uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
                    if(pSystemMan)
                        pSystemMan->Sys_Verify(szMessage);

                }
            }            

			if(GetRqstFieldMoveMapCode() == 10003)	// ���ڷ��� ���ԺҰ��϶�
			{
				uiHelpMan* helpMan =
						static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

				if (helpMan)
				{
					helpMan->CheckHelpMessage(eHK_ETHERAIN);
				}

				SetRqstFieldMoveMapCode(0);
			}
        }
        break;

	default:
		{
			BattleScene::OnRecvCG_ZONE( pMsg );
		}
		break;
	}
}
