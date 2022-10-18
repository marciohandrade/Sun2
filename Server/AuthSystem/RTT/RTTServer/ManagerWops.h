#pragma once

#include <eXtreme/net/SessionManager.h>
#include "Wops.h"

class ManagerWops : public eXtreme::SessionManager
{
	Singleton( ManagerWops );

public:
			void		SetSustenance( Wops* session )
			{
				m_sustenance = session;
			}

			Wops*		GetSustenance()
			{
				return m_sustenance;
			}


protected:
	virtual	eXtreme::ServerSession*		CreateSession()
			{
				return new Wops();
			}

	virtual	void						DeleteSession( eXtreme::ServerSession* session )
			{
				delete session;
			}

private:
			Wops*		m_sustenance;
};