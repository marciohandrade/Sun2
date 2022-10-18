#include "SunClientPrecompiledHeader.h"
#include "uiWebDialogMan.h"
#include "uiWebDialog/uiWebDialog.h"
#include "globalfunc.h"
#include "interfacemanager.h"
#ifdef _KIKI_UI_WEB_DIALOG

//------------------------------------------------------------------------------
/**
*/
uiWebDialogMan::uiWebDialogMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
    m_pUIWebDialog = NULL;
	
	ZeroMemory(m_pURL, sizeof(m_pURL));
	ZeroMemory(m_pLoginURL, sizeof(m_pLoginURL));

	ZeroMemory(m_pAuthURL, sizeof(m_pAuthURL));
	ZeroMemory(m_pGLOGURL, sizeof(m_pGLOGURL));
	ZeroMemory(m_pUserURL, sizeof(m_pUserURL));

	ZeroMemory(m_szAuthURL, sizeof(m_szAuthURL));
	ZeroMemory(m_szUserURL, sizeof(m_szUserURL));

	_loadURL();
}

//------------------------------------------------------------------------------
/**
*/
void uiWebDialogMan::_loadURL()
{
	ZeroMemory(m_szAuthURL, sizeof(m_szAuthURL));
	ZeroMemory(m_szUserURL, sizeof(m_szUserURL));

	TCHAR szGlogURL[MAX_PATH] ={0,};

	// 996	http://betaglog.sunonline.co.kr/Game/checkLogin.asp?sID=
	g_InterfaceManager.GetInterfaceString(996, m_szAuthURL, MAX_PATH);
	
	// 995	http://betaglog.sunonline.co.kr/Game/goGlog.asp?bTP=1
	g_InterfaceManager.GetInterfaceString(995, szGlogURL, MAX_PATH);
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szGlogURL, _tcslen(szGlogURL), m_pGLOGURL, MAX_PATH );
	
	// 994	http://betaglog.sunonline.co.kr/Game/goGlog.asp?bTP=2&sID=
	g_InterfaceManager.GetInterfaceString(994, m_szUserURL, MAX_PATH);
}

void uiWebDialogMan::OnInitialize()
{
    _LoadUI();
}

//------------------------------------------------------------------------------
/**
*/
void uiWebDialogMan::OnRelease()
{
    for (int i=UID_BEGIN; i<UID_END; ++i)
    {
        this->UnloadUI(i);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiWebDialogMan::ToggleWebDialog( void )
{
	if( m_pUIWebDialog )
	{
		
		if(m_pUIWebDialog->IsVisibleWindow())
		{
			m_pUIWebDialog->ShowInterface( FALSE );
		}
		else
		{
			RequestGLOG();
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiWebDialogMan::SetURL( WCHAR * pURL )
{
	if( pURL )
	{
		wcscpy( m_pURL, pURL );
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiWebDialogMan::SetURL( char * pURL )
{
	if( pURL )
	{
		ConvertToUNICODE( pURL , m_pURL);
	}
}

#ifdef _KIKI_WEB_AUTH	
//------------------------------------------------------------------------------
/**
*/
void uiWebDialogMan::RequestLogin(TCHAR* pID, char* pszGLogKey)
{
	// �� ������ ��û�Ѵ�
	ZeroMemory(m_pAuthURL, sizeof(m_pAuthURL));


	//  ������ URL ����
	TCHAR szURL[MAX_PATH] = {0,};
	Snprintf(szURL, MAX_PATH-1, _T("%s%s&sAK=%s"), m_szAuthURL, pID, pszGLogKey);

	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szURL, _tcslen(szURL), m_pAuthURL, MAX_PATH );

	m_pUIWebDialog->Navigate( m_pAuthURL, FALSE );	// ������� ����� ����, ������ ���¸� �Ѵ�.


	
}

#endif

//------------------------------------------------------------------------------
/**
*/
void uiWebDialogMan::RequestGLOG()
{
	m_pUIWebDialog->Navigate( m_pGLOGURL, TRUE );
}

//------------------------------------------------------------------------------
/**
*/
void uiWebDialogMan::RequestBLOG(TCHAR* pCharID)
{
	// ���� ��α� URL ����
	TCHAR szURL[MAX_PATH] = {0,};

	Snprintf(szURL, MAX_PATH-1, _T("%s%s"), m_szUserURL, pCharID);

	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szURL, _tcslen(szURL), m_pUserURL, MAX_PATH );

	m_pUIWebDialog->Navigate( m_pUserURL, TRUE );

	
}

//------------------------------------------------------------------------------
/**
*/
void uiWebDialogMan::RequestUserID(TCHAR* pCharName)
{
	BOOL bRet = FALSE;

	MSG_CW_COMM_USERID_REQ sync;
			
	StrnCopy( sync.m_ptszCharName, pCharName, MAX_CHARNAME_LENGTH );

	bRet = GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof(sync));

	if(!bRet)
	{
		TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
		
#ifdef _DEBUG
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, _T("ERR SendPacket: MSG_CW_COMM_USERID_REQ"));
#endif
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiWebDialogMan::NetworkProcWorld( MSG_BASE * pMsg )
{
	TCHAR   szURL[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szWideString[20] = {0,};
	TCHAR	szWideStringID[ID_MAX_LEN+1] = {0,};

    switch (pMsg->m_byCategory)
    {
    case CW_COMMUNITY:
        {
            switch (pMsg->m_byProtocol)
            {
			case CW_COMM_USERID_ACK:
				{
					MSG_CW_COMM_USERID_ACK *pRecvPacket = (MSG_CW_COMM_USERID_ACK *)pMsg;

					StrnCopy( szWideString, pRecvPacket->m_ptszCharName, sizeof(pRecvPacket->m_ptszCharName));
					StrnCopy( szWideStringID, pRecvPacket->m_ptszAccountID, sizeof(pRecvPacket->m_ptszAccountID));

					szWideString[MAX_CHARNAME_LENGTH] = NULL;
					szWideStringID[ID_MAX_LEN] = NULL;

					if(_tcslen(szWideStringID))
					{
						RequestBLOG(szWideStringID);
					}
					else
					{
						// �ش� ������ ������ ������ �� �����ϴ�. 5214
						const int c_iSvrErr = 5214;
						GetUIMan()->GetInterfaceString( c_iSvrErr, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
#ifdef _DEBUG
					Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("CW_COMM_USERID_ACK [%s][%s]"), szWideString, szWideStringID);
					GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, szMessage);
#endif
				}
				break;

			case CW_COMM_USERID_NAK:
				{
					MSG_CW_COMM_USERID_NAK	*pRecvPacket = (MSG_CW_COMM_USERID_NAK*)pMsg;
					if(pRecvPacket->m_wErrorCode == MSG_CW_COMM_USERID_NAK::_ERR_NOTCONNUSER)
					{
						// ���� ���� �ƴ� ����� ���� ��û
						const int	c_iNotFoundUser = 70262;
						GetUIMan()->GetInterfaceString( c_iNotFoundUser, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
					else
					{
						// �˼����� ����
						// �ش� ������ ������ ������ �� �����ϴ�.
						const int c_iSvrErr = 5214;
						GetUIMan()->GetInterfaceString( c_iSvrErr, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}

#ifdef _DEBUG
					Snprintf(szMessage, INTERFACE_STRING_LENGTH-1,_T("CW_COMM_USERID_NAK [%d]"), pRecvPacket->m_wErrorCode);
					GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, szMessage);
#endif
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
void uiWebDialogMan::NetworkProc( MSG_BASE * pMsg )
{
	
}

#endif