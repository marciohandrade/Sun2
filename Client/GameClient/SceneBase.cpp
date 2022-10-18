//------------------------------------------------------------------------------
//  SceneBase.cpp
//  (C) 2005 Webzen 
//------------------------------------------------------------------------------


#include "SunClientPrecompiledHeader.h"
#include "SceneBase.h"
#include "ChatDialog.h"
#include "InterfaceManager.h"
#include "Camera.h"
#include "MouseCursor.h"
#include "ObjectManager.h"
#include "uiLoginMan/uiLoginMan.h"
#include "CryptManager_4Client.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Hero.h"
#include "ResultCode.h"
#include "uiHelpMan/uiHelpMan.h"
#include "uiChaoSystemMan/uiChaoSystemMan.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "ItemManager.h"
#include "HelpMessageParser.h"
#include "CursorChangeTransact.h"
#include "Application.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "GameFunc.h"
#include "uiQuestMan/uiQuestMan.h"
#include "SCItemSlot.h"
#include "WrapArmorKit.h"
#include "uitiredsystemman/uitiredsystemman.h"
#include "tiredsystemdialog.h"
#include "uiChatMan/uiChatMan_def.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"

#include <SCSlotContainer.h>
#include "InstanceDungeonScene.h"
#include "NPC.h"
#include "ssqscene.h"
#include "Pet.h"

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
#include "CharacterScene.h"
#include "CastLeaveFieldDialog.h"
#include <Client/3DEngine/3DRenderer/ManagerTexture.h>
#endif

#include "LoadingTextureInfoParser.h"
#ifdef __NA000000_070913_HACKSHIELD_INSTALLATION__
#include "HShield/HackShield.h"
#endif //__NA000000_070913_HACKSHIELD_INSTALLATION__

#include "BattleScene.h"
#include "XignCode/XignCodeApi.h"

#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "TotalGuideParser.h"

#include "Autobot/Condition/AutobotConditionManager.h"
#include "KBMoveCheckManagerInClient.h"
#include "DominationInfoParser.h"
#include "map.h"

#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "HonorSystem/HonorSystem.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include "uiCharacterStatus/uiCharacterStatusCurrencyInfo.h"
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#include "HotKeyManager.h"
#ifdef _DH_LAST_SELECTED_CHARACTER 
#include "uiCharacterSelectMan/uiCharacterSelectMan.h"
#endif//_DH_LAST_SELECTED_CHARACTER 
#include "CooltimeManager.h"
#include "HeroData.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemData.h"
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/uiChaosZoneHistory.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
#include "uiSkillMan/uiNewSkillMan.h"
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
#if USING_GAMEGUARD
#include "GameGuard/GameGuardApi.h"
#endif // USING_GAMEGUARD

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
#include "PacketStruct_MonsterBook.h"
#include "uiMonsterBookMan/uiMonsterBookMan.h"
#include "NPCInfoParser.h"
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
  #include "uiAttendanceCheck/uiAttendanceCheckMan.h"
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "uiSUNRankingMan/uiSUNRankingMan.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "uiNotificationsSystemMan/uiNotificationsSystemMan.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
int			g_iLoadingImage = 0;
//
//
//FTextParser		SceneBase::m_FTextParser;
//FTextSentence	SceneBase::m_FTextSentence;
//FTextRender		SceneBase::m_FTextRender;
//
//
//FTextParser		SceneBase::m_TipFTextPraser;
//FTextSentence	SceneBase::m_TipFTextSentence;
//FTextRender		SceneBase::m_TipFTextRender;
//


using namespace RC;


void _CallBack_BadFriend_Add(bool bYes) 
{
	if(GameFramework::GetCurrentScene())
	{
		if(bYes)
		{
			MSG_CG_CHAO_ADD_REVENGE_CHAR_SYN sync;
			sync.m_byCategory = CG_CHAO;
			sync.m_byProtocol = CG_CHAO_ADD_REVENGE_CHAR_SYN;

			StrnCopy(sync.m_ptszRevengeName, 
                GameFramework::GetCurrentScene()->GetRevengeName(), 
                MAX_CHARNAME_LENGTH);
            sync.m_ptszRevengeName[_countof(sync.m_ptszRevengeName) - 1] = '\0';
			if(g_pHero)
				g_pHero->SendPacket(&sync,sizeof(sync));
		}


		GameFramework::GetCurrentScene()->SetReadyDeadFlag(FALSE);
	}
}


void SceneBase::Release( void )
{
	ReleaseLoadingImg();
	this->m_bReadyDeadFlag = FALSE;
	m_bChangeImage = FALSE;
}

eFADE_STATE SceneBase::GetFadeState()
{
	return m_eFadeState;
}

void SceneBase::StartFadeIn()
{
	m_eFadeState=eFADE_STATE_IN;
	m_dwCurFadeTime=0;

	m_dwCurFadeTime+=10;

	float fRate=(float)m_dwCurFadeTime/1000.0f;
	fRate=min(1.0f,fRate);

	float fGammaRate=32.0f* (1.0f - fRate) + 255.0f* (fRate);
	SetGamma(DWORD(fGammaRate));
}

void SceneBase::ProcessFade(DWORD dwTick)
{
	if(m_eFadeState==eFADE_STATE_IN)
	{
		if(m_bFristRender)
		{
			m_bFristRender=FALSE;
			dwTick=0;
		}

		m_dwCurFadeTime+=dwTick;

		if(m_dwCurFadeTime>=1000)
		{
			m_eFadeState=eFADE_STATE_NONE;
		}
		float fRate=(float)m_dwCurFadeTime/1000.0f;
		fRate=min(1.0f,fRate);

		float fGammaRate = 32.0f * (1.0f - fRate) + 255.0f * (fRate);
		SetGamma(DWORD(fGammaRate));
	}
}

BOOL SceneBase::Process( DWORD dwTick )
{
    if (this->PeekChangeScene())
    {
        if (this->ProcessChangeSceneOverTick(dwTick) > this->GetChangeSceneTickOver())
        {
            this->ClearSceneReady();

            GameFramework::ChangeScene( SCENE_TYPE_LOAD );
        }
    }

    return TRUE;
}

void SceneBase::IncreaseLoadingProcess()
{
	m_fLoadingCount += 1.0f;
}

BOOL SceneBase::Render( DWORD dwTick )
{
	return TRUE;
}

void SceneBase::OnRecvCG_CHARINFO( MSG_BASE * pMsg )
{
	switch( pMsg->m_byProtocol )
	{

	case CG_SECURE_QUERY_RESOURCE_CHK_T2_CMD:
		{
			ISecureWorkNode::OnMsgRecvSignal::ResourceCheckT2( pMsg, 0 );
		}
		break;
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	case CG_CHARINFO_FATIGUE_CMD:
		{
			MSG_CG_CHARINFO_FATIGUE_CMD * pRecvMsg = ( MSG_CG_CHARINFO_FATIGUE_CMD *) pMsg;

			TiredSystemDialog* pTiredDlg = GET_CAST_DIALOG( TiredSystemDialog, IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM ); 

			if( pTiredDlg )
			{
				pTiredDlg->SetOffLineTime( pRecvMsg->m_wOfflineTime );
				pTiredDlg->SetTime(pRecvMsg->m_wPlayTime);
				pTiredDlg->SetUseFatigue( pRecvMsg->m_byType );
			}
		}
		break;
#endif
    case CG_CHARINFO_ACCUMULATE_POINT_CMD:
        {   
            uiCharacterStatusMan* character_status_manager = GET_DIALOG_MANAGER(uiCharacterStatusMan, UIMAN_CHARACTERSTATUS);
            SUN_ASSERT(character_status_manager);
            if (character_status_manager)
            {
                MSG_CG_CHARINFO_ACCUMULATE_POINT_CMD* recv_msg = (MSG_CG_CHARINFO_ACCUMULATE_POINT_CMD*)pMsg;
                uiCharacterStatusMan::ShowGainAccumulatePoint(recv_msg->m_wAccumulatePoint);
                character_status_manager->RECV_MSG_CG_CHARINFO_ACCUMULATE_POINT_CMD(recv_msg);
    #ifdef _DEV_VER
                GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("Recv MSG_CG_CHARINFO_ACCUMULATE_POINT_CMD : %d:%d , %d"), 
                    recv_msg->m_dwAccumulateTime/3600, (recv_msg->m_dwAccumulateTime/60)%60, recv_msg->m_wAccumulatePoint);
    #endif //_DEV_VER
            }
            uiGuideSystemMan* guide_manager =
                static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

            if (guide_manager)
            {
                guide_manager->CheckHelpMessage(TotalGuideNotice::kAccumulatePoint);
            }
        }
        break;

    case CG_CHARINFO_SKILL_COOL_TIME_CMD:
        {
            MSG_CG_CHARINFO_SKILL_COOL_TIME_CMD* cmd = static_cast<MSG_CG_CHARINFO_SKILL_COOL_TIME_CMD*>(pMsg);
            xGetCooltimeManager()->RestoreSkillCooltime(cmd->skill_cool_time);

            //xGetHeroData()->BackupSkillCooltimeFromPacket(cmd->skill_cool_time);
            //if (g_pHero != NULL)
            //{
            //    xGetHeroData()->RestoreSkillCooltimeToHero(NULL, g_pHero);
            //}
        }
        break;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    case CG_HONORSYSTEM_PACKET:
        {
            HonorSystem::HonorSystemPacketParser::ParsePacketHonorSystem(pMsg);
        }
        break;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    case CG_POINTWALLET_PACKET:
        {
            uiCharacterStatusCurrencyInfo* ui_currency_info = 
                GET_CAST_DIALOG(uiCharacterStatusCurrencyInfo, uiCharacterStatusCurrencyInfo::kDialog_ID);
            if (ui_currency_info != NULL)
            {
                ui_currency_info->NetworkProc(pMsg);
            }
        }
        break;
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

        // ĳ������ ��۾����������� ����
    case CG_CHARINFO_CHAR_ITEM_TOGGLE_CMD:
        {
            MSG_CG_CHARINFO_CHAR_ITEM_TOGGLE_CMD* pRecvPacket = (MSG_CG_CHARINFO_CHAR_ITEM_TOGGLE_CMD*)pMsg;
            xGetHeroData()->SetBadgeItemData( pRecvPacket->toggle_items_info_[kToggleItem_Badge] );
            xGetHeroData()->SetPCBangItemData( pRecvPacket->toggle_items_info_[kToggleItem_PCBang] );
            //g_pHero->RefreshBadgeItemEffect();
            break;
        }

    case CG_CHARINFO_CHAOS_ZONE_RECORD_INFOS_CMD:
        {
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            MSG_CG_CHARINFO_CHAOS_ZONE_RECORD_INFOS_CMD* message =
                static_cast<MSG_CG_CHARINFO_CHAOS_ZONE_RECORD_INFOS_CMD*>(pMsg);

            ChaosSystem::GetChaosZoneData().set_chaos_zone_record(&(message->chaos_zone_record_infos));
            uiChaosZoneHistory* chaos_zone_history = GET_CAST_DIALOG(uiChaosZoneHistory, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_HISTORY);
            if (chaos_zone_history)
            {
                chaos_zone_history->RefreshHistryInfo();
            }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        }
        break;
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	case CG_CHARINFO_SUN_RANKING_GRADE_PACKET:
		{
			MSG_CG_CHARINFO_SUN_RANKING_GRADE_PACKET* message = 
				static_cast<MSG_CG_CHARINFO_SUN_RANKING_GRADE_PACKET*>(pMsg);

			CLIENT_PLAYER_RANKING_GRADE_INFO client_player_ranking_grade_info;
			client_player_ranking_grade_info.ranking_type = message->ranking_type;
			client_player_ranking_grade_info.grade        = message->grade;
			xGetHeroData()->SetClientPlayerRankingGrade(client_player_ranking_grade_info);
		}
		break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
    case CG_CHARINFO_MONSTERBOOK_PACKET:
        {
#ifdef _NA_007086_20140318_MONSTERBOOK
            MSG_CG_CHARINFO_MONSTERBOOK_PACKET* pRecvMsg = (MSG_CG_CHARINFO_MONSTERBOOK_PACKET*) pMsg;
            switch (pRecvMsg->packet_type)
            {
            case MONSTERBOOK_SERIALIZE_CMD:
                {
                    MSG_CG_MONSTERBOOK_SERIALIZE_CMD* pRecvMsg = (MSG_CG_MONSTERBOOK_SERIALIZE_CMD*)pMsg;
                    
                    uiMonsterBookMan* monsterbook_man = GET_DIALOG_MANAGER(uiMonsterBookMan, UIMAN_MONSTERBOOK);
                    if (monsterbook_man)
                    {
                        monsterbook_man->ResetMonsterBookData();
                        monsterbook_man->ActiveMonsterIcon(pRecvMsg->monster_code, pRecvMsg->monster_code_count);
                        monsterbook_man->OnCharacterConnect();
                    }
                }
                break;
            case MONSTERBOOK_ADD_CMD:
                {
                    MSG_CG_MONSTERBOOK_ADD_CMD* pRecvMsg = (MSG_CG_MONSTERBOOK_ADD_CMD*)pMsg;
                    
                    uiMonsterBookMan* monsterbook_man = GET_DIALOG_MANAGER(uiMonsterBookMan, UIMAN_MONSTERBOOK);
                    if (monsterbook_man)
                    {
                        monsterbook_man->ActiveMonsterIcon(pRecvMsg->monster_code_, TRUE);

                        BASE_NPCINFO* monster_info = NPCInfoParser::Instance()->GetNPCInfo(pRecvMsg->monster_code_);
                        if (monster_info)
                        {
                            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
                            TCHAR name_string[INTERFACE_STRING_LENGTH] = {0,};
                            TCHAR result_string[INTERFACE_STRING_LENGTH] = {0,};
                            TCHAR grade_string[INTERFACE_STRING_LENGTH] = {0,};
                            g_InterfaceManager.GetInterfaceString(monster_info->m_NCode, name_string);
#ifdef _DEV_VER
                            //#ifdef _SCJ_SHOW_NPC_ID
                            if (CGeneralGameParam::Instance()->IsShowNpcId())
                            {
                                TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
                                StrnCopy(buffer_string, name_string, INTERFACE_STRING_LENGTH-1); 
                                Snprintf(name_string, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
                                    buffer_string, monster_info->m_MonsterCode);
                            }
                            //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER
                            BOOL bGrade = TRUE;
                            switch (monster_info->m_byGrade)
                            {
                            case eNPC_GENERAL: //11501	�Ϲ�
                            case eNPC_MAPOBJECT_NPC:
                                g_InterfaceManager.GetInterfaceString(11501, grade_string);
                                break;
                            case eNPC_ELITE: //11503	����
                                g_InterfaceManager.GetInterfaceString(11503, grade_string);
                                break;
                            case eNPC_LEADER: //11502	����
                                g_InterfaceManager.GetInterfaceString(11502, grade_string);
                                break;
                            case eNPC_MIDDLEBOSS: //11504	ĸƾ
                                g_InterfaceManager.GetInterfaceString(11504, grade_string);
                                break;
                            case eNPC_BOSS: //11505	����
                                g_InterfaceManager.GetInterfaceString(11505, grade_string);
                                break;
                            case eNPC_LUCKY_MONSTER: //11500	��Ű
                                g_InterfaceManager.GetInterfaceString(11500, grade_string);
                                break;
                            case eNPC_MERCHANT_NPC:
                                bGrade = FALSE;
                                break;
                            default:
                                break;
                            }

                            if (bGrade == TRUE)
                            {
                                Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, "%s(%s)",
                                    name_string, 
                                    grade_string);
                            }
                            else
                            {
                                Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, "%s",
                                    name_string);
                            }

                            // 3234	���� ������ %s�� ������ ��ϵǾ����ϴ�.
                            g_InterfaceManager.GetInterfaceStringFormat(
                                result_string, INTERFACE_STRING_LENGTH-1, 3234 , temp_string);
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, result_string);
                        }
                    }
                }
                break;
            }
#endif //_NA_007086_20140318_MONSTERBOOK
        }
        break;
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    case CG_CHARINFO_ATTENDANCE_INFO_PACKET:
        {
            MSG_CG_CHARINFO_ATTENDANCE_INFO_PACKET* pRecvMsg = (MSG_CG_CHARINFO_ATTENDANCE_INFO_PACKET*) pMsg;
            uiAttendanceCheckMan* manager = GET_DIALOG_MANAGER(uiAttendanceCheckMan, UIMAN_ATTENDANCE_CHECK);
            if (manager)
            {
                manager->set_recv_attendance_info(&pRecvMsg->attendance_infos);
            }
        }
        break;
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
	}
    
}

void SceneBase::OnRecvCG_MAP( MSG_BASE * pMsg )
{

}

void SceneBase::OnRecvCG_SYNC( MSG_BASE * pMsg )
{

}

void SceneBase::OnRecvCG_BATTLE( MSG_BASE * pMsg )
{

}

void SceneBase::OnRecvCG_SKILL( MSG_BASE * pMsg )
{

}


void SceneBase::OnRecvCG_CONVERSATION( MSG_BASE * pMsg )
{

}

void SceneBase::OnRecvCG_ETC( MSG_BASE * pMsg )
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
	switch (pMsg->m_byProtocol)
	{
	case CG_ETC_DEBUGINFO_CMD:
		{
			MSG_CG_ETC_DEBUGINFO_CMD * pRecvMsg = (MSG_CG_ETC_DEBUGINFO_CMD *)pMsg;

			ZeroMemory(szMessage, sizeof(szMessage));
			StrnCopy(szMessage, pRecvMsg->m_pszChatMsg, MSG_CG_ETC_DEBUGINFO_CMD::_MAX_CHATMSG_SIZE);

			ZeroMemory(szMessage, sizeof(szMessage));
			StrnCopy(szMessage, pRecvMsg->m_pszChatMsg, pRecvMsg->m_byMsgLength) ;

			TCHAR	szWideMessage[512];

			StrCopy( szWideMessage, szMessage );

			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szWideMessage);
			Object * pObject = g_ObjectManager.GetObject( pRecvMsg->m_dwObjectKey );
			if (pObject)
			{
				pObject->AddChatMessage(szWideMessage);
			}
		}
		break;

	case CG_ETC_BATTLE_OPEN_TIME_NOTICE_BY_CODE_BRD:
		{
#ifdef _LTJ_BLOCK_NOTICE_TOP_OF_TRIAL_TIME
            // no operation
#else
            MSG_CG_ETC_BATTLE_OPEN_TIME_NOTICE_BY_CODE_BRD * pRecvMsg = 
                (MSG_CG_ETC_BATTLE_OPEN_TIME_NOTICE_BY_CODE_BRD *)pMsg;

            g_InterfaceManager.GetInterfaceString(pRecvMsg->m_NoticeCode, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::default_OutputMsg( eOUTPUT_MSG_CHAT, szMessage);
            g_InterfaceManager.ShowMessageBox( szMessage, 3000, NULL, 0 );
#endif //_LTJ_BLOCK_NOTICE_TOP_OF_TRIAL_TIME
		}
		break;

	case CG_ETC_GET_BATTLE_OPEN_TIME_ACK:
		{
			const MSG_CG_ETC_GET_BATTLE_OPEN_TIME_ACK* pRecvMsg = (MSG_CG_ETC_GET_BATTLE_OPEN_TIME_ACK*)pMsg;
			if (pRecvMsg->m_eJoinState != eJION_TOP_STATE_NONE)
			{
				g_InstanceDungeonScene.SetJoinState(pRecvMsg->m_eJoinState);
				g_InstanceDungeonScene.SetStartCheckTime(pRecvMsg->m_JoinOverTime);
				g_InstanceDungeonScene.SetStartProcessTick(clock_function::GetTickCount());
			}

			g_SsqScene.Func_OnRecv_(pRecvMsg);
		}
		break;

	case CG_ETC_BUFF_EVENT_TIME_CONTROL_BRD:
		{
			MSG_CG_ETC_BUFF_EVENT_TIME_CONTROL_BRD* pRecvPacket = (MSG_CG_ETC_BUFF_EVENT_TIME_CONTROL_BRD*)pMsg;

			TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};

            Object_Map_Itr found_itr = g_ObjectManager.GetBegin();
            Object_Map_Itr end_itr = g_ObjectManager.GetEnd();
            for ( ; found_itr != end_itr; ++found_itr)
            {			
                Object* pObject = found_itr->second;
                if (pObject == NULL)
                {
                    continue;
                }

                eOBJECT_TYPE eObjectType = pObject->GetSCObject().GetObjectType();
                if (eObjectType != NPC_OBJECT)
                {
                    continue;
                }

                NPC* pNPC = static_cast<NPC*>(pObject);

				if(pNPC)
				{
					//�ش� �ʿ� BuffEvent NPC�� �ϳ���� ����.
					if(eNPC_TYPE_EVENT_BUFF == pNPC->GetExtraInfo()->m_eNPCTYPE)
					{
						if(MSG_CG_ETC_BUFF_EVENT_TIME_CONTROL_BRD::TC_START == pRecvPacket->m_TimeControl)
						{
							pNPC->SetNextState(STATE::NPC_SPECIAL_ACT_LOOP_STATE, g_CurTime);
							pNPC->SetSubTypeEffectShow(TRUE);

							//6351 �������� ���� ������ �����մϴ�. �ź��� ������ �ູ�� ���� �� �ֽ��ϴ�.
							g_InterfaceManager.GetInterfaceString(6351, szMessage);
						}
						else if(MSG_CG_ETC_BUFF_EVENT_TIME_CONTROL_BRD::TC_END == pRecvPacket->m_TimeControl)
						{
							pNPC->SetNextState(STATE::NPC_IDLE_STATE, g_CurTime);
							pNPC->SetSubTypeEffectShow(FALSE);

							//6352 �������� ���� ���� �������ϴ�. ������ ���� ������ϴ�.
							g_InterfaceManager.GetInterfaceString(6352, szMessage);
						}
						break;
					}
				}
			}

			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
		}
		break;

	case CG_ETC_PET_SUMMON_RETURN_BRD:
		{
			MSG_CG_ETC_PET_SUMMON_RETURN_BRD* pRecvPacket = (MSG_CG_ETC_PET_SUMMON_RETURN_BRD*)pMsg;

			Player* player = static_cast<Player*>(g_ObjectManager.GetObject(pRecvPacket->m_dwObjectKey));

			if (player == NULL)
			{
                break;
            }

			if (pRecvPacket->m_bIsSummon)
			{
                if (player->IsKindOfObject(PLAYER_OBJECT) &&
                    player->CreatePetSummon(pRecvPacket->m_wPetIndex))
                {
                    player->SetPetAttributes(0, TRUE, pRecvPacket->m_byFullness);	//������
                    player->SetPetAttributes(0, FALSE, pRecvPacket->m_Intimacy);	//ģ�е�
                    player->SetPetNameUse(pRecvPacket->m_bSetPetName);				//����ӻ��
                    if (pRecvPacket->m_bSetPetName)
                    {
                        player->SetPetName(pRecvPacket->m_PetName);
                    }
                }
			}
			else
			{
				player->DeletePetSummon();
			}

            if (player->IsHero() == false)
            {
                break;
            }

            // ���ΰ��϶��� ó��
            SCItemSlot& item_slot = static_cast<SCItemSlot&>(g_pHero->GetSlotContainer(SI_INVENTORY)->GetSlot(g_pHero->GetPetItemPos()));
            
            if (item_slot.IsContainValidItem() &&
                item_slot.IsPetItem())
            {
                // ���ΰ� ��ó��
                item_slot.SetUserBlocked(pRecvPacket->m_bIsSummon);
                // ���ΰ� �Ⱓ��ó��
                if (pRecvPacket->m_bIsSummon == FALSE)
                {
                    g_pHero->UpdateSkillAttr();
                    item_slot.EndQuantity();
                }    
            }
            
		}
		break;

	case CG_ETC_PET_STATUS_CHANGE_BRD:
		{
			MSG_CG_ETC_PET_STATUS_CHANGE_BRD* pRecvPacket = (MSG_CG_ETC_PET_STATUS_CHANGE_BRD*)pMsg;
			
			Player* pPlayer = (Player*)g_ObjectManager.GetObject(pRecvPacket->m_dwObjectKey);

			//���ΰ� & �÷��̾� ��ο��� �����
			if (pPlayer)
			{
				pPlayer->SetPetAttributes(pRecvPacket->m_ItemPos, pRecvPacket->m_bIsFullness, pRecvPacket->m_byValue);
			}
			
		}
		break;

	case CG_ETC_PET_RETURN_CMD:
		{
			MSG_CG_ETC_PET_RETURN_CMD * pRecvPacket = (MSG_CG_ETC_PET_RETURN_CMD*)pMsg;
			
			if (g_pHero)
			{
				//6391	�������� 0�� �Ǹ� PET ��ȯ �Ұ���
				TCHAR	szMessage[INTERFACE_STRING_LENGTH];
				g_InterfaceManager.GetInterfaceString(6391,szMessage);
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				g_pHero->DeletePetSummon();
				g_pHero->UpdateSkillAttr();
		
				//��Ⱓ��
                SCItemSlot& item_slot = static_cast<SCItemSlot&>(g_pHero->GetSlotContainer(SI_INVENTORY)->GetSlot(g_pHero->GetPetItemPos()));
                
                if (item_slot.IsContainValidItem() &&
                    item_slot.IsPetItem())
                {
                    item_slot.SetBlocked(FALSE);
                    item_slot.EndQuantity();
                }
                
			}
				
		}
		break;

	case CG_ETC_PET_SET_NAME_BRD:
		{
			//Player�� ���̸��� �����Ѵ�.
			MSG_CG_ETC_PET_SET_NAME_BRD* pRecvPacket =(MSG_CG_ETC_PET_SET_NAME_BRD*)pMsg;
			
			Player* pPlayer = (Player*)g_ObjectManager.GetObject(pRecvPacket->m_dwObjectKey);
			
			if (pPlayer)			
			{
				pPlayer->SetPetNameUse(TRUE);
				pPlayer->SetPetName(pRecvPacket->m_PetName);
			}
		}
		break;
    case CG_ETC_GAMEOPTION_LOAD_ACK:
        {
            MSG_CG_ETC_GAMEOPTION_LOAD_ACK* pRecvPacket = static_cast<MSG_CG_ETC_GAMEOPTION_LOAD_ACK*>(pMsg);

#if defined(_YMS_AUTOBOT_GAME)
            if (pRecvPacket->game_option_.option_type_ == GameOption::kAutoHunting)
            {

                ArchiveOfStream ar(pRecvPacket->game_option_.binary_data_, pRecvPacket->game_option_.binary_data_size_);
                AutobotConditionManager::Instance()->LoadFromBuffer(ar);
            }
#endif //#if defined(_YMS_AUTOBOT_GAME)

            if (pRecvPacket->game_option_.option_type_ == GameOption::kGuidePerChar)
            {
                uiGuideSystemMan* guide_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
                if (guide_manager)
                {
                    guide_manager->LoadLibraryData(&(pRecvPacket->game_option_));
                }
            }
            else if (pRecvPacket->game_option_.option_type_ == GameOption::kNoticePerChar)
            {

                uiGuideSystemMan* guide_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
                if (guide_manager)
                {
                    guide_manager->LoadRepeatData(&(pRecvPacket->game_option_));
                }
            }
            if ((pRecvPacket->game_option_.option_type_ == GameOption::kKeyCustomizing) || 
                (pRecvPacket->game_option_.option_type_ == GameOption::kKeyCustomizingPerChar))
            {
                HotKeyManager::Instance()->RecvHotKeyInfo(pRecvPacket->game_option_);                
            }

#ifdef _DH_LAST_SELECTED_CHARACTER 
            if (pRecvPacket->game_option_.option_type_ == GameOption::kLastSelectedChar)
            {
                uiCharacterSelectMan* select_manager = GET_DIALOG_MANAGER(uiCharacterSelectMan, UIMAN_CHARACTERSELECT);
                if (select_manager)
                {
                    select_manager->RecvLastSelect(pRecvPacket->game_option_);
                }
            }
#endif//_DH_LAST_SELECTED_CHARACTER
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
            if (pRecvPacket->game_option_.option_type_ == GameOption::kPerkListChar)
            {
                uiSkillMan* ui_skill_manager_ptr = GET_DIALOG_MANAGER(uiSkillMan, UIMAN_SKILL);
                if (ui_skill_manager_ptr)
                {
                    ui_skill_manager_ptr->RECV_Perk_List_ACK(pRecvPacket->game_option_);
                }
            }
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
        }
        break;
    case CG_ETC_GAMEOPTION_LOAD_NAK:
        {
            MSG_CG_ETC_GAMEOPTION_LOAD_NAK* pRecvPacket = static_cast<MSG_CG_ETC_GAMEOPTION_LOAD_NAK*>(pMsg);
            
            int a = pRecvPacket->error_code_;

            if ((pRecvPacket->option_type_ == GameOption::kKeyCustomizing) || 
                (pRecvPacket->option_type_ == GameOption::kKeyCustomizingPerChar))
            {
                HotKeyManager::Instance()->RecLoadNak(
                    static_cast<GameOption::Type>(pRecvPacket->option_type_));                
            }
        }
        break;
    case CG_ETC_GAMEOPTION_SAVE_ACK:
        {
            MSG_CG_ETC_GAMEOPTION_SAVE_ACK* pRecvPacket = static_cast<MSG_CG_ETC_GAMEOPTION_SAVE_ACK*>(pMsg);
#ifdef _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
#else
            if ((pRecvPacket->option_type_ == GameOption::kNoticePerChar) ||
                (pRecvPacket->option_type_ == GameOption::kGuidePerChar))
            {
                uiGuideSystemMan* guide_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
                if (guide_manager)
                {
                    //repeat �� library ���� ��Ŷ �� ���� �� ��Ŷ�� �ִ���
                    if (guide_manager->waiting_first_packet_recv() == false)
                    {
                        //ù��° �� ��Ŷ�̸� true�� ����
                        guide_manager->set_waiting_first_packet_recv(true);
                    }
                    else
                    {
                        //�ι�° �� ��Ŷ�̸� Ÿ�̸� �ʱ�ȭ
                        guide_manager->SetWaitingSaveTimer(0);
                    }
                }
            }
#endif //_NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
        }
        break;
    case CG_ETC_GAMEOPTION_SAVE_NAK:
        {
            MSG_CG_ETC_GAMEOPTION_SAVE_NAK* pRecvPacket = static_cast<MSG_CG_ETC_GAMEOPTION_SAVE_NAK*>(pMsg);
#ifdef _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
#else
            if ((pRecvPacket->option_type_ == GameOption::kNoticePerChar) ||
                (pRecvPacket->option_type_ == GameOption::kGuidePerChar))
            {
                uiGuideSystemMan* guide_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
                if (guide_manager)
                {
                    //repeat �� library ���� ��Ŷ �� ���� �� ��Ŷ�� �ִ���
                    if (guide_manager->waiting_first_packet_recv() == false)
                    {
                        //ù��° �� ��Ŷ�̸� true�� ����
                        guide_manager->set_waiting_first_packet_recv(true);
                    }
                    else
                    {
                        //�ι�° �� ��Ŷ�̸� Ÿ�̸� �ʱ�ȭ
                        guide_manager->SetWaitingSaveTimer(0);
                    }
                }
            }
#endif //_NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
        }
        break;
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	case CG_ETC_STATUE_INFO_ACK:
		{
			MSG_CG_ETC_STATUE_INFO_ACK* recv_packet = static_cast<MSG_CG_ETC_STATUE_INFO_ACK*>(pMsg);

			uiSUNRankingMan* sun_ranking_manager = GET_DIALOG_MANAGER(uiSUNRankingMan, UIMAN_SUNRANKING);
			
			if ( sun_ranking_manager )
			{
				sun_ranking_manager->AddStatueInfo(recv_packet->info.statue_index, recv_packet->info);
			}
		}
		break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	
	}

	
}

void SceneBase::OnRecvCG_GM( MSG_BASE * pMsg )
{

}

BOOL SceneBase::IsEnablePkScene()
{

	//�̱ۿ����� ������ �ǰ� �Ѵ�.
	if( GENERALPARAM->IsNet()==FALSE)
	{
		return TRUE;
	}

	if((GetType() == SCENE_TYPE_FIELD||
		GetType() == SCENE_TYPE_VILLAGE)&&
		g_isPkServer)
		return	TRUE;

	return FALSE;
}

BOOL SceneBase::InPVPScene()
{
    if ((GetType() == SCENE_TYPE_FREEPVP) ||
        (GetType() == SCENE_TYPE_BATTLEGROUND) ||
        (GetType() == SCENE_TYPE_DOMINATION)
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        || GetType() == SCENE_TYPE_GOLDRUSH
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
        || (GetType() == SCENE_TYPE_SONNENSCHEIN)
#endif //_NA_008334_20150608_SONNENSCHEIN
        )
    {
        return	TRUE;
    }

    return FALSE;
}

void SceneBase::OnRecvCG_Party( MSG_BASE * pMsg)
{

}



void SceneBase::OnRecvCG_CONNECTION( MSG_BASE * pMsg )
{
	TCHAR	szTitle[MAX_MESSAGE_LENGTH], szMessage[MAX_MESSAGE_LENGTH];

	switch( pMsg->m_byProtocol )
	{

	case CG_SECURE_QUERY_RESOURCE_CHK_T1_CMD:
		{
			ISecureWorkNode::OnMsgRecvSignal::ResourceCheckT1( pMsg, 0 );
		}
		break;

	case CG_SECURE_QUERY_BUILTIN_CHK_CMD:
		{
			ISecureWorkNode::OnMsgRecvSignal::BuiltinCodeCheck( pMsg, 0 );
			BYTE* pSendingMsg;
			WORD wSize;
			if( ISecureWorkNode::OnMsgSend::BuiltinCodeCheck( pSendingMsg, wSize ) )
				GlobalFunc::SendPacket( CI_GAMESERVERIDX, pSendingMsg, wSize );
		}
		break;


	case CG_CONNECTION_BILLING_ALARM_BRD:	// ���� �ð� ���� �˸�
		{
			MSG_CG_CONNECTION_BILLING_ALARM_BRD *pPacket = (MSG_CG_CONNECTION_BILLING_ALARM_BRD *)pMsg;

			//pPacket->dwRestTerm // ���� �ð� (��!!!!)
			ShowBillingAlarm(pPacket->dwRestTerm);
		}
		break;

	case CG_CONNECTION_DISCONNECT_CMD:
		{
			MSG_CG_CONNECTION_DISCONNECT_CMD *pPacket = (MSG_CG_CONNECTION_DISCONNECT_CMD *)pMsg;

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
			if ( RC_DISCONNECT_FOR_SERVER_SELECT_TRANSACTION == pPacket->m_dwErrorCode )
			{
				// �ش� ����ڵ�� ������ �ƴ�
				// loginfront connect �õ�
				g_isChangeServer = true;
				g_CharSelectScene.m_bConnected = false;
			}
			else				
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__
				ShowDisconnectAlarm(pPacket->m_dwErrorCode);
		}
		break;

	case CG_CONNECTION_RESTART_CMD:
		{
			MSG_CG_CONNECTION_RESTART_CMD * pPacket = (MSG_CG_CONNECTION_RESTART_CMD *)pMsg;

			//	���� ����! ����� �������� �ֽʽÿ�
			g_InterfaceManager.GetInterfaceString( eST_VERIFY, szTitle, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.GetInterfaceString( eST_SERVER_RESTERT_RECONNECT, szMessage, INTERFACE_STRING_LENGTH );

			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_VerifyLock(szMessage, TRUE);
		}
		break;

    case CG_PREPARE_WORLD_CONNECT_ACK:
		OnRecvCG_CONNECTION__CG_PREPARE_WORLD_CONNECT_ACK( pMsg );
        break;

    case CG_PREPARE_WORLD_CONNECT_NAK:
		OnRecvCG_CONNECTION__CG_PREPARE_WORLD_CONNECT_NAK( pMsg );
        break;
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
	case CG_CONNECTION_SERVER_SELECT_REQUEST_ACK:
		{
			// �������� ���� ó��
			MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_ACK* pPacket = (MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_ACK*)pMsg;
			switch ( pPacket->m_Result )
			{
			case RC_ROOM_SUCCESS:
				{
#ifdef _NHN_USA_CHANNELING
					GlobalFunc::SendHanReportPacket(602);//���ӳ������������ý����������̵�����(602)
#endif//_NHN_USA_CHANNELING(�Ϲ� ä�θ�:��ǥ)

					// ���� - ��Ʈ�� LoginMan�� ����
					uiLoginMan* pLoginMan = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);
					if (pLoginMan)
					{
						TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
						if ( pLoginMan->RECV_REAUTH_TOKENSTEAM(pPacket->m_ReAuthToken) )
						{
							CastLeaveFieldDialog * pDlg = 
								(CastLeaveFieldDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_DELAY_CASTING);
							if ( pDlg )
							{
								pDlg->ShowWindow(FALSE);
                                g_InterfaceManager.UnLockDlgs();
							}
							g_KeyQueueManager.DeleteMsg(eDoType_ServerSelectStandingBy);

							// 70516	��ø� ��ٷ� �ֽʽÿ�.
							g_InterfaceManager.GetInterfaceString(eST_PLEASE_WAIT_SORRY, szMessage, INTERFACE_STRING_LENGTH);
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
						}
						else
						{
							// ������ ��ū�� ��� �޽��� ���
							// 85122	���� ���°� ��Ȱ���� �ʽ��ϴ�. ��� �� �ٽ� �õ��� �ֽʽÿ�.
							g_InterfaceManager.GetInterfaceString(eST_SERVER_STATUS_NOT_SMOOTH, szMessage, INTERFACE_STRING_LENGTH);
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
						}

                        if (g_pHero && g_pHero->IsDead())
                        {
                            //Scene�� ���� ��Ȱ ĳ���� ����
                            GlobalFunc::ReviveCastFromSceneType();
                        }
					}
				}
				break;
			case RC_ROOM_SERVER_SELECT_TRANSACTION_CANCELED:
				{
					// �������� ���
					CastLeaveFieldDialog * pDlg = 
						(CastLeaveFieldDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_DELAY_CASTING);
					if ( pDlg )
					{
						pDlg->ShowWindow(FALSE);
                        g_InterfaceManager.UnLockDlgs();
					}
					g_KeyQueueManager.DeleteMsg(eDoType_ServerSelectStandingBy);

                    if (g_pHero && g_pHero->IsDead())
                    {
                        //Scene�� ���� ��Ȱ ĳ���� ����
                        GlobalFunc::ReviveCastFromSceneType();
                    }
				}
				break;
			case RC_ROOM_SERVER_SELECT_TRANSACTION_CANT_CANCEL:
				// ��ҺҰ� ���� RC_ROOM_SUCCESS �� �ٽ� �´�.
				break;
			}			
		}
		break;
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__
    case CG_CONNECTION_TIME_SYNC_CMD:
        {
            MSG_CG_CONNECTION_TIME_SYNC_CMD* time_tync_cmd = 
                static_cast<MSG_CG_CONNECTION_TIME_SYNC_CMD*>(pMsg);
            util::TimeSync::Command_ClientRecv(time_tync_cmd->sync_node);
        }
        break;

    case CG_CONNECTION_CLIENT_TICK_TEST_ACK:
        {
            MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK* ack = static_cast<MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK*>(pMsg);
            if (KBMoveCheckManagerInClient::Instance()->ReceiveReplyFromServer(*ack) == false)
            {
                //WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, "KBMoveCheckManagerInClient::Instance()->IssueNextSerial(&next_serial_key) == false");

                //if (KBMoveCheckManagerInClient::Instance()->bad_count() > KBMoveCheckManagerInClient::kBadCountMax)
                //{
                //    WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, "speed hack");
                //}
            }
        }
        break;
    }
}

void SceneBase::OnRecvCG_CONNECTION__CG_PREPARE_WORLD_CONNECT_ACK( MSG_BASE * pMsg )
{
	CursorChangeTransact::Instance()->ForceChange( MouseCursor::eMouseCursor_Default );
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    // NOTE: changes world services enabled state
    if (g_pNetwork)
    {
        bool enabled_state = static_cast<MSG_CG_PREPARE_WORLD_CONNECT_ACK*>(pMsg)->enabled;
        g_pNetwork->ChangeWorldMessageSendableState(enabled_state);
    };
#else //if !defined(_NA002676_WORLD_SERVER_RENEWAL)
    if (g_pNetwork && (g_pNetwork->IsConnected(CI_CHATSERVERIDX) == FALSE))
    {
		MSG_CG_PREPARE_WORLD_CONNECT_ACK *pPacket = (MSG_CG_PREPARE_WORLD_CONNECT_ACK *)pMsg;

		// ���弭�� ���� ����
        if( g_pNetwork->Connect( CI_CHATSERVERIDX, pPacket->szWorldServerIP, pPacket->wWorldServerPort ) )
        {
			OnConnectWorldServer();
        }
		// ���弭�� ���� ����
        else
        {
            g_pNetwork->SetEnableSendChatHeartBeat(FALSE);
        }
    }

    g_pApplication->OffReconnectToWorldServer();

#ifdef _DEBUG
    if (g_pNetwork)
    {
		TCHAR	szMessage[MAX_MESSAGE_LENGTH];
		WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
		Sprintf( szMessage, _T("���弭���� �����Ͽ����ϴ�. HeartBeat Status (%d)"), g_pNetwork->ISEnableSendChatHeartBeat());
		g_InterfaceManager.ShowMessageBox(szMessage, 10000, &Color, 0);
    }
#endif
#endif //if !defined(_NA002676_WORLD_SERVER_RENEWAL)
}


//------------------------------------------------------------------------------
/**
*/
void
SceneBase::OnRecvCG_Chao( MSG_BASE * pMsg )
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};

	switch(pMsg->m_byProtocol)
	{
	case CG_CHAO_TRACK_REVENGE_CHAR_NAK:
		{
			//Todo:���� ����
			MSG_CG_CHAO_TRACK_REVENGE_CHAR_NAK *pRecvPacket = (MSG_CG_CHAO_TRACK_REVENGE_CHAR_NAK *)pMsg;
			switch(pRecvPacket->m_byErrorCode)
			{
			case RC_REVENGE_FAILED:
				{
					// 5682	������ �����߽��ϴ�.
					g_InterfaceManager.GetInterfaceString(5682, szMessage ,INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);		// ��Ʈ�� ��û
				}
				break;

			case RC_REVENGE_NO_POINT:
				{
					//5276	������ ����Ʈ�� �����մϴ�.
					g_InterfaceManager.GetInterfaceString(5276, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;

			case RC_REVENGE_INVALID_CHAR:
				{
					//5277	������ ����� �ƴմϴ�.
					g_InterfaceManager.GetInterfaceString(5277, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;

			case RC_REVENGE_INVALID_SITUATION:
				{
					//5278	������ ������ ��Ȳ�� �ƴմϴ�.
					g_InterfaceManager.GetInterfaceString(5278, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;

			case RC_REVENGE_NO_CHAR:
				{
					//5279	����� �������� �ʽ��ϴ�.
					g_InterfaceManager.GetInterfaceString(5279, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				}
				break;


			case RC_REVENGE_REVENGEITEM_NOT_EXSITS:	
				{
					// 5683	������ �� �������� �������� �ʽ��ϴ�.
					g_InterfaceManager.GetInterfaceString(5683, szMessage ,INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
				}
				break;

			case RC_REVENGE_CHAR_INVALID_STATE:		
				{
					// 5684	������ �� �����ۤ��� ����� �� ���� �����Դϴ�.
					g_InterfaceManager.GetInterfaceString(5684 , szMessage , INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);	// ��Ʈ�� ��û
				}
				break;

			case RC_REVENGE_INVALID_ROOM:			
				{
					// 5684	������ �� �����ۤ��� ����� �� ���� �����Դϴ�.
					g_InterfaceManager.GetInterfaceString(5685, szMessage ,INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);	// ��Ʈ�� ��û
				}
				break;

			case RC_REVENGE_MOVEZONE_FAIL:			
				{
					// 5682	������ �����߽��ϴ�.
					g_InterfaceManager.GetInterfaceString(5682, szMessage ,INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);	// ��Ʈ�� ��û
				}
				break;

			case RC_REVENGE_INVALID_ZONE_LEVEL:
				{
					//6430 ���� �� �� ���� ����
					g_InterfaceManager.GetInterfaceString(6430, szMessage ,INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);	// ��Ʈ�� ��û
				}
				break;

			}
		}
		break;


	case CG_CHAO_NOTIFIY_CHAO_TIME_CMD:
		{
			MSG_CG_CHAO_NOTIFIY_CHAO_TIME_CMD *pRecvPacket = (MSG_CG_CHAO_NOTIFIY_CHAO_TIME_CMD *)pMsg;
			uiChaoSystemMan * chaoMan = static_cast<uiChaoSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CHAO));
			if(chaoMan)
			{
				DWORD dwCurTime=chaoMan->GetTime();

				if(dwCurTime>pRecvPacket->m_dwLeavedTime)
				{
					DWORD dwtime=dwCurTime-pRecvPacket->m_dwLeavedTime;

					if(dwtime>=60)
					{
						TCHAR	szMessage[512] = {0,};
						TCHAR	szText[128] = {0,};
						TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
						// Second,Minute �տ� "0" �߰�

						int iHour=dwtime/3600;
						int iMinute=(dwtime-iHour*3600)/60;
						int iSecond=dwtime-iHour*3600-iMinute*60;

						if(iMinute < 10 && iSecond <10)
						{
							Snprintf(szText, 128-1,_T("%d:0%d:0%d"),iHour,iMinute,iSecond);
						}
						// Minute �տ� "0" �߰�.
						else if(iMinute< 10)
						{
							Snprintf(szText, 128-1,_T("%d:0%d:%d"),iHour,iMinute,iSecond);
						}
						// Second �տ� "0" �߰�
						else if(iSecond < 10)
						{
							Snprintf(szText, 128-1,_T("%d:%d:0%d"),iHour,iMinute,iSecond);
						}
						else
						{
							Snprintf(szText, 128-1,_T("%d:%d:%d"),iHour,iMinute,iSecond);
						}

						if(g_pHero&&g_pHero->GetChaoState()==eCHAO_STATE_CHAO)
						{
							// 5907	%s��ŭ ī�� �ð��� ����Ǿ����ϴ�.
							g_InterfaceManager.GetInterfaceString(5907 , szTemp ,INTERFACE_STRING_LENGTH);
							Snprintf(szMessage, 512-1, szTemp, szText);
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
						}
						else if(g_pHero&&g_pHero->GetChaoState()==eCHAO_STATE_PREV_CHAO)
						{
							// 5908	%s��ŭ ����ī�� �ð��� ����Ǿ����ϴ�.
							g_InterfaceManager.GetInterfaceString(5908 , szTemp , INTERFACE_STRING_LENGTH);

							Snprintf(szMessage, 512-1, szTemp ,szText);
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
						}

					}
				}

				chaoMan->SetTime(pRecvPacket->m_dwLeavedTime);	
			}
		}
		break;

	case CG_CHAO_CHANGE_BORADORI_STATE_BRD:// ������ ���� ����/����
		{
			MSG_CG_CHAO_CHANGE_BORADORI_STATE_BRD *pRecvPacket = (MSG_CG_CHAO_CHANGE_BORADORI_STATE_BRD *)pMsg;
			Player *pPlayer = (Player *)g_ObjectManager.GetObject(pRecvPacket->m_dwObjKey);

			if (pPlayer) 
			{
				pPlayer->SetAttackBeginPlayer(pRecvPacket->m_bBoradori);

				if(g_pHero&&g_pHero->GetObjectKey()==pPlayer->GetObjectKey())
				{
					g_InterfaceManager.UpdateMyHeadTexture();
				}
			}
		}
		break;

	case CG_CHAO_CHANGE_CHAO_STATE_BRD:// ī������(eCHAO_STATE)
		{
			MSG_CG_CHAO_CHANGE_CHAO_STATE_BRD *pRecvPacket = (MSG_CG_CHAO_CHANGE_CHAO_STATE_BRD *)pMsg;
			Player *pPlayer = (Player *)g_ObjectManager.GetObject(pRecvPacket->m_dwObjKey);

			if (pPlayer) 
			{
				pPlayer->SetChaoState(pRecvPacket->m_byChaoState);

				if(g_pHero&&g_pHero->GetObjectKey()==pPlayer->GetObjectKey())
				{
					g_InterfaceManager.UpdateMyHeadTexture();

					if (GENERALPARAM->GetSpecialMode()) 
					{
						if(pRecvPacket->m_byChaoState==eCHAO_STATE_CHAO)
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,_T("ī�� ����"));
						else if(pRecvPacket->m_byChaoState==eCHAO_STATE_PREV_CHAO)
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("���� ī�� ����"));
					}
                    uiGuideSystemMan* guide_manager =
                        static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

                    if (guide_manager)
                    {
                        if (pRecvPacket->m_byChaoState==eCHAO_STATE_CHAO)
                        {
                            guide_manager->CheckHelpMessage(TotalGuideNotice::kChaos);
                        }
                        else if (pRecvPacket->m_byChaoState==eCHAO_STATE_PREV_CHAO)
                        {
                            guide_manager->CheckHelpMessage(TotalGuideNotice::kFreeChaos);
                        }
                        
                    }

					// QuestNPC ���� ����. (�߰��� ���°� �ٲ� ����Ʈ�� ���� �� �ִ� ǥ�ø� �ϱ�����.)
					RefreshNPCStateForQuest();

				}

			}
		}
		break;

	case CG_CHAO_ASK_REVENGE_ADDITION_CMD:// ������ ����Ʈ�� �߰��� ��� ObjKey
		{
			MSG_CG_CHAO_ASK_REVENGE_ADDITION_CMD *pRecvPacket = (MSG_CG_CHAO_ASK_REVENGE_ADDITION_CMD *)pMsg;


			TCHAR szString[INTERFACE_STRING_LENGTH] = {0,};
			TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

			// %s�Կ��� �׾����ϴ�. %s���� ��������Ͽ� ��� ��Ű�ڽ��ϱ�? ������ ��Ͽ� ��ϵǸ� 1ȸ ������� PK�� ���������ϴ�. : 5274
			g_InterfaceManager.GetInterfaceString(eST_PK_IS_REVENGE_REGISTER , szMessage ,INTERFACE_STRING_LENGTH);
			Sprintf (szString, szMessage , pRecvPacket->m_ptszRevengeName,pRecvPacket->m_ptszRevengeName);

			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_Confirm(szString,_CallBack_BadFriend_Add);		

			SetReadyDeadFlag(TRUE);


			//ĳ���͸� ���� 
			StrnCopy(m_ptszRevengeName, pRecvPacket->m_ptszRevengeName, MAX_CHARNAME_LENGTH);
            m_ptszRevengeName[_countof(m_ptszRevengeName) - 1] = '\0';
		}
		break;


	case CG_CHAO_ADD_REVENGE_CHAR_ACK:// ������ ����Ʈ�� �߰��� ��� �̸�(�������� World���� �Ѱ��ش�)
		{
			MSG_CG_CHAO_ADD_REVENGE_CHAR_ACK *pRecvPacket = (MSG_CG_CHAO_ADD_REVENGE_CHAR_ACK *)pMsg;

			__asm nop;	//COMMENT: �ƹ��͵� ���ϳ� --;
		}
		break;

	case CG_CHAO_ADD_REVENGE_CHAR_NAK:
		{
			MSG_CG_CHAO_ADD_REVENGE_CHAR_NAK *pRecvPacket = (MSG_CG_CHAO_ADD_REVENGE_CHAR_NAK *)pMsg;

			uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));

			if(pCommMan)
			{
				pCommMan->ShowRevengeResultCode(pRecvPacket->m_byErrorCode);
			}
		}
		break;

	}
}



void SceneBase::OnRecvCG_ArmorKit(MSG_BASE * pMsg)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};

	switch(pMsg->m_byProtocol)
	{
	case CG_ARMORKIT_SEED_TEST_CMD:
		{
			MSG_CG_ARMORKIT_SEED_TEST_CMD * pRecvMsg = (MSG_CG_ARMORKIT_SEED_TEST_CMD *)pMsg;

			_asm nop; //COMMENT: �ƹ��͵� ���Ѵ�.
		}
		break;

#ifdef __NA000000_070913_HACKSHIELD_INSTALLATION__
	case CG_HACKSHIELD_SERVER_EX_QUERY_CMD:
		{
			MSG_CG_HACKSHIELD_SERVER_EX_QUERY_CMD * RecvMsg = (MSG_CG_HACKSHIELD_SERVER_EX_QUERY_CMD *)pMsg;
			MSG_CG_HACKSHIELD_SERVER_EX_QUERY_ACK sendPacket;

    #ifdef _HACKSHIELD_CHECK_TIMER_API_FUNCTION_
        #if defined(_KOREA)
            TCHAR* hs_message = _T("���ǵ������� �ǽɵǴ� ������ �����Ǿ����ϴ�.");
        #else
            TCHAR* hs_message = _T("An operation whitch seems to be Speed-hack has been detected.");
        #endif //defined(_KOREA)

            if ((DWORD)g_pSunRenderer->x_Clock.GetFuncPerformanceCount() != (DWORD)_AhnHS_QueryPerformanceCounter)
            {
                HackShield::HSMessageBox(hs_message);
                return;
            }
            else if ((DWORD)g_pSunRenderer->x_Clock.GetFuncPerformanceFrequency() != (DWORD)_AhnHS_QueryPerformanceFrequency)
            {
                HackShield::HSMessageBox(hs_message);
                return;
            }
            else if ((DWORD)g_pSunRenderer->x_Clock.GetFuncGetTickCount() != (DWORD)_AhnHS_GetTickCount)
            {
                HackShield::HSMessageBox(hs_message);
                return;
            }
    #endif //_HACKSHIELD_CHECK_TIMER_API_FUNCTION_
            {
                int nRet =  _AhnHS_MakeResponse( RecvMsg->m_HShieldReqBlock.byBuffer, RecvMsg->m_HShieldReqBlock.nLength,
                    &sendPacket.m_HShieldAnsBlock );

                if( nRet != HS_ERR_OK )
                {
                    //COMMENT: ����
                }

                GlobalFunc::SendPacketToGameServer( &sendPacket, sizeof( sendPacket ) );
            }
		}
		break;
#endif

#if USING_XIGNCODE
    case CG_XIGNCODE_SERVER_QUERY_CMD:
        if (XignCodeApi::RecvPacket(pMsg) == true)
        {
            // ����
        }
        break;
#endif //USING_XIGNCODE
#if USING_GAMEGUARD
    case CG_GAMEGUARD_SERVER_QUERY_CMD:
        {
            if (GameGuardApi::Instance()->Recv_CG_GAMEGUARD_SERVER_QUERY_CMD_Packet(pMsg) == false)
            {
                // ����
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
                TCHAR log_message[INTERFACE_STRING_LENGTH] = {0, };
                Snprintf(log_message, INTERFACE_STRING_LENGTH, _T("GameGuardApi::RecvPacket() Error\n"));
                GlobalFunc::Write_GameGuard_Log(log_message);
#endif // _NA_000000_20140519_GAMEGUARD_LOG
            }

        }
        break;
    case CG_GAMEGUARD_SERVER_QUERY_NAK:
        {
            if (GameGuardApi::Instance()->Recv_CG_GAMEGUARD_SERVER_QUERY_NAK_Packet(pMsg) == false)
            {
                // ����
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
                TCHAR log_message[INTERFACE_STRING_LENGTH] = {0, };
                Snprintf(log_message, INTERFACE_STRING_LENGTH, _T("GameGuardApi::Recv_CG_GAMEGUARD_SERVER_QUERY_NAK_Packet() Error\n"));
                GlobalFunc::Write_GameGuard_Log(log_message);
#endif // _NA_000000_20140519_GAMEGUARD_LOG
            }
        }
#endif // USING_GAMEGUARD
	}
}



void SceneBase::OnRecvCG_CONNECTION__CG_PREPARE_WORLD_CONNECT_NAK( MSG_BASE * pMsg )
{
	
}



void SceneBase::OnRecvCW_CHAT( MSG_BASE * pMsg )
{
	g_InterfaceManager.NetworkProc( IM_CHAT_MANAGER::CHAT_DIALOG, pMsg );
}

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
void SceneBase::OnRecvCW_NOTIFICATION( MSG_BASE* message )
{
	uiNotificationsSystemMan* notifications_manager = static_cast<uiNotificationsSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_NOTIFICATIONS_SYSTEM));

	if ( notifications_manager )
	{
		notifications_manager->NetworkProc( message );
	}
}
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    
void SceneBase::ParsePacket(MSG_BASE * pMsg)
{
	switch(pMsg->m_byCategory)
	{
	case CG_CONNECTION:
		{
			OnRecvCG_CONNECTION(pMsg);
			break;
		}
		break;

	case CG_CRYPTOGRAPHY:
		{
			OnRecvCG_CRYPTOGRAPHY(pMsg);
			break;
		}
		break;


	case CG_ETC:
 		{
			OnRecvCG_ETC(pMsg);
		}
		break;
		

	case CG_CHAO:
		{
			OnRecvCG_Chao( pMsg );
		}
		break;



	case CG_ARMORKIT:
		{			
			OnRecvCG_ArmorKit( pMsg );
		}
		break;

	case CG_SKILL:
		{
			OnRecvCG_SKILL(pMsg);
		}
		break;

	case CG_PARTY:
		{
			OnRecvCG_Party(pMsg);
		}
		break;

	case CG_CHARINFO:
		{
			SceneBase::OnRecvCG_CHARINFO(pMsg);
		}
		break;
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    //case CW_CHAT        :// ĳ���� ����, ĳ���� ����
    case CW_GM          :// GM ��ɾ�
    case CW_VIEWPORT    :// ���� ����Ʈ ����
    case CW_ARMORKIT    :// ArmorKit ����
    case CW_FRIEND      :// ģ�� ����
    case CW_GUILD       :// ��� ����
    //case CW_CRYPTOGRAPHY:// ���� ����
    case CW_COMMUNITY   :// ��Ÿ Ŀ�´�Ƽ ����
    case CW_REVENGE     :// ������(����ģ��) ���� ����
    case CW_MEMO        :// ���� ����
    //case CW_SYNC      :
    case CW_GROUP       :// �׷� �α� �ý��� ���� ��������
    case CW_LOTTO       :// ���� �ζ� ����
    case CW_PARTY       :// ��Ƽ ���� ��������
        UserInterfaceManagersParsePacketWorld(pMsg);
        break;
    case CW_CONNECTION  :
        // TODO: enabled(world_service)
        break;
    case CW_CHAT        :// ĳ���� ����, ĳ���� ����
        UserInterfaceManagersParsePacketWorld(pMsg);
        OnRecvCW_CHAT(pMsg);
        break;
#endif

	default:
		//assert(!"ó������ ���� ��Ŷ�� ���Խ��ϴ�.");
		break;
	}
}

void SceneBase::ParsePacketInChat( MSG_BASE * pMsg )
{
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    __UNUSED(pMsg);
    assert(!"this "__FUNCTION__" is not called anymore");
    return;
#else
	switch (pMsg->m_byCategory)
	{
	case CW_CONNECTION:
        switch( pMsg->m_byProtocol )
        {
        case CW_ENTER_READY_BRD:
            {
                MSG_CW_ENTER_SERVER_SYN SendPacket;
                SendPacket.m_byCategory = CW_CONNECTION;
                SendPacket.m_byProtocol = CW_ENTER_SERVER_SYN;
                SendPacket.dwAuthUserID = GENERALPARAM->GetUserID();
                GlobalFunc::SendPacket(CI_CHATSERVERIDX, &SendPacket, sizeof(SendPacket) );
                
                if (g_pNetwork)
                {
                    g_pNetwork->SetEnableSendChatHeartBeat(TRUE);
                }
            }
            break;
        }
		break;

	case CW_CHAT:
		OnRecvCW_CHAT( pMsg );
		break;

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
	case CW_NOTIFICATION:
		{
			OnRecvCW_NOTIFICATION( pMsg );
		}
		break;
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

	case CW_CRYPTOGRAPHY :
		OnRecvCW_CRYPTOGRAPHY(pMsg);
		return;

	}
#endif //
}


void SceneBase::OnRecvCW_CRYPTOGRAPHY( MSG_BASE * pMsg )
{
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
    switch ( pMsg->m_byProtocol)
    {
		case CW_CRYPTOGRAPHY_KEY:
			{
				// ���� MSG_CW_CRYPTOGRAPHY_ACK �޼����� ������ ������ �����ؾ� �Ѵ�.
				MSG_CW_CRYPTOGRAPHY_ACK SendPacket;
				GlobalFunc::SendPacket( CI_CHATSERVERIDX, &SendPacket, sizeof(SendPacket) );
				// ��ȣȭ ���� ����
                if (g_pNetwork)
                {
				    g_pNetwork->SetCryptographyInfo(CI_CHATSERVERIDX, CProtocolCryptographyMgr::SEND_CRYPTO, pMsg);
                }
			}
			break;

		case CW_CRYPTOGRAPHY_CMD:
            {
                if (g_pNetwork)
                {
			        g_pNetwork->CopyRecvCryptographyInfoFromSend(CI_CHATSERVERIDX);
                }
            }
			break;
	}
#endif //
}


void SceneBase::OnRecvCG_CRYPTOGRAPHY( MSG_BASE * pMsg )
{
    switch (pMsg->m_byProtocol)
    {
		case CG_CRYPTOGRAPHY_KEY:
			{
				// ���� MSG_CG_CRYPTOGRAPHY_ACK �޼����� ������ ������ �����ؾ� �Ѵ�.
				MSG_CG_CRYPTOGRAPHY_ACK SendPacket;
				GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));

                if (g_pNetwork)
                {
				    g_pNetwork->SetCryptographyInfo(CI_GAMESERVERIDX, CProtocolCryptographyMgr::SEND_CRYPTO, pMsg);
                }
			}
			break;

		case CG_CRYPTOGRAPHY_CMD:
            {
                if (g_pNetwork)
                {
			        g_pNetwork->CopyRecvCryptographyInfoFromSend(CI_GAMESERVERIDX);
                }
            }
			break;

		case CG_CRYPTOGRAPHY_SNK_KEY:
			{
				MSG_CG_CRYPTOGRAPHY_ACK SendPacket;
				GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
                if (g_pNetwork)
                {
				    g_pNetwork->SetCryptographyInfo_SNK(CI_GAMESERVERIDX, CProtocolCryptographyMgr::SEND_CRYPTO, pMsg);
                }
			}
			break;

		case CG_CRYPTOGRAPHY_SNK_CMD:
            {
                if (g_pNetwork)
                {
				    g_pNetwork->CopyRecvCryptographyInfoFromSend_SNK(CI_GAMESERVERIDX);
                }
            }
			break;
	}
}


BOOL SceneBase::PostInterfaceRender(DWORD dwTick)
{
	g_InterfaceManager.PostRenderInterfaceManager( dwTick );
	return TRUE;
}

BOOL SceneBase::PreInterfaceRender(DWORD dwTick)
{	
	g_InterfaceManager.PreRenderInterfaceManager( dwTick );
	return TRUE;
}

void SceneBase::SetViewport()
{
	g_pSunRenderer->SetViewport(NULL);
}

//----------------------------------------------------------------------------
/**
*/
void	
SceneBase::InitLoadingImg()
{
	for( int i = 0 ; i < IMG_MAX_COUNT ; ++i )
	{
		m_hTexture[i] = INVALID_HANDLE_VALUE;
	}

	m_iForceIndex = -1;

	m_hLoadingGageTexture = INVALID_HANDLE_VALUE;

	m_hLoadingGageBackTexture = INVALID_HANDLE_VALUE; 

	g_Camera.InitSpringCamera();

#ifdef _JBH_ADD_AGE_LIMIT_MARK
	m_hGameRatingMarkTexture = INVALID_HANDLE_VALUE;
#endif
}

//----------------------------------------------------------------------------
/**
*/
void	
SceneBase::LoadingImg( BOOL bIsChangeImage ,int iForceIndex)
{
    char	szLoadingFileName[MAX_PATH] = {NULL };
	if (m_hTexture[IMG_LOADING_BACKGROUND] != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hTexture[IMG_LOADING_BACKGROUND] );
		m_hTexture[IMG_LOADING_BACKGROUND] = INVALID_HANDLE_VALUE;
	}
	
    BASE_LoadingTextureInfo* info = LoadingTextureInfoParser::Instance()->GetLoadingTextureInfo(iForceIndex);
    if (info != NULL)
    {
        //g_iLoadingImage = info->index;
        strcpy(szLoadingFileName, info->file_name.c_str());
    }

    if (strlen(szLoadingFileName) > 0)
    {
        m_hTexture[IMG_LOADING_BACKGROUND] = g_pSunRenderer->x_pManagerTexture->LoadTexture( szLoadingFileName ,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);	
    }

	this->loadLoadingMessage();

	if(m_hLoadingGageTexture==INVALID_HANDLE_VALUE)
	{
		_snprintf( szLoadingFileName, MAX_PATH, "Data\\Interface\\Loading_bar.tga");

		m_hLoadingGageTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( szLoadingFileName ,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);	
	}


	if(m_hLoadingGageBackTexture == INVALID_HANDLE_VALUE)
	{
		_snprintf( szLoadingFileName, MAX_PATH, "Data\\Interface\\Loading_back.tga");
		m_hLoadingGageBackTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( szLoadingFileName ,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);	
	}

}

//----------------------------------------------------------------------------
/**
*/
void	
SceneBase::LoadingCI( BOOL bIsChangeImage )
{
	if (m_hTexture[IMG_LOADING_BACKGROUND] != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hTexture[IMG_LOADING_BACKGROUND] );
		m_hTexture[IMG_LOADING_BACKGROUND] = INVALID_HANDLE_VALUE;
	}

	char	szLoadingFileName[MAX_PATH];


	if (g_pApplication->IsWideModeFlag())
		_snprintf( szLoadingFileName, MAX_PATH, "Data\\Interface\\Loading_WebzenCI_01.JPG");
	else
		_snprintf( szLoadingFileName, MAX_PATH, "Data\\Interface\\Loading_WebzenCI.JPG");

	m_hTexture[IMG_LOADING_BACKGROUND] = 
		g_pSunRenderer->x_pManagerTexture->LoadTexture( szLoadingFileName ,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);	

	if( bIsChangeImage )
	{
		this->loadInitMessage();
		int iPreLoadingImage=g_iLoadingImage;	
	}


}


//----------------------------------------------------------------------------
/**
*/
void	
SceneBase::ReleaseLoadingImg()
{
	for( int i = 0 ; i < IMG_MAX_COUNT ; ++i )
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hTexture[i] );
		m_hTexture[i] = INVALID_HANDLE_VALUE;
	}

	if(m_hLoadingGageTexture!=INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hLoadingGageTexture );
		m_hLoadingGageTexture=INVALID_HANDLE_VALUE;
	}

	if(m_hLoadingGageBackTexture != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hLoadingGageBackTexture );
		m_hLoadingGageBackTexture = INVALID_HANDLE_VALUE;
	}

}

//------------------------------------------------------------------------------ 
void SceneBase::RenderLoadingImg()
{	
	g_pApplication->Set2DMode(TRUE);
	
	POINT pt = g_pApplication->Get2DModeMapping();

	if (m_hTexture[IMG_LOADING_BACKGROUND] == INVALID_HANDLE_VALUE)
		SceneBase::LoadingImg( m_bChangeImage );

	if (m_hTexture[IMG_LOADING_BACKGROUND] != INVALID_HANDLE_VALUE)
	{
        St_TextureContainer* texture_info = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(m_hTexture[IMG_LOADING_BACKGROUND]);
        if (texture_info != NULL)
        {
            float image_height = kLoadingImageWidth * static_cast<float>(pt.y) / pt.x;
            if (image_height > texture_info->m_nImageHeight)
            {
                image_height = texture_info->m_nImageHeight;
            }

            float top_gab = (texture_info->m_nImageHeight - image_height) / 2.0f;
            g_pSunRenderer->RenderTexture(m_hTexture[IMG_LOADING_BACKGROUND], 
                                        0.0f, 0.0f, (float)pt.x, (float)pt.y,
                                        0.0f, top_gab, texture_info->m_nImageWidth, image_height);
        }

		m_FTextRender.Render();
		m_TipFTextRender.Render();
	}

	g_pApplication->Set2DMode(FALSE);
}

void	
SceneBase::RenderLoadingGage()
{	
	g_pApplication->Set2DMode(TRUE);

	if( m_hLoadingGageTexture != INVALID_HANDLE_VALUE && m_hLoadingGageBackTexture != INVALID_HANDLE_VALUE )
	{
		St_TextureContainer *ptc = g_pSunRenderer->x_pManagerTexture->GetTextureInfo( m_hLoadingGageTexture );
		St_TextureContainer *backPtc = g_pSunRenderer->x_pManagerTexture->GetTextureInfo( m_hLoadingGageBackTexture );

		float fMaxLength = ptc ? (float)ptc->m_nImageWidth : 0;

		float fRate = (float)(m_fLoadingCount / m_fMaxCount);
		float iLoadingGageDestXPos, iLoadingGageDestYPos;
		POINT pt = g_pApplication->Get2DModeMapping();

		iLoadingGageDestXPos = (float)pt.x - (backPtc ? (float)backPtc->m_nImageWidth : 0);
		iLoadingGageDestYPos = (float)pt.y - (backPtc ? (float)backPtc->m_nImageHeight : 0);

        iLoadingGageDestYPos -= 11; //25;

		ENUM_ALPHABLEND eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
		g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);
		g_pSunRenderer->RenderTexture( m_hLoadingGageBackTexture, iLoadingGageDestXPos, iLoadingGageDestYPos, 
			(float)backPtc->m_nImageWidth, (float)backPtc->m_nImageHeight );	
		g_pSunRenderer->SetAlphaBlendMode(eAlphablendMode);
			
		float fWidth = fMaxLength*fRate;

		if(fWidth>=536)
			fWidth =536;

		g_pSunRenderer->RenderTexture( m_hLoadingGageTexture, iLoadingGageDestXPos+11, iLoadingGageDestYPos+40, fWidth,
			(float)ptc->m_nImageHeight, 0.0f, 0.0f, fMaxLength, (float)ptc->m_nImageHeight);

	}

	g_pApplication->Set2DMode(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void
SceneBase::UserInterfaceManagersParsePacket( MSG_BASE * pMsg )
{
    g_InterfaceManager.UpdateUserInterfaceManagerNetworkMessage(pMsg);
}


//------------------------------------------------------------------------------
/**
*/
void
SceneBase::UserInterfaceManagersParsePacketWorld( MSG_BASE * pMsg )
{
    g_InterfaceManager.UpdateUserInterfaceManagetNetworkMessageWorld(pMsg);
}

BOOL  
SceneBase::SceneWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam ,SI_MESSAGE *stMessage)
{
	return FALSE;
}

void
SceneBase::OnDeviceReset()
{
	return;
}

void
SceneBase::KeyManagerProc(DWORD wParam,DWORD lParam)
{

}


void
SceneBase::loadInitMessage()
{
	g_pApplication->Set2DMode(TRUE);

	POINT pt = g_pApplication->Get2DModeMapping();
	RECT rt;
	rt.left = 10;
	rt.right = pt.x;
	rt.bottom = pt.y;
	const int cFontSize = 12;
	int iSpaceTextTop =pt.y -  (pt.y /12);
	rt.top = iSpaceTextTop;

	m_FTextParser.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);
	m_FTextRender.Init(g_pSunRenderer, rt);

	TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
	g_InterfaceManager.GetInterfaceString(1186,message,INTERFACE_STRING_LENGTH);
	TCHAR tempBuff[INTERFACE_STRING_LENGTH];
		
	float fPercent=100.0f*(m_fLoadingCount/m_fMaxCount);

	fPercent=min(fPercent,100.0f);

	Sprintf( tempBuff,_T("%s... %d%%"),message,(DWORD)fPercent);
	GlobalFunc::WriteLoadingTimeLog(tempBuff);

	SUN_STRING strTemp = tempBuff;

	BOOL ret = m_FTextParser.Parser(strTemp,m_FTextSentence);	

	m_FTextRender.ClearSentence();
	m_FTextRender.Append(m_FTextSentence);
	m_FTextRender.SetRect(rt);
	m_FTextRender.Render();

	g_pApplication->Set2DMode(FALSE);


}


void
SceneBase::loadLoadingMessage()
{
	
	POINT pt = g_pApplication->Get2DModeMapping();
	RECT rt;
	rt.left = 10;
	rt.right = pt.x;
	rt.bottom = pt.y;
	const int cFontSize = 12;
	int iSpaceTextTop =pt.y -  (pt.y /12);
	rt.top = iSpaceTextTop;
    rt.top += 14;
	
	m_FTextParser.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);
	m_FTextRender.Init(g_pSunRenderer, rt);
	
	TCHAR message[INTERFACE_STRING_LENGTH];
	g_InterfaceManager.GetInterfaceString(eST_YOUNGMAN_PROTECT,message,INTERFACE_STRING_LENGTH);
	SUN_STRING strTemp = message;
	BOOL ret = m_FTextParser.Parser(strTemp,m_FTextSentence);	
	
	m_FTextRender.ClearSentence();
	m_FTextRender.Append(m_FTextSentence);
	m_FTextRender.SetRect(rt);

	rt.left		= 10;
	rt.right	= pt.x;
	rt.bottom	= pt.y / 10;
	rt.top		= 10;
	m_TipFTextRender.Init(g_pSunRenderer, rt);

	TCHAR* pszHelpMessage;
	pszHelpMessage = HelpMessageParser::Instance()->GetHelpMessage();
	if ( pszHelpMessage )
	{
		strTemp = pszHelpMessage;
		ret = m_TipFTextPraser.Parser(strTemp,m_TipFTextSentence);
		if(ret)
		{
			m_TipFTextRender.ClearSentence();
			m_TipFTextRender.Append(m_TipFTextSentence);
			m_TipFTextRender.SetRect(rt);
		}
	}


	

	m_FTextRender.Render();
	m_TipFTextRender.Render();


	
}

//------------------------------------------------------------------------------
/**
*/
void
SceneBase::ShowBillingAlarm(DWORD dwRestTerm)
{
	// ���� ���� �ð��� %s ���ҽ��ϴ�.
	// 59	�ð�
	// 11302	��

	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTime[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szHour[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMin[INTERFACE_STRING_LENGTH] = {0,};
	
	g_InterfaceManager.GetInterfaceString(59, szHour, INTERFACE_STRING_LENGTH);
	g_InterfaceManager.GetInterfaceString(11302, szMin, INTERFACE_STRING_LENGTH);
	
	DWORD hour = 0;
	DWORD min = 0;
	
	if(dwRestTerm)
	{
		hour = (dwRestTerm / 60);
		min = (dwRestTerm %  60);
		
		if(hour)
		{
			if(min)
				Snprintf(szTime, INTERFACE_STRING_LENGTH-1, _T("%d%s %d%s"), hour, szHour, min, szMin);
			else
				Snprintf(szTime, INTERFACE_STRING_LENGTH-1, _T("%d%s"), hour, szHour);
		}
		else
		{
			Snprintf(szTime, INTERFACE_STRING_LENGTH-1, _T("%d%s"), min, szMin);
		}
		//PC�� ���� �ܿ��ð��� %s ���ҽ��ϴ�.
		g_InterfaceManager.GetInterfaceString(85227, szTemp, INTERFACE_STRING_LENGTH);
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szTime);

		GlobalFunc::OutputMsg_SysMsg(szMessage);
	}
}


//------------------------------------------------------------------------------
/**
*/
void SceneBase::ShowDisconnectAlarm(DWORD dwErrCode)
{
	TCHAR szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	
	switch(dwErrCode)
	{
	case RC_DISCONNECT_INCORRECT_VERSION:				//< ������ Ʋ���ϴ�.
		{
			//	85191	������ ������ �������ϴ� [%d] \n ������ �ٸ��ϴ�. Ȯ�����ּ���.
			g_InterfaceManager.GetInterfaceString( 85191, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_DISCONNECT_FROM_GM:							//< GM������ ���� ���� �Ǿ����ϴ�.
		{
			// 85192	������ ������ �������ϴ� [%d] \n GM�� ���� ���� ���� �Ǿ����ϴ�.
			g_InterfaceManager.GetInterfaceString( 85192, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_DISCONNECT_INCORRECT_AUTH_KEY:				//< �߸��� ����Ű�Դϴ�.
		{
			// 85193	������ ������ �������ϴ� [%d] \n �߸��� ����Ű�Դϴ�.
			g_InterfaceManager.GetInterfaceString( 85193, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_DISCONNECT_DUPLICATE_LOGIN:					//< �ߺ� �α���
	case RC_DISCONNECT_OVERLAPPED_CONN_FROM_AUTH:		//< �ߺ� ���ӿ� ���� Auth�κ��� ���� ���� ����
		{
			// 85194	������ ������ �������ϴ� [%d] \n �ߺ��α��� �Ǿ����ϴ�.
			g_InterfaceManager.GetInterfaceString( 85194, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_DISCONNECT_REQ_AUTH:							//< �������������� ��û�����ؼ� �����ϴ�.
		{
			// 85196	������ ������ �������ϴ� [%d] \n ���������� ��û�� ���� ����Ǿ����ϴ�.
			g_InterfaceManager.GetInterfaceString( 85196, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;
	case RC_DISCONNECT_BILLING_EXPIRED:					//< ���� ����
		{
			//85195	������ ������ �������ϴ� [%d] \n PC�� ���� �ܿ��ð��� �������� �ʾ� ���� �̿��� �Ͻ� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString( 85195, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;
	default:
		{
			//	70002 ������ ������ �������ϴ� [%d]
			g_InterfaceManager.GetInterfaceString( eST_SERVER_DISCONNECT_ERRCODE, szMessage, INTERFACE_STRING_LENGTH );
		}
		break;
	}

	Snprintf(szString, INTERFACE_STRING_LENGTH-1, szMessage, dwErrCode);

	GlobalFunc::OutputMsg_SysAlertMsg(szString);
}

#ifdef _JBH_ADD_AGE_LIMIT_MARK
void SceneBase::RenderGameRatingMark()
{
	if( m_hGameRatingMarkTexture == INVALID_HANDLE_VALUE)
		return;

	ENUM_ALPHABLEND oldMode = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->Set2DMode( TRUE);
	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
	g_pSunRenderer->SetColor(255,255,255,255);	

	POINT ScreenSize = g_pApplication->Get2DModeMapping();
	g_pSunRenderer->RenderTexture( m_hGameRatingMarkTexture ,((float) ScreenSize.x  - 252.0f), 30.0f, 232.0f , 89.0f);
	g_pSunRenderer->Set2DMode( FALSE );
	g_pSunRenderer->SetAlphaBlendMode(oldMode);
}

void SceneBase::LoadingGameRatingMarkTexture()
{
	m_hGameRatingMarkTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\15_Year_Text.tga",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD );
}

void SceneBase::ReleaseGameRatingMarkTexture()
{
	if( m_hGameRatingMarkTexture != INVALID_HANDLE_VALUE )
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(m_hGameRatingMarkTexture);
		m_hGameRatingMarkTexture = INVALID_HANDLE_VALUE;
	}
}
#endif

//------------------------------------------------------------------------------
/**
*/
#ifdef __NA_0_20080415_PROCESS_END_PROECSS
int SceneBase::OnEndProcess( DWORD dwTick )
{
    Object_Map_Itr tempitr;
    Object_Map_Itr itr = g_ObjectManager.GetBegin();
    while(itr != g_ObjectManager.GetEnd() )
    {		
        Object * pObject = static_cast<Object *>(itr->second);
        if (0 == pObject)
        {
            return 0;
        }

        itr->second->OnEndProcess(dwTick);
        itr++;
    }

    return 0;
}
#endif