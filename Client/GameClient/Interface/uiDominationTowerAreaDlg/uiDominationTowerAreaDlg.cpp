#include "SunClientPrecompiledHeader.h"
#include "uiDominationTowerAreaDlg.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "DominationInfoParser.h"
#include "Map.h"


//------------------------------------------------------------------------------
 WzID uiDominationTowerAreaDlg::m_wzId[DIALOG_MAX] = 
 {
	StrToWzID("ST00"),
	StrToWzID("PI01"),
 };

//------------------------------------------------------------------------------
/**
*/
uiDominationTowerAreaDlg::uiDominationTowerAreaDlg(InterfaceManager *pUIMan)
:uiBase(pUIMan)
{
	m_pGuildNameText = NULL;
	m_pGuildMarkPic = NULL;
}

//------------------------------------------------------------------------------
/**
*/
uiDominationTowerAreaDlg::~uiDominationTowerAreaDlg()
{

}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerAreaDlg::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	m_pGuildNameText = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_GUILDNAME_TEXT]));
	assert(m_pGuildNameText);
	m_pGuildMarkPic = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_GUILDMARK_PIC]));
	assert(m_pGuildMarkPic);
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerAreaDlg::SetGuildName(TCHAR* pGuildName)
{
	if (pGuildName)
	{
		TCHAR szGuildName[INTERFACE_STRING_LENGTH] = {0, };
		Snprintf(szGuildName, INTERFACE_STRING_LENGTH, g_InterfaceManager.GetInterfaceString(3275), pGuildName);
		m_pGuildNameText->SetTextWZ(szGuildName);
	}
	else
	{
		m_pGuildNameText->SetTextWZ(" ");
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerAreaDlg::OnShowWindow(BOOL val)
{
	if (val)
	{
		RECT rc;
		//MoveWindowPos();
        //MoveDefaultWindowPos();
		GetDialogWZ()->GetClientRect(&rc);
		DisableInterSectRect(rc);
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerAreaDlg::OnRenderSolarDialog()
{
	// 길드마크 출력
	if (IsVisibleWindow())
	{
		DOMINATION_GUILD_INFO* pGuildInfo = ((uiDominationTowerMan*)GetMan())->GetDominationGuildInfo();
		uiDominationTowerMan::RenderGuildMark(m_pGuildMarkPic, pGuildInfo, m_bAlpha);
	}	
}

//------------------------------------------------------------------------------
/**
*/
bool uiDominationTowerAreaDlg::CheckShowDialog()
{
	DOMINATION_GUILD_INFO* pGuildInfo = ((uiDominationTowerMan*)GetMan())->GetDominationGuildInfo();
	MAPCODE FieldMapCode = g_pMap->GetCurrentFieldID();
	const sDOMINATION_INFO* pDI = DominationInfoParser::Instance()->GetDominationInfo(FieldMapCode);
	if ( false == (pGuildInfo && pGuildInfo->m_GuildGuid && pDI))
	{
		return false;
	}
	return true;
}
//------------------------------------------------------------------------------
/**
*/
