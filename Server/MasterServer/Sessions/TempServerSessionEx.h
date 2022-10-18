#ifndef __TEMP_SERVER_SESSION_EX_H__
#define __TEMP_SERVER_SESSION_EX_H__

#pragma once

#include <TempServerSession.h>
#include "Services4Session.h"

// TempServerSession�� ���� Ÿ������ ������ �ʿ䰡 �ִ�.

class TempServerSessionEx : public TempServerSession
{
public:
	TempServerSessionEx() {};
	~TempServerSessionEx() {};

	inline Services4Session&	Services() { return m_Services; }

	//VOID						MigrateObject( MasterServerSession* pTargetMaster );

protected:
	//
	//-------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------
	// Init & Release
	VOID						_Initialize();
	VOID						_Release();

public:
	// <INTERFACEs>
	virtual VOID				Init() { _Initialize(); };
	virtual VOID				Release() { _Release(); };

	// ������ �ʱ�ȭ ( �ܺο��� ����ϴ� ������ ���� ) : Factory�� Alloc()�ÿ� ȣ��, init()�� �뵵�� �ٸ�! ����!
	//virtual VOID				Clear() {}

	static VOID					ReleaseProcess( TempServerSessionEx* pTempSession );
	static VOID					Redirect2NewSession( DWORD dwIP, SERVER_KEY skKey );

	inline SERVER_KEY&			TempServerKey() { return m_skTmpServerKey; }

protected:
	virtual	VOID				OnRecv( BYTE* pMsg, WORD wSize );
public:
	virtual BOOL				SendPacket( MSG_BASE* pMsg, WORD wSize );

private:
	Services4Session			m_Services;
protected:
	DWORD						m_dwPacketStrictSequence;
	SERVER_KEY					m_skTmpServerKey;
};


#endif //__TEMP_SERVER_SESSION_EX_H__

