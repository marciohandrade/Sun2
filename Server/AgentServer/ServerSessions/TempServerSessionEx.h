#pragma once

//=======================================================================================================================
// TempServerSession
//	- 서버의 접속 수신용 임시 ServerSession ( 서버가 접속해 들어 올때 임시로 할당되는 세션 )
//	- Agent서버에서는 접속해 들어 오는 서버는 없을꺼다!!
//=======================================================================================================================

#include <TempServerSession.h>

struct MSG_SERVER_TYPE;

class TempServerSessionEx : public TempServerSession
{
public:
	TempServerSessionEx();
	~TempServerSessionEx();

public:
    // CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
    void Redirect2NewSession(MSG_SERVER_TYPE* const first_contact_msg);
protected:
	virtual VOID			Init();

private:
	virtual	VOID			OnRecv( BYTE * pMsg, WORD wSize );
};

