#include "SunClientPrecompiledHeader.h"

#include "uiHelpMan/uiHelpMan.h"
#include "uiHelpMonsterInfo.h"
#include "Hero.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "ObjectManager.h"
#include "HeroActionInput.h"
#include "Monster.h"
#include "SkillInfoParser.h"
#include "DlgPosInfoParser.h"


const int c_Help_iPageLine	= 10;
const int c_Help_TextLen	= 512;
const int c_Help_ListLine	= 11;

//------------------------------------------------------------------------------
WzID uiHelpMonsterInfo::m_wzId[DIALOG_MAX] = 
{
	StrToWzID("B999"),	//DLGPOS_BTN_EXIT,
	StrToWzID("B001"),	//DLGPOS_BTN_OK,
	StrToWzID("T001"), 	//DLGPOS_TXT_TITLE,
	StrToWzID("LS00"), 	//DLGPOS_LST_DESC,
	StrToWzID("S001"), 	//DLGPOS_SCR_VERTICAL,
};
//------------------------------------------------------------------------------
/**	 ������
*/
uiHelpMonsterInfo::uiHelpMonsterInfo(InterfaceManager *pUIMan):
uiBase(pUIMan),
m_dwOldSelectId(0),
m_pTitleControl(NULL),
m_pListInfo(NULL),
m_pScrollControl(NULL),
m_iCurScrollPos(0),
m_bTargeting(FALSE),
m_eState(ST_INIT_NONE)
{
	//��ư�ʱ�ȭ
	memset ((CCtrlButtonWZ *)this->m_pBtnControls, NULL, sizeof (this->m_pBtnControls));
}

uiHelpMonsterInfo::~uiHelpMonsterInfo(void)
{
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMonsterInfo::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	this->InitControls();
}
//------------------------------------------------------------------------------
/**	 ��Ʈ�ѷ� �ʱ�ȭ
*/
void uiHelpMonsterInfo::InitControls()
{
	this->m_pScrollControl = static_cast<CCtrlVScrollWZ *>(this->getControl(DLGPOS_SCR_VERTICAL));
	assert (this->m_pScrollControl);

	for (int i=0; i<POS_BTN_SIZE; ++i)
	{
		this->m_pBtnControls[i] = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_EXIT + i));
		assert (this->m_pBtnControls[i]);

		this->m_pBtnControls[i]->ShowWindowWZ(WZ_SHOW);
	}
	
	//1.ĸ�Ǹ��� �ٲ۴�.
	if(m_pBtnControls[1])
		GlobalFunc::SetCaption(m_pBtnControls[1], _T("����"));

	this->m_pTitleControl = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TXT_TITLE ));
	assert (this->m_pTitleControl);
		
	CCtrlStaticWZ *pSubTitle =  static_cast<CCtrlStaticWZ *>(GetControlWZ( StrToWzID("T002")));
	
	//2.����Ÿ��Ʋ�� �����ش�.
	if(pSubTitle)
		pSubTitle->ShowWindowWZ(WZ_HIDE);

	m_pListInfo = static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_LST_DESC));
	assert (m_pListInfo);
	
	//3.����Ʈ�� �⺻����
	if(m_pListInfo)
	{
		m_pListInfo->SetUseReport(FALSE);
		m_pListInfo->SetSellHeight(22);
	}
}
//------------------------------------------------------------------------------
/** 
*/
void uiHelpMonsterInfo::InsertSkillList(WORD wSkillCode,BOOL bSpecial)
{

	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)wSkillCode);

	if(!pInfo)	return;
	
	m_vSkillCode.push_back(wSkillCode);
	
	TCHAR szString[c_Help_TextLen] = {0,};

	if(bSpecial)
		Snprintf(szString, c_Help_TextLen-1, _T("Ư��(%d):%s"),pInfo->m_SkillClassCode,pInfo->m_szSkillName);
	else
		Snprintf(szString, c_Help_TextLen-1, _T("��ų(%d):%s"),pInfo->m_SkillClassCode,pInfo->m_szSkillName);

	//����Ʈ�� �����Ѵ�.
	if(_tcslen(szString) != 0)
	{
		int index = m_pListInfo->InsertItemBack();
		
		COLORREF BaseColor = RGBA(120, 120, 120, 255);

		if(bSpecial)
			BaseColor = RGBA(255, 255, 0, 255);

		GlobalFunc::SetItem(m_pListInfo,index,0,szString,StrToWzID("mn12"), 
			DT_LEFT | DT_VCENTER,
			RGBA(0,0,0,0),
			BaseColor, 
			RGBA (0, 255, 0, 255));
	}

}
//------------------------------------------------------------------------------
/** ���ͽ�ų������ �����Ѵ�.
*/
void uiHelpMonsterInfo::SetMonsterInfo()
{
	Monster *pMon = (Monster *)g_ObjectManager.GetObject(m_dwOldSelectId);

	if(!pMon) return;
	
	//STEP1:��� ���� ������ �̸����
	TCHAR szString[c_Help_TextLen] = {0,};
	GlobalFunc::SetCaption(m_pTitleControl, pMon->GetName());
	
	//STEP2:��Ȱ,��,��ȯ��ų
	m_pListInfo->DeleteItemAll();
	m_vSkillCode.clear();

	WORD wSpecialSkill[3] = {pMon->GetMonsterInfo()->m_wReviveCode,pMon->GetMonsterInfo()->m_wHealCode,pMon->GetMonsterInfo()->m_wSummonCode};
		
	int index = 0;
	//Ư����ų����(3��)
	for(int i=0;i<3;++i)
	{
		InsertSkillList(wSpecialSkill[i],TRUE);
	}

	//�Ϲݽ�ų���Ժκ�(9��)
	for(int i=0;i<9;++i)
	{
		InsertSkillList(pMon->GetMonsterInfo()->m_wSkillCode[i],FALSE);
	}
	
	//��ų������ ��������
	if(m_vSkillCode.empty())
	{
		index = m_pListInfo->InsertItemBack();

		GlobalFunc::SetItem(m_pListInfo,index,0,_T("��ų���¸���"),StrToWzID("mn12"), 
			DT_LEFT | DT_VCENTER,
			RGBA(0,0,0,0),
			RGBA(255, 0, 0, 255), 
			RGBA (255, 255, 255, 255));
	}

	m_iCurScrollPos = 0;
	int iPage=(m_vSkillCode.size()-c_Help_ListLine);
	
	if(iPage<=0) iPage=0; 
	
	m_pScrollControl->SetScrollRangeWZ(0,iPage);
	m_pScrollControl->SetScrollPosWZ(m_iCurScrollPos);
	m_pListInfo->SetBeginDrawIndex(m_iCurScrollPos);
	m_pListInfo->SetCurSel(-1);
}
//------------------------------------------------------------------------------
/**
*/
void uiHelpMonsterInfo::OnUpdateSolarDialog()
{
	if(!m_pListInfo) 
        return; 

	DWORD dwTargetKey =  g_HeroInput.GetCurrentTarget();
	Character *pChr = (Character *)g_ObjectManager.GetObject(dwTargetKey);
	m_bTargeting	=FALSE;

	//Ÿ����������
	if(pChr)	
	{
		if(pChr->IsKindOfObject(MONSTER_OBJECT))
		{	
			//Ÿ���ùٲ������:���°� �ٲ�������� ����
			m_eState =  (m_dwOldSelectId==dwTargetKey) ? (ST_NOT_CHANGED):(ST_TARGET_CHANGE);			
			m_bTargeting = TRUE;
		}
		else
		{
			//���;ƴ�:���°� �ٲ�������� ����
			m_eState =  (m_eState==ST_NOT_MONSTER) ? (ST_NOT_CHANGED):(ST_NOT_MONSTER);
		}
	}
	else
	{
		//Ÿ�Ͼ���:���°� �ٲ�������� ����
		m_eState =  (m_eState==ST_TARGET_NONE || m_eState==ST_INIT_NONE ) ? (ST_NOT_CHANGED):(ST_TARGET_NONE);
	}	
	
	//Ÿ�����������
	if(m_bTargeting)
	{	
		Monster *pMon = (Monster *)pChr;
		SetMoveCaption(!pMon->GetSingMoveState());
	}
	else	
	{
		GlobalFunc::SetCaption(m_pBtnControls[1], _T("����"));
	}

	//�ٲ����ʾҴٸ� ����!
	if(m_eState==ST_NOT_CHANGED) return;
	
	//����Ʈ����
	m_dwOldSelectId =0;
	
	//������Ʈ�� ���� ������Ʈ�Ѵ�.
	switch(m_eState)
	{
	case ST_TARGET_NONE:		//Ÿ���þ���
	{
		GlobalFunc::SetCaption(m_pTitleControl, _T("Ÿ���þ���"));
		m_pListInfo->ShowWindowWZ(WZ_HIDE);
	}
	break;
	case ST_NOT_MONSTER:		//���;ƴ�
	{
		GlobalFunc::SetCaption(m_pTitleControl, _T("���;ƴ�"));
		m_pListInfo->ShowWindowWZ(WZ_HIDE);
	}
	break;

	case ST_TARGET_CHANGE:		//Ÿ�Ϲٲ�
	{
		m_dwOldSelectId = dwTargetKey;
		SetMonsterInfo();
		m_pListInfo->ShowWindowWZ(WZ_SHOW);
	}
	break;
	}
		
}
//------------------------------------------------------------------------------
/**	 �̵�ĸ�Ǽ���
*/
void uiHelpMonsterInfo::SetMoveCaption(BOOL bMove)
{
	if(!m_pBtnControls[1]) return; 

	if(bMove)
		GlobalFunc::SetCaption(m_pBtnControls[1], _T("�̵�"));
	else
		GlobalFunc::SetCaption(m_pBtnControls[1], _T("����"));
}
//------------------------------------------------------------------------------
/**	�̵��ϱ�
*/
void	uiHelpMonsterInfo::ActionMove()
{
	if(!m_bTargeting || !m_pListInfo) return;

	Monster *pMon=(Monster *)g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());

	if(!pMon) return;

	BOOL bSingleMove = pMon->GetSingMoveState();
	pMon->SetSingMoveState(!bSingleMove);

	SetMoveCaption(bSingleMove);

	if(bSingleMove)
	{
		pMon->StopMove();
	}
}
//------------------------------------------------------------------------------
/**	��ų�ߵ�
*/
void uiHelpMonsterInfo::ActionSkill()
{
	if(!m_pListInfo || m_pListInfo->GetCurSel()==-1 ) return;

	Monster *pMon=(Monster *)g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());

	if(!pMon) return; 

	PLAYER_ACTION action;
	ZeroMemory(&action,sizeof(action));

	action.SKILL.dwSkillID =  m_vSkillCode[m_pListInfo->GetCurSel()];
	action.ActionID = ACTION_SKILL;				
	action.SKILL.dwTargetID = g_pHero->GetObjectKey();
	action.SKILL.dwAttackSerial = 0;
	action.SKILL.vCurPos.wvPos =pMon->GetPosition();					
	action.SKILL.vCurPos.wTile = -1;
	action.SKILL.vDestPos.wvPos =pMon->GetPosition();
	action.SKILL.vDestPos.wTile = -1;
	action.SKILL.vTargePos=g_pHero->GetVisiblePos();
	
	if (GENERALPARAM->IsServerDebug()) 
	{
		pMon->DoAction(&action);
		pMon->CommitChangeState(g_CurTime);

	}
	else
	{
		pMon->PutAction(&action);
	}

}
//------------------------------------------------------------------------------
/**	���ν���
*/
void uiHelpMonsterInfo::MessageProc( SI_MESSAGE * pMessage )
{

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONDCLICK:
		{
			if( getCtrlIDToPos( pMessage->dwCtrlID )==DLGPOS_LST_DESC )
			{
				ActionSkill();
			}
		}
		break;

	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_BTN_EXIT:ShowInterface(FALSE);break;	 
			case DLGPOS_BTN_OK:ActionMove();break;
			}
		}
		break;
	case RT_MSG_SB_PAGETOP:
	case RT_MSG_SB_LINETOP:
		{
			//.���� ȭ��ǥ�� ������.
			m_iCurScrollPos = this->m_pScrollControl->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
			int mmin = 0 , mmax = 0;
			this->m_pScrollControl->GetScrollRangeWZ( &mmin, &mmax );  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

			if(m_iCurScrollPos>mmin)    // ���� �Էµ� ���μ� ���� ������ 
			{
				m_iCurScrollPos--;      // ���� ���� ��Ű�� 
				this->m_pScrollControl->SetScrollPosWZ(m_iCurScrollPos);

				
			}
		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:
	case RT_MSG_SB_LINEBOTTOM:
		{
			//.�Ʒ��� ȭ��ǥ�� ������
			m_iCurScrollPos = this->m_pScrollControl->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
			int mmin = 0 , mmax = 0;
			this->m_pScrollControl->GetScrollRangeWZ( &mmin, &mmax );  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

			if(m_iCurScrollPos<mmax)    // ���� �Էµ� ���μ� ���� ������ 
			{
				m_iCurScrollPos++;      // ���� ���� ��Ű�� 
				this->m_pScrollControl->SetScrollPosWZ(m_iCurScrollPos);

			
			}
		}
		break;

	case RT_MSG_SB_THUMBTRACK:
	case RT_MSG_SB_THUMBPOSITION:
	case RT_MSG_SB_ENDSCROLL:
		{
			m_iCurScrollPos = this->m_pScrollControl->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
			m_pScrollControl->SetScrollPosWZ(m_iCurScrollPos);
		}
		break;
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}