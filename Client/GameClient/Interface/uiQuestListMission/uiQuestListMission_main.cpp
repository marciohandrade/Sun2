//------------------------------------------------------------------------------
//  uiQuestListMission_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestListMission.h"
#include "ItemUnitRenderer.h"

//------------------------------------------------------------------------------
/**
*/
uiQuestListMission::uiQuestListMission(InterfaceManager *pUIMan) :
    uiBase(pUIMan), m_pItemUnitRenderer(NULL), m_bInitControls(FALSE)
{
	SCItemSlotContainer::Init(MAX_SLOT, SI_QUEST_LIST_CONTENT);

	m_pCurQuest = NULL;
	m_btDlgState = 0;
    reward_point_button_ = NULL;
}

//------------------------------------------------------------------------------
/**
*/
uiQuestListMission::~uiQuestListMission()
{
    this->Release();

	SCItemSlotContainer::Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestListMission::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	//MoveWindowWZ(0, 0);

    InitControls();

	// Init ItemRenderer
	assert( !m_pItemUnitRenderer );

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( g_pSunRenderer, SCSlotContainer::GetMaxSlotNum() );

	m_FTextParser.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);  //ARGB

	RECT rc = {0, };
	m_FTextRender.Init(g_pSunRenderer, rc);

	//MoveWindowWZ(0,0);
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestListMission::InitControls()
{
#ifdef _SCJ_LONG_TEXT_REDUCING
	CCtrlStaticWZ* title_ctrl = (CCtrlStaticWZ*)getControl(eDLGPOS_TITLE);
	title_ctrl->SetTextReducing(true);
#endif //_SCJ_LONG_TEXT_REDUCING

	this->m_bInitControls = true; 

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

	m_bInitControls = (BOOL)GetCtrlQuestMissionList();
	assert(m_bInitControls);
	if (!m_bInitControls)	
		return;

	m_bInitControls = (BOOL)GetCtrlQuestMissionVScroll();
	assert(m_bInitControls);
	if (!m_bInitControls)	
		return;

	m_bInitControls = (BOOL)GetCtrlQuestCancelBtn();
	assert(m_bInitControls);
	if (!m_bInitControls)	
		return;

	ShowQuestCancelBtn(FALSE);

	m_bInitControls = (BOOL)GetCtrlRewardExp();
	assert(m_bInitControls);
	if (!m_bInitControls)	
		return;		


	m_bInitControls = BOOL( GetCtrlRewardChao() );
	assert(m_bInitControls);
	if ( !m_bInitControls )
		return;

    reward_point_button_ = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[eDLGPOS_BTN_REWARD_POINT]));
    SUN_ASSERT(reward_point_button_);

    #ifdef _RU_110209_ACCUMULATE_POINT_BLOCK
    if (reward_point_button_ != NULL)
    {
        reward_point_button_->ShowWindowWZ(WZ_HIDE);
    }
    CControlWZ* control_ptr = GetControlWZ(StrToWzID("P000"));
    if (control_ptr != NULL)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
    #endif //_RU_110209_ACCUMULATE_POINT_BLOCK

}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestListMission::Release()
{
	this->ClearInfo();

	if ( m_pItemUnitRenderer )
	{
		this->m_pItemUnitRenderer->Release();
		SAFE_DELETE(this->m_pItemUnitRenderer);
	}
}



//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
