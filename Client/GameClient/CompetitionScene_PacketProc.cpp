#include "SunClientPrecompiledHeader.h"
#include "CompetitionScene.h"
#include "Map.h"
#include "BGMSoundInfoParser.h"
#include "MapInfoParser.h"
#include "NPCInfoParser.h"
#include "NPCPortalDialog.h"
#include "LoadScene.h"
#include "Hero.h"

#include "HeroActionInput.h"
#include "ItemManager.h"
#include "ObjectManager.h"

#include "uiBattleZoneWinDialog/uiBattleZoneWinDialog.h"
#include "uiBattleZoneCHuntingScoreBoard/uiBattleZoneCHuntingScoreBoard.h"
#include "uiBattleZoneDoing/uiBattleZoneDoing.h"

#include "uiHelpMan/uiHelpMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"

#include "uiBattleScoreMan\uiBattleScoreMan.h"
//----------------------------------------------------------------------------
/**
*/
void 
CompetitionScene::ParsePacket( MSG_BASE * pMsg )
{
    switch ( pMsg->m_byCategory )
    {
    case CG_CHUNTING:
        {
            switch (pMsg->m_byProtocol)
            {
            case CG_CHUNTING_ENTER_NEW_PARTY_BRD:
                {
                    MSG_CG_CHUNTING_ENTER_NEW_PARTY_BRD* recv =
                        (MSG_CG_CHUNTING_ENTER_NEW_PARTY_BRD *)pMsg;

					TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
					TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
                    TCHAR	szTemp2[INTERFACE_STRING_LENGTH] = {0,};

					uiBattleScoreMan* battle_score_manager_ptr = 
                        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
					if (battle_score_manager_ptr && 
                        (battle_score_manager_ptr->GetMode() == uiBattleScoreMan::eBTS_HUNTING))
					{
                        if (battle_score_manager_ptr->IsBattleScoreWinOpen() == true)  // 창이 열린 경우만 다시 요청하자
                        {
                            battle_score_manager_ptr->set_toggle_ui_open(true);
						    battle_score_manager_ptr->SendPacketBattleResultSYN();
                        }                        
						battle_score_manager_ptr->ResetHuntingData();
                        battle_score_manager_ptr->RefreshInfo(0, 0);
					}

                    BattleRoomEntry* entry = BattleScene::GetPendingBattleRoomEntry();
                    if (entry)
                    {
                        const ADDITIONAL_CHUNTINGINFO* info = entry->GetAdditionalCHuntingInfo(NULL);

                        if (info)
                        {
                            switch (info->m_Mode)
                            {
                            case eCOMPETITION_MODE_SURVIVAL:
                                {
                                    g_InterfaceManager.GetInterfaceString(9091,szTemp,INTERFACE_STRING_LENGTH);
                                    g_InterfaceManager.GetInterfaceString(9096,szTemp2,INTERFACE_STRING_LENGTH);

									Snprintf(szMessage, INTERFACE_STRING_LENGTH-1,  _T("%s\n%s"), szTemp, szTemp2);
                                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);

                                    g_InterfaceManager.GetInterfaceString(9097,szTemp,INTERFACE_STRING_LENGTH);
									Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp);
                                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
                                }
                                break;
                            default:
                            case eCOMPETITION_MODE_TIME_ATTACK:
                                {
                                    g_InterfaceManager.GetInterfaceString(9091,szTemp,INTERFACE_STRING_LENGTH);
                                    g_InterfaceManager.GetInterfaceString(9092,szTemp2,INTERFACE_STRING_LENGTH);
									Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s\n%s"), szTemp, szTemp2);
                                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);

                                    g_InterfaceManager.GetInterfaceString(9093,szTemp,INTERFACE_STRING_LENGTH);
									Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp);
                                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
                                }
                                break;
                            case eCOMPETITION_MODE_DEATH_MATCH:
                                {
                                    g_InterfaceManager.GetInterfaceString(9091,szTemp,INTERFACE_STRING_LENGTH);
                                    g_InterfaceManager.GetInterfaceString(9101,szTemp2,INTERFACE_STRING_LENGTH);
									Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s\n%s"), szTemp, szTemp2);
                                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);

                                    g_InterfaceManager.GetInterfaceString(9102,szTemp,INTERFACE_STRING_LENGTH);
									Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp);
                                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
                                }
                                break;
                            }
                        }
                    }

                    m_statusInfo.clear();
                    m_statusInfo.m_status = eCOMPETITION_SCENE_START_READY;
                    m_statusInfo.m_dwTickStart = clock_function::GetTickCount();

					if ( !m_winInfo.m_wWinPoint)
                        m_statusInfo.m_renderstatus = eCOMPETITION_RENDER_NONE;

                    m_winInfo.m_wKillPoint = 0; 
					m_winInfo.m_wOtherKillPoint = 0;

					uiBattleZoneDoing *pDialog = GET_CAST_DIALOG( uiBattleZoneDoing, IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_DOING );

					if( pDialog )
					{
						pDialog->SetKillPoint(m_winInfo.m_wKillPoint, m_winInfo.m_wOtherKillPoint);
					}

                    // disable PK
                    g_HeroInput.SetCannotAttack(TRUE);
                }
                break;

            case CG_CHUNTING_START_COMPETITION_BRD:
                {
                    m_statusInfo.m_status = eCOMPETITION_SCENE_START;

                    // enable PK
                    g_HeroInput.SetCannotAttack(FALSE);
                    g_HeroInput.SetAutoAttack(FALSE);

                    m_statusInfo.m_dwTickCompStart = clock_function::GetTickCount();
                }
                break;

			case CG_CHUNTING_TIE_COMPETITION_BRD:	// 무승부
				{
					m_statusInfo.m_status = eCOMPETITION_SCENE_NONE;
					m_statusInfo.m_renderstatus = eCOMPETITION_RENDER_TIE;

					m_winInfo.m_wKillPoint = 0;
					m_winInfo.m_wOtherKillPoint = 0;

					uiBattleZoneDoing * pDialog = GET_CAST_DIALOG( uiBattleZoneDoing, IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_DOING );
					if( pDialog )
					{
						pDialog->SetKillPoint(m_winInfo.m_wKillPoint, m_winInfo.m_wOtherKillPoint);
					}

					TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
					//9090	양 팀이 무승부로 처리되었습니다.
					g_InterfaceManager.GetInterfaceString(9090,szMessage,INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
					uiBattleScoreMan* battle_score_manager_ptr = 
                        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
					if(battle_score_manager_ptr && 
                        (battle_score_manager_ptr->GetMode() == uiBattleScoreMan::eBTS_HUNTING))
					{
                        battle_score_manager_ptr->set_toggle_ui_open(true);
						battle_score_manager_ptr->SendPacketBattleResultSYN();
					}
				}
				break;

            case CG_CHUNTING_WIN_COMPETITION_BRD:
                {
					MSG_CG_CHUNTING_WIN_COMPETITION_BRD* recv =
                        (MSG_CG_CHUNTING_WIN_COMPETITION_BRD *)pMsg;

					if(!g_pHero)
						return;

					m_winInfo.m_wWinPoint = recv->m_wWinPoint;
                    
					m_winInfo.m_PlusPVPPoint = recv->m_PlusPVPPoint;

					MONEY iMoney = g_pHero->GetMoney();
                    m_winInfo.m_CurMoney = recv->m_CurMoney - g_pHero->GetMoney();

                    g_pHero->SetMoney(recv->m_CurMoney);

                    CODETYPE dwCode = ItemManager::Instance()->InsertInventoryTotalInfo(REWARD, recv->m_ItemInfo);

					EXPTYPE i64Exp = g_pHero->GetExp();
                    m_winInfo.m_i64Exp = recv->m_i64Exp - g_pHero->GetExp();
                    g_pHero->SetExp(recv->m_i64Exp);

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
					uiBattleZoneWinDialog * winDialog = GET_CAST_DIALOG( uiBattleZoneWinDialog, IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_WIN );
					if( winDialog )
					{
						winDialog->ShowResultWin((int)m_winInfo.m_wWinPoint, (int)m_winInfo.m_i64Exp, (int)m_winInfo.m_CurMoney, dwCode, recv->m_ItemInfo.m_InvenCount);
					}
#endif//#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)

#ifdef _DEBUG
					TCHAR   szDebug[INTERFACE_STRING_LENGTH] = {0,};

					Snprintf (szDebug,INTERFACE_STRING_LENGTH-1,  _T("%d Win, Exp[%d], Money[%d], item[%d]"),
						m_winInfo.m_wWinPoint,
						m_winInfo.m_i64Exp,
						m_winInfo.m_CurMoney,
						recv->m_ItemInfo.m_InvenCount);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szDebug);					
#endif // _DEBUG

                    m_statusInfo.m_renderstatus = eCOMPETITION_RENDER_WIN;
                    m_statusInfo.m_status = eCOMPETITION_SCENE_NONE;

					uiBattleScoreMan* battle_score_manager_ptr = 
                        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
					if(battle_score_manager_ptr && 
                        (battle_score_manager_ptr->GetMode() == uiBattleScoreMan::eBTS_HUNTING))
					{
                        battle_score_manager_ptr->set_toggle_ui_open(true);
						battle_score_manager_ptr->SendPacketBattleResultSYN();
					}
                }
                break;

            case CG_CHUNTING_LOSE_COMPETITION_BRD:
                {
					m_ProcessDeathMessage = true;
                    MSG_CG_CHUNTING_LOSE_COMPETITION_BRD* recv =
                        (MSG_CG_CHUNTING_LOSE_COMPETITION_BRD *)pMsg;

                    m_statusInfo.m_renderstatus = eCOMPETITION_RENDER_LOSE;                    

                    m_winInfo.clear();

					TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
					g_InterfaceManager.GetInterfaceString(eST_CHUNTING_LOSE_COMPETITION,szMessage,INTERFACE_STRING_LENGTH);
            //      당신의 팀은 패배하였습니다!\n\n잠시 후 마을로 이동됩니다.
					uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if(pSystemMan)
						pSystemMan->Sys_Verify(szMessage);

					uiBattleScoreMan* battle_score_manager_ptr = 
                        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
					if(battle_score_manager_ptr && battle_score_manager_ptr->GetMode() == uiBattleScoreMan::eBTS_HUNTING)
					{
                        battle_score_manager_ptr->set_toggle_ui_open(true);
						battle_score_manager_ptr->SendPacketBattleResultSYN();
					}
                }
                break;

            case CG_CHUNTING_UPDATE_KILLPOINT_BRD:
                {
                    MSG_CG_CHUNTING_UPDATE_KILLPOINT_BRD* recv =
						(MSG_CG_CHUNTING_UPDATE_KILLPOINT_BRD *)pMsg;

					if(recv->m_wTeamKey == m_wMyTeamKey)
					{
						m_winInfo.m_wKillPoint = recv->m_wKillPoint;
					}
					else
					{
						m_winInfo.m_wOtherKillPoint = recv->m_wKillPoint;
					}

					uiBattleZoneDoing *pDialog = GET_CAST_DIALOG( uiBattleZoneDoing, IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_DOING );

					if( pDialog )
					{
						pDialog->SetKillPoint(m_winInfo.m_wKillPoint, m_winInfo.m_wOtherKillPoint);
					}
                }
                break;

			case CG_CHUNTING_TEAM_INFO_CMD:
				{
					MSG_CG_CHUNTING_TEAM_INFO_CMD* recv = (MSG_CG_CHUNTING_TEAM_INFO_CMD *)pMsg;

					m_wMyTeamKey = recv->m_wTeamKey;
					m_winInfo.m_wKillPoint = 0;
					m_winInfo.m_wOtherKillPoint = 0;
	
					uiBattleZoneDoing *pDialog = GET_CAST_DIALOG( uiBattleZoneDoing, IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_DOING );
					if( pDialog )
					{
						pDialog->SetKillPoint(m_winInfo.m_wKillPoint, m_winInfo.m_wOtherKillPoint);
					}
				}
				break;

			case CG_CHUNTING_UPDATE_POTIONCOUNT_BRD:
				{
					MSG_CG_CHUNTING_UPDATE_POTIONCOUNT_BRD* recv = (MSG_CG_CHUNTING_UPDATE_POTIONCOUNT_BRD*)pMsg;
				}
				break;

			case CG_CHUNTING_POINT_INFO_ACK:
				{
					MSG_CG_CHUNTING_POINT_INFO_ACK* recv = 
						(MSG_CG_CHUNTING_POINT_INFO_ACK*)pMsg;
				}
				break;

            default:
                BattleScene::ParsePacket( pMsg );
                break;
            }
        }
        break;
    
	case CG_STATUS:
		{
			switch( pMsg->m_byProtocol )
			{
			case CG_STATUS_RESURRECTION_BRD:
				{
					BattleScene::OnRecvCG_STATUS( pMsg );
					
					MSG_CG_STATUS_RESURRECTION_BRD	*pRecvPacket = (MSG_CG_STATUS_RESURRECTION_BRD *)pMsg;
					
					if ( g_pHero && (g_pHero->GetObjectKey() == pRecvPacket->m_dwObjectKey))
					{
						m_bDeadFlag = FALSE;
					}
				
					this->ResetProcessDeathMessage();
				}
				break;

			case CG_STATUS_RESURRECTION_TO_SAFEZONE_NAK:
				{
					MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_NAK *pRecvPacket = (MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_NAK *)pMsg;
					//pRecvPacket->m_byErrorCode 
					// 5364	부활을 할 수 없습니다.
					TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
					g_InterfaceManager.GetInterfaceString(5364,szMessage,INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;

			default:
				BattleScene::OnRecvCG_STATUS( pMsg );
				break;
			}
		}
		break;


    default:
        BattleScene::ParsePacket( pMsg );
        break;
    }
}
