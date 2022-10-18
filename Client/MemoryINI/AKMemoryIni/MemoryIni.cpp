/*-----------------------------------------------------------------------------
 * MemoryIni.cpp
 *-----------------------------------------------------------------------------
 * ��ȣȭ�� Ini������ �޸𸮿��� �а� ���� �ְ� �ϱ� ���Ͽ� ���� Ŭ����
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
	\brief		ppos���ۿ��� linsz���� ���� ũ��� ���� �о line�� ä��
	
	\remark		sgets2()�Լ��� �����丵�� �Լ���

	\param	
				line	- �� Line�� ä����
				linesz	- line �� ũ��
				ppos	- line�� �˻��� ����(IN/OUT���)
	\return
				������ ������ ������ ����, 0�� �����ϸ� Line���� ����� 
				���ڰ� �����Ƿ�(��:���ӵ� ĳ�������ϵ�, ���ӵ� NULL���ڵ�)
				���н� -1�� ������
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
	
	
	//	������ġ�� ���̰ų�(�̹� �˻������� ���) ���� ����
	//

	if ( pos >= end )
	{
		return -1;
	}

	//	���� ��ġ�� ���� ���� ������, ä��� �ִ� ���۰� �������������� �ݺ�
	//

	while ( pos < end && linesz > 0 )
	{
		int chsize = (int)_tcsnbcnt( pos, 1 );
		UINT ch = _tcsnextc(pos);

		if ( ch == _T('\0') )
		{
			if ( !nread )
			{
				//	pos = CharNext(pos) �� �ƴ�!!
				//	�ֳ��ϸ� *pos�� NULL�̸� ���� �����͸� �����ϱ� ����
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

		
		//	�߿�!!
		//			�����ִ� ���ۻ���� chsize���� �۴ٸ� chsize�� ������
		//			���⼭ üũ�� ���� ������ linsz�� �������� �ɰ��̸�
		//			line�� linesz���� �߸��� ����� �� ���̴�
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
	\brief		ppos���ۿ��� linsz���� ���� ũ��� ���� �о line�� ä��

	\param	
				line	- �� Line�� ä����
				linesz	- line �� ũ��
				ppos	- line�� �˻��� ����(IN/OUT���)
	\return
				������ ������ ������ ����, 0�� �����ϸ� Line���� ����� 
				���ڰ� �����Ƿ�(��:���ӵ� ĳ�������ϵ�, ���ӵ� NULL���ڵ�)
				���н� -1�� ������
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


	// pos�� end_marker�� ���ٸ� �̹� ��ȸ�� ��ģ ���Ƿ� ����
	//

	if (pos >= end_marker)
	{
		return -1;
	}


	//
	//	���� pos�� end_marker�� �����ϱ��������� NULL���ڴ� ������
	//	pos�� end_marker�� ���� ������ NULL�̶�� ������ ����
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


	//	���̻� ������ ������ ���� ���
	//

	if (0 == i && pos == end_marker)
		return -1;


	line[i] = _T('\0');
	*ppos = pos;

	return (int)i;
}


/**	-----------------------------------------------------------------------
	\brief		���ڷ� ���� s�� ó������ �˻��Ͽ� �� ������ ������
				���鹮�ڸ� �˻��Ͽ� �ǳʶ�

	\param	
				s		- ��Ʈ��
	\return
				���н� NULL����, ������ ���鹮�ڰ� �ƴ� ��ġ�� ������
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
	\brief		�¿� ���鹮�ڰ� �����ϸ� ������

	\param	
				str		- ���鹮�ڸ� ������ ��Ʈ��
	\return
				���ϰ� ����
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
	\brief		���Ǹ��� ���ڷ� ������ ���ο� ������ �߰���

	\param	
				name	- ���Ǹ�
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
	\brief		���� �м��ϰ� �ִ� ������ ���� �ֱ� ���ǿ� key, value
				���� �߰���
	
	\remark		Internal �Լ���

	\param	
				key		- �߰��� Ű�̸�
				value	- �߰��� ��������
	\return
	\code
	\endcode		
-------------------------------------------------------------------------*/
int MemoryIni::addtoLastSection(LPCTSTR key, LPCTSTR value)
{

	//	���� �ֱٿ� ������ ��ϵȰ� ���ٸ� key, value���� ����� �� ����
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
	\brief		�����Ϳ��� ������ ���Ͽ� �������̽� ������(������ �ʿ�)

	\remark		MemoryIni�� ��� m_data�� ���� Ȯ���ϱ� ���� �ʿ�

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
	\brief	�����Ϳ��� ������ ���Ͽ� �������̽� ������(������ �ʿ�)

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
	\brief		m_data�� ��� ini�����͸� �м���

	\remark		Internal �Լ���
	\param	
	\return
				���н� -1, ������ 0
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
		// ������ �ǳʶڴ�
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


	//	MAX_LINE�� ��ġ�� �ʰ� �ϱ� ���ؼ� key, value ���̸� ����
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

				//	������ key�� �ִٸ� ���� ��
				//

				if ( 0 == _tcsicmp(ini_key->key, key) )
				{
					valuelen = MAX_LINE - _tcslen(ini_key->key);
					strncpy(ini_key->value, value, valuelen);
					return 0;
				}
			}


			// key�� �������� ������, ���ο� Ű�� �߰�
			
			valuelen = MAX_LINE - _tcslen(key);

			KEYDATA keydata = {0};			
			strncpy(keydata.key, key, MAX_LINE);			
			strncpy(keydata.value, value, valuelen);

			ini_sec->keys.push_back( keydata );
			return 0;
		}
	}
	
	// section�� �������� ������, ���ο� ���� �߰�
	addNewSection( section );
	addtoLastSection( key, value );

	return 0;
}


/**	-----------------------------------------------------------------------
	\brief	�޸� INI������ String������ ���� ��

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
	\brief	�޸� INI������ Integer������ ���� ��

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
	\brief	�޸� INI������ String������ �߰� ��

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
	\brief	�޸� INI������ Integer������ �߰� ��

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

#if (_MSC_VER >= 1500) // vs2008 �̻�
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


	// �޸��Ҵ翡 �ʿ��� ���̸� ����
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
		
#if (_MSC_VER >= 1500) // vs2008 �̻�
        sprintf_s( line, MAX_LINE, _T("[%s]\r\n"), ini_sec->name );
#else
        StringCchPrintf( line, MAX_LINE, _T("[%s]\r\n"), ini_sec->name );
#endif
        
		
		// �޸� �Ҵ��� �����ϸ� ���� �÷��ش�
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
			
#if (_MSC_VER >= 1500) // vs2008 �̻�
            sprintf_s( line, MAX_LINE, _T("%s = %s\r\n"), ini_key->key, ini_key->value );
#else
            StringCchPrintf( line, MAX_LINE, _T("%s = %s\r\n"), ini_key->key, ini_key->value );
#endif

			// �޸� �Ҵ��� �����ϸ� ���� �÷��ش�
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

	// m_data �켱 free���Ѿ� �Ѵ�
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
	\brief	�޸��� INI������ ���Ϸ� ��

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


	//	���Ϸ� �����ϱ� ���� INI���·� �����͸� ��ȯ ��
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


	// ��ȣȭ�� ���Ѵٸ� ��ȣȭ���Ѽ� ����
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
	\brief	INI������ �޸𸮷� �ε���

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


	//	������ m_data�� ������ �޸� ����
	//

	if ( NULL != m_data)
	{
		free(m_data);
		m_data = NULL;
		m_data_size = 0 ;
	}


	//	��ȣȭ�� ���Ͽ� ���� ��ȣȭ�� ���Ѵٸ�
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


	//	�ε�� INI�����͸� �Ľ��Ѵ�
	//

	parseData();

	return 0;
}


