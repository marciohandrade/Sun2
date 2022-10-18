
#include "CryptFile.h"
#include <stdio.h>

CryptFile::~CryptFile()
{
	Close();
}

//BOOL	CryptFile::IsExistFile( CHAR* IN pszFileName )
//{
//}

BOOL	CryptFile::Load( BYTE* OUT pBuffer, DWORD IN OUT dwLen )
{
	if( !pBuffer || !dwLen )
	{
		printf( "Check Argument\n" );
		return FALSE;
	}

	if( !GetFD() )
	{
		printf( "Can't Load file\n" );
		return FALSE;
	}

	DWORD dwReaded=0;

	if( !ReadFile( GetFD(), pBuffer, dwLen, &dwReaded, NULL ) )
	{
		printf( "Can't Read File\n" );
		return FALSE;
	}

	if( dwReaded != dwLen )
	{
		printf( "Corrupt File\n" );
		return FALSE;
	}

	return TRUE;

	//while( !feof(GetFD()) )
	//{
	//	ret = fread( pBuffer+readed, sizeof(BYTE), dwLen, GetFD() );
	//	if( ret != 0 )
	//	{
	//		readed+=ret;
	//	}
	//}

	//FILE* ppp = fopen( "test2.dat", "w+" );
	//fwrite( pBuffer, sizeof(BYTE), dwLen, ppp );
	//fflush( ppp );

	////// 아래 루틴은 파일 내용을 모두 읽지 못한다.
	////readed = fread( pBuffer+readed, sizeof(BYTE), dwLen, GetFD() );

	//if( readed < 0 /*|| readed != (size_t)dwLen*/ )
	//	return FALSE;

	return TRUE;
}


BOOL	CryptFile::Save( BYTE* IN pBuffer, DWORD IN dwLen )
{
	if( !pBuffer || !dwLen )
	{
		printf( "Check Argument\n" );
		return FALSE;
	}

	if( !GetFD() )
	{
		printf( "Can't Load file\n" );
		return FALSE;
	}

	DWORD dwWritten;
	if( !WriteFile( GetFD(), pBuffer, dwLen, &dwWritten, NULL ) )
	{
		printf( "Can't Write File\n" );
		return FALSE;
	}

	if( dwWritten != dwLen )
	{
		printf( "Corrupt File\n" );
		return FALSE;
	}

	//fpos_t pos = 0;
	//fsetpos( GetFD(), &pos );
	//size_t written=0, ret;
	//do {
	//	ret = fwrite( pBuffer+written, sizeof(BYTE), dwLen, GetFD() );
	//	if( ret != 0 )
	//		written+= ret;
	//} while( written != dwLen );
	//fflush( GetFD() );
	//if( written < 0 /*|| written != (size_t)dwLen*/ )
	//	return FALSE;
	return TRUE;
}

VOID	CryptFile::Close()
{
	if( !GetFD() || ( GetFD() == INVALID_HANDLE_VALUE ) )
		return;

	FlushFileBuffers( GetFD() );
	CloseHandle( GetFD() );
	//fflush( GetFD() );
	//fclose( GetFD() );
	SetFD(NULL);
}


