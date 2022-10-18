//------------------------------------------------------------------------------
//  uiQuestMissionList_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestMissionList.h"
#include "ItemUnitRenderer.h"

//------------------------------------------------------------------------------
/**
*/
uiQuestMissionList::uiQuestMissionList(InterfaceManager *pUIMan) :
    uiBase(pUIMan), m_bInitControls(FALSE)
{
}

//------------------------------------------------------------------------------
/**
*/
uiQuestMissionList::~uiQuestMissionList()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestMissionList::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	//MoveWindowWZ(0, 0);

    InitControls();

	initMissionGroupMap();

	//MoveWindowWZ(0,0);
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestMissionList::InitControls()
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

	for ( int i = 0; i < MAX_MISSION_TYPE; ++i)
	{
		m_pCtrlStaticMissionInfo[i] = (CCtrlButtonCheckWZ *)getControl(BEGIN_MISSION_TYPE+i);

		assert(m_pCtrlStaticMissionInfo[i]);
		if ( !m_pCtrlStaticMissionInfo[i] ) 
		{
			m_bInitControls = FALSE;
			return;
		}			
	}

	for ( int i = 0; i < MAX_REWARD_COUNT; ++i)
	{
		m_pCtrlStaticRewardInfo[i] = (CCtrlStaticWZ *)getControl(BEGIN_REWARD+i);
		assert(m_pCtrlStaticRewardInfo[i]);
		if (!m_pCtrlStaticRewardInfo[i]) 
		{
			m_bInitControls = FALSE;
			return;
		}			
	}

	m_bInitControls = (BOOL)GetCtrlMissionList();
	assert(m_bInitControls);
	if (!m_bInitControls)	
		return;   

	m_bInitControls = (BOOL)GetCtrlCurSelMission();
	assert(m_bInitControls);
	if (!m_bInitControls)	
		return;   
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestMissionList::Release()
{
	this->ClearInfo();

	m_MissionMapGroupMap.clear();

}



//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
