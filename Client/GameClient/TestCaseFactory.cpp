#include "SunClientPrecompiledHeader.h"
#include "testcasefactory.h"
#include "TestCaseChat.h"
#include "TestCaseMap.h"

SUN_STRING TestCaseNames[TestCaseFactory::TEST_CASE_MAX]	= 
{
	_T("FormatText"),	// TEST_CASE_FORMATTEXT
	_T("CHAT"),			// TEST_CASE_CHAT
	_T("Map"),			// TEST_CASE_MAP
    _T("BATTLEZONE"),   // TEST_BATTLEZONE
};


TestCaseFactory::TestCaseFactory(void)
{
}

TestCaseFactory::~TestCaseFactory(void)
{
}

TestCase * 
TestCaseFactory::MakeTestCase( SUN_STRING strTestCaseName )
{
	UiTestCaseType eType = TEST_CASE_NONE;

	TestCase * pTestCase = NULL;
	int i;
	for (i = 0; i < TEST_CASE_MAX; ++i)
	{
		if ( _Strncmp( 
			(TCHAR *)(const TCHAR *)TestCaseNames[i].c_str(), 
			_tcslen(TestCaseNames[i].c_str()), 
			(TCHAR *)(const TCHAR *)strTestCaseName.c_str(), 
			_tcslen(TestCaseNames[i].c_str()) ) ) 
		{
			break;
		}
	}
	
	eType = (UiTestCaseType)i;

	switch (eType ) 
	{
	case TEST_CASE_FORMATTEXT:
		{
			pTestCase =	new TestCaseFormatText;
		}
		break;

	case TEST_CASE_CHAT:
		{
			pTestCase = new TestCaseChat;
		}
		break;

	case TEST_CASE_MAP:
		{
			pTestCase = new TestCaseMap;
		}
		break;

	case TEST_CASE_NONE:
	case TEST_CASE_MAX:
	default:
		break;
	}

	return pTestCase;
}
