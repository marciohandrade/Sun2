#ifndef _TEMP_SERVER_SESSION_H_
#define _TEMP_SERVER_SESSION_H_

#include "ServerSession.h"

#pragma once

class TempServerSession : public ServerSession
{
public:
	TempServerSession();
	~TempServerSession();

	/// �ӽ� Ŭ�����̱� ������ �ƹ��ϵ� ���� �ʴ´�.!!!!
	virtual VOID			Init() {}
	virtual VOID			Release();
	virtual VOID			Update() {}

	eSERVER_TYPE			GetServerType() { return TEMP_SERVER; }

protected:
	virtual VOID			OnAccept( DWORD dwNetworkIndex );
	virtual	VOID			OnRecv( BYTE *pMsg, WORD wSize );

private:
	BOOL					m_bFirstPacketReceived;
};

#endif // __TEMP_SERVER_SESSION_H__