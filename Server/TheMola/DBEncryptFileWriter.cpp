
#include "DBEncryptFileWriter.h"


BOOL	DBEncryptFileWriter::WriteStart( const CHAR* pFileName, QueryKey IN dbp )
{
	union
	{
		CHAR	pChr[sizeof(DWORD)];
		DWORD	pDW;
	};

	memcpy( pChr, dbp, sizeof(DWORD) );

	return Create( pFileName, pDW, 2 );
}

VOID	DBEncryptFileWriter::WriteEnd()
{
	Save( (BYTE*)GetChunk(), /*sizeof(CRYPT_FILE_CHUNK)*/ 0 );
	Close();
}

BOOL	DBEncryptFileWriter::SetIdentification( BYTE* IN OUT pID )
{
	if( this->Write( pID, MAX_ID_SIZE ) < 0 )
		return FALSE;
	return TRUE;
}

BOOL	DBEncryptFileWriter::SetPassword( BYTE* IN OUT pPWD )
{
	if( this->Write( pPWD, MAX_PWD_SIZE ) < 0 )
		return FALSE;
	return TRUE;
}

