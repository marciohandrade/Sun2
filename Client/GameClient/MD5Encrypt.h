#ifndef __MD5ENCRYPT_H__
#define __MD5ENCRYPT_H__

#include <stdio.h>

#include <Windows.h>
#include <wincrypt.h>
#define KEYLENGTH  CRYPT_CREATE_SALT

#define ENCRYPT_ALGORITHM CALG_RC4 

#define MD5_HASH_KEY_STRING						"1111"

#if _MSC_VER < 1300
#pragma comment ( lib , "AuMD5EnCrypt" )
#endif

class MD5Encrypt
{
private:
	HCRYPTPROV		m_hCryptProv; 

	bool Initialize();
	bool CleanUp();

public:
	MD5Encrypt();
	~MD5Encrypt();
	bool EncryptString( char *pstrHashString, char *pstrData, unsigned long istrSize );
	bool DecryptString( char *pstrHashString, char *pstrData, unsigned long istrSize );
	char *GetRandomHashString( int iStringSize );
};

#endif