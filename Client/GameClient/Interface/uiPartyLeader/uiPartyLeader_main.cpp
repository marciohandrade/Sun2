//------------------------------------------------------------------------------
//  uiPartyLeader_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiPartyLeader.h"
#include "uiPartyMan/uiPartyMan.h"

//------------------------------------------------------------------------------
/**
*/
uiPartyLeader::uiPartyLeader(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    MAX_SKILLEFFECT_AREA(10)
{
    this->m_PartyInfos = NULL;
    this->m_bLeader = false;
    this->m_ctrlBtnOption = NULL;
    this->m_fGaugeSize = 1.0f;
    ZeroMemory(m_arEffectImage,sizeof(m_arEffectImage));
}

//------------------------------------------------------------------------------
/**
*/
uiPartyLeader::~uiPartyLeader()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->m_ctrlBtnOption = (CCtrlButtonWZ *)getControl(DLGPOS_OPTION);
    this->m_ctrlBtnOption->ShowWindowWZ(WZ_HIDE);

    this->m_rcInit = getControl(DLGPOS_HPBAR)->GetSize();
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::Release()
{
    ClearSkillEffectInfo();
    SAFE_DELETE(m_PartyInfos);
}

//------------------------------------------------------------------------------
/**
*/
// ProgrammCommon/ResultCode.h ÂüÁ¶
void
uiPartyLeader::Reason(ePARTY_RESULT eReason, WzColor Color)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	g_InterfaceManager.GetInterfaceString( (eST_PARTY_RESULT_MESSAGE + eReason), szMessage, INTERFACE_STRING_LENGTH );
    GlobalFunc::OutputMsg_Notice((eOUTPUT_MSG_TYPE)(eOUTPUT_MSG_CHAT | eOUTPUT_MSG_TOP_SHOW), szMessage);
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::PrintInterfaceMessage(int msgIdx)
{
    TCHAR	szMessage[INTERFACE_STRING_LENGTH];
    g_InterfaceManager.GetInterfaceString( msgIdx, szMessage, INTERFACE_STRING_LENGTH );
    WzColor color = RGBA(155,155,255,255);
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::PrintInterfaceMessage(TCHAR *pstr)
{
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, pstr);
}

//------------------------------------------------------------------------------
/**
*/
DWORD
uiPartyLeader::GetObjectKey()
{
	return m_dwObjectKey;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------

