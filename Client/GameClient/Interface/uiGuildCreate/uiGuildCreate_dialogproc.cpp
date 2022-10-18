//------------------------------------------------------------------------------
//  uiGuildCreate_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildCreate.h"
#include "BattleRoomEntry.h"
#include "BattleEntryTable.h"
#include "MapInfoParser.h"
#include "BattleScene.h"
#include "Hero.h"
#include "ChatDialog.h"
#include "uiBattleZonePassword/uiBattleZonePassword.h"
#include "map.h"
#include "uiGuildMan/uiGuildMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "GuildRankInfoParser.h"

void
guildcreateConfirm( bool bYes )
{
    uiBase* pUIBase = GET_DIALOG( IM_GUILD_MANAGER::GUILD_CREATE );

    if (pUIBase)
    {
        pUIBase->ShowInterface(FALSE);
    }
    else
    {
        assert (!"uiGuildCreate Ŭ������ �����ϴ�.");
    }

    TCHAR   szMessage[INTERFACE_STRING_LENGTH];

    if (bYes)
    {
        uiGuildMan* guildMan = 
            static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
        assert (guildMan);

        uiGuildCreate* guildCreate = GET_CAST_DIALOG( uiGuildCreate, IM_GUILD_MANAGER::GUILD_CREATE );
        assert (guildCreate);

        MSG_CG_GUILD_CREATE_SYN send;
        memset (send.m_tszGuildName, 0, MAX_GUILDNAME_LENGTH);
        StrnCopy(send.m_tszGuildName, guildCreate->m_szGuildName, MAX_GUILDNAME_LENGTH);
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send, sizeof (send));
    }
    else
    {
		//	��� â���� ��� �ϼ̽��ϴ�.
		const int	c_iCancelCreateGuild = 70434;
		g_InterfaceManager.GetInterfaceString( c_iCancelCreateGuild, szMessage, INTERFACE_STRING_LENGTH );

        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }
}


//------------------------------------------------------------------------------
WzID uiGuildCreate::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("TI00"),    // DLGPOS_BTN_GUILD_CREATE,
        StrToWzID("TI01"),    // DLGPOS_BTN_GUILD_CANCEL,

        StrToWzID("T002"),    // DLGPOS_TXT_LIMITED_LEVEL,   // ��� â���� ���� ���� ����
        StrToWzID("T003"),    // DLGPOS_TXT_LIMITED_HIME,    // ��� â���� ���� �ʿ� ����
        
        StrToWzID("E001"),    // DLGPOS_EDT_GUILD_NAME,      // ��� �̸�
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiGuildCreate::OnShowWindow( BOOL val )
{
    if (val)
    {
        SolarDialog::MoveCenter();

        this->InitControls();

        TCHAR   szMessage[INTERFACE_STRING_LENGTH], szText[INTERFACE_STRING_LENGTH];

        // ���� ����
        const int c_iGUILD_LIMIT_LEVEL = 70437;
        g_InterfaceManager.GetInterfaceString( c_iGUILD_LIMIT_LEVEL, szText, INTERFACE_STRING_LENGTH );
        Sprintf( szMessage, szText, CREATE_GUILD_LIMIT_LEVEL );
        GlobalFunc::SetCaption(this->m_pTxtControls[0], szMessage);

        // ���� ����
        const int c_iGUILD_LIMIT_MONEY = 70436;
        g_InterfaceManager.GetInterfaceString( c_iGUILD_LIMIT_MONEY, szText, INTERFACE_STRING_LENGTH );
		
		MONEY	money = -1;
		GuildRankInfo	* pRankInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( GRADE_AT_GUILD_CREATE );
		if( pRankInfo != NULL )
			money = pRankInfo->m_dwHeim;

		TCHAR szMoney[INTERFACE_STRING_LENGTH + 1] = {0,};
		NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
		Sprintf(szMoney, _T("%I64u"), money);
		::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szMoney, &nFmt, szMessage, sizeof(szMessage));

		GlobalFunc::SetCaption(this->m_pTxtControls[1], szMessage);

        // ��� �̸�
        GlobalFunc::SetText(this->m_pEdtControl, _T(""));
        this->m_pEdtControl->SetFontID(StrToWzID("mn12"));
        this->m_pEdtControl->SetColorText(RGBA(255, 255, 255, 255));
    
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_guild_create;
        msg.wParam = InterfaceManager::DIALOG_GUILD_CREATE;
        msg.DoSomething = GlobalFunc::DoGuildCreate;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_guild_create);
		
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildCreate::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_BTN_GUILD_CREATE:
                {
					if(g_pHero&&g_pHero->GetChaoState()==eCHAO_STATE_CHAO)
					{
						//	ī�� ���¿��� ��� â�� �Ҽ� �����ϴ�.
						g_InterfaceManager.GetInterfaceString( eST_CHECK_GUILD_CHAO, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
						return;
					}


					TCHAR   lpszTxt[INTERFACE_STRING_LENGTH] = {0,};
                    GlobalFunc::GetText(this->m_pEdtControl, lpszTxt, INTERFACE_STRING_LENGTH);
                    if (lstrlen(lpszTxt)==0)
                    {
                        //	������ �Է��ϼ���.
                        g_InterfaceManager.GetInterfaceString( eST_INPUT_GUILD_NAME, szMessage, INTERFACE_STRING_LENGTH );

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_Verify(szMessage);

                    }
                    else
                    {
                        if (_tcslen(lpszTxt) > MAX_GUILDNAME_LENGTH)
                        {
							//	16�� ������ ����˴ϴ�.
							g_InterfaceManager.GetInterfaceString( eST_CHECK_GUILD_NAME_LENGTH_16, szMessage, INTERFACE_STRING_LENGTH );
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
							return;
                        }

                        lpszTxt[MAX_GUILDNAME_LENGTH] = 0;

                        memset (this->m_szGuildName, 0, sizeof(m_szGuildName));
						StrnCopy(this->m_szGuildName, lpszTxt, MAX_GUILDNAME_LENGTH);
                        
						char	szString[INTERFACE_STRING_LENGTH] = {0,};
						
#if defined(__KR) || defined(_KOREA)
							if(g_slangNameFilter.IsHaveReplace(m_szGuildName)||g_slangNameFilter.CheckIncludeString(m_szGuildName))
							{
								//	���� �ܾ� ���
								g_InterfaceManager.GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, MAX_MESSAGE_LENGTH );
								g_InterfaceManager.ShowMessageBox( szMessage, 3000, NULL, 0 );
								return;
							}
#else	
						
							if(g_slangNameFilter.IsHaveReplace(m_szGuildName)||GlobalFunc::CheckIncludeStringOnAbroad(m_szGuildName) )
							{
								//	���� �ܾ� ���
								g_InterfaceManager.GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, MAX_MESSAGE_LENGTH );
								g_InterfaceManager.ShowMessageBox( szMessage, 3000, NULL, 0 );
								return;
							}
#endif	//#if defined(__KR) || defined(_KOREA)

                        //	��带 â�� �Ͻðڽ��ϱ�?
                        const int	c_iCreateGuild = 70433;
                        g_InterfaceManager.GetInterfaceString( c_iCreateGuild, szMessage, INTERFACE_STRING_LENGTH );

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_Confirm(szMessage, guildcreateConfirm);

                    }
                }
                break;

            case DLGPOS_BTN_GUILD_CANCEL:
                {
					//	��� â���� ��� �ϼ̽��ϴ�.
					this->ShowInterface (FALSE);
                    GET_DIALOG( IM_GUILD_MANAGER::GUILD_MENU )->ShowInterface(TRUE);
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

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
