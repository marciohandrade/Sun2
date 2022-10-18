#pragma once

//=======================================================================================================================
// TempServerSession
//	- ������ ���� ���ſ� �ӽ� ServerSession ( ������ ������ ��� �ö� �ӽ÷� �Ҵ�Ǵ� ���� )
//	- Agent���������� ������ ��� ���� ������ ��������!!
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

