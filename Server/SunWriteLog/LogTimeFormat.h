#pragma once

#include "LogCommonDefine.h"

#define MAX_UNIT_TIME	10

class DLL_DECLARE CLogTimeFormat
{
public:
	CLogTimeFormat(VOID);
	~CLogTimeFormat(VOID);

	static int		GetTime(int nType, TCHAR* szTime, int size);					// nType�� �ð��� ��ȯ�Ѵ�.
	static int		GetDiffTimeType3(const TCHAR* szTime1, const TCHAR* szTime2);	//3�� ���� �ð��� ��.. �ݵ��. ���� �ð��� ���ǽð����� Ŀ����.
	static int		GetDiffTimeType4(const TCHAR* szTime1, const TCHAR* szTime2);	//4�� ���� �ð��� ��.. �ݵ��. ���� �ð��� ���ǽð����� Ŀ����.

private:
	static int		GetTimeType1(TCHAR* szTime, int size);			//1�� ���� 20051018_07:43:32 �� ���� �������� ��ȯ�Ѵ�.
	static int		GetTimeType2(TCHAR* szTime, int size);			//2�� ���� 20051018�� ���� �������� ��ȯ�Ѵ�.(������ ��ȯ)
	static int		GetTimeType3(TCHAR* szTime, int size);			//3�� ���� 051021171543111 (2005�� 10�� 21�� 17�� 15�� 43.111��)
	static int		GetTimeType4(TCHAR* szTime, int size);			//4�� ���� 051021171543 (2005�� 10�� 21�� 17�� 15�� 43��). �ʴ���
	static DWORD	ConvertTime4ToSecond(const TCHAR* szTime);
	static VOID		GetTimeFormFromWord(WORD wTime, TCHAR* szTime, int nSize );
};
