//------------------------------------------------------------------------------
//  uiQuestCommonForward_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"



#include "uiQuestCommonForward.h"
#include "ItemUnitRenderer.h"

//------------------------------------------------------------------------------
/**
*/
uiQuestCommonForward::uiQuestCommonForward(InterfaceManager *pUIMan) :
    uiBase(pUIMan),  
	m_bInitControls(FALSE)
{
}

//------------------------------------------------------------------------------
/**
*/
uiQuestCommonForward::~uiQuestCommonForward()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestCommonForward::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	//MoveWindowWZ(0, 0);

    InitControls();

	m_FTextParser.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);

	RECT rc = {0, };
	m_FTextRender.Init(g_pSunRenderer, rc);

	//MoveWindowWZ(0,0);
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestCommonForward::InitControls()
{
	this->m_bInitControls = true;

	assert(GetCtrlTitle());
	m_bInitControls = BOOL( GetCtrlTitle() );
	if ( !m_bInitControls )
		return;
	
	assert( GetCtrlConversation() );
	m_bInitControls = BOOL( GetCtrlTitle() );
	if ( !m_bInitControls )
		return;

	m_pBTN_Job = (CCtrlButtonWZ*)this->getControl(eDLG_BTN_JOB);
	assert(m_pBTN_Job);
	m_bInitControls = BOOL(m_pBTN_Job);
	if (m_pBTN_Job == NULL) 
	{
		return;
	}

	m_pBTN_Tab = (CCtrlButtonWZ*)this->getControl(eDLG_BTN_TAB);
	assert(m_pBTN_Tab);
	m_bInitControls = BOOL(m_pBTN_Tab);
	if (m_pBTN_Tab == NULL) 
	{
		return;
	}

	m_bInitControls = BOOL( this->GetCtrlQuestList() );
	if ( !m_bInitControls )
		return;

#ifdef _GS_ADD_QUEST_SCROLL_1143
	m_pScroll		= (CCtrlVScrollWZ*)this->getControl(eDLG_SCR_QUEST);
	assert(m_pScroll);
	if ( !m_pScroll) 
		return;
#endif//_GS_ADD_QUEST_SCROLL_1143(중국스크롤바추가)

	GetCtrlQuestList()->SetUseReport(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestCommonForward::Release()
{
	this->ClearInfo();

}



//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
