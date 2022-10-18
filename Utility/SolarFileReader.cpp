#include "UtilityCommon.h"

#include <windows.h>
#include <assert.h>
#include ".\solarfilereader.h"
#include "GeneralFunction.h"
#include "FilePackerManager.h"
#include <ProgramCommon/wzFilePacker.h>



namespace util
{

//=====================================================================================================


SolarFileReader::SolarFileReader()
	:	m_pszBuffer ( NULL ),
		m_dTokenNumber ( 0.0 ),
		m_dwBufferSize ( 0 ),
		m_dwBufferPos ( 0 ),
		m_Mode ( SEPERATOR_ERROR )
{
	m_szFileName[0] = '\0';
	m_bNewLine = TRUE;		//���ʿ��� ������ ���ο� �����̴�.

}

SolarFileReader::~SolarFileReader()
{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	SAFE_DELETE( m_pszBuffer );
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	assert( m_pszBuffer == NULL );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

}

VOID SolarFileReader::_clear()
{
	_clearToken();
	m_szFileName[0] = NULL;
}

VOID	SolarFileReader::_clearToken()
{
	m_dTokenNumber = 0.0;
	m_pszTokenString[0] = NULL;
}

VOID SolarFileReader::CloseFile()
{
	CloseBuffer();
}

VOID SolarFileReader::CloseBuffer()
{
	if ( m_pszBuffer )
	{
		delete [] m_pszBuffer;
		m_pszBuffer = NULL;
	}
	m_dwBufferSize	= 0;
	m_dwBufferPos	= 0;


}

VOID SolarFileReader::Reset()
{
	m_dwBufferPos	= 0;
	_clear();
}

//-------------------------------------------------------------------------------------------
/**
*/

BOOL SolarFileReader::OpenBuffer( BYTE * pBuffer, DWORD dwBufferSize, eSEPERATOR_MODE mode )
{
	assert( NULL == m_pszBuffer );
	m_dwBufferSize	= dwBufferSize;
	m_pszBuffer = new char [dwBufferSize];
	memcpy( m_pszBuffer, pBuffer, dwBufferSize );

	// �ɹ� �ʱ�ȭ
	m_Mode = mode;
	m_dwBufferPos = 0;

	return TRUE;
}

BOOL SolarFileReader::OpenFile( const char * pszFileName, eSEPERATOR_MODE mode )
{
	assert( NULL == m_pszBuffer );
	ZeroMemory( m_szFileName, MAX_PATH );

	SetMode( mode );

	HANDLE hFile = ::CreateFile( pszFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if ( INVALID_HANDLE_VALUE == hFile )
	{
		return FALSE;
	}

	m_dwBufferSize = GetFileSize( hFile, NULL );
	if ( m_dwBufferSize < 0 )
	{
		return FALSE;
	}
	
	m_pszBuffer		= new char [m_dwBufferSize];
	m_dwBufferPos	= 0;

	DWORD dwNumber	= 0;
	BOOL rt = ::ReadFile( hFile, m_pszBuffer, m_dwBufferSize, &dwNumber, NULL );
	if( !rt ) 
	{
		return FALSE;
	}

	::CloseHandle( hFile );

	strncpy( m_szFileName, pszFileName, MAX_PATH );

	return TRUE;
}

char SolarFileReader::_getchar( DWORD * pPos )
{
	if( pPos == NULL )
		pPos = &m_dwBufferPos;

	if( *pPos < m_dwBufferSize )
		return m_pszBuffer[(*pPos)++];
	else
	{
		++(*pPos);
		return TOKEN_END;
	}
}

// ����!!! : char�߿� (TOKEN_END=1)�� �Ǵ� ���ڰ� ����???
char SolarFileReader::_getcurchar( DWORD * pPos )
{
	if( pPos == NULL )
		pPos = &m_dwBufferPos;

	return (char)( *pPos < m_dwBufferSize ? m_pszBuffer[*pPos] : TOKEN_END );
}
char SolarFileReader::_getcurchar( DWORD pos )
{
	return (char)( pos < m_dwBufferSize ? m_pszBuffer[pos] : TOKEN_END );
}

VOID SolarFileReader::_rewind( int i, DWORD * pPos )
{
	if( pPos == NULL )
		pPos = &m_dwBufferPos;

	assert( *pPos - i >= 0 );
	*pPos -= i;
}

int SolarFileReader::_isspace( char c )
{
	if( c == TOKEN_SPACE || c == TOKEN_NEWLINE || c == TOKEN_CARRIAGERETURN || c == TOKEN_TAB )
		return 1;
	return 0;
}

int SolarFileReader::_isnewline( char c )
{
	if( c == TOKEN_NEWLINE || c == TOKEN_CARRIAGERETURN )
		return 1;
	return 0;
}

BOOL SolarFileReader::_isnumber( char c )
{
	switch( c )
	{
		// ����
	case '0':	case '1':	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	case '8':	case '9':
		return TRUE;
	}

	return FALSE;
}

char	SolarFileReader::_skipComment( DWORD * INOUT pPos )
{
	char c = 0, nc = 0;
	BOOL bInfiniteLoop = TRUE;
	while( bInfiniteLoop )
	{
		c = _getchar(pPos);
		nc = _getcurchar(pPos);

		if( TOKEN_SLASH == c && TOKEN_SLASH == nc )
		{
			while( ( c = _getchar(pPos) ) != TOKEN_END && c != TOKEN_NEWLINE );
		}
		else if( TOKEN_SLASH == c && TOKEN_ASTERISK  == nc )
		{
			c = _getchar(pPos);// * ��������
			while ( ( c = _getchar(pPos) ) != TOKEN_END && c != TOKEN_ASTERISK && _getcurchar(pPos) != TOKEN_SLASH );
			c = _getchar(pPos);// * ��������
			c = _getchar(pPos);// / ��������
		}

		// ������ Ȥ�� ���� ���� üũ
		if( 0 == _isspace( c ) || c == TOKEN_END )
			break;
	}
	return c;
}

eTOKEN_TYPE	SolarFileReader::_getNextTokenType_whitespace( eTOKEN_TYPE desireTokenType )
{
    __UNUSED(desireTokenType);
	char c = _skipComment();
	if( c == TOKEN_END ) return TOKEN_END;

	eTOKEN_TYPE getTokenType = TOKEN_ERROR;

	if( _isnumber( c ) != 0  || c == '-' || c == '+' || c == '.' )
	{
		getTokenType = TOKEN_NUMBER;
	}
	else
	{
		getTokenType = TOKEN_STRING;
	}

	bool b_comma = false;
	char * pbp = &m_pszTokenString[0];
	*(pbp++) = c;
	while( ( c = _getchar() ) != TOKEN_END && _isspace( c ) == 0 )
	{
		if( _isnumber( c ) != 0 || ( b_comma == false && c == '.' ) /*|| c == 'f'*/ )
		{
			// ����	
			if( c == '.' )
				b_comma = true;
		}
		else
		{
			// ����
			getTokenType	= TOKEN_STRING;
		}
		*(pbp++) = c;
	}
	*pbp = '\0';
	m_dTokenNumber	= atof( m_pszTokenString );
	_rewind(1);

#ifndef _SERVER
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		WriteParserErrLog(1,_T("==============================================================================================="));
		WriteParserErrLog(1,_T("[ScriptError]: File[%s], desireToken[%s], getToken[%s]"), this->GetFileName(), _getTokenTypeName(desireTokenType) ,_getTokenTypeName(getTokenType));
		WriteParserErrLog(1,_T("TokenString : %s "), this->GetTokenString());
		WriteParserErrLog(1,_T("==============================================================================================="));
	}
#endif //_SERVER

#ifdef _DEBUG
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		assert( !"���ϴ� ��ū Ÿ�԰� �ٸ��ϴ�. : ������ ������ ���ɼ��� Ů�ϴ�" );
	}
#endif //_DEBUG

	return getTokenType;
}

eTOKEN_TYPE	 SolarFileReader::_getNextTokenType_quotationForStr( eTOKEN_TYPE desireTokenType )
{
    __UNUSED(desireTokenType);
	char c = _skipComment();
	if( c == TOKEN_END ) return TOKEN_END;

	eTOKEN_TYPE getTokenType = TOKEN_ERROR;

	char * pbp = &m_pszTokenString[0];

	switch( c )
	{
	case TOKEN_END: break;
	// ����
	case '0':	case '1':	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	case '8':	case '9':
	case '.':	case '-':	case '+':
		{
			*(pbp++) = c;
			while( ( c = _getchar() ) != TOKEN_END && ( isdigit( c ) != 0 || c == '.'/* || c == 'f'*/) )
			{
				*(pbp++) = c;
			}
			*pbp = NULL;
			_rewind(1);
			m_dTokenNumber	= atof( m_pszTokenString );
			getTokenType	= TOKEN_NUMBER;
		}	
		break;
	case '"': //< ���ڿ�
		{
			while( ( c = _getchar() ) != TOKEN_END && ( c != '"' ) )
			{
				// 2007.12.17 / lammy / 2byte �������� �˻�	2byte������ ��� ���� ���ڱ��� �о�´�. / �Ϻ����� ��� ��ġ�� �ڵ�(��:\\)�� �ִ�.
				if( _ismbblead( c ) )
				{
					*(pbp++) = c;
					c = _getchar();
					*(pbp++) = c;
				}
				else
				{
					if( c == '\\') c = _getchar();
					*(pbp++) = c;
				}
			}
			*pbp = NULL;
			getTokenType	= TOKEN_STRING;
		}
		break;
	default: //< ���Ŀ� ���� �ʴ� ���ڵ�
		{
			*(pbp++) = c;
			*pbp = NULL;
			getTokenType	= TOKEN_EXTRA;
		}
	}

#ifndef _SERVER
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		WriteParserErrLog(1,_T("==============================================================================================="));
		WriteParserErrLog(1,_T("[ScriptError]: File[%s], desireToken[%s], getToken[%s]"), this->GetFileName(), _getTokenTypeName(desireTokenType) ,_getTokenTypeName(getTokenType));
		WriteParserErrLog(1,_T("TokenString : %s "), this->GetTokenString());
		WriteParserErrLog(1,_T("==============================================================================================="));
	}
#endif //_SERVER

#ifdef _DEBUG
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		assert( !"���ϴ� ��ū Ÿ�԰� �ٸ��ϴ�. : ������ ������ ���ɼ��� Ů�ϴ�" );
	}
#endif //_DEBUG

	return getTokenType;
}

eTOKEN_TYPE	SolarFileReader::_getNextTokenType_newline( eTOKEN_TYPE desireTokenType )
{
    __UNUSED(desireTokenType);
	char c = _skipComment();
	if( c == TOKEN_END ) return TOKEN_END;

	eTOKEN_TYPE getTokenType = TOKEN_ERROR;

	char * pbp = &m_pszTokenString[0];

	switch( c )
	{
	case TOKEN_END: break;
	default: // ����, ����, ���鿡 ������� ������ new line ���� �и��ڷ� �ν�
		{
			*(pbp++) = c;
			while( ( c = _getchar() ) != TOKEN_END && _isnewline( c ) == 0 )
	
			{
				*(pbp++) = c;
			}
			*pbp = NULL;
			_rewind(1);
			getTokenType	= TOKEN_STRING;
		}
	}

#ifndef _SERVER
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		WriteParserErrLog(1,_T("==============================================================================================="));
		WriteParserErrLog(1,_T("[ScriptError]: File[%s], desireToken[%s], getToken[%s]"), this->GetFileName(), _getTokenTypeName(desireTokenType) ,_getTokenTypeName(getTokenType));
		WriteParserErrLog(1,_T("TokenString : %s "), this->GetTokenString());
		WriteParserErrLog(1,_T("==============================================================================================="));
	}
#endif //_SERVER

#ifdef _DEBUG
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		assert( !"���ϴ� ��ū Ÿ�԰� �ٸ��ϴ�. : ������ ������ ���ɼ��� Ů�ϴ�" );
	}
#endif //_DEBUG

	return getTokenType;
}

eTOKEN_TYPE	SolarFileReader::_getNextTokenType_whitespace_onlystring( eTOKEN_TYPE desireTokenType )
{
    __UNUSED(desireTokenType);
	char c = _skipComment();
	if( c == TOKEN_END ) return TOKEN_END;

	eTOKEN_TYPE getTokenType = TOKEN_ERROR;

	char * pbp = &m_pszTokenString[0];

	switch( c )
	{
	case TOKEN_END: break;
	default: // ������ ���ڿ��θ� �ν�
		{
			*(pbp++) = c;
			while( ( c = _getchar() ) != TOKEN_END && _isspace( c ) == 0 )
	
			{
				*(pbp++) = c;
			}
			*pbp = NULL;
			_rewind(1);
			getTokenType	= TOKEN_STRING;
		}
	}

#ifndef _SERVER
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		WriteParserErrLog(1,_T("==============================================================================================="));
		WriteParserErrLog(1,_T("[ScriptError]: File[%s], desireToken[%s], getToken[%s]"), this->GetFileName(), _getTokenTypeName(desireTokenType) ,_getTokenTypeName(getTokenType));
		WriteParserErrLog(1,_T("TokenString : %s "), this->GetTokenString());
		WriteParserErrLog(1,_T("==============================================================================================="));
	}
#endif //_SERVER

#ifdef _DEBUG
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		assert( !"���ϴ� ��ū Ÿ�԰� �ٸ��ϴ�. : ������ ������ ���ɼ��� Ů�ϴ�" );
	}
#endif //_DEBUG

	return getTokenType;
}

eTOKEN_TYPE SolarFileReader::GetNextTokenType( eTOKEN_TYPE desireTokenType )
{
//	_clear();  //Ŭ����� �����̸��� �ʱ�ȭ ���� ������. �ʱ�ȭ ��ų ������ ����.
	_clearToken();

	switch( m_Mode )
	{
	case SEPERATOR_WHITESPACE:
		return _getNextTokenType_whitespace( desireTokenType );
	case SEPERATOR_QUOTATIONMARKFORSTR:
		return _getNextTokenType_quotationForStr( desireTokenType );
	case SEPERATOR_ONLY_NEWLINE:
		return _getNextTokenType_newline( desireTokenType );
	case SEPERATOR_WHITESPACE_ONLY_STRING :
		return _getNextTokenType_whitespace_onlystring( desireTokenType );
	case SEPERATOR_WHITESPACE_FOR_COMMENTS :
		return _getNextTokenType_whitespace_forComments( desireTokenType );
	}
	return TOKEN_ERROR;
}

BOOL SolarFileReader::IsEOF()
{
	DWORD m_CopyCurPos = m_dwBufferPos;
	if( _skipComment( &m_CopyCurPos ) == TOKEN_END )  return TRUE;
	return FALSE;
}

char * SolarFileReader::GetNextString( eTOKEN_TYPE desireTokenType )
{
	GetNextTokenType( desireTokenType );
	return GetTokenString();
}

double SolarFileReader::GetNextNumber( eTOKEN_TYPE desireTokenType )
{
	GetNextTokenType( desireTokenType );
	return GetTokenNumber();
}

eTOKEN_TYPE	SolarFileReader::_getNextTokenType_whitespace_forComments( eTOKEN_TYPE desireTokenType )
{
    __UNUSED(desireTokenType);
	char c = _skipNotComment();
	if( c == TOKEN_END ) return TOKEN_END;

	// �ּ� ������ " " �� �ð�� ó����, �ּ��� �ְ� �ƹ��͵� ���� �� �ϰ���� ����ó�� ����.
	while( _isspace(c) == 1 )
	{
		c = _getchar();

		if( c == TOKEN_END ) return TOKEN_END;
		if( c == TOKEN_NEWLINE )
		{
			m_bNewLine = TRUE;
			c = _skipNotComment();
		}
	}

	eTOKEN_TYPE getTokenType = TOKEN_ERROR;

	if( _isnumber( c ) != 0  || c == '-' || c == '+' || c == '.' )
	{
		getTokenType = TOKEN_NUMBER;
	}
	else
	{
		getTokenType = TOKEN_STRING;
	}

	bool b_comma = false;
	char* pbp = &m_pszTokenString[0];
	//���ʿ� _skipNotComment���� ���ϵ� ���� pbp�� m_pszTokenString[0]�� ���� �����Ŀ� ++ �Ѵ�.
	*(pbp++) = c;

	// ���⳪�� ������ ���� "abc " �̷������� �ڿ� ������ ���ö� ���Ǵ� �����̴�. 
	// ���� ���� ���� ó������  " " �ϼ��� �ִٴ� ������ ����.
	while( ( c = _getchar() ) != TOKEN_END && _isspace( c ) == 0 )
	{
		if( _isnumber( c ) != 0 || ( b_comma == false && c == '.' ) /*|| c == 'f'*/ )
		{
			// ����	
			if( c == '.' )
				b_comma = true;
		}
		else
		{
			// ����
			getTokenType	= TOKEN_STRING;
		}
		*(pbp++) = c;
	}
	*pbp = '\0';
	m_dTokenNumber	= atof( m_pszTokenString );
	_rewind(1);

#ifndef _SERVER
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		WriteParserErrLog(1,_T("==============================================================================================="));
		WriteParserErrLog(1,_T("[ScriptError]: File[%s], desireToken[%s], getToken[%s]"), this->GetFileName(), _getTokenTypeName(desireTokenType) ,_getTokenTypeName(getTokenType));
		WriteParserErrLog(1,_T("TokenString : %s "), this->GetTokenString());
		WriteParserErrLog(1,_T("==============================================================================================="));
	}
#endif //_SERVER

#ifdef _DEBUG
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		assert( !"���ϴ� ��ū Ÿ�԰� �ٸ��ϴ�. : ������ ������ ���ɼ��� Ů�ϴ�" );
	}
#endif //_DEBUG

	return getTokenType;
}


// �ּ��� �ƴѰ��� ��ŵ�Ѵ�. �ּ��� ���, �ּ� ������ ������ �����Ѵ�.
char	SolarFileReader::_skipNotComment( DWORD * INOUT pPos )
{
	char c = 0, nc = 0;
	BOOL bInfiniteLoop = TRUE;
	while( bInfiniteLoop )
	{
		c = _getchar(pPos);
		nc = _getcurchar(pPos);

		// ���� ���ο� ������ �ƴ϶��, ������ ���� �ּ��� �Ľ����ε�
		// ���� ���Դٴ� ���� �ǹ��Ѵ�. ���� �׳� �����Ѵ�.
		if( m_bNewLine == FALSE )
			return c;

		// ���� c�� nc�� �ּ��̸�, �������� ���� �е��� �ϰ�, �ƴϸ� 
		if( TOKEN_SLASH == c && TOKEN_SLASH == nc )
		{
			c = _getchar(pPos);	// / ��������
			c = _getchar(pPos);	// / ��������
			m_bNewLine = FALSE;
			break;
		}
		else if( TOKEN_SLASH == c && TOKEN_ASTERISK  == nc )
		{
			c = _getchar(pPos);	// / ��������
			c = _getchar(pPos);	// * ��������
			m_bNewLine = FALSE;
			break;
		}

		// ���ο� �����ε� �ּ� üũ�� �ɸ��� �ʾҴ�? ==> �Ϲ� �����ʹ�. ���� ������ �ѱ��.
		while( ( c = _getchar(pPos) ) != TOKEN_END && c != TOKEN_NEWLINE );
		m_bNewLine = TRUE;

		// ������ Ȥ�� ���� ���� üũ
		if( 0 == _isspace( c ) || c == TOKEN_END )
			break;
	}

	return c;
}

VOID	SolarFileReader::SetFileName( const TCHAR* pszFileName )
{
	strncpy( m_szFileName, pszFileName, MAX_PATH );
	m_szFileName[MAX_PATH-1] = '\0';
}

char* SolarFileReader::_getTokenTypeName(eTOKEN_TYPE TokenType)
{
	
	ZeroMemory(m_szTokenType, sizeof(m_szTokenType));

	switch(TokenType)
	{
	case TOKEN_ANYTYPE:
		strcpy(m_szTokenType, "TOKEN_ANYTYPE");
		break;

	case TOKEN_ERROR:
		strcpy(m_szTokenType, "TOKEN_ERROR");
		break;

	case TOKEN_END:
		strcpy(m_szTokenType, "TOKEN_END");
		break;

	case TOKEN_NUMBER:
		strcpy(m_szTokenType, "TOKEN_NUMBER");
		break;

	case TOKEN_STRING:
		strcpy(m_szTokenType, "TOKEN_STRING");
		break;

	case TOKEN_EXTRA:
		strcpy(m_szTokenType, "TOKEN_EXTRA");
		break;

	case TOKEN_SLASH:
		strcpy(m_szTokenType, "TOKEN_SLASH");
		break;

	case TOKEN_ASTERISK:
		strcpy(m_szTokenType, "TOKEN_ASTERISK");
		break;


	case TOKEN_SPACE:
		strcpy(m_szTokenType, "TOKEN_SPACE");
		break;

	case TOKEN_NEWLINE:
		strcpy(m_szTokenType, "TOKEN_NEWLINE");
		break;

	case TOKEN_CARRIAGERETURN:
		strcpy(m_szTokenType, "TOKEN_CARRIAGERETURN");
		break;

	case TOKEN_TAB:
		strcpy(m_szTokenType, "TOKEN_TAB");
		break;

	default:
		strcpy(m_szTokenType, "TOKEN_UNKNOWN");
		break;
	}

	return m_szTokenType;
}


BOOL SolarFileReader::OpenFileFromPack(char * pszPackFileName, const char * pszFileName, eSEPERATOR_MODE mode)
{
	char szMsg[256] = {0,};
	_snprintf(szMsg, 255, "<<ReadFile>> %s From PackFile", pszFileName);

	DWORD	dwSize = 0;
	HANDLE	hFile = 0;

	CPackFile* pPackFile = PACKER_MANAGER()->GetPackFile( pszPackFileName, pszFileName );

	if(pPackFile)
	{
		hFile = pPackFile->OpenFile( pszFileName, PACKFILE_OPEN_READONLY );

		if ( FALSE == hFile )
		{
			return FALSE;
		}
		else
		{
            ULONGLONG llSize;
			if( !pPackFile->GetFileSize( hFile, &llSize ) )
			{
				pPackFile->CloseFile( hFile );
				return FALSE;
			}

	        dwSize = (DWORD)llSize;
			if ( dwSize <= 0 )
			{
				pPackFile->CloseFile( hFile );
				return FALSE;
			}

			m_pszBuffer = new char[dwSize];

			BOOL bResult = pPackFile->ReadFile(hFile, m_pszBuffer, dwSize);
			if( bResult )
			{	
				SetMode( mode );
				SetBufferSize( dwSize );
				SetBufferPos( 0 );

				pPackFile->CloseFile( hFile );
				return TRUE;
			}

			pPackFile->CloseFile( hFile );
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	// ���� �ڵ�� ���� �������� �þ ��� ����� ��.
	
}


} //util namespace


