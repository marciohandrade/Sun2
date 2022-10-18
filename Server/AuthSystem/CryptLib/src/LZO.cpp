#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../LZO.h"

using namespace	Crypt;

LZO::LZO()
{
	if( lzo_init() != LZO_E_OK )
	{
		//  Error
		return;
	}
}

LZO::~LZO()
{
}

bool LZO::Compress( char* src, int srcLen, char* dest, int* destLen )
{
	int		ret;

	ret = lzo1x_1_compress( ( unsigned char* )src, srcLen,
							( unsigned char* )dest, ( lzo_uint* )destLen, wrkmem );

	return ( ret == LZO_E_OK );
}

bool LZO::Decompress( char* src, int srcLen, char* dest, int* destLen )
{
	int		ret;

	ret = lzo1x_decompress( ( unsigned char* )src, srcLen,
							( unsigned char* )dest, ( lzo_uint* )destLen, 0 );

	return ( ret == LZO_E_OK );
}