#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "WZ_MD5.h"
#include "MD5_KEYVAL.h"

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21


static unsigned char PADDING[64] =
{
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

MD5::MD5()
{
}

MD5::~MD5()
{
}

char* MD5::DigestToString( BYTE md5Digest[16] )
{
	char	buffer[256];
	char	chEach[10];
	int		nCount;

	::memset( buffer, 0, 256 );
	::memset( chEach, 0, 10 );

	for( nCount = 0 ; nCount < 16 ; nCount++ )
	{
		::sprintf( chEach, "%02x", md5Digest[nCount] );
		::strncat( buffer, chEach, sizeof( chEach ) );
	}

	return ::strdup( buffer );
}

void MD5::EncodeString( BYTE* szString, int srcLen, BYTE digest[16], BYTE* key, int keyLen )
{
	MD5		alg;
	int		i, keyIndex;

	keyIndex = 0;
	for( i = 0 ; i < keyLen ; i++ )
		keyIndex += key[i] + MD5_INDEX_SUM_NUM;
	keyIndex %= MD5_INDEX_MAX_NUM;

	alg.Initialize( keyIndex );
	alg.Update( szString, ( unsigned int )srcLen );
	alg.Finalize( digest );
}

bool MD5::Compare( BYTE* source, int srcLen, BYTE* encoded, BYTE* key, int keyLen )
{
	BYTE	buffer[16] = { 0, };
	int		i;

	EncodeString( source, srcLen, buffer, key, keyLen );
	for( i = 0 ; i < 16 ; i++ )
	{
		if( encoded[i] != buffer[i] )
			return false;
	}

	return true;
}

void MD5::Initialize( int index )
{
	::memset( m_count, 0, 2 * sizeof( UInt4 ) );

	if( index == -1 )
	{
		m_state[0]	= 0x67452301;
		m_state[1]	= 0xefcdab89;
		m_state[2]	= 0x98badcfe;
		m_state[3]	= 0x10325476;
	}
	else if( index > -1 && index < MAX_KEY_INDEX )
	{
		m_state[0]	= MD5_KEYVAL[( index << 2 ) + 0];
		m_state[1]	= MD5_KEYVAL[( index << 2 ) + 1];
		m_state[2]	= MD5_KEYVAL[( index << 2 ) + 2];
		m_state[3]	= MD5_KEYVAL[( index << 2 ) + 3];
	}
}
	
void MD5::Update( BYTE* chInput, UInt4 nInputLen )
{
	UInt4	i, index, partLen;

	index = ( unsigned int )( ( m_count[0] >> 3 ) & 0x3F );

	if( ( m_count[0] += ( nInputLen << 3 ) ) < ( nInputLen << 3 ) )
		m_count[1]++;

	m_count[1]	+= ( nInputLen >> 29 );
	partLen		= 64 - index;

	if( nInputLen >= partLen )
	{
		::memcpy( &m_buffer[index], chInput, partLen );
		Transform( m_buffer );

		for( i = partLen ; i + 63 < nInputLen ; i += 64 )
			Transform( &chInput[i] );

		index = 0;
	}
	else
		i = 0;

	::memcpy( &m_buffer[index], &chInput[i], nInputLen - i );
}

void MD5::Finalize( BYTE md5Digest[16] )
{
	BYTE	bits[8];
	UInt4	index, padLen;

	Encode( bits, m_count, 8 );

	index	= ( unsigned int )( ( m_count[0] >> 3 ) & 0x3f );
	padLen	= ( index < 56 ) ? ( 56 - index ) : ( 120 - index );

	Update( PADDING, padLen );
	Update( bits, 8 );

	Encode( m_digest, m_state, 16 );

	::memset( m_count, 0, 2 * sizeof( UInt4 ) );
	::memset( m_state, 0, 4 * sizeof( UInt4 ) );
	::memset( m_buffer,0, 64 * sizeof( BYTE ) );


	char*	tmp;

	tmp = DigestToString( m_digest );
	::strncpy( ( char* )md5Digest, tmp, 16 );
	free( tmp );
}

void MD5::Transform( BYTE* block )
{
	UInt4	a = m_state[0],
			b = m_state[1],
			c = m_state[2],
			d = m_state[3],
			x[16];

	Decode( x, block, 64 );

	// Round 1
	FF( a, b, c, d, x[ 0], S11, 0xd76aa478 );
	FF( d, a, b, c, x[ 1], S12, 0xe8c7b756 );
	FF( c, d, a, b, x[ 2], S13, 0x242070db );
	FF( b, c, d, a, x[ 3], S14, 0xc1bdceee );
	FF( a, b, c, d, x[ 4], S11, 0xf57c0faf );
	FF( d, a, b, c, x[ 5], S12, 0x4787c62a );
	FF( c, d, a, b, x[ 6], S13, 0xa8304613 );
	FF( b, c, d, a, x[ 7], S14, 0xfd469501 );
	FF( a, b, c, d, x[ 8], S11, 0x698098d8 );
	FF( d, a, b, c, x[ 9], S12, 0x8b44f7af );
	FF( c, d, a, b, x[10], S13, 0xffff5bb1 );
	FF( b, c, d, a, x[11], S14, 0x895cd7be );
	FF( a, b, c, d, x[12], S11, 0x6b901122 );
	FF( d, a, b, c, x[13], S12, 0xfd987193 );
	FF( c, d, a, b, x[14], S13, 0xa679438e );
	FF( b, c, d, a, x[15], S14, 0x49b40821 );

	// Round 2
	GG( a, b, c, d, x[ 1], S21, 0xf61e2562 );
	GG( d, a, b, c, x[ 6], S22, 0xc040b340 );
	GG( c, d, a, b, x[11], S23, 0x265e5a51 );
	GG( b, c, d, a, x[ 0], S24, 0xe9b6c7aa );
	GG( a, b, c, d, x[ 5], S21, 0xd62f105d );
	GG( d, a, b, c, x[10], S22,  0x2441453 );
	GG( c, d, a, b, x[15], S23, 0xd8a1e681 );
	GG( b, c, d, a, x[ 4], S24, 0xe7d3fbc8 );
	GG( a, b, c, d, x[ 9], S21, 0x21e1cde6 );
	GG( d, a, b, c, x[14], S22, 0xc33707d6 );
	GG( c, d, a, b, x[ 3], S23, 0xf4d50d87 );
	GG( b, c, d, a, x[ 8], S24, 0x455a14ed );
	GG( a, b, c, d, x[13], S21, 0xa9e3e905 );
	GG( d, a, b, c, x[ 2], S22, 0xfcefa3f8 );
	GG( c, d, a, b, x[ 7], S23, 0x676f02d9 );
	GG( b, c, d, a, x[12], S24, 0x8d2a4c8a );

	// Round 3
	HH( a, b, c, d, x[ 5], S31, 0xfffa3942 );
	HH( d, a, b, c, x[ 8], S32, 0x8771f681 );
	HH( c, d, a, b, x[11], S33, 0x6d9d6122 );
	HH( b, c, d, a, x[14], S34, 0xfde5380c );
	HH( a, b, c, d, x[ 1], S31, 0xa4beea44 );
	HH( d, a, b, c, x[ 4], S32, 0x4bdecfa9 );
	HH( c, d, a, b, x[ 7], S33, 0xf6bb4b60 );
	HH( b, c, d, a, x[10], S34, 0xbebfbc70 );
	HH( a, b, c, d, x[13], S31, 0x289b7ec6 );
	HH( d, a, b, c, x[ 0], S32, 0xeaa127fa );
	HH( c, d, a, b, x[ 3], S33, 0xd4ef3085 );
	HH( b, c, d, a, x[ 6], S34,  0x4881d05 );
	HH( a, b, c, d, x[ 9], S31, 0xd9d4d039 );
	HH( d, a, b, c, x[12], S32, 0xe6db99e5 );
	HH( c, d, a, b, x[15], S33, 0x1fa27cf8 );
	HH( b, c, d, a, x[ 2], S34, 0xc4ac5665 );

	// Round 4
	II( a, b, c, d, x[ 0], S41, 0xf4292244 );
	II( d, a, b, c, x[ 7], S42, 0x432aff97 );
	II( c, d, a, b, x[14], S43, 0xab9423a7 );
	II( b, c, d, a, x[ 5], S44, 0xfc93a039 );
	II( a, b, c, d, x[12], S41, 0x655b59c3 );
	II( d, a, b, c, x[ 3], S42, 0x8f0ccc92 );
	II( c, d, a, b, x[10], S43, 0xffeff47d );
	II( b, c, d, a, x[ 1], S44, 0x85845dd1 );
	II( a, b, c, d, x[ 8], S41, 0x6fa87e4f );
	II( d, a, b, c, x[15], S42, 0xfe2ce6e0 );
	II( c, d, a, b, x[ 6], S43, 0xa3014314 );
	II( b, c, d, a, x[13], S44, 0x4e0811a1 );
	II( a, b, c, d, x[ 4], S41, 0xf7537e82 );
	II( d, a, b, c, x[11], S42, 0xbd3af235 );
	II( c, d, a, b, x[ 2], S43, 0x2ad7d2bb );
	II( b, c, d, a, x[ 9], S44, 0xeb86d391 );

	m_state[0] += a;
	m_state[1] += b;
	m_state[2] += c;
	m_state[3] += d;

	::memset( x, 0, sizeof( x ) );
}

void MD5::Encode( BYTE* dest, UInt4* src, UInt4 nLength )
{
	UInt4 i, j;

	assert( nLength % 4 == 0 );

	for( i = 0, j = 0; j < nLength ; i++, j += 4 )
	{
		dest[j]		= ( BYTE )( src[i] & 0xff );
		dest[j + 1]	= ( BYTE )( ( src[i] >> 8 ) & 0xff );
		dest[j + 2]	= ( BYTE )( ( src[i] >> 16 ) & 0xff );
		dest[j + 3]	= ( BYTE )( ( src[i] >> 24 ) & 0xff );
	} 
}

void MD5::Decode( UInt4* dest, BYTE* src, UInt4 nLength )
{
	UInt4 i, j;

	assert( nLength % 4 == 0 );

	for( i = 0, j = 0 ; j < nLength; i++, j += 4 )
	{
		dest[i] = ( ( UInt4 )src[j] ) | ( ( ( UInt4 )src[j + 1] ) << 8 ) |
			      ( ( ( UInt4 )src[j + 2] ) << 16 ) | ( ( ( UInt4 )src[j + 3] ) <<24 );
	}
}