#ifndef __PROGRAMCOMMON_TEST_PARSER_H
#define __PROGRAMCOMMON_TEST_PARSER_H
#pragma once // delete reserved

#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM

struct TestInfo
{
	unsigned int 	a;
	char			b[10];
	float			c;
	float			d;
	char			e[10];	
};

class TestParser : public util::Singleton<TestParser>, public IParser
{
public:
	TestParser(void);
	~TestParser(void);

public:
			VOID		Init( char * pszPackFileName );
			VOID		Release();

private:
	BOOL				_Load( util::SolarFileReader& sr, BOOL bReload );

public:
	virtual BOOL		ILoad( eSCRIPT_CODE ScriptCode, BYTE* pSTREAM, DWORD dwSize );
	virtual	BOOL		_LoadEx( eSCRIPT_CODE ScriptCode, util::SolarFileReader& sr, BOOL bReload );

};


#endif // __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM

#endif //__PROGRAMCOMMON_TEST_PARSER_H
