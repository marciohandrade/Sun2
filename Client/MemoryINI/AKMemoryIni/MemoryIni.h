/*-----------------------------------------------------------------------------
 * MemoryIni.h
 *-----------------------------------------------------------------------------
 * 암호화된 Ini파일을 메모리에서 읽고 쓸수 있게 하기 위하여 만든 클래스
 *-----------------------------------------------------------------------------
**---------------------------------------------------------------------------*/

#ifndef __MEMROYINI_H__
#define __MEMROYINI_H__


#include <stdlib.h>
#include <crtdbg.h>
#include <vector>
#include <windows.h>
#include <crtdbg.h>
#include <tchar.h>

#define STRSAFE_LIB
#include <strsafe.h>
//using namespace std;

#define MAX_LINE 1024


typedef int (__stdcall *INI_CRYPT)(	PBYTE in_data, 
								    DWORD in_size, 
									PBYTE *out_data,
									DWORD *out_size );

typedef struct _KEYDATA
{
	TCHAR key[MAX_LINE];
	TCHAR value[MAX_LINE];

} KEYDATA, *PKEYDATA;


typedef struct _INI_SECTION
{
	TCHAR name[MAX_LINE];
	std::vector<KEYDATA> keys;

} INI_SECTION, *PINI_SECTION;



class MemoryIni
{
public:
	MemoryIni();
	virtual ~MemoryIni();

	int		LoadFromFile( LPCTSTR inifile, INI_CRYPT decrypt );
	int		SaveToFile( LPCTSTR inifile, INI_CRYPT encrypt );
	int		SetData( LPTSTR ini_data, DWORD ini_size );
	int		GetData( LPTSTR ini_data, DWORD ini_size, PDWORD ini_written );

	int		getString( LPCTSTR section, LPCTSTR key, LPTSTR pStr, DWORD slen );
	int		putString( LPCTSTR section, LPCTSTR key, LPCTSTR pStr );
	int		getInteger( LPCTSTR section, LPCTSTR key, LONG *pInt );
	int		putInteger( LPCTSTR section, LPCTSTR key, LONG value );

private:
	int		addNewSection( LPCTSTR name );
	int		addtoLastSection( LPCTSTR key, LPCTSTR value );
	int		getValue( LPCTSTR section, LPCTSTR key, LPCTSTR value, DWORD slen );
	int		putValue( LPCTSTR section, LPCTSTR key, LPCTSTR value );
	int		sgets( LPTSTR line, DWORD linesz, LPTSTR *ppos );
	int		sgets2( LPSTR line, DWORD linesz, LPSTR *ppos);
	int		sputs( LPCTSTR line, LPCTSTR data, ULONG datasz );
	int		parseData();
	int		dumpData();

	void	strip( LPTSTR str );
	LPTSTR	strskip( LPTSTR s );


	PBYTE	m_data;
	ULONG	m_data_size;
	BOOL	m_bParsed;
	BOOL	m_bInit;

	std::vector<INI_SECTION> m_sec;
};


#endif	/*	__AKMEMROYINI_H__	*/

