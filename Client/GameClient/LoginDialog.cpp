//------------------------------------------------------------------------------
//  LoginDialog.cc
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "SceneBase.h"
#include "LoginScene.h"
#include "InterfaceManager.h"
#include ".\logindialog.h"
#include <Protocol_CL.h>
#include <PacketStruct_CL.h>
#include "LoadScene.h"
#include "../../ProgramCommon/PacketCrypt.h"

#pragma  comment(lib, "../../ProgramCommon/CryptLib.lib")
//#define _AUTH_LOG_

WzID LoginDialog::m_wzId[LOGIN_MAX] = 
{ 
    StrToWzID("E000"),      // ���̵�
    StrToWzID("E001"),      // ��й�ȣ
    StrToWzID("TI00"),      // ���� ����
    StrToWzID("TI01"),      // �����ϱ�
    StrToWzID("TI02"),      // ����
};

//------------------------------------------------------------------------------
/**
*/
LoginDialog::LoginDialog()
{
}

//------------------------------------------------------------------------------
/**
*/
LoginDialog::~LoginDialog()
{
}

//------------------------------------------------------------------------------
/**
*/
VOID
LoginDialog::Init( CDrawBase * pDrawBase )
{
    m_pDraw = pDrawBase;
}


VOID LoginDialog::showWindow(BOOL val)
{
	if (val)
	{
		MoveWindowWZ( -1.0f, -1.0f );

		CCtrlDxIMEEditBox * pE = (CCtrlDxIMEEditBox *)getControl( LOGIN_ID );
		if (pE)
		{
			//pE->SetColorText( RGB( 255, 255, 255 ) );
			//pE->SetColorCaret( RGB( 255, 255, 255 ) );
			pE->SetFocusWZ();
			//		SetFocus( pE->GetEditHwnd() );
            pE->SetLimitText(9);
            pE->SetFontID("m212");
            pE->SetColorCaret(RGBA(255,100,100,255));
            pE->SetColorText(RGBA(255,255,255,255));
		}

		CCtrlDxIMEEditBox * pE2 = (CCtrlDxIMEEditBox *)getControl( LOGIN_PW );
		if (pE2)
		{
			//pE2->SetColorText( RGB( 255, 255, 255 ) );
			//pE2->SetColorCaret( RGB( 255, 255, 255 ) );
            pE2->SetLimitText(9);
            pE2->SetFontID("m212");
            pE2->SetColorCaret(RGBA(255,100,100,255));
            pE2->SetColorText(RGBA(255,255,255,255));
		}

		RECT rc;
		GetDialogWZ()->GetDlgSize(&rc);

		// GetDlgSize ���⼭ ���ϵǴ� RC�� Right, bottom�� width, height�� �׻� ����ְ�
		// left, top�� x, y ��ǥ�� ���Ѵ�.

		double width = rc.right;
		double height = rc.bottom;

		// TODO:
		// ��ü ȭ�� �ػ󵵸� �����ؼ� ���̾�α׸� ȭ�� �߾ӿ� ��ġ��Ų��.
		// ex) 1024x768

		double mw = width * 0.5f;
		double mh = height * 0.5f;

		RECT *rcWindow = g_InterfaceManager.GetWindowRC();

		long sw;
		long sh;
	/*	if (rcWindow)
		{
			sw = (rcWindow->right - rcWindow->left);
			sh = (rcWindow->bottom - rcWindow->top);
		}
		else*/
		{
			sw = 1024; sh = 768;
		}

		long pw = (sw * 0.5f) - mw;
		long ph = (sh * 0.5f) - mh;

		MoveWindowWZ(pw, ph);
	
//        SendMessage(g_hWndMain, WM_IME_SETCONTEXT, 0, 0);
    }
}

//------------------------------------------------------------------------------
/**
*/
VOID LoginDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
    case RT_MSG_DLGMOVE:
        {
        }
        break;

	case RT_MSG_EDIT_TABKEYDOWN:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case LOGIN_ID:
				{
					CCtrlDxIMEEditBox * pE = (CCtrlDxIMEEditBox *)getControl( LOGIN_PW );
					pE->SetFocusWZ();
				}
				break;
			case LOGIN_PW:
				{
					CCtrlDxIMEEditBox * pE = (CCtrlDxIMEEditBox *)getControl( LOGIN_ID );
					pE->SetFocusWZ();
				}
				break;
			}
		}
		break;

	case RT_MSG_EDIT_ENTERKEYDOWN:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case LOGIN_ID:
            case LOGIN_PW:
				{
					if ( GENERALPARAM->IsTryConnectAuthServer() )
					{
						this->execLoginQuery();
					}
					else
					{
						g_LoadScene.SetGoingToMap(GENERALPARAM->GetCharSelectMapID());
						g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_CHARSELECT);
						GameFramework::ChangeScene(SCENE_TYPE_LOAD);
					}
				}
				break;
			}
		}
		break;

	case RT_MSG_LBUTTONCLICK:
		{
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case LOGIN_CONNECT:
				{
                    if ( GENERALPARAM->IsTryConnectAuthServer() )
                    {
                        this->execLoginQuery();
                    }
                    else
                    {
                        g_LoadScene.SetGoingToMap(GENERALPARAM->GetCharSelectMapID());
                        g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_CHARSELECT);
                        GameFramework::ChangeScene(SCENE_TYPE_LOAD);
                    }
                }
                break;

            case LOGIN_EXIT:
                {
                    SendMessage(g_hWndMain, WM_CLOSE, 0, 0);
                }
                break;

            case LOGIN_REGISTER:
                {
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
void
LoginDialog::execLoginQuery(char *strID, char *strPW)
{
	TCHAR	szMessage[MAX_MESSAGE_LENGTH];


    char pszIdMsg[ID_MAX_LEN];
    char pszPwMsg[PASSWD_MAX_LEN];

    if (strID==NULL||strPW==NULL)
    {
        CCtrlDxIMEEditBox * pE = (CCtrlDxIMEEditBox *)getControl( LOGIN_ID );
        pE->GetText( pszIdMsg, 128 );
        if( strlen( pszIdMsg ) == 0 )
        {
			//	���̵� �Է� �ϼ���
            WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
			g_InterfaceManager.GetInterfaceString( eST_INPUT_ID, szMessage, MAX_MESSAGE_LENGTH );
            g_InterfaceManager.ShowMessageBox( szMessage, 10000, &Color );
            return;
        }

        CCtrlDxIMEEditBox * pE2 = (CCtrlDxIMEEditBox *)getControl( LOGIN_PW );
        pE2->GetText( pszPwMsg, 128 );
        if( strlen( pszPwMsg ) == 0 )
        {
			//	��й�ȣ�� �Է� �ϼ���
            WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
			g_InterfaceManager.GetInterfaceString( eST_INPUT_PASSWORD, szMessage, MAX_MESSAGE_LENGTH );
            g_InterfaceManager.ShowMessageBox( szMessage, 10000, &Color );
            return;
        }
    }
    else
    {
        strcpy(pszIdMsg, (const char *)strID);
        strcpy(pszPwMsg, (const char *)strPW);
    }

    if( GENERALPARAM->IsNet() )
    {
        if(!g_LoginScene.IsReadyForQuery())
        {
            if (g_LoginScene.IsConnectedToAuthServer())
            {
				//	�̹� �α��� �Ǿ� �ֽ��ϴ�
                WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
				g_InterfaceManager.GetInterfaceString( eST_ALREADY_LOGIN, szMessage, MAX_MESSAGE_LENGTH );
                g_InterfaceManager.ShowMessageBox( szMessage, 10000, &Color );
            }
            else
            {
				//	��� �� �ٽ� �õ� �Ͻʽÿ�
                WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
				g_InterfaceManager.GetInterfaceString( eST_RETRY_AFTER_WHILE, szMessage, MAX_MESSAGE_LENGTH );
                g_InterfaceManager.ShowMessageBox( szMessage, 10000, &Color );
            }
        }
        else
        {
            //
            // --> ���� ���� ��û
            //
            MSG_C2S_ASK_AUTH                SendPacket;
            SendPacket.m_byCategory         = CL_AUTH;
            SendPacket.m_byProtocol         = CL_AUTH_C2S_ASK_AUTH;

            SendPacket.dwAuthUserID = GENERALPARAM->GetUserID();
            sprintf (SendPacket.szID, "%s", pszIdMsg);
            sprintf (SendPacket.szPasswd, "%s", pszPwMsg);

#ifdef      _AUTH_LOG_
            int i;
            FILE *fp;
            fp = fopen("auth.log", "a");
            SYSTEMTIME st;
            GetLocalTime(&st);
            fprintf(fp, "\n==================================================\n");
            fprintf(fp, "Y/M/H: %d / %d / %d\n", st.wYear, st.wMonth, st.wDay);
            fprintf(fp, "H/M/S/MM: %d:%d:%d:%d\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
            fprintf(fp, "dwEncKey: %d\n", g_LoginScene.GetEncKey());
            fprintf(fp, "�Է� ��: %s\n", SendPacket.szPasswd);
#endif
            Crypt::PacketEncode( (unsigned char *)SendPacket.szPasswd, PASSWD_MAX_LEN, (unsigned char *)&SendPacket.szPasswd, g_LoginScene.GetEncKey() );

#ifdef      _AUTH_LOG_
            fprintf(fp, "��ȣȭ �� ��й�ȣ: %s\n", SendPacket.szPasswd);

            fprintf (fp, "[����] ��ȣȭ �� ��й�ȣ�� ���� HEX �ڵ�\n");
            for (i=0; i<PASSWD_MAX_LEN; ++i)
            {
                fprintf (fp, "%02d: %x\n", i, SendPacket.szPasswd[i] );
            }
            fprintf (fp, "[ ��] ��ȣȭ �� ��й�ȣ�� ���� HEX �ڵ�\n");
#endif
            g_LoginScene.checkSendPacket(sizeof(MSG_C2S_ASK_AUTH), (char *)&SendPacket);
            g_LoginScene.SetPendingConfrim(FALSE);
        
#ifdef      _AUTH_LOG_
            Crypt::PacketDecode( (unsigned char *)SendPacket.szPasswd, PASSWD_MAX_LEN, (unsigned char *)&SendPacket.szPasswd, g_LoginScene.GetEncKey() );
            fprintf(fp, "�ص� �� ��й�ȣ : %s\n", SendPacket.szPasswd);
            fprintf(fp, "==================================================\n");
            fclose (fp);
#endif
        }
    }
    else
    {
        g_LoadScene.SetGoingToMap( GENERALPARAM->GetCharSelectMapID() );
        g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_CHARSELECT);
        GameFramework::ChangeScene( SCENE_TYPE_LOAD );

        g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_LOGIN, FALSE);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
LoginDialog::makePasswordCryption(char *pw)
{
    unsigned char	source[128];
    unsigned char	output1[128];
    unsigned char	output2[128];
    
    sprintf ((char *)source, "%s", (unsigned char *)pw);
    source[4] = 0;

    Crypt::PacketEncode( source, 4, output1, g_LoginScene.GetEncKey() );
    Crypt::PacketDecode( output1, 4, output2, g_LoginScene.GetEncKey() );

    int len = sizeof(output1);
    memcpy (pw, output1, len );
}

//------------------------------------------------------------------------------
/**
*/
void
LoginDialog::Clear(eLOGIN_TXT_FIELD tf)
{
    switch (tf)
    {
    default:
    case TXT_FIELD_ALL:
        {
            CCtrlDxIMEEditBox * pE1 = (CCtrlDxIMEEditBox *)getControl( LOGIN_ID );
            pE1->ClearText();
            CCtrlDxIMEEditBox * pE2 = (CCtrlDxIMEEditBox *)getControl( LOGIN_PW );
            pE2->ClearText();
            
            pE1->SetFocusWZ();
        }
        break;
    case TXT_FIELD_ID:
        {
            CCtrlDxIMEEditBox * pE = (CCtrlDxIMEEditBox *)getControl( LOGIN_ID );
            pE->ClearText();

            pE->SetFocusWZ();
        }
        break;
    case TXT_FIELD_PW:
        {
            CCtrlDxIMEEditBox * pE = (CCtrlDxIMEEditBox *)getControl( LOGIN_PW );
            pE->ClearText();

            pE->SetFocusWZ();
        }
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
POSTYPE LoginDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < LOGIN_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return LOGIN_MAX;
}

//------------------------------------------------------------------------------
/**
*/
CControlWZ * LoginDialog::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < LOGIN_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}
