/**
*	@file		ClientVer.h
*	@brief		client version header
*  @author		Unknown
*	@version	Unknown
*/

#pragma once

/////////////////////////////////////////////////////////////
// 버전 관련 규칙
// 1. 메이저버전횟수_빌드날짜 (ex. 081701_1001  :0817버전의 1번째 빌드_빌드한 날은 10월 1일)
//	0817			// 메이저버전
//	   + 01			// 빌드 횟수
//	       _1001	// 빌드한 날짜
//
// 2. 실행파일 배포시에는 "무조건" "빌드 횟수"를 한 단계씩 올린다.
//	  (Ex. 081701_1001   => 081702_1001 )
//
// 3. BuildSystem을 이용하여 빌드를 하는 경우엔 버전이 자동으로 올라간다. 단,메이저 업데이트시 최초 한번은 빌드버전을 메이저버전+00으로 세팅할 것
//    (Ex. 0905메이저 첫빌드의 버전 예시-> 090500_0000 로 저장후 Commit. 이후, BuildSystem을 통해 버전 생성을 하면 090501_빌드날짜가 자동으로 세팅됨 )
/////////////////////////////////////////////////////////////

//client version info

#ifdef _CHINA
const char	c_tcBUILD_VERSION[] = "cn110201_0524";
#elif defined _JAPAN
const char	c_tcBUILD_VERSION[] = "jp120200_0000";
#elif defined _GLOBAL
const char	c_tcBUILD_VERSION[] = "gsp090402_0322";
#elif defined (_RUSSIA)
const char	c_tcBUILD_VERSION[] = "ru100402_0106";
#else // _KOREA
const char	c_tcBUILD_VERSION[] = "160115_0419";
#endif




