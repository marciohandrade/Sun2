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
//< Desc: 특정 문자열을 인덱스를 이용하여 MD5 값으로 변환
//< retval: 성공하면 true / 실패하면 false
//<=====================================================
// 입력된 문자열과 키인덱스 (0~255)를 사용하여 128 bits (16 bytes) 의 MD5 키값 생성
WZ_DECLAREDLL bool XP_MD5_CryptKey( SRV_PROC* pSrvProc );


//<=====================================================
//< Name: bool WZ_MD5_EncodeString()
//< Desc: 특정 문자열을 인덱스를 이용하여 MD5로 변환한 후 이것을 다시 문자열로 변환
//< retval: 성공하면 true / 실패하면 false
//<=====================================================
// 입력된 문자열과 키인덱스 (0~255)를 사용하여 128 x 2 bits (32 bytes) 의 MD5 문자열 키값 생성
WZ_DECLAREDLL bool XP_MD5_EncodeString( SRV_PROC* pSrvProc );


//<=====================================================
//< Name: bool WZ_MD5_CheckValue()
//< Desc: 특정 MD5값이 문자열과 인덱스를 넣은 결과와 일치하는지 확인
//< retval: 성공하면 true / 실패하면 false
//<=====================================================
// 문자열과 MD5 키값과 키인덱스 (0~255) 를 입력받아 키값을 인증함 (true : 맞음 / false : 틀림)
// P.S.> 키값은 문자열(256 bits)이 아닌 128 bits MD5 키여야 한다.
WZ_DECLAREDLL bool XP_MD5_Compare( SRV_PROC* pSrvProc );