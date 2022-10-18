//------------------------------------------------------------------------------
//  uiQuestListContent_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestListContent.h"
#include "ItemUnitRenderer.h"

//------------------------------------------------------------------------------
/**
*/
uiQuestListContent::uiQuestListContent(InterfaceManager *pUIMan) :
    uiBase(pUIMan), m_bInitControls(FALSE)
{
}

//------------------------------------------------------------------------------
/**
*/
uiQuestListContent::~uiQuestListContent()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestListContent::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
	
	//MoveWindowWZ(0, 0);

    InitControls();

	m_FTextParser.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);  //ARGB

	RECT rc = {0, };
	m_FTextRender.Init(g_pSunRenderer, rc);

	//MoveWindowWZ(0,0);
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestListContent::InitControls()
{
	this->m_bInitControls = true;

	for (int i = 0; i < QUEST_CATEGORY_SIZE; ++i)
	{
		m_pCtrlCheckCategory[i] = (CCtrlButtonCheckWZ *)getControl(QUEST_CATEGORY_BEGIN+i);
		assert(m_pCtrlCheckCategory[i]);
		if (!m_pCtrlCheckCategory[i]) 
		{
			m_bInitControls = FALSE;
			return;
		}		
	}

	m_bInitControls = TRUE;

	for ( int i = 0; i < QUEST_CHECK_LIST_TYPE_SIZE; ++i)
	{
		m_pCtrlCheckQuestType[i] = (CCtrlButtonCheckWZ *)getControl(QUEST_CHECK_LIST_TYPE_BEGIN+i);
		assert(m_pCtrlCheckQuestType[i]);
		if (!m_pCtrlCheckQuestType[i]) 
		{
			m_bInitControls = FALSE;
			return;
		}			
	}

	m_bInitControls = TRUE;    

	m_bInitControls = (BOOL)GetCtrlProgressQuestList();
	assert(m_bInitControls);
	if (!m_bInitControls)	
		return;

	m_bInitControls = (BOOL)GetCtrlQuestContentList();
	assert(m_bInitControls);
	if (!m_bInitControls)	
		return;

	m_bInitControls = (BOOL)GetCtrlProgressQuestVScroll();
	assert(m_bInitControls);
	if (!m_bInitControls)	
		return;

	m_bInitControls = (BOOL)GetCtrlQuestContentVScroll();
	assert(m_bInitControls);
	if (!m_bInitControls)	
		return;

	m_bInitControls = (BOOL)GetCtrlQuestCount();
	assert(m_bInitControls);
	if (!m_bInitControls)	
		return;

	m_bInitControls = (BOOL)GetCtrlQuestCancelBtn();
	assert(m_bInitControls);
	if (!m_bInitControls)	
		return;

	ShowQuestCancelBtn(FALSE);


	m_bInitControls = (BOOL)GetCtrlQuestPreviewChk();
	assert(m_bInitControls);
	if (!m_bInitControls)	
		return;

	
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestListContent::Release()
{
	this->ClearInfo();

}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
