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
// 리시브 패킷관련된 사항은 이곳으로
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

            WORD wIndex = pRecvMsg->m_wIndex;							// 이용하려는 포탈 idx
        
            sMAP_ENTERENCE_PORTAL* entPortal =
                MapInfoParser::Instance()->GetMapPortal( wIndex );

            if (entPortal)
            {
                if (g_pHero) 
                {
					if (!Strcmp(_T("WP01"), entPortal->sAreaToID))	// 웨이포인트로 이동
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

					// 아이템 제거
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
        
            // 이유에 대해서 자세히 설명해준다.
            // Verify 로 표시해줌.

            TCHAR   szMessage[INTERFACE_STRING_LENGTH];
            AskAirShipPortal* airshipportal_dialog =
                static_cast<AskAirShipPortal*>(GET_CAST_DIALOG(AskAirShipPortal, IM_ASSIST_MANAGER::AIRSHIP_PORTAL_DIALOG));

            if (airshipportal_dialog && airshipportal_dialog->AirshipErrorMessage(err))
            {
                /* 메시지 출력 
                11472	이동 가능한 레벨이 아닙니다.
                AirshipErrorMessage() 안에서 처리
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

			if(GetRqstFieldMoveMapCode() == 10003)	// 에텔레인 진입불가일때
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
