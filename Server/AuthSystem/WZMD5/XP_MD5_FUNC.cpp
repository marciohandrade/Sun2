#include "XP_MD5_FUNC.H"
#include "WZ_MD5.H"


#define CHECK_LIMIT(C_VALUE, C_MAX) (C_VALUE<0?FALSE:C_VALUE>(C_MAX-1)?FALSE:TRUE)


extern void printError (SRV_PROC *pSrvProc, CHAR* szErrorMsg);
extern void printUsage (SRV_PROC *pSrvProc);
extern void BytesToStr(DWORD cb, void* pv, LPSTR sz);



//<=====================================================
//< Name: bool WZ_MD5_EncodeKeyVal()
//< Desc: 특정 문자열을 인덱스를 이용하여 MD5 값으로 변환
//< retval: 성공하면 true / 실패하면 false
//<=====================================================
WZ_DECLAREDLL bool XP_MD5_CryptKey( SRV_PROC* pSrvProc )
{
	// XP 중 인자체크에 들어가는 변수들
	BYTE	bType[XP_MD5_ENCODE_PARAM_NUM];
	ULONG	cbMaxLen[XP_MD5_ENCODE_PARAM_NUM];
	ULONG	cbActualLen[XP_MD5_ENCODE_PARAM_NUM];
	BOOL	fNull[XP_MD5_ENCODE_PARAM_NUM];

	BYTE	szInputStr[32]		= {0,};			// 입력 문자열
	BYTE	szInputIndex[32]	= {0,};			// 인덱스가 되는 입력 문자열
	BYTE	szOutputKeyVal[32]	= {0,};			// 출력 버퍼
	INT		iInputKeyIndex		= 0;			// 입력 키인덱스 (0~255)

    // 입력 파라메터의 개수 세기
    INT nParams = srv_rpcparams(pSrvProc);
    if (nParams != XP_MD5_ENCODE_PARAM_NUM)
	{
		printError (pSrvProc, "usage: exec XP_MD5_CryptKey @string, @strindex, @keyval OUTPUT\nWhere @keyval is a char of at least 16 bytes.");
        return (XP_ERROR);
	}

	// srv_paraminfo 를 사용하여 데이터 타입과 길이정보 등을 얻는다.
	for (INT iPNUM = 0 ; iPNUM < XP_MD5_ENCODE_PARAM_NUM ; iPNUM++) {
		if (srv_paraminfo(pSrvProc, iPNUM+1, &bType[iPNUM], &cbMaxLen[iPNUM], &cbActualLen[iPNUM],
			NULL, &fNull[iPNUM]) == FAIL)
		{
			printError (pSrvProc, "srv_paraminfo failed... : while get data type and length");
 			return (XP_ERROR);    
		}
	}

	// 마지막(3번째) 인자인 @keyval 은 확실히 출력값인지 확인한다.
	if ((srv_paramstatus(pSrvProc, 3) & SRV_PARAMRETURN) == FAIL)
	{
		printError (pSrvProc, "usage: exec XP_MD5_CryptKey @string, @strindex, @keyval OUTPUT\nWhere @keyval is a char of at least 16 bytes.");
        return (XP_ERROR);    
	}
	
    // 마지막(3번째) 인자인 @keyval 의 크기가 데이터를 담기에 충분한지 확인한다.
	if (cbMaxLen[2] < 16)
	{
        printError (pSrvProc, "Output param Max. Length should be at least 16 bytes.");
        return (XP_ERROR);  
	}

	if (fNull[0] == 0)
	{
		// 첫번째 인자의 실제 값을 얻는다.
		if (srv_paraminfo(pSrvProc, 1, &bType[0], &cbMaxLen[0],	&cbActualLen[0], szInputStr, &fNull[0]) == FAIL)
		{
	        printError (pSrvProc, "srv_paraminfo failed... : while retrieve data");
	        return (XP_ERROR);    
		}
	}

	if (fNull[1] == 0)
	{
		// 두번째 인자의 실제 값을 얻는다.
		if (srv_paraminfo(pSrvProc, 2, &bType[1], &cbMaxLen[1],	&cbActualLen[1], szInputIndex, &fNull[1]) == FAIL)
		{
	        printError (pSrvProc, "srv_paraminfo failed... : while retrieve data");
	        return (XP_ERROR);    
		}
	}


	// 여기서 변환을 실시한다.
	MD5 cMD5;

	cMD5.EncodeString( szInputStr, cbActualLen[0], szOutputKeyVal, szInputIndex, cbActualLen[1] );

	if (srv_paramsetoutput(pSrvProc, 3, szOutputKeyVal, 16, FALSE ) == FAIL) {
	    printError (pSrvProc, "srv_paramsetoutput failed...");
	    return (XP_ERROR);    
	}

	return XP_NOERROR;
}


//<=====================================================
//< Name: bool WZ_MD5_EncodeString()
//< Desc: 특정 문자열을 인덱스를 이용하여 MD5로 변환한 후 이것을 다시 문자열로 변환
//< retval: 성공하면 true / 실패하면 false
//<=====================================================
WZ_DECLAREDLL bool XP_MD5_EncodeString( SRV_PROC* pSrvProc )
{
	// XP 중 인자체크에 들어가는 변수들
	BYTE	bType[XP_MD5_ENCODE_PARAM_NUM];
	ULONG	cbMaxLen[XP_MD5_ENCODE_PARAM_NUM];
	ULONG	cbActualLen[XP_MD5_ENCODE_PARAM_NUM];
	BOOL	fNull[XP_MD5_ENCODE_PARAM_NUM];

	BYTE	szInputStr[32]		= {0,};			// 입력 문자열
	BYTE	szInputIndex[32]	= {0,};			// 인덱스가 되는 입력 문자열
	BYTE	szOutputKeyVal[64]	= {0,};			// 출력 버퍼
	INT		iInputKeyIndex		= 0;			// 입력 키인덱스 (0~255)

    // 입력 파라메터의 개수 세기
    INT nParams = srv_rpcparams(pSrvProc);
    if (nParams != XP_MD5_ENCODE_PARAM_NUM)
	{
		printError (pSrvProc, "usage: exec XP_MD5_EncodeString @string, @strindex, @keystring OUTPUT\nWhere @keystring is a char of at least 32 bytes.");
        return (XP_ERROR);
	}

	// srv_paraminfo 를 사용하여 데이터 타입과 길이정보 등을 얻는다.
	for (INT iPNUM = 0 ; iPNUM < XP_MD5_ENCODE_PARAM_NUM ; iPNUM++) {
		if (srv_paraminfo(pSrvProc, iPNUM+1, &bType[iPNUM], &cbMaxLen[iPNUM], &cbActualLen[iPNUM],
			NULL, &fNull[iPNUM]) == FAIL)
		{
			printError (pSrvProc, "srv_paraminfo failed... : while get data type and length");
 			return (XP_ERROR);    
		}
	}

	// 마지막(3번째) 인자인 @keystring 은 확실히 출력값인지 확인한다.
	if ((srv_paramstatus(pSrvProc, 3) & SRV_PARAMRETURN) == FAIL)
	{
		printError (pSrvProc, "usage: exec XP_MD5_EncodeString @string, @strindex, @keystring OUTPUT\nWhere @keystring is a char of at least 32 bytes.");
        return (XP_ERROR);    
	}
	
    // 마지막(3번째) 인자인 @keystring 의 크기가 데이터를 담기에 충분한지 확인한다.
	if (cbMaxLen[2] < 32)
	{
        printError (pSrvProc, "Output param Max. Length should be at least 32 bytes.");
        return (XP_ERROR);    
	}

	if (fNull[0] == 0)
	{
		// 첫번째 인자의 실제 값을 얻는다.
		if (srv_paraminfo(pSrvProc, 1, &bType[0], &cbMaxLen[0],	&cbActualLen[0], szInputStr, &fNull[0]) == FAIL)
		{
	        printError (pSrvProc, "srv_paraminfo failed... : while retrieve data");
	        return (XP_ERROR);    
		}
	}

	if (fNull[1] == 0)
	{
		// 두번째 인자의 실제 값을 얻는다.
		if (srv_paraminfo(pSrvProc, 2, &bType[1], &cbMaxLen[1],	&cbActualLen[1], szInputIndex, &fNull[1]) == FAIL)
		{
	        printError (pSrvProc, "srv_paraminfo failed... : while retrieve data");
	        return (XP_ERROR);    
		}
	}


	// 여기서 변환을 실시한다.
	MD5 cMD5;

	cMD5.EncodeString( szInputStr, cbActualLen[0], szOutputKeyVal, szInputIndex, cbActualLen[1] );

	if (srv_paramsetoutput(pSrvProc, 3, szOutputKeyVal, 32, FALSE ) == FAIL) {
	    printError (pSrvProc, "srv_paramsetoutput failed...");
	    return (XP_ERROR);    
	}

	return XP_NOERROR;
}


//<=====================================================
//< Name: bool WZ_MD5_CheckValue()
//< Desc: 특정 MD5값이 문자열과 인덱스를 넣은 결과와 일치하는지 확인
//< retval: 성공하면 true / 실패하면 false
//<=====================================================
WZ_DECLAREDLL bool XP_MD5_Compare( SRV_PROC* pSrvProc )
{
	// XP 중 인자체크에 들어가는 변수들
	BYTE	bType[XP_MD5_CHECK_PARAM_NUM];
	ULONG	cbMaxLen[XP_MD5_CHECK_PARAM_NUM];
	ULONG	cbActualLen[XP_MD5_CHECK_PARAM_NUM];
	BOOL	fNull[XP_MD5_CHECK_PARAM_NUM];

	BYTE	szInputStr[32]		= {0,};			// 입력 문자열
	BYTE	szInputIndex[32]	= {0,};			// 인덱스가 되는 입력 문자열
	BYTE	szInputKeyVal[64]	= {0,};			// 입력 MD5값
	INT		iInputKeyIndex		= 0;			// 입력 키인덱스 (0~255)
	BYTE	iOutputResult		= 0;			// 출력 결과값 (0:틀리다 / 1:맞다)

    // 입력 파라메터의 개수 세기
    INT nParams = srv_rpcparams(pSrvProc);
    if (nParams != XP_MD5_CHECK_PARAM_NUM)
	{
		printError (pSrvProc, "usage: exec XP_MD5_Compare @string, @keyval, @strindex, @result OUTPUT\nWhere @result is a char of at least 1 bytes.");
        return (XP_ERROR);
	}

	// srv_paraminfo 를 사용하여 데이터 타입과 길이정보 등을 얻는다.
	for (INT iPNUM = 0 ; iPNUM < XP_MD5_CHECK_PARAM_NUM ; iPNUM++) {
		if (srv_paraminfo(pSrvProc, iPNUM+1, &bType[iPNUM], &cbMaxLen[iPNUM], &cbActualLen[iPNUM],
			NULL, &fNull[iPNUM]) == FAIL)
		{
			printError (pSrvProc, "srv_paraminfo failed... : while get data type and length");
 			return (XP_ERROR);    
		}
	}

	// 마지막(4번째) 인자인 @result 는 확실히 출력값인지 확인한다.
	if ((srv_paramstatus(pSrvProc, 4) & SRV_PARAMRETURN) == FAIL)
	{
		printError (pSrvProc, "usage: exec XP_MD5_Compare @string, @keyval, @strindex, @result OUTPUT\nWhere @result is a char of at least 1 bytes.");
        return (XP_ERROR);    
	}
	
    // 마지막(4번째) 인자인 @result 의 크기가 데이터를 담기에 충분한지 확인한다.
	if (cbMaxLen[3] < 1)
	{
        printError (pSrvProc, "Output param Max. Length should be at least 1 bytes.");
        return (XP_ERROR);    
	}

	if (fNull[0] == 0)
	{
		// 첫번째 인자의 실제 값을 얻는다.
		if (srv_paraminfo(pSrvProc, 1, &bType[0], &cbMaxLen[0],	&cbActualLen[0], szInputStr, &fNull[0]) == FAIL)
		{
	        printError (pSrvProc, "srv_paraminfo failed... : while retrieve data");
	        return (XP_ERROR);    
		}
	}

	if (fNull[1] == 0)
	{
		// 두번째 인자의 실제 값을 얻는다.
		if (srv_paraminfo(pSrvProc, 2, &bType[1], &cbMaxLen[1],	&cbActualLen[1], szInputKeyVal, &fNull[1]) == FAIL)
		{
	        printError (pSrvProc, "srv_paraminfo failed... : while retrieve data");
	        return (XP_ERROR);    
		}
	}

	if (fNull[2] == 0)
	{
		// 세번째 인자의 실제 값을 얻는다.
		if (srv_paraminfo(pSrvProc, 3, &bType[2], &cbMaxLen[2],	&cbActualLen[2], szInputIndex, &fNull[2]) == FAIL)
		{
	        printError (pSrvProc, "srv_paraminfo failed... : while retrieve data");
	        return (XP_ERROR);    
		}
	}


	MD5 cMD5;

	iOutputResult = cMD5.Compare( szInputStr, cbActualLen[0], szInputKeyVal, szInputIndex, cbActualLen[2] );
	if (srv_paramsetoutput(pSrvProc, 4, (LPBYTE)&iOutputResult, 1, FALSE ) == FAIL) {
	    printError (pSrvProc, "srv_paramsetoutput failed...");
	    return (XP_ERROR);    
	}

	return XP_NOERROR;
}