
//=============================================================================================================================
/// FileListLoader class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 03
	@remark
	- Webzen Crypt File 
	@note
	- 특정 파일로부터 탭과 속성값을 읽어들이는 클래스
	@history 
	- 
*/
//=============================================================================================================================


#ifndef __FILE_LIST_LOADER_H
#define __FILE_LIST_LOADER_H

#include <list>
#include <vector>
#include <functional>
#include <algorithm>

#include <windows.h>


typedef std::vector<CHAR*>				COLUMN_LIST;
typedef COLUMN_LIST::iterator		COLUMN_LIST_IT;
typedef std::pair<CHAR*, COLUMN_LIST*>	LINE_PAIR;
typedef std::list<LINE_PAIR>				LINE_LIST;
typedef LINE_LIST::iterator			LINE_LIST_IT;

#define SAFE_NUMERIC_TYPECAST( F_TYPE, F_VALUE, T_TYPE )	\
	( (F_TYPE)(((T_TYPE)(0xFFFFFFFFFFFFFFFFLL))&F_VALUE) == F_VALUE )

//
//
//

class AttrTag
{
private:
	CHAR*	m_Name;
	DWORD	m_Length;

public:
	AttrTag( const CHAR* name, const DWORD length ) : m_Name((CHAR*)name), m_Length(length) {}
	inline BOOL		IsMatch( const CHAR* matchname )
	{
		return (BOOL)( strncmp( m_Name, matchname, m_Length ) == 0 );
	}

	inline CHAR*	GetString() const
	{
		return m_Name;
	}
};

typedef AttrTag		AttrValueName;

class AttrValue
{
private:
	CHAR*	m_Name;
	DWORD	m_Length;

public:
	AttrValue( CHAR* name, DWORD length ) : m_Name(name), m_Length(length) {}
	inline CHAR*	GetString() const
	{
		return m_Name;
	}

	template<typename _VALUETYPE>
	inline BOOL		GetValue( _VALUETYPE& value )
	{
		DWORD _tValue;

		if( sscanf( m_Name, "%u", &_tValue ) != 1 )
			return FALSE;

		if( SAFE_NUMERIC_TYPECAST(DWORD, _tValue, _VALUETYPE) )
		{
			value = (_VALUETYPE)_tValue;
			return TRUE;
		}

		return FALSE;
	}

	inline CHAR*	ToString()
	{
		return m_Name;
	}
};

//
//
//

class FileListLoader
{
public:
	typedef INT	SECTION_TYPE;
	enum eSECTION_TYPE
	{
		SECTION_DEFAULT = 0,
		SECTION_COLUMNS = 1,
		SECTION_DATA	= 2,
		SECTION_EOL		= 3,
		SECTION_TAB		= 10,
		SECTION_ERROR	= -1
	};
public:
	FileListLoader();
	~FileListLoader() { _UnLoad(); }

public:	// <INTERFACEs>
	// <Name>
	//   Load
	// <Arguments>
	//   <0> pszFileName : TCHAR*  ->
	//   <1> bReload : BOOL  -> 리로드 여부
	// <Description>
	//   주어진 파일을 읽어들여 분석작업을 수행한다.
	//   연결된 부가작업: 읽어들인 파일이 암호화 되어 있는지의 여부를 판별해서 선택적으로 수행한다.
	BOOL			Load( const TCHAR* pszFileName, BOOL bReload );
	// <Name>
	//   LoadBuffer
	// <Arguments>
	//   <0> pBuffer : BYTE*  -> 버퍼 포인터
	// <Description>
	//   이미 읽어들인 버퍼를 통해 분석작업을 수행한다.
	BOOL			LoadBuffer( BYTE* pBuffer, const DWORD dwBufSize );
	VOID			Close() { _UnLoad(); };
	template<typename _T>
	BOOL			ReadValue( _T& OUT value );
	BOOL			ReadString( CHAR* value, DWORD length );

public: // <INTERFACEs><INFOs>
	DWORD			GetLineCount() { return (DWORD)m_ScriptList.size(); }	// 다 읽은 이후에 유효한데... 적절한 방안을 강구하자.

public: // <INTERFACEs><CALLBACK>
	//typedef SECTION_TYPE	(*PARSESECTION)( PVOID pPassingPtr, const CHAR* pszSectionName, const DWORD dwBufSize );
	//typedef BOOL			(*PARSESECTION_DATA)( PVOID pPassingPtr, const CHAR* pszSectionName, const DWORD dwBufSize, const DWORD dwValue );

	typedef SECTION_TYPE	(*PARSESECTION)(
									PVOID pPassingPtr,
									AttrTag tagName
									);
	typedef BOOL			(*PARSESECTION_DATA)(
									PVOID pPassingPtr,
									AttrValueName tagValueName,
									AttrValue tagValue
									);

	VOID			SectionCallbackAlloc( PARSESECTION pHandler, PVOID pPassingPtr )
	{ fnParseSection = pHandler; pParseSectionPassingPtr = pPassingPtr; }
	VOID			SectionCallbackRelease() { fnParseSection = NULL; }
	VOID			SectionDataCallbackAlloc( PARSESECTION_DATA pHandler, PVOID pPassingPtr )
	{ fnParseSectionData = pHandler; pParseSectionDataPassingPtr = pPassingPtr; }
	VOID			SectionDataCallbackRelease() { fnParseSectionData = NULL; }

private: //for_each
	VOID			ColoumnFree( LINE_PAIR value );

private:
	HANDLE			_OPENFILEnREADBUFER( const TCHAR* slList );
	BOOL			_Load();
	BOOL			_UnLoad();
	eSECTION_TYPE	_LoadSection( LINE_LIST_IT& it_line, LINE_LIST_IT& itend_line );
	BOOL			_LoadList();

	// LIST CONTROL
	VOID			_RemoveLine( LINE_LIST_IT& it_line );

	inline BOOL		IsComplete() { return m_bLoaded; }
	inline DWORD	GetColumnCount() { return m_dwColumnCount; }

	inline BOOL		IsWhitespaceChar (CHAR ch)
	{ return (ch == ' ' || ch == '\t'); }

	inline BOOL		IsComment (CHAR* pCh)
	{ return (*pCh == '/' && *(pCh+1) == '/'); }

	inline BOOL		IsSection (CHAR ch)
	{ return (ch == '['); }

	inline BOOL		IsEOLChar( CHAR* pCh )
	{ return (BOOL)(*(pCh+1) == '\0'); }


private:
	BOOL			m_bLoaded;
	LINE_LIST		m_ScriptList;
	CHAR*			m_pBuffer;
	DWORD			m_dwColumnCount;
	eSECTION_TYPE	m_eSECTION_TYPE;

	// iterator control fields
	struct
	{
		LINE_LIST_IT	it_LINE;
		COLUMN_LIST_IT	it_COL;
		COLUMN_LIST_IT	itend_COL;
	};

	PARSESECTION		fnParseSection;
	PVOID				pParseSectionPassingPtr;
	PARSESECTION_DATA	fnParseSectionData;
	PVOID				pParseSectionDataPassingPtr;
};

//
//
//

template<typename _T>
BOOL		FileListLoader::ReadValue( _T& OUT value )
{
	DWORD _tValue;
	BOOL bContinue;

	if( m_ScriptList.size() == 0 )
		return FALSE;

	do
	{
		bContinue = FALSE;
		if( it_COL != itend_COL )
		{
			if( sscanf( *it_COL, "%u", &_tValue ) != 1 )
				return FALSE;

			if( SAFE_NUMERIC_TYPECAST(DWORD, _tValue, _T) )
			{
				value = (_T)_tValue;
				++it_COL;
				return TRUE;
			}
			printf( "TYPE OVERFLOW\n" );
			return FALSE;
		}
		else if( it_LINE != m_ScriptList.end() )
		{
			++it_LINE;
			if( it_LINE != m_ScriptList.end() )
			{
				it_COL = (*it_LINE).second->begin();
				itend_COL = (*it_LINE).second->end();
				bContinue = TRUE;
			}
		}
	} while( bContinue );

	return FALSE;
}


#endif //__FILE_LIST_LOADER_H

