#include "SunClientPrecompiledHeader.h"
#include "MD5Encrypt.h"

//#include "mmsystem.h"

char *g_pstrCryptContainer = "MyKeyContainer";  

MD5Encrypt::MD5Encrypt()
{
	Initialize();
}

MD5Encrypt::~MD5Encrypt()
{
	CleanUp();
}

bool MD5Encrypt::Initialize()
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

bool MD5Encrypt::CleanUp()
{
	if(m_hCryptProv) 
	   CryptReleaseContext(m_hCryptProv, 0);

	return true;
}

bool MD5Encrypt::EncryptString( char *pstrHashString, char *pstrData, unsigned long istrSize )
{
	HCRYPTKEY		hKey; 
	HCRYPTHASH		hHash; 

	bool			bResult;

	bResult = true;

	//Hash�� �����.
	if(!CryptCreateHash( m_hCryptProv, CALG_MD5, 0, 0, &hHash))
	{
		bResult = false;
		return bResult;
	}  

	//Hash Object�� Hash Data�� �ִ´�.
	if(!CryptHashData( hHash, (BYTE *)pstrHashString, (DWORD)strlen(pstrHashString), 0))
	{
		bResult = false;
		return bResult;
	}

	//Key�� �̾Ƴ���.
	if(!CryptDeriveKey( m_hCryptProv, ENCRYPT_ALGORITHM, hHash, KEYLENGTH, &hKey))
	{
		bResult = false;
		return bResult;
	}

	//Key�� �̾Ƴ����� Hash OBJ�� �ʱ�ȭ�Ѵ�.
	CryptDestroyHash(hHash); 
	hHash = 0;

    //Key�� �̿��� pstrData�� Encoding�Ѵ�.
	//���̻� Encoding�Ұ� ������ Final���ڸ� TRUE��....
	if(!CryptEncrypt( hKey, 0, FALSE, 0, (unsigned char *)pstrData, &istrSize, istrSize))
	{ 
		bResult = false;
		return bResult;
	}

	//Key�� ���̻� ����� ���� �����Ƿ� ���ش�.
	CryptDestroyKey( hKey );

	return bResult;
}

bool MD5Encrypt::DecryptString( char *pstrHashString, char *pstrData, unsigned long istrSize )
{
	HCRYPTKEY		hKey; 
	HCRYPTHASH		hHash; 

	bool			bResult;

	bResult = true;

	//Hash�� �����.
	if(!CryptCreateHash( m_hCryptProv, CALG_MD5, 0, 0, &hHash))
	{
		bResult = false;
		return bResult;
	}  

	//Hash Object�� Hash Data�� �ִ´�.
	if(!CryptHashData( hHash, (BYTE *)pstrHashString, (DWORD)strlen(pstrHashString), 0))
	{
		bResult = false;
		return bResult;
	}

	//Key�� �̾Ƴ���.
	if(!CryptDeriveKey( m_hCryptProv, ENCRYPT_ALGORITHM, hHash, KEYLENGTH, &hKey))
	{
		bResult = false;
		return bResult;
	}

	//Key�� �̾Ƴ����� Hash OBJ�� �ʱ�ȭ�Ѵ�.
	CryptDestroyHash(hHash); 
	hHash = 0;

	//���̻� Encoding�Ұ� ������ Final���ڸ� TRUE��....
	if(!CryptDecrypt( hKey, 0, FALSE, 0, (unsigned char *)pstrData, &istrSize))
	{ 
		bResult = false;
		return bResult;
	} 

	//Key�� ���̻� ����� ���� �����Ƿ� ���ش�.
	CryptDestroyKey( hKey );

	return bResult;
}

char *MD5Encrypt::GetRandomHashString( int iStringSize )
{
	char			*pstrHashString;

	pstrHashString = NULL;

	if( (0 < iStringSize) && (iStringSize<100000) )
	{
		pstrHashString = new char[iStringSize];
		memset( pstrHashString, 0, iStringSize );

		if( pstrHashString != NULL )
		{
			srand( timeGetTime() );

			int		limit1 = iStringSize-1;

			for( int i=0; i<limit1; ++i )
			{
				pstrHashString[i] = (char)(rand()%256);
			}
		}
	}

	return pstrHashString;
}
