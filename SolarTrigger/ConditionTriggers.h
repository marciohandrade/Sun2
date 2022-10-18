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

// Quest �Ϸ� üũ Ʈ����, �ִ°����� Ȯ���� ��
__BEGIN_DECL_CONDITION( CONDITION_QUEST_PROGRESS )
	virtual BOOL IsSatisfied();
__END_DECL_CONDITION

// ���� Ŭ���� ����(���� Ʈ����)
// [USER CLASS ID] �̸�, //0:���, 1:����Ŀ, 2:�巡��, 3:������, 4:��Ű��, 5:������Ż����Ʈ
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
