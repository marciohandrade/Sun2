//#include "stdafx.h"
#include "UtilityCommon.h"
#include "AuMD5Encrypt.h"

AuMD5Encrypt::AuMD5Encrypt()
{
	Initialize();
}

AuMD5Encrypt::~AuMD5Encrypt()
{
	CleanUp();
}

bool AuMD5Encrypt::Initialize()
{
	bool			bResult;

	bResult = true;

	CryptSetProvider( MS_DEF_PROV, PROV_RSA_FULL );

	if(!CryptAcquireContext( &m_hCryptProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT ) )
	{
		DWORD			lError;
		lError = GetLastError();

		FILE		*file;

		file = fopen( "errorcode.txt", "ab" );
		
		if( file != NULL )
		{
			fwrite( &lError, 1, sizeof( lError ), file );

			fclose( file );
		}

		bResult = false;
		return bResult;
	}

	return bResult;
}

bool AuMD5Encrypt::CleanUp()
{
	if(m_hCryptProv) 
	   CryptReleaseContext(m_hCryptProv, 0);

	return true;
}

bool AuMD5Encrypt::EncryptString( const char *pstrHashString, char *pstrData, unsigned long istrSize )
{
	HCRYPTKEY		hKey; 
	HCRYPTHASH		hHash; 

	bool			bResult;

	bResult = true;

	//Hash를 만든다.
	if(!CryptCreateHash( m_hCryptProv, CALG_MD5, 0, 0, &hHash))
	{
		bResult = false;
		return bResult;
	}  

	//Hash Object에 Hash Data를 넣는다.
	if(!CryptHashData( hHash, (BYTE *)pstrHashString, (DWORD)strlen(pstrHashString), 0))
	{
		bResult = false;
		return bResult;
	}

	//Key를 뽑아낸다.
	if(!CryptDeriveKey( m_hCryptProv, ENCRYPT_ALGORITHM, hHash, KEYLENGTH, &hKey))
	{
		bResult = false;
		return bResult;
	}

	//Key를 뽑아냈으니 Hash OBJ를 초기화한다.
	CryptDestroyHash(hHash); 
	hHash = 0;

    //Key를 이용해 pstrData를 Encoding한다.
	//더이상 Encoding할게 없으니 Final인자를 TRUE로....
	if(!CryptEncrypt( hKey, 0, FALSE, 0, (unsigned char *)pstrData, &istrSize, istrSize))
	{ 
		bResult = false;
		return bResult;
	}

	//Key를 더이상 사용할 일은 없으므로 없앤다.
	CryptDestroyKey( hKey );

	return bResult;
}

bool AuMD5Encrypt::DecryptString( const char *pstrHashString, char *pstrData, unsigned long istrSize )
{
	HCRYPTKEY		hKey; 
	HCRYPTHASH		hHash; 

	bool			bResult;

	bResult = true;

	//Hash를 만든다.
	if(!CryptCreateHash( m_hCryptProv, CALG_MD5, 0, 0, &hHash))
	{
		bResult = false;
		return bResult;
	}  

	//Hash Object에 Hash Data를 넣는다.
	if(!CryptHashData( hHash, (BYTE *)pstrHashString, (DWORD)strlen(pstrHashString), 0))
	{
		bResult = false;
		return bResult;
	}

	//Key를 뽑아낸다.
	if(!CryptDeriveKey( m_hCryptProv, ENCRYPT_ALGORITHM, hHash, KEYLENGTH, &hKey))
	{
		bResult = false;
		return bResult;
	}

	//Key를 뽑아냈으니 Hash OBJ를 초기화한다.
	CryptDestroyHash(hHash); 
	hHash = 0;

	//더이상 Encoding할게 없으니 Final인자를 TRUE로....
	if(!CryptDecrypt( hKey, 0, FALSE, 0, (unsigned char *)pstrData, &istrSize))
	{ 
		bResult = false;
		return bResult;
	} 

	//Key를 더이상 사용할 일은 없으므로 없앤다.
	CryptDestroyKey( hKey );

	return bResult;
}
