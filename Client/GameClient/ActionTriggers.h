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
#include "..\\..\\SolarTrigger\\ActionInfo.h"
#include "IActionTriggerEx.h"


__BEGIN_DECL_ACTION( ACTION_CHANGE_OBJECTANI )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_CHANGE_PATHTILE )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_CHANGE_OBJECTSTATE )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_PLAY_EVENT )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_SET_ISATTACK )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_PORTAL_PLAYER )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_SET_OBJECTTHRUST )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_APPLY_DAMAGE )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_REFLECT_DAMAGE )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_CREATE_MONSTER )
	virtual VOID OnInit();
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_DISPLAY_MSGBOX )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_REMOVE_ITEM )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_REWARD_PLAYER )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_CLEAR_MISSION )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_PORTAL_RANDOM )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_ACTIVATE_TRIGGER )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_AREA_DAMAGE )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_OPERATE_SWITCH )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_ROAD_CONNECT )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_CREATE_DIR_MONSTER )
	virtual VOID OnInit();
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_CHANGE_LIGHT )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_CHANGE_OBJECT_TYPE )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////


__BEGIN_DECL_ACTION( ACTION_RANDOM_MAP_MOVE )
virtual VOID OnAction();
__END_DECL_ACTION


//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_QUEST_SATISFACTION )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_QUEST_GRANT )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_QUEST_SETUP )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_USE_CATEGORY )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

//// 기존에 존재하는 액션 트리거
////__BEGIN_DECL_ACTION( ACTION_CREATE_MONSTER )
////__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_CREATE_MONSTER_GROUP )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_SHOW_OR_HIDE_UNIT )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_SHOW_GUIDE_MSGBOX )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_SHOW_OR_HIDE_MSGBOX )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_RANDOM_AREA_MOVE )
virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////

// 몬스터 랜덤 스폰
__BEGIN_DECL_ACTION( ACTION_RANDOM_MONSTER_SPAWN )
	virtual VOID OnInit();
	virtual VOID OnAction();
__END_DECL_ACTION


//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR )
	virtual VOID OnAction();
__END_DECL_ACTION
//////////////////////////////////////////////////////////////////////////
__BEGIN_DECL_ACTION( ACTION_REMOVE_MONSTER )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_ADD_EXP )
virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////
__BEGIN_DECL_ACTION( ACTION_MONSTER_STAT_UP_OR_DOWN )
	virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_RANDUM_TRIGER_ON_OR_OFF )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////
 
__BEGIN_DECL_ACTION( ACTION_PLAY_MUSIC )
	virtual VOID OnAction();
__END_DECL_ACTION
 
//////////////////////////////////////////////////////////////////////////
__BEGIN_DECL_ACTION( ACTION_DOWN_OBJETC_HP )
    virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_CHANGE_ATTACK_KIND )
    virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_ACTIVE_STAMP )
    virtual VOID OnAction();
__END_DECL_ACTION
//////////////////////////////////////////////////////////////////////////
__BEGIN_DECL_ACTION( ACTION_ATTACK_OBJECTKIND )
    virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_ACTIVE_SKILL_OBJECTKIND )
    virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_SHOW_MESSAGEBOX_OBJECTKIND )
    virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_SET_ATTACK_STATE_OBJECTKIND )
    virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_DESTROY_OBJECTKIND )
    virtual VOID OnAction();
__END_DECL_ACTION

//  [5/5/2010 iforall]
__BEGIN_DECL_ACTION( ACTION_CREATE_MONSTER_LINKED_MAPOBJECT )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION(ACTION_CONTROL_TIMER)
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_DO_COMMAND )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_MOVE_CONTROL )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_ANIMATION_CONTROL )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_NPC_APPLY_DAMAGE )
virtual VOID OnAction();
__END_DECL_ACTION

//_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
__BEGIN_DECL_ACTION( ACTION_COMPLETE_COLLECTION )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_APPLY_HPMPSD )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_CHANGE_OBJECT_HP )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_CHANGE_RADAR_MAP )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_ATTACH_STATE_RANGE )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_CHANGE_PLAYER_HP )
virtual VOID OnAction();
__END_DECL_ACTION

