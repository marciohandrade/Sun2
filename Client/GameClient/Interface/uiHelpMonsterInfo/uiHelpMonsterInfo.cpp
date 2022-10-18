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
/**	 생성자
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
	//버튼초기화
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
/**	 컨트롤러 초기화
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
	
	//1.캡션명을 바꾼다.
	if(m_pBtnControls[1])
		GlobalFunc::SetCaption(m_pBtnControls[1], _T("없음"));

	this->m_pTitleControl = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TXT_TITLE ));
	assert (this->m_pTitleControl);
		
	CCtrlStaticWZ *pSubTitle =  static_cast<CCtrlStaticWZ *>(GetControlWZ( StrToWzID("T002")));
	
	//2.서브타이틀은 가려준다.
	if(pSubTitle)
		pSubTitle->ShowWindowWZ(WZ_HIDE);

	m_pListInfo = static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_LST_DESC));
	assert (m_pListInfo);
	
	//3.리스트를 기본셋팅
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
		Snprintf(szString, c_Help_TextLen-1, _T("특수(%d):%s"),pInfo->m_SkillClassCode,pInfo->m_szSkillName);
	else
		Snprintf(szString, c_Help_TextLen-1, _T("스킬(%d):%s"),pInfo->m_SkillClassCode,pInfo->m_szSkillName);

	//리스트에 삽입한다.
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
/** 몬스터스킬정보를 셋팅한다.
*/
void uiHelpMonsterInfo::SetMonsterInfo()
{
	Monster *pMon = (Monster *)g_ObjectManager.GetObject(m_dwOldSelectId);

	if(!pMon) return;
	
	//STEP1:상단 몬스터 레벨및 이름출력
	TCHAR szString[c_Help_TextLen] = {0,};
	GlobalFunc::SetCaption(m_pTitleControl, pMon->GetName());
	
	//STEP2:부활,힐,소환스킬
	m_pListInfo->DeleteItemAll();
	m_vSkillCode.clear();

	WORD wSpecialSkill[3] = {pMon->GetMonsterInfo()->m_wReviveCode,pMon->GetMonsterInfo()->m_wHealCode,pMon->GetMonsterInfo()->m_wSummonCode};
		
	int index = 0;
	//특수스킬삽입(3개)
	for(int i=0;i<3;++i)
	{
		InsertSkillList(wSpecialSkill[i],TRUE);
	}

	//일반스킬삽입부분(9개)
	for(int i=0;i<9;++i)
	{
		InsertSkillList(pMon->GetMonsterInfo()->m_wSkillCode[i],FALSE);
	}
	
	//스킬정보가 전혀없다
	if(m_vSkillCode.empty())
	{
		index = m_pListInfo->InsertItemBack();

		GlobalFunc::SetItem(m_pListInfo,index,0,_T("스킬없는몬스터"),StrToWzID("mn12"), 
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

	//타켓존재유무
	if(pChr)	
	{
		if(pChr->IsKindOfObject(MONSTER_OBJECT))
		{	
			//타켓팅바뀌었는지:상태가 바뀌었을때만 갱신
			m_eState =  (m_dwOldSelectId==dwTargetKey) ? (ST_NOT_CHANGED):(ST_TARGET_CHANGE);			
			m_bTargeting = TRUE;
		}
		else
		{
			//몬스터아님:상태가 바뀌었을때만 갱신
			m_eState =  (m_eState==ST_NOT_MONSTER) ? (ST_NOT_CHANGED):(ST_NOT_MONSTER);
		}
	}
	else
	{
		//타켓없음:상태가 바뀌었을때만 갱신
		m_eState =  (m_eState==ST_TARGET_NONE || m_eState==ST_INIT_NONE ) ? (ST_NOT_CHANGED):(ST_TARGET_NONE);
	}	
	
	//타켓팅있을경우
	if(m_bTargeting)
	{	
		Monster *pMon = (Monster *)pChr;
		SetMoveCaption(!pMon->GetSingMoveState());
	}
	else	
	{
		GlobalFunc::SetCaption(m_pBtnControls[1], _T("없음"));
	}

	//바뀌지않았다면 리턴!
	if(m_eState==ST_NOT_CHANGED) return;
	
	//디폴트셋팅
	m_dwOldSelectId =0;
	
	//스테이트에 따라 업데이트한다.
	switch(m_eState)
	{
	case ST_TARGET_NONE:		//타켓팅없음
	{
		GlobalFunc::SetCaption(m_pTitleControl, _T("타켓팅없음"));
		m_pListInfo->ShowWindowWZ(WZ_HIDE);
	}
	break;
	case ST_NOT_MONSTER:		//몬스터아님
	{
		GlobalFunc::SetCaption(m_pTitleControl, _T("몬스터아님"));
		m_pListInfo->ShowWindowWZ(WZ_HIDE);
	}
	break;

	case ST_TARGET_CHANGE:		//타켓바뀜
	{
		m_dwOldSelectId = dwTargetKey;
		SetMonsterInfo();
		m_pListInfo->ShowWindowWZ(WZ_SHOW);
	}
	break;
	}
		
}
//------------------------------------------------------------------------------
/**	 이동캡션셋팅
*/
void uiHelpMonsterInfo::SetMoveCaption(BOOL bMove)
{
	if(!m_pBtnControls[1]) return; 

	if(bMove)
		GlobalFunc::SetCaption(m_pBtnControls[1], _T("이동"));
	else
		GlobalFunc::SetCaption(m_pBtnControls[1], _T("정지"));
}
//------------------------------------------------------------------------------
/**	이동하기
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
/**	스킬발동
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
/**	프로시져
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
			//.윗쪽 화살표를 눌렀다.
			m_iCurScrollPos = this->m_pScrollControl->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
			int mmin = 0 , mmax = 0;
			this->m_pScrollControl->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

			if(m_iCurScrollPos>mmin)    // 현재 입력된 라인수 보다 작으면 
			{
				m_iCurScrollPos--;      // 한줄 감소 시키고 
				this->m_pScrollControl->SetScrollPosWZ(m_iCurScrollPos);

				
			}
		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:
	case RT_MSG_SB_LINEBOTTOM:
		{
			//.아랫쪽 화살표를 눌렀다
			m_iCurScrollPos = this->m_pScrollControl->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
			int mmin = 0 , mmax = 0;
			this->m_pScrollControl->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

			if(m_iCurScrollPos<mmax)    // 현재 입력된 라인수 보다 작으면 
			{
				m_iCurScrollPos++;      // 한줄 증가 시키고 
				this->m_pScrollControl->SetScrollPosWZ(m_iCurScrollPos);

			
			}
		}
		break;

	case RT_MSG_SB_THUMBTRACK:
	case RT_MSG_SB_THUMBPOSITION:
	case RT_MSG_SB_ENDSCROLL:
		{
			m_iCurScrollPos = this->m_pScrollControl->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
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