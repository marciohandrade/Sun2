
#include "Structure.h"
#include "Crypt.RndKey.h"
#include "DBDecryptFileReader.h"
#include <stdio.h>

BOOL	DBDecryptFileReader::ReadStart( const CHAR* pFileName, QueryKey IN dbp )
{
	union
	{
		CHAR	pChr[sizeof(DWORD)];
		DWORD	pDW;
	};

	memcpy( pChr, dbp, sizeof(DWORD) );

	if( Open( pFileName, pDW ) == FALSE )
	{
		printf( "Can't open file\n" );
		return FALSE;
	}

	for( int i=1 ; i<(4+1) ; ++i )
	{
		_GetIdentification( (BYTE*)ID_PWD[i].m_pUserID );
		_GetPassword( (BYTE*)ID_PWD[i].m_pUserPWD );
	}

	return TRUE;
}

BOOL	DBDecryptFileReader::ReadStartFromBuffer( const BYTE* pSTREAM, QueryKey IN dbp )
{
	union
	{
		CHAR	pChr[sizeof(DWORD)];
		DWORD	pDW;
	};

	memcpy( pChr, dbp, sizeof(DWORD) );

	if( ReadEncryptedBuffer(
						pSTREAM,
						MIN_FILE_DATA_SIZE + (DWORD)sizeof(CRYPT_FILE_HEADER),
						pDW
						) == FALSE )
	{
		printf( "Can't open file\n" );
		return FALSE;
	}

	for( int i=1 ; i<(4+1) ; ++i )
	{
		_GetIdentification( (BYTE*)ID_PWD[i].m_pUserID );
		_GetPassword( (BYTE*)ID_PWD[i].m_pUserPWD );
	}

	return TRUE;
}

VOID	DBDecryptFileReader::ReadEnd()
{
	Close();
}

BOOL	DBDecryptFileReader::_GetIdentification( BYTE* IN OUT pID )
{
	if( this->Read( pID, MAX_ID_SIZE ) < 0 )
		return FALSE;
	return TRUE;
}

BOOL	DBDecryptFileReader::_GetPassword( BYTE* IN OUT pPWD )
{
	if( this->Read( pPWD, MAX_PWD_SIZE ) < 0 )
		return FALSE;
	return TRUE;
}

