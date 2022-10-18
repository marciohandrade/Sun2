#include <Windows.h>

#include "../Tea.h"

void Crypt::TeaEncode( DWORD* data, DWORD* key )
{
	DWORD	y = data[0], z = data[1], sum = 0;
	DWORD	delta = 0x9e3779b9, n = 32;

	while( n-- > 0 )
	{
		sum	+= delta;
		y	+= ( z << 4 ) + key[0] ^ z + sum ^ ( z >> 5 ) + key[1];
		z	+= ( y << 4 ) + key[2] ^ y + sum ^ ( y >> 5 ) + key[3];
	}

	data[0] = y;
	data[1] = z;
}

void Crypt::TeaDecode( DWORD* data, DWORD* key )
{
	DWORD	n = 32, sum, y = data[0], z = data[1];
	DWORD	delta = 0x9e3779b9;

	sum = delta << 5;

	while( n-- > 0 )
	{
		z -= ( y << 4 ) + key[2] ^ y + sum ^ ( y >> 5 ) + key[3];
		y -= ( z << 4 ) + key[0] ^ z + sum ^ ( z >> 5 ) + key[1];

		sum -= delta;
	}

	data[0] = y;
	data[1] = z;
}