#include "SunClientPrecompiledHeader.h"
#include "testcasemap.h"

TestCaseMap::TestCaseMap(void)
{
	SetName( _T("TestCaseMap") );
}

TestCaseMap::~TestCaseMap(void)
{
}


//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
TestCaseMap::Init()							
{
	return TRUE;	
}


//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
TestCaseMap::Process( DWORD dwTick )
{
	return TRUE;
}


//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
TestCaseMap::Render( DWORD dwTick )
{
	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
TestCaseMap::Release()
{

}

//-------------------------------------------------------------------------------------------
/**
*/
void 
TestCaseMap::ParsePacket( MSG_BASE * pMsg )
{

}

//-------------------------------------------------------------------------------------------
/**
*/
void 
TestCaseMap::RenderText()
{

}


//-------------------------------------------------------------------------------------------
/**
*/
void 
TestCaseMap::ProcessKeyInput(DWORD dwTick)
{

}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
TestCaseMap::EntMessage(WORD wMessage, int iParam1, int iParam2, int iParam3/* = 0*/, int iParam4/* = 0*/)
{
	return FALSE;

}