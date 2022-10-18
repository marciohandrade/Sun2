#include "XP_MD5_FUNC.H"
#include "WZ_MD5.H"


#define CHECK_LIMIT(C_VALUE, C_MAX) (C_VALUE<0?FALSE:C_VALUE>(C_MAX-1)?FALSE:TRUE)


extern void printError (SRV_PROC *pSrvProc, CHAR* szErrorMsg);
extern void printUsage (SRV_PROC *pSrvProc);
extern void BytesToStr(DWORD cb, void* pv, LPSTR sz);



//<=====================================================
//< Name: bool WZ_MD5_EncodeKeyVal()
//< Desc: Ư�� ���ڿ��� �ε����� �̿��Ͽ� MD5 ������ ��ȯ
//< retval: �����ϸ� true / �����ϸ� false
//<=====================================================
WZ_DECLAREDLL bool XP_MD5_CryptKey( SRV_PROC* pSrvProc )
{
	// XP �� ����üũ�� ���� ������
	BYTE	bType[XP_MD5_ENCODE_PARAM_NUM];
	ULONG	cbMaxLen[XP_MD5_ENCODE_PARAM_NUM];
	ULONG	cbActualLen[XP_MD5_ENCODE_PARAM_NUM];
	BOOL	fNull[XP_MD5_ENCODE_PARAM_NUM];

	BYTE	szInputStr[32]		= {0,};			// �Է� ���ڿ�
	BYTE	szInputIndex[32]	= {0,};			// �ε����� �Ǵ� �Է� ���ڿ�
	BYTE	szOutputKeyVal[32]	= {0,};			// ��� ����
	INT		iInputKeyIndex		= 0;			// �Է� Ű�ε��� (0~255)

    // �Է� �Ķ������ ���� ����
    INT nParams = srv_rpcparams(pSrvProc);
    if (nParams != XP_MD5_ENCODE_PARAM_NUM)
	{
		printError (pSrvProc, "usage: exec XP_MD5_CryptKey @string, @strindex, @keyval OUTPUT\nWhere @keyval is a char of at least 16 bytes.");
        return (XP_ERROR);
	}

	// srv_paraminfo �� ����Ͽ� ������ Ÿ�԰� �������� ���� ��´�.
	for (INT iPNUM = 0 ; iPNUM < XP_MD5_ENCODE_PARAM_NUM ; iPNUM++) {
		if (srv_paraminfo(pSrvProc, iPNUM+1, &bType[iPNUM], &cbMaxLen[iPNUM], &cbActualLen[iPNUM],
			NULL, &fNull[iPNUM]) == FAIL)
		{
			printError (pSrvProc, "srv_paraminfo failed... : while get data type and length");
 			return (XP_ERROR);    
		}
	}

	// ������(3��°) ������ @keyval �� Ȯ���� ��°����� Ȯ���Ѵ�.
	if ((srv_paramstatus(pSrvProc, 3) & SRV_PARAMRETURN) == FAIL)
	{
		printError (pSrvProc, "usage: exec XP_MD5_CryptKey @string, @strindex, @keyval OUTPUT\nWhere @keyval is a char of at least 16 bytes.");
        return (XP_ERROR);    
	}
	
    // ������(3��°) ������ @keyval �� ũ�Ⱑ �����͸� ��⿡ ������� Ȯ���Ѵ�.
	if (cbMaxLen[2] < 16)
	{
        printError (pSrvProc, "Output param Max. Length should be at least 16 bytes.");
        return (XP_ERROR);  
	}

	if (fNull[0] == 0)
	{
		// ù��° ������ ���� ���� ��´�.
		if (srv_paraminfo(pSrvProc, 1, &bType[0], &cbMaxLen[0],	&cbActualLen[0], szInputStr, &fNull[0]) == FAIL)
		{
	        printError (pSrvProc, "srv_paraminfo failed... : while retrieve data");
	        return (XP_ERROR);    
		}
	}

	if (fNull[1] == 0)
	{
		// �ι�° ������ ���� ���� ��´�.
		if (srv_paraminfo(pSrvProc, 2, &bType[1], &cbMaxLen[1],	&cbActualLen[1], szInputIndex, &fNull[1]) == FAIL)
		{
	        printError (pSrvProc, "srv_paraminfo failed... : while retrieve data");
	        return (XP_ERROR);    
		}
	}


	// ���⼭ ��ȯ�� �ǽ��Ѵ�.
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
//< Desc: Ư�� ���ڿ��� �ε����� �̿��Ͽ� MD5�� ��ȯ�� �� �̰��� �ٽ� ���ڿ��� ��ȯ
//< retval: �����ϸ� true / �����ϸ� false
//<=====================================================
WZ_DECLAREDLL bool XP_MD5_EncodeString( SRV_PROC* pSrvProc )
{
	// XP �� ����üũ�� ���� ������
	BYTE	bType[XP_MD5_ENCODE_PARAM_NUM];
	ULONG	cbMaxLen[XP_MD5_ENCODE_PARAM_NUM];
	ULONG	cbActualLen[XP_MD5_ENCODE_PARAM_NUM];
	BOOL	fNull[XP_MD5_ENCODE_PARAM_NUM];

	BYTE	szInputStr[32]		= {0,};			// �Է� ���ڿ�
	BYTE	szInputIndex[32]	= {0,};			// �ε����� �Ǵ� �Է� ���ڿ�
	BYTE	szOutputKeyVal[64]	= {0,};			// ��� ����
	INT		iInputKeyIndex		= 0;			// �Է� Ű�ε��� (0~255)

    // �Է� �Ķ������ ���� ����
    INT nParams = srv_rpcparams(pSrvProc);
    if (nParams != XP_MD5_ENCODE_PARAM_NUM)
	{
		printError (pSrvProc, "usage: exec XP_MD5_EncodeString @string, @strindex, @keystring OUTPUT\nWhere @keystring is a char of at least 32 bytes.");
        return (XP_ERROR);
	}

	// srv_paraminfo �� ����Ͽ� ������ Ÿ�԰� �������� ���� ��´�.
	for (INT iPNUM = 0 ; iPNUM < XP_MD5_ENCODE_PARAM_NUM ; iPNUM++) {
		if (srv_paraminfo(pSrvProc, iPNUM+1, &bType[iPNUM], &cbMaxLen[iPNUM], &cbActualLen[iPNUM],
			NULL, &fNull[iPNUM]) == FAIL)
		{
			printError (pSrvProc, "srv_paraminfo failed... : while get data type and length");
 			return (XP_ERROR);    
		}
	}

	// ������(3��°) ������ @keystring �� Ȯ���� ��°����� Ȯ���Ѵ�.
	if ((srv_paramstatus(pSrvProc, 3) & SRV_PARAMRETURN) == FAIL)
	{
		printError (pSrvProc, "usage: exec XP_MD5_EncodeString @string, @strindex, @keystring OUTPUT\nWhere @keystring is a char of at least 32 bytes.");
        return (XP_ERROR);    
	}
	
    // ������(3��°) ������ @keystring �� ũ�Ⱑ �����͸� ��⿡ ������� Ȯ���Ѵ�.
	if (cbMaxLen[2] < 32)
	{
        printError (pSrvProc, "Output param Max. Length should be at least 32 bytes.");
        return (XP_ERROR);    
	}

	if (fNull[0] == 0)
	{
		// ù��° ������ ���� ���� ��´�.
		if (srv_paraminfo(pSrvProc, 1, &bType[0], &cbMaxLen[0],	&cbActualLen[0], szInputStr, &fNull[0]) == FAIL)
		{
	        printError (pSrvProc, "srv_paraminfo failed... : while retrieve data");
	        return (XP_ERROR);    
		}
	}

	if (fNull[1] == 0)
	{
		// �ι�° ������ ���� ���� ��´�.
		if (srv_paraminfo(pSrvProc, 2, &bType[1], &cbMaxLen[1],	&cbActualLen[1], szInputIndex, &fNull[1]) == FAIL)
		{
	        printError (pSrvProc, "srv_paraminfo failed... : while retrieve data");
	        return (XP_ERROR);    
		}
	}


	// ���⼭ ��ȯ�� �ǽ��Ѵ�.
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
//< Desc: Ư�� MD5���� ���ڿ��� �ε����� ���� ����� ��ġ�ϴ��� Ȯ��
//< retval: �����ϸ� true / �����ϸ� false
//<=====================================================
WZ_DECLAREDLL bool XP_MD5_Compare( SRV_PROC* pSrvProc )
{
	// XP �� ����üũ�� ���� ������
	BYTE	bType[XP_MD5_CHECK_PARAM_NUM];
	ULONG	cbMaxLen[XP_MD5_CHECK_PARAM_NUM];
	ULONG	cbActualLen[XP_MD5_CHECK_PARAM_NUM];
	BOOL	fNull[XP_MD5_CHECK_PARAM_NUM];

	BYTE	szInputStr[32]		= {0,};			// �Է� ���ڿ�
	BYTE	szInputIndex[32]	= {0,};			// �ε����� �Ǵ� �Է� ���ڿ�
	BYTE	szInputKeyVal[64]	= {0,};			// �Է� MD5��
	INT		iInputKeyIndex		= 0;			// �Է� Ű�ε��� (0~255)
	BYTE	iOutputResult		= 0;			// ��� ����� (0:Ʋ���� / 1:�´�)

    // �Է� �Ķ������ ���� ����
    INT nParams = srv_rpcparams(pSrvProc);
    if (nParams != XP_MD5_CHECK_PARAM_NUM)
	{
		printError (pSrvProc, "usage: exec XP_MD5_Compare @string, @keyval, @strindex, @result OUTPUT\nWhere @result is a char of at least 1 bytes.");
        return (XP_ERROR);
	}

	// srv_paraminfo �� ����Ͽ� ������ Ÿ�԰� �������� ���� ��´�.
	for (INT iPNUM = 0 ; iPNUM < XP_MD5_CHECK_PARAM_NUM ; iPNUM++) {
		if (srv_paraminfo(pSrvProc, iPNUM+1, &bType[iPNUM], &cbMaxLen[iPNUM], &cbActualLen[iPNUM],
			NULL, &fNull[iPNUM]) == FAIL)
		{
			printError (pSrvProc, "srv_paraminfo failed... : while get data type and length");
 			return (XP_ERROR);    
		}
	}

	// ������(4��°) ������ @result �� Ȯ���� ��°����� Ȯ���Ѵ�.
	if ((srv_paramstatus(pSrvProc, 4) & SRV_PARAMRETURN) == FAIL)
	{
		printError (pSrvProc, "usage: exec XP_MD5_Compare @string, @keyval, @strindex, @result OUTPUT\nWhere @result is a char of at least 1 bytes.");
        return (XP_ERROR);    
	}
	
    // ������(4��°) ������ @result �� ũ�Ⱑ �����͸� ��⿡ ������� Ȯ���Ѵ�.
	if (cbMaxLen[3] < 1)
	{
        printError (pSrvProc, "Output param Max. Length should be at least 1 bytes.");
        return (XP_ERROR);    
	}

	if (fNull[0] == 0)
	{
		// ù��° ������ ���� ���� ��´�.
		if (srv_paraminfo(pSrvProc, 1, &bType[0], &cbMaxLen[0],	&cbActualLen[0], szInputStr, &fNull[0]) == FAIL)
		{
	        printError (pSrvProc, "srv_paraminfo failed... : while retrieve data");
	        return (XP_ERROR);    
		}
	}

	if (fNull[1] == 0)
	{
		// �ι�° ������ ���� ���� ��´�.
		if (srv_paraminfo(pSrvProc, 2, &bType[1], &cbMaxLen[1],	&cbActualLen[1], szInputKeyVal, &fNull[1]) == FAIL)
		{
	        printError (pSrvProc, "srv_paraminfo failed... : while retrieve data");
	        return (XP_ERROR);    
		}
	}

	if (fNull[2] == 0)
	{
		// ����° ������ ���� ���� ��´�.
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