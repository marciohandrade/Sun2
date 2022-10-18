#ifndef __SERVICES4SESSION_H__
#define __SERVICES4SESSION_H__

#pragma once

#include <ServerBuildVersion.h>
#include "PacketHandler/PacketHandler.h"

#include <WorkLoad/WorkLoadPacketStream.h>

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
		//CRYPTINFO.Init();
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
	//VOID						MigrateObject( Services4Session& rTargetService )

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

private:
	WorkLoadPacketStream				m_WorkLoadPacketStream;
	WORK_LOAD_CONTAINER_SESSION_WORK	m_WorkLoad;
};

//
//
//

#include <SolarAuth.CryptFlowPolicy.h>

//// Cryptography for ServerRole
//struct sCRYPTINFO
//{
//	// <FIELDs>
//	BOOL					m_bIsCrypted;
//	ClientSNKey				m_RandomSNK;
//	CryptManagerS2S*		m_pCryptManager;
//
//	// <METHODs>
//	VOID					Init() { m_bIsCrypted = FALSE; m_pCryptManager = NULL; }
//	VOID					Release()
//	{
//		SAFE_DELETE(m_pCryptManager);
//		m_bIsCrypted = FALSE;
//	}
//	BOOL&					IsCrypted() { return m_bIsCrypted; }
//	ClientSNKey&			RandomSNK() { return m_RandomSNK; }
//	CryptManagerS2S&		CryptManager() { return *m_pCryptManager; }
//
//	VOID					NextRoundSNKey4Send() { m_RandomSNK.NextRoundSNKey4Send(); };
//	VOID					NextRoundSNKey4Recv() { m_RandomSNK.NextRoundSNKey4Recv(); };
//	BYTE					GetSendSNK()		{ return m_RandomSNK.GetSendSNKey( RandomSNKey::c_dwBaseKey ); };
//	BYTE					GetSendPairSNK()	{ return m_RandomSNK.GetSendSNKey( RandomSNKey::c_dwPairKey ); };
//	BYTE					GetRecvSNK()		{ return m_RandomSNK.GetRecvSNKey( RandomSNKey::c_dwBaseKey ); };
//	BYTE					GetRecvPairSNK()	{ return m_RandomSNK.GetRecvSNKey( RandomSNKey::c_dwPairKey ); };
//};


#endif //__SERVICES4SESSION_H__


