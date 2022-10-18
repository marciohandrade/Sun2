#include "SunClientPrecompiledHeader.h"

#include "uiQuestMan.h"
#include "npcInfoParser.h"
#include "uiQuestCommonForward/uiQuestCommonForward.h"
#include "uiQuestReward/uiQuestReward.h"
#include "NPC.h"
#include "ObjectManager.h"
#include "uiQuestAutoAcceptAlram/uiQuestAutoAcceptAlram.h"
#include "globalfunc.h"
#include "QuestInfoManagerEx.h"

//-------------------------------------------------------------------------------------------
/**
*/
SolarDialog *			
uiQuestMan::OpenRewardWindow( QuestInfo * pQuestInfo, eQUEST_GAIN_STATE eGainState )
{
	if (!m_pUIQuestReward )
		return NULL;

	if ( !pQuestInfo )
		return NULL;

	m_pUIQuestReward->SetCurQuestCode(pQuestInfo, eGainState);

	// ���� NPC �� ����Ʈ �����ؼ� �����ִ� â�� �ݰ�.
	CloseQuestNPC();


	switch ( pQuestInfo->GetQuestAcceptType() )
	{
	case eQUEST_ACCEPT_GENERAL:
		{
			NPC * pNPC = (NPC *)g_ObjectManager.GetObject(this->GetPendingNPCKey());
			if ( pNPC )
			{
				pNPC->SetOpenDialog( (SolarDialog *)this->m_pUIQuestReward );			
			}

			m_pUIQuestReward->ShowInterfaceWithSound(TRUE);

			// key queueing
			keyMsg msg;
			ZeroMemory(&msg,sizeof(msg));
			msg.dwType = eDoType_dialog_Quest_NPC;
			msg.wParam = QUEST_WINDOW_NPC;
			msg.DoSomething = GlobalFunc::DoShowQuestWindow;
			g_KeyQueueManager.PushBack(msg);

		}
		break;

    case eQUEST_ACCEPT_AUTO:
    default:
        {
            // ESC Ű�� ���ݰ� ��.
            m_pUIQuestReward->ShowInterfaceWithSound(TRUE);
        }		
        break;
	}

	return this->m_pUIQuestReward;
}


SolarDialog *	
uiQuestMan::OpenRewardWindowForAutoAlram( Quest * pQuest )
{
	if (!m_pUIQuestReward )
		return NULL;

	assert( pQuest );
	if ( !pQuest )
		return NULL;

    // ���� NPC �� ����Ʈ �����ؼ� �����ִ� â�� �ݰ�.
    CloseQuestNPC();

	m_pPendingQuest = pQuest;

	SetForwardAutoAlram(TRUE);
    OpenRewardWindow(pQuest->GetQuestInfo(), QUEST_AUTO_ACCEPT );
	SetForwardAutoAlram(FALSE);

	return m_pUIQuestReward;
}




//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::addAcceptAutoQuest(Quest * pQuest)
{
	assert(pQuest);
	if (!pQuest)
	{
		return;
	}

	// add autoAcceptQuest Vector
	m_PendingAutoQuestList.push_back( pQuest->GetQuestCode() );

	// Refresh AutoQuest Alram Window	
    m_pUIAutoAcceptAlram->ShowInterface(TRUE);
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::ClearAcceptAuto()
{
	m_PendingAutoQuestList.clear();
}

//-------------------------------------------------------------------------------------------
/**
*/
Quest *			
uiQuestMan::GetAcceptAutoQuest()
{
	Quest * pQuest = NULL;
	if (0 == GetAcceptAutoQuestCount() )
	{
		return NULL;
	}

	std::list<QCODE>::iterator itr = m_PendingAutoQuestList.begin();
	QCODE QuestCode = (QCODE)(*itr);
	m_PendingAutoQuestList.erase( itr );

	pQuest = g_pQMGR_Real->FindQuest( QuestCode );
	       
	return pQuest;
}

//-------------------------------------------------------------------------------------------
/**
*/
WORD			
uiQuestMan::GetAcceptAutoQuestCount()
{
	return m_PendingAutoQuestList.size();
}





