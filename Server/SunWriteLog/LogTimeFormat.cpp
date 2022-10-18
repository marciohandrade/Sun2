#include "stdafx.h"
#include ".\logtimeformat.h"

CLogTimeFormat::CLogTimeFormat(VOID)
{
}

CLogTimeFormat::~CLogTimeFormat(VOID)
{
}

int CLogTimeFormat::GetTime(int nType, TCHAR* szTime, int size)
{
	switch(nType)
	{
	case 1:
		return GetTimeType1(szTime,size);
	case 2:
		return GetTimeType2(szTime,size);
	case 3:
		return GetTimeType3(szTime,size);
	case 4:
		return GetTimeType4(szTime,size);

	}
	return 0;		 
}

int CLogTimeFormat::GetTimeType1(TCHAR* szTime, int size)
{
	TCHAR szMonth[MAX_UNIT_TIME], szDay[MAX_UNIT_TIME], szHour[MAX_UNIT_TIME], szMinute[MAX_UNIT_TIME], szSecond[MAX_UNIT_TIME];

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	WORD wYear = sysTime.wYear;

	GetTimeFormFromWord(sysTime.wMonth, szMonth, MAX_UNIT_TIME * sizeof(TCHAR) );  //��
	GetTimeFormFromWord(sysTime.wDay, szDay, MAX_UNIT_TIME * sizeof(TCHAR) );      //��

	GetTimeFormFromWord(sysTime.wHour, szHour, MAX_UNIT_TIME * sizeof(TCHAR) );
	GetTimeFormFromWord(sysTime.wMinute, szMinute, MAX_UNIT_TIME * sizeof(TCHAR) );
	GetTimeFormFromWord(sysTime.wSecond, szSecond, MAX_UNIT_TIME * sizeof(TCHAR) );

	return _sntprintf(szTime, size, "%d%s%s_%s:%s:%s", wYear, szMonth, szDay, szHour, szMinute, szSecond);
}

int CLogTimeFormat::GetTimeType2(TCHAR* szTime, int size)
{
	TCHAR szMonth[MAX_UNIT_TIME], szDay[MAX_UNIT_TIME];

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	GetTimeFormFromWord(sysTime.wMonth, szMonth, MAX_UNIT_TIME * sizeof(TCHAR) );  //��
	GetTimeFormFromWord(sysTime.wDay, szDay, MAX_UNIT_TIME * sizeof(TCHAR) );      //��

	return _sntprintf(szTime, size, "%d%s%s", sysTime.wYear, szMonth, szDay);
}

int CLogTimeFormat::GetTimeType3(TCHAR* szTime, int size)
{
	TCHAR szMonth[MAX_UNIT_TIME], szDay[MAX_UNIT_TIME], szHour[MAX_UNIT_TIME];
	TCHAR szMinute[MAX_UNIT_TIME], szSecond[MAX_UNIT_TIME], szYear[MAX_UNIT_TIME];

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	//�⵵�� �� �� 2�ڸ��� �̾Ƴ���.
	WORD wYear = sysTime.wYear;
	_sntprintf(szYear, MAX_UNIT_TIME, "%d", wYear);
	std::string strYear = szYear;
	strYear = strYear.substr(2, 2);

	GetTimeFormFromWord(sysTime.wMonth, szMonth, MAX_UNIT_TIME * sizeof(TCHAR) );  //��
	GetTimeFormFromWord(sysTime.wDay, szDay, MAX_UNIT_TIME * sizeof(TCHAR) );      //��

	GetTimeFormFromWord(sysTime.wHour, szHour, MAX_UNIT_TIME * sizeof(TCHAR) );
	GetTimeFormFromWord(sysTime.wMinute, szMinute, MAX_UNIT_TIME * sizeof(TCHAR) );
	GetTimeFormFromWord(sysTime.wSecond, szSecond, MAX_UNIT_TIME * sizeof(TCHAR) );

	return _sntprintf(szTime, size-1, "%s%s%s%s%s%s%d", strYear.c_str(), szMonth, szDay, szHour, szMinute, szSecond, sysTime.wMilliseconds);
}

//4�� ���� 051021171543 (2005�� 10�� 21�� 17�� 15�� 43��). �ʴ���
int CLogTimeFormat::GetTimeType4(TCHAR* szTime, int size)
{
	TCHAR szMonth[MAX_UNIT_TIME], szDay[MAX_UNIT_TIME], szHour[MAX_UNIT_TIME];
	TCHAR szMinute[MAX_UNIT_TIME], szSecond[MAX_UNIT_TIME], szYear[MAX_UNIT_TIME];

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	//�⵵�� �� �� 2�ڸ��� �̾Ƴ���.
	WORD wYear = sysTime.wYear;
	_sntprintf(szYear, MAX_UNIT_TIME, "%d", wYear);
	std::string strYear = szYear;
	strYear = strYear.substr(2, 2);

	GetTimeFormFromWord(sysTime.wMonth, szMonth, MAX_UNIT_TIME * sizeof(TCHAR) );  //��
	GetTimeFormFromWord(sysTime.wDay, szDay, MAX_UNIT_TIME * sizeof(TCHAR) );      //��

	GetTimeFormFromWord(sysTime.wHour, szHour, MAX_UNIT_TIME * sizeof(TCHAR) );
	GetTimeFormFromWord(sysTime.wMinute, szMinute, MAX_UNIT_TIME * sizeof(TCHAR) );
	GetTimeFormFromWord(sysTime.wSecond, szSecond, MAX_UNIT_TIME * sizeof(TCHAR) );

	return _sntprintf(szTime, size, "%s%s%s%s%s%s\0", strYear.c_str(), szMonth, szDay, szHour, szMinute, szSecond);
}


//3�� ���� �ð��� ��.. �ݵ��. ���� �ð��� ���ǽð����� Ŀ����.
int	CLogTimeFormat::GetDiffTimeType3(const TCHAR* szTime1, const TCHAR* szTime2)
{
	//������ 3�� Ÿ���̶�� �ص� �и�������� ������� �����Ƿ�,
	//4�� Ÿ�԰� ����� ����.
	return GetDiffTimeType4( szTime1, szTime2 );
}


//43�� ���� �ð��� ��.. 
//�Ѵ��� 30�Ϸ� ���..
int	CLogTimeFormat::GetDiffTimeType4(const TCHAR* szTime1, const TCHAR* szTime2)
{
	if( szTime1 == NULL || (strcmp( szTime1, "") == 0) )
		return 0;

	if( szTime2 == NULL || (strcmp( szTime2, "") == 0) )
		return 0;

	int nSecond1 = ConvertTime4ToSecond( szTime1 );
	int nSecond2 = ConvertTime4ToSecond( szTime2 );
	if( nSecond2 >= nSecond1)
		return nSecond2 - nSecond1;
	else
		return nSecond1 - nSecond2;
}



DWORD CLogTimeFormat::ConvertTime4ToSecond(const TCHAR* szTime)
{
	if( szTime == NULL || (strcmp( szTime, "") == 0) )
		return 0;

	std::string strTemp;
	std::string strTime = szTime;
	int nYear = 0, nMonth = 0, nDay = 0, nHour = 0, nMinute = 0, nSecond = 0;
	DWORD dwTotal = 0;

	//1. �⵵
	strTemp = strTime.substr(0, 2);	
	nYear = atoi( strTemp.c_str() );
	dwTotal += nYear*12*30*24*60*60;

	//2. ��
	strTemp = strTime.substr(2, 2);	
	nMonth = atoi( strTemp.c_str() );
	dwTotal += nMonth*30*24*60*60;

	//3. ��
	strTemp = strTime.substr(4, 2);	
	nDay = atoi( strTemp.c_str() );
	dwTotal += nDay*24*60*60;

	//4. �ð�
	strTemp = strTime.substr(6, 2);	
	nHour = atoi( strTemp.c_str() );
	dwTotal += nHour*60*60;

	//5. ��
	strTemp = strTime.substr(8, 2);	
	nMinute = atoi( strTemp.c_str() );
	dwTotal += nMinute*60;

	//6. ��
	strTemp = strTime.substr(10, 2);	
	nSecond = atoi( strTemp.c_str() );
	dwTotal += nSecond;

	return dwTotal;
}

//Word ������ ���� �ð����� ���� ���ڿ� ������ �ð����·� ��ȯ�Ѵ�.
VOID CLogTimeFormat::GetTimeFormFromWord( WORD wTime, TCHAR* szTime, int nSize )
{
	if(wTime < 10)
		_sntprintf(szTime, nSize-1, "0%d", wTime);
	else
		_sntprintf(szTime, nSize-1, "%d", wTime);

	szTime[nSize-1] = '\0';
}
