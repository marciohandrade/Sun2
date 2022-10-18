#include "SunClientPrecompiledHeader.h"
#include ".\tutorialmsg.h"
#include "InterfaceManager.h"
#ifdef _GS_MOD_CNINA_TUTORIAL_POPUP
#include "uiTutorialMissionMan.h"
#include "TutorialMissionInvite.h"
#endif//_GS_MOD_CNINA_TUTORIAL_POPUP

#define TUTORIAL_MSG_TIME	5000

TutorialMsg::TutorialMsg(void)
: m_dwCurTime(0),
  m_eState(eTUTORIAL_NONE),
  m_eOldState(eTUTORIAL_NONE)
{
}

TutorialMsg::~TutorialMsg(void)
{

}
//------------------------------------------------------------------------------
/** Ʃ�丮�� �ʱ�ȭ����
	���� 0815�� 0816���̿� �������ϰ� �޶� �������� ����
*/
void TutorialMsg::InitTutorial(TCHAR *szName)
{
	//NULL���� ������ �ʱ�ȭ�����̴�
	if(!szName)
	{	
		m_eOldState = m_eState	= eTUTORIAL_NONE;
		return;
	}

//ver0816-Ʃ�丮��
#ifdef _GS_MOD_TUTORIAL_MSG2_1135
	//��â���� �Դٸ� �����Ѵ�.
	if(m_eOldState==eTUTORIAL_STEP4) return;

	#ifdef _GS_MOD_CNINA_TUTORIAL_POPUP
		SetTutorialStep(eTUTORIAL_STEP0);
	#else
		SetTutorialStep(eTUTORIAL_STEP1);
	#endif//_GS_MOD_CNINA_TUTORIAL_POPUP
	
	return;
#endif//_GS_MOD_TUTORIAL_MSG2_1135

//ver0815-Ʃ�丮��
#ifdef _GS_MOD_TUTORIAL_MSG_1089
	if(!m_vPlayerName.empty())
	{
		for(size_t i = 0;i < m_vPlayerName.size();++i)
		{
			TCHAR * pszName = const_cast<TCHAR *>(m_vPlayerName[i].c_str());

			if(NULL==Strcmp(pszName, szName))
			{
				//�ѹ��ô�ĳ���ʹ� �޽ø� �Ѹ����ʴ´�.
				return;
			}
		}
	}
	m_vPlayerName.push_back(util::_tstring(szName));
	
	#ifdef _GS_MOD_CNINA_TUTORIAL_POPUP
		SetTutorialStep(eTUTORIAL_STEP0);
	#else
		SetTutorialStep(eTUTORIAL_STEP1);
	#endif//_GS_MOD_CNINA_TUTORIAL_POPUP

	return;
#endif//_GS_MOD_TUTORIAL_MSG_1089

}
//------------------------------------------------------------------------------
/** 
*/
void TutorialMsg::UpdateTutorial()
{
	//�ʱ�ȭ�ȵȻ��¸� ����
	if(eTUTORIAL_NONE==m_eState || m_listStringCode.empty()) return; 

	switch(m_eState)
	{
	//5�ʰ������� �޽������
	case eTUTORIAL_STEP1:
	case eTUTORIAL_STEP4:
		{
			//���ѽð��� �Ǹ� �����޽������
			if( (g_CurTime - m_dwCurTime) >= TUTORIAL_MSG_TIME )
			{
				m_dwCurTime =  g_CurTime;
				
				int iStringCode  = m_listStringCode.front();
				
				g_InterfaceManager.GetInterfaceString(iStringCode, m_szMessage ,INTERFACE_STRING_LENGTH);	
				
				m_listStringCode.pop_front();

				int iDelay = (m_listStringCode.empty() ? (-1):(TUTORIAL_MSG_TIME));
				
				g_InterfaceManager.ShowMessageBox(m_szMessage, iDelay);

			}
		}
		break;

	//��Ʋ��â������ ó��
	case eTUTORIAL_STEP2:
	case eTUTORIAL_STEP3:
		{
			g_InterfaceManager.GetInterfaceString(m_listStringCode.front(), m_szMessage ,INTERFACE_STRING_LENGTH);	

			g_InterfaceManager.ShowMessageBox(m_szMessage,-1);

			m_listStringCode.pop_front();
		}
		break;
	}
	
}
//------------------------------------------------------------------------------
/**  ���ܺ��� �ϵ��ڵ��� �Ѵ�.
*/ 
void TutorialMsg::SetTutorialStep(eTUTORIAL_STATE eState)
{
	//��Ʈ���ڵ�� �׻� ����¿�����
	if(!m_listStringCode.empty())
		m_listStringCode.clear();
	
	//�����޽����� �����Ѵ�.
	g_InterfaceManager.HideMessageBox();
	
	m_eOldState = m_eState;
	m_eState	= eState;

	switch(eState)
	{
#ifdef _GS_MOD_CNINA_TUTORIAL_POPUP
	//STEP0:�߱����������� Ʃ�丮�� �޽��������� �˾�â������
	case eTUTORIAL_STEP0:
		{
			// Ʃ�丮�� �̼�â ����
			TutorialMissionInvite * pTutorial = GET_CAST_DIALOG(TutorialMissionInvite, IM_TUTORIAL_MISSION_MANAGER::TUTORIALINVITE);
		
			if( pTutorial )
			{
				pTutorial->ShowInterface(TRUE);
			}

			g_InterfaceManager.LockDlgs(IM_TUTORIAL_MISSION_MANAGER::TUTORIALINVITE);
		}
		break;
#endif//_GS_MOD_CNINA_TUTORIAL_POPUP

	//STEP1:�ʵ嵵�Ժο��� �޽��������Ѵ�.
	case eTUTORIAL_STEP1:
		{
			m_dwCurTime = g_CurTime-3500;
//ver0816 - Ʃ�丮��
#ifdef _GS_MOD_TUTORIAL_MSG2_1135
			//203203~203207 �޽�������
			for(int i=203203;i<203208;++i)
			{
				m_listStringCode.push_back(i);
			}
			break;
#endif//_GS_MOD_TUTORIAL_MSG2_1135

//ver0815 - Ʃ�丮��
#ifdef _GS_MOD_TUTORIAL_MSG_1089
			//203202~203210���� ����
			for(int i=203202;i<203211;++i)
			{
				m_listStringCode.push_back(i);
			}
			break;
#endif//_GS_MOD_TUTORIAL_MSG_1089
		}
		break;

	case eTUTORIAL_STEP2://�κ�â
		{
			m_listStringCode.push_back(203208);
		}
		break;

	case eTUTORIAL_STEP3://����â
		{
			m_listStringCode.push_back(203209);
		}
		break;
	case eTUTORIAL_STEP4://��â
		{
			m_listStringCode.push_back(203210);
			m_listStringCode.push_back(203211);
		}
		break;
	}
}






















