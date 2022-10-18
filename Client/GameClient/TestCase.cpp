#include "SunClientPrecompiledHeader.h"
#include "testcase.h"

TestCase::TestCase(void)
{
}

TestCase::~TestCase(void)
{
}

//-------------------------------------------------------------------------------------------
/**
*/
void				
TestCase::SetName(const SUN_STRING & strName)
{
	this->m_strName = strName;
}

//-------------------------------------------------------------------------------------------
/**
*/
void				
TestCase::SetName(TCHAR * pszName)
{
	this->m_strName = pszName;
}

//-------------------------------------------------------------------------------------------
/**
*/
const SUN_STRING &	
TestCase::GetName() const
{
	return this->m_strName;
}


//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
TestCase::EntMessage(WORD wMessage, int iParam1, int iParam2, int iParam3 /*= 0*/, int iParam4 /*= 0*/)
{
	return FALSE;
}