#include <Windows.h>
#include <TChar.h>
#include "Base64.h"

static char		base64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void Base64Encode( LPCTSTR data, int size, LPTSTR output, int bufLen )
{
	TCHAR	ch[2] = { 0, 0 };
	int		sourceCount = 0;

	::memset( output, 0, sizeof( TCHAR ) * bufLen );
	while( sourceCount < size )
	{
		ch[0] = base64Table[( ( data[sourceCount] & 0xFC ) >> 2 )];
		_tcscat( output, ch );
		if( sourceCount + 1 >= size )
		{
			ch[0] = base64Table[( ( data[sourceCount] & 0x03 ) << 4 )];
			_tcscat( output, ch );
			_tcscat( output, "=" );
			_tcscat( output, "=" );

			break;
		}
		else 
		{
			ch[0] = base64Table[
				( ( data[sourceCount] & 0x03 ) << 4 ) | ( ( data[sourceCount + 1] & 0xF0 ) >> 4 )];

			_tcscat( output, ch );
		}

		if( sourceCount + 2 >= size )
		{
			ch[0] = base64Table[( ( data[sourceCount + 1] & 0x0F ) << 2 )];

			_tcscat( output, ch );
			_tcscat( output, "=" );
			break;
		}
		else
		{
			ch[0] = base64Table[
				( ( data[sourceCount + 1] & 0x0F ) << 2 ) | ( ( data[sourceCount + 2] & 0xC0 ) >> 6 )];

			_tcscat( output, ch );

			ch[0] = base64Table[( ( data[sourceCount + 2] & 0x3F ) )];
			_tcscat( output, ch );
		}

		sourceCount += 3;

		if( bufLen <= ( int )_tcslen( output ) + 3 )
			break;
	}

	_tcscat( output, "\0" );
}


int GetRealValue( int c )
{
	if( 'A' <= c && c <= 'Z' )		return c - 'A';
	if( 'a' <= c && c <= 'z' )		return c - 'a' + 26;
	if( '0' <= c && c <= '9' )		return c - '0' + 52;
	if( c == '+' )		return 62;
	if( c == '/' )		return 63;
	if( c == '=' )		return -1;			// end of line

	return -2;	// none value;
}


void Base64Decode( LPCTSTR data, int size, LPTSTR output, int bufLen )
{
	TCHAR	temp[5] = "";
	TCHAR	ch[2] = { 0, 0 };
	int		count = 0, i, r, result;

	while( count < size )
	{
		i = 0;
		_tcscpy( temp, "" );

		while( count < size && i < 4 )
		{
			r = data[count++];
			result = GetRealValue( r );

			if( result != -2 )
			{
				if( result != -1 )	temp[i++] = result;
				else				temp[i++] = '@';
			}
		}

		if( i == 4 )
		{
			if( count + 3 >= size )
			{
				if( temp[1] == '@' )
				{
					ch[0] = temp[0] << 2;
					_tcscat( output, ch );

					break;
				}
				else
				{
					ch[0] = ( temp[0] << 2 | temp[1] >> 4 );
					_tcscat( output, ch );
				}

				if( temp[2] == '@' )
				{
					ch[0] = temp[1] << 4;
					_tcscat( output, ch );

					break;
				}
				else
				{
					ch[0] = ( temp[1] << 4 | temp[2] >> 2 );
					_tcscat( output, ch );
				}

				if( temp[3] == '@' )
				{
					ch[0] = temp[2] << 6;
					_tcscat( output, ch );

					break;
				}
				else
				{
					ch[0] = ( temp[2] << 6 | temp[3] );
					_tcscat( output, ch );
				}
			}
			else
			{
				ch[0] = ( temp[0] << 2 | temp[1] >> 4 );		_tcscat( output, ch );
				ch[0] = ( temp[1] << 4 | temp[2] >> 2 );		_tcscat( output, ch );
				ch[0] = ( temp[2] << 6 | temp[3] );				_tcscat( output, ch );
			}
		}

		if( bufLen <= ( int )_tcslen( output ) )
			break;
	}
}
