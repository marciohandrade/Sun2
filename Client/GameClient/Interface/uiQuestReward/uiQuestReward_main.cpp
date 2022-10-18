//------------------------------------------------------------------------------
//  uiQuestReward_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestReward.h"
#include "ItemUnitRenderer.h"

//------------------------------------------------------------------------------
/**
*/
uiQuestReward::uiQuestReward(InterfaceManager *pUIMan) :
    uiBase(pUIMan), m_pItemUnitRenderer(NULL), m_bInitControls(FALSE)
{
	SCItemSlotContainer::Init(SLOT_SIZE, SI_QUEST_REWARD);
    reward_point_button_ = NULL;
}

//------------------------------------------------------------------------------
/**
*/
uiQuestReward::~uiQuestReward()
{
    this->Release();

	SCItemSlotContainer::Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestReward::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	memset(&m_VScrollInfo, 0, sizeof(m_VScrollInfo));

	//MoveWindowWZ(0, 0);

    InitControls();

	// Init ItemRenderer
	assert( !m_pItemUnitRenderer );

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( g_pSunRenderer, SCSlotContainer::GetMaxSlotNum() );

	m_FTextParser.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);

	RECT rc = {0, };
	m_FTextRender.Init(g_pSunRenderer, rc);
	
	//MoveWindowWZ(0,0);
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestReward::InitControls()
{
	this->m_bInitControls = true; 

	assert(GetCtrlTitle());
	m_bInitControls = BOOL( GetCtrlTitle() );
	if ( !m_bInitControls )
		return;

#ifdef _SCJ_LONG_TEXT_REDUCING
	CCtrlStaticWZ* title_ctrl = (CCtrlStaticWZ*)GetCtrlTitle();
	title_ctrl->SetTextReducing(true);
#endif //_SCJ_LONG_TEXT_REDUCING

	assert(GetCtrlQuestInfo());
	m_bInitControls = BOOL( GetCtrlQuestInfo() );
	if ( !m_bInitControls )
		return;

	assert(GetCtrlRewardHeim());
	m_bInitControls = BOOL( GetCtrlRewardHeim() );
	if ( !m_bInitControls )
		return;

	assert(GetCtrlRewardOK());
	m_bInitControls = BOOL( GetCtrlRewardOK() );
	if ( !m_bInitControls )
		return;

	assert(GetCtrlRewardCancel());
	m_bInitControls = BOOL( GetCtrlRewardCancel() );
	if ( !m_bInitControls )
		return;

	assert(GetCtrlQuestInfoVscr());
	m_bInitControls = BOOL( GetCtrlQuestInfoVscr() );
	if ( !m_bInitControls )
		return;

	assert(GetCtrlRewardExp());
	m_bInitControls = BOOL( GetCtrlRewardExp() );
	if ( !m_bInitControls )
		return;

	assert(GetCtrlRewardChao());
	m_bInitControls = BOOL( GetCtrlRewardChao() );
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
uiQuestReward::Release()
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
