#ifndef __SOLAR_FILE_READER_H__
#define __SOLAR_FILE_READER_H__

//=============================================================================================================================
/// 각종 스크립트 파일(txt)읽기 위한 parser : SolarFileReader class
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2005. 1. 31
	@remarks
		- 기존의 scriptRead의 기능 개선판
	@note
		- open mode : SEPERATOR_WHITESPACE
		  구분자는 white space만 이다.
		- open mode : SEPERATOR_QUOTATIONMARKFORSTR
		  구분자는 white space이고 문자열은 '"'따옴표로 표시한다.
		- 새로운 함수 제작 : 유저 편의를 위해서 : 2005.3.29
			char *				GetNextString();
			float				GetNextNumber();	
	@todo

	@history
		- 2005.2.24 : EOF의 버그 수정
		- 2005.4.12 : number의 datatype float -> double
		- string, number 구분없이 로직에서 알아서 구분해서 사용하는 모드 추가
		- 2005.5.11 : case '+': 항목 추가
		- 2005.8.22 : Reset()함수 추가, 다시 첫 위치부터 읽기 위해
		- 2005.9.23 : CPackFile을 이용해 File을 불러오기 위해 OpenFileFromPack() 함수 추가
		- 2005.9.26 : 오직 개행문자만 분리자로 인식하는 SEPERATOR_ONLY_NEWLINE 타입과 _getNextTokenType_newline() 함수 추가
		- 2005.9.27 : 빈공간을 분리자로 사용하고 모든 토큰을 문자열로만 인식하는 SEPERATOR_WHITESPACE_ONLY_STRING 타입과 _getNextTokenType_whitespace_onlystring() 함수 추가
		- 2006.7.19 : white space 모드에서 => [ 1003  0음냥 34 5 7 ] => 스크립트가 옆과 같을 때 '0음냥'을 숫자로 보는 버그! fixed
					  엄격한 문법 오류 체크, 좀더 정확한 타입체크가 필요 ex) 0.0.4(X), 0.f04(X), 0.4f(x)
	@usage
		- 방법1
		SolarFileReader sr;
		sr.OpenFile( pszCalcCharFileName, SEPERATOR_QUOTATIONMARKFORSTR );

		while( TOKEN_END != sr.GetNextTokenType() )
		{		
			int iGetNumber = sr.GetTokenNumber();
			sr.GetNextTokenType( TOKEN_STRING );
			strncpy( pszCharName, sr.GetTokenString(), MAX_CHARNAME_LENGTH );
		}

		sr.CloseFile();

		- 방법2
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
	SEPERATOR_ONLY_NEWLINE				= 0x03,	// 숫자, 문자, 공백에 관계없이 무조건 new line 만을 분리자로 인식
	SEPERATOR_WHITESPACE_ONLY_STRING	= 0x04,  // 공백으로 분리되는 모든 토큰은 무조건 문자임
	// 주석에 대한 Reader
	SEPERATOR_WHITESPACE_FOR_COMMENTS	= 0x05,	//< 주석부분을 whitespace 방식으로 읽음
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
