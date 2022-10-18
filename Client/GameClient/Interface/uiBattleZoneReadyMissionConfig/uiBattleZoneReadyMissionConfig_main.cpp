//------------------------------------------------------------------------------
//  uiBattleZoneReadyMissionConfig_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)

#include "uiBattleZoneReadyMissionConfig.h"

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneReadyMissionConfig::uiBattleZoneReadyMissionConfig(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneReadyMissionConfig::~uiBattleZoneReadyMissionConfig()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyMissionConfig::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyMissionConfig::InitControls()
{
    for (int i=0; i<POS_BTN_SIZE; ++i)
    {
        m_pBtnControls[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(POS_BTN_BEGIN + i));
        assert (m_pBtnControls[i]);
    }

    for (int i=0; i<POS_CHK_SIZE; ++i)
    {
        m_pChkControls[i] =
            static_cast<CCtrlButtonCheckWZ *>(this->getControl(POS_CHK_BEGIN + i));
        assert (m_pChkControls[i]);
    }

    this->m_bInitControls = true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyMissionConfig::Release()
{
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
#endif // !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
