
#include "FileListLoader.h"

FileListLoader::FileListLoader()
{
	m_bLoaded			= FALSE;
	m_pBuffer			= NULL;
	m_dwColumnCount		= 0;
	m_eSECTION_TYPE		= eSECTION_TYPE::SECTION_DEFAULT;

	m_ScriptList.clear();

	fnParseSection		= NULL;
	pParseSectionPassingPtr = NULL;
	fnParseSectionData	= NULL;
	pParseSectionDataPassingPtr = NULL;
}

BOOL		FileListLoader::ReadString( CHAR* value, DWORD length )
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

VOID		FileListLoader::_RemoveLine( LINE_LIST_IT& it_line )
{
	LINE_PAIR& rPair = *it_line;
	COLUMN_LIST* pCols = rPair.second;

	delete pCols;
	m_ScriptList.erase(it_line--);
}

VOID	FileListLoader::ColoumnFree( LINE_PAIR value )
{
	COLUMN_LIST* pCols = value.second;
	pCols->clear();
	delete pCols;
}

HANDLE	FileListLoader::_OPENFILEnREADBUFER( const CHAR* slList )
{
	if( !slList )
		return INVALID_HANDLE_VALUE;

	OFSTRUCT pOF;
	HANDLE hFILE = (HANDLE)(__int64)OpenFile( slList, &pOF, OF_READ );
	DWORD dwReaded;

	if( INVALID_HANDLE_VALUE == hFILE )
	{
		printf( "%u\n", GetLastError() );
		return INVALID_HANDLE_VALUE;
	}

	dwReaded = GetFileSize( hFILE, NULL );
	DWORD dwAllocedSize = (DWORD)(sizeof(CHAR)*dwReaded + 10);

	//CHAR* pSTREAM = (CHAR*)malloc( dwAllocedSize );
	CHAR* pSTREAM = new CHAR[dwAllocedSize];
	memset( pSTREAM, 0, dwAllocedSize );
	ReadFile( hFILE, pSTREAM, dwAllocedSize, &dwReaded, NULL );
	CloseHandle( (HANDLE)hFILE );

	if( m_pBuffer )
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}

	m_pBuffer = pSTREAM;

	return hFILE;
}

FileListLoader::eSECTION_TYPE
FileListLoader::_LoadSection( LINE_LIST_IT& it_line, LINE_LIST_IT& itend_line )
{
	eSECTION_TYPE eType = eSECTION_TYPE::SECTION_COLUMNS;
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
			_RemoveLine( it_line++ );
			eType = SECTION_ERROR;
			continue;
		}

		if( IsSection( *pOffset ) )
		{
			if( (strlen( pOffset ) > sizeof(buffer))
				|| sscanf( pOffset, "%s", buffer ) != 1 )
			{
				_RemoveLine( it_line++ );
				eType = SECTION_ERROR;
				continue;
			}

			if( strncmp( buffer, "[COLOUMNS]", sizeof(buffer) ) == 0 )
			{
				_RemoveLine( it_line++ );
				m_eSECTION_TYPE = SECTION_COLUMNS;
				eType = SECTION_TAB;
				continue;
			}
			else if( strncmp( buffer, "[DATA]", sizeof(buffer) ) == 0 )
			{
				_RemoveLine( it_line-- );
				m_eSECTION_TYPE = SECTION_DATA;
				eType = SECTION_TAB;
				return SECTION_DATA;
			}
			else if( NULL != fnParseSection )
			{
				AttrTag tag = AttrTag( (CHAR*)buffer, (DWORD)sizeof(buffer) );
				SECTION_TYPE sect = fnParseSection( pParseSectionPassingPtr, tag );
				if( SECTION_COLUMNS == sect )
				{
					_RemoveLine( it_line++ );
					m_eSECTION_TYPE = SECTION_COLUMNS;
					eType = SECTION_TAB;
					continue;
				}
				else if( SECTION_DATA == sect )
				{
					_RemoveLine( it_line-- );
					m_eSECTION_TYPE = SECTION_DATA;
					eType = SECTION_TAB;
					return SECTION_DATA;
				}
				else
				{
					_RemoveLine( it_line++ );
					eType = SECTION_ERROR;
					continue;
				}
			}
			else
			{
				_RemoveLine( it_line++ );
				m_eSECTION_TYPE = SECTION_COLUMNS;
				eType = SECTION_TAB;
				continue;
			}
		}
		else if( SECTION_COLUMNS == m_eSECTION_TYPE )
		{
			if( sscanf( pOffset, "%s = %s", buffer, buffer2 ) != 2 )
			{
				_RemoveLine( it_line++ );
				eType = SECTION_ERROR;
				continue;
			}

			if( strncmp( buffer, "DATA_COL_COUNT", sizeof(buffer) ) == 0 )
			{
				if( sscanf( buffer2, "%u", &value ) != 1 )
				{
					_RemoveLine( it_line++ );
					eType = SECTION_ERROR;
					continue;
				}

				m_dwColumnCount = (DWORD)value;
				_RemoveLine( it_line++ );
				continue;
			}
			else if( NULL != fnParseSectionData )
			{
				AttrValueName attrValueName = AttrValueName( (CHAR*)buffer, (DWORD)sizeof(buffer) );
				AttrValue attrValue = AttrValue( (CHAR*)buffer2, (DWORD)sizeof(buffer2) );
				fnParseSectionData( pParseSectionDataPassingPtr, attrValueName, attrValue );
				_RemoveLine( it_line++ );
				continue;
			}
			else
			{
				_RemoveLine( it_line++ );
			}
		}
		else
		{
			_RemoveLine( it_line++ );
		}
	}


	return SECTION_EOL;
}

BOOL	FileListLoader::_LoadList()
{
	return FALSE;
}

BOOL	FileListLoader::_Load()
{
	const CHAR* c_delim_line = "\n\r";

	CHAR* pChr = strtok( m_pBuffer, c_delim_line );
	while( pChr != NULL )
	{
		COLUMN_LIST* pCols = new COLUMN_LIST;
		m_ScriptList.insert( m_ScriptList.end(), LINE_PAIR(pChr, pCols) );
		//printf( "%s\n", pChr );
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
		//pChr = IsEOLChar( pOffset ) ? (++iCols, NULL) : strtok( pOffset, "\t" );
		//pChr = strtok( pOffset, "\t" );
		while( pChr != NULL )
		{
			if( IsComment( pChr ) )
				break;
			++iCols;
			pCols->insert( pCols->end(), pChr );
			//printf( "%s\t", pChr );
			pChr = strtok( NULL, "\t" );
		}

		if( iCols == 1 )
			break;

		if( !m_dwColumnCount )
			m_dwColumnCount = iCols;
		else if( m_dwColumnCount != iCols )
		{
			printf( "Invalid Cols\n" );
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

BOOL	FileListLoader::_UnLoad()
{
	if( m_bLoaded )
	{
		m_bLoaded = FALSE;
		m_dwColumnCount = 0;
		m_eSECTION_TYPE = eSECTION_TYPE::SECTION_DEFAULT;
	}

	if( m_pBuffer )
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}

	if( m_ScriptList.size() )
	{
		for_each(
			m_ScriptList.begin(),
			m_ScriptList.end(),
			bind1st(mem_fun(&FileListLoader::ColoumnFree), this)
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
			bind1st(mem_fun(&FileListLoader::ColoumnFree), this)
			);

		m_ScriptList.clear();

		return TRUE;
	}
	else
		return FALSE;
*/
}

BOOL	FileListLoader::Load( const TCHAR* pszFileName, BOOL bReload )
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

BOOL	FileListLoader::LoadBuffer( BYTE* pBuffer, const DWORD dwBufSize )
{
	if( !(pBuffer && dwBufSize) )
		return FALSE;

	if( m_pBuffer )
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}

	m_pBuffer = new CHAR[dwBufSize];
	memcpy( m_pBuffer, pBuffer, dwBufSize );

	BOOL bRet = _Load();
	if( bRet )
		m_bLoaded = TRUE;
	return bRet;
}

