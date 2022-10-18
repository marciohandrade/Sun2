#pragma once

#include "GameServerSession.h"

class FieldServerSession : public GameServerSession
{
public:
	FieldServerSession();
	~FieldServerSession();

	eSERVER_TYPE	GetServerType() { return FIELD_SERVER;	}

public:
	virtual VOID	Init();

public:
	VOID			CreateVillage();
private:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
};

