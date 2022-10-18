//------------------------------------------------------------------------------
//  uiQuestAutoAcceptAlram_main.cpp
//  (C) 2005 y2jinc
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestAutoAcceptAlram.h"
#include "ItemUnitRenderer.h"

//------------------------------------------------------------------------------
/**
*/
uiQuestAutoAcceptAlram::uiQuestAutoAcceptAlram(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
uiQuestAutoAcceptAlram::~uiQuestAutoAcceptAlram()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestAutoAcceptAlram::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	//MoveWindowWZ(0, 0);

    InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestAutoAcceptAlram::InitControls()
{
	m_bInitControls = true; 
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestAutoAcceptAlram::Release()
{
	this->ClearInfo();
}



//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
