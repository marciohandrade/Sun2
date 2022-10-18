#pragma once

#define WZ_DECLAREDLL			extern "C" _declspec(dllexport)


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <srv.h>
#include <time.h>



#define XP_NOERROR						0
#define XP_ERROR						1

#define MAX_SERVER_ERROR				20000
#define XP_PARAM_ERROR					MAX_SERVER_ERROR + 1

#define XP_MD5_ENCODE_PARAM_NUM			3
#define XP_MD5_CHECK_PARAM_NUM			4



//<=====================================================
//< Name: bool XP_MD5_CryptKey()
//< Desc: Ư�� ���ڿ��� �ε����� �̿��Ͽ� MD5 ������ ��ȯ
//< retval: �����ϸ� true / �����ϸ� false
//<=====================================================
// �Էµ� ���ڿ��� Ű�ε��� (0~255)�� ����Ͽ� 128 bits (16 bytes) �� MD5 Ű�� ����
WZ_DECLAREDLL bool XP_MD5_CryptKey( SRV_PROC* pSrvProc );


//<=====================================================
//< Name: bool WZ_MD5_EncodeString()
//< Desc: Ư�� ���ڿ��� �ε����� �̿��Ͽ� MD5�� ��ȯ�� �� �̰��� �ٽ� ���ڿ��� ��ȯ
//< retval: �����ϸ� true / �����ϸ� false
//<=====================================================
// �Էµ� ���ڿ��� Ű�ε��� (0~255)�� ����Ͽ� 128 x 2 bits (32 bytes) �� MD5 ���ڿ� Ű�� ����
WZ_DECLAREDLL bool XP_MD5_EncodeString( SRV_PROC* pSrvProc );


//<=====================================================
//< Name: bool WZ_MD5_CheckValue()
//< Desc: Ư�� MD5���� ���ڿ��� �ε����� ���� ����� ��ġ�ϴ��� Ȯ��
//< retval: �����ϸ� true / �����ϸ� false
//<=====================================================
// ���ڿ��� MD5 Ű���� Ű�ε��� (0~255) �� �Է¹޾� Ű���� ������ (true : ���� / false : Ʋ��)
// P.S.> Ű���� ���ڿ�(256 bits)�� �ƴ� 128 bits MD5 Ű���� �Ѵ�.
WZ_DECLAREDLL bool XP_MD5_Compare( SRV_PROC* pSrvProc );