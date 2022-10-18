
#pragma once

#include "QuestConditionInfo.h"
#include <ResultCode.h>
#include "Quest.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_REPEAT )
	RC::eQUEST_RESULT IsSatisfy( QUESTCOUNT CurRepeatNum );
	__PROPERTY( QUESTCOUNT, MaxRepeatNum )				//< 최대 퀘스트 반복 플레이 할 수 있는 횟수
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_CHARLV )
	RC::eQUEST_RESULT IsSatisfy( WORD CurCharLV );
	__PROPERTY( WORD, MinCharLV )
	__PROPERTY( WORD, MaxCharLV )
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_CHARCLASS )
	RC::eQUEST_RESULT IsSatisfy( BYTE byCharClass );
	__PROPERTY( BYTE, CharClassBit )
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_QUEST )
	enum { _DEF_CHECK_NUM=3, _MAX_CHECK_NUM=5, };
	__PROPERTY( BYTE, QuestArrayCount )			//< 총 3개, 5개 : 최적화를 위한것
	__ARRAY_PROPERTY( QCODE, _MAX_CHECK_NUM, QuestCode )
	__ARRAY_PROPERTY( QCODE, _MAX_CHECK_NUM, QuestNum )		//< 완료 퀘스트만 사용
	__PROPERTY( BYTE, Operator )				//< eQUEST_OPERATOR참조 => 0 : AND , 1 : OR
	__BOOL_PROPERTY( ProgrQuest )				//< 퀘스트가 진행중인가 or 완료된건가
	__BOOL_PROPERTY( NecessaryQuest )			//< 이 배열의 퀘스트가 있어야하는가(TRUE) or 없어야 하는가(FALSE)
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_ITEM )
	enum { _DEF_CHECK_NUM=3, _MAX_CHECK_NUM=5, };
	__BOOL_PROPERTY( NecessaryItem )			//< 이 아이템이 있어야 한다(TRUE) or 없어야 한다(FALSE)
	__PROPERTY( BYTE, ItemArrayCount )			//< 총 3개, 5개 : 최적화를 위한것
	__ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, ItemCode )
	__ARRAY_PROPERTY( BYTE, _MAX_CHECK_NUM, ItemNum )
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_CLASSITEM )
	__PROPERTY( BYTE, CharClassLimit )			//< 제한된 클래스의 아이템을 가지면 안됨
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_CHAO )
	RC::eQUEST_RESULT IsSatisfy( BYTE byChaoState );
	__PROPERTY( BYTE, ChaoState )				//< 카오 상태( 일반, 프리카오, 카오 )
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION_INFO(QUEST_CONDITION_CREATE_PORTAL)
    __PROPERTY(MAPCODE, MapPortalCode)      //< 해당 맵으로 가기 위한 포탈 생성 검사
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION_INFO(QUEST_CONDITION_HONOR_RATING)
__PROPERTY(BYTE, HonorType)
__PROPERTY(WORD, HonorCode)
__PROPERTY(BYTE, HonorRating)
END_DECL_QCONDITION_INFO

BEGIN_DECL_QCONDITION_INFO(QUEST_CONDITION_DAILY_QUEST)
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 완료 조건에만 있음
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_MONEY )
	RC::eQUEST_RESULT IsSatisfy( DWORD64 CurMoney );
	__PROPERTY( DWORD64, NeedMoney )
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 완료 조건에만 있음
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_KILLMONSTER )
	enum { _MAX_CHECK_NUM=5, };
	__ARRAY_PROPERTY( DWORD, _MAX_CHECK_NUM, MonsterCode )
    __ARRAY_PROPERTY( BYTE, _MAX_CHECK_NUM, MonsterGrade )
    __ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, MonsterLevel )
	__ARRAY_PROPERTY( BYTE, _MAX_CHECK_NUM, MonsterNum )
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_MISSION )
	enum { _MAX_CHECK_NUM=2, };
	__ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, MissionCode )
	__ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, MissionNum )
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 완료 조건에만 있음
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_AREA )
    enum { _MAX_CHECK_NUM=3, };
    __ARRAY_PROPERTY( DWORD, _MAX_CHECK_NUM, AreaID )
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 20060711_PM12:19:10
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_ITEM_SPACE )
	enum { _MAX_CHECK_NUM=5, };
	__PROPERTY( BYTE, ItemArrayCount )					//< 총 3개, 5개 : 최적화를 위한것
	__ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, ItemCode )
	__ARRAY_PROPERTY( BYTE, _MAX_CHECK_NUM, ItemNum )
END_DECL_QCONDITION_INFO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_MISSION_CLEAR)
    enum { _MAX_CHECK_NUM=2, };
    __ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, MissionCode )
    __ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, MissionNum )
END_DECL_QCONDITION_INFO
#endif //_NA_006607_20130402_ADD_QUESTTYPE
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#ifdef _DEVTAIYO_QUEST_TRIGGER_WORK

// 완료 조건에만 있음
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_TRIGGER )
	enum { _MAX_CHECK_NUM=4, };
	__ARRAY_PROPERTY( DWORD, _MAX_CHECK_NUM, TriggerNCode )
END_DECL_QCONDITION_INFO
#endif 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
