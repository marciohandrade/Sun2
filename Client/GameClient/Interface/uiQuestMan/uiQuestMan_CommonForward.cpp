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

// ���ڰ� ������ �켱������ ����.
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

	// ���� �����ؼ� ������ ����Ʈ��
	// �����ؾ��� �ӹ��� �ִ��� Check �ؼ� ������ ä���.
    BOOL bGetListRet = getQuestListsMeetNPC();

	// ���� �����ؾ��� ����Ʈ�� ���� ���� ���� ����Ʈ�� ���ٸ�
	// �ٷ� ����â�� ����ְ� ������ ���³��̶�� â�� ���������.
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

		// ���� NPC �� ����Ʈ �����ؼ� �����ִ� â�� �ݰ�.
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

	// ����
	Quest * pRealQuest = g_pQMGR_Real->FindQuest(QuestCode);
	if ( pRealQuest )
	{
		if ( pRealQuest->GetQuestInfo()->IsChildQuest() )
		{ 
			// �ڽ� ����Ʈ�� ��Ͽ��� ����.
			return QUEST_NONE;
		}

		// ���� ���� ����Ʈ ���
		// ������� �� �ִ� ������[�Ϸ�ǥ��]�� ����Ʈ
		BOOL bRetCompl = BOOL(pRealQuest->GetQState() == QS_COMPLETED);// g_pQMGR_Real->CanComplete(QuestCode, g_pHero);
		if( bRetCompl )
		{
			if ( NPCCode == pRealQuest->GetQuestInfo()->GetNPCCode_Reward() )
			{
				// 1. ���� NPC ���Լ� ���� ���� �� �ִ� ����Ʈ ���				[ ���� ]
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
				// 2. ���� NPC ���Լ� ���� ���� �� ������ �Ϸ� ���� ���� ���	[ ���� ]
				return QUEST_INPERFECT_REWARD;
			}
			else if (NPCCode == pRealQuest->GetQuestInfo()->GetNPCCode_Start())
			{
				//3. ���� NPC ���Լ� ���� �޾Ƽ� �������� ����Ʈ ���			[ �ӹ� ]
				return QUEST_PROGRESS;
			}
		}
	}
	else
	{
		// ���� ������ ����Ʈ		
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
			// �ڽ� ����Ʈ�� ��Ͽ��� ����.
			return QUEST_NONE;
		}

        // ���ǽ� �ڵ� üũ
        if (NPCCode != pQuest->GetQuestInfo()->GetNPCCode_Start())
        {
            return QUEST_NONE;
        }

        // ���ǽ� ��ġ üũ(���ڵ�, �ʵ��ڵ�)
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

            // 4. ���� NPC ���� ���� ���� �� �ִ� ���						[ �ӹ� ]
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
/** �� �Լ��� ȣ���ؾ� �Ҷ�
	1. Player�� ó�� ��Ʋ���� ��������
	2. Accept �޾��� ��
	3. �Ϸ� ���� ��
	4. ���� �޾��� ��
	5. ���� ���� ��
	6. ���� ���� ��
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
				// �켱������ ���� ���� ������ �� ���͵� ����.
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

		// eRefState Ÿ�Կ� ���� Ư�� ���޽� Ÿ���� ����������.
		pNPC->ShowEmoticonIcon( iEmoticon, INT_MAX); // �ϴ� INT_MAX��
	}	
}

//-------------------------------------------------------------------------------------------
/**
	���� �� ����
	  1. ���� NPC ���Լ� ���� ���� �� �ִ� ����Ʈ ���			 [ ���� ]
		m_CompleteQuestList
	  2. ���� NPC ���Լ� ���� ���� �� ������ �Ϸ� ���� ���� ��� [ ���� ] 
		m_InperfectCompleteQuestList
	  3. ���� NPC ���Լ� ���� �޾Ƽ� �������� ����Ʈ ���		 [ ���� ]
		m_ProgressQuestList		
	  4. ���� NPC ���� ���� ���� �� �ִ� ���					 [ �ӹ� ]
		(��, �������� ����Ʈ�� ������)
		m_CanAcceptQuestList


		// TODO : �Ϸ�� ����Ʈ�߿� ���̻� ���� ���� �� ���� ����?
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
				// 4. ���� NPC ���� ���� ���� �� �ִ� ���						[ �ӹ� ]
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
				//3. ���� NPC ���Լ� ���� �޾Ƽ� �������� ����Ʈ ���			[ ���� ]
				//m_ProgressQuestList.push_back(pRealQuest);	
			}
			break;
		case QUEST_REWARD:
			{
				// 1. ���� NPC ���Լ� ���� ���� �� �ִ� ����Ʈ ���				[ ���� ]
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
				// 2. ���� NPC ���Լ� ���� ���� �� ������ �Ϸ� ���� ���� ���	[ ���� ]
				// ��ȹ�ڿ� ���ؼ� �̰͵� �������� ��Ͽ� ����.
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
                // �ʱ�ȭ������ �ð� ����� ���� ���� ����
  #ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
                // ����ϴ� ���� ã���� ����
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