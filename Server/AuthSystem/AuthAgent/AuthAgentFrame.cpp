#include "stdafx.h"
#include ".\AuthAgentFrame.h"
#include <ServerSession.h>
#include "GameAgentManager.h"
#include "GameAgentInfoList.h"



NetworkObject*
AuthAgentFrame::DoAllocServerSession()
{
	return (NetworkObject*)AuthAgentFrame::Instance()->AllocServerSession(AGENT_SERVER);
}

VOID
AuthAgentFrame::DoFreeServerSession(NetworkObject* pNetObject)
{
	AuthAgentFrame::Instance()->FreeServerSession(static_cast<ServerSession*>(pNetObject));
}

VOID
AuthAgentFrame::DoFreeConnectSession(NetworkObject* pNetObject)
{
	//AuthAgentFrame::Instance()->FreeServerSession((ServerSession*)pNetObject);
}

AuthAgentFrame::AuthAgentFrame()
    : m_pAuthServerSession(NULL)
    , m_pGameAgentServerSession(NULL)
{

}

AuthAgentFrame::~AuthAgentFrame()
{

}

BOOL
AuthAgentFrame::Init()
{
	SUNLOG(eCRITICAL_LOG, "");

	if(!CreateSunLog())
	{
		DISPMSG(" Create SunLog Failed..");
		return FALSE;
	}

	m_GameAgentServerSessionPool.Initialize(50);
	m_AuthServerSessionPool.Initialize(10);

	GameAgentServerSession::SetPool(&m_GameAgentServerSessionPool);
	AuthServerSession::SetPool(&m_AuthServerSessionPool);

	GetReadyState().Init(AGENT_READY_MAX);

#ifdef _JP_0_20101123_PMS
    SERVER_KEY key = GetKey();
    key.Set(0,0,GetServerType(),0);
    SetKey(key);
#endif

	return TRUE;
}

BOOL
AuthAgentFrame::Update(DWORD tick)
{
	g_pSunLog->Update();

	return TRUE;
}

void
AuthAgentFrame::Release()
{
	SAFE_DELETE(g_pSunLog);

	GameAgentManager::DestroyInstance();
	GameAgentInfoList::DestroyInstance();
}

BOOL
AuthAgentFrame::SendAuthServer(MSG_BASE_INTERNAL* pMsg, WORD wSize)
{
	if(!m_pAuthServerSession)
        return FALSE;
	return m_pAuthServerSession->Send((BYTE*)pMsg, wSize);
}

BOOL
AuthAgentFrame::IsAuthServerConnected()
{
	if(m_pAuthServerSession && m_pAuthServerSession->IsConnected())
		return TRUE;
	return FALSE;
}

BOOL
AuthAgentFrame::SendGameAgentServer(MSG_BASE_INTERNAL* pMsg, WORD wSize)
{
	if(!m_pGameAgentServerSession)
        return FALSE;
	return m_pGameAgentServerSession->Send((BYTE*)pMsg, wSize);
}

BOOL
AuthAgentFrame::IsGameAgentConnected()
{
	if(m_pGameAgentServerSession && m_pGameAgentServerSession->IsConnected())
		return TRUE;
	return FALSE;
}


ServerSession*
AuthAgentFrame::AllocServerSession(eSERVER_TYPE type)
{
	ServerSession* pServerSession = NULL;
	switch(type)
	{
	case AUTH_SERVER:
		{
			pServerSession = AuthServerSession::ALLOC(); 
			m_pAuthServerSession = (AuthServerSession*)pServerSession;
		}
		break;
	case AGENT_SERVER:
		{
			pServerSession = GameAgentServerSession::ALLOC(); 
			m_pGameAgentServerSession = (GameAgentServerSession*)pServerSession;
		}
        break;
	default:
        {
            ASSERT(!"존재하지 않는 서버타입입니다.");
            return NULL;
        }
	}

	pServerSession->Init(this);
	return pServerSession;
}

void
AuthAgentFrame::FreeServerSession(ServerSession* pSession)
{
	pSession->Release();
	switch(pSession->GetServerType())
	{
	case AUTH_SERVER:
		{
			AuthServerSession::FREE((AuthServerSession*)pSession); 
			m_pAuthServerSession = NULL;
		}
        break;
	case AGENT_SERVER:
		{
			GameAgentServerSession::FREE((GameAgentServerSession*)pSession); 
			m_pGameAgentServerSession = NULL;
		}
        break;
	default:
		{
			ASSERT(!"서버타입이 존재하지 않습니다.");
		}
        break;
	}
}

BOOL
AuthAgentFrame::KeyEvent(char chr)
{
	switch(chr)
	{
	case ' ': 
		DisplayServerInfo();
		break;
	case 'S':
		GameAgentInfoList::Instance()->Display();
		break;
	}

	return TRUE;
}

VOID
AuthAgentFrame::DisplayServerInfo()
{

}

ServerSession*
AuthAgentFrame::GetSession(eSERVER_TYPE type)
{
	switch(type)
	{
	case AUTH_SERVER:
		return m_pAuthServerSession;
	case AGENT_SERVER:
		return m_pGameAgentServerSession;
	}
	return NULL;
}

BOOL
AuthAgentFrame::ConnectTo(eSERVER_TYPE type)
{
	ServerSession* pSession = NULL;
	switch(type)
	{
	case AUTH_SERVER:
		pSession = m_pAuthServerSession;
        break;
	case AGENT_SERVER:
		pSession = m_pGameAgentServerSession;
        break;
	}

	if(pSession == 0)
        return FALSE;

	if(pSession->IsConnecting() || pSession->IsConnected())
	{
		ASSERT(!"중복 Connect 시도");
		return FALSE;
	}

	Connect(pSession);

	return TRUE;
}

BOOL
AuthAgentFrame::CreateSunLog()
{
	BYTE bySunLogOption = 0,
         bySunLogFileLevel = 0;
	TCHAR szSunLogFilePath[256];

    const char* config_file = GetDescFileName();
	__READ_CONFIG_NUMBER(bySunLogOption, "LOGFILE", "SUN_LOGFILE_OPTION", 0, config_file);
	__READ_CONFIG_NUMBER(bySunLogFileLevel, "LOGFILE", "SUN_LOGFILE_LEVEL", 0, config_file);
	__READ_CONFIG_STRING(szSunLogFilePath, "LOGFILE", "SUN_LOGFILE_PATH", "", config_file);

	g_pSunLog = new CSunLog;
	if(!g_pSunLog->Init(bySunLogOption, bySunLogFileLevel, szSunLogFilePath, "AuthAgent"))
		return FALSE;

	g_pSunLog->SetFileOutDelayOption(FALSE);

	return TRUE;
}

















