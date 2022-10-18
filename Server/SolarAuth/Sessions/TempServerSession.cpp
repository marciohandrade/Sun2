
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
	// 새로운 NetworkObject로 교체
	Redirect( pTargetMaster );

    DWORD session_index = this->GetSessionIndex();
    // 새로운 세션에 정보 복사
    pTargetMaster->SetSessionIndex(session_index);

    GetFrame()->RemoveSession(session_index);
    MSG_SERVER_TYPE dummy;
    dummy.m_byCategory = 0;
    dummy.m_byProtocol = 0;
    pTargetMaster->OnRedirect(session_index, &dummy);

    // TempServer 정리하고
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
			"[TempServerSessionEx] 이미 해제한 연결에 대한 제거를 수행하려고 하고 있다. Idx[%d]",
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
	// SolarAuthServer에 접속하는 서버는 Master서버라 판단한다.
	// 하지만, 허용된 Master서버인지 아닌지 확인하는 절차가 요구된다.
	// 즉, TempServerSessionEx으로 접속을 유지한 채 여러가지 질의 및 응답을 통해 인증할 필요가 있다.
	//
	// 엄격한 패킷 순서가 정의될 것이다. 이것 저것 들어올 수 있게 되면, 자율성은 높아지겠지만,
	// 보안상 헛점이 생기기 마련이다.
	//
	// 일차적으로 사용하는 것은
	//    m_dwPacketStrictSequence : 0~ 으로 허용하는 한도 내에서 이 값은 OnRecv시 증가될 것이다.
	// 더 필요한 부분에 대해서는 좀더 고려한 뒤 결정하기로 한다.

	ScriptLoadManager* pScriptLoadManager = ScriptLoadManager::Instance();
	SolarAcceptableMasterServerListLoaderDelegator& rMasters =
		pScriptLoadManager->SolarAcceptableMasterServerListLoaderHandle;

	ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR& rMasterInfo =
		rMasters->GetMasterServerByIP( this->GetIPAddress() );

	if( rMasterInfo == NULL )
	{
		MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
				_T("인증되지 않은 서버에서 접속을 요청했습니다. IP {%s}"), this->GetIP() );
		// 끊어내야 하지만, 약간의 유예기간을 둘까?
		return;
	}

	DWORD		dwIPAddress = rMasterInfo->GetIPAddress();
	CRYPT_KEY_T cryptKey = rMasterInfo->GetCryptKey();

	MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
			_T("IP {%s} IPAddress [%08X] CryptKey [%08X]"), this->GetIP(), dwIPAddress, cryptKey );

	Services().SetServerIPAddress( rMasterInfo->m_dwNetworkIP_Server );
	// 아직 암호화되지 않은 접속절차

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
//	// m_dwPacketStrictSequence의 수치에 따른 핸들링 정책을 정의한다.
//	// <1> Switch by m_dwPacketStrictSequence
//	// <1-1> 접속 정보 확인
//	// <1-2> 상대방이 갖고 있는 (개별적으로) 자신의 키정보를 자신이 준 정보로 변환해 전송
//	// <1-3> 상대방이 보내는 정보를 상대방에 대한 키정보로 복원해 확인
//	//   <1-3-1> T : 상대방을 신뢰할 수 있다, 패킷암호화용 키 전송, 암호화를 거치는 핸들러 등록
//	//   <1-3-1> F : 신뢰할 수 없는 대상, 일단 끊는 것으로 하고 이후 블록 및 모니터링하는 정책 설정
//	// <1-4~> ACK, KEY, ACK의 과정을 거쳐 키교환을 완료한다.
//	// <2> 상대방에게 보내줄 수 있는 핸들러들을 허용하고, MasterServer로 Session전환
//
//	// 풀로부터 NetworkObject를 받아 온다.
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
//	// 새로운 NetworkObject로 교체
//	Redirect( pNewSession() );
//
//	/*
//	// 서버 정보 테이블에서 IP와 서버 타입으로 서버를 찾아서 그 키를 세팅한다.
//	DWORD dwServerKey = ServerInfoManager::Instance()->GetServerKey( (eSERVER_TYPE)pRecvMsg->m_byServerType, GetIP() );
//	pNewSession->SetServerKey( dwServerKey );
//	*/
//
//	// 새로운 세션에 정보 복사
//	pNewSession->SetSessionIndex( GetSessionIndex() );
////	pNewSession->OnRedirect();
//
//	// TempServer 정리하고
//	GLOBAL_SERVER_KEY globalServerKey = this->GetGlobalServerKey();
//	this->Release();
//	ServerSessionManager::Instance()->RemoveServer( globalServerKey );
//	ServerSessionFactory::Instance()->FreeServerSession( this );
//
//	// 실제 사용할 세션을 매니저에 추가
//	ServerSessionManager::Instance()->AddServer( pNewSession );
}


