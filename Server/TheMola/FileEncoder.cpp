
#include "Structure.h"
#include "Crypt.RndKey.h"
#include "FileEncoder.h"

BOOL	FileEncoder::WriteStart( const CHAR* pFileName, QueryKey IN key )
{
	union
	{
		CHAR	pChr[sizeof(DWORD)];
		DWORD	pDW;
	};

	memcpy( pChr, key, sizeof(DWORD) );

	return Create( pFileName, pDW, 0 );
}

BOOL	FileEncoder::WriteStart( const HANDLE hFD, QueryKey IN key )
{
	union
	{
		CHAR	pChr[sizeof(DWORD)];
		DWORD	pDW;
	};

	memcpy( pChr, key, sizeof(DWORD) );

	return Create( hFD, pDW, 0 );
}

VOID	FileEncoder::WriteEnd()
{
	Save( (BYTE*)GetChunk(), /*sizeof(CRYPT_FILE_CHUNK)*/ 0 );
	Close();
}

BOOL	FileEncoder::SetData( BYTE* IN pData, const DWORD dwSize )
{
	// dwSize의 값을 나눠 사용할 필요가 있다.
	// '/' + '%'
	DWORD dwCount = (DWORD)(dwSize/TOKEN_SIZE);
	DWORD dwRemain = (DWORD)(dwSize%TOKEN_SIZE);
	BYTE* pOffset = pData;

	for( DWORD c=0 ; c<dwCount ; ++c )
	{
		if( this->Write( pOffset, TOKEN_SIZE ) < 0 )
			return FALSE;
		pOffset += TOKEN_SIZE;
	}

	if( this->Write( pOffset, dwRemain ) < 0 )
		return FALSE;

	return TRUE;
}

