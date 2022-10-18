// WZScriptEncode.cpp: implementation of the CWZScriptEncode class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "WZScriptEncode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWZScriptEncode::CWZScriptEncode()
{
	m_pBuffer = NULL;
	m_iBufferSize = 0;
	m_iBufferCount = 0;
	m_hFile = INVALID_HANDLE_VALUE;
	m_btFileType = ENCRYPT;	// 일반 게임파일

	m_XorTable[0] = 0xA1;
	m_XorTable[1] = 0xB2;
	m_XorTable[2] = 0xAA;
	m_XorTable[3] = 0x12;
	m_XorTable[4] = 0x23;
	m_XorTable[5] = 0xF1;
	m_XorTable[6] = 0xF3;
	m_XorTable[7] = 0xD3;
	m_XorTable[8] = 0x78;
	m_XorTable[9] = 0x02;

	m_EtcXorTable[0] = 0xF3;
	m_EtcXorTable[1] = 0xC2;
	m_EtcXorTable[2] = 0x3A;
	m_EtcXorTable[3] = 0xF2;
	m_EtcXorTable[4] = 0x23;
}

CWZScriptEncode::~CWZScriptEncode()
{
	Close();
}

int	CWZScriptEncode::Open(char* filename, BYTE btFileType)
{
	m_btFileType = btFileType;
	if( m_hFile !=  INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
	}
	
	m_hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if( m_hFile == INVALID_HANDLE_VALUE )
		return 0;

	int filesize = GetFileSize(m_hFile, NULL);

	char* pBuffer = (char*)malloc(filesize);

	DWORD iReadFileSize;
	ReadFile(m_hFile, pBuffer, filesize, &iReadFileSize, NULL);

	if( iReadFileSize == 0 )
	{
		Close();
		return 0;
	}	

	Encode(pBuffer, iReadFileSize);
	SetScriptParsingBuffer(pBuffer, iReadFileSize);

	CloseHandle(m_hFile);
	return 1;
}

void	CWZScriptEncode::Close()
{
	if( m_pBuffer )
		free(m_pBuffer);
}

void CWZScriptEncode::Encode(char* buffer, int size )
{
	if( m_btFileType == ETC_ENCRYPT )
	{
		for( int i = 0; i < size; i++ )
		{
			buffer[i] = buffer[i]^m_EtcXorTable[i%5];
		}
	} else {
		for( int i = 0; i < size; i++ )
		{
			buffer[i] = buffer[i]^m_XorTable[i%MAX_XORCODE_TABLE_COUNT];
		}
	}
}

int CWZScriptEncode::GetC()
{
	if( m_iBufferCount >= m_iBufferSize )
		return EOF;

	return m_pBuffer[m_iBufferCount++];
}

void CWZScriptEncode::UnGetC(int ch)
{
	if( m_iBufferCount <= 0 )
		return;
	
	m_iBufferCount--;
	m_pBuffer[m_iBufferCount] = ch;
}

void CWZScriptEncode::SetScriptParsingBuffer(char* buffer, int size)
{
	m_pBuffer = buffer;
	m_iBufferSize = size;
	m_iBufferCount = 0;
}

int	CWZScriptEncode::GetNumber()
{	
	return	(int)TokenNumber;
}

char* CWZScriptEncode::GetString()
{
	return	TokenString;
}

WZSMDToken CWZScriptEncode::GetToken()
{
	char ch;
	TokenString[0] = '\0';
	do
	{
		if ( (ch =(char) GetC()) == EOF) return T_END;
		if (ch=='/' && (ch =(char) GetC() )=='/')	
		{
			while( (ch = (char) GetC()) != '\n' );
		}
	} while(  isspace(ch) );
	
	char *p, TempString[100];
	switch(ch)
	{	
	case '#':
		return CurrentToken = T_COMMAND;
	case ';':
		return CurrentToken = T_SEMICOLON;
	case ',':
		return CurrentToken = T_COMMA;
	case '{':
		return CurrentToken = T_LP;
	case '}':
		return CurrentToken = T_RP;
	case '0':	case '1':	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	case '8':	case '9':
	case '.':	case '-':
		UnGetC(ch);
		p = TempString;
		while ( (  (ch = GetC() ) !=EOF) && (ch=='.' || isdigit(ch) || ch=='-') )
			*p++ = ch;
		*p = 0;
		TokenNumber = (float)atof(TempString);
		//			sscanf(TempString," %f ",&TokenNumber);
		return CurrentToken = T_NUMBER;
	case '"':
		p = TokenString;
		while ( (  (ch = GetC() ) !=EOF) && (ch!='"'))// || isalnum(ch)) )
			*p++ = ch;
		if (ch!='"')
			UnGetC(ch);
		*p = 0;
		return CurrentToken = T_NAME;
	default:
		if (isalpha(ch))	
		{
			p = TokenString;
			*p++ = ch;
			while ( (  (ch = GetC() ) !=EOF) && (ch=='.' || ch=='_' || isalnum(ch)) )
				*p++ = ch;
			UnGetC(ch);
			*p = 0;
			return CurrentToken = T_NAME;
		}
		return CurrentToken = T_SMD_ERROR;
	}
}