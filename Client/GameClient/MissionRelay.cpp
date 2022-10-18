#include "SunClientPrecompiledHeader.h"

#include "InterfaceManager.h"
#include "MissionRelay.h"
#include "GlobalFunc.h"
#include "BattleScene.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"

const DWORD c_Str_Interval = 11;

WzID MissionRelay::m_wzID[DLGPOS_MAX] = 
{
		StrToWzID("TI00") ,  //eDIALOG_BTN_OK
		StrToWzID("TI01") ,  //eDIALOG_BTN_CANCLE
		StrToWzID("ST00") ,  //eDIALOG_TXT_MAIN

};

MissionRelay::MissionRelay(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
}

MissionRelay::~MissionRelay()
{
}

//------------------------------------------------------------------------------
/**
*/
void MissionRelay::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow = false;

	m_pStatic = ( CCtrlStaticWZ * ) getControl(eDIALOG_TXT_MAIN);  
	m_pStatic->SetTextID(NULL);

	m_pOKBtn = (CCtrlButtonWZ*) getControl(eDIALOG_BTN_OK);
	m_pOKBtn->SetTextID(NULL);

	m_pCancelBtn = (CCtrlButtonWZ*) getControl(eDIALOG_BTN_CANCLE);
	m_pCancelBtn->SetTextID(NULL);

	m_iDelayTime = MISSION_RELAY_NOTICE_TIME;//다이얼로그 노출 시간(11초).
}
//------------------------------------------------------------------------------
/**
*/
void MissionRelay::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void MissionRelay::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void MissionRelay::OnRenderSolarDialog()
{
	if(g_pHero)
		AddFormatText();
}
//------------------------------------------------------------------------------
/**
*/
void MissionRelay::Process(DWORD dwTick)
{
	if((DWORD)m_iDelayTime > dwTick && m_bShow)
	{
		m_iDelayTime -= dwTick;
		if( m_iDelayTime <= 1000 )
		{
			m_bShow = false;
			m_iDelayTime = MISSION_RELAY_NOTICE_TIME;
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void MissionRelay::OnShowWindow(BOOL val)
{
	SetForceMember();

	if( val )
	{
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
void MissionRelay::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_BTN_OK:
				{
					if(TRUE == BattleScene::m_bMissionReward)
					{
						//보상 받기전에는 재도전 못한다.
						TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
						g_InterfaceManager.GetInterfaceString(5121, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
						break;
					}

					uiPartyMan* partyMan =
						static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));


					//방장일때와 파티원일때 스트링인덱스(0:방장 1:파티원)
					bool bLeader = false;

					 if(partyMan && partyMan->IsLeader())
						 bLeader = true;

					 //무조건파티원으로 강제한다.
					 if(m_bForcePartyMember)
						 bLeader = false;
					 
					if(bLeader)
					{
						m_bShow = false;
						m_iDelayTime = MISSION_RELAY_NOTICE_TIME;
						MSG_CG_ZONE_MISSION_RELAY_CREATE_SYN sync;
						sync.m_byCategory = CG_ZONE;
						sync.m_byProtocol = CG_ZONE_MISSION_RELAY_CREATE_SYN;
						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync ,sizeof(sync));
					}
					else
					{
						m_bShow = false;
						m_iDelayTime = MISSION_RELAY_NOTICE_TIME;
						MSG_CG_ZONE_MISSION_RELAY_JOIN_SYN sync;
						sync.m_byCategory = CG_ZONE;
						sync.m_byProtocol = CG_ZONE_MISSION_RELAY_JOIN_SYN;
						sync.m_LobbyKey	  = BattleScene::GetCurLobbyKey();
						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
					}
				}
				break;

			case eDIALOG_BTN_CANCLE:
				{
					m_bShow = false;
					m_iDelayTime = MISSION_RELAY_NOTICE_TIME;
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
void MissionRelay::NetworkProc(MSG_BASE * pMsg)
{

}

void MissionRelay::AddFormatText()
{
#ifdef _INTERNATIONAL_UI
	this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(_T("st10"));
#else
	this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(_T("mn12"));
#endif//_INTERNATIONAL_UI
	RECT rt       = m_pStatic->GetSizeRect();
	RECT rtOk	  = m_pOKBtn->GetSizeRect();
	RECT rtCancel = m_pCancelBtn->GetSizeRect();

#ifdef _INTERNATIONAL_UI
		rtOk.top += 10;
		rtCancel.top += 10;
#else
		rtOk.top += 8;
		rtCancel.top += 8;
#endif//_INTERNATIONAL_UI


	rt.top -= 6;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szResult[INTERFACE_STRING_LENGTH]  = {0,};
	TCHAR szOK[INTERFACE_STRING_LENGTH]		 = {0,};
	TCHAR szCancel[INTERFACE_STRING_LENGTH]  = {0,};

	uiPartyMan* partyMan =
		static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));


	const int arString[2][3]= { {1259,1350,1448},		//파티원일대 메시지스트링
							   	{1258,1350,171}};		//방장일때 메시지스트링

	//방장일때와 파티원일때 스트링인덱스(0:방장 1:파티원)
	int iLeader = 0;
	
	if(partyMan && partyMan->IsLeader())
	{
		iLeader=1;
	}	

	//무조건파티원으로 강제한다.
	if(m_bForcePartyMember)
		iLeader = 0;

	g_InterfaceManager.GetInterfaceString(arString[iLeader][0], szMessage, INTERFACE_STRING_LENGTH);
	g_InterfaceManager.GetInterfaceString(arString[iLeader][1], szOK, INTERFACE_STRING_LENGTH);
	g_InterfaceManager.GetInterfaceString(arString[iLeader][2], szCancel, INTERFACE_STRING_LENGTH);

	
	if( g_pHero )
		Snprintf(szResult , INTERFACE_STRING_LENGTH -1 , szMessage , (int)(m_iDelayTime/1000));

	std::vector<util::_tstring> vstr;
	int cutLen = rt.right - rt.left - 5;
#ifdef _INTERNATIONAL_UI
	GlobalFunc::ProcessWordWrapByFontSize( &vstr, szResult, StrToWzID(_T("st10")),_tcslen(szResult), cutLen );
#else
	GlobalFunc::ProcessWordWrapByFontSize( &vstr, szResult, StrToWzID(_T("mn12")),_tcslen(szResult), cutLen );
#endif//_INTERNATIONAL_UI

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
				iExtraInfoHeight + rt.top + (c_Str_Interval*i),
				rt.right,
				//BUG_FIX: 유동적으로 폰트사이즈만큼 bottom를 내려야맞음
				iExtraInfoHeight + rt.top + (c_Str_Interval*i) + FontSize.cy+2);

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