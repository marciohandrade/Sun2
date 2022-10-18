#ifndef __GAMESERVER_OBJECT_POLICY_H
#define __GAMESERVER_OBJECT_POLICY_H
#pragma once

struct sPOLICY_CHARACTER
{
	enum eFLAG {
		PCC_DISABLE_ALL				= 0,
		PCC_IN_DEF_FIELD			= 1<< 0,
		PCC_IN_SSQ_FIELD			= 1<< 1,
		PCC_IN_DEF_FIELD_PLAYER		= 1<< 2,	// 기본 필드 & 플레이어일 경우 설정된다.
		PCC_IN_SSQ_FIELD_PLAYER		= 1<< 3,	// SSQ 필드 & 플레이어일 경우 설정된다.
		PCC_STATUS_BRD_TO_ALL		= 1<< 4,	// 상태 정보 전송 BRD 필요한가? 미설정시 자신에게만 적용되도록 한다.
		PCC_BATTLE_RECORD_PRIORITY	= 1<< 5,	// BattleRecord 캐릭터 타입별 가중치 적용 여부
        PCC_CONTINUING_NPC_UPDATE   = 1<< 6,    // NPC기준 상태 갱신이 필요한 객체들의 update여부
        PCC_EXIST_AREA_EVENT_NPC    = 1<< 7,    // whether this object has a trigger area event for NPC
        PCC_PROTECTED_NPC           = 1<< 8,    // to support NpcExtraInfo.NpcType exception
		//PCC_DEF_FIELD_CTRL			=
		//	( PCC_IN_DEF_FIELD
		//	| PCC_STATUS_BRD_TO_ALL
		//	| PCC_BATTLE_RECORD_PRIORITY
		//	),	// 타락한 사원이 아닌 일반 처리일 경우 + is Player
		//PCC_SSQ_FIELD_CTRL			=
		//	( PCC_IN_SSQ_FIELD
		//	),	// 타락한 사원인 경우 + is Player
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
