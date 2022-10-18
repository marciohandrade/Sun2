#include "SunClientPrecompiledHeader.h"

#include "uiQuestMan.h"
#include "npcInfoParser.h"
#include "uiQuestCommonForward/uiQuestCommonForward.h"
#include "uiQuestReward/uiQuestReward.h"
#include "QuestManager_Concrete.h"
#include "GameFunc.h"
#include "NPC.h"
#include "ObjectManager.h"
#include "HeadMeshInfoParser.h"
#include "globalFunc.h"
#include "Hero.h"
#include "Map.h"
#include "ResultCode.h"

// 숫자가 높으면 우선순위가 높다.
WORD PriorityQuestStateTable[QUEST_GAIN_MAX] =
{
	0, //QUEST_NONE = 0
	3, //QUEST_ACCEPT,		
	2, //QUEST_PROGRESS,			
	4, //QUEST_REWARD,			
	2, //QUEST_INPERFECT_REWARD,
	0, //QUEST_AUTO_ACCEPT
	0, //QUEST_CANT_ACCEPT_LV
    0, //QUEST_CANT_ACCEPT_DAILYQUEST
};

//-------------------------------------------------------------------------------------------
/**
*/
SolarDialog *			uiQuestMan::OpenForwardWindow(MAPID MapID, FIELDID FieldID, CODETYPE NpcCode, DWORD dwNPCKey)
{
	m_dwPendingOpenNPCKey = dwNPCKey;
	m_OpenNPCCode = NpcCode;
	m_MapID = MapID;
	m_FieldID = FieldID;

	// 현재 수락해서 진행한 퀘스트와
	// 수락해야할 임무가 있는지 Check 해서 정보를 채운다.
    BOOL bGetListRet = getQuestListsMeetNPC();

	// 만약 수락해야할 퀘스트가 없고 수락 받을 퀘스트도 없다면
	// 바로 본업창을 띄워주고 본업도 없는놈이라면 창을 띄우지말자.
	if (!bGetListRet)
	{
		SolarDialog* pDlg = GameFunc::OpenNPCMeetDialog(MapID, FieldID, NpcCode);

		return pDlg;
	}
	else
	{
		QUEST_LIST_ITER iter = m_CanAcceptQuestList.begin();
		QUEST_LIST_ITER iterEnd = m_CanAcceptQuestList.end();
		while (iter != iterEnd)
		{
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
            QCODE quest_code = (*iter);

            Quest* pQ = g_pQMGR_Temp->FindQuest(quest_code);
            if (NULL == pQ)
            {
                pQ = g_pQMGR_Real->FindQuest(quest_code);
            }
#else
			Quest* pQ = (*iter);
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
			if (FALSE == pQ->GetQuestInfo()->GetQDisplay())
			{
				NET_SEND_CG_QUEST_ACCEPT_SYN(pQ->GetQuestCode());
				return NULL;
			}

			++iter;
		}

		if (!this->m_pUIQuestCommonForward)
		{
			return NULL;
		}

		// 기존 NPC 가 퀘스트 관련해서 열어주는 창을 닫고.
		CloseQuestNPC();

		m_pUIQuestCommonForward->ShowInterfaceWithSound(TRUE);

		// key queueing
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_dialog_Quest_NPC;
		msg.wParam = QUEST_WINDOW_NPC;
		msg.DoSomething = GlobalFunc::DoShowQuestWindow;
		g_KeyQueueManager.PushBack(msg);

		return m_pUIQuestCommonForward;
	}
}


//-------------------------------------------------------------------------------------------
/**
*/
eQUEST_GAIN_STATE 
uiQuestMan::GetQuestGainState( QCODE QuestCode, CODETYPE NPCCode, NPC * pNPC )
{
	if ( !g_pHero || !g_pQMGR_Temp || !g_pQMGR_Real) 
		return QUEST_NONE;

	// 실제
	Quest * pRealQuest = g_pQMGR_Real->FindQuest(QuestCode);
	if ( pRealQuest )
	{
		if ( pRealQuest->GetQuestInfo()->IsChildQuest() )
		{ 
			// 자식 퀘스트는 목록에서 제외.
			return QUEST_NONE;
		}

		// 보상 가능 리스트 출력
		// 보상받을 수 있는 진행중[완료표시]인 리스트
		BOOL bRetCompl = BOOL(pRealQuest->GetQState() == QS_COMPLETED);// g_pQMGR_Real->CanComplete(QuestCode, g_pHero);
		if( bRetCompl )
		{
			if ( NPCCode == pRealQuest->GetQuestInfo()->GetNPCCode_Reward() )
			{
				// 1. 현재 NPC 에게서 보상 받을 수 있는 퀘스트 목록				[ 보상 ]
#ifdef _NA_003027_20111013_HONOR_SYSTEM
                if (pNPC != NULL)
                {
                    if (pRealQuest->GetQuestInfo()->IsDailyQuest() == TRUE)
                    {
                        pNPC->SetHaveDailyQuest();
                        if (pRealQuest->GetQuestInfo()->GetDailyQuest() == DAILYQUEST_GUILD)
                        {
                            pNPC->SetHaveGuildQuest();
                        }
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
                        if (pRealQuest->GetQuestInfo()->GetDailyQuest() == DAILYQUEST_CHALLENGE)
                        {
                            pNPC->SetHaveChallengeDailyQuest();
                        }
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST
                    }

                    QUEST_COND_HASH& AcceptConHash = pRealQuest->GetAcceptContionHash();
                    QUEST_COND_HASH_IT itr = AcceptConHash.begin();
                    QUEST_COND_HASH_IT itrEnd = AcceptConHash.end(); 

                    while(itr != itrEnd)
                    {
                        QuestCondition * pAcceptCondition = (*itr);
                        if(eQUEST_CONDITION_REPEAT == pAcceptCondition->GetType())
                        {
                            QUEST_CONDITION_REPEAT* pQC = (QUEST_CONDITION_REPEAT*)pAcceptCondition;
                            pNPC->SetMaxQuestRepeat( pQC->GetInfo()->GetMaxRepeatNum() );
                            break;
                        }
                        itr++;
                    }
                }
#endif //_NA_003027_20111013_HONOR_SYSTEM

				return QUEST_REWARD;
			}
		}
		else
		{
			if ( NPCCode == pRealQuest->GetQuestInfo()->GetNPCCode_Reward() )
			{
				// 2. 현재 NPC 에게서 보상 받을 수 있으나 완료 하지 못한 목록	[ 보상 ]
				return QUEST_INPERFECT_REWARD;
			}
			else if (NPCCode == pRealQuest->GetQuestInfo()->GetNPCCode_Start())
			{
				//3. 현재 NPC 에게서 수락 받아서 진행중인 퀘스트 목록			[ 임무 ]
				return QUEST_PROGRESS;
			}
		}
	}
	else
	{
		// 수락 가능한 퀘스트		
		Quest * pQuest = g_pQMGR_Temp->FindQuest(QuestCode);
		if ( !pQuest )
		{
			pQuest = g_pQMGR_Temp->CreateQuest(QuestCode);
			if ( !pQuest )
			{
				return QUEST_NONE;
			}
		}

		if (eQUEST_ACCEPT_GENERAL != pQuest->GetQuestInfo()->GetQuestAcceptType() &&
			QS_NORMAL == pQuest->GetQState() )
		{
			return QUEST_NONE;
		}

		if ( pQuest->GetQuestInfo()->IsChildQuest() )
		{ 
			// 자식 퀘스트는 목록에서 제외.
			return QUEST_NONE;
		}

        // 엔피시 코드 체크
        if (NPCCode != pQuest->GetQuestInfo()->GetNPCCode_Start())
        {
            return QUEST_NONE;
        }

        // 엔피시 위치 체크(맵코드, 필드코드)
        EXTRA_NPCINFO* npc_extra_info = NPCInfoParser::Instance()->GetExtraInfo(NPCCode);
        if (npc_extra_info == NULL)
        {
            return QUEST_NONE;
        }
        MAPID current_map_code = g_pMap->GetMapID();
        FIELDID current_field_code = g_pMap->GetCurrentFieldID();
        if ((current_map_code != npc_extra_info->m_MapCode) || 
            (current_field_code != npc_extra_info->m_FieldID))
        {
            return QUEST_NONE;
        }

        RC::eQUEST_RESULT bRet = g_pQMGR_Temp->CanAccept(QuestCode, g_pHero);
        if ( RC::RC_QUEST_SUCCESS == bRet )
        {
            if (FALSE == pQuest->GetQuestInfo()->GetQDisplay())
            {
                return QUEST_ACCEPT_NONE_DISPLAY;
            }

            // 4. 현재 NPC 에서 수락 받을 수 있는 목록						[ 임무 ]
            if( pNPC )
            {
                QUEST_COND_HASH& AcceptConHash = pQuest->GetAcceptContionHash();
                QUEST_COND_HASH_IT itr = AcceptConHash.begin(); 
                QUEST_COND_HASH_IT itrEnd = AcceptConHash.end(); 

                while(itr != itrEnd)
                {
                    QuestCondition * pAcceptCondition = (*itr);
                    if(eQUEST_CONDITION_REPEAT == pAcceptCondition->GetType())
                    {
                        QUEST_CONDITION_REPEAT* pQC = (QUEST_CONDITION_REPEAT*)pAcceptCondition;
                        pNPC->SetMaxQuestRepeat( pQC->GetInfo()->GetMaxRepeatNum() );
                        break;
                    }
                    itr++;
                }
            }

#ifdef _NA_003027_20111013_HONOR_SYSTEM
            if (pQuest->GetQuestInfo()->IsDailyQuest() == TRUE)
            {
                WORD quest_group_code = pQuest->GetQuestInfo()->GetSquadCode();
                if (quest_group_code > 0)
                {
                    QCODE today_quest_code = g_pQMGR_Real->GetTodayQuestCode(quest_group_code);
                    if ((today_quest_code != 0) && 
                        (today_quest_code != QuestCode))
                    {
                        return QUEST_NONE;
                    }
                }

                bool is_guild_quest = (pQuest->GetQuestInfo()->GetDailyQuest() == DAILYQUEST_GUILD);
                if ((is_guild_quest == true) && 
                    (g_pHero->IsExistGuild() == FALSE))
                {
                    return QUEST_NONE;
                }

#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
                bool is_challenge_daily_quest = (pQuest->GetQuestInfo()->GetDailyQuest() == DAILYQUEST_CHALLENGE);
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST

                BOOL is_can_accept_daily_quest = 
                    g_pQMGR_Real->IsCompletedDailyQuest(QuestCode, pQuest->GetQuestInfo());
                if (is_can_accept_daily_quest == TRUE)
                {
                    return QUEST_CANT_ACCEPT_DAILYQUEST;
                }

                is_can_accept_daily_quest = g_pQMGR_Real->IsCompletedDailyQuestGroup(quest_group_code);
                if (is_can_accept_daily_quest == TRUE)
                {
                    return QUEST_CANT_ACCEPT_DAILYQUEST;
                }

                if (pNPC != NULL)
                {
                    pNPC->SetHaveDailyQuest();
                    if (is_guild_quest == true)
                    {
                        pNPC->SetHaveGuildQuest();
                    }
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
                    if (is_challenge_daily_quest == true)
                    {
                        pNPC->SetHaveChallengeDailyQuest();
                    }
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST
                }
                
            }
#endif //_NA_003027_20111013_HONOR_SYSTEM

            return QUEST_ACCEPT;
        }
        else
        {
            QUEST_CONDITION_CHARLV* pQCC = NULL;
            RC::eQUEST_RESULT bCanAccept = RC::RC_QUEST_FAILED;

            QUEST_COND_HASH & AcceptConHash = pQuest->GetAcceptContionHash();
            QUEST_COND_HASH_IT itr = AcceptConHash.begin(); 
            while(itr != AcceptConHash.end())
            {
                QuestCondition * pAcceptCondition = (*itr);
                if(eQUEST_CONDITION_CHARLV == pAcceptCondition->GetType())
                {
                    pQCC = (QUEST_CONDITION_CHARLV*)pAcceptCondition;
                }
                else
                {
                    bCanAccept = pAcceptCondition->CanAccept(g_pHero);
                    if(RC::RC_QUEST_SUCCESS != bCanAccept)
                        break;
                }

                ++itr;
            }

#ifdef _NA_003027_20111013_HONOR_SYSTEM
            if (pQuest->GetQuestInfo()->IsDailyQuest() == TRUE)
            {
                if (pQuest->GetQuestInfo()->GetDailyQuest() == DAILYQUEST_GUILD)
                {
                    if (g_pHero->IsExistGuild() == FALSE)
                    {
                        return QUEST_NONE;
                    }
                }

                WORD quest_group_code = pQuest->GetQuestInfo()->GetSquadCode();
                if (quest_group_code > 0)
                {
                    QCODE today_quest_code = g_pQMGR_Real->GetTodayQuestCode(quest_group_code);
                    if ((today_quest_code != 0) && 
                        (today_quest_code != QuestCode))
                    {
                        return QUEST_NONE;
                    }
                }
            }
#endif //_NA_003027_20111013_HONOR_SYSTEM

            if( (RC::RC_QUEST_SUCCESS == bCanAccept) && pQCC)
            {
                if(g_pHero->GetLevel() < pQCC->GetInfo()->GetMinCharLV())
                {
                    if( MAX_SHOW_QUEST_LEVEL_GAP > (pQCC->GetInfo()->GetMinCharLV() - g_pHero->GetLevel()) )
                        return QUEST_CANT_ACCEPT_LV;
                }
            }
        }
	}

	return QUEST_NONE;
}

//-------------------------------------------------------------------------------------------
/** 이 함수를 호출해야 할때
	1. Player가 처음 베틀존에 들어왔을때
	2. Accept 받았을 때
	3. 완료 했을 때
	4. 보상 받았을 때
	5. 실패 했을 때
	6. 포기 했을 때
*/
void			
uiQuestMan::RefreshNPCState()
{
    if (g_pQMGR_Temp)
    {
	    g_pQMGR_Temp->DestroyAll();
    }

    if (g_pQMGR_Real)
    {
	    g_pQMGR_Real->SerializeStream(g_ClientQuestCompleteTotalInfo, QSERIALIZE_LOAD);
    }


    if (g_pQMGR_Temp)
    {
	    g_pQMGR_Temp->SerializeStream(g_ClientQuestCompleteTotalInfo, QSERIALIZE_SAVE);
    }

	QCODE QuestCode = 1;
    Object_Map_Itr found_itr = g_ObjectManager.GetBegin();
    Object_Map_Itr end_itr = g_ObjectManager.GetEnd();
    for ( ; found_itr != end_itr; ++found_itr)
	{			
		eQUEST_GAIN_STATE eRefState = QUEST_NONE;
		eQUEST_GAIN_STATE eCurState = QUEST_NONE;

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
		if ( !pNPC )
		{
			continue;
		}
		pNPC->ClearQuestInfo();

		for (QuestCode = 1; QuestCode < MAX_QUEST_COMPL_NUM; ++QuestCode)
		{
			eRefState = GetQuestGainState(QuestCode, pNPC->GetExtraInfo()->m_NPCCODE, pNPC);

			if ( QUEST_NONE == eCurState )
			{
				eCurState = eRefState;		
			}
			else if (QUEST_REWARD == eRefState) 
			{
				// 우선순위가 제일 높기 때문에 더 볼것도 없다.
				eCurState = eRefState;
				break;
			}
			else
			{
				if ( PriorityQuestStateTable[eCurState] < PriorityQuestStateTable[eRefState] )
				{
					eCurState = eRefState;
				}
			}	
		}

		int iEmoticon = HEAD_MESH_NONE;

		switch ( eCurState )
		{
		case QUEST_ACCEPT:		
			{
#ifdef _NA_004921_20120628_ADD_QUEST_NPC_EMOTICON_FOR_QUEST_TYPE
                if (pNPC->GetMaxQuestRepeat() == 1)
                {
                    iEmoticon = HEAD_MESH_SCROLL_ACTIVE;
                }
                else if (pNPC->IsHaveGuildQuest() == true)
                {
                    iEmoticon = HEAD_MESH_SCROLL_GUILD;
                }
                else if (pNPC->IsHaveDailyQuest() == true)
                {
                    iEmoticon = HEAD_MESH_SCROLL_DAILY;
                }
                else
                {
                    iEmoticon = HEAD_MESH_SCROLL_REPEAT;
                }
#else
                iEmoticon = HEAD_MESH_SCROLL_ACTIVE;
#endif //_NA_004921_20120628_ADD_QUEST_NPC_EMOTICON_FOR_QUEST_TYPE
			}
			break;
		case QUEST_REWARD:		
			{
#ifdef _NA_004921_20120628_ADD_QUEST_NPC_EMOTICON_FOR_QUEST_TYPE
                if (pNPC->GetMaxQuestRepeat() == 1)
                {
                    iEmoticon = HEAD_MESH_DOCUMENT_ACTIVE;;
                }
                else if (pNPC->IsHaveGuildQuest() == true)
                {
                    iEmoticon = HEAD_MESH_DOCUMENT_GUILD;
                }
                else if (pNPC->IsHaveDailyQuest() == true)
                {
                    iEmoticon = HEAD_MESH_DOCUMENT_DAILY;
                }
                else
                {
                    iEmoticon = HEAD_MESH_DOCUMENT_REPEAT
                }
#else
                iEmoticon = HEAD_MESH_DOCUMENT_ACTIVE;
#endif //_NA_004921_20120628_ADD_QUEST_NPC_EMOTICON_FOR_QUEST_TYPE
			}
			break;
		case QUEST_INPERFECT_REWARD:
			{
				iEmoticon = HEAD_MESH_DOCUMENT_DEACTIVE;
			}
			break;

		case QUEST_CANT_ACCEPT_LV:
        case QUEST_CANT_ACCEPT_DAILYQUEST:
			{
				iEmoticon = HEAD_MESH_SCROLL_DEACTIVE;
			}
			break;

		case QUEST_PROGRESS:
		case QUEST_NONE:
		case QUEST_ACCEPT_NONE_DISPLAY:
		default:
			break;
		}

		// eRefState 타입에 따라서 특정 헤드메쉬 타입을 설정해주자.
		pNPC->ShowEmoticonIcon( iEmoticon, INT_MAX); // 일단 INT_MAX초
	}	
}

//-------------------------------------------------------------------------------------------
/**
	얻어야 할 내용
	  1. 현재 NPC 에게서 보상 받을 수 있는 퀘스트 목록			 [ 보상 ]
		m_CompleteQuestList
	  2. 현재 NPC 에게서 보상 받을 수 있으나 완료 하지 못한 목록 [ 보상 ] 
		m_InperfectCompleteQuestList
	  3. 현재 NPC 에게서 수락 받아서 진행중인 퀘스트 목록		 [ 보상 ]
		m_ProgressQuestList		
	  4. 현재 NPC 에서 수락 받을 수 있는 목록					 [ 임무 ]
		(단, 진행중인 퀘스트를 제외한)
		m_CanAcceptQuestList


		// TODO : 완료된 퀘스트중에 더이상 보상 받을 수 없는 것은?
*/
BOOL			
uiQuestMan::getQuestListsMeetNPC()	
{
	clearQuestList();

    if (g_pQMGR_Temp)
    {
	    g_pQMGR_Temp->DestroyAll();
    }

    if (g_pQMGR_Real)
    {
	    g_pQMGR_Real->SerializeStream(g_ClientQuestCompleteTotalInfo, QSERIALIZE_LOAD);
    }

    if (g_pQMGR_Temp)
    {
	    g_pQMGR_Temp->SerializeStream(g_ClientQuestCompleteTotalInfo, QSERIALIZE_SAVE);
    }
	
	QCODE QuestCode = 1;
	for (; QuestCode < MAX_QUEST_COMPL_NUM; ++QuestCode)
	{
		switch ( GetQuestGainState(QuestCode, GetNPCCode()) )
		{
		case QUEST_ACCEPT:
		case QUEST_ACCEPT_NONE_DISPLAY:
		case QUEST_CANT_ACCEPT_LV:
			{
				// 4. 현재 NPC 에서 수락 받을 수 있는 목록						[ 임무 ]
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
                m_CanAcceptQuestList.push_back(QuestCode);
#else
				Quest * pQuest = g_pQMGR_Temp->FindQuest(QuestCode);
				m_CanAcceptQuestList.push_back(pQuest);
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
			}
			break;
		case QUEST_PROGRESS:
			{
				Quest * pRealQuest = g_pQMGR_Real->FindQuest(QuestCode);
				//3. 현재 NPC 에게서 수락 받아서 진행중인 퀘스트 목록			[ 보상 ]
				//m_ProgressQuestList.push_back(pRealQuest);	
			}
			break;
		case QUEST_REWARD:
			{
				// 1. 현재 NPC 에게서 보상 받을 수 있는 퀘스트 목록				[ 보상 ]
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
                m_CompleteQuestList.push_back(QuestCode);
#else
				Quest * pRealQuest = g_pQMGR_Real->FindQuest(QuestCode);
				m_CompleteQuestList.push_back(pRealQuest);
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
			}
			break;
		case QUEST_INPERFECT_REWARD:
            {
				// 2. 현재 NPC 에게서 보상 받을 수 있으나 완료 하지 못한 목록	[ 보상 ]
				// 기획자에 의해서 이것도 진행중인 목록에 포함.
				// m_InperfectCompleteQuestList
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
                m_ProgressQuestList.push_back(QuestCode);
#else
				Quest * pRealQuest = g_pQMGR_Real->FindQuest(QuestCode);
				m_ProgressQuestList.push_back(pRealQuest);
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
			}
			break;
        case QUEST_CANT_ACCEPT_DAILYQUEST:
            {
#ifdef _NA_003027_20111013_HONOR_SYSTEM
                // 초기화까지의 시간 출력을 위한 정보 수집
  #ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
                // 사용하는 곳을 찾지는 못함
                m_CompleteDailyQuestList.push_back(QuestCode);
  #else
                Quest* pQuest = g_pQMGR_Temp->FindQuest(QuestCode);
                m_CompleteDailyQuestList.push_back(pQuest);
  #endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
#endif //_NA_003027_20111013_HONOR_SYSTEM
            }
            break;

		default:
			continue;
			break;
		}
	}

	int count_of_list = m_CompleteQuestList.size() + m_InperfectCompleteQuestList.size() +
			m_ProgressQuestList.size() + m_CanAcceptQuestList.size();
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    count_of_list += m_CompleteDailyQuestList.size();
#endif //_NA_003027_20111013_HONOR_SYSTEM

	return (BOOL)count_of_list;
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::clearQuestList()
{
	m_CanAcceptQuestList.clear();
	m_CompleteQuestList.clear();
	m_InperfectCompleteQuestList.clear();
	m_ProgressQuestList.clear();
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    m_CompleteDailyQuestList.clear();
#endif //_NA_003027_20111013_HONOR_SYSTEM
}

//-------------------------------------------------------------------------------------------
/**
*/
eQUEST_GAIN_STATE 
uiQuestMan::GetStateInQuestListForForward(QCODE QuestCode)
{
    QUEST_LIST::iterator itr = m_CanAcceptQuestList.begin();
    for (; itr != m_CanAcceptQuestList.end(); ++itr)
    {
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
        QCODE quest_code = *itr;

        if ( quest_code == QuestCode )
        {
            return QUEST_ACCEPT;
        }
#else
        Quest * pQuest = *itr;

        if ( !pQuest ) 
            continue;

        if ( pQuest->GetQuestCode() == QuestCode )
        {
            return QUEST_ACCEPT;
        }
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
    }

    itr = m_CompleteQuestList.begin();
    for (; itr != m_CompleteQuestList.end(); ++itr)
    {
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
        QCODE quest_code = *itr;

        if ( quest_code == QuestCode )
        {
            return QUEST_REWARD;
        }
#else
        Quest * pQuest = *itr;

        if ( !pQuest ) 
            continue;

        if ( pQuest->GetQuestCode() == QuestCode )
        {
            return QUEST_REWARD;
        }
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
    }

    itr = m_InperfectCompleteQuestList.begin();
    for (; itr != m_InperfectCompleteQuestList.end(); ++itr)
    {
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
        QCODE quest_code = *itr;

        if ( quest_code == QuestCode )
        {
            return QUEST_INPERFECT_REWARD;
        }
#else
        Quest * pQuest = *itr;

        if ( !pQuest ) 
            continue;

        if ( pQuest->GetQuestCode() == QuestCode )
        {
            return QUEST_INPERFECT_REWARD;
        }
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
    }

    itr = m_ProgressQuestList.begin();
    for (; itr != m_ProgressQuestList.end(); ++itr)
    {
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
        QCODE quest_code = *itr;

        if ( quest_code == QuestCode )
        {
            return QUEST_PROGRESS;
        }
#else
        Quest * pQuest = *itr;

        if ( !pQuest ) 
            continue;

        if ( pQuest->GetQuestCode() == QuestCode )
        {
            return QUEST_PROGRESS;
        }
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
    }
    
    return QUEST_NONE;
}