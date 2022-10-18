#pragma once

namespace Crypt
{
	class SHA1
	{
	public:
				SHA1();
				~SHA1();

		static	void	EncodeString( BYTE* szString, BYTE digest[20] );
		static	void	EncodeFile( BYTE* szFilename, BYTE digest[20] );


				void	Initialize();
				void	Update( BYTE* data, unsigned int len );
				void	Finalize( BYTE digest[20] );

	private:
				void	Transform( unsigned long state[5], BYTE buffer[64] );


				typedef struct
				{
					unsigned long	state[5];
					unsigned long	count[2];
					BYTE	buffer[64];
				} SHA1_CTX;

				SHA1_CTX		m_ctx;
	};
}