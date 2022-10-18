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

#ifdef __NA_000360_20070323_TRIGGER_LIBRARY
	#include "..\\..\\SolarTrigger\\ConditionInfo.h"
	#include "IConditionTriggerEx.h"
#else
	#include "ConditionInfo.h"
	#include "IConditionTrigger.h"
	#include <SolarHashTable.h>
#endif


__BEGIN_DECL_CONDITION( CONDITION_ALWAYS )
	virtual VOID OnInit();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_COMPARE_SWITCH )
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
	} m_PartyMember[MAX_PARTYMEMBER_NUM];
	util::SolarHashTable<PARTY_MEMBER*> m_PartyMemberHash;
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_PASS_TIME )
	virtual VOID OnInit();
	virtual BOOL IsSatisfied();
	virtual BOOL NeedUpdate() { return TRUE; }
private:
	DWORD	m_dwStartTick;
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_ENTER_AREA )
	virtual VOID OnInit();
	virtual BOOL IsSatisfied();
	virtual VOID onMsg( TRIGGER_MSG * pmsg );
	virtual VOID OnReset();
__END_DECL_CONDITION


__BEGIN_DECL_CONDITION( CONDITION_HAVE_ITEM )
	static const DWORD c_dwDelayCheckTime = 100;
	virtual VOID OnInit();
	virtual BOOL IsSatisfied();
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


#ifdef __NA_000328_UNIT_DESTROY_TRIGGER
__BEGIN_DECL_CONDITION( CONDITION_NPCKILL_FOR_UNITID )
	virtual VOID onMsg( TRIGGER_MSG * pmsg );
__END_DECL_CONDITION
#endif
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
