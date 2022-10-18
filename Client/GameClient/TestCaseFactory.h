#pragma once

#include "TestCaseType.h"
#include "TestCaseFormatText.h"

class TestCase;

class TestCaseFactory
{
public:
    enum UiTestCaseType
    {
        TEST_CASE_NONE = -1,
        // here is add Type
        TEST_CASE_FORMATTEXT = 0,
        TEST_CASE_CHAT,
        TEST_CASE_MAP,
        TEST_CASE_BATTLEZONE,

        TEST_CASE_MAX
    };

public:
	TestCaseFactory(void);
	virtual ~TestCaseFactory(void);

	static TestCase * MakeTestCase( SUN_STRING strTestCaseName );
};
