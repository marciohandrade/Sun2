#ifndef __FILED_SERVER_H__
#define __FILED_SERVER_H__

#pragma once

#include "GameServerEx.h"

class FieldServer : public GameServerEx
{
public:
	FieldServer(void);
	virtual ~FieldServer(void);

	eSERVER_TYPE		GetServerType() { return FIELD_SERVER; }

private:

	virtual BOOL	Init();
	virtual BOOL	Update( DWORD tick );
	virtual void	Release();
	virtual BOOL	KeyEvent( CHAR ch );
};

#endif // __FILED_SERVER_H__