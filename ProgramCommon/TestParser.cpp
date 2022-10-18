#include "StdAfx.h"
#include ".\testparser.h"

#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
// (NOTE) (100131) (WAVERIX) �� �״�� �׽�Ʈ���̹Ƿ� �ɾ�� ä ���α�� �Ѵ�. ���� ���Ž� ���ϵ�
// ������ �� �ְų� �׽�Ʈ�� ��� �������.

TestParser::TestParser(void)
{
}

TestParser::~TestParser(void)
{
}

VOID		TestParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );	
}

VOID		TestParser::Release()
{


}

BOOL		TestParser::ILoad( eSCRIPT_CODE ScriptCode, BYTE* pSTREAM, DWORD dwSize )
{
    __UNUSED((ScriptCode, pSTREAM, dwSize));
	return TRUE;
}

BOOL		TestParser::_LoadEx( eSCRIPT_CODE ScriptCode, util::SolarFileReader& sr, BOOL bReload )
{
    __UNUSED(sr);
	switch( ScriptCode )
	{
	case SCRIPT_TEST:
		return _Load( sr, bReload );
	}

	return TRUE;
}

BOOL		TestParser::_Load( util::SolarFileReader& sr, BOOL bReload )
{
    __UNUSED((sr, bReload));
	int nRowSize = GetRowSize();

	TestInfo info;

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		info.a = GetDataDWORD( "A", nRow );
		strncpy( info.b, GetDataString( "B", nRow ), 10 );		
		info.c = GetDataFloat( "C", nRow );
		info.d = GetDataFloat( "D", nRow );
		strncpy( info.e, GetDataString( "E", nRow ), 10 );
	}

	return TRUE;
}


#endif //__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM