#pragma once

namespace Crypt
{
	bool		PacketEncode( unsigned char* source, int sourceLen, unsigned char* output, DWORD key );
	bool		PacketDecode( unsigned char* source, int sourceLen, unsigned char* output, DWORD key );
}