//------------------------------------------------------------------------------
//  uiQuestTimeInfo_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestTimeInfo.h"
#include "ItemUnitRenderer.h"

//------------------------------------------------------------------------------
/**
*/
uiQuestTimeInfo::uiQuestTimeInfo(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
uiQuestTimeInfo::~uiQuestTimeInfo()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestTimeInfo::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	//MoveWindowWZ(0, 0);

    InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestTimeInfo::InitControls()
{
	m_bInitControls = true; 

	m_bInitControls = (BOOL)GetCtrlQuestName();
	assert(m_bInitControls);
	if (!m_bInitControls)
		return;

	m_bInitControls = (BOOL)GetCtrlQuestTimeInfo();
	assert(m_bInitControls);
	if (!m_bInitControls)
		return;
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestTimeInfo::Release()
{
	this->ClearInfo();
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
