#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "InterfaceManager.h"
#include "uiGuildAllianceRequest/uiGuildAllianceRequest.h"
#include "GlobalFunc.h"
#include "BattleScene.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"

const DWORD  c_dw_Delay_Time = 11000;
const DWORD c_Str_Interval = 11;

WzID uiGuildAllianceRequest::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("T999") ,  //eDIALOG_TXT_TITLE
	StrToWzID("TI00") ,  //eDIALOG_BTN_OK
	StrToWzID("TI01") ,  //eDIALOG_BTN_CANCLE
	StrToWzID("L000") ,  //eDIALOG_LIST_MAIN

};

uiGuildAllianceRequest::uiGuildAllianceRequest(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_szGuildName[0] = '0';
}

uiGuildAllianceRequest::~uiGuildAllianceRequest()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildAllianceRequest::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow = false;

	m_pTitleStatic = (CCtrlStaticWZ*) getControl(eDIALOG_TXT_TITLE);
	m_pTitleStatic->SetTextID(NULL);

	m_pList = ( CCtrlListWZ * ) getControl(eDIALOG_LIST_MAIN);

	m_pOKBtn = (CCtrlImageStaticWZ*) getControl(eDIALOG_BTN_OK);
	m_pOKBtn->SetTextID(NULL);

	m_pCancelBtn = (CCtrlImageStaticWZ*) getControl(eDIALOG_BTN_CANCLE);
	m_pCancelBtn->SetTextID(NULL);

	m_iDelayTime = c_dw_Delay_Time;//다이얼로그 노출 시간(11초).
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildAllianceRequest::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildAllianceRequest::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildAllianceRequest::OnRenderSolarDialog()
{
	if(g_pHero)
		AddFormatText();
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildAllianceRequest::Process(DWORD dwTick)
{
	if((DWORD)m_iDelayTime > dwTick && m_bShow)
	{
		m_iDelayTime -= dwTick;
		if( m_iDelayTime <= 1000 )
		{
			m_bShow = false;
			MSG_CG_GUILD_RELATION_ANSWER_SYN SendPacket;
			SendPacket.m_eAnswerType = eGUILD_ANSWER_NO;
			SendPacket.m_eRelationType = m_eGuildRelation;
			SendPacket.m_RequestUserKey = m_iCharGUID;
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildAllianceRequest::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_iDelayTime = c_dw_Delay_Time;

		MoveCenter();
		m_bShow = true;
	}
	else
	{
		m_bShow = false;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildAllianceRequest::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_BTN_OK:
				{
					m_bShow = false;

					MSG_CG_GUILD_RELATION_ANSWER_SYN SendPacket;
					SendPacket.m_eAnswerType = eGUILD_ANSWER_YES;
					SendPacket.m_eRelationType = m_eGuildRelation;
					SendPacket.m_RequestUserKey = m_iCharGUID;
					GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));		
				}
				break;

			case eDIALOG_BTN_CANCLE:
				{
					m_bShow = false;

					MSG_CG_GUILD_RELATION_ANSWER_SYN SendPacket;
					SendPacket.m_eAnswerType = eGUILD_ANSWER_NO;
					SendPacket.m_eRelationType = m_eGuildRelation;
					SendPacket.m_RequestUserKey = m_iCharGUID;
					GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
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
void uiGuildAllianceRequest::SetGuildName(TCHAR* szGuildName)
{
	if('0' != szGuildName[0])
		StrnCopy(m_szGuildName, szGuildName, MAX_GUILDNAME_LENGTH);
}

void uiGuildAllianceRequest::SetGuildRelationType(eGUILD_RELATION eGuildRelation)
{
	m_eGuildRelation = eGuildRelation;
}

void uiGuildAllianceRequest::SetCharGUID(CHARGUID iCharGUID)
{
	m_iCharGUID = iCharGUID;
}

void uiGuildAllianceRequest::AddFormatText()
{
	this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(_T("mn12"));

	RECT rt       = m_pList->GetSizeRect();
	RECT rtTitle  = m_pTitleStatic->GetSizeRect();
	RECT rtOk	  = m_pOKBtn->GetSizeRect();
	RECT rtCancel = m_pCancelBtn->GetSizeRect();
	rtOk.top += 8;
	rtCancel.top += 8;
	rt.top -= 6;

	TCHAR szResult[INTERFACE_STRING_LENGTH + 1]  = {0,};
	TCHAR szOK[INTERFACE_STRING_LENGTH + 1]		 = {0,};
	TCHAR szCancel[INTERFACE_STRING_LENGTH + 1]  = {0,};
	TCHAR szTitle[INTERFACE_STRING_LENGTH + 1]   = {0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szFinal[INTERFACE_STRING_LENGTH + 1]   = {0,};

	//1387	동맹신청 확인
	g_InterfaceManager.GetInterfaceString(1387, szTitle, INTERFACE_STRING_LENGTH);
	//1167 %s 길드로 부터 동맹 권유 메세지가 도착했습니다. 승낙하시겠습니까? - %d
	g_InterfaceManager.GetInterfaceString(1167, szMessage, INTERFACE_STRING_LENGTH);
	//21107 동맹 수락
	g_InterfaceManager.GetInterfaceString(21107, szOK);
	//9011 동맹 거절
	g_InterfaceManager.GetInterfaceString(9011, szCancel);

	if('0' == m_szGuildName[0])
	{
		assert(!"길드네임 이상하다");
	}

	if( g_pHero)
		Snprintf(szResult , INTERFACE_STRING_LENGTH -1 , szMessage , m_szGuildName, (int)(m_iDelayTime/1000));

	std::vector<util::_tstring> vstr;
	int cutLen = rt.right - rt.left - 14;
	GlobalFunc::ProcessWordWrapByFontSize( &vstr, szResult, StrToWzID(_T("mn12")), INTERFACE_STRING_LENGTH, cutLen );

	SIZE FontSize;
	this->GetDrawBase()->x_pManagerTextOut->GetTextExtentPoint(_T("1"), 1, &FontSize);
	int	 iExtraInfoHeight = 0;
	iExtraInfoHeight += FontSize.cy+8;

	if (szResult && szOK && szCancel) 
	{
		for (size_t i=0; i<vstr.size(); ++i)
		{
			RECT rcResult;
			SetRect(&rcResult,
				rt.left + 2,
				iExtraInfoHeight + rt.top + (iExtraInfoHeight*i),
				rt.right,
				iExtraInfoHeight + rt.top + (iExtraInfoHeight*i) + iExtraInfoHeight + 1);

			TCHAR *pStr = const_cast<TCHAR *>(vstr[i].c_str());

			if (pStr) 
			{
				this->GetDrawBase()->x_pManagerTextOut->DrawText(
					pStr,
					&rcResult,
					WzColor_RGBA(235,224,200,255 ),
					WzColor_RGBA(0,0,0,0),
					TP_HCENTER);
			}
		}

		this->GetDrawBase()->x_pManagerTextOut->DrawText(
			szTitle,
			&rtTitle,
			WzColor_RGBA(255, 133, 50, 255),
			WzColor_RGBA(0,0,0,0),
			TP_HCENTER);
		this->GetDrawBase()->x_pManagerTextOut->DrawText(
			szOK,
			&rtOk,
			WzColor_RGBA(255, 133, 50, 255),
			WzColor_RGBA(0,0,0,0),
			TP_HCENTER);
		this->GetDrawBase()->x_pManagerTextOut->DrawText(
			szCancel,
			&rtCancel,
			WzColor_RGBA(255, 133, 50, 255),
			WzColor_RGBA(0,0,0,0),
			TP_HCENTER);
	}

	this->GetDrawBase()->x_pManagerTextOut->EndUseFont();
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
