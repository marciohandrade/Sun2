#ifndef _TESTCASEMSG_H_
#define _TESTCASEMSG_H_

enum eTESTCASE_MSG
{
	TESTCASE_FT_APPEND_STRING = 0,	// 특정 스트링을 추가한다. ( lParam 참고 )
	TESTCASE_FT_CLEAR_STRING,		// 스트링 초기화.
	TESTCASE_FT_SET_RECT,			// 영역 설정.
};


#endif
// _TESTCASEMSG_H_