//------------------------------------------------------------------------------
/**

	Ʃ�丮�� �ϵ��ڵ�Ŭ����

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
		eTUTORIAL_NONE=0,	//�ش���׾���
#ifdef _GS_MOD_CNINA_TUTORIAL_POPUP
		eTUTORIAL_STEP0,	//�߱������߰�:������ �˾�
#endif//_GS_MOD_CNINA_TUTORIAL_POPUP
		eTUTORIAL_STEP1,	//�ʵ忡�� MSG�� ����
		eTUTORIAL_STEP2,	//�κ�â
		eTUTORIAL_STEP3,	//����â
		eTUTORIAL_STEP4,    //��â
		eTUTORIAL_IDLE
	};

public:
	TutorialMsg(void);
	~TutorialMsg(void);


public:
	//�ܺ��������̽�
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

