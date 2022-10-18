
#include "Structure.h"
#include "Crypt.RndKey.h"
#include "FileDecoder.h"

#include <stdio.h>

BOOL	FileDecoder::ReadStart( const CHAR* pFileName, QueryKey IN key )
{
	union
	{
		CHAR	pChr[sizeof(DWORD)];
		DWORD	pDW;
	};

	memcpy( pChr, key, sizeof(DWORD) );

	if( Open( pFileName, pDW ) == FALSE )
	{
		printf( "Can't open file %s\n", pFileName );
		return FALSE;
	}

	return TRUE;
}

VOID	FileDecoder::ReadEnd()
{
	Close();
}

BOOL	FileDecoder::GetData( BYTE* IN OUT pData, DWORD IN dwSize )
{
	// dwSize의 값을 나눠 사용할 필요가 있다.
	// '/' + '%'
	DWORD dwCount = (DWORD)(dwSize/TOKEN_SIZE);
	DWORD dwRemain = (DWORD)(dwSize%TOKEN_SIZE);
	BYTE* pOffset = pData;

	for( DWORD c=0 ; c<dwCount ; ++c )
	{
		if( this->Read( pOffset, TOKEN_SIZE ) < 0 )
			return FALSE;
		pOffset += TOKEN_SIZE;
	}

	if( this->Read( pOffset, dwRemain ) < 0 )
		return FALSE;

	return TRUE;
}

