#include "UtilityCommon.h"
#include "ErrorControl.h"

static void
DefaultAssertionHandler(char* msg, char* file_name, DWORD line_number)
{
	printf("AssertionMessage: %s [file: %s : line: %u]\n", msg, file_name, line_number);
}

FlowControl::AssertionHandler_t
FlowControl::ms_AssertionHandler = DefaultAssertionHandler;

