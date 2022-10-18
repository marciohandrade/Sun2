#ifndef __GAMESERVER_OBJECT_POLICY_H
#define __GAMESERVER_OBJECT_POLICY_H
#pragma once

struct sPOLICY_CHARACTER
{
	enum eFLAG {
		PCC_DISABLE_ALL				= 0,
		PCC_IN_DEF_FIELD			= 1<< 0,
		PCC_IN_SSQ_FIELD			= 1<< 1,
		PCC_IN_DEF_FIELD_PLAYER		= 1<< 2,	// �⺻ �ʵ� & �÷��̾��� ��� �����ȴ�.
		PCC_IN_SSQ_FIELD_PLAYER		= 1<< 3,	// SSQ �ʵ� & �÷��̾��� ��� �����ȴ�.
		PCC_STATUS_BRD_TO_ALL		= 1<< 4,	// ���� ���� ���� BRD �ʿ��Ѱ�? �̼����� �ڽſ��Ը� ����ǵ��� �Ѵ�.
		PCC_BATTLE_RECORD_PRIORITY	= 1<< 5,	// BattleRecord ĳ���� Ÿ�Ժ� ����ġ ���� ����
        PCC_CONTINUING_NPC_UPDATE   = 1<< 6,    // NPC���� ���� ������ �ʿ��� ��ü���� update����
        PCC_EXIST_AREA_EVENT_NPC    = 1<< 7,    // whether this object has a trigger area event for NPC
        PCC_PROTECTED_NPC           = 1<< 8,    // to support NpcExtraInfo.NpcType exception
		//PCC_DEF_FIELD_CTRL			=
		//	( PCC_IN_DEF_FIELD
		//	| PCC_STATUS_BRD_TO_ALL
		//	| PCC_BATTLE_RECORD_PRIORITY
		//	),	// Ÿ���� ����� �ƴ� �Ϲ� ó���� ��� + is Player
		//PCC_SSQ_FIELD_CTRL			=
		//	( PCC_IN_SSQ_FIELD
		//	),	// Ÿ���� ����� ��� + is Player
	};

public:
	const	eFLAG&		FIELD;
private:	eFLAG		m_Field;

public:
	sPOLICY_CHARACTER() : FIELD(m_Field), m_Field( PCC_DISABLE_ALL ) {}
	inline	VOID			Clear() { m_Field = PCC_DISABLE_ALL; }
	inline	VOID			SetField( const DWORD flags ) { m_Field = (eFLAG)flags; }
	inline	eFLAG			GetField() { return m_Field; }

	//	<Other>
	//	if( Policyment.FIELD&(flag) )
	inline	BOOLEAN			IsChecked( const eFLAG flag ) { return !!(m_Field&flag); }
    //
    __DISABLE_COPY(sPOLICY_CHARACTER);
};


//__NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__
#include "./Skill_Info.h"

#endif //__GAMESERVER_OBJECT_POLICY_H
