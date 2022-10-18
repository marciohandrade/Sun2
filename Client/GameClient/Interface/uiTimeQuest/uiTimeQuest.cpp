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
	StrToWzID("T001"),		// ����Ʈ���� DLG_QUEST_NAME,
	StrToWzID("T002"),		// �����ð�	  DLG_REMAINING_TIME
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
/**	Ÿ������Ʈ ����
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
			//�������� ���۽� �ѹ���
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

	//�������һ�Ȳ��
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

	//1�г������� �޽����� ��´�
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

	//�����ð�:
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
	//TODO: �̺�Ʈ ó���� �̰���
	return;
}

