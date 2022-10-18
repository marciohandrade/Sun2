#pragma once

//  Tiny Encryption Algorithm

namespace Crypt
{
	void		TeaEncode( DWORD* data, DWORD* key );
	void		TeaDecode( DWORD* data, DWORD* key );
}