#pragma once
//=============================================================================================================================
/// ����� Ʈ���ŵ��� ���� Ŭ����
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
// NPC�� Ư�� ������ ���� ��
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

// Quest �Ϸ� üũ Ʈ����, �ִ°����� Ȯ���� ��
__BEGIN_DECL_CONDITION( CONDITION_QUEST_PROGRESS )
	virtual BOOL IsSatisfied();
__END_DECL_CONDITION

// ���� Ŭ���� ����(���� Ʈ����)
// [USER CLASS ID] �̸�, //0:���, 1:����Ŀ, 2:�巡��, 3:������, 4:��Ű��, 5:������Ż����Ʈ
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
// �̼ǳ� ä�� Ʈ����
__BEGIN_DECL_CONDITION(CONDITION_COMPLETE_COLLECTION)
    virtual void OnInit();
    virtual BOOL IsSatisfied();
    virtual void OnReset();    
    virtual void onMsg(TRIGGER_MSG* trigger_msg);
__END_DECL_CONDITION

// _NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER
// ���� �̺�Ʈ Ʈ����
__BEGIN_DECL_CONDITION(CONDITION_SERVER_EVENT)
	virtual void OnInit();
	virtual BOOL IsSatisfied();
	virtual void OnReset();    
	virtual void onMsg(TRIGGER_MSG* trigger_msg);
__END_DECL_CONDITION

// _NA_000000_20130104_ADD_CONDITION_TRIGGER_CHECK_TEAM
// �� üũ Ʈ����
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
// �ָ��� ���
/*
	��ȣ��Ÿ����
	1. Ŭ���� ����
	2. ������ ���� ����
	3. ������ ������ �̼� ����Ʈ �����ϴ� ����
	��ȣ��������
	4. Ư�� ������Ʈ�� �������� ���� ����
	5. �������� ���� ����

	1. �̺�Ʈ �������� ������ �ִ�. 
	������ ��� �� ó�� ������ �������� ������ ���� �ʰ�
	���� ������ �������� ������ ���� ��
	ActionTrigger���� ��������� � ������ ����Ű�°�?

*/
//////////////////////////////////////////////////////////////////////////////////////////
