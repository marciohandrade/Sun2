#include "SunClientPrecompiledHeader.h"

#include "ChatPersonalOption.h"
#include "InterfaceManager.h"
#include "uichatman/uiChatMan.h"
#include "ChatDialog.h"
#include "HeroActionInput.h"
#include "uiPartyMan/uiPartyMan.h"
#include "Mouse.h"
#include "ObjectManager.h"

#include "GlobalFunc.h"
#include "GameConst_Color.h"

#include "ChatDialog.h"
#include "uiCommunityMan/uiCommunityMan.h"  

const WzColor c_CharNameColor			= WzColor_RGBA( 198, 178, 137, 255 );
const WzColor c_MouseOverClr	        = WzColor_RGBA( 0, 255, 0, 255);

const BYTE    c_btSellHeight			= 15;

WzID ChatPersonalOption::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("L001"),		//	DLGPOS_LIST
    StrToWzID("B000"),		//	DLGPOS_CANCLE
};							
//------------------------------------------------------------------------------
/**
*/
ChatPersonalOption::ChatPersonalOption(InterfaceManager* pUIMan):
	uiBase(pUIMan)
{
	memset(m_szClickedCharName, 0, INTERFACE_STRING_LENGTH+1);
}
//------------------------------------------------------------------------------
/**
*/
ChatPersonalOption::~ChatPersonalOption()
{

}
//------------------------------------------------------------------------------
/**
*/
void ChatPersonalOption::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	m_pListCtrl = static_cast<CCtrlListWZ*>(getControl(DLGPOS_LIST));
	assert(m_pListCtrl);

	memset(m_szClickedCharName, 0, INTERFACE_STRING_LENGTH+1);
}
//------------------------------------------------------------------------------
/**
*/
void ChatPersonalOption::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void ChatPersonalOption::OnUpdateSolarDialog()
{
	if(!m_bShow)
		ShowDialog(FALSE);
}
//------------------------------------------------------------------------------
/**
*/
void ChatPersonalOption::Process(DWORD dwTick)
{

}
//------------------------------------------------------------------------------
/**
*/
void ChatPersonalOption::OnShowWindow(BOOL val)
{
	if( val )
	{
		if(m_pListCtrl)
		{
			m_pListCtrl->DeleteItemAll();
			m_pListCtrl->SetBeginDrawIndex(0);
			m_pListCtrl->SetColorSelBar(c_InterfaceBKColor);
			m_pListCtrl->SetSellHeight(c_btSellHeight);

			ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
				

			if(pChatDlg)
			{
				RECT rt;
				rt = pChatDlg->GetListRect();
                this->MoveWindowWZ(rt.right + 22, rt.top + c_iAddYSizeofChatPersonalOption);
			}

			int idx = m_pListCtrl->InsertItemBack();
			GlobalFunc::SetItem(m_pListCtrl, idx, 0, m_szClickedCharName, StrToWzID("mn12"), //Clicked Character Name
				DT_CENTER | DT_BOTTOM, c_InterfaceBKColor, c_CharNameColor, c_CharNameColor);

            m_pListCtrl->SetItemUserData(idx, 0, (void*)this);
            m_pListCtrl->SetItemCallbackDraw(idx, 0, CallbackRender);

			TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString(9706, szMessage, INTERFACE_STRING_LENGTH);//귓속말.
			idx = m_pListCtrl->InsertItemBack();
			GlobalFunc::SetItem(m_pListCtrl , idx , 0 , szMessage, StrToWzID("mn12"), 
				DT_CENTER | DT_BOTTOM,  c_InterfaceBKColor, c_InterfaceTextColor, c_MouseOverClr );

            m_pListCtrl->SetItemUserData(idx, 0, (void*)this);
            m_pListCtrl->SetItemCallbackDraw(idx, 0, CallbackRender);

			g_InterfaceManager.GetInterfaceString(9721, szMessage, INTERFACE_STRING_LENGTH);//친구추가.
			idx = m_pListCtrl->InsertItemBack();
			GlobalFunc::SetItem(m_pListCtrl , idx , 0 , szMessage, StrToWzID("mn12"), 
				DT_CENTER | DT_BOTTOM,  c_InterfaceBKColor, c_InterfaceTextColor, c_MouseOverClr );

            m_pListCtrl->SetItemUserData(idx, 0, (void*)this);
            m_pListCtrl->SetItemCallbackDraw(idx, 0, CallbackRender);

			g_InterfaceManager.GetInterfaceString(9722, szMessage, INTERFACE_STRING_LENGTH);//거래요청.
			idx = m_pListCtrl->InsertItemBack();
			GlobalFunc::SetItem(m_pListCtrl , idx , 0 , szMessage, StrToWzID("mn12"), 
				DT_CENTER | DT_BOTTOM,  c_InterfaceBKColor, c_InterfaceTextColor, c_MouseOverClr );

            m_pListCtrl->SetItemUserData(idx, 0, (void*)this);
            m_pListCtrl->SetItemCallbackDraw(idx, 0, CallbackRender);

			g_InterfaceManager.GetInterfaceString(9705, szMessage, INTERFACE_STRING_LENGTH);//파티초대.
			idx = m_pListCtrl->InsertItemBack();
			GlobalFunc::SetItem(m_pListCtrl , idx , 0 , szMessage, StrToWzID("mn12"), 
				DT_CENTER | DT_BOTTOM,  c_InterfaceBKColor, c_InterfaceTextColor, c_MouseOverClr );

            m_pListCtrl->SetItemUserData(idx, 0, (void*)this);
            m_pListCtrl->SetItemCallbackDraw(idx, 0, CallbackRender);

            g_InterfaceManager.GetInterfaceString(1318, szMessage, INTERFACE_STRING_LENGTH);//차단추가.
			idx = m_pListCtrl->InsertItemBack();
			GlobalFunc::SetItem(m_pListCtrl , idx , 0 , szMessage, StrToWzID("mn12"), 
				DT_CENTER | DT_BOTTOM,  c_InterfaceBKColor, c_InterfaceTextColor, c_MouseOverClr );

            m_pListCtrl->SetItemUserData(idx, 0, (void*)this);
            m_pListCtrl->SetItemCallbackDraw(idx, 0, CallbackRender);
		}
		m_bShow   = TRUE;
	}
	else
	{
		m_bShow   = FALSE;
	}
}
//------------------------------------------------------------------------------
/**
*/
void ChatPersonalOption::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_RBUTTONCLICK:
		{
			ShowWindow(FALSE);
		}
		break;

	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getControlIDToPos(pMessage->dwCtrlID);
			switch (pos)
			{
			case DLGPOS_LIST:
				{
					switch(m_pListCtrl->GetCurSel())
					{
					case CONTROL_NAME:		//캐릭터 네임.
						{
						}
						break;

					case CONTROL_WISPER:		//귓속말.
						{
							TCHAR szTemp[INTERFACE_STRING_LENGTH]	 = {0,};
							TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

							ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
								
							if (pChatDlg)
							{
								g_InterfaceManager.GetInterfaceString( 2010, szTemp, INTERFACE_STRING_LENGTH );// /귓속말
								Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%s %s "), szTemp, m_szClickedCharName);

								if(!pChatDlg->IsVisibleWindow())
									pChatDlg->ShowWindow(TRUE);

								pChatDlg->SetChatMessage(szMessage);
								ShowWindow(FALSE);
							}
						}
						break;

					case CONTROL_ADD_FRIEND:		//친구추가.
						{
							g_HeroInput.ProcessUserAction(eACTION_FRIEND_ADD_IN_CHAT_DLG + eACTION_START_SKILL_CODE);
							ShowWindow(FALSE);
						}
						break;

					case CONTROL_TRADE:		//거래요청.
						{
							g_HeroInput.ProcessUserAction(eACTION_TRADE_IN_CHAT_DLG + eACTION_START_SKILL_CODE);
							ShowWindow(FALSE);
						}
						break;

					case CONTROL_ADD_PARTY:		//파티초대.
						{
							bool  bFounded = false;
							DWORD dwTargetKey = 0;

							uiPartyMan* partyMan =
								static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
							if (partyMan)
							{
#ifdef	_K2P_OBJECT_MANAGER_MODIFIDY
								if( g_ObjectManager.GetObject( m_szClickedCharName ) == 0 )
								{
									// 그런 유저가 없슴
									//	'%s' 님을 찾을 수 없습니다.
									TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
									g_InterfaceManager.GetInterfaceString( eST_NAMECODE_NOT_FOUND, szMessage, INTERFACE_STRING_LENGTH );
									GlobalFunc::OutputMsg_Notice (eOUTPUT_MSG_CHAT, szMessage, m_szClickedCharName);
									return;
								}
#else
#endif
								if ( partyMan->IsParty() == false )
									partyMan->AskJoinParty( m_szClickedCharName );
								else if	( partyMan->IsParty() && partyMan->IsLeader() )
									partyMan->AskJoinParty( m_szClickedCharName );
								else 
								{
									TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
									g_InterfaceManager.GetInterfaceString( eST_PARTY_ISNOT_MASTEROFPARTY, szMessage, INTERFACE_STRING_LENGTH );
									GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, szMessage);
								}								
								ShowWindow(FALSE);
							}
						}
						break;

                    case CONTROL_ADD_BLOCK:		//차단.
						{
                            bool  bFounded = false;
							DWORD dwTargetKey = 0;

                            ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
							if (pChatDlg != NULL)
							{
                                uiCommunityMan* community_manager = 
                                    static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
                                if (community_manager != NULL && community_manager->FindBlockFriend(m_szClickedCharName) == -1)// -1 = block  
                                {
                                    MSG_CW_FRIEND_BLOCK_SYN sync;
                                    ZeroMemory(sync.ptszBlockName,sizeof(sync.ptszBlockName));
                                    TCHAR szString[MAX_CHARNAME_LENGTH+1] ={0,};
                                    StrnCopy(szString, m_szClickedCharName, MAX_CHARNAME_LENGTH);
                                    StrnCopy(sync.ptszBlockName ,szString, MAX_CHARNAME_LENGTH);
                                    GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
								}								
								ShowWindow(FALSE);
							}
						}
						break;
					}
				}
				break;
            case DLGPOS_CANCLE: // 취소(X) 버튼
                {
                    OnShowWindow(FALSE);
                }
                break;
			}
		}
		break;

	case RT_MSG_MOUSEOVER:
		{
			switch(getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_LIST:
				{
					int iMouseY = Mouse::Instance()->GetMouseY();
					RECT rc_listctrl = m_pListCtrl->GetSizeRect();
					int iPos = iMouseY - rc_listctrl.top;

					if(iPos > 0)
					{
						int cursel = iPos / c_btSellHeight;
						m_pListCtrl->SetCurSel(cursel);
					}
				}
				break;
			}
		}
		break;

	default:
		{
			SolarDialog::MessageProc(pMessage);
		}
		break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void ChatPersonalOption::NetworkProc(MSG_BASE * pMsg)
{
}
//------------------------------------------------------------------------------
/**
*/
void ChatPersonalOption::MoveDefaultPosition()
{
    ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
	if(pChatDlg)
	{
		RECT rt;
		rt = pChatDlg->GetListRect();
        this->MoveWindowWZ(rt.right + 22, rt.top + c_iAddYSizeofChatPersonalOption);
	}
}

void ChatPersonalOption::CallbackRender( CCtrlListWZ* list_control, CCtrlListWZ::CListSellData* sell_data, int sell_index, RECT& render_area )
{
    if (list_control == NULL || sell_data == NULL) 
    {
        return;
    }

    int select_sell = list_control->GetCurSel();
    int over_sell = list_control->GetOverSel();

    g_pSunRenderer->x_pManagerTextOut->StartToUseFont(sell_data->dwFontID);
    {
        g_pSunRenderer->x_pManagerTextOut->DrawText(sell_data->stdStrText.c_str(),
            &render_area,
            (sell_index == over_sell) ? c_MouseOverClr : sell_data->FontColor,
            WzColor_RGBA(0, 0, 0, 0),
            TP_HCENTER | TP_VBOTTOM,
            ETS_OUTLINE);
    }
    g_pSunRenderer->x_pManagerTextOut->EndUseFont();

}
////------------------------------------------------------------------------------
///**
//*/
//void ChatPersonalOption::UpdatePosition()
//{
//    ////SolarDialog::UpdatePosition();
//}
