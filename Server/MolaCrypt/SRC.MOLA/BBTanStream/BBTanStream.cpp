#include "stdafx.h"

#include "BBTanStream.h"


BBTanStream::BBTanStream()
{
}

BBTanStream::~BBTanStream()
{
}




VOID	BBTanStream::Init()
{
	Init_T1();
	Init_T2();
	//m_StartHandler.insert( m_StartHandler.end(), make_pair( 2, BBTAN_RECURSIVE_HANDLER_NAME(2) ) );
}

VOID	BBTanStream::Encode( uRND_CONVKEY& rKEY, BYTE* pBYTE, WORD wMSG_BASE, BYTE& rbyLinkedNo )
{
#if LOG_DEBUG_WRITER(__MOLA_CLIENT_DEBUGGING_WRITELOG__)
	{
		CHAR buffer[16384];
		INT iWritten = 0;
		CHAR* pBuffer = buffer;
		WORD wLEN = 0;
		BYTE* pB = pBYTE;

		while( wLEN < wMSG_BASE )
		{
			BOOL iRET = _snprintf( pBuffer, sizeof(buffer)-iWritten, "[%02X]", *pB );
			if( iRET < 0 )
				break;

			iWritten += iRET;
			pBuffer += iRET;
			++wLEN;
			++pB;
		}

		_snprintf( pBuffer, sizeof(buffer)-iWritten, "ST\r\n" );

		DWORD dwWritten;
		WriteFile( shFILE, buffer, wLEN*4+4, &dwWritten, NULL );
		FlushFileBuffers( shFILE );
	}
#endif //

	Converter( m_Sender, rKEY, pBYTE, wMSG_BASE, rbyLinkedNo );

#if LOG_DEBUG_WRITER(__MOLA_CLIENT_DEBUGGING_WRITELOG__)
	{
		CHAR buffer[16384];
		INT iWritten = 0;
		CHAR* pBuffer = buffer;
		WORD wLEN = 0;
		BYTE* pB = pBYTE;

		while( wLEN < wMSG_BASE )
		{
			BOOL iRET = _snprintf( pBuffer, sizeof(buffer)-iWritten, "[%02X]", *pB );
			if( iRET < 0 )
				break;

			iWritten += iRET;
			pBuffer += iRET;
			++wLEN;
			++pB;
		}

		_snprintf( pBuffer, sizeof(buffer)-iWritten, "EN\r\n" );

		DWORD dwWritten;
		WriteFile( shFILE, buffer, wLEN*4+4, &dwWritten, NULL );
		FlushFileBuffers( shFILE );
	}
#endif //
}

VOID	BBTanStream::Decode( uRND_CONVKEY& rKEY, BYTE* pBYTE, WORD wMSG_BASE, BYTE& rbyLinkedNo )
{
#if LOG_DEBUG_WRITER(__MOLA_CLIENT_DEBUGGING_WRITELOG__)
	{
		CHAR buffer[16384];
		INT iWritten = 0;
		CHAR* pBuffer = buffer;
		WORD wLEN = 0;
		BYTE* pB = pBYTE;

		while( wLEN < wMSG_BASE )
		{
			BOOL iRET = _snprintf( pBuffer, sizeof(buffer)-iWritten, "[%02X]", *pB );
			if( iRET < 0 )
				break;

			iWritten += iRET;
			pBuffer += iRET;
			++wLEN;
			++pB;
		}

		_snprintf( pBuffer, sizeof(buffer)-iWritten, "ST\r\n" );

		DWORD dwWritten;
		WriteFile( shFILE, buffer, wLEN*4+4, &dwWritten, NULL );
		FlushFileBuffers( shFILE );
	}
#endif //

	Converter( m_Receiver, rKEY, pBYTE, wMSG_BASE, rbyLinkedNo );

#if LOG_DEBUG_WRITER(__MOLA_CLIENT_DEBUGGING_WRITELOG__)
	{
		CHAR buffer[16384];
		INT iWritten = 0;
		CHAR* pBuffer = buffer;
		WORD wLEN = 0;
		BYTE* pB = pBYTE;

		while( wLEN < wMSG_BASE )
		{
			BOOL iRET = _snprintf( pBuffer, sizeof(buffer)-iWritten, "[%02X]", *pB );
			if( iRET < 0 )
				break;

			iWritten += iRET;
			pBuffer += iRET;
			++wLEN;
			++pB;
		}

		_snprintf( pBuffer, sizeof(buffer)-iWritten, "EN\r\n" );

		DWORD dwWritten;
		WriteFile( shFILE, buffer, wLEN*4+4, &dwWritten, NULL );
		FlushFileBuffers( shFILE );
	}
#endif //
}


#pragma warning(push)
#pragma warning(disable:4307)
const BYTE sc_RND_A = GETRNDNO_RNG08_T2(BBTAN_KEY_INITIAL_GENERATOR(7));
const BYTE sc_RND_B = GETRNDNO_RNG08_T3(BBTAN_KEY_INITIAL_GENERATOR(9));
const WORD sc_RND_SIZE = GETRNDNO_RNG128_T1((BBTAN_KEY_INITIAL_GENERATOR(4)+BBTAN_KEY_SERVICE_GENERATOR2(9)));
#pragma warning(pop)


VOID	BBTanStream::Converter( BBTAN_MAP& rStarter, uRND_CONVKEY& rKEY, BYTE* pBYTE, WORD wMSG_BASE, BYTE& rbyLinkedNo )
{
	if( !(2 < wMSG_BASE) ) return;

	WORD wCATEnPROTO = *(WORD*)pBYTE;

#if CODE_BACKUP
	// <Case1>
	BYTE byA1 = (pBYTE[0]&0x0F)<<sc_RND_A | (pBYTE[1]&0x0F);
	BYTE byB1 = (pBYTE[1]&0xF0) | (pBYTE[0]&0xF0)>>sc_RND_B;
	WORD wCATEnPROTO1 = MAKEWORD( byA1, byB1 );
	// <Case2>
	BYTE byA2 = (pBYTE[0]&0x0F)<<8 | (pBYTE[1]&0x0F);
	BYTE byB2 = (pBYTE[1]&0xF0) | (pBYTE[0]&0xF0)>>8;
	WORD wCATEnPROTO2 = MAKEWORD( byA2, byB2 );
#endif

	wCATEnPROTO = wCATEnPROTO%256;

#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_BBTAN__)
	printf( "STHANDLE %u : %u : %u\n", wCATEnPROTO, wMSG_BASE, rbyLinkedNo );
#endif

	BBTAN_MAP_IT it = rStarter.find( (BYTE)wCATEnPROTO );
	if( it != rStarter.end() )
	{
		ConvertBBTanHandler& rHANDLER = it->second;
		rHANDLER( rKEY, &pBYTE[2], wMSG_BASE-2, rbyLinkedNo );
	}

#if CODE_BACKUP
	ConvertBBTanHandler& rHANDLER = rStarter[wCATEnPROTO%256];
	rHANDLER( &pBYTE[2], wMSG_BASE-2, rbyLinkedNo );
#endif
}

VOID	BBTanStream::Release()
{
}




