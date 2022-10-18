
#include "stdafx.h"
#include "ScriptDataLoader.h"
#if SUN_CODE_BACKUP
typedef ScriptDataLoader::COLUMN_LIST			COLUMN_LIST;
typedef ScriptDataLoader::COLUMN_LIST_IT		COLUMN_LIST_IT;
typedef ScriptDataLoader::LINE_PAIR			LINE_PAIR;
typedef ScriptDataLoader::LINE_LIST			LINE_LIST;
typedef ScriptDataLoader::LINE_LIST_IT		LINE_LIST_IT;

ScriptDataLoader::ScriptDataLoader()
{
	m_bLoaded			= FALSE;
	m_pBuffer			= NULL;
	m_dwColumnCount		= 0;
	m_eSECTION_TYPE		= SECTION_DEFAULT;

	m_ScriptList.clear();

	fnParseSection		= NULL;
	pParseSectionPassingPtr = NULL;
	fnParseSectionData	= NULL;
	pParseSectionDataPassingPtr = NULL;
}

BOOL		ScriptDataLoader::ReadString( CHAR* value, DWORD length )
{
	BOOL bContinue;
	if( !value )
		return FALSE;

	if( m_ScriptList.size() == 0 )
		return FALSE;

	do
	{
		bContinue = FALSE;
		if( it_COL != itend_COL )
		{
			CHAR* pStr = *it_COL;
			strncpy( value, pStr, length );
			++it_COL;
			return TRUE;
		}
		else if( it_LINE != m_ScriptList.end() )
		{
			++it_LINE;
			it_COL = (*it_LINE).second->begin();
			itend_COL = (*it_LINE).second->end();
			bContinue = TRUE;
		}
	} while( bContinue );

	return FALSE;
}

VOID		ScriptDataLoader::_RemoveLine( LINE_LIST_IT& it_line )
{
	LINE_PAIR& rPair = *it_line;
	COLUMN_LIST* pCols = rPair.second;

	delete pCols;
	it_line = m_ScriptList.erase(it_line);
}

VOID	ScriptDataLoader::ColoumnFree( LINE_PAIR value )
{
	COLUMN_LIST* pCols = value.second;
	pCols->clear();
	delete pCols;
}

HANDLE	ScriptDataLoader::_OPENFILEnREADBUFER( const CHAR* slList )
{
	if( !slList )
		return INVALID_HANDLE_VALUE;

	OFSTRUCT pOF;
	HANDLE hFILE = (HANDLE)OpenFile( slList, &pOF, OF_READ );
	DWORD dwReaded;

	dwReaded = GetFileSize( hFILE, NULL );
	DWORD dwAllocedSize = (DWORD)(sizeof(CHAR)*dwReaded + 10);

	//CHAR* pSTREAM = (CHAR*)malloc( dwAllocedSize );
	CHAR* pSTREAM = new CHAR[dwAllocedSize];
	memset( pSTREAM, 0, dwAllocedSize );
	ReadFile( hFILE, pSTREAM, dwAllocedSize, &dwReaded, NULL );
	CloseHandle( (HANDLE)hFILE );

	if( m_pBuffer )
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}

	m_pBuffer = pSTREAM;

	return hFILE;
}

ScriptDataLoader::eSECTION_TYPE
ScriptDataLoader::_LoadSection( LINE_LIST_IT& it_line, LINE_LIST_IT& itend_line )
{
	eSECTION_TYPE eType = SECTION_COLUMNS;
	INT value;
	CHAR buffer[128];
	CHAR buffer2[128];

	while( it_line != itend_line )
	{
		LINE_PAIR& rPair = *it_line;

		CHAR* pOffset = rPair.first;
		COLUMN_LIST* pCols = rPair.second;

		while( IsWhitespaceChar(*pOffset) )
			pOffset++;

		if( IsComment( pOffset ) )
		{
			_RemoveLine(it_line);
			eType = SECTION_ERROR;
			continue;
		}

		if( IsSection( *pOffset ) )
		{
			if( (strlen( pOffset ) > sizeof(buffer))
				|| sscanf( pOffset, "%s", buffer ) != 1 )
			{
				_RemoveLine(it_line);
				eType = SECTION_ERROR;
				continue;
			}

			if( strncmp( buffer, "[COLOUMNS]", sizeof(buffer) ) == 0 )
			{
				_RemoveLine(it_line);
				m_eSECTION_TYPE = SECTION_COLUMNS;
				eType = SECTION_TAB;
				continue;
			}
			else if( strncmp( buffer, "[DATA]", sizeof(buffer) ) == 0 )
			{
				_RemoveLine(it_line);
                --it_line;
				m_eSECTION_TYPE = SECTION_DATA;
				eType = SECTION_TAB;
				return SECTION_DATA;
			}
			else if( NULL != fnParseSection )
			{
				SDAttrTag tag = SDAttrTag( (CHAR*)buffer, (DWORD)sizeof(buffer) );
				SECTION_TYPE sect = fnParseSection( pParseSectionPassingPtr, tag );
				if( SECTION_COLUMNS == sect )
				{
					_RemoveLine(it_line);
					m_eSECTION_TYPE = SECTION_COLUMNS;
					eType = SECTION_TAB;
					continue;
				}
				else if( SECTION_DATA == sect )
				{
					_RemoveLine(it_line);
                    --it_line;
					m_eSECTION_TYPE = SECTION_DATA;
					eType = SECTION_TAB;
					return SECTION_DATA;
				}
				else
				{
					_RemoveLine(it_line);
					eType = SECTION_ERROR;
					continue;
				}
			}
			else
			{
				_RemoveLine(it_line);
				m_eSECTION_TYPE = SECTION_COLUMNS;
				eType = SECTION_TAB;
				continue;
			}
		}
		else if( SECTION_COLUMNS == m_eSECTION_TYPE )
		{
			if( sscanf( pOffset, "%s = %s", buffer, buffer2 ) != 2 )
			{
				_RemoveLine(it_line);
				eType = SECTION_ERROR;
				continue;
			}

			if( strncmp( buffer, "DATA_COL_COUNT", sizeof(buffer) ) == 0 )
			{
				if( sscanf( buffer2, "%u", &value ) != 1 )
				{
					_RemoveLine(it_line);
					eType = SECTION_ERROR;
					continue;
				}

				m_dwColumnCount = (DWORD)value;
				_RemoveLine(it_line);
				continue;
			}
			else if( NULL != fnParseSectionData )
			{
				SDAttrValueName attrValueName = SDAttrValueName( (CHAR*)buffer, (DWORD)sizeof(buffer) );
				SDAttrValue attrValue = SDAttrValue( (CHAR*)buffer2, (DWORD)sizeof(buffer2) );
				fnParseSectionData( pParseSectionDataPassingPtr, attrValueName, attrValue );
				_RemoveLine(it_line);
				continue;
			}
			else
			{
				_RemoveLine(it_line);
			}
		}
		else
		{
			_RemoveLine(it_line);
		}
	}


	return SECTION_EOL;
}

BOOL	ScriptDataLoader::_LoadList()
{
	return FALSE;
}

BOOL	ScriptDataLoader::_Load()
{
	const CHAR* c_delim_line = "\n\r";

	CHAR* pChr = strtok( m_pBuffer, c_delim_line );
	while( pChr != NULL )
	{
		COLUMN_LIST* pCols = new COLUMN_LIST;
		m_ScriptList.insert( m_ScriptList.end(), LINE_PAIR(pChr, pCols) );
		pChr = strtok( NULL, c_delim_line );
	}

	LINE_LIST_IT it(m_ScriptList.begin());

	for( ; it != m_ScriptList.end() ; ++it )
	{
		LINE_PAIR& rPair = *it;

		CHAR* pOffset = rPair.first;
		COLUMN_LIST* pCols = rPair.second;

		while( IsWhitespaceChar(*pOffset) )
			pOffset++;

		if( IsComment( pOffset ) )
		{
			//m_ScriptList.erase( it-- );
			_RemoveLine( it-- );
			continue;
		}

		if( IsSection( *pOffset ) )
		{
			_LoadSection( it, m_ScriptList.end() );
			continue;
		}

		int iCols = 0;

		if( IsEOLChar( pOffset ) )
		{
			iCols = 1;
			pChr = NULL;
		}
		else
			pChr = strtok( pOffset, "\t" );

		while( pChr != NULL )
		{
			if( IsComment( pChr ) )
				break;
			++iCols;
			pCols->insert( pCols->end(), pChr );
			pChr = strtok( NULL, "\t" );
		}

		if( iCols == 1 )
			break;

		if( !m_dwColumnCount )
			m_dwColumnCount = iCols;
		else if( m_dwColumnCount != iCols )
		{
#ifdef _SERVER
			printf( "Invalid Cols\n" );
#endif //
			return FALSE;
		}

		while( IsWhitespaceChar(*pOffset) )
			pOffset++;

		if( IsComment( pOffset ) )
		{
			//m_ScriptList.erase( it-- );
			_RemoveLine( it-- );
			continue;
		}
	}

// <DEBUG><LIST>
/*
	LINE_LIST_IT it1(m_ScriptList.begin()), end1(m_ScriptList.end());

	printf( "start\n" );
	for( ; it1 != end1 ; ++it1 )
	{
		LINE_PAIR& rPair = *it1;

		CHAR* pOffset = rPair.first;
		COLUMN_LIST* pCols = rPair.second;

		COLUMN_LIST_IT it2(pCols->begin()), itend2(pCols->end());
		for( ; it2 != itend2 ; ++it2 )
		{
			printf( "%s\t", *it2 );
		}
		printf( "\n" );
	}
	printf( "\n" );
*/

/////////////////////////////////////////////////////////////////////////////
	it_LINE = m_ScriptList.begin();
	if( m_ScriptList.size() != 0 )
	{
		it_COL = (*it_LINE).second->begin();
		itend_COL = (*it_LINE).second->end();
	}

	return TRUE;
}

BOOL	ScriptDataLoader::_UnLoad()
{
	if( m_bLoaded )
	{
		m_bLoaded = FALSE;
		m_dwColumnCount = 0;
		m_eSECTION_TYPE = SECTION_DEFAULT;
	}

	if( m_pBuffer )
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}

	if( m_ScriptList.size() )
	{
		for_each(
			m_ScriptList.begin(),
			m_ScriptList.end(),
			bind1st(mem_fun(&ScriptDataLoader::ColoumnFree), this)
			);

		m_ScriptList.clear();
	}

	return TRUE;

/*
	if( m_bLoaded )
	{
		m_bLoaded = FALSE;
		m_dwColumnCount = 0;
		m_eSECTION_TYPE = eSECTION_TYPE::SECTION_DEFAULT;

		if( m_pBuffer )
		{
			delete m_pBuffer;
			m_pBuffer = NULL;
		}

		for_each(
			m_ScriptList.begin(),
			m_ScriptList.end(),
			bind1st(mem_fun(&ScriptDataLoader::ColoumnFree), this)
			);

		m_ScriptList.clear();

		return TRUE;
	}
	else
		return FALSE;
*/
}

BOOL	ScriptDataLoader::Load( const TCHAR* pszFileName, BOOL bReload )
{
	if( INVALID_HANDLE_VALUE  == _OPENFILEnREADBUFER( pszFileName ) )
	{
		//printf( "error --
		return FALSE;
	}

	if( bReload )
	{
		if( !_UnLoad() )
		{
			//printf( "error --
			return FALSE;
		}
	}

	BOOL bRet = _Load();
	if( bRet )
		m_bLoaded = TRUE;
	return bRet;
}

BOOL	ScriptDataLoader::LoadBuffer( BYTE* pBuffer, const DWORD dwBufSize )
{
	if( !(pBuffer && dwBufSize) )
		return FALSE;

	if( m_pBuffer )
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}

	DWORD dwAllocSize = dwBufSize;
	if( pBuffer[dwBufSize-1] != '\0' )
		++dwAllocSize;

	m_pBuffer = new CHAR[dwAllocSize];
	memcpy( m_pBuffer, pBuffer, dwBufSize );
	m_pBuffer[dwAllocSize-1] = '\0';

	BOOL bRet = _Load();
	if( bRet )
		m_bLoaded = TRUE;
	return bRet;
}

#endif //SUN_CODE_BACKUP