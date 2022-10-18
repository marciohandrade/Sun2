#ifndef __AUMD5ENCRYPT_H__
#define __AUMD5ENCRYPT_H__

#include <stdio.h>

#include <Windows.h>
#include <wincrypt.h>
#define KEYLENGTH  CRYPT_CREATE_SALT

#define ENCRYPT_ALGORITHM CALG_RC4 

class AuMD5Encrypt
{
private:
	HCRYPTPROV		m_hCryptProv; 

	bool Initialize();
	bool CleanUp();

public:
	AuMD5Encrypt();
	~AuMD5Encrypt();
	bool EncryptString( const char *pstrHashString, char *pstrData, unsigned long istrSize );
	bool DecryptString( const char *pstrHashString, char *pstrData, unsigned long istrSize );
};

#endif