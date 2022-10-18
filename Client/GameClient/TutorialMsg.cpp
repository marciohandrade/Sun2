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
/** 튜토리얼 초기화구문
	현재 0815와 0816사이에 어정쩡하게 달라서 디파인을 공존
*/
void TutorialMsg::InitTutorial(TCHAR *szName)
{
	//NULL값이 들어오면 초기화상태이다
	if(!szName)
	{	
		m_eOldState = m_eState	= eTUTORIAL_NONE;
		return;
	}

//ver0816-튜토리얼
#ifdef _GS_MOD_TUTORIAL_MSG2_1135
	//룸창에서 왔다면 리턴한다.
	if(m_eOldState==eTUTORIAL_STEP4) return;

	#ifdef _GS_MOD_CNINA_TUTORIAL_POPUP
		SetTutorialStep(eTUTORIAL_STEP0);
	#else
		SetTutorialStep(eTUTORIAL_STEP1);
	#endif//_GS_MOD_CNINA_TUTORIAL_POPUP
	
	return;
#endif//_GS_MOD_TUTORIAL_MSG2_1135

//ver0815-튜토리얼
#ifdef _GS_MOD_TUTORIAL_MSG_1089
	if(!m_vPlayerName.empty())
	{
		for(size_t i = 0;i < m_vPlayerName.size();++i)
		{
			TCHAR * pszName = const_cast<TCHAR *>(m_vPlayerName[i].c_str());

			if(NULL==Strcmp(pszName, szName))
			{
				//한번봤던캐릭터는 메시를 뿌리지않는다.
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
	//초기화안된상태면 리턴
	if(eTUTORIAL_NONE==m_eState || m_listStringCode.empty()) return; 

	switch(m_eState)
	{
	//5초간격으로 메시지출력
	case eTUTORIAL_STEP1:
	case eTUTORIAL_STEP4:
		{
			//제한시간이 되면 다음메시지출력
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

	//배틀존창에서의 처리
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
/**  스텝별로 하드코딩를 한다.
*/ 
void TutorialMsg::SetTutorialStep(eTUTORIAL_STATE eState)
{
	//스트링코드는 항상 빈상태여야함
	if(!m_listStringCode.empty())
		m_listStringCode.clear();
	
	//기존메시지는 제거한다.
	g_InterfaceManager.HideMessageBox();
	
	m_eOldState = m_eState;
	m_eState	= eState;

	switch(eState)
	{
#ifdef _GS_MOD_CNINA_TUTORIAL_POPUP
	//STEP0:중국버전에서는 튜토리얼 메시지이전에 팝업창를띄운다
	case eTUTORIAL_STEP0:
		{
			// 튜토리얼 미션창 띄우기
			TutorialMissionInvite * pTutorial = GET_CAST_DIALOG(TutorialMissionInvite, IM_TUTORIAL_MISSION_MANAGER::TUTORIALINVITE);
		
			if( pTutorial )
			{
				pTutorial->ShowInterface(TRUE);
			}

			g_InterfaceManager.LockDlgs(IM_TUTORIAL_MISSION_MANAGER::TUTORIALINVITE);
		}
		break;
#endif//_GS_MOD_CNINA_TUTORIAL_POPUP

	//STEP1:필드도입부에서 메시지랜더한다.
	case eTUTORIAL_STEP1:
		{
			m_dwCurTime = g_CurTime-3500;
//ver0816 - 튜토리얼
#ifdef _GS_MOD_TUTORIAL_MSG2_1135
			//203203~203207 메시지삽입
			for(int i=203203;i<203208;++i)
			{
				m_listStringCode.push_back(i);
			}
			break;
#endif//_GS_MOD_TUTORIAL_MSG2_1135

//ver0815 - 튜토리얼
#ifdef _GS_MOD_TUTORIAL_MSG_1089
			//203202~203210까지 삽입
			for(int i=203202;i<203211;++i)
			{
				m_listStringCode.push_back(i);
			}
			break;
#endif//_GS_MOD_TUTORIAL_MSG_1089
		}
		break;

	case eTUTORIAL_STEP2://로비창
		{
			m_listStringCode.push_back(203208);
		}
		break;

	case eTUTORIAL_STEP3://생성창
		{
			m_listStringCode.push_back(203209);
		}
		break;
	case eTUTORIAL_STEP4://룸창
		{
			m_listStringCode.push_back(203210);
			m_listStringCode.push_back(203211);
		}
		break;
	}
}






















