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


__BEGIN_DECL_CONDITION( CONDITION_ALWAYS )
	virtual VOID OnInit();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_COMPARE_SWITCH )
	virtual BOOL IsSatisfied();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_COMPARE_SWITCH_EX )
	virtual BOOL IsSatisfied();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_CLICK_OBJECT )
	virtual VOID onMsg( TRIGGER_MSG * pmsg );
	virtual VOID OnReset();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_DAMAGE_OBJECT )
	virtual VOID onMsg( TRIGGER_MSG * pmsg );
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_COUNT_NPCKILL )
	virtual VOID OnInstanciate();
	virtual VOID OnInit();
	virtual VOID onMsg( TRIGGER_MSG * pmsg );
private:
	struct PARTY_MEMBER {
		DWORD	m_dwPlayerKey;
		int		m_iKillCount;
	}
	m_PartyMember[1];
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_PASS_TIME )
	virtual VOID OnInit();
	virtual BOOL IsSatisfied();
	virtual BOOL NeedUpdate() { return TRUE; }
    //_NA_008069_20150210_ADD_SPA_SYSTEM
    virtual VOID OnReset();
private:
	DWORD	m_dwStartTick;
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_ENTER_AREA )
	virtual VOID OnInit();
	virtual BOOL IsSatisfied();
	virtual VOID onMsg( TRIGGER_MSG * pmsg );
	virtual VOID OnReset();
    TriggerAreaLocalStorage local_storage_;
__END_DECL_CONDITION

// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
// NPC가 특정 영역에 들어갔을 때
__BEGIN_DECL_CONDITION(CONDITION_NPC_ENTER_AREA)
    virtual void OnInit();
	virtual BOOL IsSatisfied();
    virtual void OnReset();    
    virtual void onMsg(TRIGGER_MSG* trigger_msg);
    TriggerAreaLocalStorageForNpc local_storage_;
__END_DECL_CONDITION

__BEGIN_DECL_CONDITION( CONDITION_HAVE_ITEM )
	static const DWORD c_dwDelayCheckTime = 100;
	virtual VOID OnInit();
	virtual BOOL IsSatisfied();
    //_NA_008069_20150210_ADD_SPA_SYSTEM
    virtual VOID OnReset();
private:
	DWORD	m_dwCheckTimer;
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_MULTI_SWITCH )
	virtual BOOL IsSatisfied();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_MISSION_POINT )
	virtual VOID OnInit();
	virtual VOID onMsg( TRIGGER_MSG * pmsg );
private:
	//INT	m_iNumberOfPlayersInArea;
__END_DECL_CONDITION

__BEGIN_DECL_CONDITION( CONDITION_COMPARE_VARIABLE )
	virtual BOOL IsSatisfied();
__END_DECL_CONDITION

// Quest 완료 체크 트리거, 있는것인지 확인할 것
__BEGIN_DECL_CONDITION( CONDITION_QUEST_PROGRESS )
	virtual BOOL IsSatisfied();
__END_DECL_CONDITION

// 유저 클래스 설정(의존 트리거)
// [USER CLASS ID] 이면, //0:모두, 1:버서커, 2:드래곤, 3:쉐도우, 4:발키리, 5:엘리멘탈리스트
__BEGIN_DECL_CONDITION( CONDITION_CHECK_CLASS_ID )
	virtual BOOL IsSatisfied();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_NPCKILL_FOR_UNITID )
	virtual VOID onMsg( TRIGGER_MSG * pmsg );
__END_DECL_CONDITION

__BEGIN_DECL_CONDITION( CONDITION_LIVE_PARTY_MEMBER )
virtual BOOL IsSatisfied();
__END_DECL_CONDITION

__BEGIN_DECL_CONDITION( CONDITION_CHECK_OBJECT_HP )
virtual VOID OnInit();
virtual BOOL IsSatisfied();
virtual BOOL NeedUpdate() { return TRUE; }
private:
    DWORD	m_dwStartTick;
__END_DECL_CONDITION

__BEGIN_DECL_CONDITION( CONDITION_DESTROY_OBJECT )
virtual VOID OnInit();
virtual BOOL IsSatisfied();
virtual BOOL NeedUpdate() { return TRUE; }
private:
    DWORD	m_dwStartTick;
__END_DECL_CONDITION

//_NA_0_20100222_UNIT_TRIGGERS_ADD
__BEGIN_DECL_CONDITION( CONDITION_CHECK_OBJECTKIND_HP )
virtual VOID OnInit();
virtual BOOL IsSatisfied();
virtual BOOL NeedUpdate() { return TRUE; }
private:
    DWORD	start_tick_;
__END_DECL_CONDITION

//_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
// 미션내 채집 트리거
__BEGIN_DECL_CONDITION(CONDITION_COMPLETE_COLLECTION)
    virtual void OnInit();
    virtual BOOL IsSatisfied();
    virtual void OnReset();    
    virtual void onMsg(TRIGGER_MSG* trigger_msg);
__END_DECL_CONDITION

// _NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER
// 서버 이벤트 트리거
__BEGIN_DECL_CONDITION(CONDITION_SERVER_EVENT)
	virtual void OnInit();
	virtual BOOL IsSatisfied();
	virtual void OnReset();    
	virtual void onMsg(TRIGGER_MSG* trigger_msg);
__END_DECL_CONDITION

// _NA_000000_20130104_ADD_CONDITION_TRIGGER_CHECK_TEAM
// 팀 체크 트리거
__BEGIN_DECL_CONDITION(CONDITION_CHECK_TEAM)
virtual BOOL IsSatisfied();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION(CONDITION_MONSTER_ENTER_AREA)
    virtual void OnInit();
    virtual void OnReset();    
    virtual BOOL IsSatisfied();
    virtual void onMsg(TRIGGER_MSG* trigger_msg);
    TriggerAreaLocalStorageForNpc local_storage_;
__END_DECL_CONDITION

//

//////////////////////////////////////////////////////////////////////////////////////////
//
// 애매한 사례
/*
	상호배타적임
	1. 클릭한 유저
	2. 영역에 들어온 유저
	3. 영역에 들어오고 미션 포인트 만족하는 유저
	상호연관적임
	4. 특정 오브젝트에 데미지를 입힌 유저
	5. 아이템을 가진 유저

	1. 이벤트 아이템을 가지고 있다. 
	영역에 들어 온 처음 유저가 아이템을 가지고 있지 않고
	다음 유저가 아이템을 가지고 있을 때
	ActionTrigger에서 대상유저는 어떤 유저를 가르키는가?

*/
//////////////////////////////////////////////////////////////////////////////////////////
