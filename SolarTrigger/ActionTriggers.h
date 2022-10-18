#pragma once
//=============================================================================================================================
/// 액션 트리거들의 구현 클래스
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
#ifdef __NA_000360_20070323_TRIGGER_LIBRARY
	#include "..\\..\\SolarTrigger\\ActionInfo.h"
	#include "IActionTriggerEx.h"
#else
	#include "ActionInfo.h"
	#include "IActionTrigger.h"
#endif //__NA_000360_20070323_TRIGGER_LIBRARY

#include <Timer.h>
#include <ReliableRatioRandomizer.h>


__BEGIN_DECL_ACTION( ACTION_CHANGE_OBJECTANI )
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_CHANGE_PATHTILE )
	virtual VOID OnInit();
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_CHANGE_OBJECTSTATE )
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_PLAY_EVENT )
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_SET_ISATTACK )
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_PORTAL_PLAYER )
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_SET_OBJECTTHRUST )
	virtual VOID OnAction();
__END_DECL_ACTION



__BEGIN_DECL_ACTION( ACTION_APPLY_DAMAGE )
	virtual VOID OnAction();
__END_DECL_ACTION



__BEGIN_DECL_ACTION( ACTION_REFLECT_DAMAGE )
	virtual VOID OnAction();
__END_DECL_ACTION



__BEGIN_DECL_ACTION( ACTION_CREATE_MONSTER )
	virtual VOID OnAction();
	virtual BOOL OnActionUpdate();
private:
	INT m_iRegenCount;
	INT m_iMaxCreateCount;
	util::Timer m_RegenTimer;
__END_DECL_ACTION



__BEGIN_DECL_ACTION( ACTION_DISPLAY_MSGBOX )
	virtual VOID OnAction();
__END_DECL_ACTION



__BEGIN_DECL_ACTION( ACTION_REMOVE_ITEM )
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_REWARD_PLAYER )
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_CLEAR_MISSION )
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_PORTAL_RANDOM )
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_ACTIVATE_TRIGGER )
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_AREA_DAMAGE )
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_OPERATE_SWITCH )
	virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_ROAD_CONNECT )
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_CREATE_DIR_MONSTER )
	virtual VOID OnAction();
	virtual BOOL OnActionUpdate();
private:
	INT m_iRegenCount;
	INT m_iMaxCreateCount;
	util::Timer m_RegenTimer;
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_CHANGE_LIGHT )
	virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_CHANGE_OBJECT_TYPE )
	virtual VOID OnAction();
__END_DECL_ACTION


__BEGIN_DECL_ACTION( ACTION_QUEST_SATISFACTION )
	virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_QUEST_GRANT )
	virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_QUEST_SETUP )
	virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_USE_CATEGORY )
	virtual VOID OnAction();
__END_DECL_ACTION

// 기존에 존재하는 액션 트리거
//__BEGIN_DECL_ACTION( ACTION_CREATE_MONSTER )
//__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_CREATE_MONSTER_GROUP )
	virtual VOID OnAction();
	virtual BOOL OnActionUpdate();
private:
	INT m_iRegenCount;
	INT m_iMaxCreateCount;
	DWORD m_dwTimeout;
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_SHOW_OR_HIDE_UNIT )
	virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_SHOW_GUIDE_MSGBOX )
	virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_SHOW_OR_HIDE_MSGBOX )
	virtual VOID OnAction();
__END_DECL_ACTION

// 몬스터 랜덤 스폰
#ifdef __NA_000277_20070306_MONSTER_RANDOM_SPAWN_TRIGGER
__BEGIN_DECL_ACTION( ACTION_RANDOM_MONSTER_SPAWN )
	virtual VOID OnAction();
	virtual BOOL OnActionUpdate();
			INT	 GetMonsterCount();
			INT	 GetRandomMonster();	

	enum { MAX_RANDOM_STEP_NUM = 50+3, MAX_MONSTER_RATIO = 100 };
	typedef ReliableRatioRandomizer<MAX_RANDOM_STEP_NUM, DWORD, DWORD > RANDOMIZER;

private:
	INT				m_nCurRegenCount;
	util::Timer		m_RegenTimer;
	RANDOMIZER		m_MonsterRandomizer;
__END_DECL_ACTION
#endif

//#ifdef __NA_000104_20070123_TOP_OF_TRIAL
__BEGIN_DECL_ACTION( ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR )
	virtual VOID OnAction();
__END_DECL_ACTION
//#endif //
