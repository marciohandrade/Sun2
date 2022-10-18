#include "SunClientPrecompiledHeader.h"

#include "uiQuestMan.h"
#include "QuestManager_Concrete.h"
#include "ItemManager.h"
#include "uiQuestReward/uiQuestReward.h"
#include "MissionManager_Concrete.h"
#include "QuestTextInfoParser.h"
#include "ResultCode.h"
#include "SoundEffect.h"
#include "hero.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "uiChaoSystemMan/uiChaoSystemMan.h"
#include "GameFunc.h"
#include "uiQuestListMission/uiQuestListMission.h"
#include "uiHelpMan/uiHelpMan.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "uiBattleScoreMan\uiBattleScoreMan.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "TotalGuideParser.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "QuestPacket.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD
#include "uiQuestCommonForward/uiQuestCommonForward.h"
#endif // _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD

#include "ObjectManager.h"

using namespace RC;


//-------------------------------------------------------------------------------------------
/**
*/
void	        
uiQuestMan::NetworkProc( MSG_BASE * pMsg )
{
	if (CG_MISSION == pMsg->m_byCategory)
	{
		switch (pMsg->m_byProtocol)
		{
		case CG_MISSION_STREAM_CMD:
			{
				MSG_CG_MISSION_STREAM_CMD * pRecvMsg = (MSG_CG_MISSION_STREAM_CMD *)pMsg;
				assert( g_pMission_Concrete );
				if ( g_pMission_Concrete )
				{
					g_pMission_Concrete->SerializeStream(pRecvMsg->m_TotalInfo, SERIALIZE_STORE);					
				}
			}
			break;
		}
	}
	else 
	if ( CG_QUEST == pMsg->m_byCategory)  
	{
		switch (pMsg->m_byProtocol)
		{
		case CG_QUEST_STREAM_CMD:
			{
				MSG_CG_QUEST_STREAM_CMD * pRecvMsg = (MSG_CG_QUEST_STREAM_CMD *)pMsg;

				assert( g_pQMGR_Real );
				if ( g_pQMGR_Real )
				{
					g_pQMGR_Real->RecvQuestStream( pRecvMsg );

					UpdateQuestInfo(); 

                    m_Activated_Quest_Item.clear();
                    Quest * pQuest = NULL;
                    uiQuestMan * pQuestMan = 
                        (uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
                    for( int i = 1; i < MAX_QUEST_COMPL_NUM; ++i)
                    {
                        pQuest = g_pQMGR_Real->FindQuest(i);
                        if ( !pQuest )
                        {
                            continue; 
                        }
                        pQuestMan->UpdateQuestItem(i);//아이템 퀘스트 아이템 갱신추가
                    }                       
				}
			}
			break;

            // NPC로 부터 퀘스트를 수락할때
		case CG_QUEST_ACCEPT_ACK:
			{
				MSG_CG_QUEST_ACCEPT_ACK * pRecvMsg = (MSG_CG_QUEST_ACCEPT_ACK *)pMsg;
				UnsetPKBool(ePKBOOL_QUEST_ACCEPT);

				assert(m_PendingQuestCode[ePKBOOL_QUEST_ACCEPT] > 0 && 
					m_PendingQuestCode[ePKBOOL_QUEST_ACCEPT] < MAX_QUEST_COMPL_NUM);


				// Create 시 진행중인 퀘스트 스트림에 정보를 저장한다.
				Quest * pQuest = g_pQMGR_Real->CreateQuest(m_PendingQuestCode[ePKBOOL_QUEST_ACCEPT]);			
				assert(pQuest && g_pHero);
				if (pQuest && g_pHero)
				{
                    UpdateQuestItem(m_PendingQuestCode[ePKBOOL_QUEST_ACCEPT]);
					// [%s]퀘스트를 수락했습니다.(5902)
					GetUIMan()->GetInterfaceString(5902, m_szText, INTERFACE_STRING_LENGTH);
					// 퀘스트 네임
					GetUIMan()->GetInterfaceString(
						pQuest->GetQuestInfo()->GetQNCode(), 
						m_szMessage, 
						INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, m_szText, m_szMessage);

					RC::eQUEST_RESULT bRet = g_pQMGR_Real->CanDoAccept(
						m_PendingQuestCode[ePKBOOL_QUEST_ACCEPT], 
						g_pHero );
					assert( RC::RC_QUEST_SUCCESS == bRet);

					if(pQuest->IsTimeQuest())
					{
						//5386 수행시간 내에 퀘스트를 완료해야 합니다.
						QuestMessage(5386);
					}
                    DWORD accept_text_code = pQuest->GetQuestInfo()->GetAccept_Text();
                    QuestTextInfo* info = QuestTextInfoParser::Instance()->GetQuestTextInfo(accept_text_code);
                    if ( accept_text_code && info )
                    {
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, info->m_pszText);
                    }
					//완료조건이 아무것도 없는 퀘스트를 위해서 무조건 한번 이벤트를 호출한다.
					g_pQMGR_Real->OnEventForClient(QUEST_EVENT_ITEM, NULL, pQuest);
				}	
                
				this->m_pUIQuestReward->ShowInterfaceWithSound(FALSE);

				g_pQMGR_Real->Event( QUEST_EVENT_ITEM, NULL);

                // 우선 순위가 보상이 우위다.
                BOOL bReward = FALSE;
                bReward = UpdateAutoRewardRequest();

                if (!bReward)
                {
                    UpdateAutoAcceptRequest();
                }

				this->SetNaviQuest(pQuest);
				this->UpdateQuestInfo();

				// 사운드 출력
				g_SoundEffect.PlayUI( UI_SOUND_QUEST_ACCEPT );

#ifdef _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD
				if (this->m_pUIQuestCommonForward)
				{
					// 현재 수락해서 진행한 퀘스트와
					// 수락해야할 임무가 있는지 Check 해서 정보를 채운다.
					getQuestListsMeetNPC();
					m_pUIQuestCommonForward->ShowInterface(TRUE);
					if (m_pUIQuestCommonForward->AutoFirstRewardQuest() == false)
					{
						m_pUIQuestCommonForward->AutoclickFirstQuest();
					}
				}
#endif // _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD

			}
			break;

		case CG_QUEST_ACCEPT_NAK:
			{
				MSG_CG_QUEST_ACCEPT_NAK * pRecvMsg = (MSG_CG_QUEST_ACCEPT_NAK *)pMsg;
				UnsetPKBool(ePKBOOL_QUEST_ACCEPT);
				
				if ( pRecvMsg->m_byIsItemResult == 1)
				{
					ItemManager::Instance()->NoticeReasonItemErrorCode(pRecvMsg->m_byErrorCode);
				}
				else
				{
					GetUIMan()->GetStringQuestError( pRecvMsg->m_byErrorCode, m_szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, m_szMessage );			
				}				
			}
			break;

			// 퀘스트가 자동 수락 되었을 때
		case CG_QUEST_ACCEPT_CMD:
			{
				MSG_CG_QUEST_ACCEPT_CMD * pRecvMsg = (MSG_CG_QUEST_ACCEPT_CMD *)pMsg;
				pRecvMsg->m_QuestCode;

				assert(pRecvMsg->m_QuestCode > 0 && 
					   pRecvMsg->m_QuestCode < MAX_QUEST_COMPL_NUM);

				// Create 시 진행중인 퀘스트 스트림에 정보를 저장한다.
				Quest * pQuest = g_pQMGR_Real->CreateQuest(pRecvMsg->m_QuestCode);			
				assert(pQuest && g_pHero);
				if (pQuest && g_pHero)
				{
                    UpdateQuestItem(pRecvMsg->m_QuestCode);
					// [%s] 퀘스트가 자동 수락 되었습니다.(5903)
					GetUIMan()->GetInterfaceString(5903, m_szText, INTERFACE_STRING_LENGTH);

					// 퀘스트 네임
					GetUIMan()->GetInterfaceString(
						pQuest->GetQuestInfo()->GetQNCode(), 
						m_szMessage, 
						INTERFACE_STRING_LENGTH);

					GlobalFunc::OutputMsg_Notice(
						eOUTPUT_MSG_CHAT, 
						m_szText, 
						m_szMessage);

					RC::eQUEST_RESULT bRet = g_pQMGR_Real->CanDoAccept(
						pRecvMsg->m_QuestCode, 
						g_pHero );
					assert( RC::RC_QUEST_SUCCESS == bRet);
                                        
					if(pQuest->IsTimeQuest())
					{
						//5386 수행시간 내에 퀘스트를 완료해야 합니다.
						QuestMessage(5386);
					}

					//완료조건이 아무것도 없는 퀘스트를 위해서 무조건 한번 이벤트를 호출한다.
					g_pQMGR_Real->OnEventForClient(QUEST_EVENT_ITEM, NULL, pQuest);

                    assert(eQUEST_ACCEPT_GENERAL != pQuest->GetQuestInfo()->GetQuestAcceptType());
                    if (eQUEST_ACCEPT_GENERAL != pQuest->GetQuestInfo()->GetQuestAcceptType()) 
                    {
                        addAcceptAutoQuest(pQuest);
                    }
				}	

				m_pUIQuestReward->ShowInterfaceWithSound(FALSE);

				g_pQMGR_Real->Event( QUEST_EVENT_ITEM, NULL);

                // 우선 순위가 보상이 우위다.
                BOOL bReward = FALSE;
                bReward = UpdateAutoRewardRequest();

                if (!bReward)
                {
                    UpdateAutoAcceptRequest();
                }

				this->UpdateQuestInfo();

				// 사운드 출력
				g_SoundEffect.PlayUI( UI_SOUND_QUEST_ACCEPT );
			}
			break;
		case CG_QUEST_INCOMPLETE_ACK:
			{
                // no operation
			}
			break;

		case CG_QUEST_INCOMPLETE_NAK:
			{
                // no operation
			}
			break;

			// 퀘스트가 완료되었을 때
		case CG_QUEST_COMPLETE_ACK:
			{
				MSG_CG_QUEST_COMPLETE_ACK * pRecvMsg = (MSG_CG_QUEST_COMPLETE_ACK *)pMsg;

				UnsetPKBool(ePKBOOL_QUEST_COMPLETE);

				assert(pRecvMsg->m_QuestCode > 0 && 
					   pRecvMsg->m_QuestCode < MAX_QUEST_COMPL_NUM);

				DelPendingQuestCompletePacket(pRecvMsg->m_QuestCode);

                Quest * pQuest = g_pQMGR_Real->FindQuest( pRecvMsg->m_QuestCode );
				assert(pQuest);
				if ( pQuest && g_pHero )
				{
					assert(RC::RC_QUEST_SUCCESS == pQuest->CanComplete(g_pHero));
					pQuest->DoComplete(g_pHero);

					g_pQMGR_Real->Event(QUEST_EVENT_ITEM, NULL);	
					g_pQMGR_Real->OnEventForClient(QUEST_EVENT_ITEM, NULL, pQuest );

					DWORD dwCompleteText = pQuest->GetQuestInfo()->GetCompl_Text();

					QuestTextInfo * pInfo =
						QuestTextInfoParser::Instance()->GetQuestTextInfo(dwCompleteText);

					if ( dwCompleteText && pInfo )
					{
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, pInfo->m_pszText);
					}
				}	

                // 우선 순위가 보상이 우위다.
                BOOL bReward = FALSE;
                bReward = UpdateAutoRewardRequest();

                if (!bReward)
                {
                    UpdateAutoAcceptRequest();
                }

				UpdateQuestInfo();

				SEND_CG_COLLECT_ALLCOLLECTIVE_SYN();

				g_SoundEffect.PlayUI( UI_SOUND_QUEST_COMPLETE );

#ifdef _NA_008302_20150601_QUEST_COMPLET_SYN_RECAST
                refresh_quest_check_pending_ = false;
#endif //_NA_008302_20150601_QUEST_COMPLET_SYN_RECAST
			}
			break;

		case CG_QUEST_COMPLETE_NAK:
			{
				MSG_CG_QUEST_COMPLETE_NAK * pRecvMsg = (MSG_CG_QUEST_COMPLETE_NAK *)pMsg;

				UnsetPKBool(ePKBOOL_QUEST_COMPLETE);

				ClearPendingQuestCompletePacket();	// NAK에 퀘스트코드가 들어있지 않으므로 그냥 전체를 Clear하는 수 밖에 없다...

                GetUIMan()->GetStringQuestError( pRecvMsg->m_byErrorCode, m_szMessage, INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, m_szMessage );			

#ifdef _NA_008302_20150601_QUEST_COMPLET_SYN_RECAST
                if (refresh_quest_check_pending_ == false)
                {
                    SetRefreshQuestCheckTimer();
                    refresh_quest_check_pending_ = true;
                }
#endif //_NA_008302_20150601_QUEST_COMPLET_SYN_RECAST
			}
			break;

			// 보상 받을 때
		case CG_QUEST_REWARD_ACK:
			{
				MSG_CG_QUEST_REWARD_ACK * pRecvMsg = (MSG_CG_QUEST_REWARD_ACK *)pMsg;

				if( pRecvMsg->m_byFatigueType >= RC_FATIGUE_REWARD_OVER_THREE_HOURS )
				{
					g_InterfaceManager.GetInterfaceString( 6045, m_szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Error( static_cast<eOUTPUT_MSG_TYPE>( eOUTPUT_MSG_CHAT | eOUTPUT_MSG_TOP_SHOW ), m_szMessage );
				}

	            RemoveQuestItem(m_PendingQuestCode[ePKBOOL_QUEST_REWARD]);
				UnsetPKBool(ePKBOOL_QUEST_REWARD);

				if (g_pHero) 
				{
                    MONEY currentmoney = g_pHero->GetMoney();
					g_pHero->SetMoney(pRecvMsg->m_Money);

					EXPTYPE i64Exp = pRecvMsg->m_i64Exp - g_pHero->GetExp();
					if ( GameFramework::GetCurrentScene() && i64Exp)
					{
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION                        
                        GameFramework::GetCurrentScene()->PopupExp(i64Exp, pRecvMsg->m_BunusExpRatio);
#else
						GameFramework::GetCurrentScene()->PopupExp(i64Exp );	
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
					}

					g_pHero->SetExp( pRecvMsg->m_i64Exp );

#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
					uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));
					if (helpMan)
					{
						helpMan->ShowQuestHelp((int)pRecvMsg->m_wQuestCode,eHK_QUEST_REWARD);
					}
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW
                    uiGuideSystemMan* guide_manager =
                        static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

                    if (guide_manager)
                    {
                        guide_manager->CheckHelpMessage(TotalGuideNotice::kQuest, TotalGuideNotice::kCompleteQuest, pRecvMsg->m_wQuestCode);
                    }

			        uiBattleScoreMan* battle_score_manager_ptr = 
                        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
			        if (battle_score_manager_ptr)
			        {
                        uiBattleScoreMan::eBTSCROE_MODE bts_mode = battle_score_manager_ptr->GetMode();
                        if (bts_mode == uiBattleScoreMan::eBTS_MISSION || bts_mode == uiBattleScoreMan::eBTS_SSQ)
                        {
			    	        battle_score_manager_ptr->RefreshInfo(i64Exp, (g_pHero->GetMoney() - currentmoney));
                        }
			        }
				}
 
				ItemManager::Instance()->InsertInventoryTotalInfo( REWARD, pRecvMsg->m_ItemInfo );

				g_pQMGR_Real->DoReward( m_PendingQuestCode[ePKBOOL_QUEST_REWARD], g_pHero );

				DelQuestForProgress(m_PendingQuestCode[ePKBOOL_QUEST_REWARD]);


				m_pUIQuestReward->ShowInterface(FALSE);

				g_pQMGR_Real->Event(QUEST_EVENT_ITEM, NULL);

                // 우선 순위가 보상이 우위다.
                BOOL bReward = FALSE;
                bReward = UpdateAutoRewardRequest();

                if (!bReward)
				{
                    UpdateAutoAcceptRequest();
                }

				uiQuestListMission* pDlg = GET_CAST_DIALOG(uiQuestListMission, IM_QUEST_MANAGER::QUEST_LIST_MISSION);
				if(pDlg && pDlg->IsVisible())
				{
					pDlg->ShowInterface(FALSE);
				}
				UpdateQuestInfo();

				Quest* pComQ = g_pQMGR_Temp->FindQuest(pRecvMsg->m_wQuestCode);
				if (pComQ)
				{
					QUEST_CONDITION_QUEST_INFO* pInfo = (QUEST_CONDITION_QUEST_INFO*)pComQ->GetQuestInfo()->FindAcceptCondition( eQUEST_CONDITION_QUEST );
					if (pInfo)
					{
						for (int i = 0; i < pInfo->GetQuestArrayCount(); ++i)
						{
							QCODE QCode = pInfo->GetQuestCode(i);
							Quest* pQ = g_pQMGR_Real->FindQuest(QCode);
							if(pQ && (QS_NORMAL == pQ->GetQState()) && (RC::RC_QUEST_SUCCESS == pQ->CanComplete(g_pHero))) 
                            {
								NET_SEND_CG_QUEST_COMPLETE_SYN(pQ->GetQuestCode());
                            }
						}
					}
				}

#ifdef _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD
				if (this->m_pUIQuestCommonForward)
				{
					// 현재 수락해서 진행한 퀘스트와
					// 수락해야할 임무가 있는지 Check 해서 정보를 채운다.
					getQuestListsMeetNPC();
					m_pUIQuestCommonForward->ShowInterface(TRUE);
					if (m_pUIQuestCommonForward->AutoFirstRewardQuest() == false)
					{
						m_pUIQuestCommonForward->AutoclickFirstQuest();
					}
				}
#endif // _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD


				// 사운드 출력
				g_SoundEffect.PlayUI( UI_SOUND_QUEST_COMPLETE );

                uiCharacterStatusMan::ShowGainAccumulatePoint(pRecvMsg->m_wAccumulatePoint);
			}
			break;

		case CG_QUEST_REWARD_NAK:
			{
				MSG_CG_QUEST_REWARD_NAK * pRecvMsg = (MSG_CG_QUEST_REWARD_NAK *)pMsg;

				UnsetPKBool(ePKBOOL_QUEST_REWARD);

				if ( RC_QUEST_TOCOMPLETED == pRecvMsg->m_byErrorCode )
				{
					Quest * pQuest = 
						g_pQMGR_Real->FindQuest( m_PendingQuestCode[ePKBOOL_QUEST_REWARD] );
					if (pQuest)
					{
						pQuest->SetQState( QS_NORMAL );
					}					
				}

				GetUIMan()->GetStringQuestError( pRecvMsg->m_byErrorCode, m_szMessage, INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, m_szMessage );			
			}
			break;

		case CG_QUEST_FAILED_ACK:
			{
				MSG_CG_QUEST_FAILED_ACK * pRecvMsg = (MSG_CG_QUEST_FAILED_ACK *)pMsg;

				UnsetPKBool(ePKBOOL_QUEST_FAILED);

				assert(m_PendingQuestCode[ePKBOOL_QUEST_FAILED] > 0 && 
					m_PendingQuestCode[ePKBOOL_QUEST_FAILED] < MAX_QUEST_COMPL_NUM);

				Quest * pQuest = g_pQMGR_Real->FindQuest( m_PendingQuestCode[ePKBOOL_QUEST_FAILED] );
				assert(pQuest);
				if (pQuest)
				{
				
					g_pQMGR_Real->DoFailed(m_PendingQuestCode[ePKBOOL_QUEST_FAILED]);
				
					pQuest->SetQState(QS_FAILED);
	                RemoveQuestItem(m_PendingQuestCode[ePKBOOL_QUEST_FAILED]);
				}

				UpdateQuestInfo();

				SEND_CG_COLLECT_ALLCOLLECTIVE_SYN();
			}
			break;

		case CG_QUEST_FAILED_NAK:
			{
				MSG_CG_QUEST_FAILED_NAK * pRecvMsg = (MSG_CG_QUEST_FAILED_NAK *)pMsg;

				UnsetPKBool(ePKBOOL_QUEST_FAILED);

				GetUIMan()->GetStringQuestError( pRecvMsg->m_byErrorCode, m_szMessage, INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, m_szMessage );			

				UpdateQuestInfo();
			}
			break;

		case CG_QUEST_ABANDON_ACK:
			{
				MSG_CG_QUEST_ABANDON_ACK * pRecvMsg = (MSG_CG_QUEST_ABANDON_ACK *)pMsg;

				UnsetPKBool(ePKBOOL_QUEST_ABANDON);

				Quest * pQuest = g_pQMGR_Real->FindQuest(m_PendingQuestCode[ePKBOOL_QUEST_ABANDON]);
				assert(pQuest);
				if ( pQuest )
				{
	                RemoveQuestItem(m_PendingQuestCode[ePKBOOL_QUEST_ABANDON]);

					if(pRecvMsg->m_Force)
					{
						// 5388	퀘스트수행을 실패하였습니다.
						QuestMessage(5388);
					}
					else
					{
						GetUIMan()->GetInterfaceString( 
							pQuest->GetQuestInfo()->GetQNCode(), 
							m_szMessage, 
							INTERFACE_STRING_LENGTH);
						// 5905	[%s] 퀘스트가 삭제되었습니다.
						GetUIMan()->GetInterfaceString(5905, m_szText, INTERFACE_STRING_LENGTH);                    
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, m_szText, m_szMessage );	
					}
					pQuest->DoAbandon(g_pHero);

					DelQuestForProgress(m_PendingQuestCode[ePKBOOL_QUEST_ABANDON]);

					if(TRUE == GET_DIALOG(IM_QUEST_MANAGER::QUEST_COMMON_FORWARD)->IsVisible())
						GET_DIALOG(IM_QUEST_MANAGER::QUEST_COMMON_FORWARD)->ShowInterface(FALSE);

					if(TRUE == GET_DIALOG(IM_QUEST_MANAGER::QUEST_REWARD)->IsVisible())
						GET_DIALOG(IM_QUEST_MANAGER::QUEST_REWARD)->ShowInterface(FALSE);

					g_pQMGR_Real->DestroyQuest( m_PendingQuestCode[ePKBOOL_QUEST_ABANDON] );

                    RemoveKillMonQuest4Navi(m_PendingQuestCode[ePKBOOL_QUEST_ABANDON]);
				}
	
				UpdateQuestInfo();

				SEND_CG_COLLECT_ALLCOLLECTIVE_SYN();

				g_SoundEffect.PlayUI( UI_SOUND_QUEST_CANCEL );
			}
			break;

		case CG_QUEST_ABANDON_NAK:
			{
				MSG_CG_QUEST_ABANDON_NAK * pRecvMsg = (MSG_CG_QUEST_ABANDON_NAK *)pMsg;

				UnsetPKBool(ePKBOOL_QUEST_ABANDON);

				this->GetUIMan()->GetStringQuestError( pRecvMsg->m_byErrorCode, m_szMessage, INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, m_szMessage );	
			}
			break;

		case CG_QUEST_EVENT_AREA_ACK:
			{
				MSG_CG_QUEST_EVENT_AREA_ACK * pRecvMsg = (MSG_CG_QUEST_EVENT_AREA_ACK *)pMsg;

				UnsetPKBool(ePKBOOL_QUEST_EVENT_AREA);

				Quest * pQuest = g_pQMGR_Real->FindQuest(m_PendingQuestCode[ePKBOOL_QUEST_EVENT_AREA]);
				assert(pQuest);

				QUEST_COND_HASH& rConHash = pQuest->GetCompleteConditionHash();

				QUEST_COND_HASH_IT it, end( rConHash.end() );
				for( it = rConHash.begin() ; it != end ; ++it )
				{
					if ( eQUEST_CONDITION_AREA == (*it)->GetType() )
					{
						int Index = ((QUEST_CONDITION_AREA*)(*it))->CompleteArea(pRecvMsg->m_AreaID);
						if ( -1 != Index )
						{
							TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
							TCHAR szSucs[INTERFACE_STRING_LENGTH] = {0, };
							g_InterfaceManager.GetInterfaceString(pQuest->GetQuestInfo()->GetTISMissionText(Index), szMessage);
							g_InterfaceManager.GetInterfaceString(9051, szSucs);
							strcat(szMessage, szSucs);
							WzColor TextColor = TEXT_COLOR_QUEST_FOR_PREVIEW_COMPLETE;
							g_InterfaceManager.ShowCenterMessageBox(szMessage, 3000, &TextColor, FALSE);
						}					
					}					
				}			

				if(QS_NORMAL == pQuest->GetQState() && RC::RC_QUEST_SUCCESS == pQuest->CanComplete( g_pHero )) 
				{
					//QUEST_COMPLETE_SYN 보냄
					NET_SEND_CG_QUEST_COMPLETE_SYN(pQuest->GetQuestCode());
				}
				else if(QS_COMPLETED == pQuest->GetQState() && RC::RC_QUEST_SUCCESS != pQuest->CanComplete( g_pHero ))
				{
					pQuest->SetQState(QS_NORMAL);
				}
				else
				{
					g_SoundEffect.PlayUI( UI_SOUND_QUEST_COMPLETE );
				}

				// 연출이 있을수 있겠당.

				UpdateQuestInfo();
			}
			break;

		case CG_QUEST_EVENT_AREA_NAK:
			{
				MSG_CG_QUEST_EVENT_AREA_NAK * pRecvMsg = (MSG_CG_QUEST_EVENT_AREA_NAK *)pMsg;

				UnsetPKBool(ePKBOOL_QUEST_EVENT_AREA);

				this->GetUIMan()->GetStringQuestError( pRecvMsg->m_byErrorCode, m_szMessage, INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Error( eOUTPUT_MSG_CHAT, m_szMessage );	
			}
			break;

			// 몬스터 킬수 보내기
		case CG_QUEST_MONSTER_KILL_CMD:
			{
				MSG_CG_QUEST_MONSTER_KILL_CMD * pRecvMsg = (MSG_CG_QUEST_MONSTER_KILL_CMD *)pMsg;

				QUEST_MONSTER_KILL_MSG SendQuestMsg;
				SendQuestMsg.m_MonsterCode = pRecvMsg->m_MonsterCode;
				g_pQMGR_Real->Event( pRecvMsg->m_QuestCode, QUEST_EVENT_MONSTERKILL, &SendQuestMsg);

				UpdateQuestInfo();
			}
			break;

		case CG_QUEST_MONSTER_KILL_GRADE_CMD:
			{
				MSG_CG_QUEST_MONSTER_KILL_GRADE_CMD * pRecvMsg = (MSG_CG_QUEST_MONSTER_KILL_GRADE_CMD *)pMsg;

				QUEST_MONSTER_KILL_GRADE_MSG SendQuestMsg;
				SendQuestMsg.m_MonsterGrade = pRecvMsg->m_MonsterGrade;
				SendQuestMsg.m_MonsterLevel = pRecvMsg->m_MonsterLevel;
				g_pQMGR_Real->Event( pRecvMsg->m_QuestCode, QUEST_EVENT_MONSTERKILL_GRADE, &SendQuestMsg);

				UpdateQuestInfo();
			}
			break;


		case CG_QUEST_ITEM_OBTAIN_CMD:
			{
				MSG_CG_QUEST_ITEM_OBTAIN_CMD * pRecvMsg = (MSG_CG_QUEST_ITEM_OBTAIN_CMD *)pMsg;

				BOOL bRet = ItemManager::Instance()->InsertJustInventory(QUEST_OBTAIN, pRecvMsg->m_TotalInfo);
				assert(bRet);

				g_pQMGR_Real->Event( QUEST_EVENT_ITEM, NULL);

				UpdateQuestInfo();
			}
			break;

		case CG_QUEST_ITEM_DELETE_CMD:
			{
				MSG_CG_QUEST_ITEM_DELETE_CMD * pRecvMsg = (MSG_CG_QUEST_ITEM_DELETE_CMD *)pMsg;

				BOOL bRet = ItemManager::Instance()->DeleteInventoryTotalInfo( QUEST_DELETE, SI_INVENTORY, pRecvMsg->m_TotalInfo );
				assert(bRet);

				g_pQMGR_Real->Event( QUEST_EVENT_ITEM, NULL);

				UpdateQuestInfo();
			}
			break;


		case CG_QUEST_CHANGEofCLASS_CMD:
			{
				MSG_CG_QUEST_CHANGEofCLASS_CMD * pRecvMsg = ( MSG_CG_QUEST_CHANGEofCLASS_CMD *)pMsg;
				if( g_pHero )
				{
					g_pHero->GetCharInfo()->m_ChangeOfClassStep.Set(pRecvMsg->m_ChangeOfClassCode.Get());
					g_pHero->CreateEffectAtHead(StrToWzID("A585"),0.0f);

					//인벤 창의 캐릭터도 업데이트 해준다.
					Player * pPlayer = (Player *)g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY);
					if( pPlayer)
					{
						pPlayer->GetCharInfo()->m_ChangeOfClassStep.Set(pRecvMsg->m_ChangeOfClassCode.Get());
					}

					TCHAR szClassName[INTERFACE_STRING_LENGTH] ={0,}, szMessage[INTERFACE_STRING_LENGTH] = {0,};
					g_pHero->GetClassExName(szClassName);
					//6267	축하합니다. %s로 전직하였습니다.
					Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(6267) ,szClassName);
					GlobalFunc::OutputMsg_SysMsg(szMessage);

				}
			}
			break;

		case CG_QUEST_OBJECT_NAVI_POS_CMD:
			{
				

				MSG_CG_QUEST_OBJECT_NAVI_POS_CMD* pRecvMsg = (MSG_CG_QUEST_OBJECT_NAVI_POS_CMD*)pMsg;

				if(MSG_CG_QUEST_OBJECT_NAVI_POS_CMD::OPERATION_ALL_INFO == pRecvMsg->m_CodeType)
				{
					ClearKillMonQuest4Navi();
				}

				for(int i = 0; i < pRecvMsg->m_NumberOfNodes; ++i)
				{
					MSG_CG_QUEST_OBJECT_NAVI_POS_CMD::Node node = pRecvMsg->m_Nodes[i];
					POINT pt; 
					pt.x = node.posX; 
					pt.y = node.posY;
					SetKillMonQuest4Navi(node.questCode, node.npcCode, pt);
				}

#ifdef _DEV_VER
				//OSAN_0617:가변인자 %d를 넣어서 해외버전에서 뻗었음 
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CG_QUEST_OBJECT_NAVI_POS_CMD::Cnt"));
#endif 
			}
			break;

		case CG_QUEST_REWARD_CMD:
			{
				// 보상 처리 및 퀘스트 완료 처리
				MSG_CG_QUEST_REWARD_CMD * pRecvMsg = (MSG_CG_QUEST_REWARD_CMD *)pMsg;

				if( pRecvMsg->m_byFatigueType >= RC_FATIGUE_REWARD_OVER_THREE_HOURS )
				{
					g_InterfaceManager.GetInterfaceString( 6045, m_szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Error( static_cast<eOUTPUT_MSG_TYPE>( eOUTPUT_MSG_CHAT | eOUTPUT_MSG_TOP_SHOW ), m_szMessage );
				}


				if (g_pHero) 
				{
                    MONEY currentmoney = g_pHero->GetMoney();
					g_pHero->SetMoney(pRecvMsg->m_Money);

					EXPTYPE i64Exp = pRecvMsg->m_i64Exp - g_pHero->GetExp();
					if ( GameFramework::GetCurrentScene() && i64Exp)
					{
						GameFramework::GetCurrentScene()->PopupExp(i64Exp );	
					}

					g_pHero->SetExp( pRecvMsg->m_i64Exp );

#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
					uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));
					if (helpMan)
					{
						helpMan->ShowQuestHelp((int)pRecvMsg->m_wQuestCode,eHK_QUEST_REWARD);
					}
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW
                    uiGuideSystemMan* guide_manager =
                        static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

                    if (guide_manager)
                    {
                        guide_manager->CheckHelpMessage(TotalGuideNotice::kQuest, TotalGuideNotice::kCompleteQuest, pRecvMsg->m_wQuestCode);
                    }

			        uiBattleScoreMan* battle_score_manager_ptr = 
                        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
			        if (battle_score_manager_ptr)
			        {
                        uiBattleScoreMan::eBTSCROE_MODE bts_mode = battle_score_manager_ptr->GetMode();
                        if (bts_mode == uiBattleScoreMan::eBTS_MISSION || bts_mode == uiBattleScoreMan::eBTS_SSQ)
                        {
			    	        battle_score_manager_ptr->RefreshInfo(i64Exp, (g_pHero->GetMoney() - currentmoney));
                        }
			        }
				}
	            RemoveQuestItem(pRecvMsg->m_wQuestCode);
				g_pQMGR_Real->DoReward( pRecvMsg->m_wQuestCode, g_pHero );
				DelQuestForProgress( pRecvMsg->m_wQuestCode );
				g_pQMGR_Real->Event(QUEST_EVENT_ITEM, NULL);

				uiQuestListMission* pDlg = GET_CAST_DIALOG(uiQuestListMission, IM_QUEST_MANAGER::QUEST_LIST_MISSION);
				if(pDlg && pDlg->IsVisible())
				{
					pDlg->ShowInterface(FALSE);
				}
				UpdateQuestInfo();

				Quest * pQuest = g_pQMGR_Real->FindQuest( pRecvMsg->m_wQuestCode );
				if ( pQuest )
				{
					pQuest->DoComplete(g_pHero);

					g_pQMGR_Real->OnEventForClient(QUEST_EVENT_ITEM, NULL, pQuest );

					DWORD dwCompleteText = pQuest->GetQuestInfo()->GetCompl_Text();

					QuestTextInfo * pInfo =
						QuestTextInfoParser::Instance()->GetQuestTextInfo(dwCompleteText);

					if ( dwCompleteText && pInfo )
					{
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, pInfo->m_pszText);
					}
				}		

				// 사운드 출력
				g_SoundEffect.PlayUI( UI_SOUND_QUEST_COMPLETE );

                uiCharacterStatusMan::ShowGainAccumulatePoint(pRecvMsg->m_wAccumulatePoint);
			}
			break;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        case CG_QUEST_DAILYQUEST_PACKET:
            {
                RECV_QUEST_DAILYQUEST_PACKET(pMsg);
            }
            break;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
        case CG_QUEST_MISSION_CLEAR_CMD:
            {
                MSG_CG_QUEST_MISSION_CLEAR_CMD * pRecvMsg = (MSG_CG_QUEST_MISSION_CLEAR_CMD *)pMsg;

                QUEST_MISSION_CLEAR_MSG SendQuestMsg;
                SendQuestMsg.mission_code = pRecvMsg->map_code;
                g_pQMGR_Real->Event( pRecvMsg->quest_code, QUEST_EVENT_MISSION_CLEAR, &SendQuestMsg);

                UpdateQuestInfo();
            }
            break;
#endif // _NA_006607_20130402_ADD_QUESTTYPE
		}
	}
}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
void uiQuestMan::RECV_QUEST_DAILYQUEST_PACKET(MSG_BASE* recv_packet)
{
    MSG_CG_QUEST_DAILYQUEST_PACKET* daily_quest_packet = 
        static_cast<MSG_CG_QUEST_DAILYQUEST_PACKET*>(recv_packet);

    switch (daily_quest_packet->packet_type)
    {
    case CG_QUEST_DAILYQUEST_INFO:
        {
            RECV_QUEST_DAILYQUEST_INFO_PACKET(daily_quest_packet);
        }
        break;
    }
}

void uiQuestMan::RECV_QUEST_DAILYQUEST_INFO_PACKET(MSG_CG_QUEST_DAILYQUEST_PACKET* recv_packet)
{
    if (g_pQMGR_Real == NULL)
    {
        return;
    }

    MSG_CG_QUEST_DAILYQUEST_INFO_PACKET* daily_quest_packet = static_cast<MSG_CG_QUEST_DAILYQUEST_INFO_PACKET*>(recv_packet);
    switch (daily_quest_packet->packet_type2)
    {
    case CG_QUEST_DAILYQUEST_INFO_ALL_ACK:
        {
            MSG_CG_QUEST_DAILYQUEST_INFO_ALL_ACK* daily_quest_info_all = static_cast<MSG_CG_QUEST_DAILYQUEST_INFO_ALL_ACK*>(daily_quest_packet);
            g_pQMGR_Real->SerializeDailyQuestStream(&(daily_quest_info_all->daily_quest_data[0]), daily_quest_info_all->count_, SERIALIZE_STORE);
        }
        break;
    case CG_QUEST_DAILYQUEST_INFO_CMD:
        {
            MSG_CG_QUEST_DAILYQUEST_INFO_CMD* daily_quest_info_cmd = static_cast<MSG_CG_QUEST_DAILYQUEST_INFO_CMD*>(daily_quest_packet);

            // 일반 일일퀘스트 완료개수
            DWORD prev_today_complete_count = 0;
            g_pQMGR_Real->GetDailyQuestCompleteCountToday(DAILYQUEST_NORMAL, &prev_today_complete_count);
            // 길드 일일퀘스트 완료개수
            DWORD prev_guild_today_complete_count = 0;
            g_pQMGR_Real->GetDailyQuestCompleteCountToday(DAILYQUEST_GUILD, &prev_guild_today_complete_count);

            g_pQMGR_Real->SetDailyQuestData(&(daily_quest_info_cmd->daily_quest_info));
            RefreshNPCState();

#ifdef _NA_003027_20111013_HONOR_SYSTEM
            if (daily_quest_info_cmd->daily_quest_info.status_ == QS_COMPLETED)
            {
                // 남은 일일퀘스트 개수 출력
                TCHAR notice_message[INTERFACE_STRING_LENGTH + 1] = {'\0', };
                DWORD today_complete_count = 0;
                g_pQMGR_Real->GetDailyQuestCompleteCountToday(DAILYQUEST_NORMAL, &today_complete_count);
                if (today_complete_count != prev_today_complete_count)
                {
                    if (today_complete_count >= MAX_DAILYQUEST_PROGRESS_COUNT)
                    {
                        // 6498 금일 수행 할 수 있는 일일 퀘스트가 모두 끝났습니다.
                        g_InterfaceManager.GetInterfaceString(6498, notice_message);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, notice_message);
                    }
                    else
                    {
                        // 6497 오늘 수행 할 수 있는 일일 퀘스트가 %s개 남았습니다.
                        TCHAR remain_count[INTERFACE_STRING_LENGTH + 1] = {'\0', };
                        _itot((MAX_DAILYQUEST_PROGRESS_COUNT - today_complete_count), remain_count, 10);
                        g_InterfaceManager.GetInterfaceStringFormat(notice_message, 
                            INTERFACE_STRING_LENGTH, 
                            6497, 
                            remain_count);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, notice_message);
                    }
                }

                // 남은 길드 일일 퀘스트 개수 출력
                g_pQMGR_Real->GetDailyQuestCompleteCountToday(DAILYQUEST_GUILD, &today_complete_count);
                if (today_complete_count != prev_guild_today_complete_count)
                {
                    if (today_complete_count >= MAX_GUILD_DAILYQUEST_PROGRESS_COUNT)
                    {
                        // 7358 금일 수행 할 수 있는 길드 퀘스트가 모두 끝났습니다.
                        g_InterfaceManager.GetInterfaceString(7358, notice_message);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, notice_message);
                    }
                    else
                    {
                        // 7357 금일 수행 할 수 있는 길드 퀘스트가 %s개 남았습니다.
                        TCHAR remain_count[INTERFACE_STRING_LENGTH + 1] = {'\0', };
                        _itot((MAX_GUILD_DAILYQUEST_PROGRESS_COUNT - today_complete_count), remain_count, 10);
                        g_InterfaceManager.GetInterfaceStringFormat(notice_message, 
                            INTERFACE_STRING_LENGTH, 
                            7357, 
                            remain_count);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, notice_message);
                    }
                }
            }
#endif //_NA_003027_20111013_HONOR_SYSTEM 
        }
        break;
    case CG_QUEST_TODAYQUEST_INFO_CMD:
        {
            MSG_CG_QUEST_TODAYQUEST_INFO_CMD* today_quest_info_cmd = static_cast<MSG_CG_QUEST_TODAYQUEST_INFO_CMD*>(daily_quest_packet);
            g_pQMGR_Real->RecvGroupQuestInfo(today_quest_info_cmd);
            RefreshNPCState();
        }
        break;
#ifdef  _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
    case CG_QUEST_DAILYQUEST_RESET_ACK:
        {
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
            MSG_CG_QUEST_DAILYQUEST_RESET_ACK* pRecvMsg = static_cast<MSG_CG_QUEST_DAILYQUEST_RESET_ACK*>(daily_quest_packet);
            if (pRecvMsg )
            {
                TCHAR Date[5][256];

                sprintf(Date[0],"%d",pRecvMsg->item_use_time.GetYear());
                sprintf(Date[1],"%d",pRecvMsg->item_use_time.GetMonth());
                sprintf(Date[2],"%d",pRecvMsg->item_use_time.GetDay());
                sprintf(Date[3],"%d",pRecvMsg->item_use_time.GetHour());
                sprintf(Date[4],"%d",pRecvMsg->item_use_time.GetMinute());

                //사용날짜
                g_InterfaceManager.GetInterfaceStringFormat(szMessage,INTERFACE_STRING_LENGTH,5268,Date[0],Date[1],Date[2],Date[3],Date[4]);

                GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage );
            }
        }
        break;
    }
#endif
}
#endif //_NA_003027_20111013_HONOR_SYSTEM