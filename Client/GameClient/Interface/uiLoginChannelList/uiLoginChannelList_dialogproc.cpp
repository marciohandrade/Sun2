//------------------------------------------------------------------------------
//  uiLoginChannelList_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginChannelList.h"
#include "SceneBase.h"
#include "LoginScene.h"
#include "InterfaceManager.h"
#include <Protocol_CL.h>
#include <PacketStruct_CL.h>
#include "LoadScene.h"
#include "Mouse.h"
#include "uiLoginServerList/uiLoginServerList.h"
#include "uiLoginMan/uiLoginMan.h"
#include "globalfunc.h"
#include "uiAssistMan/uiAssistMan.h"
//------------------------------------------------------------------------------
#ifdef	_K2P_SERVER_LIST_BY_WORD_000897
WzID uiLoginChannelList::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("B001"),  // DLGPOS_BTN_CHANNEL_1=0,
    StrToWzID("B002"),  // DLGPOS_BTN_CHANNEL_2,
    StrToWzID("B003"),  // DLGPOS_BTN_CHANNEL_3,
    StrToWzID("B004"),  // DLGPOS_BTN_CHANNEL_4,
    StrToWzID("B005"),  // DLGPOS_BTN_CHANNEL_5,
    StrToWzID("B006"),  // DLGPOS_BTN_CHANNEL_6,
    StrToWzID("B007"),  // DLGPOS_BTN_CHANNEL_7,

    StrToWzID("TI00"),  // DLGPOS_TXT_CHANNEL_1,
    StrToWzID("TI01"),  // DLGPOS_TXT_CHANNEL_2,
    StrToWzID("TI02"),  // DLGPOS_TXT_CHANNEL_3,
    StrToWzID("TI03"),  // DLGPOS_TXT_CHANNEL_4,
    StrToWzID("TI04"),  // DLGPOS_TXT_CHANNEL_5,
    StrToWzID("TI05"),  // DLGPOS_TXT_CHANNEL_6,
    StrToWzID("TI06"),  // DLGPOS_TXT_CHANNEL_7,

    StrToWzID("ST00"),  // DLGPOS_TXT_STATE_1
    StrToWzID("ST01"),  // DLGPOS_TXT_STATE_2
    StrToWzID("ST02"),  // DLGPOS_TXT_STATE_3
    StrToWzID("ST03"),  // DLGPOS_TXT_STATE_4
    StrToWzID("ST04"),  // DLGPOS_TXT_STATE_5
    StrToWzID("ST05"),  // DLGPOS_TXT_STATE_6
	StrToWzID("ST06"),  // DLGPOS_TXT_STATE_7
};
#else
WzID uiLoginChannelList::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("B001"),  // DLGPOS_BTN_CHANNEL_1=0,
    StrToWzID("B002"),  // DLGPOS_BTN_CHANNEL_2,
    StrToWzID("B003"),  // DLGPOS_BTN_CHANNEL_3,
    StrToWzID("B004"),  // DLGPOS_BTN_CHANNEL_4,
    StrToWzID("B005"),  // DLGPOS_BTN_CHANNEL_5,
    StrToWzID("B006"),  // DLGPOS_BTN_CHANNEL_6,
    StrToWzID("B007"),  // DLGPOS_BTN_CHANNEL_7,

    StrToWzID("TI00"),  // DLGPOS_TXT_CHANNEL_1,
    StrToWzID("TI01"),  // DLGPOS_TXT_CHANNEL_2,
    StrToWzID("TI02"),  // DLGPOS_TXT_CHANNEL_3,
    StrToWzID("TI03"),  // DLGPOS_TXT_CHANNEL_4,
    StrToWzID("TI04"),  // DLGPOS_TXT_CHANNEL_5,
    StrToWzID("TI05"),  // DLGPOS_TXT_CHANNEL_6,
    StrToWzID("TI06"),  // DLGPOS_TXT_CHANNEL_7,

    StrToWzID("P001"),  // DLGPOS_IMG_CH1_00,
    StrToWzID("P002"),  // DLGPOS_IMG_CH1_01,
    StrToWzID("P003"),  // DLGPOS_IMG_CH1_02,
    StrToWzID("P004"),  // DLGPOS_IMG_CH1_03,
    StrToWzID("P005"),  // DLGPOS_IMG_CH1_04,
    StrToWzID("P006"),  // DLGPOS_IMG_CH1_05,
    StrToWzID("P007"),  // DLGPOS_IMG_CH1_06,
    StrToWzID("P008"),  // DLGPOS_IMG_CH1_07,
    StrToWzID("P009"),  // DLGPOS_IMG_CH1_08,

    StrToWzID("P011"),  // DLGPOS_IMG_CH2_00,
    StrToWzID("P012"),  // DLGPOS_IMG_CH2_01,
    StrToWzID("P013"),  // DLGPOS_IMG_CH2_02,
    StrToWzID("P014"),  // DLGPOS_IMG_CH2_03,
    StrToWzID("P015"),  // DLGPOS_IMG_CH2_04,
    StrToWzID("P016"),  // DLGPOS_IMG_CH2_05,
    StrToWzID("P017"),  // DLGPOS_IMG_CH2_06,
    StrToWzID("P018"),  // DLGPOS_IMG_CH2_07,
    StrToWzID("P019"),  // DLGPOS_IMG_CH2_08,

    StrToWzID("P021"),  // DLGPOS_IMG_CH3_00,
    StrToWzID("P022"),  // DLGPOS_IMG_CH3_01,
    StrToWzID("P023"),  // DLGPOS_IMG_CH3_02,
    StrToWzID("P024"),  // DLGPOS_IMG_CH3_03,
    StrToWzID("P025"),  // DLGPOS_IMG_CH3_04,
    StrToWzID("P026"),  // DLGPOS_IMG_CH3_05,
    StrToWzID("P027"),  // DLGPOS_IMG_CH3_06,
    StrToWzID("P028"),  // DLGPOS_IMG_CH3_07,
    StrToWzID("P029"),  // DLGPOS_IMG_CH3_08,

    StrToWzID("P031"),  // DLGPOS_IMG_CH4_00,
    StrToWzID("P032"),  // DLGPOS_IMG_CH4_01,
    StrToWzID("P033"),  // DLGPOS_IMG_CH4_02,
    StrToWzID("P034"),  // DLGPOS_IMG_CH4_03,
    StrToWzID("P035"),  // DLGPOS_IMG_CH4_04,
    StrToWzID("P036"),  // DLGPOS_IMG_CH4_05,
    StrToWzID("P037"),  // DLGPOS_IMG_CH4_06,
    StrToWzID("P038"),  // DLGPOS_IMG_CH4_07,
    StrToWzID("P039"),  // DLGPOS_IMG_CH4_08,

    StrToWzID("P041"),  // DLGPOS_IMG_CH5_00,
    StrToWzID("P042"),  // DLGPOS_IMG_CH5_01,
    StrToWzID("P043"),  // DLGPOS_IMG_CH5_02,
    StrToWzID("P044"),  // DLGPOS_IMG_CH5_03,
    StrToWzID("P045"),  // DLGPOS_IMG_CH5_04,
    StrToWzID("P046"),  // DLGPOS_IMG_CH5_05,
    StrToWzID("P047"),  // DLGPOS_IMG_CH5_06,
    StrToWzID("P048"),  // DLGPOS_IMG_CH5_07,
    StrToWzID("P049"),  // DLGPOS_IMG_CH5_08,

    StrToWzID("P051"),  // DLGPOS_IMG_CH6_00,
    StrToWzID("P052"),  // DLGPOS_IMG_CH6_01,
    StrToWzID("P053"),  // DLGPOS_IMG_CH6_02,
    StrToWzID("P054"),  // DLGPOS_IMG_CH6_03,
    StrToWzID("P055"),  // DLGPOS_IMG_CH6_04,
    StrToWzID("P056"),  // DLGPOS_IMG_CH6_05,
    StrToWzID("P057"),  // DLGPOS_IMG_CH6_06,
    StrToWzID("P058"),  // DLGPOS_IMG_CH6_07,
    StrToWzID("P059"),  // DLGPOS_IMG_CH6_08,

    StrToWzID("P061"),  // DLGPOS_IMG_CH7_00,
    StrToWzID("P062"),  // DLGPOS_IMG_CH7_01,
    StrToWzID("P063"),  // DLGPOS_IMG_CH7_02,
    StrToWzID("P064"),  // DLGPOS_IMG_CH7_03,
    StrToWzID("P065"),  // DLGPOS_IMG_CH7_04,
    StrToWzID("P066"),  // DLGPOS_IMG_CH7_05,
    StrToWzID("P067"),  // DLGPOS_IMG_CH7_06,
    StrToWzID("P068"),  // DLGPOS_IMG_CH7_07,
    StrToWzID("P069"),  // DLGPOS_IMG_CH7_08,
};
#endif

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiLoginChannelList::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->CheckControls())
            return;

        //MoveWindowWZ( (float)0, (float)0 );

        m_vChannelIdx.clear();

        this->OffPending();
        this->OffLock();
    }
    else
    {
        this->OnPending();
        this->OnLock();
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiLoginChannelList::SelectChannel( POSTYPE pos, int svIdx )
{
	if( pos >= POS_BTN_END )
	{
		pos -= POS_BTN_END;

		if( pos < m_vChannelIdx.size() )
		{
			int toUniqueIdx = m_vChannelIdx[pos];

			for( int i = 0; i < POS_BTN_SIZE; ++i )
			{
				if( pos == i && m_addCounts >= pos)
				{
					AuthGroupData* pAuthGroupData = GetManager()->GetAuthGroupByIndex( svIdx );
					if (pAuthGroupData)
					{
						AuthSetverData* pAuthSetverData = pAuthGroupData->GetAuthServerDatabyID(toUniqueIdx);
						if (pAuthSetverData)
						{
#ifdef _DEV_VER//_DH_LOGIN_LOG_ID_AND_CHANNEL
                            TCHAR szTitle[MAX_PATH];
                            TCHAR temp_string[MAX_PATH];
                            GetWindowText(g_hWndMain, temp_string, sizeof(szTitle));
                            sprintf(szTitle, "%s@%s", strtok(temp_string, "@"), pAuthSetverData->m_ServerName.c_str());
                            ::SetWindowText(g_hWndMain, szTitle);
#endif//_DH_LOGIN_LOG_ID_AND_CHANNEL
							GetManager()->SEND_AUTH_SERVER_SELECT_SYN(pAuthSetverData);
						}
					}
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginChannelList::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            int svIdx = TestSelectedAuthServerGroup();

			if (svIdx != -1)
			{
				uiLoginServerList * serverList = GET_CAST_DIALOG( uiLoginServerList, IM_LOGIN_MANAGER::LOGIN_SERVERLIST );
				if (serverList)
				{
					POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
#ifdef _SCJ_PK_POPUP_DIALOG
					//pk안내 팝업창 뛰움					
					uiAssistMan * assist_man = GET_DIALOG_MANAGER(uiAssistMan, UIMAN_ASSIST);
					assist_man->ShowAskPkPopupDialog(true, pos, svIdx);
#else
					SelectChannel(pos, svIdx);
#endif //_SCJ_PK_POPUP_DIALOG
				}
			}
        }
        break;
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiLoginChannelList::OnUpdateSolarDialog()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH], szText[INTERFACE_STRING_LENGTH], szString[INTERFACE_STRING_LENGTH];

	m_vChannelIdx.clear();

	int i;

	for (i=0; i<POS_BTN_SIZE; ++i)
	{
		m_pBtnControls[i]->ShowWindowWZ(WZ_HIDE);
	}

	for (i=0; i<POS_TXT_SIZE; ++i)
	{
		GlobalFunc::SetCaption (m_pTxtControls[i], _T(""));
		m_pTxtControls[i]->ShowWindowWZ(WZ_HIDE);
	}

#ifdef	_K2P_SERVER_LIST_BY_WORD_000897
	for( i = 0; i < POS_TXT_STATE_SIZE; ++i )
	{
		GlobalFunc::SetCaption( m_pTxtState[i], _T("") );
		m_pTxtState[i]->ShowWindowWZ(WZ_HIDE);
	}
#else
	for (i=DLGPOS_IMG_CH1_00; i<(DLGPOS_IMG_CH7_08+1); ++i)
	{
		CCtrlStaticWZ* imgctrl =
			static_cast<CCtrlStaticWZ *>(getControl(i));
		if (imgctrl)
			imgctrl->ShowWindowWZ(WZ_HIDE);
	}
#endif

	uiLoginServerList * serverList = GET_CAST_DIALOG( uiLoginServerList, IM_LOGIN_MANAGER::LOGIN_SERVERLIST );

	INT svIdx = serverList->GetCurrentSelectedServerIdx();

	if (svIdx == -1)
	{
		serverList->GetDialogWZ()->MoveWindowWZ(
			(float)serverList->GetFixRC().left, 
			(float)serverList->GetFixRC().top);
		return;
	}

	if (!serverList)
	{
		serverList->GetDialogWZ()->MoveWindowWZ(
			(float)serverList->GetFixRC().left, 
			(float)serverList->GetFixRC().top);
		return;
	}

	RECT clientRC;

	serverList->GetDialogWZ()->GetClientRect(&clientRC);
	float fDelta = (float)((serverList->GetFixRC().left - 100) - clientRC.left)/1;
	fDelta = fabs(fDelta);

	float fAfter = clientRC.left - fDelta;
	if ( (serverList->GetFixRC().left - 100) >= fAfter )
		fAfter = (float)(serverList->GetFixRC().left - 100);

	serverList->GetDialogWZ()->MoveWindowWZ(fAfter, (float)serverList->GetFixRC().top);

	m_addCounts = 0;

	AuthGroupData* pAuthGroupData = GetManager()->GetAuthGroupByIndex(svIdx);
	if (!pAuthGroupData)
	{
		serverList->GetDialogWZ()->MoveWindowWZ(
			(float)serverList->GetFixRC().left, 
			(float)serverList->GetFixRC().top);
		return;
	}

	for (i=0; i<pAuthGroupData->GetAuthServerDataCount(); ++i)
	{
		if (m_pBtnControls[m_addCounts] && m_pTxtControls[m_addCounts])
		{
			AuthSetverData* pAuthSetverData = pAuthGroupData->GetAuthServerDatabyIndex(i);
			if (!pAuthSetverData)
			{
				continue;
			}
			
			// 상태가 없다면
			if (pAuthSetverData->m_CrowdGrade == eAUTH_SERVER_STAT_NONE)
			{
				continue;
			}

			DWORD dwStatusStringCode = eST_HIGH;
#ifdef	_K2P_SERVER_LIST_BY_WORD_000897
			DWORD dwStatusLevel = RGBA( 0, 232, 12, 255 );
#else
			DWORD dwStatusLevel = c_iLOGIN_CHANNELLIST_STATUS_LEVEL_1;
#endif	
			switch (pAuthSetverData->m_CrowdGrade)
			{
			case eAUTH_SERVER_STAT_NORMAL_LEVEL_1:
			case eAUTH_SERVER_STAT_NORMAL_LEVEL_2:
			case eAUTH_SERVER_STAT_NORMAL_LEVEL_3:
				{
					dwStatusStringCode = eST_HIGH;
#ifdef	_K2P_SERVER_LIST_BY_WORD_000897
					dwStatusLevel = RGBA( 0, 232, 12, 255 );
#else
					dwStatusLevel = c_iLOGIN_CHANNELLIST_STATUS_LEVEL_1;
#endif					
				}
				break;
			case eAUTH_SERVER_STAT_BUSY_LEVEL_4:
			case eAUTH_SERVER_STAT_BUSY_LEVEL_5:
			case eAUTH_SERVER_STAT_BUSY_LEVEL_6:
				{
					dwStatusStringCode = eST_NORMAL;
#ifdef	_K2P_SERVER_LIST_BY_WORD_000897
					dwStatusLevel = RGBA( 0, 232, 12, 255 );
#else
					dwStatusLevel = c_iLOGIN_CHANNELLIST_STATUS_LEVEL_2;
#endif				
				}
				break;
			case eAUTH_SERVER_STAT_VERY_BUSY_LEVEL_7:
			case eAUTH_SERVER_STAT_VERY_BUSY_LEVEL_8:
				{
					dwStatusStringCode = eST_LOW;
#ifdef	_K2P_SERVER_LIST_BY_WORD_000897
					dwStatusLevel = RGBA( 2, 169, 255, 255 );
#else
					dwStatusLevel = c_iLOGIN_CHANNELLIST_STATUS_LEVEL_3;
#endif				
				}
				break;
			case eAUTH_SERVER_STAT_VERY_BUSY_LEVEL_9:
				{
					dwStatusStringCode = eST_LOW;
#ifdef	_K2P_SERVER_LIST_BY_WORD_000897
					dwStatusLevel = RGBA( 255, 0, 0, 255 );
#else
					dwStatusLevel = c_iLOGIN_CHANNELLIST_STATUS_LEVEL_MAX;
#endif				
				}
				break;
			default:
				{
				}
			}

			g_InterfaceManager.GetInterfaceString( eST_CHANNEL, szText, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.GetInterfaceString( dwStatusStringCode, szString, INTERFACE_STRING_LENGTH );
			Sprintf (szMessage, _T("%s"), pAuthSetverData->m_ServerName.c_str());

			m_pTxtControls[m_addCounts]->SetFontID(StrToWzID("mn12"));
			m_pTxtControls[m_addCounts]->SetTextColor( RGBA(240, 201, 121, 255) );
			GlobalFunc::SetCaption( m_pTxtControls[m_addCounts], szMessage );
			m_pTxtControls[m_addCounts]->ShowWindowWZ(WZ_SHOW);
			m_pBtnControls[m_addCounts]->ShowWindowWZ(WZ_SHOW);

#ifdef	_K2P_SERVER_LIST_BY_WORD_000897
			m_pTxtState[m_addCounts]->ShowWindowWZ(WZ_SHOW);
			m_pTxtState[m_addCounts]->SetTextColor( dwStatusLevel );
			GlobalFunc::SetCaption( m_pTxtState[m_addCounts], szString );
#else
			DrawGauge( dwStatusLevel );
#endif

			m_vChannelIdx.push_back(pAuthSetverData->m_ChannelID);
			if (++m_addCounts >= 8)
				break;
		}
	}

	if (!m_addCounts)
	{
		serverList->GetDialogWZ()->MoveWindowWZ(
			(float)serverList->GetFixRC().left, 
			(float)serverList->GetFixRC().top );
	}
}

void	uiLoginChannelList::DrawGauge( int count )
{
#ifndef	_K2P_SERVER_LIST_BY_WORD_000897
	int imgidx;
	for( imgidx=0; imgidx < c_iLOGIN_CHANNELLIST_STATUS_GROUP_GAP; ++imgidx )
	{
		POSTYPE pos = DLGPOS_IMG_CH1_00 + (c_iLOGIN_CHANNELLIST_STATUS_GROUP_GAP * m_addCounts);

		CCtrlButtonWZ* imgctrl =
			static_cast<CCtrlButtonWZ *>(getControl(pos + imgidx));
		if (imgctrl)
			imgctrl->ShowWindowWZ(WZ_HIDE);
	}

	for( imgidx=0; imgidx < count; ++imgidx )
	{
		POSTYPE pos = DLGPOS_IMG_CH1_00 + (c_iLOGIN_CHANNELLIST_STATUS_GROUP_GAP * m_addCounts);

		CCtrlButtonWZ* imgctrl =
			static_cast<CCtrlButtonWZ *>(getControl(pos + imgidx));
		if (imgctrl)
			imgctrl->ShowWindowWZ(WZ_SHOW);
	}
#endif
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
