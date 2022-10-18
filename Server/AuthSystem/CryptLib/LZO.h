#pragma once

#include "MiniLZO/minilzo.h"

#if defined( __LZO_STRICT_16BIT )
	#define __LZO_IN_LEN		( 8 * 1024u )
#elif defined( LZO_ARCH_I086 ) && !defined( LZO_HAVE_MM_HUGE_ARRAY )
	#define __LZO_IN_LEN		( 60 * 1024u )
#else
	#define __LZO_IN_LEN		( 128 * 1024ul )
#endif
#define __LZO_OUT_LEN			( __LZO_IN_LEN + __LZO_IN_LEN / 16 + 64 + 3 )

#define __LZO_HEAP_ALLOC( var, size )	\
    lzo_align_t __LZO_MMODEL var[( size + ( sizeof( lzo_align_t ) - 1 ) ) / sizeof( lzo_align_t )]

namespace Crypt
{
	class LZO
	{
	public:
				LZO();
				~LZO();

				bool			Compress( char* src, int srcLen, char* dest, int* destLen );
				bool			Decompress( char* src, int srcLen, char* dest, int* destLen );



				unsigned char	__LZO_MMODEL	m_dataBuffer[__LZO_IN_LEN];
				unsigned char	__LZO_MMODEL	m_outBuffer[__LZO_OUT_LEN];

	private:
				__LZO_HEAP_ALLOC( wrkmem, LZO1X_1_MEM_COMPRESS );
	};
}