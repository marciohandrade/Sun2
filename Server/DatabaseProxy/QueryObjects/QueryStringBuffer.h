#pragma once

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<TYPE DEFINITIONs>
#define MAX_QUERY_LENGTH_32		32
#define MAX_QUERY_LENGTH_64		64
#define MAX_QUERY_LENGTH_128	128
#define MAX_QUERY_LENGTH_256	256
#define MAX_QUERY_LENGTH_512	512
#define MAX_QUERY_LENGTH_1024	1024


enum eQUERY_STRING_BUFFER_T
{
	QSB_T00			= 0,		//STRING_SAFER_QUERY32;
	QSB_T01			,			//STRING_SAFER_QUERY64;
	QSB_T02			,			//STRING_SAFER_QUERY128;
	QSB_T03			,			//STRING_SAFER_QUERY256;
	QSB_T04			,			//STRING_SAFER_QUERY512;
	QSB_T05			,			//STRING_SAFER_QUERY1024;
	QSB_T06			,			//STRING_SAFER_CHARSTATE;
	QSB_T07			,			//STRING_SAFER_GUILDNAME;
	QSB_T08			,
	QSB_T09			,
	QSB_T10			,
	QSB_T11			,			//STRING_SAFER_USER_ID1;
	QSB_T12			,			//STRING_SAFER_USER_ID2;
	QSB_T13			,			//STRING_SAFER_USER_ID3;
	QSB_T14			,			//STRING_SAFER_USER_ID4;
	QSB_T15			,			//STRING_SAFER_USER_ID5;
	QSB_T16			,			//STRING_SAFER_CHAR_NAME1;
	QSB_T17			,			//STRING_SAFER_CHAR_NAME2;
	QSB_T18			,			//STRING_SAFER_CHAR_NAME3;
	QSB_T19			,			//STRING_SAFER_CHAR_NAME4;
	QSB_T20			,			//STRING_SAFER_CHAR_NAME5;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<QUERY STRING BUFFER CONTROL CLASS>
template<typename eQUERY_STRING_BUFFER_T _QSB, typename WORD _MAX_STRING_LENGTH>
class QueryStringBuffer
{
public:		static const WORD MAX_STRING_LENGTH					= _MAX_STRING_LENGTH;
			static const WORD MAX_STRING_LENGTH_INDEX			= _MAX_STRING_LENGTH-1;

private:	static const WORD MAX_STRING_LENGTH_BOUNDARY		= _MAX_STRING_LENGTH+1;
#	define	CALC_ALIGN32BYTE(size)			(((size) + 0x1F) & ~0x1F)
			static const WORD MAX_STRING_LENGTH_BUFFERBLOCK		= CALC_ALIGN32BYTE(MAX_STRING_LENGTH_BOUNDARY);

public:		QueryStringBuffer() : m_pLinker(m_pQuery) { m_bInited = FALSE; }
			~QueryStringBuffer() { m_bInited = FALSE; }
			QueryStringBuffer( const TCHAR* szSimpleString ) : m_pLinker(m_pQuery) { m_bInited = FALSE; CopyFromSingleString( szSimpleString ); }

public:		inline TCHAR*	GetString() { return m_bInited ? m_pQuery : NULL; }
			inline operator TCHAR* () { return m_bInited ? m_pQuery : NULL; }
			inline operator const TCHAR* () { return m_bInited ? m_pQuery : NULL; }
/*
			// 초기화, 여러 개의 문자열을 조합하는 경우 사용하면 유용
			BOOL		InitForStringCompose( WORD wMaxQueryStringLength )
			{
				if( !(MAX_QUERY_LENGTH_1024 < wMaxQueryStringLength) )
				{
					m_wMaxLength = wMaxQueryStringLength+1;
					WORD wMaxPaddingSize = CALC_ALIGN32BYTE(m_wMaxLength);
					ZeroMemory( m_pQuery, sizeof(TCHAR)*wMaxPaddingSize );

					ms_bInited = TRUE;

					return TRUE;
				}

				SUNLOG( eCRITICAL_LOG, _T("InitForUsing : Query Length Over %u"), wMaxQueryStringLength );
				return FALSE;
			}
*/
			BOOL		InitForStringNode()
			{
				ZeroMemory( m_pQuery, MAX_STRING_LENGTH_BUFFERBLOCK );
				m_bInited = TRUE;
				return TRUE;
			}

			BOOL		CopyFromSingleString( const TCHAR* ptszString )
			{
				if( _CopyTest( ptszString ) )
				{
					_tcsncpy( m_pQuery, ptszString, this->MAX_STRING_LENGTH_BOUNDARY );
					return _CopyPostCheck();
				}
				return FALSE;
			}

			BOOL		MakeString( const TCHAR* ptszString, ... )
			{
				if( _CopyTest( ptszString ) )
				{
					va_list args; 
					va_start(args, ptszString); 

					_vsntprintf( m_pQuery, this->MAX_STRING_LENGTH_BOUNDARY, ptszString, args );

					va_end(args);

					return _CopyPostCheck();
				}
				return FALSE;
			}

			INT			CopyFromCompose( INT iOffset, const TCHAR* ptszString, ... )
			{
				if( _CopyTest( ptszString ) )
				{
					if( !((INT)this->MAX_STRING_LENGTH > iOffset) )
						return -1;

					va_list args; 
					va_start(args, ptszString); 

					INT iWritten = _vsntprintf( &m_pQuery[iOffset], this->MAX_STRING_LENGTH-iOffset, ptszString, args );

					va_end(args);

					_CopyPostCheck(); //일단 통과시키자... -_-;
					return iWritten;
				}
				return -1;
			}

private:	BOOL		_CopyTest( const TCHAR* ptszString )
			{
				if( ptszString )
				{
					if( !m_bInited )
						InitForStringNode();

					return TRUE;
				}

				SUNLOG( eCRITICAL_LOG, _T("CopyFrom : String Is NULL") );
				m_pQuery[0] = _T('\0');
				return FALSE;
			}

			BOOL		_CopyPostCheck()
			{
				if( !m_pQuery[this->MAX_STRING_LENGTH] )
					return TRUE;

				m_pQuery[this->MAX_STRING_LENGTH] = _T('\0');
				SUNLOG( eCRITICAL_LOG, _T("CopyFrom : String Is Buffer Overflow {%s}"), m_pQuery );
				m_pQuery[0] = _T('\0');
				return FALSE;
			}

private:	// <FIELDs>
			TCHAR*						m_pLinker;
			static BOOL					m_bInited;
			static TCHAR				m_pQuery[MAX_STRING_LENGTH_BUFFERBLOCK];
};


template<typename eQUERY_STRING_BUFFER_T _QSB, typename WORD _MAX_STRING_LENGTH>
BOOL	QueryStringBuffer<_QSB, _MAX_STRING_LENGTH>::m_bInited;


template<typename eQUERY_STRING_BUFFER_T _QSB, typename WORD _MAX_STRING_LENGTH>
TCHAR	QueryStringBuffer<_QSB, _MAX_STRING_LENGTH>::m_pQuery[MAX_STRING_LENGTH_BUFFERBLOCK];

typedef QueryStringBuffer<QSB_T00, MAX_QUERY_LENGTH_32>			STRING_SAFER_QUERY32;
typedef QueryStringBuffer<QSB_T01, MAX_QUERY_LENGTH_64>			STRING_SAFER_QUERY64;
typedef QueryStringBuffer<QSB_T02, MAX_QUERY_LENGTH_128>		STRING_SAFER_QUERY128;
typedef QueryStringBuffer<QSB_T03, MAX_QUERY_LENGTH_256>		STRING_SAFER_QUERY256;
typedef QueryStringBuffer<QSB_T04, MAX_QUERY_LENGTH_512>		STRING_SAFER_QUERY512;
typedef QueryStringBuffer<QSB_T05, MAX_QUERY_LENGTH_1024>		STRING_SAFER_QUERY1024;

typedef QueryStringBuffer<QSB_T06, MAX_STATEINFOSTREAM_SIZE>	STRING_SAFER_CHARSTATE;
typedef QueryStringBuffer<QSB_T07, MAX_GUILDNAME_LENGTH>		STRING_SAFER_GUILDNAME;

typedef QueryStringBuffer<QSB_T11, MAX_ID_SIZE>					STRING_SAFER_USER_ID;
typedef STRING_SAFER_USER_ID									STRING_SAFER_USER_ID1;
typedef QueryStringBuffer<QSB_T12, MAX_ID_SIZE>					STRING_SAFER_USER_ID2;
typedef QueryStringBuffer<QSB_T13, MAX_ID_SIZE>					STRING_SAFER_USER_ID3;
typedef QueryStringBuffer<QSB_T14, MAX_ID_SIZE>					STRING_SAFER_USER_ID4;
typedef QueryStringBuffer<QSB_T15, MAX_ID_SIZE>					STRING_SAFER_USER_ID5;

typedef QueryStringBuffer<QSB_T16, MAX_CHARNAME_LENGTH>			STRING_SAFER_CHAR_NAME;
typedef STRING_SAFER_CHAR_NAME									STRING_SAFER_CHAR_NAME1;
typedef QueryStringBuffer<QSB_T17, MAX_CHARNAME_LENGTH>			STRING_SAFER_CHAR_NAME2;
typedef QueryStringBuffer<QSB_T18, MAX_CHARNAME_LENGTH>			STRING_SAFER_CHAR_NAME3;
typedef QueryStringBuffer<QSB_T19, MAX_CHARNAME_LENGTH>			STRING_SAFER_CHAR_NAME4;
typedef QueryStringBuffer<QSB_T20, MAX_CHARNAME_LENGTH>			STRING_SAFER_CHAR_NAME5;

