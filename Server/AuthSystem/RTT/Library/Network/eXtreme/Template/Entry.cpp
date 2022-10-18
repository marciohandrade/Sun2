#include "stdafx.h"
#include "Entry.h"
#include <eXtreme/util/Console.h>

class Test
{
	Singleton( Test );
};

Test::Test()
{
}

Test::~Test()
{
}


void ErrorHandler( DWORD lastError, char* desc )
{
	printf( "Error = %d,  %s\n", lastError, desc );
}

int main()
{
	eXtreme::Console	console;

	if( eXtreme::Initialize( 0, ErrorHandler ) == false )
	{
		printf( "Already same server running.\n" );
		getch();

		return 0;
	}


	Entry::Instance()->Join();
	Entry::Instance()->Destroy();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
Entry::Entry()
{
	m_networkStub.Create( 1, &m_managerStub );

	if( m_networkSkel.Create( 100, &m_managerSkel ) == false )
	{
		printf( "Invalid Server Information." );

		return;
	}
	m_networkSkel.OpenAcceptor( "10.1.25.132", 20000 );

	printf( "\nService started...\n" );
}

Entry::~Entry()
{
}

void Entry::Destroy()
{
	m_networkSkel.Destroy();
	m_networkStub.Destroy();
}

void Entry::Join()
{
	while( true )
	{
		m_networkSkel.Update();
		m_networkStub.Update();

		if( kbhit() != 0 )
		{
			char ch = getch();

			if( ch == 32 )
			{
//				printf( "Active = %d, Inactive = %d\n",
//					m_managerSkel.GetActiveSessionCnt(), m_managerSkel.GetInactiveSessionCnt() );
			}
			else
				break;
		}

		::Sleep( 1 );
	}
}