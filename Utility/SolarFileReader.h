#ifndef __SOLAR_FILE_READER_H__
#define __SOLAR_FILE_READER_H__

//=============================================================================================================================
/// ���� ��ũ��Ʈ ����(txt)�б� ���� parser : SolarFileReader class
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2005. 1. 31
	@remarks
		- ������ scriptRead�� ��� ������
	@note
		- open mode : SEPERATOR_WHITESPACE
		  �����ڴ� white space�� �̴�.
		- open mode : SEPERATOR_QUOTATIONMARKFORSTR
		  �����ڴ� white space�̰� ���ڿ��� '"'����ǥ�� ǥ���Ѵ�.
		- ���ο� �Լ� ���� : ���� ���Ǹ� ���ؼ� : 2005.3.29
			char *				GetNextString();
			float				GetNextNumber();	
	@todo

	@history
		- 2005.2.24 : EOF�� ���� ����
		- 2005.4.12 : number�� datatype float -> double
		- string, number ���о��� �������� �˾Ƽ� �����ؼ� ����ϴ� ��� �߰�
		- 2005.5.11 : case '+': �׸� �߰�
		- 2005.8.22 : Reset()�Լ� �߰�, �ٽ� ù ��ġ���� �б� ����
		- 2005.9.23 : CPackFile�� �̿��� File�� �ҷ����� ���� OpenFileFromPack() �Լ� �߰�
		- 2005.9.26 : ���� ���๮�ڸ� �и��ڷ� �ν��ϴ� SEPERATOR_ONLY_NEWLINE Ÿ�԰� _getNextTokenType_newline() �Լ� �߰�
		- 2005.9.27 : ������� �и��ڷ� ����ϰ� ��� ��ū�� ���ڿ��θ� �ν��ϴ� SEPERATOR_WHITESPACE_ONLY_STRING Ÿ�԰� _getNextTokenType_whitespace_onlystring() �Լ� �߰�
		- 2006.7.19 : white space ��忡�� => [ 1003  0���� 34 5 7 ] => ��ũ��Ʈ�� ���� ���� �� '0����'�� ���ڷ� ���� ����! fixed
					  ������ ���� ���� üũ, ���� ��Ȯ�� Ÿ��üũ�� �ʿ� ex) 0.0.4(X), 0.f04(X), 0.4f(x)
	@usage
		- ���1
		SolarFileReader sr;
		sr.OpenFile( pszCalcCharFileName, SEPERATOR_QUOTATIONMARKFORSTR );

		while( TOKEN_END != sr.GetNextTokenType() )
		{		
			int iGetNumber = sr.GetTokenNumber();
			sr.GetNextTokenType( TOKEN_STRING );
			strncpy( pszCharName, sr.GetTokenString(), MAX_CHARNAME_LENGTH );
		}

		sr.CloseFile();

		- ���2
		SolarFileReader sr;
		sr.OpenFile( pszCalcCharFileName, SEPERATOR_QUOTATIONMARKFORSTR );

		while( !sr.IsEOF() )
		{
			int iGetNumber = sr.GetNextNumber();
			strncpy( pszCharName, sr.GetNextString(), MAX_CHARNAME_LENGTH );
		}

		sr.CloseFile();

*/
//=============================================================================================================================

#pragma once

enum eTOKEN_TYPE
{
	TOKEN_ANYTYPE			= -1,
	TOKEN_ERROR				= 0,
	TOKEN_END,
	TOKEN_NUMBER,
	TOKEN_STRING,
	TOKEN_EXTRA,

	TOKEN_SLASH				= '/',
	TOKEN_ASTERISK			= '*',

	// white-space
	TOKEN_SPACE				= ' ',
	TOKEN_NEWLINE			= '\n',
	TOKEN_CARRIAGERETURN	= '\r', 
	TOKEN_TAB				= '\t',
};

enum eSEPERATOR_MODE
{
	SEPERATOR_ERROR						= 0x00,
	SEPERATOR_WHITESPACE				= 0x01,
	SEPERATOR_QUOTATIONMARKFORSTR		= 0x02,
	SEPERATOR_ONLY_NEWLINE				= 0x03,	// ����, ����, ���鿡 ������� ������ new line ���� �и��ڷ� �ν�
	SEPERATOR_WHITESPACE_ONLY_STRING	= 0x04,  // �������� �и��Ǵ� ��� ��ū�� ������ ������
	// �ּ��� ���� Reader
	SEPERATOR_WHITESPACE_FOR_COMMENTS	= 0x05,	//< �ּ��κ��� whitespace ������� ����
};


namespace util 
{

class SolarFileReader
{
	enum 
	{
		MAX_TOKEN_BUFFER_SIZE			= 10240,
	};
public:
	SolarFileReader();
	~SolarFileReader();

	BOOL				OpenFile( const char * pszFileName, eSEPERATOR_MODE mode = SEPERATOR_WHITESPACE );

	BOOL				OpenFileFromPack(char * pszPackFileName, const char * pszFileName, eSEPERATOR_MODE mode);

	BOOL				OpenBuffer( BYTE * pMemoryBuffer, DWORD dwBufferSize, eSEPERATOR_MODE mode = SEPERATOR_WHITESPACE );
	VOID				CloseBuffer();
	VOID				CloseFile();

	eTOKEN_TYPE			GetNextTokenType( eTOKEN_TYPE desireTokenType = TOKEN_ANYTYPE );

	char *				GetTokenString() { return m_pszTokenString;	}
	double				GetTokenNumber() { return m_dTokenNumber;	}

	BOOL				IsEOF();
	char *				GetNextString( eTOKEN_TYPE desireTokenType = TOKEN_STRING );
	double				GetNextNumber( eTOKEN_TYPE desireTokenType = TOKEN_NUMBER );
	VOID				Reset();
	VOID				SetFileName( const TCHAR* pszFileName );
	const char *		GetFileName() { return m_szFileName; }
	const char *		GetBuffer() { return m_pszBuffer; }
	VOID				SetBuffer( char *pBuffer ) { m_pszBuffer = pBuffer; }
	VOID				SetMode( eSEPERATOR_MODE mode ){ m_Mode = mode; }
	eSEPERATOR_MODE		GetMode(){ return m_Mode; }
	DWORD				GetBufferSize() { return m_dwBufferSize; }
	VOID				SetBufferSize( DWORD dwSize) { m_dwBufferSize = dwSize; }
	VOID				SetBufferPos( DWORD dwPos )	{ m_dwBufferPos = dwPos; }

private:
	VOID				_clear();
	VOID				_clearToken();
	char				_getchar( DWORD * pPos = NULL );
	char				_getcurchar( DWORD * pPos = NULL );
	char				_getcurchar( DWORD pos );
	VOID				_rewind( int i , DWORD * pPos = NULL );
	int					_isspace( char c );
	int					_isnewline( char c );
	BOOL				_isnumber( char c );
	char				_skipComment( DWORD * INOUT pPos  = NULL );
	char				_skipNotComment( DWORD * INOUT pPos  = NULL );

	eTOKEN_TYPE			_getNextTokenType_whitespace( eTOKEN_TYPE desireTokenType );
	eTOKEN_TYPE			_getNextTokenType_quotationForStr( eTOKEN_TYPE desireTokenType );
	eTOKEN_TYPE			_getNextTokenType_newline( eTOKEN_TYPE desireTokenType );
	eTOKEN_TYPE			_getNextTokenType_whitespace_onlystring( eTOKEN_TYPE desireTokenType );
	eTOKEN_TYPE			_getNextTokenType_whitespace_forComments( eTOKEN_TYPE desireTokenType );

	char*				_getTokenTypeName(eTOKEN_TYPE TokenType);

	double				m_dTokenNumber;
	char				m_pszTokenString[MAX_TOKEN_BUFFER_SIZE];
	DWORD				m_dwBufferPos;
	DWORD				m_dwBufferSize;
	char *				m_pszBuffer;
	eSEPERATOR_MODE		m_Mode;
	char				m_szFileName[MAX_PATH];
	BOOL				m_bNewLine;

	char				m_szTokenType[64];

};


} /// namespace util


#endif // __SOLAR_FILE_READER_H__
