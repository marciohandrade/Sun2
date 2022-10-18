//------------------------------------------------------------------------------
//  AskReplyGuildDialog.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "AskReplyGuildDialog.h"
#include "GameFramework.h"
#include "interfacemanager.h"
#include "globalfunc.h"
//------------------------------------------------------------------------------
/**
*/
AskReplyGuildDialog::AskReplyGuildDialog(InterfaceManager *pUIMan)
:AskDialog(pUIMan)
{
	m_dwOperObjectKey = 0;
}

//------------------------------------------------------------------------------
/**
*/
AskReplyGuildDialog::~AskReplyGuildDialog(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void AskReplyGuildDialog::OnShowWindow(BOOL val)
{
    AskDialog::OnShowWindow(val);

    if (val)
    {
		SolarDialog::MoveCenter();

        bDestroy = true;
        bDestroyTimer = false;
        dwDestroyTick = 0;
        dwDestroyTickOver = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
void AskReplyGuildDialog::Process(DWORD dwTick)
{
    if (this->GetStatus() != ASK_STATUS_NORMAL)
        return;

    if (bDestroyTimer)
    {
        dwDestroyTick = clock_function::GetTickCount();

        if (dwDestroyTick > dwDestroyTickOver)
        {
			ShowInterface(FALSE);

            this->m_status = ASK_STATUS_CANCEL;

			NET_SEND_CG_GUILD_INVITE_ANSWER_SYN(false);
            bDestroyTimer = false;
        }
    }

    if (bDestroy)
    {
        dwDestroyTick = clock_function::GetTickCount();
        dwDestroyTickOver = clock_function::GetTickCount() + (15*1000);
        bDestroy = false;
        bDestroyTimer = true;
    }
}

//------------------------------------------------------------------------------
/**
*/
void AskReplyGuildDialog::MessageProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case ASK_OK:
                {
                    this->m_status = ASK_STATUS_OK;

					TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
					//	길드 가입을 수락했습니다.
					g_InterfaceManager.GetInterfaceString( eST_AGREE_JOIN_GUILD, szMessage, INTERFACE_STRING_LENGTH );
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

					NET_SEND_CG_GUILD_INVITE_ANSWER_SYN(true);

					ShowInterface(FALSE);
                }
                break;

            case ASK_CANCEL:
                {
                    this->m_status = ASK_STATUS_CANCEL;

					TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
					//	길드 가입을 거절했습니다.
					g_InterfaceManager.GetInterfaceString( eST_REJECT_INVITE_GUILD, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

					NET_SEND_CG_GUILD_INVITE_ANSWER_SYN(false);
					ShowInterface(FALSE);

                }
                break;
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
void AskReplyGuildDialog::OnUpdateSolarDialog()
{
    CCtrlListWZ * pLCtrl = (CCtrlListWZ *)getControl( ASK_LIST );
    pLCtrl->DeleteItemAll();
    pLCtrl->SetBeginDrawIndex(0);
    pLCtrl->SetFontID(StrToWzID("mn12"));

    TCHAR	szText[INTERFACE_STRING_LENGTH], szMessage[INTERFACE_STRING_LENGTH];

    int idx;

    //	귀하에게 길드 가입을 요청했습니다.
    g_InterfaceManager.GetInterfaceString( eST_INVITE_GUILD, szMessage, INTERFACE_STRING_LENGTH );
    pLCtrl->SetColorText(WzColor_RGBA( 152, 208, 142, 255 ));
    idx = pLCtrl->InsertItemBack();
	GlobalFunc::SetItemText( pLCtrl, idx, szMessage );
    pLCtrl->SetItemFontID( idx, 0, StrToWzID("mn12"));
    pLCtrl->SetItemBKColor( idx, 0, WzColor_RGBA(0,0,0,0));
    pLCtrl->SetItemTextColor( idx, 0, WzColor_RGBA( 152, 208, 142, 255 ));

    //	수락하시겠습니까?
    g_InterfaceManager.GetInterfaceString( eST_ACCEPT_INVITE, szMessage, INTERFACE_STRING_LENGTH );
    pLCtrl->SetColorText(WzColor_RGBA( 152, 208, 142, 255 ));
    idx = pLCtrl->InsertItemBack();
	GlobalFunc::SetItemText( pLCtrl, idx, szMessage );
    pLCtrl->SetItemFontID( idx, 0, StrToWzID("mn12"));
    pLCtrl->SetItemBKColor( idx, 0, WzColor_RGBA(0,0,0,0));
    pLCtrl->SetItemTextColor( idx, 0, WzColor_RGBA( 152, 208, 142, 255 ));

    //	남은 시간.... ( %d )
    g_InterfaceManager.GetInterfaceString( eST_REMAIN_TIME, szText, INTERFACE_STRING_LENGTH );
    Sprintf(szMessage, szText, ((dwDestroyTickOver - dwDestroyTick)/1000)+1);
    pLCtrl->SetColorText(WzColor_RGBA( 152, 208, 142, 255 ));
    idx = pLCtrl->InsertItemBack();
	GlobalFunc::SetItemText( pLCtrl, idx, szMessage );
    pLCtrl->SetItemFontID( idx, 0, StrToWzID("mn12"));
    pLCtrl->SetItemBKColor( idx, 0, WzColor_RGBA(0,0,0,0));
    pLCtrl->SetItemTextColor( idx, 0, WzColor_RGBA( 152, 208, 142, 255 ));

    pLCtrl->SetUnuseSelBar(TRUE);
    pLCtrl->OnFocusOut();

}

//------------------------------------------------------------------------------
/**
*/
void AskReplyGuildDialog::NET_SEND_CG_GUILD_INVITE_ANSWER_SYN(bool bInviteAccept)
{
	MSG_CG_GUILD_INVITE_ANSWER_SYN SendMsg;
	SendMsg.m_OperUserKey = m_dwOperObjectKey;

	if( bInviteAccept )
	{
		SendMsg.m_bInveite = true;
	}
	else
	{
		SendMsg.m_bInveite = false;
	}
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
}



