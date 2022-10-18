#pragma once

namespace Crypt
{
	typedef unsigned		int		UInt4;
	typedef unsigned short	int		UInt2;
	typedef unsigned		char	BYTE;


	class MD5
	{
	public:
				MD5();
				~MD5();

		static	void	EncodeString( BYTE* szString, BYTE digest[16] );
		static	void	EncodeFile( BYTE* szFilename, BYTE digest[16] );

				void	Initialize();
				void	Update( BYTE* chInput, UInt4 nInputLen );
				void	Finalize( BYTE md5Digest[16] );

	private:
		static	char*	DigestToString( BYTE md5Digest[16] );


				void	Transform( BYTE* block );
				void	Encode( BYTE* dest, UInt4* src, UInt4 nLength );
				void	Decode( UInt4* dest, BYTE* src, UInt4 nLength );


				UInt4	rotate_left( UInt4 x, UInt4 n )
						{	return	( ( x << n ) | ( x >> ( 32 - n ) ) );	}

				UInt4	F( UInt4 x, UInt4 y, UInt4 z )
						{	return	( ( x & y ) | ( ~x & z ) );				}

				UInt4	G( UInt4 x, UInt4 y, UInt4 z )
						{	return	( ( x & z ) | ( y & ~z ) );				}

				UInt4	H( UInt4 x, UInt4 y, UInt4 z )
						{	return	( x ^ y ^ z );							}

				UInt4	I( UInt4 x, UInt4 y, UInt4 z )
						{	return	( y ^ ( x | ~z ) );						}

				void	FF( UInt4& a, UInt4 b, UInt4 c, UInt4 d, UInt4 x, UInt4 s, UInt4 ac )
						{	a += F( b, c, d ) + x + ac;	a = rotate_left( a, s );	a += b;		}

				void	GG( UInt4& a, UInt4 b, UInt4 c, UInt4 d, UInt4 x, UInt4 s, UInt4 ac )
						{	a += G( b, c, d ) + x + ac;	a = rotate_left( a, s );	a += b;		}

				void	HH( UInt4& a, UInt4 b, UInt4 c, UInt4 d, UInt4 x, UInt4 s, UInt4 ac )
						{	a += H( b, c, d ) + x + ac;	a = rotate_left( a, s );	a += b;		}

				void	II( UInt4& a, UInt4 b, UInt4 c, UInt4 d, UInt4 x, UInt4 s, UInt4 ac )
						{	a += I( b, c, d ) + x + ac;	a = rotate_left( a, s );	a += b;		}



				UInt4		m_state[4];
				UInt4		m_count[2];
				BYTE		m_buffer[64];
				BYTE		m_digest[16];
	};
}