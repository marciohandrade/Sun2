#ifndef _FIELDSERVERSESSION_H_
#define _FIELDSERVERSESSION_H_

#pragma once

#include "GameServerSession.h"

class FieldServerSession : public GameServerSession
{
public:
	FieldServerSession() {};
	~FieldServerSession() {};

	eSERVER_TYPE				GetServerType() { return FIELD_SERVER;	}

};

#endif // _FIELDSERVERSESSION_H_