#include "StdAfx.h"
#include ".\stringcheck.h"
#include <Windows.h>

StringCheck::StringCheck(void)
{
}

StringCheck::~StringCheck(void)
{
}


BOOL StringCheck::Load( FILE * pFile, TCHAR * pszFileName)
{

	int iLength = _tcslen(pszFileName);
	// 파일 확장자 검사
	BOOL bFilter = ( pszFileName[iLength-1] == _T('h') || 
		pszFileName[iLength-1] =='p' && pszFileName[iLength-2] == _T('p') 
		&& pszFileName[iLength -3] == _T('c') );;

	if ( bFilter == FALSE )
		return FALSE;


	m_pFile = fopen(pszFileName , "rb");
	int line = 0;
	BOOL isFind = FALSE;
	while( ReadLine() )
	{

		// 축출 시작 (따음표 발견)
		BOOL bStart = FALSE;
		BOOL bRecord = FALSE;
		TCHAR szRecord[MAX_STRING_LENGTH];
		memset(szRecord , 0 , sizeof(szRecord) );
		int iLength = _tcslen(szString);
		if( iLength > MAX_STRING_LENGTH)
		{
			iLength = MAX_STRING_LENGTH;
		}
		int count = 0;
		for ( int i = 0; i < iLength ; i++ )
		{
			if(szString[i] == '"' && bStart == FALSE )
			{
				bStart = TRUE;
				continue;
			}
			if( bStart )
			{
				szRecord[count] = szString[i];
				count ++;
				if( IsHangul(szString[i]) )
				{
					bRecord = TRUE;
				}
				
				if ( szString[i] == '"')
				{
					if( bRecord)
					{
						int size = _tcslen(szRecord);
						if( size > 0 )
						{
							szRecord[count -1 ] = NULL;
							// 스트링만 축출
							fprintf(pFile , "%-50s   %s line : %d\n" , szRecord ,pszFileName , line +1); 
							// 전체 라인 축출
						//	fprintf(pFile , "%-50s   %s line : %d" , szString ,pszFileName , line +1);  // 전체 라인 축출
							isFind  = TRUE;
						}
					}
					count = 0;
					bStart = FALSE;
				}
			}
		}
		line += 1;
	}
	if( isFind )
	{
		fprintf(pFile , "-------------------------------------------------------------------------------\n");
	}

	fclose(m_pFile);
	return TRUE;
}


BOOL StringCheck::ReadLine()
{
	TCHAR ch;

	memset(szString , 0 , sizeof(szString) );

	for( int i = 0; i < MAX_STRING_LENGTH ; i++)
	{
		ch = fgetc(m_pFile);
		if( ch == EOF ) 
			return FALSE;
		szString[i] = ch;
		if( i > 0)
			if( szString[i -1] == 0x0D)
				if( szString[i] == 0x0A)
					break;
	}
	return TRUE;
}

BOOL StringCheck::IsHangul(TCHAR ch)
{
	if( ch & 0x80)
		return TRUE;
	else
		return FALSE;
}

BOOL StringCheck::ExtractString(FILE * pFile,  TCHAR * pszDirectory)
{
	TCHAR  szDirectory[MAX_PATH];

	
	sprintf(szDirectory , "%s\\\\*.*" , pszDirectory);
	TCHAR fname[MAX_PATH];
	WIN32_FIND_DATA wfd;
	HANDLE hSrch = FindFirstFile(szDirectory , &wfd );

	if( hSrch == INVALID_HANDLE_VALUE )
		return 0;
	BOOL bResult = TRUE;
	while (bResult )
	{
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( wfd.cFileName[0] != '.')
			{
				sprintf(szDirectory , "%s\\\\%s" ,pszDirectory, wfd.cFileName);
				ExtractString(pFile , szDirectory);
			}
		}
		else
		{
			sprintf(fname , "%s\\\\%s" , pszDirectory , wfd.cFileName );
			Load(pFile , fname);
		}

		bResult = FindNextFile(hSrch , &wfd);
	}
	FindClose(hSrch);

	return TRUE;
}
