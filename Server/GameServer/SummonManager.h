#ifndef __SUMMON_MANAGER_H__
#define __SUMMON_MANAGER_H__

#pragma once

#include <Singleton.h>
#include <SolarHashTable.h>
#include <MemoryPoolFactory.h>

class SummonedNPCs;
class Character;

class SummonManager : public util::Singleton<SummonManager>
{
public:
	SummonManager();
	~SummonManager();

	VOID							Release();
	VOID							Init( DWORD dwMaxPoolSize );

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    bool                            DeleteSummonedUsingType(Character* pSummoner, eSUMMON_TYPE type);

    SummonedNPCs*					AllocSummonNPCs( Character *pSummoner, BOOL bPlayerSkill, WORD ability_id = 0 );
#else
    SummonedNPCs*					AllocSummonNPCs( Character *pSummoner, BOOL bPlayerSkill );
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
	BOOL							FreeSummonNPCs( SummonedNPCs *pSummonNPCs, BOOL bFollowerDestroy=TRUE );

	VOID							AddSummonedSkill( DWORD dwSummonKey, SLOTCODE SkillCode );
	VOID							DelSummonedSkill( DWORD dwSummonKey, SLOTCODE SkillCode );

	VOID							LeaveSummonNPCs( Character *pLeaveChar );

	RC::eSUMMON_RESULT				SetCommandState( DWORD dwSummonKey, eSUMMON_COMMAND eCommand, DWORD dwObjectKey=0, DWORD dwTargetKey=0 );

public:
	SummonedNPCs*					FindSummonNPCs( DWORD dwSummonKey )
	{
		SUMMON_NPC_LIST_IT itFIND = m_SummonNPCsList.find( dwSummonKey );
		return (itFIND != m_SummonNPCsList.end()) ? itFIND->second : NULL;
	}
private:
	struct LoopNode
	{
		LoopNode() { SKILL_CODE.Set(0); }
		PROPERTY<SLOTCODE>			SKILL_CODE;
		VOID						_AddSummonedSkill( SUMMONED_MAP_PAIR rPair );
		VOID						_DelSummonedSkill( SUMMONED_MAP_PAIR rPair );
		VOID						_LeaveSummonNPCs( SUMMONED_MAP_PAIR rPair );
	};

private:
	// (WAVERIX)(NOTE) 이후 SummonedNPCs* 를 SummonedNPCs로 전환시킬 수 있는지 여부를 확인하자.
	typedef STLX_MAP<DWORD, SummonedNPCs*>			SUMMON_NPC_LIST;
	typedef SUMMON_NPC_LIST::iterator				SUMMON_NPC_LIST_IT;
	SUMMON_NPC_LIST									m_SummonNPCsList;
#	if WAVERIX_CODE_DELETE_RESERVED
	typedef STLX_SET<SummonedNPCs*>					SUMMON_HASH;	// 무결성 검사용, 제거 예정
	SUMMON_HASH										m_SummonHash;
#	endif

	util::CMemoryPoolFactory<SummonedNPCs>*				m_pSummonNPCsPool;
};

#endif // __SUMMON_MANAGER_H__
