//------------------------------------------------------------------------------
/**

	튜토리얼 하드코딩클래스

	@author
		GS
	@since 
		-08.08.11
	@remarks
	
- 
*/

#pragma once
#include "Singleton.h"

class TutorialMsg: public Singleton<TutorialMsg>
{

public:
	enum eTUTORIAL_STATE
	{
		eTUTORIAL_NONE=0,	//해당사항없음
#ifdef _GS_MOD_CNINA_TUTORIAL_POPUP
		eTUTORIAL_STEP0,	//중국버전추가:시작전 팝업
#endif//_GS_MOD_CNINA_TUTORIAL_POPUP
		eTUTORIAL_STEP1,	//필드에서 MSG만 랜더
		eTUTORIAL_STEP2,	//로비창
		eTUTORIAL_STEP3,	//생성창
		eTUTORIAL_STEP4,    //룸창
		eTUTORIAL_IDLE
	};

public:
	TutorialMsg(void);
	~TutorialMsg(void);


public:
	//외부인터페이스
	void								InitTutorial(TCHAR *szName);
	void								UpdateTutorial();
	void								SetTutorialStep(eTUTORIAL_STATE eState);
	eTUTORIAL_STATE						IsTutorialMode(){return m_eState;}

private:
	TCHAR								m_szMessage[INTERFACE_STRING_LENGTH];
	DWORD								m_dwCurTime;
	std::list<int>						m_listStringCode;
	std::vector<util::_tstring>			m_vPlayerName;
	eTUTORIAL_STATE						m_eState;
	eTUTORIAL_STATE						m_eOldState;
};

