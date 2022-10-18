#include "SunClientPrecompiledHeader.h"

#include "uiTimeQuest.h"
#include "uiQuestMan/uiQuestMan.h"
#include "GlobalFunc.h"
#include "interfacemanager.h"
#include "QuestManager_Concrete.h"
#include "GameFramework.h"
//------------------------------------------------------------------------------
WzID uiTimeQuest::m_wzId[DLG_MAX] = 
{
	StrToWzID("T001"),		// 퀘스트네임 DLG_QUEST_NAME,
	StrToWzID("T002"),		// 남은시간	  DLG_REMAINING_TIME
    StrToWzID("P000"),		//DLG_QUEST_NAME_PICTRURE,
    StrToWzID("P001"),		//DLG_REMAINING_TIME_PICTRURE,
};

uiTimeQuest::uiTimeQuest(InterfaceManager *pUIMan)
:uiBase(pUIMan),
 m_pTimeQuest(NULL),
 m_bShowLastMin(true)
{
	ZeroMemory(&m_pTxtControl,sizeof(m_pTxtControl));	 
}
//------------------------------------------------------------------------------
/** 
*/
uiTimeQuest::~uiTimeQuest(void)
{
	Release();
}
//------------------------------------------------------------------------------
/** 
*/
void
uiTimeQuest::Release()
{
}
//------------------------------------------------------------------------------
/** 
*/
VOID	
uiTimeQuest::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);
	this->InitControls();
}
//------------------------------------------------------------------------------
/** 
*/
void
uiTimeQuest::InitControls()
{
	m_pTxtControl[DLG_QUEST_NAME]	 = static_cast<CCtrlStaticWZ *>(this->getControl(DLG_QUEST_NAME));
	m_pTxtControl[DLG_REMAINING_TIME]= static_cast<CCtrlStaticWZ *>(this->getControl(DLG_REMAINING_TIME));

    if (m_pTxtControl[DLG_QUEST_NAME])
    {
        m_pTxtControl[DLG_QUEST_NAME]->SetDiscardMessage(true);
    }
    CControlWZ* control_pointer = static_cast<CControlWZ*>(this->getControl(DLG_QUEST_NAME_PICTRURE));
    if (control_pointer)
    {
        control_pointer->SetDiscardMessage(true);
    }
}
//------------------------------------------------------------------------------
/** 
*/
uiQuestMan *
uiTimeQuest::GetManager()
{
	uiQuestMan* puiQuestMan = static_cast<uiQuestMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_QUEST));
	return puiQuestMan;
}
//------------------------------------------------------------------------------
/**	
*/
void
uiTimeQuest::OnShowWindow( BOOL val )
{
	if (val)
	{
		_asm nop;
	}
	else
	{
		_asm nop;
	}
}
//------------------------------------------------------------------------------
/**	타임퀘스트 셋팅
*/
void
uiTimeQuest::SetTimeQuest(Quest * pQuest)
{
	if (NULL == m_pTimeQuest && pQuest)
	{
		m_bShowLastMin = true;
	}

	m_pTimeQuest = pQuest;

	if(m_pTimeQuest)
	{
		if(m_pTxtControl[DLG_QUEST_NAME])
		{
			//퀘스명은 시작시 한번만
			TCHAR szQuestName[INTERFACE_STRING_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString(m_pTimeQuest->GetQuestInfo()->GetQNCode(),szQuestName,INTERFACE_STRING_LENGTH);
			GlobalFunc::SetCaptionSimple( m_pTxtControl[DLG_QUEST_NAME], szQuestName );			
		}
	}
}
//------------------------------------------------------------------------------
/**	
*/
void
uiTimeQuest::OnUpdateSolarDialog()
{
	if(!m_pTimeQuest)return; 

	if(m_pTimeQuest->GetRemainedTime()==0)
	{	
		g_pQMGR_Real->OnEventQuestTime(m_pTimeQuest);
	}

	//닫혀야할상황들
	switch(GameFramework::GetCurScene())
	{
	case SCENE_TYPE_CHARSELECT:
	case SCENE_TYPE_LOGIN:
	case SCENE_TYPE_LOBBY:
	case SCENE_TYPE_NONE:
		{
			if(IsVisible())
			{
				this->ShowInterface(FALSE);
			}
		}
		break;
	}
}
//------------------------------------------------------------------------------
/**	render
*/
void uiTimeQuest::OnRenderSolarDialog()
{
	if(!m_pTimeQuest) return;

	if(!m_pTxtControl[DLG_REMAINING_TIME]) return;

	DWORD dwRemainTime  = m_pTimeQuest->GetRemainedTime();

	DWORD dwTime		= dwRemainTime / SEC_PER_TICK;
	DWORD dwTimeEx		= dwRemainTime % SEC_PER_TICK;

	DWORD minTick   = 0;
	DWORD secTick   = 0;
	DWORD msecTick  = 0;

	if(dwTime)
	{
		minTick   = dwTime / 60;
		secTick   = dwTime % 60;
	}

	if(dwTimeEx)
		msecTick  = dwTimeEx/ 10;

	//1분남았을때 메시지를 찍는다
	if( minTick == 0 && secTick <= 59 && m_bShowLastMin)
	{
		if(GetManager())
		{
			GetManager()->QuestMessage(5387);
			m_bShowLastMin = false;
		}
	}
	
	TCHAR szTime[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	//남은시간:
	g_InterfaceManager.GetInterfaceString(1767,szMessage,INTERFACE_STRING_LENGTH);
	//00'00'00
	Snprintf (szTime, INTERFACE_STRING_LENGTH-1, _T(" %02d'%02d''%02d"), minTick, secTick, msecTick);
	
	_tcscat( szMessage , szTime );
	GlobalFunc::SetCaption( m_pTxtControl[DLG_REMAINING_TIME], szMessage );

}
//------------------------------------------------------------------------------
/**	msg_proc
*/
void 
uiTimeQuest::MessageProc( SI_MESSAGE * pMessage )
{
	//TODO: 이벤트 처리는 이곳에
	return;
}

