#include <Windows.h>
#include <stdio.h>
#include <memory.h>

#include "../PacketCrypt.h"
#include "../TEA.h"


bool Crypt::PacketEncode( unsigned char* source, int sourceLen, unsigned char* output, DWORD key )
{
	DWORD	keyArray[4] = { key + 0, key + 1, key + 2, key + 3 };
	int		i;

	if( sourceLen % 8 != 0 )
		return false;

	::memcpy( output, source, sourceLen );
	for( i = 0 ; i < sourceLen ; i += 8 )
		Crypt::TeaEncode( ( DWORD* )&output[i], keyArray );


/*
	DWORD			keyArray[4];
	unsigned char	buffer[MAX_SOURCESIZE];
	unsigned char	paddingSize;
	int				i;

	paddingSize	= ( 8 - ( sourceLen % 8 ) ) % 8;
	*outputLen	= sourceLen + paddingSize;
	::memset( buffer, 0, MAX_SOURCESIZE );

	//  buffer의 첫 1바이트는 padding size.
	buffer[0] = paddingSize;

	::memcpy( &buffer[1], source, sourceLen );

	for( i = 0 ; i < *outputLen ; i += 8 )
		Crypt::TeaEncode( ( DWORD* )&buffer[1 + i], keyArray );

	*outputLen += 1;
	::memcpy( output, buffer, *outputLen );
*/
	return true;
}

bool Crypt::PacketDecode( unsigned char* source, int sourceLen, unsigned char* output, DWORD key )
{
	DWORD	keyArray[4] = { key + 0, key + 1, key + 2, key + 3 };
	int		i;

	if( sourceLen % 8 != 0 )
		return false;

	::memcpy( output, source, sourceLen );
	for( i = 0 ; i < sourceLen ; i += 8 )
		Crypt::TeaDecode( ( DWORD* )&output[i], keyArray );

/*
	//  첫 1바이트는 Padding size이다.
	paddingSize = source[0];

	sourceLen	-= 1;
	*outputLen	= sourceLen - paddingSize;
	::memcpy( output, &source[1], sourceLen );

	for( i = 0 ; i < sourceLen ; i += 8 )
		Crypt::TeaDecode( ( DWORD* )&output[i], keyArray );
*/

	return true;
}