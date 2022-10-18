
#include "stdafx.h"

#include <ServerFrame.h>
#include "TempServerSession.h"
#include "MasterServerSession.h"
#include "ServerSessionManager.h"

#include <Information/ScriptLoadManager.h>
#include <Information/SolarAcceptableMasterServerListLoader.h>

TempServerSessionEx::TempServerSessionEx()
{
	Init();
}

TempServerSessionEx::~TempServerSessionEx()
{
	Release();
}

VOID		TempServerSessionEx::Init()
{
	TempServerSession::Init();

	m_Services.Init();
	m_Services.RegisterPacketHandler();

	m_dwPacketStrictSequence = FALSE;
	m_dwAccessCount = 0;
}

VOID	TempServerSessionEx::Release()
{
	Services().CRYPTINFO.Release();
	TempServerSession::Release();
}

VOID	TempServerSessionEx::MigrateObject( MasterServerSession* pTargetMaster )
{
	// ���ο� NetworkObject�� ��ü
	Redirect( pTargetMaster );

    DWORD session_index = this->GetSessionIndex();
    // ���ο� ���ǿ� ���� ����
    pTargetMaster->SetSessionIndex(session_index);

    GetFrame()->RemoveSession(session_index);
    MSG_SERVER_TYPE dummy;
    dummy.m_byCategory = 0;
    dummy.m_byProtocol = 0;
    pTargetMaster->OnRedirect(session_index, &dummy);

    // TempServer �����ϰ�
    Services().MigrateObject(pTargetMaster->Services());
    //GLOBAL_SERVER_KEY globalServerKey = Services().GetGlobalServerKey();
}

VOID	TempServerSessionEx::OnAccept( DWORD dwNetworkIndex )
{
	TempServerSession::OnAccept( dwNetworkIndex );
	ServerSessionManager::Instance()->AddServer( this );
}

VOID	TempServerSessionEx::OnDisconnect()
{
	ServerSessionManager* pManager = ServerSessionManager::Instance();
	SERVER_SESSION_DELEGATOR& rSession = pManager->FindServer( this );

	if( rSession == NULL )
	{
		MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
			"[TempServerSessionEx] �̹� ������ ���ῡ ���� ���Ÿ� �����Ϸ��� �ϰ� �ִ�. Idx[%d]",
			this->GetSessionIndex()
			);
		return;
	}

	pManager->RemoveServer( rSession );

	TempServerSession::OnDisconnect();
}

BOOL TempServerSessionEx::SendPacket( MSG_BASE* pMsg, WORD wSize )
{
	CryptFlowPolicy::MakePacket::PreSend_DefConverter(
		(BYTE*)pMsg,
		m_Services.GetServerIPAddress(),
		m_Services.GetSeqCryptKey()
		);

	return Send( (BYTE*)pMsg, wSize );
}

VOID TempServerSessionEx::OnRecv( BYTE* pMsg, WORD wSize )
{
	// SolarAuthServer�� �����ϴ� ������ Master������ �Ǵ��Ѵ�.
	// ������, ���� Master�������� �ƴ��� Ȯ���ϴ� ������ �䱸�ȴ�.
	// ��, TempServerSessionEx���� ������ ������ ä �������� ���� �� ������ ���� ������ �ʿ䰡 �ִ�.
	//
	// ������ ��Ŷ ������ ���ǵ� ���̴�. �̰� ���� ���� �� �ְ� �Ǹ�, �������� ������������,
	// ���Ȼ� ������ ����� �����̴�.
	//
	// ���������� ����ϴ� ����
	//    m_dwPacketStrictSequence : 0~ ���� ����ϴ� �ѵ� ������ �� ���� OnRecv�� ������ ���̴�.
	// �� �ʿ��� �κп� ���ؼ��� ���� ����� �� �����ϱ�� �Ѵ�.

	ScriptLoadManager* pScriptLoadManager = ScriptLoadManager::Instance();
	SolarAcceptableMasterServerListLoaderDelegator& rMasters =
		pScriptLoadManager->SolarAcceptableMasterServerListLoaderHandle;

	ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR& rMasterInfo =
		rMasters->GetMasterServerByIP( this->GetIPAddress() );

	if( rMasterInfo == NULL )
	{
		MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
				_T("�������� ���� �������� ������ ��û�߽��ϴ�. IP {%s}"), this->GetIP() );
		// ����� ������, �ణ�� �����Ⱓ�� �ѱ�?
		return;
	}

	DWORD		dwIPAddress = rMasterInfo->GetIPAddress();
	CRYPT_KEY_T cryptKey = rMasterInfo->GetCryptKey();

	MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
			_T("IP {%s} IPAddress [%08X] CryptKey [%08X]"), this->GetIP(), dwIPAddress, cryptKey );

	Services().SetServerIPAddress( rMasterInfo->m_dwNetworkIP_Server );
	// ���� ��ȣȭ���� ���� ��������

	if( AccessCount() != 0 )
	{
		DWORD dwSeqCryptKey = Services().GetSeqCryptKey();
		CryptFlowPolicy::MakePacket::PostRecv_DefConverter(
														pMsg,
														/*cryptKey,*/ Services().GetServerIPAddress(),
														dwSeqCryptKey
														);
	}


	Services().GetPacketHandler()->ParsePacket( TEMP_SERVER, this, (MSG_BASE*)pMsg, wSize );

//	//-------------------------------------------------------------------------------------
//	// m_dwPacketStrictSequence�� ��ġ�� ���� �ڵ鸵 ��å�� �����Ѵ�.
//	// <1> Switch by m_dwPacketStrictSequence
//	// <1-1> ���� ���� Ȯ��
//	// <1-2> ������ ���� �ִ� (����������) �ڽ��� Ű������ �ڽ��� �� ������ ��ȯ�� ����
//	// <1-3> ������ ������ ������ ���濡 ���� Ű������ ������ Ȯ��
//	//   <1-3-1> T : ������ �ŷ��� �� �ִ�, ��Ŷ��ȣȭ�� Ű ����, ��ȣȭ�� ��ġ�� �ڵ鷯 ���
//	//   <1-3-1> F : �ŷ��� �� ���� ���, �ϴ� ���� ������ �ϰ� ���� ��� �� ����͸��ϴ� ��å ����
//	// <1-4~> ACK, KEY, ACK�� ������ ���� Ű��ȯ�� �Ϸ��Ѵ�.
//	// <2> ���濡�� ������ �� �ִ� �ڵ鷯���� ����ϰ�, MasterServer�� Session��ȯ
//
//	// Ǯ�κ��� NetworkObject�� �޾� �´�.
//
//	MSG_SERVER_TYPE *pRecvMsg = (MSG_SERVER_TYPE *)pMsg;
//	SERVER_SESSION_DELEGATOR pNewSession = 
//		ServerSessionFactory::Instance()->AllocServerSession( (eSERVER_TYPE)pRecvMsg->m_byServerType );
//
//	if( pNewSession == NULL )
//	{
//		OnDisconnect();
//		return;
//	}
//	// ���ο� NetworkObject�� ��ü
//	Redirect( pNewSession() );
//
//	/*
//	// ���� ���� ���̺��� IP�� ���� Ÿ������ ������ ã�Ƽ� �� Ű�� �����Ѵ�.
//	DWORD dwServerKey = ServerInfoManager::Instance()->GetServerKey( (eSERVER_TYPE)pRecvMsg->m_byServerType, GetIP() );
//	pNewSession->SetServerKey( dwServerKey );
//	*/
//
//	// ���ο� ���ǿ� ���� ����
//	pNewSession->SetSessionIndex( GetSessionIndex() );
////	pNewSession->OnRedirect();
//
//	// TempServer �����ϰ�
//	GLOBAL_SERVER_KEY globalServerKey = this->GetGlobalServerKey();
//	this->Release();
//	ServerSessionManager::Instance()->RemoveServer( globalServerKey );
//	ServerSessionFactory::Instance()->FreeServerSession( this );
//
//	// ���� ����� ������ �Ŵ����� �߰�
//	ServerSessionManager::Instance()->AddServer( pNewSession );
}


