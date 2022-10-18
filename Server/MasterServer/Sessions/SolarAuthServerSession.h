#ifndef __SOLAR_AUTH_SERVER_SESSION_H__
#define __SOLAR_AUTH_SERVER_SESSION_H__

#pragma once

#include <PacketStruct_SM.h>
#include "ServerSessionEx.h"

#include <SolarAuth.CryptFlowPolicy.h>

struct MSG_BASE;


class SolarAuthServerSession : public ServerSessionEx
{
public:
	SolarAuthServerSession() ;
	~SolarAuthServerSession();

	virtual VOID				Init();
	virtual VOID				Release();
	//virtual VOID				Update();
	virtual eSERVER_TYPE		GetServerType() { return SOLARAUTH_SERVER; }

//	virtual VOID				OnRedirect();
//
	DWORD&						AccessCount() { return m_dwAccessCount; }
	GLOBAL_SERVER_INFO&			GlobalServerInfo() { return m_GLOBAL_SERVER_INFO; }
//
//
	virtual BOOL				Send( BYTE* pMsg, WORD wSize );
	virtual BOOL				SendPacket( MSG_BASE* pMsg, WORD wSize );
//
protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual VOID				OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();
	virtual	VOID				OnRecv( BYTE *pMsg, WORD wSize );
//
//protected:
//	virtual VOID				SendHeartBeat();
//
//private:
	virtual VOID				SendServerType();
	virtual void				SendHeartbeatPacket();
//	VOID						OnSendReqServerInfo();
//
private:
	DWORD						m_dwAccessCount;
	GLOBAL_SERVER_INFO			m_GLOBAL_SERVER_INFO;


public:
	inline VOID					SetSeqCryptKey( DWORD dwSeqCryptKey ) { m_dwSeqCryptKey = dwSeqCryptKey; }
	inline DWORD				GetSeqCryptKey() { return m_dwSeqCryptKey; }

	inline VOID					SetLocalServerIPAddress( DWORD dwServerIPAddress ) { m_dwLocalServerIPAddress = dwServerIPAddress; }
	inline DWORD				GetLocalServerIPAddress() { return m_dwLocalServerIPAddress; }

	struct Crypt
	{
		static VOID		Init_ClientRoleManager( SolarAuthServerSession* pServerSession, MSG_SM_CRYPTOGRAPHY_KEY* pMsgKEY );
		static VOID		StartPacketCrypt( SolarAuthServerSession* pServerSession );
		//
		inline static VOID		ProcPacket4Send();
		inline static VOID		ProcPacket4Recv();
	};

	BOOL						SendByEncode( BYTE* pMsg, WORD wSize )
	{
		CryptFlowPolicy::MakePacket::PreSend_DefConverter(
												pMsg,
												GetLocalServerIPAddress(),
												GetSeqCryptKey()
												);
		return Send( pMsg, wSize );
	}

private:
	DWORD						m_dwSeqCryptKey;
	DWORD						m_dwLocalServerIPAddress;

protected:
	// Cryptography for ServerRole
	struct sCRYPTINFO
	{
		// <FIELDs>
		BOOL					m_bIsCrypted;
		ClientSNKey				m_RandomSNK;
		CryptManagerS2S*		m_pCryptManager;

		// <METHODs>
		VOID					Init() { m_bIsCrypted = FALSE; m_pCryptManager = NULL; }
		VOID					Release()
		{
			SAFE_DELETE(m_pCryptManager);
			m_bIsCrypted = FALSE;
		}
		BOOL&					IsCrypted() { return m_bIsCrypted; }
		ClientSNKey&			RandomSNK() { return m_RandomSNK; }
		CryptManagerS2S&		CryptManager() { return *m_pCryptManager; }

		VOID					NextRoundSNKey4Send() { m_RandomSNK.NextRoundSNKey4Send(); };
		VOID					NextRoundSNKey4Recv() { m_RandomSNK.NextRoundSNKey4Recv(); };
		BYTE					GetSendSNK()		{ return m_RandomSNK.GetSendSNKey( RandomSNKey::c_dwBaseKey ); };
		BYTE					GetSendPairSNK()	{ return m_RandomSNK.GetSendSNKey( RandomSNKey::c_dwPairKey ); };
		BYTE					GetRecvSNK()		{ return m_RandomSNK.GetRecvSNKey( RandomSNKey::c_dwBaseKey ); };
		BYTE					GetRecvPairSNK()	{ return m_RandomSNK.GetRecvSNKey( RandomSNKey::c_dwPairKey ); };
	} CRYPTINFO;

private:
};


class SolarAuthServerSessionCrypted : public SolarAuthServerSession
{
public:
	enum ePACKET_CRYPTED { NO, YES, MAX };
	static sOBJECT_CHANGER* sm_pOBJECT_CHANGER;

	static VOID	CreateCode();

	static VOID ChangeObjectNoCrypt( SolarAuthServerSessionCrypted* pSolarAuthCrypted );
	static VOID ChangeObjectCrypted( SolarAuthServerSession* pSolarAuthNoCrypt );

public:
	//virtual BOOL				Send( BYTE* pMsg, WORD wSize );
	virtual BOOL				SendPacket( MSG_BASE* pMsg, WORD wSize );
protected:
	virtual	VOID				OnRecv( BYTE *pMsg, WORD wSize );
};



VOID SolarAuthServerSession::Crypt::ProcPacket4Send()
{
}

VOID SolarAuthServerSession::Crypt::ProcPacket4Recv()
{
}


#endif //__SOLAR_AUTH_SERVER_SESSION_H__
