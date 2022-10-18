/*-----------------------------------------------------------------------------
 * MemoryIni.cpp
 *-----------------------------------------------------------------------------
 * 암호화된 Ini파일을 메모리에서 읽고 쓸수 있게 하기 위하여 만든 클래스
 *-----------------------------------------------------------------------------
**---------------------------------------------------------------------------*/

#include "MemoryIni.h"


//

MemoryIni::MemoryIni()
  : m_bInit(FALSE),
	m_bParsed(FALSE),
    m_data(NULL),
	m_data_size(0)
{
	m_sec.clear();
}

MemoryIni::~MemoryIni()
{ 
	if ( NULL != m_data )
	{
		free(m_data);
		m_data = NULL;
	}

	m_sec.clear();
	m_data_size = 0;
}

/**	-----------------------------------------------------------------------
	\brief		ppos버퍼에서 linsz보다 작은 크기로 한줄 읽어서 line에 채움
	
	\remark		sgets2()함수를 리팩토링한 함수임

	\param	
				line	- 한 Line이 채워짐
				linesz	- line 의 크기
				ppos	- line을 검색할 버퍼(IN/OUT겸용)
	\return
				성공시 읽혀진 라이의 길이, 0을 리턴하면 Line으로 취급할 
				문자가 없으므로(예:연속된 캐리지리턴들, 연속된 NULL문자들)
				실패시 -1을 리턴함
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::sgets2(LPSTR line, DWORD linesz, LPSTR *ppos)
{

	_ASSERTE( NULL != line );
	_ASSERTE( NULL != ppos );
	_ASSERTE( linesz > 0 );
	if ( IsBadWritePtr(line, linesz) ||
		 NULL == ppos ||
		 0 == linesz )
	{
		_ASSERTE(!"Invalid parameter");
		return -1;
	}


	int nread = 0;
	LPSTR end =  (LPSTR)&m_data[m_data_size];
	LPSTR pos = *ppos;
	
	
	//	현재위치가 끝이거나(이미 검색끝났을 경우) 에러 리턴
	//

	if ( pos >= end )
	{
		return -1;
	}

	//	현재 위치가 끝을 넘지 않으며, 채울수 있는 버퍼가 남아있을때까지 반복
	//

	while ( pos < end && linesz > 0 )
	{
		int chsize = (int)_tcsnbcnt( pos, 1 );
		UINT ch = _tcsnextc(pos);

		if ( ch == _T('\0') )
		{
			if ( !nread )
			{
				//	pos = CharNext(pos) 가 아님!!
				//	왜냐하면 *pos가 NULL이면 같은 포인터를 리턴하기 때문
				pos++;
				continue;
			}

			break;
		}
		else if ( ch == _T('\r') || ch == _T('\n') )
		{
			pos = CharNext(pos);

			if ( !nread )
			{
				continue;
			}

			break;
		}

		
		//	중요!!
		//			남아있는 버퍼사이즈가 chsize보다 작다면 chsize를 조절함
		//			여기서 체크를 하지 않으면 linsz는 음수값이 될것이며
		//			line과 linesz에도 잘못된 결과가 들어갈 것이다
		//
		if ( linesz < (DWORD)chsize )
		{
			break;
		}

		memcpy( (PVOID)&line[nread], pos, chsize );

		nread += chsize;
		linesz -= chsize;
  
		pos = CharNext(pos);
	}


	*ppos = pos;


	if ( nread > 0 )
	{
		line[nread] = _T('\0');
	}

	return nread;
}

/**	-----------------------------------------------------------------------
	\brief		ppos버퍼에서 linsz보다 작은 크기로 한줄 읽어서 line에 채움

	\param	
				line	- 한 Line이 채워짐
				linesz	- line 의 크기
				ppos	- line을 검색할 버퍼(IN/OUT겸용)
	\return
				성공시 읽혀진 라이의 길이, 0을 리턴하면 Line으로 취급할 
				문자가 없으므로(예:연속된 캐리지리턴들, 연속된 NULL문자들)
				실패시 -1을 리턴함
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::sgets(LPTSTR line, DWORD linesz, LPTSTR *ppos)
{
	_ASSERTE( NULL != line );
	_ASSERTE( NULL != ppos );
	_ASSERTE( linesz > 0 );	

	if ( 0 != IsBadWritePtr((LPVOID)line, linesz) ||
		 NULL == ppos ||
		 0 == linesz )
	{
		return -1;
	}


	LPTSTR end_marker = (LPTSTR) &m_data[m_data_size];
	LPTSTR pos = *ppos;
	UINT ch = _T('\0');
	size_t i = 0;


	// pos와 end_marker와 같다면 이미 순회를 마친 경우므로 에러
	//

	if (pos >= end_marker)
	{
		return -1;
	}


	//
	//	현재 pos가 end_marker에 도달하기전까지의 NULL문자는 무시함
	//	pos가 end_marker와 같지 않은데 NULL이라면 포인터 증가
	//

	while ( pos < end_marker )
	{
		if ( _T('\0') != _tcsnextc(pos) )
			break;

		LPSTR oldpos = pos;
		pos = CharNext(pos);

		if ( pos == oldpos )
			pos++;
	}

	
	while ( linesz > 0 && _T('\0') != _tcsnextc(pos) )
	{
		size_t ByteCount = _tcsnbcnt( pos, 1 );
		ch = _tcsnextc( pos );

		if ( ch == _T('\r') || ch == _T('\n') )
		{
			if ( 0 == i )
			{
				pos = CharNext(pos);

				if (pos == end_marker)
					break;

				continue;
			}
			else
				break;
		}

		memcpy( (PVOID)&line[i], pos, ByteCount );
		i = i + ByteCount;
		linesz = linesz - ByteCount;

		pos = CharNext(pos);
		
		if (pos == end_marker)
			break;		
	}


	//	더이상 생성할 라인이 없을 경우
	//

	if (0 == i && pos == end_marker)
		return -1;


	line[i] = _T('\0');
	*ppos = pos;

	return (int)i;
}


/**	-----------------------------------------------------------------------
	\brief		인자로 받은 s의 처음부터 검색하여 널 만나기 전까지
				공백문자를 검색하여 건너뜀

	\param	
				s		- 스트링
	\return
				실패시 NULL리턴, 성공시 공백문자가 아닌 위치를 리턴함
	\code
	\endcode		
-------------------------------------------------------------------------*/
LPTSTR MemoryIni::strskip( LPTSTR s )
{
	if ( NULL == s )
		return NULL;

	while ( _istspace(_tcsnextc(s)) && _T('\0') != _tcsnextc(s) ) 
		s = CharNext(s);

	return s;
}


/**	-----------------------------------------------------------------------
	\brief		좌우 공백문자가 존재하면 삭제함

	\param	
				str		- 공백문자를 삭제할 스트링
	\return
				리턴값 없음
	\code
	\endcode		
-------------------------------------------------------------------------*/
void MemoryIni::strip( LPTSTR str )
{
	LPTSTR s = str;
	int l;

	while ( _istspace(_tcsnextc(s)) ) 
		s = CharNext(s);

	l = (int) _tcslen(s);

	if (s != str)
		memmove(str, s, l+1);
	if (0 == l)
		return;

	s = str + l - 1;
	while ( _istspace(_tcsnextc(s)) )
	{
		*s = _T('\0');
		s = CharPrev( str, s );
	}
}


/**	-----------------------------------------------------------------------
	\brief		섹션명을 인자로 받으며 새로운 섹션을 추가함

	\param	
				name	- 섹션명
	\return
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::addNewSection(LPCTSTR name)
{
	INI_SECTION ini_sec = {0};
    const size_t kLength = (sizeof(ini_sec.name) / sizeof(ini_sec.name[0]));
    ::strncpy(ini_sec.name, name, kLength);
	
	ini_sec.keys.clear();
	m_sec.push_back( ini_sec );

	return 0;	
}


/**	-----------------------------------------------------------------------
	\brief		현재 분석하고 있는 섹션중 가장 최근 섹션에 key, value
				쌍을 추가함
	
	\remark		Internal 함수임

	\param	
				key		- 추가할 키이름
				value	- 추가할 값데이터
	\return
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::addtoLastSection(LPCTSTR key, LPCTSTR value)
{

	//	가장 최근에 섹션이 등록된게 없다면 key, value값을 등록할 수 없음
	//

	if ( !m_sec.empty() )
	{
		INI_SECTION &ini_sec = m_sec.back();

		KEYDATA keydata = {0};
		strncpy(keydata.key, key, MAX_LINE);
		strncpy(keydata.value, value, MAX_LINE);

		ini_sec.keys.push_back( keydata );
	}

	return 0;
}


/**	-----------------------------------------------------------------------
	\brief		에디터에서 수정을 위하여 인터페이스 노출함(디버깅시 필요)

	\remark		MemoryIni에 담긴 m_data를 직접 확인하기 위해 필요

	\param	
	\return
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::SetData(LPTSTR ini_data, DWORD ini_size)
{
	_ASSERTE( NULL != ini_data );
	_ASSERTE( 0 < ini_size );
	if ( NULL == ini_data || 0 == ini_size )
	{
		return -1;
	}

	if ( NULL != m_data )
	{
		free(m_data);
		m_data = NULL;
		m_data_size = 0;
	}

	m_data = (PBYTE) calloc(1, ini_size);

	if ( NULL == m_data )
	{
		return -1;
	}

	RtlCopyMemory(m_data, ini_data, ini_size);

	m_data_size = ini_size;

	parseData();

	return 0;
}


/**	-----------------------------------------------------------------------
	\brief	에디터에서 수정을 위하여 인터페이스 노출함(디버깅시 필요)

	\param	
	\return
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::GetData(LPTSTR ini_data, DWORD ini_size, PDWORD ini_written)
{
	_ASSERTE( NULL != ini_data );
	_ASSERTE( 0 < ini_size );
	_ASSERTE( NULL != ini_written );

	DWORD copy_size = m_data_size;

	if ( NULL == ini_data || NULL == ini_written || 0 == ini_size )
	{
		return -1;
	}

	if ( NULL == m_data || 0 == m_data_size)
	{
		return -1;
	}
	
	if ( ini_size <= m_data_size )
	{
		copy_size = ini_size;
	}
	
	RtlCopyMemory( ini_data, (LPTSTR)m_data, copy_size );

	*ini_written = copy_size;

	return 0;
}


/**	-----------------------------------------------------------------------
	\brief		m_data에 담긴 ini데이터를 분석함

	\remark		Internal 함수임
	\param	
	\return
				실패시 -1, 성공시 0
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::parseData()
{

	_ASSERTE( NULL != m_data );
	_ASSERTE( 0 != m_data_size );	
	if (NULL == m_data || 0 == m_data_size )
	{
		return -1;
	}


	LPTSTR pos = (LPTSTR) m_data; 

	TCHAR key[MAX_LINE] = {0,};
	TCHAR line[MAX_LINE] = {0,};
	TCHAR value[MAX_LINE] = {0,};
	TCHAR section[MAX_LINE] = {0,};
	
	memset( section, 0, sizeof(section) );
	memset( value, 0, sizeof(value) );
	memset( key, 0, sizeof(key) );
	memset( line, 0, sizeof(line) );

	m_sec.clear();

	while ( 0 <= sgets2(line, MAX_LINE-1, &pos) )
	{
		// 공백을 건너뛴다
		LPTSTR bc = strskip(line);

		if ( _T(';') == _tcsnextc(bc) || 
			 _T('#') == _tcsnextc(bc) || 
			 _T('\0') == _tcsnextc(bc) )
		{
			continue;
		}

		if ( 1 == _stscanf(line, _T("[%[^]]"), section) )
		{
			addNewSection(section);
		}
		else if ( 2 == _stscanf(bc, _T("%[^=] = \"%[^\"]\""), key, value) ||
				  2 == _stscanf(bc, _T("%[^=] = '%[^\']'"), key, value)	  ||
				  2 == _stscanf(bc, _T("%[^=] = %[^;#]"), key, value) )
		{

			strip(key);
			strip(value);

			if ( !_tcscmp(value, _T("\"\"")) || !_tcscmp(value, _T("''")) )
			{
				value[0] = _T('\0');
			}

			addtoLastSection( key, value );
		}

		memset( section, 0, sizeof(section) );
		memset( value, 0, sizeof(value) );
		memset( key, 0, sizeof(key) );
		memset( line, 0, sizeof(line) );
	}

	m_bParsed = TRUE;

	return 0;
}


/**	-----------------------------------------------------------------------
	\brief		

	\param	
	\return
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::getValue(LPCTSTR section, LPCTSTR key, LPCTSTR value, DWORD slen)
{
	if ( 0 == m_sec.size() )
	{
		return -1;
	}

	std::vector<INI_SECTION>::iterator it = m_sec.begin();

	for (it; it != m_sec.end(); it++)
	{
		PINI_SECTION ini_sec = &(*it);

		if ( 0 == _tcsicmp(ini_sec->name, section) )
		{
			std::vector<KEYDATA>::iterator it_key = ini_sec->keys.begin();

			for ( ; it_key != ini_sec->keys.end(); it_key++)
			{
				PKEYDATA ini_key = &(*it_key);

				if ( 0 == _tcsicmp(ini_key->key, key) )
				{
					strncpy((LPTSTR)value, ini_key->value, slen);
					return 0;
				}
			}
		}
	}
	
	return -1;
}


/**	-----------------------------------------------------------------------
	\brief		

	\param	
	\return
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::putValue(LPCTSTR section, LPCTSTR key, LPCTSTR value)
{

	if ( 0 == m_sec.size() )
	{
		addNewSection( section );
		addtoLastSection( key, value);

		return 0;
	}


	//	MAX_LINE을 넘치지 않게 하기 위해서 key, value 길이를 조절
	//

	size_t valuelen;


	std::vector<INI_SECTION>::iterator it = m_sec.begin();

	for (it; it != m_sec.end(); it++)
	{
		PINI_SECTION ini_sec = &(*it);

		if ( 0 == _tcsicmp(ini_sec->name, section) )
		{
			std::vector<KEYDATA>::iterator it_key = ini_sec->keys.begin();


			for ( ; it_key != ini_sec->keys.end(); it_key++)
			{
				PKEYDATA ini_key = &(*it_key);

				//	기존에 key가 있다면 수정 함
				//

				if ( 0 == _tcsicmp(ini_key->key, key) )
				{
					valuelen = MAX_LINE - _tcslen(ini_key->key);
					strncpy(ini_key->value, value, valuelen);
					return 0;
				}
			}


			// key가 존재하지 않으면, 새로운 키를 추가
			
			valuelen = MAX_LINE - _tcslen(key);

			KEYDATA keydata = {0};			
			strncpy(keydata.key, key, MAX_LINE);			
			strncpy(keydata.value, value, valuelen);

			ini_sec->keys.push_back( keydata );
			return 0;
		}
	}
	
	// section이 존재하지 않으면, 새로운 섹션 추가
	addNewSection( section );
	addtoLastSection( key, value );

	return 0;
}


/**	-----------------------------------------------------------------------
	\brief	메모리 INI정보에 String정보를 가져 옴

	\param	
	\return
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::getString(LPCTSTR section, LPCTSTR key, LPTSTR pStr, DWORD slen)
{
	_ASSERTE( NULL != section);
	_ASSERTE( NULL != key);
	_ASSERTE( NULL != pStr);
	_ASSERTE( 0 < slen);
	if ( NULL == section || NULL == key ||
		 NULL == pStr || 0 == slen )
	{
		return -1;
	}

	//if ( !m_bParsed )
	//{
	//	return -1;
	//}

	if ( 0 > getValue(section, key, pStr, slen) )
	{
		return -1;
	}

	return 0;
}


/**	-----------------------------------------------------------------------
	\brief	메모리 INI정보에 Integer정보를 가져 옴

	\param	
	\return
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::getInteger(LPCTSTR section, LPCTSTR key, LONG *pInt)
{
	_ASSERTE( NULL != section);
	_ASSERTE( NULL != key);
	_ASSERTE( NULL != pInt);
	if ( NULL == section || NULL == key || NULL == pInt )
	{
		return -1;
	}

	//if ( !m_bParsed )
	//{
	//	return -1;
	//}

	TCHAR value[MAX_LINE] = {0,};

	if ( 0 > getValue(section, key, value, MAX_LINE-1) )
	{
		return -1;
	}
	
	*pInt = _tstoi(value);

	return 0;
}


/**	-----------------------------------------------------------------------
	\brief	메모리 INI정보에 String정보를 추가 함

	\param	
	\return
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::putString(LPCTSTR section, LPCTSTR key, LPCTSTR pStr)
{
	_ASSERTE( NULL != section);
	_ASSERTE( NULL != key);
	_ASSERTE( NULL != pStr);
	if ( NULL == section || NULL == key ||  NULL == pStr )
	{
		return -1;
	}

	//if ( !m_bParsed )
	//{
	//	return -1;
	//}
	
	if ( 0 > putValue(section, key, pStr) )
	{
		return -1;
	}

	return 0;
}


/**	-----------------------------------------------------------------------
	\brief	메모리 INI정보에 Integer정보를 추가 함

	\param	
	\return
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::putInteger(LPCTSTR section, LPCTSTR key, LONG value)
{
	_ASSERTE( NULL != section);
	_ASSERTE( NULL != key);
	if ( NULL == section || NULL == key )
	{
		return -1;
	}

	//if ( !m_bParsed )
	//{
	//	return -1;
	//}

	TCHAR valstr[MAX_LINE] = {0,};

#if (_MSC_VER >= 1500) // vs2008 이상
    sprintf_s(valstr, MAX_LINE, _T("%d"), value);
#else
	StringCchPrintf(valstr, MAX_LINE, _T("%d"), value);
#endif

	if ( 0 > putValue( section, key, valstr ) )
	{
		return -1;
	}

	return 0;
}


int MemoryIni::dumpData()
{
	if ( 0 == m_sec.size() )
	{
		return -1;
	}


	// 메모리할당에 필요한 길이를 구함
	//

	ULONG tmp_size = MAX_LINE * 100;
	PBYTE tmp_data = (PBYTE) calloc( 1, tmp_size );
	ULONG written_size = 0;

	if ( NULL == tmp_data )
	{
		return -1;
	}

	TCHAR line[MAX_LINE] = {0};
	std::vector<INI_SECTION>::iterator it = m_sec.begin();

	for (it; it != m_sec.end(); it++)
	{
		PINI_SECTION ini_sec = &(*it);
		
#if (_MSC_VER >= 1500) // vs2008 이상
        sprintf_s( line, MAX_LINE, _T("[%s]\r\n"), ini_sec->name );
#else
        StringCchPrintf( line, MAX_LINE, _T("[%s]\r\n"), ini_sec->name );
#endif
        
		
		// 메모리 할당이 실패하면 더욱 늘려준다
		if ( 0 > sputs( line, (LPTSTR)tmp_data, tmp_size ) )
		{
			tmp_size = tmp_size * 3;
			tmp_data = (PBYTE) realloc(tmp_data, tmp_size);
			if ( NULL == tmp_data )
			{
				free(tmp_data);
				tmp_data = NULL;
				return -1;
			}

			sputs( line, (LPTSTR)tmp_data, tmp_size );
		}

		written_size = written_size + (_tcslen(line) * sizeof(TCHAR));
        
		std::vector<KEYDATA>::iterator it_key = ini_sec->keys.begin();

		for ( ; it_key != ini_sec->keys.end(); it_key++)
		{
			PKEYDATA ini_key = &(*it_key);
			
#if (_MSC_VER >= 1500) // vs2008 이상
            sprintf_s( line, MAX_LINE, _T("%s = %s\r\n"), ini_key->key, ini_key->value );
#else
            StringCchPrintf( line, MAX_LINE, _T("%s = %s\r\n"), ini_key->key, ini_key->value );
#endif

			// 메모리 할당이 실패하면 더욱 늘려준다
			if ( 0 > sputs( line, (LPTSTR)tmp_data, tmp_size ) )
			{
				tmp_size = tmp_size * 3;
				tmp_data = (PBYTE) realloc(tmp_data, tmp_size);
				if ( NULL == tmp_data )
				{
					free(tmp_data);
					tmp_data = NULL;
					return -1;
				}

				sputs( line, (LPTSTR)tmp_data, tmp_size );
			}

			written_size = written_size + (_tcslen(line) * sizeof(TCHAR));
		}
	}

	// m_data 우선 free시켜야 한다
	//

	if ( NULL != m_data )
	{
		free( m_data );
	}

	m_data = tmp_data;
	m_data_size = written_size;

	return 0;
}


int MemoryIni::sputs(LPCTSTR line, LPCTSTR data, ULONG datasz)
{
	size_t linesz = _tcslen(line) * sizeof(TCHAR);
	size_t fillsz = _tcslen(data) * sizeof(TCHAR);
	size_t leftsz = datasz - fillsz;

	if ( leftsz <= linesz )
	{
		return -1;
	}
    // dummy code
    if (FAILED(strcat((LPTSTR)data, line)))
	{
		return -1;
	}

	return 0;
}


/**	-----------------------------------------------------------------------
	\brief	메모리의 INI정보를 파일로 씀

	\param	
	\return
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::SaveToFile(LPCTSTR inifile, INI_CRYPT encrypt = NULL)
{
	_ASSERTE( NULL != inifile);
	if ( NULL == inifile)
		return -1;


	//	파일로 저장하기 전에 INI형태로 데이터를 변환 함
	//
	
	if ( 0 > dumpData() )
	{
		return -1;
	}
	
	HANDLE hFile = CreateFile( inifile, 
							   FILE_SHARE_READ|FILE_SHARE_WRITE, 
							   0, 
							   NULL, 
							   CREATE_ALWAYS, 
							   FILE_ATTRIBUTE_NORMAL, 
							   NULL );

	if ( INVALID_HANDLE_VALUE == hFile )
		return -1;


	// 암호화를 원한다면 암호화시켜서 저장
	//

	int retval = 0;
	DWORD dwWritten = 0;	

	if ( encrypt )
	{
		PBYTE save_data = NULL;
		DWORD save_size = 0;

		if ( 0 != encrypt(m_data, m_data_size, &save_data, &save_size) )
		{
			CloseHandle(hFile);
			return -1;
		}

		retval = WriteFile(hFile, save_data, save_size, &dwWritten, NULL);

		free(save_data);
	}
	else
	{
		retval = WriteFile(hFile, m_data, m_data_size, &dwWritten, NULL);
	}

	if ( 0 == retval )
	{
		CloseHandle(hFile);
		return -1;
	}

	CloseHandle(hFile);
	return 0;
}


/**	-----------------------------------------------------------------------
	\brief	INI파일을 메모리로 로드함

	\param	
	\return
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::LoadFromFile(LPCTSTR inifile, INI_CRYPT decrypt = NULL)
{

	_ASSERTE( NULL != inifile);
	if ( NULL == inifile)
		return -1;	
	
	HANDLE hFile = CreateFile( inifile, 
							   FILE_SHARE_READ,
							   0, 
							   NULL, 
							   OPEN_EXISTING, 
							   FILE_ATTRIBUTE_NORMAL, 
							   NULL );

	if ( INVALID_HANDLE_VALUE == hFile )
		return -1;


	PBYTE load_data = NULL;
	DWORD load_size = 0;
	DWORD dwSizeLow = 0;
	DWORD dwSizeHigh = 0;

	dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
	load_size = dwSizeHigh << 16 | dwSizeLow;

	HANDLE hMap = CreateFileMapping( hFile, 
									 NULL, 
									 PAGE_READONLY,
									 0, 
									 0, 
									 NULL );

	if ( INVALID_HANDLE_VALUE == hFile )
	{
		CloseHandle(hFile);
		return -1;
	}

	PBYTE pMap = (PBYTE) MapViewOfFile( hMap, 
										FILE_MAP_READ,
										0, 
										0, 
										0 );
	if ( NULL == pMap )
	{
		CloseHandle(hMap);
		CloseHandle(hFile);		
		return -1;
	}


	load_data = (PBYTE) calloc( 1, load_size );
	
	if ( NULL == load_data )
	{
		CloseHandle(hMap);
		CloseHandle(hFile);		
		return -1;
	}

	RtlCopyMemory( load_data, pMap, load_size );

	UnmapViewOfFile( pMap );
	CloseHandle( hMap );
	CloseHandle( hFile );


	//	이전에 m_data가 있으면 메모리 해제
	//

	if ( NULL != m_data)
	{
		free(m_data);
		m_data = NULL;
		m_data_size = 0 ;
	}


	//	암호화된 파일에 대한 복호화를 원한다면
	//

	if ( decrypt )
	{
		if ( 0 != decrypt(load_data, load_size, &m_data, &m_data_size) )
		{
			free(load_data);
			return -1;
		}

		free(load_data);
		load_data = NULL;
		load_size = 0;
	}
	else
	{
		m_data = load_data;
		m_data_size = load_size;
	}


	//	로드된 INI데이터를 파싱한다
	//

	parseData();

	return 0;
}


