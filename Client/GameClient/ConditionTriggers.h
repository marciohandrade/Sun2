#pragma once
//=============================================================================================================================
/// 컨디션 트리거들의 구현 클래스
/**
@author	Kim Min Wook < taiyo@webzen.com >
@since	2004. 12. 29
@remark
- 
@note
- 
@history 
- 
*/
//=============================================================================================================================
#include <Define.h>

#include "..\\..\\SolarTrigger\\ConditionInfo.h"
#include "IConditionTriggerEx.h"

__BEGIN_DECL_CONDITION(CONDITION_ALWAYS)
	virtual VOID OnInit();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION(CONDITION_COMPARE_SWITCH)
	virtual VOID OnInit();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION(CONDITION_CLICK_OBJECT)
	virtual VOID OnInit();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION(CONDITION_DAMAGE_OBJECT)
	virtual VOID OnInit();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION(CONDITION_COUNT_NPCKILL)
	virtual VOID OnInit();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION(CONDITION_PASS_TIME)
	virtual VOID OnInit();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION(CONDITION_ENTER_AREA)
	virtual VOID OnInit();
	virtual VOID onMsg(TRIGGER_MSG* pmsg);
	BOOL		CheckHeroInArea(DWORD areaID);
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION(CONDITION_HAVE_ITEM)
	virtual VOID OnInit();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION(CONDITION_MULTI_SWITCH)
	virtual VOID OnInit();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION(CONDITION_MISSION_POINT)
	virtual VOID OnInit();
__END_DECL_CONDITION

__BEGIN_DECL_CONDITION(CONDITION_COMPARE_VARIABLE)
	virtual VOID OnInit();
__END_DECL_CONDITION

	// Quest 완료 체크 트리거, 있는것인지 확인할 것
__BEGIN_DECL_CONDITION(CONDITION_QUEST_PROGRESS)
	virtual VOID OnInit();
__END_DECL_CONDITION

	// 유저 클래스 설정(의존 트리거)
	// [USER CLASS ID] 이면, //0:모두, 1:버서커, 2:드래곤, 3:쉐도우, 4:발키리, 5:엘리멘탈리스트
__BEGIN_DECL_CONDITION(CONDITION_CHECK_CLASS_ID)
	virtual VOID OnInit();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION(CONDITION_NPCKILL_FOR_UNITID)
	virtual VOID OnInit();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION(CONDITION_LIVE_PARTY_MEMBER)
    virtual VOID OnInit();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION(CONDITION_CHECK_OBJECT_HP)
virtual VOID OnInit();
__END_DECL_CONDITION

__BEGIN_DECL_CONDITION(CONDITION_DESTROY_OBJECT)
virtual VOID OnInit();
__END_DECL_CONDITION
 
__BEGIN_DECL_CONDITION(CONDITION_CHECK_OBJECTKIND_HP)
virtual VOID OnInit();
__END_DECL_CONDITION

__BEGIN_DECL_CONDITION(CONDITION_NPC_ENTER_AREA)
virtual VOID OnInit();
__END_DECL_CONDITION

//_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
__BEGIN_DECL_CONDITION(CONDITION_COMPLETE_COLLECTION)
virtual VOID OnInit();
__END_DECL_CONDITION

__BEGIN_DECL_CONDITION(CONDITION_SERVER_EVENT)
virtual VOID OnInit();
__END_DECL_CONDITION

__BEGIN_DECL_CONDITION(CONDITION_CHECK_TEAM)
virtual VOID OnInit();
__END_DECL_CONDITION

__BEGIN_DECL_CONDITION(CONDITION_COMPARE_SWITCH_EX)
virtual VOID OnInit();
__END_DECL_CONDITION

__BEGIN_DECL_CONDITION(CONDITION_MONSTER_ENTER_AREA)
virtual VOID OnInit();
__END_DECL_CONDITION

