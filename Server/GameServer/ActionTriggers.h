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


__BEGIN_DECL_ACTION( ACTION_NPC_RANDOM_AREA_MOVE )
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


__BEGIN_DECL_ACTION( ACTION_OPERATE_SWITCH_EX )
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

__BEGIN_DECL_ACTION( ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR )
	virtual VOID OnAction();
__END_DECL_ACTION

//////////////////////////////////////////////////////////////////////////
//
struct sSingletonValue
{
	sSingletonValue() : m_bInit(FALSE) {}
	inline BOOL		IsInited() { return m_bInit; }
	inline VOID		Inited() { m_bInit = TRUE; }
	BOOL			m_bInit;
};

__BEGIN_DECL_ACTION( ACTION_RANDOM_MAP_MOVE )
			virtual VOID	OnInit();
			virtual VOID	OnAction();
//////////////////////////////////////////////////////////////////////////
public:		DWORD			PrepareNextCode();
private:
			typedef STLX_VECTOR<BYTE>					FIELDCODE_LIST;
			typedef FIELDCODE_LIST::iterator			FIELDCODE_LIST_IT;
			typedef ACTION_RANDOM_MAP_MOVE_INFO::eENUM	ENUM_T;
//
			sSingletonValue		m_INIT;
			FIELDCODE_LIST		m_List;
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_RANDOM_AREA_MOVE )
	virtual VOID			OnInit();
	virtual VOID			OnAction();
//////////////////////////////////////////////////////////////////////////
public:		DWORD			PrepareNextCode();
private:
			typedef STLX_VECTOR<BYTE>					AREA_ID_LIST;
			typedef AREA_ID_LIST::iterator				AREA_ID_LIST_IT;
			typedef ACTION_RANDOM_AREA_MOVE_INFO::eENUM	ENUM_T;
//
			sSingletonValue		m_INIT;
			AREA_ID_LIST		m_List;
__END_DECL_ACTION
//
//////////////////////////////////////////////////////////////////////////

__BEGIN_DECL_ACTION( ACTION_REMOVE_MONSTER )
	virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_ADD_EXP )
	virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_MONSTER_STAT_UP_OR_DOWN )
	virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_RANDUM_TRIGER_ON_OR_OFF )
	virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_PLAY_MUSIC )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_DOWN_OBJETC_HP )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_CHANGE_ATTACK_KIND )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_ACTIVE_STAMP )
virtual VOID OnAction();
__END_DECL_ACTION

// implemented by _NA_0_20100222_UNIT_TRIGGERS_ADD = {

__BEGIN_DECL_ACTION( ACTION_ATTACK_OBJECTKIND )
typedef VOID (ACTION_ATTACK_OBJECTKIND::*ProcessFunction)(const DWORD, const eACTION_ATTACK_TYPE);
ProcessFunction GetProcessFunction(const eOBJECTCODE_TYPE object_type);
VOID ActionDefaultType(const DWORD object_key, const eACTION_ATTACK_TYPE attack_type);
VOID ActionNPCCodeType(const DWORD object_key, const eACTION_ATTACK_TYPE attack_type);
VOID ActionUnitIDType(const DWORD object_key, const eACTION_ATTACK_TYPE attack_type);
VOID ActionMapNpcType(const DWORD object_key, const eACTION_ATTACK_TYPE attack_type);
static VOID StatusProc( StatusManager& status_manager, const eACTION_ATTACK_TYPE attack_type);

virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_ACTIVE_SKILL_OBJECTKIND )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_SHOW_MESSAGEBOX_OBJECTKIND )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_SET_ATTACK_STATE_OBJECTKIND )
static VOID StatusProc(NPC* src_npc, const eACTION_ATTACK_TYPE attack_type, 
                       const eTARGET_SEARCH_TYPE search_type, const DWORD dest_object_key);

virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_DESTROY_OBJECTKIND )
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION( ACTION_CREATE_MONSTER_LINKED_MAPOBJECT )
virtual VOID OnAction();
__END_DECL_ACTION

//} end implement '_NA_0_20100222_UNIT_TRIGGERS_ADD'

__BEGIN_DECL_ACTION(ACTION_CONTROL_TIMER)
virtual VOID OnAction();
__END_DECL_ACTION

// added by _NA_0_20100705_DOMINATION_EVENT
__BEGIN_DECL_ACTION(ACTION_DO_COMMAND)
virtual VOID OnAction();
__END_DECL_ACTION

//_NA_0_20100817_HELLON_LAST_TRIGGER
__BEGIN_DECL_ACTION(ACTION_MOVE_CONTROL)
static VOID StatusProc(NPC* npc, const eACTION_MOVE_TYPE move_type);
virtual VOID OnAction();
__END_DECL_ACTION

__BEGIN_DECL_ACTION(ACTION_ANIMATION_CONTROL)
virtual VOID OnAction();
__END_DECL_ACTION

// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
__BEGIN_DECL_ACTION(ACTION_NPC_APPLY_DAMAGE)
virtual VOID OnAction();
__END_DECL_ACTION

//_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
__BEGIN_DECL_ACTION(ACTION_COMPLETE_COLLECTION)
    virtual VOID OnAction();
__END_DECL_ACTION

//_NA_000000_20130104_ADD_ACTION_TRIGGER_APPLY_HPMPSD
__BEGIN_DECL_ACTION(ACTION_APPLY_HPMPSD)
virtual VOID OnAction();
__END_DECL_ACTION

// _NA_006654_20120422_MISSION_TRIGGER_ADD_AND_MODIFY
__BEGIN_DECL_ACTION(ACTION_CHANGE_OBJECT_HP)
virtual VOID OnAction();
__END_DECL_ACTION

//_NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
__BEGIN_DECL_ACTION(ACTION_CHANGE_RADAR_MAP)
virtual VOID OnAction();
__END_DECL_ACTION

// _NA_006654_20120422_MISSION_TRIGGER_ADD_AND_MODIFY
__BEGIN_DECL_ACTION(ACTION_ATTACH_STATE_RANGE)
virtual VOID OnAction();
__END_DECL_ACTION

// _NA_000000_20130925_ADD_ACTION_TRIGGER_CHANGE_PLAYER_HP
__BEGIN_DECL_ACTION(ACTION_CHANGE_PLAYER_HP)
virtual VOID OnAction();
__END_DECL_ACTION