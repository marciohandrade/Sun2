// embedded compile  "MasterServerInfo.h"
#include <Sessions/TempServerSessionEx.h>

struct sTEMP_SERVER_LIFETIME
{
	DWORD					dwTimeout;
	TempServerSessionEx*	pSession;
};

typedef std::map<DWORD64, sTEMP_SERVER_LIFETIME>	WAITED_SESSION_AUTHENTICATION_MAP;
typedef WAITED_SESSION_AUTHENTICATION_MAP::iterator	WAITED_SESSION_AUTHENTICATION_MAP_IT;
typedef std::pair<DWORD64, sTEMP_SERVER_LIFETIME>	WAITED_SESSION_AUTHENTICATION_PAIR;

// �ٸ� ��忡�� ���񽺷� ��ȯ�Ǿ��� ��, �۾��� ����
class PROCESS_SERVER_AUTHENTICATION : public IWorkNode
{
public:
	PROCESS_SERVER_AUTHENTICATION() {}
	~PROCESS_SERVER_AUTHENTICATION() {}

public:
	// <INTERFACE><IMPLEMENTATION>
	virtual BOOL	DoInit()
	{
		m_dwTimeout = 0;
		m_WaitedList.clear();
		return TRUE;
	}

	virtual BOOL	DoProcess()
	{
		DWORD dwCurTick = GetTickCount();
		if( !(m_dwTimeout < dwCurTick) )
			return TRUE;

		BOOL bIsBusyUpdate = FALSE;

		WAITED_SESSION_AUTHENTICATION_MAP_IT it(m_WaitedList.begin());
		for( ; it != m_WaitedList.end() ; ++it )
		{
			sTEMP_SERVER_LIFETIME& rLIFETIME = it->second;
			if( rLIFETIME.dwTimeout < dwCurTick )
			{
				TempServerSessionEx* pSession = rLIFETIME.pSession;
				if( pSession && pSession->IsConnected() )
					pSession->Disconnect( TRUE );

				m_WaitedList.erase( it );
				bIsBusyUpdate = TRUE;
				break;
			}
		}

		if( bIsBusyUpdate )
			m_dwTimeout = dwCurTick + c_BusyUpdatePeriod;
		else
			m_dwTimeout = dwCurTick + c_CheckPeriod;

		return TRUE;
	}

	virtual BOOL	IsComplete()
	{
		return FALSE;
	}

	virtual BOOL	DoRelease()
	{
		return TRUE;
	}

	virtual VOID	OnMsg( sSIG_MSG* pSIG_MSG );

private:
	static const DWORD					c_CheckPeriod = 7000;
	static const DWORD					c_BusyUpdatePeriod = 1;
	DWORD								m_dwTimeout;
	WAITED_SESSION_AUTHENTICATION_MAP	m_WaitedList;
};

// Disconnect�� �ش� ���� �����ϱ� ���� ��
VOID	PROCESS_SERVER_AUTHENTICATION::OnMsg( sSIG_MSG* pSIG_MSG )
{
	if( !pSIG_MSG || eSIGMSG::SERVERAUTHENTICATION != pSIG_MSG->_TYPE )
		return;

	sSIG_SERVER_AUTHENTICATION* pSIGNAL = (sSIG_SERVER_AUTHENTICATION*)pSIG_MSG;

	switch( pSIGNAL->m_dwSubType )
	{
	case pSIGNAL->REGISTERATION:
		{
			if( !pSIGNAL->m_pServerSession ) break;

			DWORD64 dw64Key = pSIGNAL->m_pServerSession->GetIPAddress();
			dw64Key |= ( ((DWORD64)pSIGNAL->m_pServerSession->TempServerKey())<<32 );

			WAITED_SESSION_AUTHENTICATION_MAP_IT it = m_WaitedList.find( dw64Key );
			if( it != m_WaitedList.end() )
			{
				ASSERT( !"����ȭ ������ ������ ����" );
				m_WaitedList.erase( it );
			}

			sTEMP_SERVER_LIFETIME sINFO;
			sINFO.dwTimeout = GetTickCount() + 25000;	//���� �ȵ� ���·� ����� �� �ִ� �ð�
			sINFO.pSession = pSIGNAL->m_pServerSession;

			m_WaitedList.insert( m_WaitedList.end(), std::make_pair( dw64Key, sINFO ) );
		}
		break;
	case pSIGNAL->UNREGISTRATION:
		{
			if( !pSIGNAL->m_pServerSession ) break;

			// Session���� ã�� �� �ִ� ��찡 ������ ������, ���� ���� �ִ�. ������ ���
			// ����, (IP|ServerKey)�� ã��� �Ѵ�.
			DWORD64 dw64Key = pSIGNAL->m_pServerSession->GetIPAddress();
			dw64Key |= ( ((DWORD64)pSIGNAL->m_pServerSession->TempServerKey())<<32 );

			if( !m_WaitedList.erase( dw64Key ) )
			{
				ASSERT( !"PROCESS_SERVER_AUTHENTICATION::OnMsg ����..." );
				// �̹� ������ ������ ���̴�.
				return;
			}
		}
		break;
	case pSIGNAL->AUTHED_RELEASE:
		{
			// Session���� ã�� �� �ִ� ��찡 ������ ������, ���� ���� �ִ�. ������ ���
			// ����, (IP|ServerKey)�� ã��� �Ѵ�.
			DWORD64 dw64Key = pSIGNAL->m_dwInnerIP;
			SERVER_KEY skServerKey = pSIGNAL->m_dwServerKey;
			dw64Key |= ( ((DWORD64)skServerKey)<<32 );

			WAITED_SESSION_AUTHENTICATION_MAP_IT it = m_WaitedList.find(dw64Key);
			if( it == m_WaitedList.end() )
			{
				ASSERT( !"PROCESS_SERVER_AUTHENTICATION::OnMsg ����..." );
				// �̹� ������ ������ ���̴�.
				return;
			}

			sTEMP_SERVER_LIFETIME& rPair = it->second;
			TempServerSessionEx* pTempSession = rPair.pSession;

			MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
			ServerSessionEx* pNewSession = (ServerSessionEx*)ServerSessionManager::AllocNewSession( (eSERVER_TYPE)skServerKey.GetServerType() );
			((ServerSession*)pNewSession)->Init( pMasterServer );

			// Copy Session Information
			pNewSession->SetSessionIndex( pTempSession->GetSessionIndex() );
			pNewSession->SetServerKey( skServerKey );

			// ���ο� NetworkObject�� ��ü
			pTempSession->Redirect( pNewSession );
			TempServerSessionEx::ReleaseProcess( pTempSession );

            ;{  // CHANGES: f110617.1L
                MSG_SERVER_TYPE msg_server_type;
                msg_server_type.m_byServerType = static_cast<BYTE>(pNewSession->GetServerType());
                pNewSession->OnRedirect(pTempSession->GetSessionIndex(), &msg_server_type);
            };
			// ���� ����� ������ �Ŵ����� �߰�
			ServerSessionManager::AddServer( pNewSession );
			MessageOut( eCRITICAL_LOG,
				_T("Add Server (%u/%u/%u/%u)"),
				skServerKey.GetWorldID(), 
				skServerKey.GetChannelID(),
				skServerKey.GetServerType(),
				skServerKey.GetServerID() );

			m_WaitedList.erase( dw64Key );

#	ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
			if( ScriptDataManager::IsAllDataLoaded() )
			{
				if( pMasterServer->GetPhaseStatus() == PHASE_SERVICE )
				{
					Services4Session& rSERVICE = pNewSession->Services();
					rSERVICE.SetPhaseStatus( PHASE_EXCHANGE );
					ServerSessionManager::SEND_PHASE_SHIFT_CMD( pNewSession, PHASE_EXCHANGE, PHASE_NOACTION );
				}
			}
#	endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		}
		break;
	default:
		{
			ASSERT( !"�̷� �� �ͼ� �ȵ�¡2~" );
		}
		return;
	}
}

//
//
//
//
//
//



