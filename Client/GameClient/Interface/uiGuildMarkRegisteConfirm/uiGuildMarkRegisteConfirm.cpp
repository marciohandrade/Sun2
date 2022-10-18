#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMarkRegisteConfirm.h"
#include "uiGuildMarkCombination/uiGuildMarkCombination.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "GuildMarkInfoParser.h"
#include "uiGuildMan/uiGuildMan.h"

WzID uiGuildMarkRegisteConfirm::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("BT01"),	//DLG_BTN_OK,
	StrToWzID("BT02"),//DLG_BTN_CANCLE,
	StrToWzID("BT00"),//DLG_BTN_EXIT,
	StrToWzID("BT03"),//DLG_BTN_TARGET_MARK,	
};
//------------------------------------------------------------------------------
/**
*/
uiGuildMarkRegisteConfirm::uiGuildMarkRegisteConfirm(InterfaceManager * pUIMan) : uiBase(pUIMan)
{

}
//------------------------------------------------------------------------------
/**
*/
uiGuildMarkRegisteConfirm::~uiGuildMarkRegisteConfirm()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkRegisteConfirm::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
	m_pMarkPictureButton = ( CCtrlButtonWZ *) getControl(DLG_BTN_TARGET_MARK);
	assert(m_pMarkPictureButton);

	m_pGuildMan = (uiGuildMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
	assert(m_pGuildMan);
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkRegisteConfirm::Release()
{

}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkRegisteConfirm::OnRenderSolarDialog()
{
	if( !m_pMarkPictureButton ||!GetManager())
		return;

	HANDLE hGuildMarkTexture = GetManager()->GetGuildMarkTexture();
	if( hGuildMarkTexture == INVALID_HANDLE_VALUE)
		return;

	RECT rtMark = m_pMarkPictureButton->GetSizeRect();

	float fMarkSize = (float)rtMark.right - rtMark.left;

	for( int i = 0; i < 2; ++i)
	{
		BASE_GuildMarkInfo * pInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(m_indexMark[i]);
		if( pInfo)
		{
			float fSrcX = (float)pInfo->iRow * 32.0f;
			float fSrcY = (float)pInfo->iColumn * 32.0f;
			g_pSunRenderer->RenderTexture(hGuildMarkTexture , (float)rtMark.left ,(float)rtMark.top , fMarkSize, fMarkSize , fSrcX, fSrcY, 32.0f,32.0f );
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkRegisteConfirm::Process(DWORD dwTick)
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkRegisteConfirm::OnShowWindow(BOOL val)
{
	if( val )
	{
		MoveCenter();
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkRegisteConfirm::MessageProc(SI_MESSAGE * pMessage)
{
	switch( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getControlIDToPos(pMessage->dwCtrlID))
			{
			case DLG_BTN_OK:
				{
					uiGuildMarkCombination * pMarkCombination = GET_CAST_DIALOG( uiGuildMarkCombination, IM_GUILD_MANAGER::GUILD_MARK_COMBINATION );

					if( pMarkCombination )
					{
						pMarkCombination->NET_SEND_CG_GUILD_GUILDMARK_REGISTER_SYN();
						ShowInterface(FALSE);
					}
				}
				break;
			case DLG_BTN_CANCLE:
			case DLG_BTN_EXIT:
				{
					ShowInterface(FALSE);
				}
				break;
			}
		}
		break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMarkRegisteConfirm::NetworkProc(MSG_BASE * pMsg)
{

}

void uiGuildMarkRegisteConfirm::SetMarkIndex(int iBackGround, int iPattern)
{
	m_indexMark[0] = iBackGround;
	m_indexMark[1] = iPattern;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
