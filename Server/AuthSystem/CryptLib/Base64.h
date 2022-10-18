#pragma once

namespace Crypt
{
	void		Base64Encode( LPCTSTR data, int size, LPTSTR output, int bufLen );
	void		Base64Decode( LPCTSTR data, int size, LPTSTR output, int bufLen );
}