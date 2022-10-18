#pragma once

#include "LogCommonDefine.h"

#define MAX_UNIT_TIME	10

class DLL_DECLARE CLogTimeFormat
{
public:
	CLogTimeFormat(VOID);
	~CLogTimeFormat(VOID);

	static int		GetTime(int nType, TCHAR* szTime, int size);					// nType의 시간을 반환한다.
	static int		GetDiffTimeType3(const TCHAR* szTime1, const TCHAR* szTime2);	//3번 형식 시간의 차.. 반드시. 뒤의 시간이 앞의시간보다 커야함.
	static int		GetDiffTimeType4(const TCHAR* szTime1, const TCHAR* szTime2);	//4번 형식 시간의 차.. 반드시. 뒤의 시간이 앞의시간보다 커야함.

private:
	static int		GetTimeType1(TCHAR* szTime, int size);			//1번 형식 20051018_07:43:32 과 같은 형식으로 반환한다.
	static int		GetTimeType2(TCHAR* szTime, int size);			//2번 형식 20051018과 같은 형식으로 반환한다.(날까지 반환)
	static int		GetTimeType3(TCHAR* szTime, int size);			//3번 형식 051021171543111 (2005년 10월 21일 17시 15분 43.111초)
	static int		GetTimeType4(TCHAR* szTime, int size);			//4번 형식 051021171543 (2005년 10월 21일 17시 15분 43초). 초단위
	static DWORD	ConvertTime4ToSecond(const TCHAR* szTime);
	static VOID		GetTimeFormFromWord(WORD wTime, TCHAR* szTime, int nSize );
};
