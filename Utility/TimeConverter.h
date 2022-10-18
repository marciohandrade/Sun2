#pragma once

namespace util
{

#include <time.h>


inline int ConvertTCharToInt(const TCHAR* pszNumber)
{
	TCHAR szUnit[2];	szUnit[1] = '\0';

	szUnit[0] = pszNumber[0];

	return atoi( szUnit );
}

inline __time64_t ConvertTCharToTime_t(const TCHAR* pszTime)
{
	TCHAR szUnit[2];	szUnit[1] = '\0';

	// DB datetime 타입의 문자열을 struct tm 구조체로 변환한다.
	struct tm tmDate = { };
	tmDate.tm_year = ConvertTCharToInt(&pszTime[0])*1000 + ConvertTCharToInt(&pszTime[1])*100 + ConvertTCharToInt(&pszTime[2])*10 + ConvertTCharToInt(&pszTime[3]) - 1900;
	tmDate.tm_mon = ConvertTCharToInt(&pszTime[5])*10 + ConvertTCharToInt(&pszTime[6]) - 1;
	tmDate.tm_mday = ConvertTCharToInt(&pszTime[8])*10 + ConvertTCharToInt(&pszTime[9]);
	tmDate.tm_hour = ConvertTCharToInt(&pszTime[11])*10 + ConvertTCharToInt(&pszTime[12]);
	tmDate.tm_min = ConvertTCharToInt(&pszTime[14])*10 + ConvertTCharToInt(&pszTime[15]);

	// struct tm를 time_t로 변환한다.
	__time64_t time64_t = _mktime64( &tmDate );

	return time64_t;
}


//__NA_000313_20070223_REALTIME_CHAT_BLOCK
inline DWORD64 ConvertTCharToDWORD64(const TCHAR *ptszTime)
{
    int cnt = 0;
    TCHAR tszConvertTime[MAX_SMALLDATETIME_SIZE + 1];
    memset(tszConvertTime, 0, sizeof(TCHAR) * MAX_SMALLDATETIME_SIZE);

    _tcsncpy(tszConvertTime, ptszTime, sizeof(TCHAR) * MAX_SMALLDATETIME_SIZE);
    tszConvertTime[MAX_SMALLDATETIME_SIZE] = '\0';

    for(int i = 0; i < MAX_SMALLDATETIME_SIZE; i++)
    {
        switch(i)
        {
        case 4:
        case 7:
        case 10:
        case 13:
            break;
        default:
            {
                tszConvertTime[cnt] = ptszTime[i];
                cnt++;
            }
            break;
        }
    }

    tszConvertTime[cnt] = '\0';

    return _atoi64(tszConvertTime);
}
//#endif

inline DWORD64 ConvertTMidTimeToDWORD64(TCHAR *ptszTime)
{
    int cnt = 0;
    TCHAR tszConvertTime[MAX_MIDDATETIME_SIZE + 1];
    memset(tszConvertTime, 0, sizeof(TCHAR) * MAX_MIDDATETIME_SIZE);

    _tcsncpy(tszConvertTime, ptszTime, sizeof(TCHAR) * MAX_MIDDATETIME_SIZE);
    tszConvertTime[MAX_MIDDATETIME_SIZE] = '\0';

    for(int i = 0; i < MAX_MIDDATETIME_SIZE; i++)
    {
        switch(i)
        {
        case 4:
        case 7:
        case 10:
        case 13:
        case 16:
            break;
        default:
            {
                tszConvertTime[cnt] = ptszTime[i];
                cnt++;
            }
            break;
        }
    }

    tszConvertTime[cnt] = '\0';

    return _atoi64(tszConvertTime);
}
}