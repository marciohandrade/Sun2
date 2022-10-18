#include "StdAfx.h"
#include "SummonManager.h"
#include "SummonedNPCs.h"
#include "SkillInfoParser.h"
#include "Summoned.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SummonManager::SummonManager()
{
	m_pSummonNPCsPool = new util::CMemoryPoolFactory<SummonedNPCs>;
}

SummonManager::~SummonManager()
{
	SAFE_DELETE( m_pSummonNPCsPool );
}

VOID SummonManager::Release()
{
	m_SummonNPCsList.clear();
	if( m_pSummonNPCsPool )
	{
		m_pSummonNPCsPool->Release();
	}
}

VOID SummonManager::Init( DWORD dwMaxPoolSize )
{
	//	(WAVERIX)(REMARK)(수치 조정)
	//		- SummonedNPCs는 그리 부담가는 자료 구조는 아니다 <-( 물론 다른 것에 비해서... )
	//		- SUMMON_NPC_LIST의 형식을 풀로부터 받을 필요가 없을 듯 한데, 외부 모듈에서 포인터를
	//		사용하고 있는 관계로 정확히 확인후 수정하자.
	m_pSummonNPCsPool->Initialize( 100, 100, "SummonedNPCs", AssertionLogger4Pool_Server );
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
SummonedNPCs* SummonManager::AllocSummonNPCs( Character* pSummoner, BOOL bPlayerSkill, WORD ability_id /*= 0*/ )
#else
SummonedNPCs* SummonManager::AllocSummonNPCs( Character* pSummoner, BOOL bPlayerSkill )
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
{
	SummonedNPCs* pSummonedNPCs = FindSummonNPCs( pSummoner->GetObjectKey() );

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    if( pSummonedNPCs )
    {
        // 플레이어 소환의 경우
        if( bPlayerSkill )
        {
            if (ability_id == eABILITY_SUMMON) {
                // 일반소환일 경우 일반소환된 소환수만 지운다
                DeleteSummonedUsingType(pSummoner, eSUMMON_NORMAL);
            }
            else if (ability_id == eABILITY_SUMMON_IMMOVABLE) {
                // 고정형소환수일경우 고정형 소환수만 지운다
                DeleteSummonedUsingType(pSummoner, eSUMMON_IMMOVABLE);
            }
            else 
            {
                FreeSummonNPCs(pSummonedNPCs);
                // 해당하는 그룹이 없으면 새로생성
                pSummonedNPCs = (SummonedNPCs*)m_pSummonNPCsPool->Alloc();
            }
        }
        else
        {
            // NPC의 경우 이전 소환정보를 그대로 이용한다.
            return pSummonedNPCs;
        }
    }
    else {
        pSummonedNPCs = (SummonedNPCs*)m_pSummonNPCsPool->Alloc();
    }
#else
    if( pSummonedNPCs )
    {
        // 플레이어 소환의 경우
        if( bPlayerSkill )
        {
            // 이미 소환을 했으면 이전 소환체를 삭제한다.
            FreeSummonNPCs( pSummonedNPCs );
        }
        else
        {
            // NPC의 경우 이전 소환정보를 그대로 이용한다.
            return pSummonedNPCs;
        }
    }
    // 해당하는 그룹이 없으면 새로생성
    pSummonedNPCs = (SummonedNPCs*)m_pSummonNPCsPool->Alloc();
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

	if( !pSummonedNPCs )
	{
		SUNLOG( eCRITICAL_LOG,  "[SummonManager::AllocSummonNPCs] Can't Allocate SummonedNPCs " );
		return NULL;
	}

	// SummonedNPCs 초기화
	if(pSummoner->IsEqualObjectKind(PLAYER_OBJECT))
	{
		pSummonedNPCs->Init( pSummoner, eSUMMON_COMMAND_DELEGATE_ATTACK_IGNORE_PLAYER );
	}
	else
	{
		pSummonedNPCs->Init( pSummoner, eSUMMON_COMMAND_DELEGATE_ATTACK );
	}

	// SummonManager에 등록
	m_SummonNPCsList.insert( std::make_pair( pSummoner->GetObjectKey(), pSummonedNPCs ) );

#ifdef WAVERIX_CODE_DELETE_RESERVED
	m_SummonHash.insert( pSummonedNPCs );
#endif

	return pSummonedNPCs;
}

BOOL SummonManager::FreeSummonNPCs( SummonedNPCs *pSummonNPCs, BOOL bFollowerDestroy )
{
	if( NULL == FindSummonNPCs( pSummonNPCs->GetSummonerKey() ) )
	{
		SUNLOG( eCRITICAL_LOG, "[SummonManager::FreeSummonNPCs] Can't Remove SummonNPCs  " );
		return FALSE;
	}

	m_SummonNPCsList.erase( pSummonNPCs->GetSummonerKey() );

#ifdef WAVERIX_CODE_DELETE_RESERVED
	SUMMON_HASH::iterator it = m_SummonHash.find(pSummonNPCs);
	if( it == m_SummonHash.end() )
	{
		SUNLOG( eCRITICAL_LOG, "[SusunLove] FreeSummonNPCs Free Same Memory Pool! SummonerKey[%d]", pSummonNPCs->GetSummonerKey() );
		return FALSE;
	}
	else
	{
		m_SummonHash.erase(it);
	}
#endif

	pSummonNPCs->Release( bFollowerDestroy );

	m_pSummonNPCsPool->Free( pSummonNPCs );

	return TRUE;
}


VOID	SummonManager::LoopNode::_AddSummonedSkill( SUMMONED_MAP_PAIR rPair )
{
	NPC* pNPC = rPair.second;
	if(pNPC->IsEqualObjectKind(SUMMON_OBJECT) == 0)
		return;

	Summoned* pSummoned = (Summoned*)pNPC;
	pSummoned->AddSkill( SKILL_CODE.Get() );
}



VOID	SummonManager::AddSummonedSkill( DWORD dwSummonKey, SLOTCODE scSkillCode )
{
	SummonedNPCs* pSummonedNPCs = FindSummonNPCs( dwSummonKey );
	if( !pSummonedNPCs )
		return;

	SkillScriptInfo* pBaseCurSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( scSkillCode );
	if( pBaseCurSkillInfo->m_bySkillUserType != eSKILL_USER_SUMMONED )
		return;

	LoopNode loopNode;
	loopNode.SKILL_CODE.Set( scSkillCode );
	pSummonedNPCs->ForeachSummonedList( &SummonManager::LoopNode::_AddSummonedSkill, &loopNode );
}


VOID	SummonManager::LoopNode::_DelSummonedSkill( SUMMONED_MAP_PAIR rPair )
{
	NPC* pNPC = rPair.second;
	if(pNPC->IsEqualObjectKind(SUMMON_OBJECT) == 0)
		return;

	Summoned *pSummoned = (Summoned*)pNPC;
	pSummoned->DelSkill( SKILL_CODE.Get() );
}



VOID	SummonManager::DelSummonedSkill( DWORD dwSummonKey, SLOTCODE scSkillCode )
{
	SummonedNPCs *pSummonedNPCs = FindSummonNPCs( dwSummonKey );
	if( !pSummonedNPCs )
		return;

	SkillScriptInfo *pBaseCurSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( scSkillCode );
	if( pBaseCurSkillInfo->m_bySkillUserType != eSKILL_USER_SUMMONED )
		return;

	LoopNode loopNode;
	loopNode.SKILL_CODE.Set( scSkillCode );
	pSummonedNPCs->ForeachSummonedList( &SummonManager::LoopNode::_DelSummonedSkill, &loopNode );
}


// 현재는 소환체가 1마리라서 오브젝트키로 구분이 필요없지만
// 나중을 위해 클라로부터 명령을 내릴 소환체 오브젝트키를 받아야 한다!

RC::eSUMMON_RESULT SummonManager::SetCommandState( DWORD dwSummonKey, eSUMMON_COMMAND eCommand, DWORD dwObjectKey, DWORD dwTargetKey )
{
	SummonedNPCs *pSummonedNPCs = FindSummonNPCs( dwSummonKey );
	if( !pSummonedNPCs )
		return RC::RC_SUMMON_SUMMONED_NPC_NOTEXIST;

   switch(eCommand)
   {
   case eSUMMON_COMMAND_DESTROY:
       {
           //!~ _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
           if (dwTargetKey)
               pSummonedNPCs->Destroy( dwTargetKey );
           else
               pSummonedNPCs->Destroy( dwObjectKey );
           //~! _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
       } break;
   default:     
       return pSummonedNPCs->SetCommandState( eCommand, dwObjectKey, dwTargetKey ); // 상태를 바꾸어준다.
   }

	return RC::RC_SUMMON_SUCCESS;
}


VOID	SummonManager::LoopNode::_LeaveSummonNPCs( SUMMONED_MAP_PAIR rPair )
{
	NPC* pNPC = rPair.second;
	pNPC->SetSummonerKey( 0 );
}


VOID	SummonManager::LeaveSummonNPCs( Character *pLeaveChar )
{
#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
    SummonedNPCs *pSummonedNPCs = FindSummonNPCs( pLeaveChar->GetObjectKey() );
    if (pSummonedNPCs)
    {
	    if(pLeaveChar->IsEqualObjectKind(PLAYER_OBJECT))
	    {
            // 플레이어가 떠날경우 소환정보를 무조건 해제한다.
		    FreeSummonNPCs( pSummonedNPCs );
	    }
        else
	    {
            // 소환을 시킨 주체인 경우 소환NPC들의 공유소환키(m_dwSummonerKey)를 NULL로 초기화한다.
            // NPC가 소환한 부하(소환체)들은 삭제하지 않아야한다.
		    LoopNode loopNode;
		    pSummonedNPCs->ForeachSummonedList( &SummonManager::LoopNode::_LeaveSummonNPCs, &loopNode );
		    FreeSummonNPCs( pSummonedNPCs, FALSE );
	    }
    }
    else
    {
        // 소환을 당한 NPC인 경우 소환정보에서 제거한다.
        pSummonedNPCs = FindSummonNPCs( pLeaveChar->GetSummonerKey() );
        if (pSummonedNPCs)
        {
            DWORD dwObjectKey = pLeaveChar->GetObjectKey();
            NPC* pLeaveNPC = static_cast<NPC*>(pLeaveChar);

            pSummonedNPCs->DecreaseSummoned( pLeaveNPC->GetBaseInfo()->m_MonsterCode, dwObjectKey );

            // 소환리스트의 멤버수가 0이면 풀에 반환한다.
            if (pSummonedNPCs->GetNumberOfNPCs() == 0) {
                FreeSummonNPCs( pSummonedNPCs );
            }
        }

        pLeaveChar->SetSummonerKey(0);
    }
#else
    SummonedNPCs *pSummonedNPCs = FindSummonNPCs( pLeaveChar->GetObjectKey() );
    if( !pSummonedNPCs )
        return;

    // 플레이어가 떠날경우 소환정보를 무조건 해제한다.
    if(pLeaveChar->IsEqualObjectKind(PLAYER_OBJECT))
    {
        FreeSummonNPCs( pSummonedNPCs );
    }
    else
    {
        DWORD dwObjectKey = pLeaveChar->GetObjectKey();
        NPC* pLeaveNPC = (NPC*)pLeaveChar;

        // 소환을 시킨 주체인 경우 소환NPC들의 공유소환키(m_dwSummonerKey)를 NULL로 초기화한다.
        // NPC가 소환한 부하(소환체)들은 삭제하지 않아야한다.
        if( pSummonedNPCs->IsSummoner( dwObjectKey ) )
        {
            LoopNode loopNode;
            pSummonedNPCs->ForeachSummonedList( &SummonManager::LoopNode::_LeaveSummonNPCs, &loopNode );
            FreeSummonNPCs( pSummonedNPCs, FALSE );
        }
        // 소환을 당한 NPC인 경우 소환정보에서 제거한다.
        else
        {
            pSummonedNPCs->DecreaseSummoned( pLeaveNPC->GetBaseInfo()->m_MonsterCode, dwObjectKey );

            // 소환리스트의 멤버수가 0이면 풀에 반환한다.
            if( !pSummonedNPCs->GetNumberOfNPCs() )
            {
                FreeSummonNPCs( pSummonedNPCs );
            }
        }
    }

    pLeaveChar->SetSummonerKey(0);
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
bool SummonManager::DeleteSummonedUsingType( Character* pSummoner, eSUMMON_TYPE type )
{
    SummonedNPCs* pSummonedNPCs = FindSummonNPCs( pSummoner->GetObjectKey() );

    if (pSummonedNPCs == NULL) {
        return false;
    }

    Summoned* summoned = pSummonedNPCs->FindSummonedUsingType(type);
    if (summoned == NULL) {
        //어찌되었든 없는상태로 만들어야 하니..
        return true;
    }

    pSummonedNPCs->Destroy(summoned->GetObjectKey());
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL


