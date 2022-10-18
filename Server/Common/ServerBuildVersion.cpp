#include "stdafx.h"
#include "ServerBuildVersion.h"


DWORD
SERVER_VERSION_INFO::BASE_NUMBER()
{
#if defined(__KR) || defined(_KOREA)
    return 1000000;
#elif defined(__JP) || defined(_JAPAN)
    return 2000000;
#elif defined(__US) || defined(_AMERICA)
    return 3000000;
#elif defined(__TW) || defined(_TAIWAN)
    return 4000000;
#elif defined(__CN) || defined(_CHINA)
    return 5000000;
#elif defined(__US) || defined( _USA )
    return 6000000;
#elif defined(_GLOBAL)
    return 7000000;
#elif defined(_RUSSIA)
    return 8000000;
#endif
}

#define BASE_NO     SERVER_VERSION_INFO::BASE_NUMBER()

DWORD
SERVER_VERSION_INFO::RELEASE_BUILD_NUMBER()
{
    DWORD buildNumber = 0;
#if defined(_KOREA)
    buildNumber = 15;
#elif defined(_JAPAN)
    buildNumber = 0;
#elif defined(_AMERICA)
    buildNumber = 0;
#elif defined(_TAIWAN)
    buildNumber = 0;
#elif defined(_CHINA)
    buildNumber = 0;
#elif defined( _USA )
    buildNumber = 0;
#elif defined(_GLOBAL)
    buildNumber = 0;
#elif defined(_RUSSIA)
    buildNumber = 0;
#endif
    return (BASE_NO + NATION_BUILD_VERSION*100 + buildNumber);
}

// Server Build Version History
//------------------------------------------------------------------------------------------------------------------
// 2009-05-22(금) Ver 190302 : 빌드가 제대로 안된 부분 수정 빌드 + Agent Zone관련 문제 수정
// 2009-05-22(금) Ver 190301 : 0903 1st Build
// 2009-05-18(월) Ver 190226 : 인증 서버(서버 선택 기능상 보안/로그 강화 처리)
// 2009-05-07(목) Ver 190225 : 서버 선택 기능 추가, 섹터 크기 수정 및 스킬 표현 거리 수치 수정
// 2009-04-30(목) Ver 190223 : 2nd 타임 퀘스트 관련 문제 추가 수정
// 2009-04-27(월) Ver 190223 : 퀘스트 수락시 서버 다운 문제 수정, 필드 뷰포트 제거, 로그 일부 제거
// 2009-04-20(월) Ver 190222 : 190221 sub 2 (+ 배틀존 수퍼방 정책 변경), 드래곤변신 저장 관련 문제 수정
// 2009-04-20(월) Ver 190221 : 포탈 미션(방장권한 이동), 거래, 드랍안되는 문제 수정, AC실시간 갱신
// 2009-04-14(화) Ver 190220 : 190219 sub:2
// 2009-04-14(화) Ver 190219 : 길드 관련 버그들/기능 수정, 퀘스트 관련 버그/기능 추가
// 2009-04-08(수) Ver 190218 : 한게임/일반 유저 구분 동접 지표 처리, 파티 인원제한 문제, 파티 관련 문제 코드 수정
// 2009-04-06(월) Ver 190217 : 미션 입장 (로비 풀 상태) 버그, 거래 안되는 상황 로그 작업
// 2009-04-06(월) Ver 190216 : 라이브 패치땜시 적용 내역 17, 18로 이전하고 빽! (17+18)
// 2009-04-03(금) Ver 190215 : 파티원 로딩중 저탑 입장 가능 + 특정 영역 광역 스킬이 미 동작, 채집 퀘스트 포기시 DB정보 초기화
// 2009-04-01(수) Ver 190214 : 버그수정 (파티 관련 문제로 서버 연쇄 다운 문제 수정)
// 190211-1, 190211-2 : 맵버전 문제, 몬스터 소환 퀘스트 포기시 몬스터 제거, 마을에서 부활 기능, 필드에서 홀로 '여신은혜'사용시 부활 등등...
// 2009-03-27(금) Ver 190211 : 추가 기능 (로비 입장 조건 추가 + 마을에서 부활)
// 2009-03-26(목) Ver 190210 : 0902 버그 수정
// 2009-03-25(수) Ver 190209 : 0902 채널링 적용전 최종
// 2009-03-19(목) Ver 190208 : 0902 라이브 패치전 최종
// 2009-03-19(목) Ver 190207
// 2009-03-18(수) Ver 190206
// 2009-03-17(화) Ver 190205
// 2009-03-16(월) Ver 190204
// 2009-03-12(목) Ver 190203
// 2009-03-06(금) Ver 190202 : 퀘스트 진행불가 버그
// 2009-03-05(목) Ver 190201 : 0902최초 빌드 => 타임라인 & 채널링 버전
// Server Build Version History
//------------------------------------------------------------------------------------------------------------------

// 2009-03-27(금) Ver 190211 : 추가 기능 (로비 입장 조건 추가 + 마을에서 부활)
// 2009-03-26(목) Ver 190210 : 0902 버그 수정
// 2009-03-25(수) Ver 190209 : 0902 채널링 적용전 최종
// 2009-03-19(목) Ver 190208 : 0902 라이브 패치전 최종
// 2009-03-19(목) Ver 190207
// 2009-03-18(수) Ver 190206
// 2009-03-17(화) Ver 190205
// 2009-03-16(월) Ver 190204
// 2009-03-12(목) Ver 190203
// 2009-03-06(금) Ver 190202 : 퀘스트 진행불가 버그
// 2009-03-05(목) Ver 190201 : 0902최초 빌드 => 타임라인 & 채널링 버전

// 2009-02-25(수) Ver 190118
// 2009-02-19(목) Ver 190117 : 3D엔진 교체
// 2009-02-13(금) Ver 190116
// 2009-02-06(금) Ver 190115
// 2009-02-04(수) Ver 190114
// 2009-01-29(목) Ver 190113 : 엔진최신꺼로 교체
// 2009-01-19(월) Ver 190112
// 2009-01-16(금) Ver 190111
// 2009-01-14(수) Ver 190110 : 버전명 변경 0819 -> 0901
// 2009-01-13(화) Ver 181909
// 2009-01-12(월) Ver 181908
// 2009-01-09(금) Ver 181907
// 2009-01-08(목) Ver 181906
// 2009-01-07(수) Ver 181905
// 2009-01-06(화) Ver 181904
// 2008-12-24(수) Ver 181903
// 2008-12-17(수) Ver 181902

// 2008-12-15(월) Ver 181803
// 2008-12-09(화) Ver 181802
// 2008-12-04(목) Ver 181801 : 0818최초 빌드 => 0818이 중간에 껴 들어감(크리스마스+아이올트메인미션)

// 2008-11-26(수) Ver 181901 : 0819최초 빌드 => 섀도우 적용

// 2008-11-17(월) Ver 181714 : ProcessMissionRecord의 타입BYTE 체크 제거
// 2008-11-11(화) Ver 181713 : 
// 2008-11-10(월) Ver 181712 : 이벤트 수성전 적용
// 2008-10-29(월) Ver 181711 : 
// 2008-10-27(월) Ver 181710 : 
// 2008-10-24(금) Ver 181709 : 
// 2008-10-23(목) Ver 181708 : 
// 2008-10-17(금) Ver 181707 : 사내배포 버전 + 신규월드서버 적용
// 2008-10-14(화) Ver 181706 : 
// 2008-10-09(목) Ver 181705 : 
// 2008-10-07(화) Ver 181704 : 타락한사원 기능개선 + 신규 트리거 추가
// 2008-10-06(월) Ver 181703 : 타락한사원 버그수정
// 2008-10-02(목) Ver 181702 : 타락한사원 기능개선
// 2008-10-01(수) Ver 181701 : 0817 최초 빌드

// 2008-09-18(목) Ver 181609 : 라이브 본섭 수정버전2
// 2008-09-09(화) Ver 181608 : 라이브 본섭 수정버전
// 2008-09-08(월) Ver 181607 : 라이브 본섭 1차 버전
// 2008-09-03(수) Ver 181606 : 
// 2008-09-01(월) Ver 181605 : 
// 2008-08-27(수) Ver 181604 : 사내배포 버전
// 2008-08-25(월) Ver 181603
// 2008-08-19(화) Ver 181602
// 2008-08-13(수) Ver 181601 : 0816 최초 빌드

// 2008-07-14(월) Ver 181511 : 길드버그 및 마스터서버 어뷰징 로직 수정
// 2008-07-04(금) Ver 181510 : 
// 2008-07-01(화) Ver 181509 : 분쟁지역 몬스터 버그
// 2008-06-30(월) Ver 181508 : 내일 라이브 본섭 적용을 앞두고 최종빌드(서버만)
// 2008-06-27(금) Ver 181507
// 2008-06-23(월) Ver 181506 : 내일 라이브 테섭 적용을 앞두고 최종빌드
// 2008-06-19(목) Ver 181505
// 2008-06-17(화) Ver 181504
// 2008-06-11(수) Ver 181503
// 2008-06-02(월) Ver 181502
// 2008-05-30(금) Ver 181501 : 0815 최초 빌드

// 2008-05-26(월) Ver 181415 : 펫 타이머 주기 변경, 럭키 아이템 인증시 로그 추가, NPC거리체크 보강 다시 추가
// 2008-05-23(금) Ver 181415 : 길드 정보를 가져오는 시점과 길드 관계 정보를 가져오는 시점의 미묘한 차이로 길드 관계 정보를 게임서버에서 Select 하지 못하는 현상이 발생할 소지를 방지.
// 2008-05-19(월) Ver 181414 : 세트아이템 마이너스값 지원
// 2008-05-16(금) Ver 181413 : 펫,에테르 관련 로그 추가, 스킬 옵션관련 서버 다운 수정, OBJECT 예외시 적군 판단, 리미티드 제한수치 반올림으로 수정
// 2008-05-14(수) Ver 181412 : 리미티드 제한수치 힘, 체력 버그 수정 , Skill클래스 릴리즈시 캐릭터 포인터 NULL 제거
// 2008-05-08(목) Ver 181411 : 월드서버 네트웍 버그수정, IsFriend 서버다운 수정, 소켓옵션 버그
// 2008-05-07(수) Ver 181410 : 라이브 전 거의 최종 빌드
// 2008-05-06(화) Ver 181409 : 
// 2008-04-28(월) Ver 181408 : 
// 2008-04-24(목) Ver 181407 : 경험치 사이즈 증가
// 2008-04-21(월) Ver 181406 : 기타 버그 수정
// 2008-04-15(화) Ver 181405 : 기타 버그 수정
// 2008-04-11(금) Ver 181404 : 기타 버그 수정
// 2008-04-02(수) Ver 181403 : 기타 버그 수정
// 2008-03-28(금) Ver 181402 : 에테르 충전 기능 추가
// 2008-03-24(월) Ver 181401 : 0814 최초 빌드

// 2008-03-07(금) Ver 181307 : 0.8.1.3 버전의 리소스 07번째 취합후 빌드 : 길드UP기부 버그, PC방 아이템 버그
// 2008-03-06(목) Ver 181306 : 0.8.1.3 버전의 리소스 06번째 취합후 빌드 : BonusMoney, Shop타입변경, 트리거 영역조건 변경
// 2008-03-04(화) Ver 181305 : 0.8.1.3 버전의 리소스 05번째 취합후 빌드 : 인챈트 가중치 공식수정
// 2008-03-03(월) Ver 181304 : 0.8.1.3 버전의 리소스 04번째 취합후 빌드 : 아이템 스텟제한 공식 수정
// 2008-02-26(화) Ver 181303 : 0.8.1.3 버전의 리소스 03번째 취합후 빌드 : PC방 슬롯 추가
// 2008-02-21(목) Ver 181302 : 0.8.1.3 버전의 리소스 02번째 취합후 빌드 : 
// 2008-02-13(수) Ver 181301 : 0.8.1.3 버전의 리소스 01번째 취합후 빌드 : 0813 최초 빌드

// 2008-01-31(목) Ver 181214 : 0.8.1.2 버전의 14번째 빌드 : 버그수정 및 발렌타인데이 이벤트 패치
// 2008-01-24(목) Ver 181213 : 0.8.1.2 버전의 13번째 빌드 : 버그수정
// 2008-01-23(수) Ver 181212 : 0.8.1.2 버전의 12번째 빌드 : 캐쉬 반지아이템 거래되게
// 2008-01-22(화) Ver 181211 : 0.8.1.2 버전의 11번째 빌드 : 설 버프 이벤트
// 2007-12-24(월) Ver 181208 : 0.8.1.2 버전의 08번째 빌드 : Agent Buffer Overflow
// 2007-12-18(화) Ver 181207 : 0.8.1.2 버전의 07번째 빌드 : PC방 옵션 적용 안되는 버그수정, 악세사리 아이템의 랭크옵션이 적용 안되는 버그수정
// 2007-12-17(월) Ver 181206 : 0.8.1.2 버전의 리소스 06번째 취합후 빌드 : 
// 2007-12-14(금) Ver 181205 : 0.8.1.2 버전의 리소스 05번째 취합후 빌드 : 
// 2007-12-12(수) Ver 181204 : 0.8.1.2 버전의 04번째 빌드 : 뷰포트 리팩토링
// 2007-12-11(화) Ver 181203 : 0.8.1.2 버전의 리소스 03번째 취합후 빌드 : 아이템 옵션필드 추가, 럭키 아이템 기간 만료 후에도 파괴되지 않도록 수정
// 2007-12-07(금) Ver 181202 : 0.8.1.2 버전의 리소스 02번째 취합후 빌드 : 뷰포트 버그 수정
// 2007-12-05(수) Ver 181201 : 0.8.1.2 버전의 리소스 01번째 취합후 빌드 : 0812 최초 빌드

// 2007-12-04(화) Ver 181107 : 0.8.1.1 버전의 07번째 빌드 : 뷰포트 버그 수정
// 2007-12-03(월) Ver 181106 : 0.8.1.1 버전의 리소스 06번째 취합후 빌드 : 
// 2007-11-30(금) Ver 181105 : 0.8.1.1 버전의 리소스 05번째 취합후 빌드 : 
// 2007-11-28(수) Ver 181104 : 0.8.1.1 버전의 리소스 04번째 취합후 빌드 : 샵 아이템 128개 초과 관련 수정, 뷰포트옵션 INI로 빼기
// 2007-11-22(목) Ver 181103 : 0.8.1.1 버전의 리소스 03번째 취합후 빌드 : 
// 2007-11-15(화) Ver 181102 : 0.8.1.1 버전의 리소스 02번째 취합후 빌드 : 
// 2007-11-06(화) Ver 11400 : 0.8.1.1 버전의 리소스 01번째 취합후 빌드 : 0811 최초 빌드

// 2007-10-29(월) Ver 11349 : 0.8.1.0 버전의 12번째 빌드 : 보내기 버퍼 풀 상황에서 Network 연결 종료 및 console출력 제거, 메모리 풀 사이즈 조정
// 2007-10-22(월) Ver 11348 : 0.8.1.0 버전의 리소스 11번째 취합후 빌드 : 일부 버그수정
// 2007-10-17(수) Ver 11347 : 0.8.1.0 버전의 리소스 10번째 취합후 빌드 : AgentServer 무한루프 수정
// 2007-10-16(화) Ver 11346 : 0.8.1.0 버전의 리소스 09번째 취합후 빌드 : 일부 버그수정
// 2007-10-11(목) Ver 11345 : 0.8.1.0 버전의 리소스 08번째 취합후 빌드 : 일부 버그수정
// 2007-10-08(월) Ver 11344 : 0.8.1.0 버전의 리소스 07번째 취합후 빌드 : 일부 버그수정
// 2007-10-04(목) Ver 11343 : 0.8.1.0 버전의 리소스 06번째 취합후 빌드 : 일부 버그수정
// 2007-10-01(월) Ver 11342 : 0.8.1.0 버전의 리소스 05번째 취합후 빌드 : 일부 버그수정
// 2007-09-27(목) Ver 11341 : 0.8.1.0 버전의 리소스 04번째 취합후 빌드 : 각종 버그수정, FPE 보안적용, 필드파티경험치 수정
// 2007-09-14(금) Ver 11340 : 0.8.1.0 버전의 리소스 03번째 취합후 빌드 : 20레벨이하 스텟초기화, NetworkLib 수정, Agent Gameserver간에 Brd 패킷량 감소
// 2007-09-10(월) Ver 11339 : 0.8.1.0 버전의 리소스 02번째 취합후 빌드 : chunk_map 적용
// 2007-09-03(월) Ver 11338 : 0.8.1.0 버전의 리소스 01번째 취합후 빌드 : 0810 최초 빌드

// 2007-08-13(월) Ver 11337 : 0.8.0.9 버전 09번째 빌드 : 리소스 취합, Mola 적용, 소스 취합
// 2007-08-09(목) Ver 11336 : 0.8.0.9 버전 08번째 빌드 : 리소스 취합, 소스 취합(저탑 층수 버그 수정, 방이름 필터링 적용), 8월 9일자 3D엔진 적용
// 2007-08-06(월) Ver 11335 : 0.8.0.9 버전 07번째 빌드 : 리소스 취합, MemoryPoolFactory 로그 남기기
// 2007-08-06(월) Ver 11334 : 0.8.0.9 버전 06번째 빌드 : 소스 모두 땡긴 후 리빌드
// 2007-08-03(금) Ver 11333 : 0.8.0.9 버전 05번째 빌드 : 스크립트 Version 체크 대소문자 구분 제거
// 2007-08-02(목) Ver 11332 : 0.8.0.9 버전 04번째 빌드 : 7월16일자 3D엔진 적용
// 2007-07-31(화) Ver 11331 : 0.8.0.9 버전의 리소스 03번째 취합후 빌드 : 리소스취합
// 2007-07-23(월) Ver 11330 : 0.8.0.9 버전의 리소스 02번째 취합후 빌드 : 랭크업 변경된 내용 제외, 튜토리얼 스킵 기능 추가
// 2007-07-19(목) Ver 11329 : 0.8.0.9 버전의 리소스 01번째 취합후 빌드 : 0809 최초 빌드

// 2007-07-18(수) Ver 11328 : 0.8.0.8 버전의 리소스 21번째 취합후 빌드 : Agent NULL 체크 버그 수정
// 2007-07-06(월) Ver 11327 : 0.8.0.8 버전의 리소스 20번째 취합후 빌드 : 맵데이타 및 스크립트, 스텟초기화시 드래곤변신 스킬 버그수정, 칼버스 층 오류 수정
// 2007-07-02(월) Ver 11326 : 0.8.0.8 버전의 리소스 19번째 취합후 빌드 : 맵데이타 및 스크립트, 인벤토리잠금 수정
// 2007-06-29(금) Ver 11325 : 0.8.0.8 버전의 리소스 18번째 취합후 빌드 : 버그수정 및 스크립트
// 2007-06-28(목) Ver 11324 : 0.8.0.8 버전의 리소스 17번째 취합후 빌드 : 서버 취합 최종 빌드. 버그수정. 피로도 리팩토링. 
//																		AC무한보상버그, 포인트랭킹 버그, 경험치표시 버그
// 2007-06-21(목) Ver 11323 : 0.8.0.8 버전의 리소스 16번째 취합후 빌드 : 
// 2007-06-21(목) Ver 11322 : 0.8.0.8 버전의 리소스 15번째 취합후 빌드 : 엔진 FilePacker Lib 수정4
// 2007-06-14(목) Ver 11321 : 0.8.0.8 버전의 리소스 14번째 취합후 빌드 : 엔진 FilePacker Lib 수정3
// 2007-06-08(금) Ver 11320 : 0.8.0.8 버전의 리소스 13번째 취합후 빌드 : 엔진 FilePacker Lib 수정2
// 2007-05-30(수) Ver 11319 : 0.8.0.8 버전의 리소스 12번째 취합후 빌드 : 엔진 FilePacker Lib 수정
// 2007-05-21(월) Ver 11318 : 0.8.0.8 버전의 리소스 11번째 취합후 빌드 : 미션 릴레이 버그 수정
// 2007-05-18(금) Ver 11317 : 0.8.0.8 버전의 리소스 10번째 취합후 빌드 : 포인트 랭킹 GM명령어 수정
// 2007-05-17(목) Ver 11316 : 0.8.0.8 버전의 리소스 9번째 취합후 빌드 : 포인트 랭킹 버그 수정
// 2007-05-16(수) Ver 11315 : 0.8.0.8 버전의 리소스 8번째 취합후 빌드 : 포인트 랭킹 추가
// 2007-05-03(목) Ver 11314 : 0.8.0.8 버전의 리소스 7번째 취합후 빌드 : 하트 비트 오차 조정2
// 2007-04-27(금) Ver 11313 : 0.8.0.8 버전의 리소스 6번째 취합후 빌드 : 다운예외처리 및 하트 비트 오차 조정
// 2007-04-18(화) Ver 11312 : 0.8.0.8 버전의 리소스 5번째 취합후 빌드 : 
// 2007-04-11(수) Ver 11311 : 0.8.0.8 버전의 리소스 4번째 취합후 빌드 : 
// 2007-04-02(월) Ver 11310 : 0.8.0.8 버전의 리소스 3번째 취합후 빌드 : 버그 수정
// 2007-03-26(월) Ver 11309 : 0.8.0.8 버전의 리소스 2번째 취합후 빌드 : solarslot, network, armorkit lib
// 2007-03-19(월) Ver 11308 : 0.8.0.8 버전의 리소스 취합후 정기 빌드 : 저주받은탑 빌드
// 2007-03-14(수) Ver 11208 : 0.8.0.7 버전의 : 중국 OBT 버전 최신 소스 빌드 : 암호화, 비트 스트림, 섹터 딜레이
// 2007-03-06(화) Ver 11207 : 0.8.0.7 버전의 리소스 8번째 취합후 빌드 : 패키지샵 등록 안되는 문제, 기획 스크립트 수정, 어뷰징로그 수정
// 2007-02-22(목) Ver 11206 : 0.8.0.7 버전의 리소스 7번째 취합후 빌드 : 기획스크립트 수정 및 방 타임아웃 증가+슬롯중복적용처리수정
// 2007-02-21(수) Ver 11205 : 0.8.0.7 버전의 리소스 6번째 취합후 빌드 : abuse처리 수정, Master+Agent
// 2007-02-12(월) Ver 11204 : 0.8.0.7 버전의 리소스 5번째 취합후 빌드 : 버그 패치
// 2007-02-08(목) Ver 11203 : 0.8.0.7 버전의 리소스 4번째 취합후 빌드 : 버그 패치
// 2007-02-07(수) Ver 11202 : 0.8.0.7 버전의 리소스 3번째 취합후 빌드 : 버그 패치 및 튜토리얼 미션 버그와 리소스 추가
// 2007-02-05(월) Ver 11201 : 0.8.0.7 버전의 리소스 2번째 취합후 빌드 : 버그 패치 및 Disonnect사유추가로깅, 튜토리얼미션,필드 추가
// 2007-01-31(수) Ver 11200 : 0.8.0.7 버전의 리소스 취합후 정기 빌드 : 패키지/선물/용족미션추가/유료아이템추가
// 2007-01-24(수) Ver 11199 : DBP로그 강화 버전
// 2007-01-22(월) Ver 11198 : Guild서버 연결 부분수정, Master버그수정, 특정 status 버그 수정, Agent버그수정
// 2007-01-15(월) Ver 11197 : 길드서버 통합 frame으로 변경ini수정, 럭키몬스터드랍확률 파서 수정1.1
// 2007-01-11(금) Ver 11196 : 최신소스버전적용
// 2007-01-09(화) Ver 11195 : 최신소스버전적용
// 2007-01-05(금) Ver 11194 : AC스크립트 패치, 엔진(071004A) 업데이트, 랜덤리젠, 맵파서1.1
// 2007-01-03(수) Ver 11193 : 이벤트종료로인한스크립트패치 및 버그 수정
// 2006-12-27(수) Ver 11191 : 캐시아이템 2차 버그 수정
// 2006-12-22(금) Ver 11190 : 캐시아이템 2차 추가(여신의 은혜 등), NPC상점거래시 유저와의 거리 체크
// 2006-12-19(화) Ver 11180 : PC방 전용옵션 적용
// 2006-12-19(화) Ver 11173 : 게임로그 버전관리
// 2006-12-16(토) Ver 11172 : 수리할 아이템이 없을 때 결과코드 전송, 이벤트 초단위를 분단위로 변경
// 2006-12-15(금) Ver 11171 : 기획스크립트 수정, 게임 유료화 관련로그 수정
// 2006-12-14(목) Ver 11170 : [SUN 아이템 상용화 시작버전] 아이템 상용화 버그 수정, 개인상점 리스트 재배포
// 2006-12-13(수) Ver 11162 : 유료화 관련 로그 추가, 핫비트 응답없는 클라이언트 처리
// 2006-12-12(화) Ver 11161 : 아이템 유료화 개발 2차 릴리즈 버전
// 2006-12-09(토) Ver 11160 : 아이템 유료화 개발 1차 릴리즈 시작 버전
// 2006-12-06(수) Ver 11152 : 인증 강제종료 사유처리
// 2006-12-04(월) Ver 11151 : DB변경, 유료화 관련 처리 추가, DBP리팩토링
// 2006-11-29(수) Ver 11150 : 카오퀘스트 추가, 채팅 개선 1차, AC시스템 확장
// 2006-11-28(화) Ver 11144 : C/S 소켓옵션 크래쉬 버그 수정
// 2006-11-23(목) Ver 11143 : 유료화캐릭터이전관련 처리, 버그수정된엔진업데이트
// 2006-11-21(화) Ver 11142 : MasterServer 리펙토링
// 2006-11-17(금) Ver 11141 : 랭크업시 다른 아이템도 랭크업 가능하게 하는 작업 -> define 닫아둠
// 2006-11-14(화) Ver 11140 : 개인상점 난립에 대한 개선
// 2006-11-06(월) Ver 11131 : 최대90레벨처리 및 주민번호 처리
// 2006-10-30(월) Ver 11130 : 신규 미션 추가
// 2006-10-25(수) Ver 11123 : 미션랭킹 수정
// 2006-10-20(금) Ver 11122 : 보스급 몬스터 HP Overflow버그 수정, 유니크아이템 처리
// 2006-10-17(월) Ver 11121 : 럭키몬스터 추가, 미션랭킹 추가 요청사항 처리, 개인상점 구매개수 추가
// 2006-10-12(목) Ver 11120 : 개인상점 수수료 부과(5%)
// 2006-10-10(화) Ver 11111 : 미션랭킹 체크 강화, 300% 경험치 이벤트
// 2006-09-29(금) Ver 11110 : 미션랭킹 정보 전송 최적화
// 2006-09-28(목) Ver 11105 : 피크타임이벤트 버그 수정+랭킹관련게임서버죽는버그수정
// 2006-09-28(목) Ver 11104 : PK안되던 버그 수정
// 2006-09-25(월) Ver 11103 : 피로도 시간 변경(12->9)
// 2006-09-22(금) Ver 11102 : 사망시 내구도 감소에 따른 아이템 삭제 처리, 미션랭킹 버그 수정, 길드UP버그 수정
// 2006-09-19(화) Ver 11101 : 빌드버전 확장(마지막자리는 빌드 후 수정횟수), 3DTerrain 20060914 버전 적용
// 2006-09-14(목) Ver 1110  : 길드성장시스템, 거래버그 수정, 플레이어 내구도 패널티, GM명령어 수정 및 추가, 스킬버그 수정
// 2006-09-08(금) Ver 1109  : SolarAuth적용
// 2006-09-06(수) Ver 1108  : 미션랭킹, 포탈서버
// 2006-09-06(수) Ver 1107  : 몬스터 끝까지 따라오는 버그 수정
// 2006-08-29(화) Ver 1106  : 아이템 가격 계산 수정, GM명령어 아이템 제거 버그 수정
// 2006-08-28(월) Ver 1105  : 경험치 안들어오던 버그 수정, GM명령어 안먹히는 부분 수정
// 2006-08-25(금) Ver 1104  : PK버그 수정, 프로토콜 랜덤성 강화
// 2006-08-23(수) Ver 1103  : 필드 리펙토링, wmo분리작업 완료버전
// 2006-08-21(월) Ver 1102  : DBProxy 이벤트 관련 버그 및 오류 수정
// 2006-08-16(수) Ver 1101  : 더블하임 PC방 이벤트, 친구상세정보 처리
// 2006-08-08(화) Ver 1100  : PK릴리즈, 캐릭터 밸런스 수정 
// 2006-08-04(목) Ver 1099  : 게임로그 수정요청 사항 반영
// 2006-08-01(화) Ver 1098  : 웨이포인트 작업
// 2006-07-26(수) Ver 1097  : 투명 몬스터 버그 수정 버전(클라이언트 버그 패치), 테스트 서버 준비 버전
// 2006-07-25(화) Ver 1096  : 경쟁헌팅 보상강화 및 어뷰징 방지처리, 채널채팅 기능
// 2006-07-21(금) Ver 1095  : 이벤트 아이템 드랍관련 소유권 문제 수정
// 2006-07-19(수) Ver 1094  : 이벤트 관련 DB수정, 트리거 업데이트 로직 원래대로 수정, 경쟁헌팅 수정 작업
// 2006-07-11(화) Ver 1093  : 친구차단 리스트 최대값 버그 수정
// 2006-07-11(화) Ver 1092  : 스킬 다운 버그 수정, 1/100확률로 인첸트 실패하는 버그 수정
// 2006-07-05(수) Ver 1091  : 파티 버그 수정, Battle서버 다운 상황
// 2006-07-04(화) Ver 1090  : 실시간 통계 추가정보 보냄, 몬스터 점프공격 추가, 친구 상세정보 추가
// 2006-06-30(금) Ver 1089  : 아이템 조합 배포, 컨텐츠 경험치 로그 추가
// 2006-06-28(수) Ver 1088  : WOPS 운영툴 버그로 인한 아이템정보 복구 로직 추가
// 2006-06-26(월) Ver 1087  : 3DTerrain 20060622 버전 적용 : *.map과 *.wmo는 쌍으로 넘어와야 함!!
// 2006-06-23(금) Ver 1086  : 퀘스트 완료 최적화, if define 정리작업
// 2006-06-22(목) Ver 1085  : 파티MP창 버그 수정, 스킬 버그 수정, 길드공지 처리
// 2006-06-20(화) Ver 1084  : 경쟁헌팅방이 제대로 안만들어지던 버그 수정
// 2006-06-20(화) Ver 1083  : 길드원 리스트 안보이는 버그 수정
// 2006-06-19(월) Ver 1082  : 파티원 수에 따른 추가비율 수치 변경
// 2006-06-16(금) Ver 1081  : 블럭된 아이템에 대한 게임기능 정지 처리
// 2006-06-13(화) Ver 1080  : 경쟁헌팅 자동매칭 기능, 아이템 조합 기능, 길드 및 친구 레벨업 실시간 통지기능
// 2006-06-08(목) Ver 1079  : 퀘스트 Max값 오류 수정, DB SQL 문법 에러 수정 버전
// 2006-06-07(수) Ver 1078  : 창고 및 잿팟 통계 추가, DB 재연결 처리, 10초 후 종료처리
// 2006-06-05(월) Ver 1077  : 방어력 계산시 셔츠 대신 갑옷을 기준으로 계산하도록 수정, 아이템시리얼에 월드아이디 추가
// 2006-06-04(일) Ver 1076  : 월드서버 길드탈퇴 버그 수정
// 2006-06-01(목) Ver 1075  : 이벤트 아이템 가져오기 버그 수정, 아이템 합치기 버그 수정, 길드채팅 버그 수정
// 2006-05-30(화) Ver 1074  : 스킬 어빌리티 버그로 이전으로 돌림
// 2006-05-29(월) Ver 1073  : 게임로그 운영팀 요청 반영, FSM 렉 걸리는 부분 수정, 캐릭터 생성 금지 리턴값 처리
// 2006-05-28(일) Ver 1072  : 파티원 MAXHP 0으로 나누는 버그 수정
// 2006-05-28(일) Ver 1071  : 수리버그 수정
// 2006-05-28(일) Ver 1070  : GM 레벨업 무한루프 버그 수정
// 2006-05-27(토) Ver 1069  : NPC FSM버그로 부하를 일으킬 수 있는 부분 수정, 미션기록 버그 수정
// 2006-05-27(토) Ver 1068  : 채팅서버 오류 수정
// 2006-05-26(금) Ver 1067  : 개인상점 및 교환시 아이템 복사 버그 수정
// 2006-05-25(목) Ver 1066  : 아이템 시리얼 낭비하는 버그 수정
// 2006-05-25(목) Ver 1065  : 개인 1:1교환 로그 남긴 버전
// 2006-05-25(목) Ver 1064  : 몬스터 소환체 Update처리시 엄청난 시간을 잡아 먹던 문제 수정, 아이템 시리얼 처리 버그 수정
// 2006-05-25(목) Ver 1063  : 몬스터 AI 상당시간동안 루프를 도는 문제 디버그용 로그
// 2006-05-24(수) Ver 1062  : 디버그용 로그
// 2006-05-24(수) Ver 1061  : 퀘스트 파티간 공유처리, 미션랭킹 저장 버그 수정
// 2006-05-24(수) Ver 1060  : 오픈베타 시작 버전
// 2006-05-23(화) Ver 1059  : 레벨별 게임 기능 제한, 아이템 중복시리얼 블럭 처리
// 2006-05-22(월) Ver 1058  : 게임로그 수정
// 2006-05-22(월) Ver 1057  : 아이템 나누기 기능, 내구도 처리
// 2006-05-19(금) Ver 1056  : 내구도 관련 패킷 추가, 파티 경험치 버그 수정, 몬스터 재부활 버그 수정
// 2006-05-18(목) Ver 1055  : 월드서버 버그 수정, Network모듈 sleep(1)처리
// 2006-05-18(목) Ver 1054  : 파티관련 디버깅용 로그남김, AgentServer에 sleep(1)처리
// 2006-05-15(월) Ver 1053  : 경쟁헌팅 시작 위치 다르게 설정, 필드에서 사망시 시작위치 가기, AllocEnemySlot 다운버그 수정
// 2006-05-13(토) Ver 1052  : 몬스터별 리젠시간 다르게 설정
// 2006-05-12(금) Ver 1051  : 리젠 확률 적용
// 2006-05-11(목) Ver 1050  : 서버 안정화 버전
// 2006-05-10(수) Ver 1049  : 아이템 복사 버그(결정체) 수정, 실시간 기능 제한 기능 수정
// 2006-05-10(수) Ver 1048  : 거래 상대방이 없을 시 예외처리
// 2006-05-10(수) Ver 1047  : 스킬 스타일 등록시 버그 수정
// 2006-05-10(수) Ver 1046  : 서버 안정화 처리
// 2006-05-10(수) Ver 1045  : 서버 안정화 처리
// 2006-05-10(수) Ver 1044  : 서버 안정화 처리
// 2006-05-09(화) Ver 1043  : 몬스터 소환 NULL참조 버그 수정(BattleServer), 동접수에서 경쟁헌팅 통계 안보내던 버그 수정
// 2006-05-09(화) Ver 1042  : 게임서버 메모리릭 체크(FieldServer버그)
// 2006-05-09(화) Ver 1041  : GM프로토콜 변경
// 2006-05-08(월) Ver 1040  : [StressTest 시작 버전] 
// 2006-05-08(월) Ver 1039  : 경쟁헌팅 경험치 버그 수정
// 2006-05-08(월) Ver 1038  : 서버 안정화 처리
// 2006-05-08(월) Ver 1037  : 실시간 게임 기능 제한
// 2006-05-08(월) Ver 1036  : 퀘스트 버그 수정
// 2006-05-07(일) Ver 1035  : 마을 시작위치 버그 수정
// 2006-05-07(일) Ver 1034  : 피크타임 이벤트, 경쟁헌팅 추가 기능, GM등급 인증에서 받아옴
// 2006-05-05(금) Ver 1033  : [E3 Version] 미션 보상 경험치 버그 수정, PVP동기화 버그 수정
// 2006-05-04(목) Ver 1032  : 경쟁헌팅 요청사항 처리, 신규 GM명령어 추가 
// 2006-05-03(수) Ver 1031  : 창고 저장이 안되던 버그 수정
// 2006-05-03(수) Ver 1030  : 미션랭킹 관련 DB처리, 점프시 동기화 문제 수정
// 2006-05-02(화) Ver 1029  : 장착창에서 퀵슬롯에 옮기는 버그 수정
// 2006-05-02(화) Ver 1028  : ODBC변경(SUNOnline_0426), 미션버그 수정, 아이템 버그 수정, 길드 버그 수정
// 2006-04-29(토) Ver 1027  : 맵파일에서 트리거 정보는 따로 파일로 뺌
// 2006-04-28(금) Ver 1026  : 미션보상관련 서버 다운 버그 수정
// 2006-04-26(수) Ver 1025  : 클라이언트와 주고받는 기본 패킷(MSG_FORWARD)에서 DWORD값 없앰
// 2006-04-24(월) Ver 1024  : 주기적인 패킷시리얼 변경 기능
// 2006-04-21(금) Ver 1023  : 경쟁헌팅 버그 수정, 매칭DB처리 버전
// 2006-04-19(수) Ver 1022  : AC시스템 1차 완료
// 2006-04-18(화) Ver 1021  : 미션 테스트 완료
// 2006-04-15(토) Ver 1020  : 필드 사망시 마을 이동 버그 수정 버전, 프로토콜 빌드시 마다 변경 기능
// 2006-04-15(토) Ver 1019  : 거래 관련 버그 수정, 퀘스트 관련 버그 수정, 장착장에서 인벤이동 버그 수정버전 
// 2006-04-14(금) Ver 1018  : 방과 필드 경험치 구분처리, ODBC변경(SUNOnline_0412), 경쟁헌팅 예외처리
// 2006-04-12(수) Ver 1017  : 하임 0만 떨어지는 버그 수정, ODBC변경(SUNOnline_0411),몬스터 업데이트 영역 버그 수정
// 2006-04-11(화) Ver 1016  : Ready안되는 버그 수정, 아이템 드랍 안되는 버그 수정, FieldServer FPS 정상화시킨 버전
// 2006-04-10(월) Ver 1015  : 몬스터 아이템/하임 드랍방식 재수정, 드랍스크립트 업데이트, npcinfo업데이트
// 2006-04-07(금) Ver 1014  : 몬스터 아이템/하임 드랍방식 수정, 경쟁헌팅 1차,마을 전투맵 위치 저장, 퀘스트 최적화 버전
// 2006-04-05(수) Ver 1013  : 마을 필드이동 버그 수정, 마을 필드맵 리젠 버그 수정, 바운딩 리젠영역 처리
// 2006-04-04(화) Ver 1012  : 음성채팅, 퀘스트 1차 완료, 파티장 근처로 워프 및 소환, 필드 사냥맵 처리
// 2006-03-31(금) Ver 1011  : 캐릭터 점프 기능, 멀티파티에 따른 경험치 및 아이템 분배, 아이템 소유권 기능
// 2006-03-21(화) Ver 1010  : 아이템 인첸트,조합,디바인 관련 작업, 패킷 암호화 적용, 멀티파티 작업, 이용자 정지 시스템
// 2006-03-09(목) Ver 1009  : 멀티파티 프로토콜 추가, 서버 모듈 라이브러리화 작업, 패킷 암호화 모듈 추가 버전
// 2006-02-21(화) Ver 1008  : 기본 파라미터 변경, 속성 데미지 적용, 서버 옵션 정보 정리 버전
// 2006-02-15(수) Ver 1007  : 아이템 패킷 최적화 버전
// 2006-01-31(화) Ver 1006  : 소환관련 버그 수정 버전
// 2006-01-28(일) Ver 1005  : 12시간 피로도 시스템 적용 버전
// 2006-01-28(토) Ver 1004  : 배틀서버 다운 버그 수정(소환관련 버그로 인한 다운현상)
// 2006-01-28(토) Ver 1003  : 포탈 트리거 작동 버그 수정, 소환체가 주인을 공격하는 버그 수정, 채팅 게임서버 처리로 변경
// 2006-01-27(금) Ver 1002  : 창고 겹쳐서 올릴 경우 아이템 삭제되는 버그 수정, 몬스터 스킬 쿨타임 NULL참조 버그 수정
// 2006-01-27(금) Ver 1001  : 개인상거래 버그 수정, 채팅서버 버그 수정
// 2006-01-26(목) Ver 1000  : [PreOpen] 서버 빌드버전 체크 시작
//------------------------------------------------------------------------------------------------------------------


// Server Build Version History( 해외버전 - 영문 )
//------------------------------------------------------------------------------------------------------------------
// 0.9.0.2_690208_3_3_6_3DEngine090302_20090727 : C/S 버전 동기화 빌드, 북미 인증은 추가 적용 예정
// 0.9.0.2_690206_3_3_6_3DEngine090302_20090707 : 클라이언트 버전 동기화 빌드 + 북미 인증(테스트) 빌드
// 0.9.0.2_690205_3_3_6_3DEngine090302_20090624 : 경험치 공식 국내랑 동일하게...
// 0.9.0.2_690204_3_3_1_3DEngine090302_20090616 : 0902 1st Version
// 0.8.1.5_681503_3_3_1_3DEngine090302_20090527 : SolarAuth적용 버전
// 2008-09-19(금) ver 681502 : 해외 테스트 용. 솔라오스 적용 안함.
// 2008-08-20(수) ver 681501 : 캐릭터 생성 스크립트 적용 안되는 버그 수정.
// 2008-08-14(목) ver 681500 : 영문버전 최초 빌드
//------------------------------------------------------------------------------------------------------------------


// Server Build Version History( 해외버전 - 중국 )
//------------------------------------------------------------------------------------------------------------------
// 2009-11-05(금) ver 590400 : 0904 최초빌드
// 2009-09-25(금) ver 590305 : 아이템 스트림 귀속부분과 reserved영역 초기화
// 2009-09-15(화) ver 590304 : 인증 서버 902 -> 816 중국 비번 카드 문제로 인해 롤백 
// 2009-09-08(화) ver 590303 : 귀속 아이템 드랍 처리
// 2009-08-27(목) ver 590302 : 0903 클라이언트 동기화, 미션 보스방 재진입 버그 수정
// 2009-08-12(수) ver 590301 : 0903 최초 빌드
// 2009-08-04(화) ver 590213 : 중문 케릭명 버그 수정(테스트버전임), 테스트 클라이언트 계정 추가.
// 2009-07-29(수) ver 590212 : 아이템 재구매 로그 추가, gm 아이템 생성 명령어 로그 로직변경.
// 2009-07-23(목) ver 590211 : 퀘스트 요구 아이템 삭제 3개까지만 되는 버그.
// 2009-06-30(월) ver 590210 : 클라이언트 동기화.
// 2009-06-27(월) ver 590209 : 중국 길드창고와 명지휘관 아이템을 이용한 복사 버그 수정.
// 2009-06-22(월) ver 590209 : 피로도 적용 이상 현상 수정.
// 2009-06-20(토) ver 590209 : 미션 포탈, 에소드 포탈 못들어가는 버그 수정. 
// 2009-06-19(금) ver 590208 : 카오 부분만 재수정. 
// 2009-06-19(금) ver 590208 : PK룰 원복, 중어 비스타 문자 특문으로 인식 뺀 부분 원복 ,도트 데미지 관련 수정. 
// 2009-06-19(금) ver 581714 : 중국 52파티션 서버 다운 수정.(DBP)
// 2009-06-17(수) ver 590207 : 0902 버그 수정. 소환수에 의한 공격시 20랩 이하가 프리카오가 되는 현상, 프리카오는 모든 상점이용가능. 등...  
// 2009-06-16(화) ver 581714 : 중국 52파티션 서버 다운 수정.(FIELD)
// 2009-06-08(월) ver 590206 : 0902 마지막 빌드(핵쉴드 변경).
// 2009-06-04(목) ver 590205 : Agent Server 다운 버그 수정.
// 2009-05-30(  ) ver 590204 : 클라 동기화.
// 2009-05-28(목) ver 590203 : 중국 GM 케릭터 친구 추가 못함에서 온라인 정보를 주지 않는걸로 변경.
// 2009-05-25(금) ver 590202 : 중국 채집완료후 보상 패킷 변경./중국 GM 케릭터 친구 추가 못함./중국 쉐도우 입장 불가 맵 추가/중국 길드 창고에 보관 불가능 타입추가./X 버튼 사용 가능 하게 변경./저탑 이동 버그 수정.
// 2009-04-30(금) ver 590201 : 클라빌드에 동기화.
// 2009-04-24(금) ver 590200 : 0902 첫 빌드.
// 2009-04-15(목) ver 581713 : 클라빌드에 동기화
// 2009-04-13(목) ver 581712 : 보스방 재진입 가능 버그 수정.
// 2009-04-10(목) ver 581711 : 아이템 복사 버그 수정
// 2009-04-06(월) ver 581711 : 클라빌드에 맞춰...
// 2009-03-18(수) ver 581710 : 계정 49자 버그 수정
// 2009-03-13(금) ver 581709 : 계정 확장
// 2009-03-02(월) ver 581708 : 클라 버그 수정 같이 빌드함
// 2009-02-27(금) ver 581707 : 거래시 서버 죽는 버그 수정
// 2009-02-23(월) ver 581706 : 인첸트 아이템 레벨 추가, 상태 추가
// 2009-01-22(xx) ver 581705 : 파티버그로 Agent죽는 현상 수정
// 2009-01-14(수) ver 581704 : 칼버스 악세사리 엘리트 아이템 버그 수정
// 2009-01-08(목) ver 581703 : 내구도 개선 내용 삭제
// 2009-01-05(월) ver 581702 : 악세사리 셋트 아이템 옵션 적용
// 2008-12-22(월) ver 581701 : 경험치 공식 0815로 적용함
// 2008-12-08(월) ver 581700 : 0817 첫 빌드
// 2008-12-01(월) ver 581526 : 어뷰즈 수정
// 2008-11-25(화) ver 581525 : 엔진 최신
//				  ver 581524 : 
//				  ver 581523 : 
// 2008-11-17(월) ver 581522 : 
// 2008-11-14(금) ver 581521 : 0815 마이너패치 - 스피릿슬러그 수정
// 2008-11-12(수) ver 581520 : 0815 마이너패치
// 2008-11-07(금) ver 581519 : 개발, 0815 마이너패치
// 2008-11-05(수) ver 581518 : 라이브, 클라소스 덜 받아서 재빌드 (후우...)
// 2008-11-04(화) ver 581517 : 라이브, 만렙버그 수정 재빌드
// 2008-11-03(월) ver 581516 : 엘리트 1.5차, 스피릿슬러그 엠블렘 중복 버그 수정
// 2008-10-27(월) ver 581515 : 0815 15번째 빌드 클라 수정
// 2008-10-24(금) ver 581514 : 0815 14번째 빌드 LIVE 마스터 버그 수정
// 2008-10-24(금) ver 581513 : 0815 13번째 빌드 LIVE 마스터 버그 수정
// 2008-10-22(수) ver 581512 : 0815 12번째 빌드 8번째 빌드와 동일에서 저탑입장 버그, 100레벨 체인지업 시 스텟 초기화, 복수의 돌 버그
// 2008-10-22(수) ver 581512 : 0815 12번째 빌드 8번째 빌드와 동일에서 저탑입장 버그, 100레벨 체인지업 시 스텟 초기화, 복수의 돌 버그
// 2008-10-22(수) ver 581511 : 0815 11번째 빌드 저탑입장 버그, 100레벨 체인지업 시 스텟 초기화, 복수의 돌 버그
// 2008-10-16(목) ver 581510 : 0815 10번째 빌드 8번째 빌드와 동일 (핵쉴드 패치)
// 2008-10-01(수) ver 581509 : 0815 9번째 빌드 마이너 패치 - 1.5차 엘리트, 유료인벤토리 퀵슬롯 가능
// 2008-09-23(화) ver 581508 : 0815 8번째 빌드 vendor 에서 죽는 버그 수정, 판도라 behave 수정
// 2008-09-18(목) ver 581507 : 0815 7번째 빌드 20레벨미만 스킬 공격 안되는 버그, 판도라 버그 수정
// 2008-09-11(목) ver 581506 : 0815 6번째 빌드 아이템 드랍 안하는 버그 , 경험치 1% 더 먹는 버그 수정
// 2008-09-09(화) ver 581505 : 0815 5번째 빌드 길드창고이동 불가 가능한 버그 수정
// 2008-09-05(금) ver 581504 : 0815 4번째 빌드 길드창고이동 불가 가능한 버그 수정
// 2008-09-04(목) ver 481503 : 0815 3번째 빌드 ( 로또작업 ) 펫 버그 수정
// 2008-08-27(수) ver 481502 : 0815 2번째 빌드 ( 버그의 원인은 DB수정 )
// 2008-08-19(화) ver 481501 : 0815 최초빌드
// 2008-09-10(수) Ver 481419 : 아이템 드랍 안하는 버그 , 경험치 1% 더 먹는 버그 수정
// 2008-09-09(화) Ver 481418 : 창고이동 아이템 길드창고 이동 가능한 버그 수정
// 2008-09-05(금) Ver 481417 : 창고이동 아이템 길드창고 이동 가능한 버그 수정
// 2008-08-18(월) ver 481416 : 클라이언트 옵션 수치 표기 오류 수정
// 2008-08-14(목) ver 481415 : 클라와 맞추어 빌드
// 2008-08-11(월) ver 481414 : PK 버그수정(저렙 카오불가 수정, 길드 무관한 유저들 강제공격 수정)
// 2008-08-01(금) ver 481413 : 경계지역에서 법위공격시 Ctrl 키 없이 강제 공격 되던 현상 수정
// 2008-07-24(목) ver 481412 : 보라돌이 공격 재수정 / 프로토콜 변경 / Filed 서버 안정화 / 길드장 튕김시 복귀 불가 수정
// 2008-07-16(화) ver 481411 : 길드 패널티 / 아이템 이펙트 / 보라돌이 공격
// 2008-07-15(화) ver 481410 : 솔라오스 압축 기능 추가 및 전 서버 적용. //x81409 추가기능 제거(리미티드 수치오류)
// 2008-07-14(월) ver x81409 : 길드창고 로그 업데이트 불가 수정, 리이티드 일부아이템 수치 오류수정
// 2008-06-11(수) ver x81408 : 맵버전이 틀려 팅기던 현상
// 2008-06-11(수) ver x81407 : Patcket 압축관련 기능추가, Quest 프리카오도 습득가능하도록 수정, 길드창고 관련 아이템 이동불가 수정
// 2008-06-11(수) ver x81406 : 펫소환중 거래 불가, 길드 권한보기 버그 수정
// 2008-06-11(수) ver x81405 : viewport 성능개선,
// 2008-06-11(수) ver x81404 : GP 권한 미적용 버그 수정, 창고 잠금기능
// 2008-06-11(수) ver x81402 : 0814버전 3차, 길드관련 리절트 코드 추가, 길드 창고 로그 일반 아이템과 리미티드 아이템 구별, 월드 서버 가상 클라이언트 구별패킷
// 2008-05-20(화) ver x81401 : 0814버전 최초빌드
// 2008-05-16(금) ver x81311 : 핵쉴드 297버전 적용. NPC 재귀 버그 수정.
// 2008-04-15(화) ver x81310 : 잘못된 슬롯 헤더파일 제거. 슬롯 버그 수정.
// 2008-04-14(월) ver x81309 : 아이템 슬롯 버그 수정.
// 2008-04-10(목) ver x81308 : 피어스킬 버그 수정.
// 2008-04-08(목) ver x81207 : 0812버전 핵쉴드 적용버전. 핵쉴드 및 타임아웃 관련 수치 조정.
// 2008-04-04(목) ver x81206 : 중국 피로도 마을에서 적용 안되는 버그 수정.( 실제로 중국에서는 솔라오스로 받으므로 상관없음. )
// 2008-04-03(목) ver x81205 : 필드서버 죽는 버그 수정.
// 2008-04-03(목) ver x81305 : 유저 끼임 버그 수정.
// 2008-03-29(토) ver x81204 : 핵실드 제거.
// 2008-03-29(토) ver x81302 : 솔라슬롯 버그 수정.
// 2008-03-28(금) ver x81301 : 0813 최초 빌드.
// 2008-03-12(수) ver x81203 : 핵실드
// 2008-03-12(수) ver x81203 : Field 서버 죽는 원인 파악 로직 추가
// 2008-03-12(수) ver x81203 : 복사 방지 2, 로그파일 이름에 버전추가, 월드서버 친구관련 기능 오버플로 버그 수정, 유료 아이템 팔기 기능 체크 제거
// 2008-02-21(금) ver x81202 : 0812 2차 빌드.
// 2008-02-21(금) ver x81201 : 0812 최초 빌드.
// 2008-01-17(목) ver x81019 : 아이템 복사방지 패치.
// 2008-01-17(목) ver x81003 : 하임소유권, 복사 방지(0809에 적용)
// 2008-01-07(월) ver x81002 : 퀘스트 50개이상 관련 버그 수정,카오유저가 정상적으로 저주의 탑을 이용하도록 변경. 
//------------------------------------------------------------------------------------------------------------------

// Server Build Version History( 해외버전 - 일본 )
//------------------------------------------------------------------------------------------------------------------
// 2009-07-08(수) ver 290204 : 0902 개발 배포(GM Drop관련 추가), 일본 0902 첫번째 배포버전
// 2009-07-07(수) ver 290203 : 서버 선택 추가
// 2009-06-22(월) ver 290202 : 0902 개발 배포 + 포탈 트리거 버그
// 2009-06-19(금) ver 290201 : 0902 1st 빌드 (for ScriptTest)
// 2009-06-15(월) ver 281806 : 버그수정
// 2009-06-09(화) ver 281805 : 재빌드
// 2009-05-21(목) ver 281804 : 인첸트 실패 시 파괴 가능
// 2009-05-12(화) ver 281803 : 레벨차 경험치 적용
// 2009-04-29(수) ver 281802 : 내구도 가격 버그 수정
// 2009-04-21(화) ver 281801 : 수정내용 없음. 클라랑 동기화
// 2009-04-07(화) ver 281800 : 0818 첫 빌드
// 2009-02-18(수) ver 281524 : 여신의 은혜 버그 수정
// 2009-02-12(목) ver 281523 : 인첸트 버그, 물탑 보스 버그 수정
// 2008-12-12(금) ver 281520 : 유저 튕기는 버그수정 및 티켓 로그 추가.( 얻기 및 지우기. )
// 2008-11-19(수) ver 281518 : 네트워크 엔진 빽~!!!
// 2008-11-19(수) ver 281517 : 신규엔진 적용, 115레벨에 에 죽는 버그 수정.
// 2008-10-30(목) ver 281514 : 클라엔진 8/14 다운.
// 2008-10-30(목) ver 281513 : 레벨과 관계없이 방목록 보이도록 수정. 파티 복귀 관련 버그 수정.
// 2008-10-27(수) ver 281512 : 헤르메스 아이템 메세지 버그 수정. 핵쉴드 변경. 새로운 엔진 적용.
// 2008-10-16(수) ver 281510 : 길드 공지 버그 수정.
// 2008-10-07(화) ver 281509 : 점프시 HP 변화 없는 버그 수정.
// 2008-09-23(화) ver 281508 : 복권 버그 및 벤더 널 체크 버그 수정.
// 2008-09-18(목) ver 281507 : 복권 버그 수정 및 펫 옵션 중복 적용.
// 2008-09-12(금) ver 281506 : 복권 버그 수정.
// 2008-09-10(수) ver 281505 : 복권 및 스텟 관련 버그 수정
// 2008-09-05(금) ver 280932 : 몰라 0809버전으로 바꾸고 배포.
// 2008-08-27(화) ver 281502 : 0815 최초 일본 배포 버전.
// 2008-08-26(화) ver 280931 : 저탑 입장 로그 추가.
// 2008-08-26(화) ver 280930 : 0809버전 서버 트리거 문제및 캐릭터 삭제 문제 수정 버전.
// 2008-08-25(월) ver 281502 : 2차 사내 빌드.
// 2008-08-21(목) ver 281501 : 범위스킬버그 수정, PK버그 수정.
// 2008-08-11(월) ver 281500 : 0815최초 빌드.
// 2008-07-14(월) ver 280927 : 특정 지도에서 튕기는 현상
// 2008-07-14(월) ver 280926 : 솔라오스 압축 기능 추가 및 전 서버 적용.
// 2008-07-10(목) ver 280925 : move gm 명령어 수정 및 dbp 메모리 릭 체크 추가.
// 2008-06-30(화) ver 280924 : 메모리 릭.. 수정.
// 2008-06-17(화) ver 280923 : 클라 빌드 동기화.
// 2008-05-26(월) ver 280922 : 아이템옵션 수치 WORD->INT로 변경.
// 2008-05-21(수) ver 280921 : 인첸트 제약 안되는 버그 수정.
// 2008-05-14(수) ver 280920 : NPC.DeadState에서 무한 재귀 발생될 수 있는 부분 수정, Hackshield 297버전
// 2008-04-16(수) ver 280917 : 슬롯 관련 메세지 정확하게 나가게 수정.
// 2008-04-15(화) ver 280916 : 경험치 공식 수정. 버프 아이콘 버그. 퀵슬롯 버그 수정.
// 2008-04-14(월) ver 280915 : 일본 상용화 IBT 패치 버전.
// 2008-04-07(월) ver 280914 : 일본 상용화 IBT 패치 버전.
// 2008-03-20(목) ver 280913 : 인첸트 랭크업 실패 기능 추가.
// 2008-03-19(수) ver 280912 : 일본 Pre Final 버전
// 2008-03-12(수) ver 280911 : PVP 모드 솔라오스모드에서 읽지 못하는 버그 수정. 마우스 이동 안되는 버그 수정.
// 2008-03-11(화) ver 280910 : 일본 OBT 버전 최초 빌드
// 2008-03-07(금) ver 280908 : 길드 UP 상납시 개인 UP소모 안되는 버그 수정
// 2008-02-21(목) ver 280907 : 문자열 처리 2차.
// 2008-02-20(수) ver 280906 : 일본 문자열 처리 버그 수정. 미션 설명창 나오지 않는 버그 수정.
// 2008-02-14(목) ver 280905 : 인증번호 8자리 지원 기능 추가, 쪽지 차단 기능 추가, 경험치 수정.
// 2008-02-11(월) ver 280904 : 30레벨제한, 일본어 버전의 문자체크 
// 2008-02-04(월) ver 280903 : PVP방제목 변경 버그 수정.
// 2008-01-31(목) ver 280902 : NON PK 시스템 기능 추가. 핵쉴드 적용
// 2008-01-07(월) ver 280901 : 방제목 변경기능 추가, 퀘스트 50개이상 받아지던 버그 수정
//------------------------------------------------------------------------------------------------------------------

// Server Build Version History( 해외버전 - 대만 )
//------------------------------------------------------------------------------------------------------------------
// 2009-04-15(수) ver 481708 : 아이템 복사 버그 수정, 보스 재진입 버그 수정
// 2009-04-02(목) ver 481707 : 세트아이템 엘리트 + 칼버스 가능하도록 추가. 타락한 사원 아이템 인챈.
// 2009-03-11(목) ver 481706 : 아이템 복사 버그 수정.
// 2009-03-05(목) ver 481706 : 로직버그 수정.
// 2009-02-27(금) ver 481705 : 엘리트 1.5차.
// 2009-01-15(목) ver 481704 : 클라이언트 동기화.
// 2008-12-17(화) ver 481703 : 국내 경험치 공식 대만에 적용.
// 2008-12-11(화) ver 481702 : 뉴파싱시스템 버그 수정.
// 2008-12-09(화) ver 481423 : 해외 로그 버그 수정.
// 2008-11-27(화) ver 481701 : 0817 최초빌드.
// 2008-08-13(화) ver 481415 : 클라 빌드 동기화.
// 2008-08-13(수) ver 481414 : 파티장이 없는 파티시 인스던전 입장제한에 걸리지 않는 버그.
// 2008-08-13(수) ver 481413 : 길드 해체 안되던 버그 수정.
// 2008-07-16(화) ver 481412 : 길드원 길드 창고이용시, 추방 후에도 창고 이용가능 버그, 길드 창고 열람중 권한 삭제시 출고 가능 버그, 저탑이용시 맵 버전 틀리다는 버그, 프리카오 유저의 퀘스트 수행 불가 버그, 길드 탈퇴 패널티 
// 2008-07-15(화) ver 481411 : 솔라오스 압축 기능 추가 및 전 서버 적용.(클라 빌드 동기화)
// 2008-07-14(월) ver 481411 : 솔라오스 압축 기능 추가 및 전 서버 적용.
// 2008-06-24(화) ver 481410 : 패킷 압축 버전 클라 동기화.
// 2008-06-23(월) ver 481409 : 0814 대만 최종 빌드. (클라 빌드 동기화)
// 2008-06-17(화) ver 481408 : 0814 대만QA빌드.		(클라 빌드 동기화)
// 2008-06-05(목) ver 481407 : 길드 동맹 불가 수정.
// 2008-06-03(화) ver 481406 : 방리스트 모두 보기 대만적용, 세트옵션%표시 수정, 펫 시스템 국내 2차 추가 내용, 길드 관계 설정 권한 위임 불가. exp 배율 조정 적용
// 2008-05-23(금) ver 481405 : 인벤토리 락 기능 사용으로 변경, 상점 되사기 안되는 버그 수정.
// 2008-05-22(목) ver 481405 : 0814 최초빌드
// 2008-05-15(목) ver 481005 : 월드 외치기 레벨 제한 추가.npc 재귀 버그 수정.ac퀘스트 수행 버그.신규핵쉴드 297
// 2008-04-22(화) ver 481005 : 핵쉴드적용버전.
// 2008-02-21(금) ver x81005 : 이벤트 버그 수정.
// 2008-02-21(금) ver x81003 : 리미티트 일부 착용 불가능 하던 버그 수정
// 2008-02-21(금) ver x81003 : 랭킹 관련 버그 수정 적용, 엘리트아이템 세트옵션 적용, 아이템 복사 관련 적용
// 2008-02-14(목) ver x81002 : 버서커 스피릿리차지 버프아이콘 포탈이동시 사라지는 버그 수정
// 2008-02-14(목) ver x81001 : 0810 최초 빌드
//------------------------------------------------------------------------------------------------------------------


// Server Build Version History( 해외버전 - 구버전 )
//------------------------------------------------------------------------------------------------------------------
// 2008-01-07(월) ver x80917 : 랭킹관련 서버가 죽던 버그 수정
// 2008-01-02(화) Ver x80916 : 파티 복귀 기능에서 비정상 종료 유저의 상태가 존 이동 트랜잭션 상태로 유지되는 문제점 수정 / SharedMemoryPool 사용시 delete에 대한 초기화를 Win32 메커니즘에 호환되도록 처리
// 2006-12-27(목) Ver x80915 : 유료화 아이템 경험치 버그( 약간 못 먹는 ) 수정.
// 2006-12-26(수) Ver x80914 : 버서커 무적 버그 수정.(리미티드포스 이용한 버그 )
// 2006-12-21(금) Ver x80913 : Agent및 Battle서버 죽는 버그 수정기능 제외, 저탑에서 포탈로 마을에 돌아가기가 되지 않던 버그 수정, 방 제목 수정
// 2006-12-17(월) Ver x80912 : 네트워크 엔진 버그 수정 / Agent및 Battle서버 죽는 버그 수정 / 귓속말 차단 버그 수정 / 배틀존에서 비정상 종료버그수정
// 2006-12-11(화) Ver x80911 : 필드서버 멈추는 버그 수정(예상) / 게임로그 안남는 버그 수정(스냅샷)
// 2006-12-06(수) Ver x80910 : 해방군의 포고문 버그 2차 수정 / 트리거 버그 수정.
// 2006-12-05(수) Ver x80909 : 해방군의 외침 버그 수정 / 거래중 포탈 버그 수정.
// 2007-12-03(월) Ver x80908 : 저탑 나가기 버그 2차 수정 / 네트워크 라이브러리 수정.
// 2007-11-28(수) Ver x80907 : 저탑 나가기 버그 수정 / 캐쉬아이템 128개 이상 적용되게 수정.
// 2007-11-23(금) Ver x80906 : 미션 / 퀘스트 하임 및 경험치 피로도 적용 안되는 버그 수정.
// 2007-11-21(수) Ver x80905 : 월드서버 죽는 버그 수정 / 맥스레벨에서 메세지 안나가는 버그 수정/ AC및 미션랭킹 피로도 적용.
// 2007-11-19(월) Ver x80904 : 만렙 메세지 나오지 않는 버그 수정 / 방장 바꾸고 미션 릴레이시 방 안만들어 지는 버그 수정.
// 2007-11-16(금) Ver 3 : 0.8.0.9 버전의 3번째 중국 배포 
// 2007-11-15(목) Ver 2 : 0.8.0.9 버전의 2번째 중국 배포 
//------------------------------------------------------------------------------------------------------------------




