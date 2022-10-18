//=============================================================================================================================
/// UnitRegen class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 7. 28
	@note
		- UnitRegenInfo가 실체화된 객체, NPC와 1:1대응되는 객체
	@history
		- 2006.10.10 : 리젠시간을 리젠정보로 공유하지 않고 리젠객체마다 가질 수 있게 RegenTime 변수 추가
*/
//=============================================================================================================================

#pragma once

#include "UnitRegenInfo.h"
#include <MemoryPoolFactory.h>


class UnitRegen
{
	__DECL_CUSTOMPOOL_PTR( UnitRegen )
public:
	UnitRegen(void):m_pUnitRegenInfo(NULL),m_RegenTickTime(0),m_RegenTime(0){}
	~UnitRegen(void){}

	//__inline void	Clear() { SetUnitRegenInfo(NULL); SetRegenTickTime(0); SetRegenTime(0); }
	__inline void	Init( UnitRegenInfo * p ) { SetUnitRegenInfo(p); Restart(); SetRegenTime(GetUnitRegenInfo()->GetRegenTime()); }
	__inline void	Restart() { SetRegenTickTime(GetTickCount()); }
	__inline BOOL	IsRegenTime()
	{
		const DWORD64 regenTick = m_RegenTickTime;
		return ( regenTick )
			?	(regenTick + m_RegenTime) <= (DWORD64)GetTickCount()
			:	FALSE
			;
	}
private:
	__PTR_PROPERTY( UnitRegenInfo, UnitRegenInfo );
	__PROPERTY( DWORD, RegenTickTime );
	__PROPERTY( DWORD, RegenTime );						//< UnitRegenInfo의 RegenTime은 공유되는 정보임
};
