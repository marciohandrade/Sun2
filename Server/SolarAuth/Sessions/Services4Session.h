#ifndef __SERVICES4SESSION_H__
#define __SERVICES4SESSION_H__

#pragma once

#include <ServerBuildVersion.h>

#include <PacketStruct_SM.h>
#include "PacketHandler/PacketHandler.h"

#include <WorkLoad/WorkLoadPacketStream.h>
#include <SolarAuth.CryptFlowPolicy.h>

class Services4Session
{
public:
	inline VOID					Init()
	{
		m_PHASE_SHIFT = PHASE_NOACTION;
		m_SyncKey = TEMP_SERVER;
		m_dwSeqCryptKey = 0;
		m_dwServerIPAddress = 0;
		m_GlobalServerKey = 0;

		//---------------------------------
		// <Cryptography>
		CRYPTINFO.Init();
		//---------------------------------

		m_WorkLoadPacketStream.Initialize( WorkLoadPacketStream::c_DefaultUpdateTick, WorkLoadPacketStream::c_DefaultLimitTransferSize );
		m_WorkLoad.Initialize();
	}

	inline VOID					SetSyncKey( eSERVER_TYPE syncKey ) { m_SyncKey = syncKey; }
	inline eSERVER_TYPE			GetSyncKey() { return m_SyncKey; }

	inline VOID					SetSeqCryptKey( DWORD dwSeqCryptKey ) { m_dwSeqCryptKey = dwSeqCryptKey; }
	inline DWORD				GetSeqCryptKey() { return m_dwSeqCryptKey; }

	inline VOID					SetServerIPAddress( DWORD dwServerIPAddress ) { m_dwServerIPAddress = dwServerIPAddress; }
	inline DWORD				GetServerIPAddress() { return m_dwServerIPAddress; }

	inline GLOBAL_SERVER_KEY	GetGlobalServerKey() { return m_GlobalServerKey; }
	inline VOID					SetGlobalServerKey( GLOBAL_SERVER_KEY globalKey ) { m_GlobalServerKey = globalKey; }

	inline ePHASE_SHIFT			GetPhaseStatus() { return m_PHASE_SHIFT; }
	inline VOID					SetPhaseStatus( ePHASE_SHIFT phase ) { m_PHASE_SHIFT = phase; }

	WORK_LOAD_CONTAINER_SESSION_WORK&	GetWorkLoadContainer() { return m_WorkLoad; }
	WorkLoadPacketStream&				GetWorkLoadPacketStream() { return m_WorkLoadPacketStream; }

	PACKET_HANDLER_DELEGATOR	GetPacketHandler() { return *m_pPacketHandler; };

	// <INTERFACE>
	VOID						RegisterPacketHandler();
	VOID						MigrateObject( Services4Session& rTargetService )
	{
		//rTargetService.m_pPacketHandler
		rTargetService.m_PHASE_SHIFT = m_PHASE_SHIFT;
		rTargetService.m_SyncKey = MASTER_SERVER; //m_SyncKey;
		rTargetService.m_dwSeqCryptKey = m_dwSeqCryptKey;
		rTargetService.m_dwServerIPAddress = m_dwServerIPAddress;
		rTargetService.m_GlobalServerKey = m_GlobalServerKey;

		memcpy( &(rTargetService.CRYPTINFO), &CRYPTINFO, sizeof(CRYPTINFO) );
		CRYPTINFO.ClearField_CryptManager();
		CRYPTINFO.Release();
	}

	// <INTERFACEs><CRYPTOGRAPHY>
	struct Crypt
	{
		inline static VOID		Init_ServerRoleManager( Services4Session& rService, MSG_SM_CRYPTOGRAPHY_KEY* pMsgKEY );
		inline static VOID		StartPacketCrypt( Services4Session& rService );
		//
		inline static VOID		ProcPacket4Send();
		inline static VOID		ProcPacket4Recv();
	};
private:
	//-------------------------------------------------------------------------------------------------
	// Connection Info & Status
	//
	PACKET_HANDLER_DELEGATOR_P	m_pPacketHandler;
	ePHASE_SHIFT				m_PHASE_SHIFT;
	eSERVER_TYPE				m_SyncKey;
	DWORD						m_dwSeqCryptKey;
	DWORD						m_dwServerIPAddress;
	GLOBAL_SERVER_KEY			m_GlobalServerKey;

public:
	// Cryptography for ServerRole
	class sCRYPTINFO
	{
		BOOL					m_bIsCrypted;
		ServerSNKey				m_RandomSNK;
		CryptManagerS2S*		m_pCryptManager;

	public:
		VOID					Init() { m_bIsCrypted = FALSE; m_pCryptManager = NULL; }
		VOID					Release()
		{
			SAFE_DELETE(m_pCryptManager);
			m_bIsCrypted = FALSE;
		}
		VOID					ClearField_CryptManager() { m_pCryptManager = NULL; }
		BOOL&					IsCrypted() { return m_bIsCrypted; }
		ServerSNKey&			RandomSNK() { return m_RandomSNK; }
		CryptManagerS2S&		CryptManager() { return *m_pCryptManager; }

		VOID					NextRoundSNKey4Send() { m_RandomSNK.NextRoundSNKey4Send(); };
		VOID					NextRoundSNKey4Recv() { m_RandomSNK.NextRoundSNKey4Recv(); };
		BYTE					GetSendSNK()		{ return m_RandomSNK.GetSendSNKey( RandomSNKey::c_dwBaseKey ); };
		BYTE					GetSendPairSNK()	{ return m_RandomSNK.GetSendSNKey( RandomSNKey::c_dwPairKey ); };
		BYTE					GetRecvSNK()		{ return m_RandomSNK.GetRecvSNKey( RandomSNKey::c_dwBaseKey ); };
		BYTE					GetRecvPairSNK()	{ return m_RandomSNK.GetRecvSNKey( RandomSNKey::c_dwPairKey ); };

		friend struct Crypt;
	} CRYPTINFO;

private:
	WorkLoadPacketStream				m_WorkLoadPacketStream;
	WORK_LOAD_CONTAINER_SESSION_WORK	m_WorkLoad;
};



VOID Services4Session::Crypt::Init_ServerRoleManager( Services4Session& rService, MSG_SM_CRYPTOGRAPHY_KEY* pMsgKEY )
{
	const DWORD dwRndKeyIncValue = 0x0082A141;
	static DWORD dwRndKeyInc = 1;

	sCRYPTINFO& rCRYPT = rService.CRYPTINFO;

	DWORD dwDefRndKey = CryptFlowPolicy::Utility::GetChunkValue();
	DWORD dwRndUsingKey = (dwRndKeyIncValue*(dwRndKeyInc++)) ^ dwDefRndKey;

	rCRYPT.RandomSNK().Initialize( dwRndUsingKey, 7, 4 /*C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO*/ );

	rCRYPT.m_pCryptManager = new CryptManagerS2S();
	rCRYPT.CryptManager().Initialize( CryptManagerS2S::ROLE_SERVER );

	BYTE bySNK = rCRYPT.GetSendSNK();
	pMsgKEY->SNKey() = bySNK;

	BYTE byCryptInfo =
		rCRYPT.CryptManager().CreateCryptKeyStream(
									rCRYPT.GetSendPairSNK(),
									(CRYPT_KEY_STREAM)pMsgKEY->GetCryptStream(),
									dwRndUsingKey
									);

	pMsgKEY->CryptInfo() = byCryptInfo;

	// 암호화 되어 있는 상태일 수도 있다... SendPacket에서는 암호화 유무에 따라서 전송하는 루틴으로 설정

	// 아래 루틴은 Init을 호출하는 쪽에서 처리한다.
	//pServerSession->SendPacket( pMsgKEY, sizeof(*pMsgKEY) );
	//rCRYPT.NextRoundSNKey4Send();
}

VOID Services4Session::Crypt::StartPacketCrypt( Services4Session& rService )
{
	sCRYPTINFO& rCRYPT = rService.CRYPTINFO;

	if( rCRYPT.CryptManager().StartLogic( TRUE ) )
	{
		rCRYPT.IsCrypted() = TRUE;
		return;
	}

	ASSERT( !"패킷 암호화 시작 실패" );
}

VOID Services4Session::Crypt::ProcPacket4Send()
{
}

VOID Services4Session::Crypt::ProcPacket4Recv()
{
}


#endif // __SERVICES4SESSION_H__

